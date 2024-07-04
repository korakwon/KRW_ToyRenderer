#pragma once
#include "VulkanCommon.h"

namespace RHI
{
    // Forward Declarations
    class VulkanRHI;
    class VulkanQueue;

    class VulkanDevice
    {
        DECLARE_PRIVATE_CLASS(VulkanDevice)
        
    public:                  
        ~VulkanDevice();
        
    public:
        void Destroy();

    public:
        VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures() const { return PhysicalDeviceFeatures;}
        VkPhysicalDeviceProperties GetPhysicalDeviceProperties() const { return PhysicalDeviceProperties;}

    public:
        UINT32 FindMemoryType(UINT32 TypeFilter, VkMemoryPropertyFlags MemoryPropertyFlags) const;
        
    private:
        void CreatePhysicalDevice();
        void CreateLogicalDevice();
        void CreateQueue();
        
    private:
        void DestroyPhysicalDevice();
        void DestroyLogicalDevice();
        void DestroyQueue();

    private:
        bool IsPhysicalDeviceSuitable(const VkPhysicalDevice& VulkanPhysicalDevice, VkSurfaceKHR Surface);
        void CreateQueueFamilyInfo();

    private:
        std::shared_ptr<VulkanQueue> GraphicsQueue = nullptr;
        std::shared_ptr<VulkanQueue> ComputeQueue = nullptr;
        std::shared_ptr<VulkanQueue> TransferQueue = nullptr;
        std::shared_ptr<VulkanQueue> PresentQueue = nullptr;
        
        std::vector<VkDeviceQueueCreateInfo> DeviceQueueCreateInfos;
        UINT32 GraphicsQueueFamilyIndex = UINT32_MAX;
        UINT32 ComputeQueueFamilyIndex = UINT32_MAX;
        UINT32 TransferQueueFamilyIndex = UINT32_MAX;
        
    private:
        VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
        VkPhysicalDeviceProperties PhysicalDeviceProperties;

    private:
        VkDevice LogicalDevice = VK_NULL_HANDLE;
        
    private:
        friend RHI::VulkanRHI;
    };
}




        