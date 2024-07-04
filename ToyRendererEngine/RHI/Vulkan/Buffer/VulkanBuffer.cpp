#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"

#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"

using RHI::VulkanBuffer;

VulkanBuffer::~VulkanBuffer()
{
    Destroy();
}

void VulkanBuffer::Destroy()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    if(MappedPointer != nullptr)
    {
        UnMap();
        MappedPointer = nullptr;
    }
    if(Buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Device, Buffer, nullptr);
        Buffer = VK_NULL_HANDLE;
    }
    if(BufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Device, BufferMemory, nullptr);
        BufferMemory = VK_NULL_HANDLE;
    }
}

void VulkanBuffer::CreateBuffer(
    VkDeviceSize BufferSize,
    VkBufferUsageFlags BufferUsageFlags,
    VkMemoryPropertyFlags MemoryPropertyFlags)
{
    ASSERT(BufferSize > 0);

    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    VkBufferCreateInfo BufferCreateInfo;
    {
        ZeroVulkanStruct(BufferCreateInfo, VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO);
        BufferCreateInfo.size = BufferSize;
        BufferCreateInfo.usage = BufferUsageFlags;
        BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    ASSERT_VK_RESULT(vkCreateBuffer(Device, &BufferCreateInfo, nullptr, &Buffer));

    VkMemoryRequirements MemoryRequirements;
    VkMemoryAllocateInfo MemoryAllocateInfo;
    {
        vkGetBufferMemoryRequirements(Device, Buffer, &MemoryRequirements);

        ZeroVulkanStruct(MemoryAllocateInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
        MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
        MemoryAllocateInfo.memoryTypeIndex = RHI->GetVulkanDevice()->FindMemoryType(MemoryRequirements.memoryTypeBits, MemoryPropertyFlags);
    }
    
    ASSERT_VK_RESULT(vkAllocateMemory(Device, &MemoryAllocateInfo, nullptr, &BufferMemory));
    ASSERT_VK_RESULT(vkBindBufferMemory(Device, Buffer, BufferMemory, 0));

    AllocateBufferSize = BufferSize;

    DescriptorBufferInfo.buffer = Buffer;
    DescriptorBufferInfo.offset = 0;
    DescriptorBufferInfo.range = AllocateBufferSize;
}

void* VulkanBuffer::Map(const VkDeviceSize& BufferSize)
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    ASSERT(BufferSize > 0);
    ASSERT(BufferSize <= AllocateBufferSize);
    ASSERT_NULL(MappedPointer);

    ASSERT_VK_RESULT(vkMapMemory(Device, BufferMemory, 0, BufferSize, 0, &MappedPointer));

    return MappedPointer;
}
void VulkanBuffer::UnMap()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    ASSERT_NOT_NULL(MappedPointer);

    vkUnmapMemory(Device, BufferMemory);

    MappedPointer = nullptr;
}
void VulkanBuffer::UpdateBuffer(const void* SrcBufferData, const VkDeviceSize& BufferSize, const UINT32& Offset)
{
    ASSERT(BufferSize > 0);
    ASSERT(BufferSize <= AllocateBufferSize);
    ASSERT((BufferSize + Offset) <= AllocateBufferSize);
    ASSERT_NOT_NULL(SrcBufferData);
    ASSERT_NULL(MappedPointer);

    Map(BufferSize);
    {
        memcpy(static_cast<BYTE*>(MappedPointer) + Offset, SrcBufferData, BufferSize);
    }
    UnMap();
}

std::shared_ptr<VulkanBuffer> RHI::CreateBuffer(
    VkDeviceSize BufferSize,
    VkBufferUsageFlags BufferUsageFlags,
    VkMemoryPropertyFlags MemoryPropertyFlags)
{
    std::shared_ptr<VulkanBuffer> ResultBuffer = std::make_shared<VulkanBuffer>();
    ResultBuffer->CreateBuffer(BufferSize, BufferUsageFlags, MemoryPropertyFlags);
    
    return ResultBuffer;
}

void RHI::CopyBuffer(
    const std::shared_ptr<VulkanCommandBuffer>& CommandBuffer,
    VkBuffer SrcBuffer,
    VkBuffer DstBuffer,
    VkDeviceSize BufferSize)
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();

    RHI->BeginSingleTimeCommandSubmit(CommandBuffer);
    {
        VkBufferCopy CopyRegion = {};
        CopyRegion.size = BufferSize;
        vkCmdCopyBuffer(CommandBuffer->GetBuffer(), SrcBuffer, DstBuffer, 1, &CopyRegion);
    }
    RHI->EndSingleTimeCommandSubmit(CommandBuffer);
}
