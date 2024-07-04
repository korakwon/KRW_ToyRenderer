#pragma once
#include "RHI/Vulkan/VulkanCommon.h"
#include "Util/VertexIndexLayout.h"

// ********************** Forward Declarations **********************
namespace RHI
{
    class VulkanVertexBuffer;
    class VulkanIndexBuffer;
    class VulkanCommandBuffer;
}

// ***************************** Scene ******************************
namespace RHI
{
    class VulkanStaticMesh
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(VulkanStaticMesh)
        
    public:
        VulkanStaticMesh(
            const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize,
            const bool& IsCpuAccessible = false);
        VulkanStaticMesh(
            const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize,
            const void* Indices, const UINT32& IndexCount, const UINT32& IndexSize, const VkIndexType& IndexType = VK_INDEX_TYPE_UINT32,
            const bool& IsCpuAccessible = false);
        virtual ~VulkanStaticMesh();

    public:
        void BindFrame(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const;
        void DrawFrame(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const;
        void DrawFrame(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer, const UINT32& DrawVertexCount, const UINT32& VertexOffset, const UINT32& DrawIndexCount, const UINT32& IndexOffset) const;

    public:
        void UpdateBuffer(const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize, const UINT32& VertexOffset) const;
        void UpdateBuffer(
            const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize, const UINT32& VertexOffset,
            const void* Indices, const UINT32& IndexCount, const UINT32& IndexSize, const UINT32& IndexOffset) const;

    private:
        std::shared_ptr<RHI::VulkanVertexBuffer> VertexBuffer;
        std::shared_ptr<RHI::VulkanIndexBuffer> IndexBuffer;
    };
}