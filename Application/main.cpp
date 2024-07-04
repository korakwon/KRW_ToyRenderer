#include "ApplicationPch.h"

// ************************** Application **************************
#include "0_ForwardRendering/App_ForwardRendering.h"
#include "1_DeferredRendering/App_DeferredRendering.h"
#include "2_PBR_DamagedHelmet/App_PBR.h"

// ***************************** Main ******************************
int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR Param, int Command)
{
    Platform::G_HINSTANCE = Instance;

    //App_ForwardRendering::InitApplication();
    //App_DeferredRendering::InitApplication();
    App_PBR::InitApplication();
    
    return 0;
}