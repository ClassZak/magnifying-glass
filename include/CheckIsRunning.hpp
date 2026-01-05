#include <windows.h>



// Global app mutex
HANDLE g_hAppMutex = NULL;

BOOL CheckIsRunning()
{
	g_hAppMutex = CreateMutex(
		NULL,
		FALSE,
		TEXT("magnifying-glass")
	);

	if (g_hAppMutex == NULL)
		return FALSE;

	// Trying catch mutex
	DWORD waitResult = WaitForSingleObject(g_hAppMutex, 0);

	if (waitResult == WAIT_TIMEOUT)
	{
		CloseHandle(g_hAppMutex);
		g_hAppMutex = NULL;
		return TRUE;
	}
	else if (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_ABANDONED)
		return FALSE;

	CloseHandle(g_hAppMutex);
	g_hAppMutex = NULL;
	return FALSE;
}

// Функция для освобождения мьютекса при завершении программы
void ReleaseAppMutex()
{
	if (g_hAppMutex)
	{
		ReleaseMutex(g_hAppMutex);
		CloseHandle(g_hAppMutex);
		g_hAppMutex = NULL;
	}
}