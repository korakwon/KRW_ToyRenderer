#include "TransformComponent.h"

using Scene::TransformComponent;

TransformComponent::TransformComponent()
    : Component("Transform"),
      IsRotateWorldAxis(false),
      Position(0.0f), Rotation(0.0f), Scale(1.0f),
      WorldMatrix(1.0f)
{
    UpdateWorld();
}
TransformComponent::TransformComponent(const Vector3& Position, const Vector3& Rotation, const Vector3& Scale)
    : Component("Transform"),
      IsRotateWorldAxis(false),
      Position(Position), Rotation(Rotation), Scale(Scale),
      WorldMatrix(1.0f)
{
    UpdateWorld();
}

void TransformComponent::Update(const float& DeltaTime)
{
    UpdateWorld();
}

void TransformComponent::Scaling(Matrix& Matrix, const Vector3& Scale)
{
    Matrix = glm::scale(Matrix, Scale);
}
void TransformComponent::RotationYawPitchRoll(Matrix& Matrix, const Vector3& Rotate, const bool& IsRotateWorldAxis)
{
    Vector3 UpVector = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 RightVector = Vector3(1.0f, 0.0f, 0.0f);
    Vector3 ForwardVector = Vector3(0.0f, 0.0f, 1.0f);

    if (IsRotateWorldAxis == true)
    {
        Matrix = glm::rotate(Matrix, glm::radians(Rotate.y), UpVector);
        Matrix = glm::rotate(Matrix, glm::radians(Rotate.x), RightVector);
        Matrix = glm::rotate(Matrix, glm::radians(Rotate.z), ForwardVector);
    }
    else
    {
        Matrix = glm::rotate(Matrix, glm::radians(Rotate.y), UpVector);
        RightVector = Right(Matrix);
        Matrix = glm::rotate(Matrix, glm::radians(Rotate.x), RightVector);
        ForwardVector = Forward(Matrix);
        Matrix = glm::rotate(Matrix, glm::radians(Rotate.z), ForwardVector);
    }
}
void TransformComponent::Translation(Matrix& Matrix, const Vector3& Position)
{
    Matrix = glm::translate(Matrix, Position);
}

Vector3 TransformComponent::Forward(const Matrix& Matrix, bool IsNormalize)
{
    Vector3 Direction = {Matrix[2][0], Matrix[2][1], Matrix[2][2]};
    return (IsNormalize == true) ? glm::normalize(Direction) : Direction;
}
Vector3 TransformComponent::Up(const Matrix& Matrix, bool IsNormalize)
{
    Vector3 Direction = {Matrix[1][0], Matrix[1][1], Matrix[1][2]};
    return (IsNormalize == true) ? glm::normalize(Direction) : Direction;
}
Vector3 TransformComponent::Right(const Matrix& Matrix, bool IsNormalize)
{
    Vector3 Direction = {Matrix[0][0], Matrix[0][1], Matrix[0][2]};
    return (IsNormalize == true) ? glm::normalize(Direction) : Direction;
}

Vector3 TransformComponent::Forward(bool IsNormalize) const
{
    return Forward(WorldMatrix, IsNormalize);
}
Vector3 TransformComponent::Up(bool IsNormalize) const
{
    return Up(WorldMatrix, IsNormalize);
}
Vector3 TransformComponent::Right(bool IsNormalize) const
{
    return Right(WorldMatrix, IsNormalize);
}

void TransformComponent::SetPosition(float X, float Y, float Z)
{
    SetPosition(Vector3(X, Y, Z));
}
void TransformComponent::SetPosition(const Vector3& TargetPosition)
{
    Position = TargetPosition;
    UpdateWorld();
}

void TransformComponent::SetRotation(float X, float Y, float Z)
{
    SetRotation(Vector3(X, Y, Z));
}
void TransformComponent::SetRotation(const Vector3& TargetRotation)
{
    Rotation = TargetRotation;
    UpdateWorld();
}
void TransformComponent::SetRotationEuler(float X, float Y, float Z)
{
    SetRotationEuler(Vector3(X, Y, Z));
}
void TransformComponent::SetRotationEuler(const Vector3& TargetRotation)
{
    SetRotation(glm::degrees(TargetRotation));
}

void TransformComponent::SetScale(float X, float Y, float Z)
{
    SetScale(Vector3(X, Y, Z));
}
void TransformComponent::SetScale(const Vector3& TargetScale)
{
    Scale = TargetScale;
    UpdateWorld();
}

void TransformComponent::SetWorldMatrix(const Matrix& TargetMatrix)
{
    Vector3 TempScale;
    Quaternion TempRotation;
    Vector3 TempTranslation;
    Vector3 TempSkew;
    Vector4 TempPerspective;

    glm::decompose(TargetMatrix, TempScale, TempRotation, TempTranslation, TempSkew, TempPerspective);

    Position = TempTranslation;
    Rotation = glm::eulerAngles(TempRotation);
    Scale = TempScale;

    WorldMatrix = TargetMatrix;
}

void TransformComponent::UpdateWorld()
{
    WorldMatrix = Matrix(1.0f);

    Translation(WorldMatrix, Position);
    RotationYawPitchRoll(WorldMatrix, Rotation, IsRotateWorldAxis);
    Scaling(WorldMatrix, Scale);
}
