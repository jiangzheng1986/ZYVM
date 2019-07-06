//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Disk Formater
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include <windows.h>

#include <stdio.h>

#define SECTOR_SIZE		512

#define BOOT_MODULE_SIZE	SECTOR_SIZE

#define KERNEL_MODULE_SIZE	(SECTOR_SIZE * 15)

#define DISK_SIZE		(100 * 1024 * 1024)

void ReadProgram(const WCHAR *pszFilename, char *pszBuffer)
{
	FILE *pFile = NULL;

	_wfopen_s(&pFile, pszFilename, L"rb");

	fseek(pFile, 0, SEEK_END);

	int nFileSize = ftell(pFile);

	fseek(pFile, 0, SEEK_SET);

	fread(pszBuffer, 1, nFileSize, pFile);

	fclose(pFile);
}

int main()
{
	char szBuf_Boot[SECTOR_SIZE];
	memset(szBuf_Boot, 0, SECTOR_SIZE);
	ReadProgram(L"./ZYOS_BOOT.bin", szBuf_Boot);

	char szBuf_Kernel[KERNEL_MODULE_SIZE];
	memset(szBuf_Kernel, 0, KERNEL_MODULE_SIZE);
	ReadProgram(L"./ZYOS_KERNEL.bin", szBuf_Kernel);

	char szBuf[SECTOR_SIZE];
	memset(szBuf, 0, SECTOR_SIZE);

	FILE *pFile = NULL;

	_wfopen_s(&pFile, L"./Disk.bin", L"wb");

	int i;
	
	int n = DISK_SIZE / SECTOR_SIZE;

	for ( i = 0; i < n; i++ )
	{
		if ( i == 0 )
		{
			fwrite(szBuf_Boot, 1, SECTOR_SIZE, pFile);			
		}
		else if ( i >= 1 && i <= 15 )
		{
			fwrite(szBuf_Kernel + (i - 1) * SECTOR_SIZE, 1, SECTOR_SIZE, pFile);			
		}
		else
		{
			fwrite(szBuf, 1, SECTOR_SIZE, pFile);
		}
	}

	fclose(pFile);

	return 0;
}