#pragma once
#include "pch.h"

// ********************** Forward Declarations **********************
namespace App
{
    class Application;
}

// **************************** Platform ****************************
namespace Platform
{
    extern HINSTANCE G_HINSTANCE;
    extern HWND G_HWND;

    class BaseWindow : public ICoreExecute
    {
        DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(BaseWindow)
        
    protected:
        BaseWindow(std::wstring PlatformTitle, const UINT32& PlatformWidth, const UINT32& PlatformHeight);
        virtual ~BaseWindow() override;
        
    public:
        void GetTitle(std::wstring& OutputString) const
        {
            OutputString = Title;
        }
        void GetExtend(UINT32& OutputWidth, UINT32& OutputHeight) const
        {
            OutputWidth = Width;
            OutputHeight = Height;
        }

    public:
        void Show() const;
        void Hide() const;

    protected:
        virtual void Initialize() override;
        virtual void Update(const float& DeltaTime) override;
        virtual void Destroy() override;

    protected:
        void OnResize(const UINT32& Width, const UINT32& Height);
        
    protected:
        std::wstring Title;
        UINT32 Width;
        UINT32 Height;
        
    private:
        friend App::Application;
    };
}
