#include "VulkanVertexBuffer.h"
#include "VulkanCommandBuffer.h"

using RHI::VulkanVertexBuffer;

VulkanVertexBuffer::~VulkanVertexBuffer()
{
    Destroy();
}

void VulkanVertexBuffer::Destroy()
{
    Super::Destroy();
}

void VulkanVertexBuffer::CreateVertexBuffer(UINT32 TargetVertexCount, VkDeviceSize BufferSize, VkBufferUsageFlags BufferUsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
{
    ASSERT(TargetVertexCount > 0);

    VertexCount = TargetVertexCount;
    CreateBuffer(BufferSize, BufferUsageFlags, MemoryPropertyFlags);
}

void VulkanVertexBuffer::BindBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer)
{
    ASSERT_NOT_NULL(CommandBuffer);

    VkDeviceSize Offsets = 0;
    vkCmdBindVertexBuffers(CommandBuffer->GetBuffer(), 0, 1, &Buffer, &Offsets);
}
void VulkanVertexBuffer::BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer)
{
    BeginDrawBuffer(CommandBuffer, VertexCount, 0, 0);
}
void VulkanVertexBuffer::BeginDrawBuffer(const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawCount, const UINT32& Offset1, const UINT32& Offset2)
{
    ASSERT_NOT_NULL(CommandBuffer);

    vkCmdDraw(CommandBuffer->GetBuffer(), DrawCount, 1, Offset1, 0);
}

void VulkanVertexBuffer::CreateBuffer(VkDeviceSize BufferSize, VkBufferUsageFlags BufferUsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
{
    VulkanBuffer::CreateBuffer(BufferSize, BufferUsageFlags, MemoryPropertyFlags);
}

std::shared_ptr<VulkanVertexBuffer> RHI::CreateVertexBuffer(const void* TargetVertices, const UINT32& VertexCount, const UINT32& VertexSize, const bool& IsCpuAccessible)
{
    VkDeviceSize BufferSize = static_cast<VkDeviceSize>(VertexCount * VertexSize);
    ASSERT(BufferSize > 0);

    std::shared_ptr<VulkanBuffer> StagingBuffer = nullptr;
    std::shared_ptr<VulkanCommandBuffer> CommandBuffer = nullptr;
    std::shared_ptr<VulkanVertexBuffer> VertexBuffer = nullptr;

    if(IsCpuAccessible == true)
    {
        VertexBuffer = std::make_shared<VulkanVertexBuffer>();
        VertexBuffer->CreateVertexBuffer(VertexCount, BufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    else
    {
        ASSERT_NOT_NULL(TargetVertices);

        StagingBuffer = RHI::CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        StagingBuffer->UpdateBuffer(TargetVertices, BufferSize);
        
        VertexBuffer = std::make_shared<VulkanVertexBuffer>();
        VertexBuffer->CreateVertexBuffer(VertexCount, BufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        
        CommandBuffer = RHI::CreateCommandBuffer();
        RHI::CopyBuffer(CommandBuffer, StagingBuffer->GetBuffer(), VertexBuffer->GetBuffer(), BufferSize);
    }
    
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

    return VertexBuffer;
}
