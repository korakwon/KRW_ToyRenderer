#include "VulkanShader.h"

#include "VulkanPipeline.h"
#include "VulkanRHI.h"
#include "Buffer/VulkanCommandBuffer.h"

#include "Core/Engine.h"
#include "Util/Path.h"

using RHI::VulkanShader;

VulkanShader::VulkanShader(VkShaderModule Vertex, VkShaderModule Pixel, VkShaderModule Another)
{
    ASSERT_NOT_NULL(Vertex);
    ASSERT_NOT_NULL(Pixel);

    VertexShaderModule = Vertex;
    PixelShaderModule = Pixel;

    ProcessShaderModule(VertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
    ProcessShaderModule(PixelShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);
    if(Another != nullptr)
    {
        //ProcessShaderModule(Another, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
}
VulkanShader::~VulkanShader()
{
    Destroy();
}

void VulkanShader::Destroy()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    if(VertexShaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(Device, VertexShaderModule, nullptr);
        VertexShaderModule = VK_NULL_HANDLE;    
    }
    if(PixelShaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(Device, PixelShaderModule, nullptr);
        PixelShaderModule = VK_NULL_HANDLE;    
    }
    if(AnotherShaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(Device, AnotherShaderModule, nullptr);
        AnotherShaderModule = VK_NULL_HANDLE;    
    }
    
    if (DescriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(Device, DescriptorPool, nullptr);
        DescriptorPool = VK_NULL_HANDLE;
    }
    if (DescriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(Device, DescriptorSetLayout, nullptr);
        DescriptorSetLayout = nullptr;
    }
}

void VulkanShader::Bind(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, VkPipelineBindPoint PipelineBindPoint, const VulkanPipeline* Pipeline) const
{
    ASSERT_NOT_NULL(Pipeline);
    vkCmdBindDescriptorSets(CommandBuffer->GetBuffer(), PipelineBindPoint, Pipeline->PipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);
}

void VulkanShader::WriteBuffer(const std::string& DescriptorSetLayoutName, const VkDescriptorBufferInfo* DescriptorBufferInfo) const
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    auto it = DescriptorSetLayoutBindings.find(DescriptorSetLayoutName);
    if (it == DescriptorSetLayoutBindings.end())
    {
        ASSERT(false);
        return;
    }

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding = it->second;

    VkWriteDescriptorSet WriteDescriptorSet;
    {
        ZeroVulkanStruct(WriteDescriptorSet, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
        WriteDescriptorSet.dstSet = DescriptorSet;
        WriteDescriptorSet.dstBinding = DescriptorSetLayoutBinding.binding;
        WriteDescriptorSet.dstArrayElement = 0;
        WriteDescriptorSet.descriptorCount = 1;
        WriteDescriptorSet.descriptorType = DescriptorSetLayoutBinding.descriptorType;
        WriteDescriptorSet.pBufferInfo = DescriptorBufferInfo;
    }

    vkUpdateDescriptorSets(Device, 1, &WriteDescriptorSet, 0, nullptr);
}
void VulkanShader::WriteTexture(const std::string& DescriptorSetLayoutName, const VkDescriptorImageInfo* DescriptorImageInfo) const
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    auto it = DescriptorSetLayoutBindings.find(DescriptorSetLayoutName);
    if (it == DescriptorSetLayoutBindings.end())
    {
        ASSERT(false);
        return;
    }

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding = it->second;

    VkWriteDescriptorSet WriteDescriptorSet;
    {
        ZeroVulkanStruct(WriteDescriptorSet, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
        WriteDescriptorSet.dstSet = DescriptorSet;
        WriteDescriptorSet.dstBinding = DescriptorSetLayoutBinding.binding;
        WriteDescriptorSet.dstArrayElement = 0;
        WriteDescriptorSet.descriptorCount = 1;
        WriteDescriptorSet.descriptorType = DescriptorSetLayoutBinding.descriptorType;
        WriteDescriptorSet.pImageInfo = DescriptorImageInfo;
    }

    vkUpdateDescriptorSets(Device, 1, &WriteDescriptorSet, 0, nullptr);
}

void VulkanShader::AddDescriptorSetLayoutBinding(const std::string& DescriptorSetLayoutName, VkDescriptorType DescriptorType, VkShaderStageFlags StageFlags)
{
    // todo : Spirv에서 자동으로 가져오도록 하자. VulkanDemo참고
    VkDescriptorSetLayoutBinding Binding;
    {
        Binding.binding = static_cast<UINT32>(DescriptorSetLayoutBindings.size());
        Binding.descriptorCount = 1;
        Binding.descriptorType = DescriptorType;
        Binding.pImmutableSamplers = nullptr;
        Binding.stageFlags = StageFlags;
    }
    
    DescriptorSetLayoutBindings.emplace(std::make_pair(DescriptorSetLayoutName, Binding));
}
void VulkanShader::AllocateDescriptorSets()
{
    // todo : Init에서 진행 필요, Clear 필요
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();
    
    std::vector<VkDescriptorSetLayoutBinding> BindingList;
    for(const auto& Binding : DescriptorSetLayoutBindings)
    {
        BindingList.emplace_back(Binding.second);
    }

    // DescriptorPool
    std::vector<VkDescriptorPoolSize> PoolSizes;
    VkDescriptorPoolCreateInfo PoolCreateInfo;
    {
        for(auto Binding: BindingList)
        {
            VkDescriptorPoolSize DescriptorPoolSize;
            DescriptorPoolSize.type = Binding.descriptorType;
            DescriptorPoolSize.descriptorCount = 1;
            
            PoolSizes.emplace_back(DescriptorPoolSize);
        }
        
        ZeroVulkanStruct(PoolCreateInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO);
        PoolCreateInfo.poolSizeCount = static_cast<UINT32>(PoolSizes.size());
        PoolCreateInfo.pPoolSizes = PoolSizes.data();
        PoolCreateInfo.maxSets = static_cast<UINT32>(PoolSizes.size());
    }
    ASSERT_VK_RESULT(vkCreateDescriptorPool(Device, &PoolCreateInfo, nullptr, &DescriptorPool));

    // DescriptorSetLayout
    VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo;
    {
        ZeroVulkanStruct(DescriptorSetLayoutCreateInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO);
        DescriptorSetLayoutCreateInfo.bindingCount = static_cast<UINT32>(BindingList.size());
        DescriptorSetLayoutCreateInfo.pBindings = BindingList.data();
    }
    ASSERT_VK_RESULT(vkCreateDescriptorSetLayout(Device, &DescriptorSetLayoutCreateInfo, nullptr, &DescriptorSetLayout));

    // DescriptorSets
    VkDescriptorSetAllocateInfo DescriptorSetAllocateInfo;
    {
        ZeroVulkanStruct(DescriptorSetAllocateInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO);
        DescriptorSetAllocateInfo.descriptorPool = DescriptorPool;
        DescriptorSetAllocateInfo.descriptorSetCount = 1;
        DescriptorSetAllocateInfo.pSetLayouts = &DescriptorSetLayout;
    }
    ASSERT_VK_RESULT(vkAllocateDescriptorSets(Device, &DescriptorSetAllocateInfo, &DescriptorSet));
}

void VulkanShader::ProcessShaderModule(VkShaderModule ShaderModule, VkShaderStageFlagBits ShaderStageFlagBits)
{
    ASSERT_NOT_NULL(ShaderModule);
    
    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo;
    {
        ZeroVulkanStruct(PipelineShaderStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
        PipelineShaderStageCreateInfo.stage = ShaderStageFlagBits;
        PipelineShaderStageCreateInfo.module = ShaderModule;
        PipelineShaderStageCreateInfo.pName = "main";
    }
    
    PipelineShaderStageCreateInfos.emplace_back(PipelineShaderStageCreateInfo);
}

VkShaderModule CreateShaderModule(const std::string& Code)
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    VkShaderModuleCreateInfo CreateInfo;
    {
        ZeroVulkanStruct(CreateInfo, VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
        CreateInfo.codeSize = Code.size();
        CreateInfo.pCode = reinterpret_cast<const UINT32*>(Code.data());
    }

    VkShaderModule ShaderModule = VK_NULL_HANDLE;
    ASSERT_VK_RESULT(vkCreateShaderModule(Device, &CreateInfo, nullptr, &ShaderModule));

    return ShaderModule;
}

std::shared_ptr<VulkanShader> RHI::CreateShader(
    const char* VertexShaderPath,
    const char* PixelShaderPath,
    const char* AnotherShaderPath)
{
    ASSERT(VertexShaderPath);
    ASSERT(PixelShaderPath);
    
    VkShaderModule VertShaderModule = CreateShaderModule(Util::Path::ReadFile(VertexShaderPath));
    VkShaderModule PixelShaderModule = CreateShaderModule(Util::Path::ReadFile(PixelShaderPath));
    VkShaderModule AnotherShaderModule = nullptr;

    if(AnotherShaderPath != nullptr)
    {
        AnotherShaderModule = CreateShaderModule(Util::Path::ReadFile(AnotherShaderPath));
    }
    
    std::shared_ptr<VulkanShader> Shader = std::make_shared<VulkanShader>(
        VertShaderModule,
        PixelShaderModule,
        AnotherShaderModule);
    
    return Shader;
}
