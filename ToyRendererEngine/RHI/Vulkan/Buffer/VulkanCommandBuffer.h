#pragma once
#include "RHI/Vulkan/VulkanCommon.h"

namespace RHI
{
    class VulkanCommandBuffer
    {
    public:
        virtual ~VulkanCommandBuffer();
        
    public:
        void Destroy();
        
    public:
        void* CreateBuffer();

    public:
        void Begin() const;
        void End() const;
        
    public:
        VkCommandBuffer* GetBufferPtr() { return &CommandBuffer; }
        VkCommandBuffer GetBuffer() const { return CommandBuffer; }
        
    private:
        VkCommandBuffer CommandBuffer = VK_NULL_HANDLE; 
    };

    std::shared_ptr<VulkanCommandBuffer> CreateCommandBuffer();
}
