#pragma once
#include "pch.h"
#include "Scene/GameObject.h"

// ********************** Forward Declarations **********************

// ***************************** Scene ******************************
namespace Scene
{
    class CameraGameObject : public GameObject
    {
    public:
        CameraGameObject() = default;
        virtual ~CameraGameObject() override = default;

    public:
        virtual void Update(const float& DeltaTime) override;

    public:
        Matrix GetViewMatrix() const { return ViewMatrix; }
        Matrix GetProjectionMatrix() const { return ProjectionMatrix; }

    public:
        void SetFOV(const float& TargetFOV);
        void SetNear(const float& NearClip);
        void SetFar(const float& FarClip);

    protected:
        float FOV = 45.0f;
        float Near = 0.1f;
        float Far = 100.0f;

    protected:
        Matrix ViewMatrix = Matrix(1.0f);
        Matrix ProjectionMatrix = Matrix(1.0f);
    };
}
