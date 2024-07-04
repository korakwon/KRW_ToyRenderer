#pragma once
#include "RHI/Vulkan/VulkanCommon.h"
#include "RHI/Vulkan/Buffer/VulkanBuffer.h"

namespace RHI
{
    class VulkanCommandBuffer;

    class VulkanUniformBuffer : public VulkanBuffer
    {
    public:
        VulkanUniformBuffer() = default;
        virtual ~VulkanUniformBuffer() override;

    private:
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) final { ASSERT(false); }
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawCount, const UINT32& Offset1, const UINT32& Offset2) final { ASSERT(false); }
    };
    
    std::shared_ptr<VulkanUniformBuffer> CreateUniformBuffer(
        VkDeviceSize BufferSize,
        VkBufferUsageFlags BufferUsageFlags,
        VkMemoryPropertyFlags MemoryPropertyFlags);

}
