#pragma once
#include "unrealEnums.h"

struct VersionStatus
{
    enum StatusCode
    {
        SUCCESS,
        FAILURE,
    };

    std::wstring ErrorMessage{};
    StatusCode Status{};

    VersionStatus() = default;
    VersionStatus(StatusCode StatusParam);
};

class UnrealVersion
{
public:
    static int32_t Major;
    static int32_t Minor;

    auto static Initialize(void* Address) -> VersionStatus;

    [[nodiscard]]
    auto static IsEqual(int32_t major_p, int32_t minor_p) -> bool { return (Major == major_p && Minor == minor_p); }

    [[nodiscard]]
    auto static IsAtLeast(int32_t major_p, int32_t minor_p) -> bool { return ((Major > major_p) || (Major == major_p && Minor >= minor_p)); }

    [[nodiscard]]
    auto static IsAtMost(int32_t major_p, int32_t minor_p) -> bool { return ((Major < major_p) || (Major == major_p && Minor <= minor_p)); }

    [[nodiscard]]
    auto static IsBelow(int32_t major_p, int32_t minor_p) -> bool { return ((Major < major_p) || (Major == major_p && Minor < minor_p)); }

    [[nodiscard]]
    auto static IsAbove(int32_t major_p, int32_t minor_p) -> bool { return ((Major > major_p) || (Major == major_p && Minor > minor_p)); }
};

template<typename To, typename From>
auto ptr_cast_deref(From ptr, int32_t offset = 0) -> To
{
    static_assert(std::is_pointer_v<From>, "ptr_cast_deref can only cast from pointer types");

    return *std::bit_cast<To*>(std::bit_cast<char*>(ptr) + offset);
}

template<typename To, typename From>
    auto ptr_cast_deref_safe(From ptr, int32_t offset, HANDLE process_handle) -> To
{
    static_assert(std::is_pointer_v<From>, "ptr_cast_deref_safe can only cast from pointer types");

    // For this example, To == UObject*
    // Therefore: To* == UObject**
    To* data_ptr = std::bit_cast<To*>(std::bit_cast<char*>(ptr) + offset);

    size_t bytes_read;
    uintptr_t is_valid_ptr_buffer;

    if (!ReadProcessMemory(process_handle, data_ptr, &is_valid_ptr_buffer, 0x8, &bytes_read))
    {
        return 0;
    }
    else
    {
        return *data_ptr;
    }
}
