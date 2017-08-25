#include "SystemInfo.h"

#include <Windows.h>
#include <iostream>

bool IsDiskConnectedByUSB(char c);
bool TestDiskPermission(char cDisk);


ZoyeeUtils::CSystemInfo::HDInfo::HDItem ZoyeeUtils::CSystemInfo::HDInfo::getItem( char cDisk )
{
	HDItem item;
	char szRoot[16] = {0};
	sprintf(szRoot, "%c:\\", cDisk);
	item.nHDType = GetDriveTypeA(szRoot);
	do{
		DWORD dwSerialNumber, dwMaxComponentLength, dwFsFlags;
		if ( ! GetVolumeInformationA(szRoot, item.szDiskName, sizeof(item.szDiskName), &dwSerialNumber, &dwMaxComponentLength, &dwFsFlags, item.szFileSystem, sizeof(item.szFileSystem))){
			break;
		}

		item.bIsUSBDirver = IsDiskConnectedByUSB(cDisk);
		item.bWriteAble = TestDiskPermission(cDisk);
		_ULARGE_INTEGER lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
		if( ! GetDiskFreeSpaceExA(szRoot, &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes)){
			break;
		}

		item.llFreeSpace = lpTotalNumberOfFreeBytes.QuadPart / (1024 *1024);
		item.llTotalSpace = lpTotalNumberOfBytes.QuadPart / (1024 *1024);
		item.cDisk = cDisk;
		item.bIsVaild = true;
	} while (false);
	return item;
}

std::vector<ZoyeeUtils::CSystemInfo::HDInfo::HDItem>& ZoyeeUtils::CSystemInfo::HDInfo::GetInfo()
{
	m_vecHDItems.clear();
	for (int i = 'a'; i < 'z' + 1; i++){
		HDItem item = getItem(i);
		if (item.bIsVaild){
			m_vecHDItems.push_back(item);
		}
	}
	return m_vecHDItems;
}

ZoyeeUtils::CSystemInfo::HDInfo::HDItem::HDItem()
{
	this->bIsUSBDirver = false;
	this->bIsVaild = false;
	this->cDisk = '?';
	this->llFreeSpace = 0;
	this->llTotalSpace = 0;
	this->nHDType = HDInfo::Unknown;
	this->szDiskName[0] = 0;
	this->szFileSystem[0] = 0;
} 

bool IsDiskConnectedByUSB(char c)
{
	STORAGE_PROPERTY_QUERY Query;
	DWORD dwOutBytes;
	BOOL bResult; 
	Query.PropertyId = StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery; 

	char szFile[128] ;
	sprintf(szFile, "\\\\.\\%c:", c);

	HANDLE hDevice=CreateFileA(
		szFile, // Ҫ�򿪵��������豸����ʽ����Ϊ��\\.\C:���������ʧ�ܣ����ش�����2��ϵͳ�Ҳ���ָ�����ļ���
		GENERIC_READ | GENERIC_WRITE,     // ����Ȩ��
		FILE_SHARE_READ | FILE_SHARE_WRITE, //����ģʽ
		NULL,             // ʹ��Ĭ�ϵİ�ȫ����
		OPEN_EXISTING,    // �򿪴��ڵ��豸
		NULL,             
		NULL            
		);
	if (hDevice == INVALID_HANDLE_VALUE){
		return false;
	}

	STORAGE_DESCRIPTOR_HEADER *pDevDescHeader;
	pDevDescHeader = (STORAGE_DESCRIPTOR_HEADER *)malloc(sizeof(STORAGE_DESCRIPTOR_HEADER));
	if (NULL == pDevDescHeader){
		return false;
	}
	bResult = DeviceIoControl(
		hDevice,     // �豸���
		IOCTL_STORAGE_QUERY_PROPERTY,     // ����������
		&Query,sizeof Query,   //�����������������С 
		pDevDescHeader,     //�������
		sizeof(STORAGE_DESCRIPTOR_HEADER),  //��������Ļ�������С
		&dwOutBytes,     //ʵ������Ĵ�С
		NULL);     
	if (!bResult){
		free(pDevDescHeader);
		(void)CloseHandle(hDevice);
		return false;
	}
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
	pDevDesc = (STORAGE_DEVICE_DESCRIPTOR *)malloc(pDevDescHeader->Size);
	bResult = DeviceIoControl(hDevice,
		IOCTL_STORAGE_QUERY_PROPERTY,
		&Query, sizeof Query,
		pDevDesc, pDevDescHeader->Size,  
		&dwOutBytes, 
		NULL);
	free(pDevDescHeader);
	CloseHandle(hDevice);
	if(pDevDesc->BusType==BusTypeUsb){//��������ΪUSB������TRUE
		free(pDevDesc);
		return true;
	}
	free(pDevDesc);
	return false;
};

bool TestDiskPermission( char cDisk )
{
	char szFilePath[128] = "";
	sprintf(szFilePath, "%c:\\diskPermissionTester.txt", cDisk);
	printf("%s\n", szFilePath);
	HANDLE hFile = CreateFileA(szFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
	bool bRes = false;
	if (hFile){
		char* sz="HeyBoy!";
		DWORD dwWriteBytes = 0;
		if(WriteFile(hFile, sz, strlen(sz), &dwWriteBytes, NULL) == true){
			bRes = true;
		}
		CloseHandle(hFile);
		DeleteFileA(szFilePath);
	}
	return bRes;
}

BOOL CALLBACK EnumMonitor(HMONITOR hMonitor, HDC hDC, LPRECT pRect, LPARAM lParam){
	ZoyeeUtils::CSystemInfo::MonitorInfo* pThis = (ZoyeeUtils::CSystemInfo::MonitorInfo*)lParam;
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	if(GetMonitorInfo(hMonitor,&mi)){
		ZoyeeUtils::CSystemInfo::MonitorInfo::MonitorItem item;
		item.rcMonitor = mi.rcMonitor;
		item.rcWork = mi.rcWork;
		int nIndex = pThis->GetSize();
		DISPLAY_DEVICE  DisplayDevice;
		ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
		DisplayDevice.cb = sizeof(DisplayDevice);
		if ( ! EnumDisplayDevices(NULL, nIndex, &DisplayDevice, 0)){
			return false;
		}
		wcscpy(item.szDisplayName, DisplayDevice.DeviceString);
		wcscpy(item.szRegisterName, DisplayDevice.DeviceKey);
		wcscpy(item.szName, DisplayDevice.DeviceName);
		wcscpy(item.szId, DisplayDevice.DeviceID);
		item.id = nIndex;
		pThis->AddMonitor(item);
	}   		
	return false;
};

std::vector<ZoyeeUtils::CSystemInfo::MonitorInfo::MonitorItem>& ZoyeeUtils::CSystemInfo::MonitorInfo::GetMonitor()
{
	EnumDisplayMonitors(NULL, NULL, EnumMonitor, (LPARAM)this);
	return m_vec;
}

void ZoyeeUtils::CSystemInfo::MonitorInfo::AddMonitor( MonitorInfo::MonitorItem& item )
{
	m_vec.push_back(item);
}

int ZoyeeUtils::CSystemInfo::MonitorInfo::GetSize()
{
	return m_vec.size();
}

ZoyeeUtils::CSystemInfo::MonitorInfo::MonitorItem::MonitorItem()
{
	szId[0] = 0;
	szDisplayName[0] = 0;
	szRegisterName[0] = 0;
	szName[0] = 0;
}

ZoyeeUtils::CSystemInfo::MemeryInfo::MemeryInfo()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	nTotalMeta = statex.ullTotalPhys / 1024 / 1024;
	nFreeMeta = statex.ullAvailPhys / 1024 / 1024;
}

int ZoyeeUtils::CSystemInfo::MemeryInfo::GetFree()
{
	return nFreeMeta;
}

int ZoyeeUtils::CSystemInfo::MemeryInfo::GetTatal()
{
	return nTotalMeta;
}

void GetCPUInfo(int cpuinfo[4], int tp)
{
	__asm
	{
		mov eax, tp
			cpuid
			mov edi, cpuinfo
			mov dword ptr [edi], eax
			mov dword ptr [edi+4], ebx
			mov dword ptr [edi+8], ecx
			mov dword ptr [edi+12], edx
	}
}

//CPU�ͺ�
char* GetCPUID()
{
	char cpuName[1024] = {0};
	int (*pTmp)[4] = (int(*)[4])cpuName;

	GetCPUInfo(*pTmp++, 0x80000002);
	GetCPUInfo(*pTmp++, 0x80000003);
	GetCPUInfo(*pTmp++, 0x80000004);
	_strupr_s(cpuName);
	return cpuName;
}

ZoyeeUtils::CSystemInfo::CpuInfo::CpuInfo()
{
	SYSTEM_INFO i;
	GetSystemInfo(&i);
	//printf("OemId = %d\n",i.dwOemId);
	//printf("��ҳ��С = %d\n",i.dwPageSize);
	//printf("��СѰַ�ռ� = %d\n",i.lpMinimumApplicationAddress);
	//printf("���Ѱַ�ռ� = %d\n",i.lpMaximumApplicationAddress);
	//printf("���������� = %d\n",i.dwActiveProcessorMask);
	//printf("��������Ŀ = %d\n",i.dwNumberOfProcessors);
	//printf("���������� = %d\n",i.dwProcessorType);
	//printf("�����ڴ�ռ������ = %d\n",i.dwAllocationGranularity);
	//printf("Revision = %d\n",i.wProcessorRevision );
	//printf("Level = %d\n",i.wProcessorLevel);
	nCoreCount = i.dwNumberOfProcessors;
	strcpy(szCpuName, GetCPUID());
}

char* ZoyeeUtils::CSystemInfo::CpuInfo::GetName()
{
	return szCpuName;
}

int ZoyeeUtils::CSystemInfo::CpuInfo::GetCoreCount()
{
	return nCoreCount;
}

std::string ZoyeeUtils::GetProcessPath()
{
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	return szPath;
}

std::string ZoyeeUtils::GetProcessDir()
{
	std::string strDirPath = GetProcessPath();
	int nFind = strDirPath.find_last_of('\\');
	if (nFind == -1){
		throw "GetProcessDir Error!!!!";
	}

	strDirPath = strDirPath.substr(0, nFind);
	return strDirPath;
}

std::string ZoyeeUtils::GetEnvironmentValue(std::string strKey)
{
	char sz[1] = { 0 };
	int nSize = GetEnvironmentVariableA(strKey.c_str(), sz, 0);
	if (nSize <= 0){
		return "";
	}
	else{
		char* pBuffer = new char[nSize + 1];
		memset(pBuffer, 0, nSize + 1);
		GetEnvironmentVariableA(strKey.c_str(), pBuffer, nSize);
		return pBuffer;
	}
}

std::string ZoyeeUtils::GetAppDataPath()
{
	return GetEnvironmentValue("AppData");
}

std::string ZoyeeUtils::GetTempPath()
{
	return GetEnvironmentValue("TMP").empty() ? GetEnvironmentValue("TEMP") : GetEnvironmentValue("TMP");
}

std::string ZoyeeUtils::GetUserProfilePath()
{
	return GetEnvironmentValue("USERPROFILE");
}
