using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using JDanielSmith.Microsoft.CloudFilter;

namespace CloudFilterConsoleApp
{
	class Program
	{
		static void Main()
		{
			var platformInfo = CloudFilter.GetPlatformInfo();

			var registration = new SyncRegistration();
			var policies = new SyncPolicies();
			var registerFlags = RegisterFlags.None;
			CloudFilter.RegisterSyncRoot(@"C:\Users\JDani\SyncRoot", registration, policies, registerFlags);
		}
	}
}
