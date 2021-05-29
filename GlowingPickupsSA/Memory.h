#pragma once
#include <cstdint>
#include <Windows.h>

class memory
{
public:
    static uintptr_t FindPattern(const char* pattStr, HMODULE moduleHandle = nullptr);
};
