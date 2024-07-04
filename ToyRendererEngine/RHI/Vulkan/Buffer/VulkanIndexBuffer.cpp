#include "VulkanIndexBuffer.h"
#include "VulkanCommandBuffer.h"

using RHI::VulkanIndexBuffer;

VulkanIndexBuffer::~VulkanIndexBuffer()
{
    Destroy();
}

void VulkanIndexBuffer::Destroy()
{
    Super::Destroy();
}

void VulkanIndexBuffer::CreateIndexBuffer(UINT32 TargetIndexCount, VkDeviceSize BufferSize, VkBufferUsageFlags BufferUsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
{
    ASSERT(TargetIndexCount > 0);

    IndexCount = TargetIndexCount;
    CreateBuffer(BufferSize, BufferUsageFlags, MemoryPropertyFlags);
}

void VulkanIndexBuffer::CreateBuffer(VkDeviceSize BufferSize, VkBufferUsageFlags BufferUsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
{
    Super::CreateBuffer(BufferSize, BufferUsageFlags, MemoryPropertyFlags);
}

void VulkanIndexBuffer::BindBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer)
{
    ASSERT_NOT_NULL(CommandBuffer);
    
    vkCmdBindIndexBuffer(CommandBuffer->GetBuffer(), Buffer, 0, IndexType);
}
void VulkanIndexBuffer::BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer)
{
    BeginDrawBuffer(CommandBuffer, IndexCount, 0, 0);
}
void VulkanIndexBuffer::BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawCount, const UINT32& Offset1, const UINT32& Offset2)
{
    ASSERT_NOT_NULL(CommandBuffer);

    vkCmdDrawIndexed(CommandBuffer->GetBuffer(), DrawCount, 1, Offset1, static_cast<INT32>(Offset2), 0);
}

std::shared_ptr<VulkanIndexBuffer> RHI::CreateIndexBuffer(const void* TargetIndices, const UINT32& IndexCount, const UINT32& IndexSize, const VkIndexType& IndexType, const bool& IsCpuAccessible)
{
    VkDeviceSize BufferSize = static_cast<VkDeviceSize>(IndexCount * IndexSize);
    ASSERT(BufferSize > 0);

    std::shared_ptr<VulkanBuffer> StagingBuffer = nullptr;
    std::shared_ptr<VulkanCommandBuffer> CommandBuffer = nullptr;
    std::shared_ptr<VulkanIndexBuffer> IndexBuffer = nullptr;

    if(IsCpuAccessible == true)
    {
        IndexBuffer = std::make_shared<VulkanIndexBuffer>();
        IndexBuffer->CreateIndexBuffer(IndexCount, BufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    else
    {
        ASSERT_NOT_NULL(TargetIndices);
        
        StagingBuffer = RHI::CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        StagingBuffer->UpdateBuffer(TargetIndices, BufferSize);
        
        IndexBuffer = std::make_shared<VulkanIndexBuffer>();
        IndexBuffer->CreateIndexBuffer(IndexCount, BufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        
        CommandBuffer = RHI::CreateCommandBuffer();
        RHI::CopyBuffer(CommandBuffer, StagingBuffer->GetBuffer(), IndexBuffer->GetBuffer(), BufferSize);
    }

    IndexBuffer->SetIndexType(IndexType);
    
    if(CommandBuffer != nullptr)
    {
        CommandBuffer->Destroy();
        CommandBuffer = nullptr;
    }
    if(StagingBuffer != nullptr)
    {
        StagingBuffer->Destroy();
        StagingBuffer = nullptr;
    }

    return IndexBuffer;
}