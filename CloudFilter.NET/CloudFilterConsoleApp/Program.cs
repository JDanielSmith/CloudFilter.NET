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

			var syncRegistration = new SyncRegistration();
		}
	}
}
