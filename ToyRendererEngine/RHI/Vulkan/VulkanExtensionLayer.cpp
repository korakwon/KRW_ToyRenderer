#include "VulkanExtensionLayer.h"
#include "Platform/VulkanWindow/VulkanWindow.h"

static std::vector<const char*> INSTANCE_EXTENSION_NAMES =
{
};
static std::vector<const char*> INSTANCE_LAYER_NAMES =
{
    "VK_LAYER_KHRONOS_validation"
};
static std::vector<const char*> DEVICE_EXTENSION_NAMES =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
};
static std::vector<const char*> DEVICE_LAYER_NAMES =
{
    "VK_LAYER_KHRONOS_validation"
};

bool IsContainName(const std::vector<const char*>& NameList, const char* CompareTargetName)
{
    for (const char* Element : NameList)
    {
        if (strcmp(Element, CompareTargetName) == 0)
        {
            return true;
        }
    }
    return false;
}

void RHI::GetInstanceExtensionLayer(std::vector<const char*>& OutExtensionNames, std::vector<const char*>& OutLayerNames)
{
    // Extensions
    UINT32 ExtensionCount = 0;
    std::vector<VkExtensionProperties> ExtensionProperties;
    std::vector<const char*> ExtensionNames;
    {
        vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);

        ExtensionProperties.resize(ExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, ExtensionProperties.data());

        for (const VkExtensionProperties& Property : ExtensionProperties)
        {
            ExtensionNames.emplace_back(Property.extensionName);
        }
    }

    // Layers
    UINT32 LayerCount;
    std::vector<VkLayerProperties> LayerProperties;
    std::vector<const char*> LayerNames;
    {
        vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

        LayerProperties.resize(LayerCount);
        vkEnumerateInstanceLayerProperties(&LayerCount, LayerProperties.data());

        for (const VkLayerProperties& Property : LayerProperties)
        {
            LayerNames.emplace_back(Property.layerName);
        }
    }

    // Result
    {
        std::vector<const char*> PlatformExtensionNames;
        Platform::VulkanWindow::GetPlatformInstanceExtension(PlatformExtensionNames);
        for (auto ExtensionName : PlatformExtensionNames)
        {
            if (IsContainName(OutExtensionNames, ExtensionName) == false)
            {
                OutExtensionNames.emplace_back(ExtensionName);
            }
        }

        for (const char* Name : INSTANCE_EXTENSION_NAMES)
        {
            if (IsContainName(ExtensionNames, Name) == true)
            {
                if (IsContainName(OutExtensionNames, Name) == false)
                {
                    OutExtensionNames.emplace_back(Name);
                }
            }
        }

#if AVAILABLE_VULKAN_DEBUG
        if (IsContainName(OutExtensionNames, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == false)
        {
            OutExtensionNames.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        for (const char* Name : INSTANCE_LAYER_NAMES)
        {
            if (IsContainName(LayerNames, Name) == true)
            {
                if (IsContainName(OutLayerNames, Name) == false)
                {
                    OutLayerNames.emplace_back(Name);
                }
            }
        }
#endif
    }
}

void RHI::GetDeviceExtensionLayer(const VkPhysicalDevice PhysicalDevice, std::vector<const char*>& OutExtensionNames, std::vector<const char*>& OutLayerNames)
{
    // Extensions
    UINT32 ExtensionCount = 0;
    std::vector<VkExtensionProperties> ExtensionProperties;
    std::vector<const char*> ExtensionNames;
    {
        vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, nullptr);

        ExtensionProperties.resize(ExtensionCount);
        vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &ExtensionCount, ExtensionProperties.data());

        for (const VkExtensionProperties& Property : ExtensionProperties)
        {
            ExtensionNames.emplace_back(Property.extensionName);
        }
    }

    // Layers
    UINT32 LayerCount;
    std::vector<VkLayerProperties> LayerProperties;
    std::vector<const char*> LayerNames;
    {
        vkEnumerateDeviceLayerProperties(PhysicalDevice, &LayerCount, nullptr);

        LayerProperties.resize(LayerCount);
        vkEnumerateDeviceLayerProperties(PhysicalDevice, &LayerCount, LayerProperties.data());

        for (const VkLayerProperties& Property : LayerProperties)
        {
            LayerNames.emplace_back(Property.layerName);
        }
    }

    // Result
    {
        for (const char* Name : DEVICE_EXTENSION_NAMES)
        {
            if (IsContainName(ExtensionNames, Name) == true)
            {
                if (IsContainName(OutExtensionNames, Name) == false)
                {
                    OutExtensionNames.emplace_back(Name);
                }
            }
        }

#if AVAILABLE_VULKAN_DEBUG
        for (const char* Name : DEVICE_LAYER_NAMES)
        {
            if (IsContainName(LayerNames, Name) == true)
            {
                if (IsContainName(OutLayerNames, Name) == false)
                {
                    OutLayerNames.emplace_back(Name);
                }
            }
        }
#endif
    }
}
