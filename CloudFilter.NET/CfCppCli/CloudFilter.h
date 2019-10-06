#pragma once

#include <windows.h>
#include <cfapi.h>

#using <System.dll>

namespace JDanielSmith
{
	namespace Microsoft
	{
		namespace CloudFilter
		{
			ref struct SyncRegistration; // forward
			ref struct SyncPolicies; // forward
			enum class RegisterFlags; // forward

			ref struct PlatformInfo; // forward

			// https://docs.microsoft.com/en-us/windows/win32/cfapi/cloud-files-functions
			public ref struct CloudFilter abstract sealed
			{
				// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfregistersyncroot
				static void RegisterSyncRoot(System::String^ syncRootPath, SyncRegistration^ registration, SyncPolicies^ policies, RegisterFlags registerFlags);

				// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfunregistersyncroot
				static void UnregisterSyncRoot(System::String^ syncRootPath);

				// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfclosehandle
				static void CloseHandle(System::IntPtr fileHandle);

				// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfgetplatforminfo
				static PlatformInfo^ GetPlatformInfo();
			};
		}
	}
}

