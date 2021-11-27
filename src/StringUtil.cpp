#include <string>
#include <iostream>
#include <codecvt>
#include <locale>

#include "StringUtil.h"

std::string StringUtil::wchar_to_string(const wchar_t* wchar_to_convert) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
    return utf8_conv.to_bytes(wchar_to_convert);
}
