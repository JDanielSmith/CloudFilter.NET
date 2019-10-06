#include "pch.h"
#include "CloudFilter.h"

#include <windows.h>
#include <cfapi.h>

#include <vcclr.h>

namespace Cf = JDanielSmith::Microsoft::CloudFilter;

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/ns-cfapi-cf_platform_info
public ref struct Cf::PlatformInfo sealed
{
	initonly DWORD BuildNumber;
	initonly DWORD RevisionNumber;
	initonly DWORD IntegrationNumber;
internal:
	PlatformInfo(const CF_PLATFORM_INFO& value)
	{
		BuildNumber = value.BuildNumber;
		RevisionNumber = value.RevisionNumber;
		IntegrationNumber = value.IntegrationNumber;
	}
};

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfgetplatforminfo
Cf::PlatformInfo^ Cf::CloudFilter::GetPlatformInfo()
{
	CF_PLATFORM_INFO value;
	const auto hr = CfGetPlatformInfo(&value);
	if (FAILED(hr))
	{
		System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(hr);
	}

	return gcnew PlatformInfo(value);
}