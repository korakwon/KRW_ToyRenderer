#include "VulkanWindow.h"
#include "RHI/Vulkan/VulkanRHI.h"

using RHI::VulkanRHI;

// ****************************** EXTENSION ******************************
static std::vector<const char*> PLATFORM_INSTANCE_EXTENSION_NAMES =
{
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
};

// ****************************** RHI ************************************
void VulkanRHI::CreateVulkanSurface()
{
    VkWin32SurfaceCreateInfoKHR SurfaceCreateInfo;
    ZeroVulkanStruct(SurfaceCreateInfo, VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR);
    SurfaceCreateInfo.hinstance = Platform::G_HINSTANCE;
    SurfaceCreateInfo.hwnd = Platform::G_HWND;
    
    ASSERT_VK_RESULT(vkCreateWin32SurfaceKHR(Instance, &SurfaceCreateInfo, nullptr, &Surface));
}
void VulkanRHI::DestroyVulkanSurface()
{
    if(Surface != VK_NULL_HANDLE)
    {
        ASSERT_NOT_NULL(Instance);
        vkDestroySurfaceKHR(Instance, Surface, nullptr);
        Surface = VK_NULL_HANDLE;
    }
}

// ****************************** Platform *******************************
using Platform::VulkanWindow;

VulkanWindow::VulkanWindow(std::wstring PlatformTitle, const UINT32& PlatformWidth, const UINT32& PlatformHeight)
    : BaseWindow(PlatformTitle, PlatformWidth, PlatformHeight)
{
}
VulkanWindow::~VulkanWindow()
{
    Destroy();
}

void VulkanWindow::GetPlatformInstanceExtension(std::vector<const char*>& OutputExtensionNames)
{
    for(auto Extension : PLATFORM_INSTANCE_EXTENSION_NAMES)
    {
        OutputExtensionNames.emplace_back(Extension);
    }
}

