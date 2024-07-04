#include "VulkanQueue.h"

using RHI::VulkanQueue;

VulkanQueue::VulkanQueue(VkDevice Device, UINT32 QueueFamilyIndex)
    : FamilyIndex(QueueFamilyIndex)
{
    ASSERT_NOT_NULL(Device);
    vkGetDeviceQueue(Device, FamilyIndex, 0, &Queue);
}
VulkanQueue::~VulkanQueue()
{
    if(Queue != VK_NULL_HANDLE)
    {
        Queue = VK_NULL_HANDLE;
    }
}

UINT32 VulkanQueue::GetFamilyIndex() const
{
    return FamilyIndex;
}
VkQueue VulkanQueue::GetHandle() const
{
    return Queue;
}
