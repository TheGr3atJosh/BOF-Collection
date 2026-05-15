#include <windows.h>
#include "bofdefs.h"
#include "base.c"
#include "fserror.h"

VOID go(IN PCHAR Buffer, IN ULONG Length)
{
	datap         parser;
	const wchar_t *targetPath = NULL;
	WIN32_FIND_DATAW fd       = {0};
	HANDLE        hFind       = INVALID_HANDLE_VALUE;
	wchar_t       targetDir[MAX_PATH] = {0};
	int           targetLen   = 0;
	int           nameLen     = 0;
	int           lastSlash   = -1;
	DWORD         dwError     = 0;
	int           i           = 0;
	int           pos         = 0;

	BeaconDataParse(&parser, Buffer, Length);
	targetPath = (const wchar_t *) BeaconDataExtract(&parser, NULL);

	if (targetPath == NULL)
	{
		BeaconPrintf(CALLBACK_ERROR, "Usage: del <target>\n");
		return;
	}

	FsNormalizeSlashesW((wchar_t *) targetPath);

	if (!bofstart()) { return; }

	targetLen = KERNEL32$lstrlenW(targetPath);
	for (i = targetLen - 1; i >= 0; i--)
	{
		if (targetPath[i] == L'\\') { lastSlash = i; break; }
	}
	if (lastSlash >= MAX_PATH - 1)
	{
		internal_printf("Target path too long\n");
		printoutput(TRUE);
		return;
	}
	if (lastSlash >= 0)
	{
		for (i = 0; i <= lastSlash; i++) { targetDir[i] = targetPath[i]; }
		targetDir[lastSlash + 1] = L'\0';
	}

	hFind = KERNEL32$FindFirstFileW(targetPath, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		char *utf8path = Utf16ToUtf8(targetPath);
		internal_printf("Could Not Find %s\n", utf8path ? utf8path : "");
		if (utf8path) { intFree(utf8path); }
		printoutput(TRUE);
		return;  // do NOT call FindClose — handle is invalid
	}

	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { continue; }

		nameLen = KERNEL32$lstrlenW(fd.cFileName);

		{
			int dirLen    = KERNEL32$lstrlenW(targetDir);
			int pathTotal = dirLen + nameLen + 1;
			wchar_t *fullPath = (wchar_t *) intAlloc(pathTotal * sizeof(wchar_t));
			if (!fullPath) { KERNEL32$FindClose(hFind); printoutput(TRUE); return; }

			pos = 0;
			for (i = 0; i < dirLen;   i++) { fullPath[pos++] = targetDir[i]; }
			for (i = 0; i < nameLen;  i++) { fullPath[pos++] = fd.cFileName[i]; }
			fullPath[pos] = L'\0';

			if (!KERNEL32$DeleteFileW(fullPath))
			{
				dwError = KERNEL32$GetLastError();
				char errMsg[256];
				FsErrorMessage(dwError, errMsg, sizeof(errMsg));
				internal_printf("%s\n", errMsg);
			}

			intFree(fullPath);
		}
	} while (KERNEL32$FindNextFileW(hFind, &fd));

	KERNEL32$FindClose(hFind);  // safe — only reached when FindFirstFileW succeeded
	printoutput(TRUE);
}
