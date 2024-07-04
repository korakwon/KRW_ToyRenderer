#include "TimeManager.h"

using Core::TimeManager;

IMPLEMENT_SINGLETON(TimeManager)

TimeManager::TimeManager()
    : TicksPerSecond(0), PreviousTime(0), CurrentTime(0), RunningTime(0), DeltaSeconds(0), FPS(0)
{
}
TimeManager::~TimeManager()
{
    Destroy();
}

void TimeManager::Initialize()
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&TicksPerSecond));
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&PreviousTime));
}
void TimeManager::Update(const float& DeltaTime)
{
    // DeltaTime & RunningTime
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&CurrentTime));
    DeltaSeconds = static_cast<float>(CurrentTime - PreviousTime) / static_cast<float>(TicksPerSecond);
    RunningTime += DeltaSeconds;

    // FPS Update
    FPS = 1.0f / DeltaSeconds;
        
    PreviousTime = CurrentTime;
}
void TimeManager::Destroy()
{
    DESTROY_SINGLETON()
}
