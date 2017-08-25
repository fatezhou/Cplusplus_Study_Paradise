#ifndef __SYSTEM_INFO_H__
#define __SYSTEM_INFO_H__

#include <vector>
#include <Windows.h>

namespace ZoyeeUtils{
	class CSystemInfo
	{
	public:				
		class HDInfo{
		public:
			enum{
				Unknown = DRIVE_UNKNOWN,
				NoRootDir = DRIVE_NO_ROOT_DIR,
				Removeable = DRIVE_REMOVABLE,
				Fixed = DRIVE_FIXED,
				Remote = DRIVE_REMOTE,
				CdRom = DRIVE_CDROM,
				Ramdisk = DRIVE_RAMDISK
			};
			class HDItem{
			public:
				long long llFreeSpace;
				long long llTotalSpace;
				char szDiskName[32];
				char szFileSystem[32];
				char cDisk;
				bool bIsUSBDirver;
				int nHDType;
				bool bIsVaild;
				bool bWriteAble;
				HDItem();
			};

			std::vector<HDItem>& GetInfo();
		private:			
			HDItem getItem(char cDisk);
			std::vector<HDItem> m_vecHDItems;
		};

		class MonitorInfo{
		public:
			class MonitorItem{
			public:
				MonitorItem();
				RECT rcMonitor;
				RECT rcWork;
				TCHAR szDisplayName[128];
				TCHAR szRegisterName[128];
				TCHAR szId[128];
				TCHAR szName[128];
				int id;
			};

			std::vector<MonitorInfo::MonitorItem>& GetMonitor();
			void AddMonitor(MonitorInfo::MonitorItem& item);
			int GetSize();
		private:
			std::vector<MonitorItem> m_vec;
		};

		class MemeryInfo{
		public:
			MemeryInfo();
			int GetFree();
			int GetTatal();
		protected:
			int nFreeMeta;
			int nTotalMeta;
		};

		class CpuInfo{
		public:
			CpuInfo();
			char* GetName();
			int GetCoreCount();
		protected:
			int nCoreCount;
			char szCpuName[128];
		};
		
	private:
		~CSystemInfo(void);
		CSystemInfo(void);
	};

	std::string GetProcessPath();
	std::string GetProcessDir();
	std::string GetEnvironmentValue(std::string strKey);
	std::string GetAppDataPath();
	std::string GetTempPath();
	std::string GetUserProfilePath();

}

#endif