#include "pch.h"

#include "app.h"

#include "UnrealVersion.h"


void GetUEVerOverride(int16_t UEMajorVerOverride, int16_t UEMinorVerOverride) ->  std::pair<bool, EUnrealVersion>
{
	auto UEVer = {};

	if (!UEMajorVerOverride && !UEMinorVerOverride)
	{
		return { false, UEVer };
	}
	
	if (UEMajorVerOverride && UEMinorVerOverride)
	{
		if (UEMajorVerOverride(5))
		{
			switch (UEMinorVerOverride)
			{
			case (UEMinorVerOverride = 0):
				UEVer = EUnrealVersion::UE5;
				break;
			case (UEMinorVerOverride = 01):
				UEVer = EUnrealVersion::UE5_01;
				break;
			case (UEMinorVerOverride = 02):
				UEVer = EUnrealVersion::UE5_02;
				break;
			}
		}
		else if (UEMajorVerOverride(4))
		{
			switch (UEMinorVerOverride)
			{
			case (UEMinorVerOverride = 26):
				UEVer = EUnrealVersion::UE4_26;
				break;
			case (UEMinorVerOverride = 27):
				UEVer = EUnrealVersion::UE4_27;
				break;
			default:
				UEVer = EUnrealVersion::NONE;
				break;
			}
		}
		else
		{
			UEVer = EUnrealVersion::NONE;
		}
	}
		return { true, UEVer };
			
}


IDumper* CreateAppInstance(EUnrealVersion Version)
{
	switch (Version)
	{
	case EUnrealVersion::UE5:
	case EUnrealVersion::UE5_01:
		return new Dumper<Engine_UE5>();
		break;
	default:
		return new Dumper<DefaultEngine<>>();
		break;
	}
}
