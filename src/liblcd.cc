#include "Core.hh"
#include "LogitechLCDLib.h"

namespace LcdSdk {
  using namespace Napi;

  static Boolean LcdInit(const CallbackInfo& info) {
    auto Env = info.Env();
    auto name = info[0].As<String>().Utf8Value();
    int type = info[1].As<Number>().Int32Value();
    return Boolean::New(Env, LogiLcdInit((wchar_t *) Util::to_wstring(name).c_str(), type));
  }

  static Boolean LcdIsConnected(const CallbackInfo& info) {
    auto env = info.Env();
    int type = info[0].As<Number>().Int32Value();
    return Boolean::New(env, LogiLcdIsConnected(type));
  }

  static Boolean LcdMonoSetText(const CallbackInfo& info) {
    auto Env = info.Env();
    auto line = info[0].As<Number>().Int32Value();
    std::string text = info[1].As<String>().Utf8Value();
    return Boolean::New(
      Env,
      LogiLcdMonoSetText(line, (wchar_t*) Util::to_wstring(text).c_str())
    );
  }

  static void LcdUpdate(const CallbackInfo& info) {
    LogiLcdUpdate();
  }

  static void LcdShutdown(const CallbackInfo& info) {
    LogiLcdShutdown();
  }

  static inline Object InitAll(Env Env, Object Exports) {
    // lcd types
    Object Types = Object::New(Env);
    Types.Set("color", LOGI_LCD_TYPE_COLOR);
    Types.Set("mono", LOGI_LCD_TYPE_MONO);
    Exports.Set("types", Types);

    // display sizes
    Object SizeColor = Object::New(Env);
    Object SizeMono = Object::New(Env);
    Object Size = Object::New(Env);
    SizeColor.Set("height", LOGI_LCD_COLOR_HEIGHT);
    SizeColor.Set("width", LOGI_LCD_COLOR_WIDTH);
    SizeMono.Set("height", LOGI_LCD_MONO_HEIGHT);
    SizeMono.Set("width", LOGI_LCD_MONO_WIDTH);
    Size.Set("color", SizeColor);
    Size.Set("mono", SizeMono);
    Exports.Set("sizes", Size);

    // functions
    Exports.Set("LcdInit", Function::New(Env, LcdInit));
    Exports.Set("LcdIsConnected", Function::New(Env, LcdIsConnected));
    Exports.Set("LcdMonoSetText", Function::New(Env, LcdMonoSetText));
    Exports.Set("LcdUpdate", Function::New(Env, LcdUpdate));
    Exports.Set("LcdShutdown", Function::New(Env, LcdShutdown));

    // return exports
    return Exports;
  }
}

Napi::Object InitAll(Napi::Env Env, Napi::Object Exports) {
  return LcdSdk::InitAll(Env, Exports);
}

NODE_API_MODULE(lcdsdk, InitAll)
