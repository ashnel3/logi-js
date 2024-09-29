#include "Core.hh"
#include "LogitechLEDLib.h"

namespace LedSdk {
  using namespace Napi;

  static inline Object InitAll(Env Env, Object Exports) {
    // return exports
    return Exports;
  }
}

Napi::Object InitAll(Napi::Env Env, Napi::Object Exports) {
  return LedSdk::InitAll(Env, Exports);
}

NODE_API_MODULE(ledsdk, InitAll)
