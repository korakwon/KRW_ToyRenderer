#pragma once

// **************************** Library ****************************
#pragma comment(lib, "vulkan-1.lib")

// **************************** Headers ****************************
#include "pch.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

// **************************** Defines ****************************
#if _DEBUG
#define AVAILABLE_VULKAN_DEBUG 1
#endif

#define ASSERT_VK_RESULT(Result) assert((Result) == VK_SUCCESS)

// *************************** Functions ***************************
template<class T>
static inline void ZeroVulkanStruct(T& VkStruct, VkStructureType VkType)
{
    VkStruct.sType = VkType;
    memset(reinterpret_cast<UINT8*>(&VkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
}