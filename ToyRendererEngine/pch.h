#pragma once

// **************************** Library ****************************
#ifdef _DEBUG
#pragma comment(lib, "../_Libraries/ThirdParty/Assimp/lib/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "../_Libraries/ThirdParty/Assimp/lib/assimp-vc143-mt.lib")
#endif

// **************************** Header ****************************
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <chrono>

#include <assert.h>

#include <algorithm>

#include <memory>
#include <mutex>
#include <functional>

#include <fstream>

#include <Windows.h>
#include <windowsx.h>

#include "Util/IExecute.h"
#include "Util/Math.h"

// **************************** Defines ****************************
#define Super __super

#define ASSERT(Target) assert((Target))
#define ASSERT_NOT_NULL(Target) assert((Target) != nullptr)
#define ASSERT_NULL(Target) assert((Target) == nullptr)
#define ASSERT_ZERO(Target) assert((Target) != 0)

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = nullptr; } }

#define DISABLE_COPY_CLASS(ClassName)                       \
private:                                                    \
    ClassName(const ClassName&) = delete;                   \
    ClassName(ClassName&&) = delete;                        \
    ClassName& operator=(const ClassName&) = delete;        \
    ClassName& operator=(ClassName&&) = delete;

#define DISABLE_BASE_CLASS_CONSTRUCTOR_OPERATOR(ClassName)  \
    DISABLE_COPY_CLASS(ClassName)                           \
private:                                                    \
    ClassName() = delete;

#define DECLARE_PRIVATE_CLASS(ClassName)                    \
    DISABLE_COPY_CLASS(ClassName)                           \
private:                                                    \
    ClassName();

#define DECLARE_SINGLETON(ClassName)                        \
    DISABLE_COPY_CLASS(ClassName)                           \
private:                                                    \
    ClassName();                                            \
public:                                                     \
    static ClassName* GetInstance();                        \
private:                                                    \
    static std::unique_ptr<ClassName> Instance;

#define IMPLEMENT_SINGLETON(ClassName)                      \
std::unique_ptr<ClassName> ClassName::Instance = nullptr;   \
ClassName* ClassName::GetInstance()					        \
{                                                           \
    if(Instance == nullptr)                                 \
    {                                                       \
        Instance.reset(new ClassName());                    \
    }                                                       \
    return Instance.get();                                  \
}

#define DESTROY_SINGLETON()                                 \
if(Instance != nullptr)                                     \
{                                                           \
    Instance.reset();                                       \
    Instance = nullptr;                                     \
}

// ****************************** Enum ******************************
static constexpr UINT32 RENDER_PASS_TYPE_NUM = 4;
enum class ERenderPassType
{
    Shadow = 1 << 0,
    Main = 1 << 1,
    PostProcess = 1 << 2,
    UI = 1 << 3,
};
inline ERenderPassType operator|(ERenderPassType lhs, ERenderPassType rhs) 
{
    return static_cast<ERenderPassType>(static_cast<UINT32>(lhs) | static_cast<UINT32>(rhs));
}

enum class EKeyStateType : byte
{
    None = 0,
    Down,
    Up,
    Press,
};

enum class ERasterizationInfo_CullModeType : byte
{
    None,
    Front,
    Back,
    FrontAndBack
};
enum class ERasterizationInfo_FrontFaceModeType : byte
{
    CW,
    CCW,
};

enum class ETextureFormatType
{
    R8G8B8A8,
    R16G16B16A16,
    R32G32B32A32,
};