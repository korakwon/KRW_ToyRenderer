#pragma once
#include "VulkanCommon.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanPipeline;
    class VulkanTexture;
    class VulkanUniformBuffer;
    class VulkanCommandBuffer;
}

// ***************************** RHI ******************************
namespace RHI
{
    class VulkanShader
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(VulkanShader)
        
    public:
        VulkanShader(VkShaderModule Vertex, VkShaderModule Pixel, VkShaderModule Another = VK_NULL_HANDLE);
        ~VulkanShader();

    public:
        virtual void Destroy();

    public:
        void Bind(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, VkPipelineBindPoint PipelineBindPoint, const VulkanPipeline* Pipeline) const;
        
    public:
        void WriteBuffer(const std::string& DescriptorSetLayoutName, const VkDescriptorBufferInfo* DescriptorBufferInfo) const;
        void WriteTexture(const std::string& DescriptorSetLayoutName, const VkDescriptorImageInfo* DescriptorImageInfo) const;
        
    public:
        const std::vector<VkPipelineShaderStageCreateInfo>& GetPipelineShaderStageInfos() { return PipelineShaderStageCreateInfos; }

    public:
        VkDescriptorSetLayout* GetDescriptorSetLayout() { return &DescriptorSetLayout; }
        
        void AddDescriptorSetLayoutBinding(const std::string& DescriptorSetLayoutName, VkDescriptorType DescriptorType, VkShaderStageFlags StageFlags);
        void AllocateDescriptorSets();
        
    private:
        void ProcessShaderModule(VkShaderModule ShaderModule, VkShaderStageFlagBits ShaderStageFlagBits);

    private:
        std::vector<VkPipelineShaderStageCreateInfo> PipelineShaderStageCreateInfos;

    private:
        VkDescriptorPool DescriptorPool = VK_NULL_HANDLE;
        VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorSet DescriptorSet = VK_NULL_HANDLE;
        std::unordered_map<std::string, VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings;

    private:
        VkShaderModule VertexShaderModule = VK_NULL_HANDLE;
        VkShaderModule PixelShaderModule = VK_NULL_HANDLE;
        VkShaderModule AnotherShaderModule = VK_NULL_HANDLE;
        
    private:
        friend RHI::VulkanPipeline;
    };

    std::shared_ptr<VulkanShader> CreateShader(
        const char* VertexShaderPath,
        const char* PixelShaderPath,
        const char* AnotherShaderPath = nullptr);
}
