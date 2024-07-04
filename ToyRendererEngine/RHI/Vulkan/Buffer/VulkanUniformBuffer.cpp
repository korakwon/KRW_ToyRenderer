#include "VulkanUniformBuffer.h"
#include "VulkanCommandBuffer.h"

// 임시 헤더
#include "Application/Application.h"

using RHI::VulkanUniformBuffer;

VulkanUniformBuffer::~VulkanUniformBuffer()
{
    Destroy();
}

std::shared_ptr<VulkanUniformBuffer> RHI::CreateUniformBuffer(VkDeviceSize BufferSize, VkBufferUsageFlags BufferUsageFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
{
    std::shared_ptr<VulkanUniformBuffer> ResultBuffer = std::make_shared<VulkanUniformBuffer>();
    ResultBuffer->CreateBuffer(BufferSize, BufferUsageFlags, MemoryPropertyFlags);
    
    return ResultBuffer;
}
