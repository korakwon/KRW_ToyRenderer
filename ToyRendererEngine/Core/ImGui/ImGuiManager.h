#pragma once
#include "pch.h"
#include "Util/Math.h"

#include "RHI/Vulkan/VulkanCommon.h"

// ****************************** Define *****************************
#define IMGUI_MANAGER Core::ImGuiManager::GetInstance()


// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanMaterial;
    class VulkanTexture;
    class VulkanVertexBuffer;
    class VulkanIndexBuffer;
    class VulkanUniformBuffer;
    class VulkanStaticMesh;
}
namespace Core
{
    class Engine;
}

// *************************** Enum Class ***************************
namespace Core
{
    enum class EImGuiStyleType
    {
        Dark,
        Vulkan
    };
}

// ****************************** Croe *******************************
namespace Core
{
    struct ImGuiUniformBuffer
    {
        Vector2 Scale;
        Vector2 Translate;
    };
    
    class ImGuiManager : public ICoreExecute
    {
        DECLARE_SINGLETON(ImGuiManager)
        
    public:
        virtual ~ImGuiManager() final;
        
    public:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        void Render();
        virtual void Destroy() final;

    public:
        void StartFrame();
        void EndFrame();

    public:
        void SetImGuiStyle(EImGuiStyleType Type);
        
    public:
        LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        void OnResize(const UINT32& Width, const UINT32& Height);

    private:
        void CreateMaterial();
        void DestroyMaterial();
        
    private:
        void CreateTexture();
        void DestroyTexture();

    private:
        void CreateDrawMeshInfos();
        void DestroyDrawMeshInfos();
            
    private:
        
    private:
        void CreateUniformBuffer();
        void DestroyUniformBuffer();
    
    private:
        std::shared_ptr<RHI::VulkanMaterial> Material;
        
    private:
        std::shared_ptr<RHI::VulkanTexture> Texture;
        
    private:
        std::shared_ptr<RHI::VulkanUniformBuffer> UniformBuffer;
        
    private:
        struct DrawMeshInfo
        {
        public:
            bool IsNeedUpdate(UINT32 CurrentVertexCount, UINT32 CurrentIndexCount) const;
            void CreateStaticMesh(const UINT32& CurrentVertexCount, const UINT32& VertexSize, const UINT32& CurrentIndexCount, const UINT32& IndexSize);
            void UpdateStaticMesh(
                const void* TargetVertices, const UINT32& UpdateVertexCount, const UINT32& VertexSize, const UINT32& VertexOffset,
                const void* TargetIndices, const UINT32& UpdateIndexCount, const UINT32& IndexSize, const UINT32& IndexOffset);

        public:
            std::shared_ptr<RHI::VulkanStaticMesh> StaticMesh;
            UINT32 VertexCount = 0;
            UINT32 IndexCount = 0;
        };
        std::vector<DrawMeshInfo> DrawMeshInfos;
        
    private:
        friend Core::Engine;
    };
}