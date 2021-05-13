#include "napi.h"
#include "uv.h"

#include <assert.h>
#include <windows.h>

#include <cstdio>
#include <memory>

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS2)(HANDLE, PUSHORT, PUSHORT);

using namespace Napi;

namespace
{
  Napi::Value IsRunningUnderARM64Translation(const Napi::CallbackInfo &info)
  {
    Napi::Env &env = info.Env();

    USHORT processMachine = 0;
    USHORT nativeMachine = 0;

    LPFN_ISWOW64PROCESS2 fnIsWow64Process2 = (LPFN_ISWOW64PROCESS2)GetProcAddress(
        GetModuleHandle("kernel32"), "IsWow64Process2");

    if (fnIsWow64Process2 != NULL)
    {
      if (!fnIsWow64Process2(GetCurrentProcess(), &processMachine,
                             &nativeMachine))
      {
        return Napi::Boolean::New(env, false);
      }
    }

    return Napi::Boolean::New(env, nativeMachine == 0xaa64);
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
