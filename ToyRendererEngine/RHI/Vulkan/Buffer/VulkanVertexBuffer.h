#pragma once
#include "RHI/Vulkan/VulkanCommon.h"
#include "VulkanBuffer.h"

namespace RHI
{
    // Forward Declarations
    class VulkanCommandBuffer;

    class VulkanVertexBuffer : public VulkanBuffer
    {
    public:
        VulkanVertexBuffer() = default;
        virtual ~VulkanVertexBuffer() final;

    public:
        virtual void Destroy() final;

    public:
        void CreateVertexBuffer(
            UINT32 TargetVertexCount,
            VkDeviceSize BufferSize,
            VkBufferUsageFlags BufferUsageFlags,
            VkMemoryPropertyFlags MemoryPropertyFlags);

    public:
        virtual void BindBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) final;
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) final;
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawCount, const UINT32& Offset1, const UINT32& Offset2) final;

    public:
        UINT32 GetVertexCount() const { return VertexCount; }

    private:
        virtual void CreateBuffer(
            VkDeviceSize BufferSize,
            VkBufferUsageFlags BufferUsageFlags,
            VkMemoryPropertyFlags MemoryPropertyFlags) final;

    private:
        UINT32 VertexCount = 0;
    };

    std::shared_ptr<VulkanVertexBuffer> CreateVertexBuffer(
        const void* TargetVertices,
        const UINT32& VertexCount,
        const UINT32& VertexSize,
        const bool& IsCpuAccessible = false);
}
