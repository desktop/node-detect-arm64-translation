#include "napi.h"
#include "uv.h"

#include <assert.h>
#include <windows.h>

#include <cstdio>
#include <memory>

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS2)(HANDLE, PUSHORT, PUSHORT);
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

using namespace Napi;

namespace
{

bool IsWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    auto IsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(IsWow64Process == nullptr)
    {
      fprintf(stderr, "IsWow64Process is nullptr");
      return false;
    }

    if (!IsWow64Process(GetCurrentProcess(), &bIsWow64))
    {
      fprintf(stderr, "Error invoking IsWow64Process");
      return false;
    }

    return bIsWow64;
}

  Napi::Value IsRunningUnderARM64Translation(const Napi::CallbackInfo &info)
  {
    Napi::Env &env = info.Env();

    USHORT processMachine = 0;
    USHORT nativeMachine = 0;

    auto IsWow64Process2 = reinterpret_cast<LPFN_ISWOW64PROCESS2>(
        GetProcAddress(GetModuleHandle("kernel32.dll"), "IsWow64Process2"));

    if (IsWow64Process2 == nullptr)
    {
      return Napi::Boolean::New(env, false);
    }

    if (!IsWow64Process2(GetCurrentProcess(), &processMachine, &nativeMachine))
    {
      return Napi::Boolean::New(env, false);
    }

    fprintf(stderr, "IsWow64: %d\n", IsWow64());
    fprintf(stderr, "Native: %x\n", nativeMachine);
    fprintf(stderr, "Process: %x\n", processMachine);

    return Napi::Boolean::New(env, nativeMachine == IMAGE_FILE_MACHINE_ARM64 && processMachine != IMAGE_FILE_MACHINE_UNKNOWN && processMachine != IMAGE_FILE_MACHINE_ARM64);
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports)
  {
    exports.Set(Napi::String::New(env, "isRunningUnderARM64Translation"), Napi::Function::New(env, IsRunningUnderARM64Translation));

    return exports;
  }
}

#if NODE_MAJOR_VERSION >= 10
NAN_MODULE_WORKER_ENABLED(detectARM64TranslationNativeModule, Init)
#else
NODE_API_MODULE(detectARM64TranslationNativeModule, Init);
#endif
