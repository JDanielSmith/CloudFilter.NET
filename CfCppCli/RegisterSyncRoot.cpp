#include "pch.h"
#include "CloudFilter.h"

#include <windows.h>
#include <cfapi.h>

#include <vcclr.h>

namespace Cf = JDanielSmith::Microsoft::CloudFilter;

[System::Flags]
public enum class Cf::RegisterFlags
{
	None = CF_REGISTER_FLAG_NONE,
	Update = CF_REGISTER_FLAG_UPDATE,
	DisableOnDemandPopulationOnRoot = CF_REGISTER_FLAG_DISABLE_ON_DEMAND_POPULATION_ON_ROOT,
	MarkInSyncOnRoot = CF_REGISTER_FLAG_MARK_IN_SYNC_ON_ROOT,
};

namespace JDanielSmith
{
	namespace Microsoft
	{
		namespace CloudFilter
		{
			public enum class HydrationPolicyPrimary : short
			{
				Partial = CF_HYDRATION_POLICY_PARTIAL,
				Progressive = CF_HYDRATION_POLICY_PROGRESSIVE,
				Full = CF_HYDRATION_POLICY_FULL,
				AlwaysFull = CF_HYDRATION_POLICY_ALWAYS_FULL,
			};

			[System::Flags]
			public enum class HydrationPolicyModifier : short
			{
				None = CF_HYDRATION_POLICY_MODIFIER_NONE,
				ValidationRequired = CF_HYDRATION_POLICY_MODIFIER_VALIDATION_REQUIRED,
				StreamingAllowed = CF_HYDRATION_POLICY_MODIFIER_STREAMING_ALLOWED,
				AutoDehydrationAllowed = CF_HYDRATION_POLICY_MODIFIER_AUTO_DEHYDRATION_ALLOWED,
			};

			public ref struct HydrationPolicy sealed
			{
				HydrationPolicyPrimary Primary;
				HydrationPolicyModifier Modifier;
			};

			public enum class PopulationPolicyPrimary : short
			{
				Partial = CF_POPULATION_POLICY_PARTIAL,
				Full = CF_POPULATION_POLICY_FULL,
				AlwaysFull = CF_POPULATION_POLICY_ALWAYS_FULL,
			};

			[System::Flags]
			public enum class PopulationPolicyModifier : short
			{
				None = CF_POPULATION_POLICY_MODIFIER_NONE,
			};

			public ref struct PopulationPolicy sealed
			{
				PopulationPolicyPrimary Primary;
				PopulationPolicyModifier Modifier;
			};

			[System::Flags]
			public enum class InSyncPolicy
			{
				None = CF_INSYNC_POLICY_NONE,
				TrackFileCreationTime = CF_INSYNC_POLICY_TRACK_FILE_CREATION_TIME,
				TrackFileReadonlyAttribute = CF_INSYNC_POLICY_TRACK_FILE_READONLY_ATTRIBUTE,
				TrackFileHiddenAttribute = CF_INSYNC_POLICY_TRACK_FILE_HIDDEN_ATTRIBUTE,
				TrackFileSystemAttribute = CF_INSYNC_POLICY_TRACK_FILE_SYSTEM_ATTRIBUTE,
				TrackDirectoryCreationTime = CF_INSYNC_POLICY_TRACK_DIRECTORY_CREATION_TIME,
				TrackDirectoryReadonlyAttribute = CF_INSYNC_POLICY_TRACK_DIRECTORY_READONLY_ATTRIBUTE,
				TrackDirectoryHiddenAttribue = CF_INSYNC_POLICY_TRACK_DIRECTORY_HIDDEN_ATTRIBUTE,
				TrackDirectorySystemAttribute = CF_INSYNC_POLICY_TRACK_DIRECTORY_SYSTEM_ATTRIBUTE,
				TrackFileLastWriteTime = CF_INSYNC_POLICY_TRACK_FILE_LAST_WRITE_TIME,
				TrackDirectoryLastWriteTime = CF_INSYNC_POLICY_TRACK_DIRECTORY_LAST_WRITE_TIME,
				TrackFileAll = CF_INSYNC_POLICY_TRACK_FILE_ALL,
				TrackDirectoryAll = CF_INSYNC_POLICY_TRACK_DIRECTORY_ALL,
				TrackAll = CF_INSYNC_POLICY_TRACK_ALL,
				PreserveInsyncForSyncEngine = CF_INSYNC_POLICY_PRESERVE_INSYNC_FOR_SYNC_ENGINE,
			};

			[System::Flags]
			public enum class HardlinkPolicy
			{
				None = CF_HARDLINK_POLICY_NONE,
				Allowed = CF_HARDLINK_POLICY_ALLOWED,
			};
		}
	}
}

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/ns-cfapi-cf_sync_policies
public ref struct Cf::SyncPolicies sealed
{
	HydrationPolicy^ Hydration = gcnew HydrationPolicy();
	PopulationPolicy^ Population = gcnew PopulationPolicy();
	InSyncPolicy InSync;
	HardlinkPolicy HardLink;
};

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/ns-cfapi-cf_sync_registration
public ref struct Cf::SyncRegistration sealed
{
	System::String^ ProviderName;
	System::String^ ProviderVersion;
	array<System::Byte>^ SyncRootIdentity;
	array<System::Byte>^ FileIdentity;
	System::Guid ProviderId;
};

// https://docs.microsoft.com/en-us/windows/win32/api/cfapi/nf-cfapi-cfregistersyncroot
void Cf::CloudFilter::RegisterSyncRoot(System::String^ syncRootPath, SyncRegistration^ registration, SyncPolicies^ policies, RegisterFlags registerFlags)
{
	const pin_ptr<const wchar_t> pSyncRootPath = PtrToStringChars(syncRootPath);

	CF_SYNC_REGISTRATION Registration;
	Registration.StructSize = sizeof(Registration);

	const pin_ptr<const wchar_t> pProviderName = PtrToStringChars(registration->ProviderName);
	Registration.ProviderName = pProviderName;

	const pin_ptr<const wchar_t> pProviderVersion = PtrToStringChars(registration->ProviderVersion);
	Registration.ProviderVersion = pProviderVersion;

	pin_ptr<System::Byte> pSyncRootIdentity = nullptr;
	{
		if (registration->SyncRootIdentity != nullptr)
		{
			pSyncRootIdentity = &(registration->SyncRootIdentity[0]);   // pin pointer to first element in arr
			Registration.SyncRootIdentityLength = registration->SyncRootIdentity->Length;
		}
		else
		{
			Registration.SyncRootIdentityLength = 0;
		}
		Registration.SyncRootIdentity = reinterpret_cast<LPCVOID>(pSyncRootIdentity);
	}

	pin_ptr<System::Byte> pFileIdentity = nullptr;
	{
		if (registration->FileIdentity != nullptr)
		{
			pFileIdentity = &(registration->FileIdentity[0]);   // pin pointer to first element in arr
			Registration.FileIdentityLength = registration->FileIdentity->Length;
		}
		else
		{
			Registration.FileIdentityLength = 0;
		}
		Registration.FileIdentity = reinterpret_cast<LPCVOID>(pFileIdentity);
	}

	// https://docs.microsoft.com/en-us/cpp/dotnet/how-to-convert-between-system-guid-and-guid?view=vs-2019
	array<System::Byte>^ providerIdData = (registration->ProviderId).ToByteArray();
	pin_ptr<System::Byte> pProviderIdData = &(providerIdData[0]);
	Registration.ProviderId = *reinterpret_cast<_GUID*>(pProviderIdData);

	CF_SYNC_POLICIES Policies;
	Policies.StructSize = sizeof(Policies);

	Policies.Hydration.Primary = static_cast<CF_HYDRATION_POLICY_PRIMARY>(policies->Hydration->Primary);
	Policies.Hydration.Modifier = static_cast<CF_HYDRATION_POLICY_MODIFIER>(policies->Hydration->Modifier);

	Policies.Population.Primary = static_cast<CF_POPULATION_POLICY_PRIMARY>(policies->Population->Primary);
	Policies.Population.Modifier = static_cast<CF_POPULATION_POLICY_MODIFIER>(policies->Population->Modifier);

	Policies.InSync = static_cast<CF_INSYNC_POLICY>(policies->InSync);
	Policies.HardLink = static_cast<CF_HARDLINK_POLICY>(policies->HardLink);

	const auto RegisterFlags = static_cast<CF_REGISTER_FLAGS>(registerFlags);

	const auto hr = CfRegisterSyncRoot(pSyncRootPath, &Registration, &Policies, RegisterFlags);
	if (FAILED(hr))
	{
		System::Runtime::InteropServices::Marshal::ThrowExceptionForHR(hr);
	}
}
