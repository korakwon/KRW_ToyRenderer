#pragma once
#include "RHI/Vulkan/VulkanCommon.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanCommandBuffer;
}

// ***************************** Scene ******************************
namespace RHI
{
    class VulkanTexture
    {
    public:
        VulkanTexture() = default;
        ~VulkanTexture();

    public:
        void Destroy();

    public:
        void CreateImage(const UINT32& ImageWidth,
                         const UINT32& ImageHeight,
                         const UINT32& MipLevels,
                         const UINT32& LayerCount,
                         VkFormat TextureFormat,
                         VkImageTiling Tiling,
                         VkImageViewType ImageViewType,
                         VkImageAspectFlags AspectFlags,
                         VkImageUsageFlags ImageUsageFlags,
                         VkMemoryPropertyFlags MemoryPropertyFlags,
                         int ImageCreateFlag = 0);

    public:
        void SetImage(VkImage TargetImage, VkFormat TextureFormat, VkImageAspectFlags AspectFlags, bool IsSelfDestroyImage = true);

    public:
        VkFormat GetFormat() const { return Format; }

    public:
        VkImage* GetImagePtr() { return &Image; }
        VkImage GetImage() const { return Image; }
        VkImageView* GetImageViewPtr() { return &ImageView; }
        VkImageView GetImageView() const { return ImageView; }

    public:
        VkDescriptorImageInfo* GetDescriptorImageInfo() { return &DescriptorImageInfo; }

    private:
        void CreateViewAndSampler(VkImageViewType ImageViewType, VkImageAspectFlags AspectFlags, VkMemoryPropertyFlags MemoryPropertyFlags);

    private:
        UINT32 Width = 0;
        UINT32 Height = 0;
        UINT32 MaxMipLevel = 1;
        UINT32 MaxLayer = 1;
        VkFormat Format = VK_FORMAT_UNDEFINED;

    private:
        VkImage Image = VK_NULL_HANDLE;
        VkImageView ImageView = VK_NULL_HANDLE;
        VkSampler Sampler = VK_NULL_HANDLE;
        VkDeviceMemory ImageMemory = VK_NULL_HANDLE;

    private:
        VkDescriptorImageInfo DescriptorImageInfo;

    private:
        bool IsSelfDestroyImage = true;
    };

    std::shared_ptr<VulkanTexture> Create2DTexture(const void* TextureData, const UINT32& Width, const UINT32& Height, const UINT32& Channels = 4, const UINT32& MipLevels = 1, const ETextureFormatType& Format = ETextureFormatType::R8G8B8A8);
    std::shared_ptr<VulkanTexture> Create2DTexture(const std::string& FileName, const UINT32& MipLevels, const ETextureFormatType& Format = ETextureFormatType::R8G8B8A8);

    std::shared_ptr<VulkanTexture> CreateCubeMapTexture(const std::array<void*, 6>& DataArray, const UINT32& Width, const UINT32& Height, const UINT32& Channels = 4, const UINT32& MipLevels = 1, const ETextureFormatType& Format = ETextureFormatType::R8G8B8A8);
    std::shared_ptr<VulkanTexture> CreateCubeMapTexture(const std::array<std::string, 6>& FileNames, const UINT32& MipLevesl, const ETextureFormatType& Format = ETextureFormatType::R8G8B8A8);
}
