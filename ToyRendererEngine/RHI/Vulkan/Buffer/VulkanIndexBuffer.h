#pragma once
#include "RHI/Vulkan/VulkanCommon.h"
#include "VulkanBuffer.h"

namespace RHI
{
    // Forward Declarations
    class VulkanCommandBuffer;

    class VulkanIndexBuffer : public VulkanBuffer
    {
    public:
        VulkanIndexBuffer() = default;
        virtual ~VulkanIndexBuffer() final;

    public:
        virtual void Destroy() final;

    public:
        void SetIndexType(VkIndexType Type) { IndexType = Type; }

    public:
        void CreateIndexBuffer(
            UINT32 TargetIndexCount,
            VkDeviceSize BufferSize,
            VkBufferUsageFlags BufferUsageFlags,
            VkMemoryPropertyFlags MemoryPropertyFlags);

    public:
        virtual void BindBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) final;
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) final;
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawCount, const UINT32& Offset1, const UINT32& Offset2) final;

    public:
        UINT32 GetIndexCount() const { return IndexCount; }
        
    private:
        virtual void CreateBuffer(
            VkDeviceSize BufferSize,
            VkBufferUsageFlags BufferUsageFlags,
            VkMemoryPropertyFlags MemoryPropertyFlags) final;

    private:
        UINT32 IndexCount = 0;
        VkIndexType IndexType = VK_INDEX_TYPE_UINT32;
    };

    std::shared_ptr<VulkanIndexBuffer> CreateIndexBuffer(
        const void* TargetIndices,
        const UINT32& IndexCount,
        const UINT32& IndexSize,
        const VkIndexType& IndexType = VK_INDEX_TYPE_UINT32,
        const bool& IsCpuAccessible = false);
}
