#pragma once
#include "pch.h"

// ****************************** Define *****************************
#define TIME_MANAGER Core::TimeManager::GetInstance()

// ****************************** Croe *******************************
namespace Core
{
    class TimeManager : public ICoreExecute
    {
        DECLARE_SINGLETON(TimeManager)
        
    public:
        virtual ~TimeManager() final;
        
    public:
        virtual void Initialize() final;
        virtual void Update(const float& DeltaTime) final;
        virtual void Destroy() final;

    public:
        float GetRunningTime() const { return RunningTime; }
        float GetDeltaTime() const { return DeltaSeconds; }
        float GetFPS() const { return FPS; }
        
    private:
	    INT64 TicksPerSecond;

    private:
        INT64 PreviousTime;
        INT64 CurrentTime;

    private:
        float RunningTime;
        float DeltaSeconds;

    private:
        float FPS;
    };
}