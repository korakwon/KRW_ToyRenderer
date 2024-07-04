#pragma once

class ICoreExecute abstract
{
public:
    virtual ~ICoreExecute() = default;

public:
    virtual void Initialize() = 0;
    virtual void Update(const float& DeltaTime) = 0;
    virtual void Destroy() = 0;
};

class ISystemExecute abstract
{
public:
    virtual ~ISystemExecute() = default;

public:
    virtual void Initialize()
    {
    }
    virtual void Update(const float& DeltaTime)
    {
    }
    virtual void Destroy() = 0;
};

class IObjectExecute abstract
{
public:
    virtual ~IObjectExecute() = default;

public:
    virtual void Initialize()
    {
        if(IsMarkForDelete == true)
        {
            return;
        }
        
        IsInitialize = true;
    }
    virtual void Update(const float& DeltaTime)
    {
        if(IsMarkForDelete == true)
        {
            return;
        }
    }
    virtual void Render()
    {
        if(IsMarkForDelete == true)
        {
            return;
        }
    }
    virtual void Destroy()
    {
        IsMarkForDelete = true;
    }

public:
    bool IsInitialized() const { return IsInitialize; }
    
protected:
    bool IsInitialize = false;
    bool IsMarkForDelete = false;
};
