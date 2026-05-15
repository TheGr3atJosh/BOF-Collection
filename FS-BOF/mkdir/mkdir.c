#include <windows.h>
#include "bofdefs.h"
#include "base.c"
#include "fserror.h"

VOID go(IN PCHAR Buffer, IN ULONG Length)
{
	datap         parser;
	const wchar_t *dirPath    = NULL;
	wchar_t       *scratch    = NULL;
	int            len        = 0;
	int            i          = 0;
	int            start      = 0;
	BOOL           anyCreated = FALSE;
	BOOL           allExisted = TRUE;
	DWORD          dwError    = 0;

	BeaconDataParse(&parser, Buffer, Length);
	dirPath = (const wchar_t *) BeaconDataExtract(&parser, NULL);

	if (dirPath == NULL)
	{
		BeaconPrintf(CALLBACK_ERROR, "No directory path specified\n");
		return;
	}

	len = KERNEL32$lstrlenW(dirPath);
	if (len == 0)
	{
		BeaconPrintf(CALLBACK_ERROR, "No directory path specified\n");
		return;
	}

	if (!bofstart()) { return; }

	scratch = (wchar_t *) intAlloc((len + 1) * sizeof(wchar_t));
	if (scratch == NULL)
	{
		internal_printf("Memory allocation failed\n");
		printoutput(TRUE);
		return;
	}

	for (i = 0; i <= len; i++) { scratch[i] = dirPath[i]; }
	FsNormalizeSlashesW(scratch);

	// Strip trailing backslash to avoid a phantom empty segment at end of walk
	if (len > 0 && scratch[len - 1] == L'\\') { scratch[--len] = L'\0'; }

	// UNC path \\server\share\: skip past the share component before walking segments
	if (len >= 2 && scratch[0] == L'\\' && scratch[1] == L'\\')
	{
		int slashes = 0;
		for (i = 2; i < len; i++)
		{
			if (scratch[i] == L'\\') { slashes++; }
			if (slashes == 2) { start = i + 1; break; }
		}
	}
	else
	{
		// Drive path C:\...: skip past drive root (not creatable)
		for (i = 0; i < len; i++)
		{
			if (scratch[i] == L'\\') { start = i + 1; break; }
		}
	}

	for (i = start; i <= len; i++)
	{
		if (scratch[i] == L'\\' || scratch[i] == L'\0')
		{
			if (i == start) { continue; } // skip empty segment

			wchar_t saved = scratch[i];
			scratch[i] = L'\0';

			if (!KERNEL32$CreateDirectoryW(scratch, NULL))
			{
				dwError = KERNEL32$GetLastError();
				if (dwError != ERROR_ALREADY_EXISTS)
				{
					if (dwError == ERROR_FILENAME_EXCED_RANGE)
					{
						internal_printf("The directory or file cannot be created.\n");
						intFree(scratch);
						printoutput(TRUE);
						return;
					}
					char errMsg[256];
					FsErrorMessage(dwError, errMsg, sizeof(errMsg));
					internal_printf("%s\n", errMsg);
					intFree(scratch);
					printoutput(TRUE);
					return;
				}
			}
			else
			{
				anyCreated = TRUE;
				allExisted = FALSE;
			}

			scratch[i] = saved;
		}
	}

	if (allExisted && !anyCreated)
	{
		char *utf8Path = Utf16ToUtf8(dirPath);
		internal_printf("A subdirectory or file %s already exists.\n", utf8Path ? utf8Path : "(path)");
		if (utf8Path) { intFree(utf8Path); }
	}

	intFree(scratch);
	printoutput(TRUE);
}
