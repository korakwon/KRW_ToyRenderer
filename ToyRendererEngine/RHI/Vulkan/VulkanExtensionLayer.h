#pragma once
#include "VulkanCommon.h"

namespace RHI
{
    void GetInstanceExtensionLayer(std::vector<const char*>& OutExtensionNames, std::vector<const char*>& OutLayerNames);
    
    void GetDeviceExtensionLayer(const VkPhysicalDevice PhysicalDevice, std::vector<const char*>& OutExtensionNames, std::vector<const char*>& OutLayerNames);

}
