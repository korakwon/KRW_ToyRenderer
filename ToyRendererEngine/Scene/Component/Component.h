#pragma once
#include "pch.h"

// ********************** Forward Declarations **********************
namespace Scene
{
    class GameObject;
}

// ***************************** Scene ******************************
namespace Scene
{
    class Component : public IObjectExecute
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(Component)

    public:
        Component(const std::string& ComponentName);
        virtual ~Component() override = default;

    public:
        virtual void Initialize() override;
        virtual void Update(const float& DeltaTime) override;
        virtual void Render() override;
        virtual void Destroy() override;
        
    public:
        const std::string& GetName() { return Name; }

    protected:
        std::string Name;

    protected:
        Scene::GameObject* ParentGameObject = nullptr;

    private:
        friend Scene::GameObject;
    };
}
