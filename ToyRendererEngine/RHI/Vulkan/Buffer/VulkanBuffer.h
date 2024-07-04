#pragma once
#include "RHI/Vulkan/VulkanCommon.h"

namespace RHI
{
    // Forward Declarations
    class VulkanCommandBuffer;

    class VulkanBuffer
    {
    public:
        VulkanBuffer() = default;
        virtual ~VulkanBuffer();

    public:
        virtual void Destroy();

    public:
        virtual void CreateBuffer(
            VkDeviceSize BufferSize,
            VkBufferUsageFlags BufferUsageFlags,
            VkMemoryPropertyFlags MemoryPropertyFlags);

    public:
        virtual void* Map(const VkDeviceSize& BufferSize);
        virtual void UnMap();
        virtual void UpdateBuffer(const void* SrcBufferData, const VkDeviceSize& BufferSize, const UINT32& Offset = 0);

    public:
        virtual void BindBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) { ASSERT(false); }
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer) { ASSERT(false); }
        virtual void BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawCount, const UINT32& Offset1, const UINT32& Offset2) { ASSERT(false); }

    public:
        VkBuffer* GetBufferPtr() { return &Buffer; }
        VkBuffer GetBuffer() const { return Buffer; }

    public:
        VkDeviceSize GetBufferSize() const { return AllocateBufferSize; }

    public:
        VkDescriptorBufferInfo* GetDescriptorBufferInfo() { return &DescriptorBufferInfo; }

    protected:
        VkBuffer Buffer = VK_NULL_HANDLE;
        VkDeviceMemory BufferMemory = VK_NULL_HANDLE;

        VkDeviceSize AllocateBufferSize = 0;
        void* MappedPointer = nullptr;

    protected:
        VkDescriptorBufferInfo DescriptorBufferInfo;
    };

    std::shared_ptr<VulkanBuffer> CreateBuffer(
        VkDeviceSize BufferSize,
        VkBufferUsageFlags BufferUsageFlags,
        VkMemoryPropertyFlags MemoryPropertyFlags);

    void CopyBuffer(
        const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer,
        VkBuffer SrcBuffer,
        VkBuffer DstBuffer,
        VkDeviceSize BufferSize);
}
