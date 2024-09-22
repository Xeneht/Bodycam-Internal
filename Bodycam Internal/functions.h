#pragma once

template<typename T>
inline bool IsBadPoint(T* ptr)
{
    std::uintptr_t Pointer = reinterpret_cast<std::uintptr_t>(ptr);

    if ((Pointer < 0xFFFFFFFFFFULL) || (Pointer > 0x2FFFFFFFFFFULL))
        return true;
    else
        return false;
}

inline SDK::FName StrToName(std::string str)
{
    std::wstring wstr(str.begin(), str.end());
    const wchar_t* wstr_cstr = wstr.c_str();

    return SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(TEXT(wstr_cstr)));
}