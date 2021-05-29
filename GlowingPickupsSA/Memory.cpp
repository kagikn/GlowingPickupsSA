#include "Memory.h"

#include <Windows.h>
#include <psapi.h>
#include <sstream>
#include <string>
#include <vector>
#pragma comment(lib, "psapi.lib")

template<class T> std::vector<std::string> split(const std::string& s, const T& separator, bool ignore_empty = 0, bool split_empty = 0)
{
    struct
    {
        auto len(const std::string& s) { return s.length(); }
        auto len(const std::string::value_type* p) { return p ? std::char_traits<std::string::value_type>::length(p) : 0; }
        auto len(const std::string::value_type  c) { return c == std::string::value_type() ? 0 : 1; /*return 1;*/ }
    } util;

    if (s.empty())
    { /// empty string ///
        if (!split_empty || util.len(separator)) return { "" };
        return {};
    }

    auto v = std::vector<std::string>();
    auto n = static_cast<std::string::size_type>(util.len(separator));
    if (n == 0)
    {    /// empty separator ///
        if (!split_empty) return { s };
        for (auto&& c : s) v.emplace_back(1, c);
        return v;
    }

    auto p = std::string::size_type(0);
    while (1)
    {      /// split with separator ///
        auto pos = s.find(separator, p);
        if (pos == std::string::npos)
        {
            if (ignore_empty && p - n + 1 == s.size()) break;
            v.emplace_back(s.begin() + p, s.end());
            break;
        }
        if (!ignore_empty || p != pos)
            v.emplace_back(s.begin() + p, s.begin() + pos);
        p = pos + n;
    }
    return v;
}


uintptr_t memory::FindPattern(const char* pattStr)
{
    std::vector<std::string> bytesStr = split(pattStr, ' ');

    MODULEINFO modInfo{};
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &modInfo, sizeof(MODULEINFO));

    auto* start_offset = static_cast<uint8_t*>(modInfo.lpBaseOfDll);
    const auto size = static_cast<uintptr_t>(modInfo.SizeOfImage);

    uintptr_t pos = 0;
    const uintptr_t searchLen = bytesStr.size();

    for (auto* retAddress = start_offset; retAddress < start_offset + size; retAddress++)
    {
        if (bytesStr[pos] == "??" || bytesStr[pos] == "?" ||
            *retAddress == static_cast<uint8_t>(std::strtoul(bytesStr[pos].c_str(), nullptr, 16)))
        {
            if (pos + 1 == bytesStr.size())
                return (reinterpret_cast<uintptr_t>(retAddress) - searchLen + 1);
            pos++;
        }
        else
        {
            pos = 0;
        }
    }
    return 0;
}
