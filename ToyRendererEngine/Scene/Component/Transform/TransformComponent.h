#pragma once
#include "pch.h"
#include "Scene/Component/Component.h"

// ********************** Forward Declarations **********************

// ***************************** Scene ******************************
namespace Scene
{
    class TransformComponent : public Component
    {
        DISABLE_COPY_CLASS(TransformComponent)

    public:
        TransformComponent();
        TransformComponent(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale);
        virtual ~TransformComponent() override = default;

    public:
        virtual void Update(const float& DeltaTime) override;

    public:
        static void Scaling(Matrix& Matrix, const Vector3& Scale);
        static void RotationYawPitchRoll(Matrix& Matrix, const Vector3& Rotate, const bool& IsRotateWorldAxis = false);
        static void Translation(Matrix& Matrix, const Vector3& Position);

    public:
        static Vector3 Forward(const Matrix& Matrix, bool IsNormalize = true);
        static Vector3 Up(const Matrix& Matrix, bool IsNormalize = true);
        static Vector3 Right(const Matrix& Matrix, bool IsNormalize = true);

    public:
        Vector3 Forward(bool IsNormalize = true) const;
        Vector3 Up(bool IsNormalize = true) const;
        Vector3 Right(bool IsNormalize = true) const;

    public:
        void SetRotateWorldAxis(const bool& State) { IsRotateWorldAxis = State; }

    public:
        void SetPosition(float X, float Y, float Z);
        void SetPosition(const Vector3& TargetPosition);
        Vector3 GetPosition() const { return Position; }

    public:
        void SetRotation(float X, float Y, float Z);
        void SetRotation(const Vector3& TargetRotation);
        Vector3 GetRotation() const { return Rotation; }

        void SetRotationEuler(float X, float Y, float Z);
        void SetRotationEuler(const Vector3& TargetRotation);
        Vector3 GetRotationEuler() const { return glm::radians(Rotation); }

    public:
        void SetScale(float X, float Y, float Z);
        void SetScale(const Vector3& TargetScale);
        Vector3 GetScale() const { return Scale; }

    public:
        void SetWorldMatrix(const Matrix& TargetMatrix);
        Matrix GetWorldMatrix() const { return WorldMatrix; }

    private:
        void UpdateWorld();

    private:
        bool IsRotateWorldAxis;

    private:
        Vector3 Position;
        Vector3 Rotation;
        Vector3 Scale;

    private:
        Matrix WorldMatrix;
    };
}
