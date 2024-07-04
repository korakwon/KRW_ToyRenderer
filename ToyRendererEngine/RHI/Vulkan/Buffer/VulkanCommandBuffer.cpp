#include "VulkanCommandBuffer.h"
#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/VulkanQueue.h"

using RHI::VulkanCommandBuffer;

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    Destroy();
}

void VulkanCommandBuffer::Destroy()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    if(CommandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(Device, RHI->GetCommandPool(), 1, &CommandBuffer);
        CommandBuffer = VK_NULL_HANDLE; 
    }
}

void* VulkanCommandBuffer::CreateBuffer()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();
    
    VkCommandBufferAllocateInfo AllocateInfo;
    ZeroVulkanStruct(AllocateInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO);
    AllocateInfo.commandPool = RHI->GetCommandPool();
    AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    AllocateInfo.commandBufferCount = 1;

    ASSERT_VK_RESULT(vkAllocateCommandBuffers(Device, &AllocateInfo, &CommandBuffer));

    return CommandBuffer;
}
void VulkanCommandBuffer::Begin() const
{
    VkCommandBufferBeginInfo BeginInfo;
    ZeroVulkanStruct(BeginInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO);

    ASSERT_VK_RESULT(vkBeginCommandBuffer(CommandBuffer, &BeginInfo));
}
void VulkanCommandBuffer::End() const
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    
    ASSERT_VK_RESULT(vkEndCommandBuffer(CommandBuffer));
}

std::shared_ptr<VulkanCommandBuffer> RHI::CreateCommandBuffer()
{
    std::shared_ptr<VulkanCommandBuffer> ResultBuffer = std::make_shared<VulkanCommandBuffer>();
    ResultBuffer->CreateBuffer();
    
    return ResultBuffer;
}
