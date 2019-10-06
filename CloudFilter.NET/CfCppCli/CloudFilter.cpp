#include "pch.h"
#include "CloudFilter.h"

#include <windows.h>
#include <cfapi.h>

#include <vcclr.h>

using Cf = JDanielSmith::Microsoft::CloudFilter::CloudFilter;

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfunregistersyncroot
void Cf::UnregisterSyncRoot(System::String^ syncRootPath)
{
	pin_ptr<const wchar_t> pSyncRootPath = PtrToStringChars(syncRootPath);
	const auto hr = CfUnregisterSyncRoot(pSyncRootPath);
	if (FAILED(hr))
	{
		System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(hr);
	}
}

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfclosehandle
void Cf::CloseHandle(HANDLE fileHandle)
{
	CfCloseHandle(fileHandle);
}
