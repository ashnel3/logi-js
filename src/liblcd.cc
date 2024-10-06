#include "core.hh"
#include "LogitechLCDLib.h"

namespace LcdSdk {
  using namespace Napi;

  typedef enum {
    COLOR = LOGI_LCD_TYPE_COLOR,
    MONO = LOGI_LCD_TYPE_MONO
  } TYPE;

  Boolean init(const CallbackInfo& Info) {
    auto name = ParamRequire<String>(Info, 0).Utf8Value();
    int type = ParamRequire<Number>(Info, 1).Int32Value();
    return Boolean::New(
      Info.Env(),
      LogiLcdInit((wchar_t *) ConvWStr(name), type)
    );
  }

  template <TYPE T>
  Boolean setText(const CallbackInfo& Info);

  template <>
  Boolean setText<TYPE::COLOR>(const CallbackInfo& Info) {
    int line = ParamRequire<Number>(Info, 0).Int32Value();
    auto text = ParamRequire<String>(Info, 1).Utf8Value();
    // TODO: add optional params
    return Boolean::New(
      Info.Env(),
      LogiLcdColorSetText(line, (wchar_t *) ConvWStr(text))
    );
  }

  template <>
  Boolean setText<TYPE::MONO>(const CallbackInfo& Info) {
    int line = ParamRequire<Number>(Info, 0).Int32Value();
    auto text = ParamRequire<String>(Info, 1).Utf8Value();
    return Boolean::New(
      Info.Env(),
      LogiLcdMonoSetText(line, (wchar_t*) ConvWStr(text))
    );
  }

  void update(const CallbackInfo& Info) {
    LogiLcdUpdate();
  }

  void shutdown(const CallbackInfo& Info) {
    LogiLcdShutdown();
  }

  template <TYPE T>
  inline Object initGroup(
    Env env,
    std::pair<int, int> size
  ) {
    return Initializer::Map(
      Object::New(env),
      {
        {
          "isConnected",
          Function::New(env, [&](CallbackInfo& Info) -> Boolean {
            return Boolean::New(Info.Env(), LogiLcdIsConnected(T));
          })
        },
        { "setText", Function::New(env, setText<T>) },
        { "size", Initializer::Box::New(env, size) },
        { "type", Number::New(env, T) },
      }
    );
  }

  inline Object InitAll(Env env, Object exports) {
    return Initializer::Map(
      exports,
      {
        // general methods
        { "init", Function::New(env, init) },
        {
          "isConnected",
          Function::New(env, [](const CallbackInfo& Info) -> Boolean {
            int type = ParamRequire<Number>(Info, 0).Int32Value();
            return Boolean::New(Info.Env(), LogiLcdIsConnected(type));
          })
        },
        { "update", Function::New(env, update) },
        { "shutdown", Function::New(env, shutdown) },
        // lcd groups
        {
          "color",
          initGroup<TYPE::COLOR>(env, { LOGI_LCD_COLOR_WIDTH, LOGI_LCD_COLOR_HEIGHT })
        },
        {
          "mono",
          initGroup<TYPE::MONO>(env, { LOGI_LCD_MONO_WIDTH, LOGI_LCD_MONO_HEIGHT })
        }
      }
    );
  }
}

Napi::Object InitAll(Napi::Env Env, Napi::Object Exports) {
  return LcdSdk::InitAll(Env, Exports);
}

NODE_API_MODULE(lcdsdk, InitAll)
