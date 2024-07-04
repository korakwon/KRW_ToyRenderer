#include "VulkanStaticMesh.h"
#include "Core/Engine.h"
#include "RHI/Vulkan/VulkanRHI.h"
#include "RHI/Vulkan/Buffer/VulkanBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanIndexBuffer.h"
#include "RHI/Vulkan/Buffer/VulkanCommandBuffer.h"

using RHI::VulkanBuffer;
using RHI::VulkanVertexBuffer;
using RHI::VulkanIndexBuffer;
using RHI::VulkanCommandBuffer;
using RHI::VulkanStaticMesh;

VulkanStaticMesh::VulkanStaticMesh(const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize, const bool& IsCpuAccessible)
{
    VertexBuffer = RHI::CreateVertexBuffer(Vertices, VertexCount, VertexSize, IsCpuAccessible);
}
VulkanStaticMesh::VulkanStaticMesh(
    const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize,
    const void* Indices, const UINT32& IndexCount, const UINT32& IndexSize, const VkIndexType& IndexType,
    const bool& IsCpuAccessible)
{
    VertexBuffer = RHI::CreateVertexBuffer(Vertices, VertexCount, VertexSize, IsCpuAccessible);
    IndexBuffer = RHI::CreateIndexBuffer(Indices, IndexCount, IndexSize, IndexType, IsCpuAccessible);
    IndexBuffer->SetIndexType(IndexType);
}
VulkanStaticMesh::~VulkanStaticMesh()
{
    if (VertexBuffer != nullptr)
    {
        VertexBuffer->Destroy();
        VertexBuffer = nullptr;
    }

    if (IndexBuffer != nullptr)
    {
        IndexBuffer->Destroy();
        IndexBuffer = nullptr;
    }
}

void VulkanStaticMesh::BindFrame(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const
{
    ASSERT_NOT_NULL(VertexBuffer);

    if (VertexBuffer != nullptr)
    {
        VertexBuffer->BindBuffer(CommandBuffer);
    }

    if (IndexBuffer != nullptr)
    {
        IndexBuffer->BindBuffer(CommandBuffer);
    }
}
void VulkanStaticMesh::DrawFrame(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer) const
{
    UINT32 VertexCount = 0;
    UINT32 IndexCount = 0;
    
    if(VertexBuffer != nullptr)
    {
        VertexCount = VertexBuffer->GetVertexCount();
    }
    if(IndexBuffer != nullptr)
    {
        IndexCount = IndexBuffer->GetIndexCount();
    }
    
    DrawFrame(CommandBuffer, VertexCount, 0, IndexCount, 0);
}
void VulkanStaticMesh::DrawFrame(const std::shared_ptr<RHI::VulkanCommandBuffer>& CommandBuffer,
    const UINT32& DrawVertexCount, const UINT32& VertexOffset,
    const UINT32& DrawIndexCount, const UINT32& IndexOffset) const
{
    ASSERT_NOT_NULL(VertexBuffer);
    ASSERT_NOT_NULL(CommandBuffer);

    if (IndexBuffer == nullptr)
    {
        if (VertexBuffer != nullptr)
        {
            VertexBuffer->BeginDrawBuffer(CommandBuffer, DrawVertexCount, VertexOffset, 0);
        }
    }
    else
    {
        IndexBuffer->BeginDrawBuffer(CommandBuffer, DrawIndexCount, IndexOffset, VertexOffset);
    }
}

void VulkanStaticMesh::UpdateBuffer(const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize, const UINT32& VertexOffset) const
{
    UpdateBuffer(Vertices, VertexCount, VertexSize, VertexOffset, nullptr, 0, 0, 0);
}
void VulkanStaticMesh::UpdateBuffer(
    const void* Vertices, const UINT32& VertexCount, const UINT32& VertexSize, const UINT32& VertexOffset,
    const void* Indices, const UINT32& IndexCount, const UINT32& IndexSize, const UINT32& IndexOffset) const
{
    if(VertexBuffer != nullptr)
    {
        VertexBuffer->UpdateBuffer(Vertices, static_cast<UINT32>(VertexCount * VertexSize), VertexOffset);
    }
    if(IndexBuffer != nullptr)
    {
        IndexBuffer->UpdateBuffer(Indices, static_cast<UINT32>(IndexCount * IndexSize), IndexOffset);
    }
}
