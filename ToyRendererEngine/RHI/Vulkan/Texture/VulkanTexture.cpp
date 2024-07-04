#include "VulkanTexture.h"
#include "RHI/Vulkan/Buffer/VulkanBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanCommandBuffer.h"

#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"

#define STB_IMAGE_IMPLEMENTATION
#include "std/stb_image.h"

using RHI::VulkanBuffer;
using RHI::VulkanTexture;

// ****************************** Global *****************************
VkFormat GetVulkanTextureFormat(const ETextureFormatType& Format)
{
    VkFormat Result = VK_FORMAT_R8G8B8A8_SRGB;

    switch (Format)
    {
    case ETextureFormatType::R8G8B8A8:
        Result = VK_FORMAT_R8G8B8A8_SRGB;
        break;
    case ETextureFormatType::R16G16B16A16:
        Result = VK_FORMAT_R16G16B16A16_SFLOAT;
        break;
    case ETextureFormatType::R32G32B32A32:
        Result = VK_FORMAT_R32G32B32A32_SFLOAT;
        break;
    }

    return Result;
}
UINT32 GetVulkanTextureFormatSize(const ETextureFormatType& Format)
{
    UINT Result = 1;

    switch (Format)
    {
    case ETextureFormatType::R8G8B8A8:
        Result = static_cast<UINT32>(sizeof(BYTE)) * 1;
        break;
    case ETextureFormatType::R16G16B16A16:
        Result = static_cast<UINT32>(sizeof(BYTE)) * 2;
        break;
    case ETextureFormatType::R32G32B32A32:
        Result = static_cast<UINT32>(sizeof(BYTE)) * 4;
        break;
    }

    return Result;
}

void* LoadTexture(const std::string& FileName, INT32& Width, INT32& Height, INT32& Channels, const ETextureFormatType& Format)
{
    void* Data = nullptr;

    switch (Format)
    {
    case ETextureFormatType::R8G8B8A8:
        Data = stbi_load(FileName.c_str(), &Width, &Height, &Channels, STBI_rgb_alpha);
        break;
    case ETextureFormatType::R16G16B16A16:
    case ETextureFormatType::R32G32B32A32:
        Data = stbi_loadf(FileName.c_str(), &Width, &Height, &Channels, STBI_rgb_alpha);
        break;
    }
    ASSERT_NOT_NULL(Data);

    return Data;
}

void TransitionImageLayout(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, VkImage Image, const UINT32& MipLevels, const UINT32& LayerCount, VkImageAspectFlags AspectFlag, VkImageLayout OldLayout, VkImageLayout NewLayout)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();

    RHI->BeginSingleTimeCommandSubmit(CommandBuffer);
    {
        VkImageMemoryBarrier ImageMemoryBarrier;
        {
            ZeroVulkanStruct(ImageMemoryBarrier, VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER);
            ImageMemoryBarrier.oldLayout = OldLayout;
            ImageMemoryBarrier.newLayout = NewLayout;
            ImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            ImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            ImageMemoryBarrier.image = Image;
            ImageMemoryBarrier.subresourceRange.aspectMask = AspectFlag;
            ImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
            ImageMemoryBarrier.subresourceRange.levelCount = MipLevels;
            ImageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
            ImageMemoryBarrier.subresourceRange.layerCount = LayerCount;
        }

        VkPipelineStageFlags SourceStage = (VkPipelineStageFlags)0;
        VkPipelineStageFlags DestinationStage = (VkPipelineStageFlags)0;

        if ((OldLayout == VK_IMAGE_LAYOUT_UNDEFINED) && (NewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL))
        {
            ImageMemoryBarrier.srcAccessMask = 0;
            ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            SourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            DestinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if ((OldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) && (NewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL))
        {
            ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            SourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            DestinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            ASSERT(false);
        }

        vkCmdPipelineBarrier(CommandBuffer->GetBuffer(),
                             SourceStage,
                             DestinationStage,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &ImageMemoryBarrier);
    }
    RHI->EndSingleTimeCommandSubmit(CommandBuffer);
}
void CopyBufferToImage(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, VkBuffer SrcBuffer, VkImage DstImage, const UINT32& Width, const UINT32& Height, const UINT32& LayerCount, VkImageAspectFlags AspectFlag)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();

    RHI->BeginSingleTimeCommandSubmit(CommandBuffer);
    {
        std::vector<VkBufferImageCopy> BufferImageCopyRegions;
        //for (UINT32 i = 0; i < 1; i++)
        {
            VkBufferImageCopy CopyRegion = {};
            {
                CopyRegion.bufferOffset = 0;
                CopyRegion.bufferRowLength = 0;
                CopyRegion.bufferImageHeight = 0;
                CopyRegion.imageSubresource.aspectMask = AspectFlag;
                CopyRegion.imageSubresource.mipLevel = 0;
                CopyRegion.imageSubresource.baseArrayLayer = 0;
                CopyRegion.imageSubresource.layerCount = LayerCount;
                CopyRegion.imageOffset = {0, 0, 0};
                CopyRegion.imageExtent = {Width, Height, 1};
            }

            BufferImageCopyRegions.push_back(CopyRegion);
        }

        vkCmdCopyBufferToImage(CommandBuffer->GetBuffer(),
                               SrcBuffer,
                               DstImage,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               static_cast<UINT32>(BufferImageCopyRegions.size()),
                               BufferImageCopyRegions.data());
    }
    RHI->EndSingleTimeCommandSubmit(CommandBuffer);
}
void GenerateMipmaps(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, VkImage Image, const UINT32& Width, const UINT32& Height, const UINT32& MipLevels, const UINT32& LayerCount, VkImageAspectFlags AspectFlag)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();

    RHI->BeginSingleTimeCommandSubmit(CommandBuffer);
    {
        // Init Transfer ImageLayout
        VkImageMemoryBarrier ImageMemoryBarrier;
        {
            ZeroVulkanStruct(ImageMemoryBarrier, VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER);
            ImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            ImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            ImageMemoryBarrier.image = Image;
            ImageMemoryBarrier.subresourceRange.aspectMask = AspectFlag;
            ImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
            ImageMemoryBarrier.subresourceRange.levelCount = 1;
            ImageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
            ImageMemoryBarrier.subresourceRange.layerCount = LayerCount; //1;
        }

        for (UINT32 i = 0; i < MipLevels - 1; i++)
        {
            // Set Transfer Info
            {
                ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                ImageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                ImageMemoryBarrier.subresourceRange.baseMipLevel = i;

                vkCmdPipelineBarrier(CommandBuffer->GetBuffer(),
                                     VK_PIPELINE_STAGE_TRANSFER_BIT,
                                     VK_PIPELINE_STAGE_TRANSFER_BIT,
                                     0,
                                     0,
                                     nullptr,
                                     0,
                                     nullptr,
                                     1,
                                     &ImageMemoryBarrier);
            }

            // Image Bit Blit (Copy)
            {
                INT32 SrcWidth = std::max(1, static_cast<INT32>(Width * std::pow(0.5f, i)));
                INT32 SrcHeight = std::max(1, static_cast<INT32>(Height * std::pow(0.5f, i)));
                INT32 DstWidth = std::max(1, static_cast<INT32>(Width * std::pow(0.5f, i + 1)));
                INT32 DstHeight = std::max(1, static_cast<INT32>(Height * std::pow(0.5f, i + 1)));

                VkImageBlit ImageBlit = {};
                {
                    ImageBlit.srcSubresource.aspectMask = AspectFlag;
                    ImageBlit.srcSubresource.mipLevel = i;
                    ImageBlit.srcSubresource.baseArrayLayer = 0;
                    ImageBlit.srcSubresource.layerCount = LayerCount;
                    ImageBlit.srcOffsets[0] = {0, 0, 0};
                    ImageBlit.srcOffsets[1] = {SrcWidth, SrcHeight, 1};

                    ImageBlit.dstSubresource.aspectMask = AspectFlag;
                    ImageBlit.dstSubresource.mipLevel = i + 1;
                    ImageBlit.dstSubresource.baseArrayLayer = 0;
                    ImageBlit.dstSubresource.layerCount = LayerCount;
                    ImageBlit.dstOffsets[0] = {0, 0, 0};
                    ImageBlit.dstOffsets[1] = {DstWidth, DstHeight, 1};
                }

                vkCmdBlitImage(CommandBuffer->GetBuffer(),
                               Image,
                               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               Image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &ImageBlit,
                               VK_FILTER_LINEAR);
            }

            // Release Transfer Info 
            {
                ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                vkCmdPipelineBarrier(CommandBuffer->GetBuffer(),
                                     VK_PIPELINE_STAGE_TRANSFER_BIT,
                                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                     0,
                                     0,
                                     nullptr,
                                     0,
                                     nullptr,
                                     1,
                                     &ImageMemoryBarrier);
            }
        }

        ImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        ImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        ImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        ImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        ImageMemoryBarrier.subresourceRange.baseMipLevel = MipLevels - 1;

        vkCmdPipelineBarrier(
            CommandBuffer->GetBuffer(),
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &ImageMemoryBarrier);
    }
    RHI->EndSingleTimeCommandSubmit(CommandBuffer);
}

// ****************************** Global *****************************
void CreateImageView(VkImage Image, const UINT32& MipLevels, const UINT32& LayerCount, VkFormat Format, VkImageViewType ImageType, VkImageAspectFlags AspectFlag, VkImageView* OutputImageView)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    VkImageViewCreateInfo ImageViewCreateInfo;
    {
        ZeroVulkanStruct(ImageViewCreateInfo, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
        ImageViewCreateInfo.image = Image;
        ImageViewCreateInfo.viewType = ImageType;
        ImageViewCreateInfo.format = Format;
        ImageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
        ImageViewCreateInfo.subresourceRange.aspectMask = AspectFlag;
        ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        ImageViewCreateInfo.subresourceRange.levelCount = MipLevels;
        ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        ImageViewCreateInfo.subresourceRange.layerCount = LayerCount;
    }

    ASSERT_VK_RESULT(vkCreateImageView(Device, &ImageViewCreateInfo, nullptr, OutputImageView));
}
void CreateTextureSampler(const UINT32& MipLevels, VkSampler* OutputSampler)
{
    RHI::VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    VkSamplerCreateInfo SamplerCreateInfo;
    {
        ZeroVulkanStruct(SamplerCreateInfo, VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO);
        SamplerCreateInfo.magFilter = VK_FILTER_LINEAR;
        SamplerCreateInfo.minFilter = VK_FILTER_LINEAR;
        SamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        SamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        SamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        SamplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        SamplerCreateInfo.mipLodBias = 0.0f;
        SamplerCreateInfo.anisotropyEnable = VK_FALSE;
        SamplerCreateInfo.maxAnisotropy = RHI->GetVulkanDevice()->GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
        SamplerCreateInfo.compareEnable = VK_FALSE;
        SamplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        SamplerCreateInfo.minLod = 0.0f;
        SamplerCreateInfo.maxLod = static_cast<float>(MipLevels);
        SamplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        SamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    }

    ASSERT_VK_RESULT(vkCreateSampler(Device, &SamplerCreateInfo, nullptr, OutputSampler));
}

// ****************************** VulkanTexture *****************************
VulkanTexture::~VulkanTexture()
{
    Destroy();
}

void VulkanTexture::Destroy()
{
    VkDevice Device = Core::GEngine->GetRHI()->GetLogicalDevice();

    if (Sampler != VK_NULL_HANDLE)
    {
        vkDestroySampler(Device, Sampler, nullptr);
        Sampler = VK_NULL_HANDLE;
    }
    if (ImageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(Device, ImageView, nullptr);
        ImageView = VK_NULL_HANDLE;
    }
    if (Image != VK_NULL_HANDLE)
    {
        if (IsSelfDestroyImage == true)
        {
            vkDestroyImage(Device, Image, nullptr);
            Image = VK_NULL_HANDLE;
        }
    }
    if (ImageMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Device, ImageMemory, nullptr);
        ImageMemory = VK_NULL_HANDLE;
    }
}

void VulkanTexture::CreateImage(const UINT32& ImageWidth,
                                const UINT32& ImageHeight,
                                const UINT32& MipLevels,
                                const UINT32& LayerCount,
                                VkFormat TextureFormat,
                                VkImageTiling Tiling,
                                VkImageViewType ImageViewType,
                                VkImageAspectFlags AspectFlags,
                                VkImageUsageFlags ImageUsageFlags,
                                VkMemoryPropertyFlags MemoryPropertyFlags,
                                int ImageCreateFlag)
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    Width = ImageWidth;
    Height = ImageHeight;
    MaxMipLevel = MipLevels;
    MaxLayer = LayerCount;
    Format = TextureFormat;

    VkImageCreateInfo ImageCreateInfo;
    {
        ZeroVulkanStruct(ImageCreateInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
        ImageCreateInfo.flags = ImageCreateFlag;
        ImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        ImageCreateInfo.format = Format;
        ImageCreateInfo.extent = {Width, Height, 1};
        ImageCreateInfo.mipLevels = MaxMipLevel;
        ImageCreateInfo.arrayLayers = LayerCount;
        ImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        ImageCreateInfo.tiling = Tiling;
        ImageCreateInfo.usage = ImageUsageFlags;
        ImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        ImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }
    ASSERT_VK_RESULT(vkCreateImage(Device, &ImageCreateInfo, nullptr, &Image));

    CreateViewAndSampler(ImageViewType, AspectFlags, MemoryPropertyFlags);
}

void VulkanTexture::SetImage(VkImage TargetImage, VkFormat TextureFormat, VkImageAspectFlags AspectFlags, bool IsSelfDestroyImage)
{
    Destroy();

    this->IsSelfDestroyImage = IsSelfDestroyImage;

    Format = TextureFormat;

    Image = TargetImage;

    CreateImageView(Image, MaxMipLevel, MaxLayer, Format, VK_IMAGE_VIEW_TYPE_2D, AspectFlags, &ImageView);
    CreateTextureSampler(MaxMipLevel, &Sampler);

    // DescriptorImageInfo
    {
        DescriptorImageInfo.sampler = Sampler;
        DescriptorImageInfo.imageView = ImageView;
        DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
}

void VulkanTexture::CreateViewAndSampler(VkImageViewType ImageViewType, VkImageAspectFlags AspectFlags, VkMemoryPropertyFlags MemoryPropertyFlags)
{
    VulkanRHI* RHI = Core::GEngine->GetRHI();
    VkDevice Device = RHI->GetLogicalDevice();

    VkMemoryRequirements MemoryRequirements;
    VkMemoryAllocateInfo MemoryAllocateInfo;
    {
        vkGetImageMemoryRequirements(Device, Image, &MemoryRequirements);

        ZeroVulkanStruct(MemoryAllocateInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
        MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
        MemoryAllocateInfo.memoryTypeIndex = RHI->GetVulkanDevice()->FindMemoryType(MemoryRequirements.memoryTypeBits, MemoryPropertyFlags);

        ASSERT_VK_RESULT(vkAllocateMemory(Device, &MemoryAllocateInfo, nullptr, &ImageMemory));
        ASSERT_VK_RESULT(vkBindImageMemory(Device, Image, ImageMemory, 0));
    }

    CreateImageView(Image, MaxMipLevel, MaxLayer, Format, ImageViewType, AspectFlags, &ImageView);
    CreateTextureSampler(MaxMipLevel, &Sampler);

    // DescriptorImageInfo
    {
        DescriptorImageInfo.sampler = Sampler;
        DescriptorImageInfo.imageView = ImageView;
        DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
}

// ****************************** CreateTexture *****************************
std::shared_ptr<VulkanTexture> RHI::Create2DTexture(const void* TextureData, const UINT32& Width, const UINT32& Height, const UINT32& Channels, const UINT32& MipLevels, const ETextureFormatType& Format)
{
    VkFormat TextureFormat = GetVulkanTextureFormat(Format);
    UINT32 TextureSize = Width * Height * 4 * GetVulkanTextureFormatSize(Format);
    VkImageAspectFlags AspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;

    std::shared_ptr<VulkanBuffer> StagingBuffer = RHI::CreateBuffer(TextureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    StagingBuffer->UpdateBuffer(TextureData, TextureSize);

    std::shared_ptr<VulkanTexture> Texture = std::make_shared<VulkanTexture>();
    Texture->CreateImage(Width,
                         Height,
                         MipLevels,
                         1,
                         TextureFormat,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_VIEW_TYPE_2D,
                         AspectFlag,
                         VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    std::shared_ptr<VulkanCommandBuffer> CommandBuffer = RHI::CreateCommandBuffer();

    TransitionImageLayout(CommandBuffer, Texture->GetImage(), MipLevels, 1, AspectFlag, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(CommandBuffer, StagingBuffer->GetBuffer(), Texture->GetImage(), Width, Height, 1, AspectFlag);
    GenerateMipmaps(CommandBuffer, Texture->GetImage(), Width, Height, MipLevels, 1, AspectFlag);
    //TransitionImageLayout(CommandBuffer, Texture->GetImage(), MipLevels, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    CommandBuffer->Destroy();
    StagingBuffer->Destroy();
    CommandBuffer = nullptr;
    StagingBuffer = nullptr;

    return Texture;
}
std::shared_ptr<VulkanTexture> RHI::Create2DTexture(const std::string& FileName, const UINT32& MipLevels, const ETextureFormatType& Format)
{
    int Width = 0, Height = 0, Channels = 0;

    void* Data = LoadTexture(FileName, Width, Height, Channels, Format);

    std::shared_ptr<VulkanTexture> Texture = Create2DTexture(Data, Width, Height, Channels, MipLevels, Format);

    stbi_image_free(Data);

    return Texture;
}

std::shared_ptr<VulkanTexture> RHI::CreateCubeMapTexture(const std::array<void*, 6>& DataArray, const UINT32& Width, const UINT32& Height, const UINT32& Channels, const UINT32& MipLevels, const ETextureFormatType& Format)
{
    VkFormat TextureFormat = GetVulkanTextureFormat(Format);
    UINT32 LayerCount = static_cast<UINT32>(DataArray.size());
    UINT32 UnitTextureSize = Width * Height * 4 * GetVulkanTextureFormatSize(Format);
    UINT32 TextureSize = UnitTextureSize * LayerCount;
    VkImageAspectFlags AspectFlag = VK_IMAGE_ASPECT_COLOR_BIT;

    std::shared_ptr<VulkanBuffer> StagingBuffer = RHI::CreateBuffer(TextureSize,
                                                                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    for (UINT32 i = 0; i < DataArray.size(); i++)
    {
        StagingBuffer->UpdateBuffer(DataArray[i], UnitTextureSize, UnitTextureSize * i);
    }

    std::shared_ptr<VulkanTexture> Texture = std::make_shared<VulkanTexture>();
    Texture->CreateImage(Width,
                         Height,
                         MipLevels,
                         LayerCount,
                         TextureFormat,
                         VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_VIEW_TYPE_CUBE,
                         AspectFlag,
                         VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                         VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);

    std::shared_ptr<VulkanCommandBuffer> CommandBuffer = RHI::CreateCommandBuffer();

    //image관련 struct만들어서 써야하네
    TransitionImageLayout(CommandBuffer, Texture->GetImage(), MipLevels, LayerCount, AspectFlag, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(CommandBuffer, StagingBuffer->GetBuffer(), Texture->GetImage(), Width, Height, LayerCount, AspectFlag);
    GenerateMipmaps(CommandBuffer, Texture->GetImage(), Width, Height, MipLevels, LayerCount, AspectFlag);
    //TransitionImageLayout(CommandBuffer, Texture->GetImage(), MipLevels, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    CommandBuffer->Destroy();
    StagingBuffer->Destroy();
    CommandBuffer = nullptr;
    StagingBuffer = nullptr;

    return Texture;
}
std::shared_ptr<VulkanTexture> RHI::CreateCubeMapTexture(const std::array<std::string, 6>& FileNames, const UINT32& MipLevels, const ETextureFormatType& Format)
{
    for (const std::string& FileName : FileNames)
    {
        ASSERT(FileName.empty() == false);
    }

    std::array<void*, 6> DataArray;
    int Width = 0, Height = 0, Channels = 0;

    for (UINT32 i = 0; i < FileNames.size(); i++)
    {
        void* Data = LoadTexture(FileNames[i], Width, Height, Channels, Format);
        DataArray[i] = Data;
    }

    std::shared_ptr<VulkanTexture> Texture = CreateCubeMapTexture(DataArray, Width, Height, Channels, MipLevels, Format);

    for (void* Data : DataArray)
    {
        stbi_image_free(Data);
    }

    return Texture;
}
