#include "pch.h"
#include "UnrealVersion.h"

#include "app.h"


int32_t UnrealVersion::Major{-1};
int32_t UnrealVersion::Minor{-1};

VersionStatus::VersionStatus(StatusCode StatusParam) : Status(StatusParam) {}


auto UnrealVersion::Initialize(void* Address) -> VersionStatus
{
    uint16_t MajorVersion = ptr_cast_deref<uint16_t>(Address);
    uint16_t MinorVersion = ptr_cast_deref<uint16_t>(Address, 0x2);

    if (MajorVersion < 4 || MajorVersion > 5)
    {
        UE_LOG("Could not determine Unreal Engine version: major version was too big or too small. Defaulting to UE_5");
        return {VersionStatus::FAILURE};
    }

    if (MinorVersion > 30)
    {
        UE_LOG("Could not determine Unreal Engine version: minor version was too big. Defaulting to UE_5");
        return {VersionStatus::FAILURE};
    }

    const wchar_t* Branch = ptr_cast_deref<const wchar_t*>(Address, 0x10);
    void* BranchTest = ptr_cast_deref_safe<void*>(Branch, 0x0, GetCurrentProcess());
    if (!BranchTest)
    {
        UE_LOG("Could not determine Unreal Engine version: 'Branch' member variable FString data was nullptr. Defaulting to UE_5");
        return {VersionStatus::FAILURE};
    }

    auto BranchView = std::wstring_view{Branch};
    if (!BranchView.starts_with(wchar_t("++"))) // Occurs in most games
            {
        UE_LOG("Could not determine Unreal Engine version: 'Branch' member variable FString does not start with '++'. Defaulting to UE_5");
        return {VersionStatus::FAILURE};
            }

    Major = static_cast<int32_t>(MajorVersion);
    Minor = static_cast<int32_t>(MinorVersion);
}