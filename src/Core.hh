#include <codecvt>
#include <sstream>
#include <napi.h>

#ifndef LOGIJS_CORE_HH
#define LOGIJS_CORE_HH

namespace {
  template <typename T>
  napi_valuetype ValueTypeFor();

  template <>
  napi_valuetype ValueTypeFor<Napi::Boolean>() { return napi_boolean; }

  template <>
  napi_valuetype ValueTypeFor<Napi::Number>() { return napi_number; }

  template <>
  napi_valuetype ValueTypeFor<Napi::String>() { return napi_string; }

  template <>
  napi_valuetype ValueTypeFor<Napi::Function>() { return napi_function; }

  const char* ValueTypeName(napi_valuetype type) {
    switch (type) {
      case napi_valuetype::napi_bigint:
        return "bigint";
      case napi_valuetype::napi_boolean:
        return "boolean";
      case napi_valuetype::napi_external:
        return "external";
      case napi_valuetype::napi_function:
        return "function";
      case napi_valuetype::napi_null:
        return "null";
      case napi_valuetype::napi_number:
        return "number";
      case napi_valuetype::napi_object:
        return "object";
      case napi_valuetype::napi_string:
        return "string";
      case napi_valuetype::napi_symbol:
        return "symbol";
      case napi_valuetype::napi_undefined:
        return "undefined";
      default:
        return "unknown";
    }
  }
}

namespace Initializer {
  using namespace Napi;

  namespace Box {
    static inline Object Set(Object obj, const std::pair<int, int>& pos) {
      obj.Set("width", pos.first);
      obj.Set("height", pos.second);
      return obj;
    }

    static inline Object New(Env env, const std::pair<int, int>& pos) {
      return Set(Object::New(env), pos);
    }
  }

  static inline Object Map(
    Object obj,
    const std::initializer_list<std::pair<const char*, Value>>& list
  ) {
    for (auto const& p : list) {
      obj.Set(p.first, p.second);
    }
    return obj;
  }
}

inline const wchar_t* ConvWStr(const std::string str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wconv;
  return wconv.from_bytes(str).c_str();
}

/// @brief Assert function parameter
/// @tparam T     Napi parameter type
/// @param info   Napi callback info
/// @param idx    Napi argument index
/// @return       Casted parameter
template <typename T>
T ParamRequire(const Napi::CallbackInfo &info, int idx) {
  napi_valuetype expected = ValueTypeFor<T>();
  napi_valuetype received = info[idx].Type();
  if (info.Length() <= idx || expected != received) {
    std::ostringstream sErr;
    sErr
      << "invalid parameter (" << idx << "), "
      << "expected \"" << ValueTypeName(expected)
      << "\" received \"" << ValueTypeName(received) << "\"!";
    throw Napi::TypeError::New(info.Env(), sErr.str());
  }
  return info[idx].As<T>();
}

#endif
