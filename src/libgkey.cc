#include "core.hh"
#include "LogitechGkeyLib.h"

namespace GKeySdk {
  using namespace Napi;

  static inline Object InitAll(Env Env, Object Exports) {
    // return exports
    return Exports;
  }
}

Napi::Object InitAll(Napi::Env Env, Napi::Object Exports) {
  return GKeySdk::InitAll(Env, Exports);
}

NODE_API_MODULE(gkeysdk, InitAll)
