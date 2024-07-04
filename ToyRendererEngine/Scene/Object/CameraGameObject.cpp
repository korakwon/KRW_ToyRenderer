#include "CameraGameObject.h"

#include "Application/Application.h"
#include "Platform/BaseWindow.h"
#include "Scene/Component/Transform/TransformComponent.h"

using Scene::CameraGameObject;

void CameraGameObject::Update(const float& DeltaTime)
{
    Super::Update(DeltaTime);
    
    // View, Projection Matrix
    {
        UINT32 Width = 0, Height = 0;
        APPLICATION->GetWindow()->GetExtend(Width, Height);

        // View Matrix
        {
            static Vector3 UpVector = Vector3(0.0f, 1.0f, 0.0f);
            Vector3 Eye = TransformComponent->GetPosition();
            Vector3 Look = Eye + TransformComponent->Forward();
            
            ViewMatrix = glm::lookAt(Eye, Look, UpVector);
        }

        // Projection Matrix
        {
            ProjectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(Width) / static_cast<float>(Height), Near, Far);
            ProjectionMatrix[1][1] *= -1;
        }
    }
}

void CameraGameObject::SetFOV(const float& TargetFOV)
{
    static float MIN_FOV = 10.0f;
    static float MAX_FOV = 180.0f;
    this->FOV = std::min(std::max(MIN_FOV, TargetFOV), MAX_FOV);
}
void CameraGameObject::SetNear(const float& NearClip)
{
    static float MIN_NEAR = 0.1f;
    Near = std::min(std::max(MIN_NEAR, NearClip), Far);
}
void CameraGameObject::SetFar(const float& FarClip)
{
    static float MAX_NEAR = 1000.0f;
    Far = std::min(std::max(Near, FarClip), MAX_NEAR);
}
