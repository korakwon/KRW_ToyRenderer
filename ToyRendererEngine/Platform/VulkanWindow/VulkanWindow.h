#pragma once
#include "RHI/Vulkan/VulkanCommon.h"
#include "../BaseWindow.h"

namespace Platform
{
    class VulkanWindow : public BaseWindow
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(VulkanWindow)
        
    public:
        VulkanWindow(std::wstring PlatformTitle, const UINT32& PlatformWidth, const UINT32& PlatformHeight);
        virtual ~VulkanWindow() final;

    public:
        static void GetPlatformInstanceExtension(std::vector<const char*>& OutputExtensionNames);
    };
}
