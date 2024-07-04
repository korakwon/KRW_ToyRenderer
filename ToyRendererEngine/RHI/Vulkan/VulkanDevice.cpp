#include "VulkanDevice.h"
#include "VulkanRHI.h"
#include "VulkanExtensionLayer.h"
#include "VulkanQueue.h"
#include "Core/Engine.h"

using RHI::VulkanRHI;
using RHI::VulkanDevice;

// ****************************** RHI ************************************
void VulkanRHI::CreateVulkanDevice()
{
    Device.reset(new VulkanDevice());
    ASSERT_NOT_NULL(Device);
}
void VulkanRHI::DestroyVulkanDevice()
{
    if(Device != nullptr)
    {
        Device->Destroy();
        Device.reset();
        Device = nullptr;
    }
}

// ****************************** Device *********************************
VulkanDevice::VulkanDevice()
{
    CreatePhysicalDevice();
    CreateLogicalDevice();
    CreateQueue();
}
VulkanDevice::~VulkanDevice()
{
    Destroy();
}

void VulkanDevice::Destroy()
{
    // Queue
    DestroyQueue();
    
    // Physical Device
    DestroyPhysicalDevice();

    // Logical Device
    DestroyLogicalDevice();
}

UINT32 VulkanDevice::FindMemoryType(UINT32 TypeFilter, VkMemoryPropertyFlags MemoryPropertyFlags) const
{
    VkPhysicalDeviceMemoryProperties MemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &MemoryProperties);

    for (UINT32 i = 0; i < MemoryProperties.memoryTypeCount; i++)
    {
        if ((TypeFilter & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & MemoryPropertyFlags) == MemoryPropertyFlags)
        {
            return i;
        }
    }

    ASSERT(false);
    return 0;
}

void VulkanDevice::CreatePhysicalDevice()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    
    UINT32 GPUCount = 0;
    ASSERT_VK_RESULT(vkEnumeratePhysicalDevices(RHI->GetVulkanInstance(), &GPUCount, nullptr));
    ASSERT_ZERO(GPUCount);
    
    std::vector<VkPhysicalDevice> VulkanPhysicalDevices(GPUCount);
    ASSERT_VK_RESULT(vkEnumeratePhysicalDevices(RHI->GetVulkanInstance(), &GPUCount, VulkanPhysicalDevices.data()));

    for (auto VulkanPhysicalDevice : VulkanPhysicalDevices)
    {
        if(IsPhysicalDeviceSuitable(VulkanPhysicalDevice, RHI->GetVulkanSurface()) == true)
        {
            PhysicalDevice = VulkanPhysicalDevice;
            break;
        }
    }
    ASSERT_NOT_NULL(PhysicalDevice);
    
    vkGetPhysicalDeviceFeatures(PhysicalDevice, &PhysicalDeviceFeatures);
    vkGetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProperties);
}
void VulkanDevice::CreateLogicalDevice()
{
    std::vector<const char*> DeviceExtensionNames;
    std::vector<const char*> DeviceLayerNames;
    GetDeviceExtensionLayer(PhysicalDevice, DeviceExtensionNames, DeviceLayerNames);

    CreateQueueFamilyInfo();

    PhysicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
    
    VkDeviceCreateInfo DeviceCreateInfo;
    ZeroVulkanStruct(DeviceCreateInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
    DeviceCreateInfo.queueCreateInfoCount = static_cast<UINT32>(DeviceQueueCreateInfos.size());
    DeviceCreateInfo.pQueueCreateInfos = (DeviceCreateInfo.queueCreateInfoCount > 0) ? DeviceQueueCreateInfos.data() : nullptr;

    DeviceCreateInfo.enabledExtensionCount = static_cast<UINT32>(DeviceExtensionNames.size());
    DeviceCreateInfo.ppEnabledExtensionNames = (DeviceCreateInfo.enabledExtensionCount > 0) ? DeviceExtensionNames.data() : nullptr;
    DeviceCreateInfo.enabledLayerCount = static_cast<UINT32>(DeviceLayerNames.size());
    DeviceCreateInfo.ppEnabledLayerNames = (DeviceCreateInfo.enabledLayerCount > 0) ? DeviceLayerNames.data() : nullptr;

    DeviceCreateInfo.pEnabledFeatures = &PhysicalDeviceFeatures;
    ASSERT_VK_RESULT(vkCreateDevice(PhysicalDevice, &DeviceCreateInfo, nullptr, &LogicalDevice));
}
void VulkanDevice::CreateQueue()
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    
    GraphicsQueue.reset(new VulkanQueue(LogicalDevice, GraphicsQueueFamilyIndex));
    ComputeQueue.reset(new VulkanQueue(LogicalDevice, ComputeQueueFamilyIndex));
    TransferQueue.reset(new VulkanQueue(LogicalDevice, TransferQueueFamilyIndex));

    // Present Queue
    VkBool32 PresentSupport = VK_FALSE;
    const UINT32 FamilyIndex = GraphicsQueue->GetFamilyIndex();
    ASSERT_VK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, FamilyIndex, RHI->GetVulkanSurface(), &PresentSupport));
    PresentQueue = GraphicsQueue;
}

void VulkanDevice::DestroyPhysicalDevice()
{
    PhysicalDevice = VK_NULL_HANDLE;
}
void VulkanDevice::DestroyLogicalDevice()
{
    if(LogicalDevice != VK_NULL_HANDLE)
    {
        vkDestroyDevice(LogicalDevice, nullptr);
        LogicalDevice = VK_NULL_HANDLE;
    }
}
void VulkanDevice::DestroyQueue()
{
    GraphicsQueue.reset();
    GraphicsQueue = nullptr;
    ComputeQueue.reset();
    ComputeQueue = nullptr;
    TransferQueue.reset();
    TransferQueue = nullptr;
    PresentQueue.reset();
    PresentQueue = nullptr;
}

bool VulkanDevice::IsPhysicalDeviceSuitable(const VkPhysicalDevice& VulkanPhysicalDevice, VkSurfaceKHR Surface)
{
    VkPhysicalDeviceFeatures SupportedPhysicalDeviceFeatures;
    vkGetPhysicalDeviceFeatures(VulkanPhysicalDevice, &SupportedPhysicalDeviceFeatures);
    if(SupportedPhysicalDeviceFeatures.samplerAnisotropy == false)
    {
        return false;
    }
    
    UINT32 QueueFamilyCount = 0;
    std::vector<VkQueueFamilyProperties> QueueFamilyProperties(QueueFamilyCount);
    {
        vkGetPhysicalDeviceQueueFamilyProperties(VulkanPhysicalDevice, &QueueFamilyCount, nullptr);
        if(QueueFamilyCount == 0)
        {
            return false;
        }
    
        QueueFamilyProperties.resize(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(VulkanPhysicalDevice, &QueueFamilyCount, QueueFamilyProperties.data());
        if(QueueFamilyProperties.empty() == true)
        {
            return false;
        }
    }

    bool IsValidQueue = false;
    for (UINT32 Index = 0; Index < QueueFamilyCount; Index++)
    {
        const VkQueueFamilyProperties& QueueFamilyProperty = QueueFamilyProperties[Index];
        
        if ((QueueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
        {
            // Present Support
            VkBool32 PresentSupport = VK_FALSE;
            {
                vkGetPhysicalDeviceSurfaceSupportKHR(VulkanPhysicalDevice, Index, Surface, &PresentSupport);
            }

            // SwapChain Support
            VkBool32 SwapChainSupport = VK_FALSE;
            {
                VkSurfaceCapabilitiesKHR Capabilities = {};
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanPhysicalDevice, Surface, &Capabilities);

                UINT32 FormatCount = 0;
                vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanPhysicalDevice, Surface, &FormatCount, nullptr);
                if(FormatCount == 0)
                {
                    continue;
                }
                
                std::vector<VkSurfaceFormatKHR> Formats(FormatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanPhysicalDevice, Surface, &FormatCount, Formats.data());
                if(Formats.empty() == true)
                {
                    continue;
                }

                UINT32 PresentModeCount = 0;
                vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanPhysicalDevice, Surface, &PresentModeCount, nullptr);
                if(PresentModeCount == 0)
                {
                    continue;
                }

                std::vector<VkPresentModeKHR> PresentModes(PresentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanPhysicalDevice, Surface, &PresentModeCount, PresentModes.data());
                if(PresentModes.empty() == true)
                {
                    continue;
                }

                SwapChainSupport = VK_TRUE;
            }

            if ((PresentSupport == VK_TRUE) && (SwapChainSupport == VK_TRUE))
            {
                IsValidQueue = true;
                break;
            }
        }
    }
    
    return IsValidQueue;
}
void VulkanDevice::CreateQueueFamilyInfo()
{
    UINT32 QueueFamilyCount = 0;
    std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
    {
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueFamilyCount, nullptr);

        QueueFamilyProperties.resize(QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueFamilyCount, QueueFamilyProperties.data());
    }

    bool IsFindGraphicsIndex = false;
    bool IsFindComputeIndex = false;
    bool IsFindTransferIndex = false;
    
    for (UINT32 Index = 0; Index < QueueFamilyCount; Index++)
    {
        const VkQueueFamilyProperties& QueueFamilyProperty = QueueFamilyProperties[Index];
        bool IsValidQueue = false;

        if ((QueueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
        {
            if (IsFindGraphicsIndex == false)
            {
                GraphicsQueueFamilyIndex = Index;
                IsFindGraphicsIndex = true;
                IsValidQueue = true;
            }
        }

        if ((QueueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
        {
            if (IsFindComputeIndex == false)
            {
                ComputeQueueFamilyIndex = Index;
                IsFindComputeIndex = true;
                IsValidQueue = true;
            }
        }

        if ((QueueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
        {
            if (IsFindTransferIndex == false)
            {
                TransferQueueFamilyIndex = Index;
                IsFindTransferIndex = true;
                IsValidQueue = true;
            }
        }

        if (IsValidQueue == false)
        {
            continue;
        }

        VkDeviceQueueCreateInfo DeviceQueueCreateInfo;
        ZeroVulkanStruct(DeviceQueueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
        DeviceQueueCreateInfo.queueFamilyIndex = Index;
        DeviceQueueCreateInfo.queueCount = QueueFamilyProperty.queueCount;

        DeviceQueueCreateInfos.emplace_back(DeviceQueueCreateInfo);
    }

    assert(IsFindGraphicsIndex);
    ComputeQueueFamilyIndex = IsFindComputeIndex ? ComputeQueueFamilyIndex : GraphicsQueueFamilyIndex;
    TransferQueueFamilyIndex = IsFindTransferIndex ? TransferQueueFamilyIndex : GraphicsQueueFamilyIndex;

    UINT32 NumPriorities = 0;
    for (VkDeviceQueueCreateInfo& DeviceQueueCreateInfo : DeviceQueueCreateInfos)
    {
        NumPriorities += DeviceQueueCreateInfo.queueCount;
    }

    float* QueuePriorities = new float[NumPriorities] { 0.0f };
    UINT32 CurrentQueuePriorityIndex = 0;
    for (VkDeviceQueueCreateInfo& DeviceQueueCreateInfo : DeviceQueueCreateInfos)
    {
        DeviceQueueCreateInfo.pQueuePriorities = &QueuePriorities[CurrentQueuePriorityIndex];

        for (UINT32 QueueIndex = 0; QueueIndex < DeviceQueueCreateInfo.queueCount; QueueIndex++)
        {
            QueuePriorities[QueueIndex] = 1.0f;
        }
    }
}
