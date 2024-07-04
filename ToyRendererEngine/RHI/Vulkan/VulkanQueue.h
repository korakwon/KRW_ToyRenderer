#pragma once
#include "VulkanCommon.h"

// ****************************** RHI *******************************
namespace RHI
{
    // Forward Declarations
    class VulkanDevice;
    
    class VulkanQueue
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(VulkanQueue)
        
    private:
        VulkanQueue(VkDevice Device, UINT32 QueueFamilyIndex);
        
    public:                  
        ~VulkanQueue();
        
    public:
        UINT32 GetFamilyIndex() const;
        VkQueue GetHandle() const;
    
    private:
        VkQueue Queue;
        UINT32  FamilyIndex;
        
    private:
        friend RHI::VulkanDevice;
    };
}
