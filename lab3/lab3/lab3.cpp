#include <windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

const long BLOCK_SIZE = 307160;
const long N = 100000000;

double totalPi;
DWORD nThreads = 0;
DWORD offset = 0;
HANDLE* arrThreads;
CRITICAL_SECTION criticalSection;


DWORD WINAPI startAddr(LPVOID param) {
	int* startBlock = (int*)param;
	double x, pi;

	for (int j = 1; *startBlock < N; ++j) {
		pi = 0;
		x = 0;
		for (int i = *startBlock; (i < *startBlock + BLOCK_SIZE) && (i < N); ++i) {
			x = (i + 0.5) * (1.0 / N);
			pi += (4.0 / (1.0 + x * x)) * (1.0 / N);
		}

		EnterCriticalSection(&criticalSection);

		*startBlock = offset;
		offset += BLOCK_SIZE;
		totalPi += pi;

		LeaveCriticalSection(&criticalSection);
	}
	
	return 0;
}

void winAPI() {
	InitializeCriticalSection(&criticalSection);

	arrThreads = new HANDLE[nThreads];

	offset = BLOCK_SIZE * nThreads;

	for (int i = 0; i < nThreads; ++i)
		arrThreads[i] = CreateThread(NULL, 0, startAddr,
			new long{ i * BLOCK_SIZE }, CREATE_SUSPENDED, NULL);

	unsigned int start = GetTickCount();

	for (int i = 0; i < nThreads; ++i)
		ResumeThread(arrThreads[i]);

	WaitForMultipleObjects(nThreads, arrThreads, TRUE, INFINITE);

	unsigned int end = GetTickCount();

	cout << " pi: " << setprecision(10) << totalPi << endl;
	cout << " Time: " << end - start << " ms" << endl;

	DeleteCriticalSection(&criticalSection);

	for (int i = 0; i < nThreads; ++i)
		CloseHandle(arrThreads[i]);

	delete[] arrThreads;
	arrThreads = nullptr;
	totalPi = 0;
}

int main() {
	int choose = 0;
	do {
		cout << "\n Number threads: ";
		cin >> nThreads;

		winAPI();

		cout << "\n Continue (1 - Yes, 0 - No): ";
		cin >> choose;
		cout << "-------------------------------" << endl;
	} while (choose);

	return 0;
}
