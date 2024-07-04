#pragma once
#include "pch.h"

// ********************** Forward Declarations **********************
namespace Scene
{
    class GameObject;
    class CameraGameObject;
}

// ***************************** Scene ******************************
namespace Scene
{
    class World : public IObjectExecute
    {
    public:
        World() = default;
        virtual ~World() override;

    public:
        virtual void Initialize() override;
        virtual void Update(const float& DeltaTime) override;
        virtual void Render() override;
        virtual void Destroy() override;

    public:
        void AddGameObject(const std::shared_ptr<GameObject>& GameObject);

    public:
        void SetCameraGameObject(const std::weak_ptr<CameraGameObject>& GameObject);
        std::weak_ptr<CameraGameObject> GetCameraGameObject();
        
    public:
        void RenderGameObjects(const ERenderPassType& Type);

    private:
        void ClearCollectedRenderGameObjects();

    private:
        std::vector<std::shared_ptr<GameObject>> InitializeGameObjects;
        std::vector<std::shared_ptr<GameObject>> UpdateGameObjects;

    private:
        std::unordered_map<ERenderPassType, std::vector<std::shared_ptr<GameObject>>> RenderCollectedGameObjects;

    private:
        std::weak_ptr<CameraGameObject> CameraGameObject;
    };
}