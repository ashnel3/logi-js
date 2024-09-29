#include <codecvt>
#include <napi.h>

namespace Util {
  static inline std::wstring to_wstring(std::string str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wconv;
    return wconv.from_bytes(str);
  }
}
