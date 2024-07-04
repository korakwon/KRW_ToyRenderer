#include "VulkanDebug.h"
#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"

#define VK_CREATE_DEBUG_REPORT_CALLBACK_EXT_NAME  "vkCreateDebugUtilsMessengerEXT"
#define VK_DESTROY_DEBUG_REPORT_CALLBACK_EXT_NAME "vkDestroyDebugUtilsMessengerEXT"

static VkDebugUtilsMessengerEXT G_DEBUG_MESSENGER = VK_NULL_HANDLE;

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance Instance,
    const VkDebugUtilsMessengerCreateInfoEXT* CreateInfo,
    const VkAllocationCallbacks* Allocator,
    VkDebugUtilsMessengerEXT* DebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, VK_CREATE_DEBUG_REPORT_CALLBACK_EXT_NAME);
    if (func != nullptr)
    {
        return func(Instance, CreateInfo, Allocator, DebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT MessageType,
    const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
    void* UserData)
{
    std::cerr << "[" << CallbackData->messageIdNumber << "][" << CallbackData->pMessageIdName << "]:[" << CallbackData->pMessage << "]" << std::endl;

    return VK_FALSE;
}

// ****************************** RHI *******************************
void RHI::CreateDebugMessenger()
{
#if AVAILABLE_VULKAN_DEBUG
    if(G_DEBUG_MESSENGER == VK_NULL_HANDLE)
    {
        VulkanRHI* RHI = Core::GEngine->GetRHI();
    
        VkDebugUtilsMessengerCreateInfoEXT DebugMessengerCreateInfo;
        ZeroVulkanStruct(DebugMessengerCreateInfo, VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT);
        DebugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        DebugMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        DebugMessengerCreateInfo.pfnUserCallback = DebugMessengerCallback;

        ASSERT_VK_RESULT(CreateDebugUtilsMessengerEXT(RHI->GetVulkanInstance(), &DebugMessengerCreateInfo, nullptr, &G_DEBUG_MESSENGER));
    }
#endif
}
void RHI::DestroyDebugMessenger()
{
#if AVAILABLE_VULKAN_DEBUG
    if(G_DEBUG_MESSENGER != VK_NULL_HANDLE)
    {
        VulkanRHI* RHI = Core::GEngine->GetRHI();
    
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(RHI->GetVulkanInstance(), VK_DESTROY_DEBUG_REPORT_CALLBACK_EXT_NAME);
        if (func != nullptr)
        {
            func(RHI->GetVulkanInstance(), G_DEBUG_MESSENGER, nullptr);
        }
        
        G_DEBUG_MESSENGER = VK_NULL_HANDLE;
    }
#endif
}
