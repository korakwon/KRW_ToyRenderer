#include "ImGuiManager.h"

#include "Core/Engine.h"
#include "Platform/BaseWindow.h"
#include "Application/Application.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanShader.h"
#include "RHI/Vulkan/VulkanMaterial.h"
#include "RHI/Vulkan/VulkanPipelineInfo.h"
#include "RHI/Vulkan/VulkanQueue.h"
#include "RHI/Vulkan/Texture/VulkanTexture.h"
#include "RHI/Vulkan/Buffer/VulkanIndexBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanUniformBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanStaticMesh.h"
#include "Util/Path.h"
#include "Util/VertexIndexLayout.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using Core::ImGuiManager;

IMPLEMENT_SINGLETON(ImGuiManager)

ImGuiManager::ImGuiManager()
{
}
ImGuiManager::~ImGuiManager()
{
}

void ImGuiManager::Initialize()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    SetImGuiStyle(EImGuiStyleType::Vulkan);

    ImGui_ImplWin32_Init(Platform::G_HWND);

    UINT32 Width = 0, Height = 0;
    APPLICATION->GetWindow()->GetExtend(Width, Height);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(Width), static_cast<float>(Height));
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    CreateMaterial();
    CreateTexture();
    CreateUniformBuffer();
    CreateDrawMeshInfos();
}
void ImGuiManager::Update(const float& DeltaTime)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();
    UINT32 BackBufferIndex = RHI->GetVulkanSwapChain()->GetCurrentBackBufferFrameIndex();

    ImDrawData* imDrawData = ImGui::GetDrawData();

    // Update Mesh
    {
        INT32 TotalVertexCount = imDrawData->TotalVtxCount;
        INT32 TotalIndexCount = imDrawData->TotalIdxCount;

        if ((TotalVertexCount == 0) || (TotalIndexCount == 0))
        {
            return;
        }

        if (imDrawData->CmdListsCount > 0)
        {
            // Create Mesh
            if (DrawMeshInfos[BackBufferIndex].IsNeedUpdate(TotalVertexCount, TotalIndexCount) == true)
            {
                DrawMeshInfos[BackBufferIndex].CreateStaticMesh(
                    TotalVertexCount, sizeof(ImDrawVert),
                    TotalIndexCount, sizeof(ImDrawIdx));
            }

            // Update Mesh
            UINT32 VertexOffset = 0;
            UINT32 IndexOffset = 0;
            for (int i = 0; i < imDrawData->CmdListsCount; i++)
            {
                const ImDrawList* cmd_list = imDrawData->CmdLists[i];

                DrawMeshInfos[BackBufferIndex].UpdateStaticMesh(
                    cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size, sizeof(ImDrawVert), VertexOffset * sizeof(ImDrawVert),
                    cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size, sizeof(ImDrawIdx), IndexOffset * sizeof(ImDrawIdx));

                VertexOffset += cmd_list->VtxBuffer.Size;
                IndexOffset += cmd_list->IdxBuffer.Size;
            }
        }
    }

    // Update UniformBuffer
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 displayPos = imDrawData->DisplayPos;

        static ImGuiUniformBuffer UBO;
        {
            ZeroMemory(&UBO, sizeof(ImGuiUniformBuffer));
            UBO.Scale.x = 2.0f / io.DisplaySize.x;
            UBO.Scale.y = 2.0f / io.DisplaySize.y;
            UBO.Translate = Vector2(-1.0f);
        }
        UniformBuffer->UpdateBuffer(&UBO, sizeof(ImGuiUniformBuffer));
    }
}
void ImGuiManager::Render()
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();
    UINT32 BackBufferIndex = RHI->GetVulkanSwapChain()->GetCurrentBackBufferFrameIndex();

    std::shared_ptr<RHI::VulkanCommandBuffer> CommandBuffer = RHI->GetActivateCommandBuffer();

    ImDrawData* imDrawData = ImGui::GetDrawData();
    UINT32 vertexOffset = 0;
    UINT32 indexOffset = 0;

    if (imDrawData->CmdListsCount > 0)
    {
        if ((Material != nullptr) && (DrawMeshInfos[BackBufferIndex].StaticMesh != nullptr))
        {
            Material->BindFrame(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
            {
                DrawMeshInfos[BackBufferIndex].StaticMesh->BindFrame(CommandBuffer);

                for (INT32 i = 0; i < imDrawData->CmdListsCount; i++)
                {
                    const ImDrawList* cmd_list = imDrawData->CmdLists[i];

                    for (INT32 j = 0; j < cmd_list->CmdBuffer.Size; j++)
                    {
                        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];

                        VkRect2D ScissorInfo;
                        {
                            ScissorInfo.offset.x = std::max(static_cast<INT32>(pcmd->ClipRect.x), 0);
                            ScissorInfo.offset.y = std::max(static_cast<INT32>(pcmd->ClipRect.y), 0);
                            ScissorInfo.extent.width = static_cast<UINT32>(pcmd->ClipRect.z - pcmd->ClipRect.x);
                            ScissorInfo.extent.height = static_cast<UINT32>(pcmd->ClipRect.w - pcmd->ClipRect.y);
                        }

                        Material->UpdateScissorInfo(ScissorInfo);
                        Material->DrawFrame(CommandBuffer);
                        DrawMeshInfos[BackBufferIndex].StaticMesh->DrawFrame(
                            CommandBuffer,
                            0, vertexOffset,
                            pcmd->ElemCount, indexOffset);

                        indexOffset += pcmd->ElemCount;
                    }
                    vertexOffset += cmd_list->VtxBuffer.Size;
                }
            }
            Material->EndFrame();
        }
    }
}
void ImGuiManager::Destroy()
{
    DestroyDrawMeshInfos();
    DestroyUniformBuffer();
    DestroyTexture();
    DestroyMaterial();

    static bool IsImGuiDestroyed = false;
    if (IsImGuiDestroyed == false)
    {
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        IsImGuiDestroyed = true;
    }

    DESTROY_SINGLETON()
}

void ImGuiManager::StartFrame()
{
    //const Vector2& mousePos = InputManager::GetMousePosition();
    //ImGuiIO& io = ImGui::GetIO();
    //io.MouseWheel  += InputManager::GetMouseDelta();
    //io.MousePos     = ImVec2(mousePos.x * io.DisplayFramebufferScale.x, mousePos.y * io.DisplayFramebufferScale.y);
    //io.MouseDown[0] = InputManager::IsMouseDown(MouseType::MOUSE_BUTTON_LEFT);
    //io.MouseDown[1] = InputManager::IsMouseDown(MouseType::MOUSE_BUTTON_RIGHT);
    //io.MouseDown[2] = InputManager::IsMouseDown(MouseType::MOUSE_BUTTON_MIDDLE);
    //
    //io.KeyCtrl  = InputManager::IsKeyDown(KeyboardType::KEY_LEFT_CONTROL) || InputManager::IsKeyDown(KeyboardType::KEY_RIGHT_CONTROL);
    //io.KeyShift = InputManager::IsKeyDown(KeyboardType::KEY_LEFT_SHIFT) || InputManager::IsKeyDown(KeyboardType::KEY_RIGHT_SHIFT);
    //io.KeyAlt   = InputManager::IsKeyDown(KeyboardType::KEY_LEFT_ALT) || InputManager::IsKeyDown(KeyboardType::KEY_RIGHT_ALT);
    //io.KeySuper = false;

    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}
void ImGuiManager::EndFrame()
{
    ImGui::Render();
}

void ImGuiManager::SetImGuiStyle(EImGuiStyleType Type)
{
    switch (Type)
    {
    case EImGuiStyleType::Dark:
        {
            ImGui::StyleColorsDark();
            break;
        }
    case EImGuiStyleType::Vulkan:
        {
            ImGuiStyle& style = ImGui::GetStyle();
            style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
            style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

            break;
        }
    }
}

LRESULT ImGuiManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiManager::OnResize(const UINT32& Width, const UINT32& Height)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(Width), static_cast<float>(Height));
}

void ImGuiManager::CreateMaterial()
{
    static std::string vet = "../_Assets/_Shaders/ImGui/ImGui_GLSL_Vertex.spv";
    static std::string fag = "../_Assets/_Shaders/ImGui/ImGui_GLSL_Pixel.spv";

    std::shared_ptr<RHI::VulkanShader> Shader = RHI::CreateShader(vet.c_str(), fag.c_str());
    {
        Shader->AddDescriptorSetLayoutBinding("ImGuiUniform", VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        Shader->AddDescriptorSetLayoutBinding("DiffuseTexture", VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    Shader->AllocateDescriptorSets();

    RHI::VulkanPipelineInfo PipelineInfo = RHI::VulkanPipelineInfoProxy<ERasterizationInfo_CullModeType::None,
                                                                        ERasterizationInfo_FrontFaceModeType::CW,
                                                                        false,
                                                                        1>::Create();
    {
        PipelineInfo.SetVertexCreateInfo(VertexImGui::GetVkPipelineVertexInputStateCreateInfo());
        PipelineInfo.SetSubpassCount(1);
    }

    // MultisampleStateInfo
    {
        // todo : imgui에서 sample을 쓸거면 수정 필요
        //PipelineInfo.MultisampleStateInfo.sampleShadingEnable = VK_FALSE;
        //PipelineInfo.MultisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }

    Material = std::make_shared<RHI::VulkanMaterial>(PipelineInfo, Shader);
}
void ImGuiManager::DestroyMaterial()
{
    if (Material != nullptr)
    {
        Material.reset();
        Material = nullptr;
    }
}

void ImGuiManager::CreateTexture()
{
    UINT8* FontTextureData = nullptr;
    INT32 TextureWidth = 0;
    INT32 TextureHeight = 0;

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.0f;
    io.Fonts->GetTexDataAsRGBA32(&FontTextureData, &TextureWidth, &TextureHeight);

    Texture = RHI::Create2DTexture(FontTextureData,
        static_cast<UINT32>(TextureWidth), static_cast<UINT32>(TextureHeight), 4, 1, ETextureFormatType::R8G8B8A8);

    Material->WriteTexture("DiffuseTexture", Texture.get());
}
void ImGuiManager::DestroyTexture()
{
    if (Texture != nullptr)
    {
        Texture->Destroy();
        Texture = nullptr;
    }
}

void ImGuiManager::CreateDrawMeshInfos()
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();

    DrawMeshInfos.resize(RHI->GetVulkanSwapChain()->GetBackBufferCount());
}
void ImGuiManager::DestroyDrawMeshInfos()
{
    for (DrawMeshInfo& Info : DrawMeshInfos)
    {
        Info.StaticMesh.reset();
        Info.StaticMesh = nullptr;
    }
    DrawMeshInfos.clear();
}

void ImGuiManager::CreateUniformBuffer()
{
    UniformBuffer = RHI::CreateUniformBuffer(
        sizeof(ImGuiUniformBuffer),
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    Material->WriteUniformBuffer("ImGuiUniform", UniformBuffer.get());
}
void ImGuiManager::DestroyUniformBuffer()
{
    if (UniformBuffer != nullptr)
    {
        UniformBuffer.reset();
        UniformBuffer = nullptr;
    }
}

bool ImGuiManager::DrawMeshInfo::IsNeedUpdate(UINT32 CurrentVertexCount, UINT32 CurrentIndexCount) const
{
    return ((StaticMesh == nullptr) || (VertexCount != CurrentVertexCount) || (IndexCount != CurrentIndexCount));
}
void ImGuiManager::DrawMeshInfo::CreateStaticMesh(const UINT32& CurrentVertexCount, const UINT32& VertexSize, const UINT32& CurrentIndexCount, const UINT32& IndexSize)
{
    if (StaticMesh != nullptr)
    {
        StaticMesh.reset();
        StaticMesh = nullptr;
    }

    VertexCount = CurrentVertexCount;
    IndexCount = CurrentIndexCount;

    StaticMesh = std::make_shared<RHI::VulkanStaticMesh>(
        nullptr, VertexCount, VertexSize,
        nullptr, IndexCount, IndexSize, VK_INDEX_TYPE_UINT16,
        true);
}
void ImGuiManager::DrawMeshInfo::UpdateStaticMesh(
    const void* TargetVertices, const UINT32& UpdateVertexCount, const UINT32& VertexSize, const UINT32& VertexOffset,
    const void* TargetIndices, const UINT32& UpdateIndexCount, const UINT32& IndexSize, const UINT32& IndexOffset)
{
    if (StaticMesh == nullptr)
    {
        return;
    }

    StaticMesh->UpdateBuffer(
        TargetVertices, UpdateVertexCount, VertexSize, VertexOffset,
        TargetIndices, UpdateIndexCount, IndexSize, IndexOffset);
}
