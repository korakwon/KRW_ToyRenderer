#include "VulkanRHI.h"
#include "VulkanExtensionLayer.h"

using RHI::VulkanRHI;

void VulkanRHI::CreateVulkanInstance()
{
    std::vector<const char*> InstanceExtensionNames;
    std::vector<const char*> InstanceLayerNames;
    
    GetInstanceExtensionLayer(InstanceExtensionNames, InstanceLayerNames);

    VkApplicationInfo ApplicationInfo;
    ZeroVulkanStruct(ApplicationInfo, VK_STRUCTURE_TYPE_APPLICATION_INFO);
    ApplicationInfo.pApplicationName = "Engine::Get()->GetTitle()";
    ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    ApplicationInfo.pEngineName = "ENGINE_NAME";
    ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    ApplicationInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo InstanceCreateInfo;
    ZeroVulkanStruct(InstanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);
    InstanceCreateInfo.pApplicationInfo = &ApplicationInfo;
    InstanceCreateInfo.enabledExtensionCount = static_cast<UINT32>(InstanceExtensionNames.size());
    InstanceCreateInfo.ppEnabledExtensionNames = (InstanceCreateInfo.enabledExtensionCount > 0) ? InstanceExtensionNames.data() : nullptr;
    InstanceCreateInfo.enabledLayerCount = static_cast<UINT32>(InstanceLayerNames.size());
    InstanceCreateInfo.ppEnabledLayerNames = (InstanceCreateInfo.enabledLayerCount > 0) ? InstanceLayerNames.data() : nullptr;
    
    ASSERT_VK_RESULT(vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance));
}

void VulkanRHI::DestroyVulkanInstance()
{
    if(Instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(Instance, nullptr);
        Instance = VK_NULL_HANDLE;    
    }
}