#include <iostream>
#include<windows.h>
#include <omp.h>
#include <iomanip>


using namespace std;

const long BLOCK_SIZE = 307160;
const long N = 100000000;

DWORD nThreads = 0;

void openMp() {
	double pi = 0;
	unsigned int start = GetTickCount();

#pragma omp parallel for schedule(dynamic, BLOCK_SIZE) num_threads(nThreads) reduction(+: pi)
		for (int i = 0; i < N; ++i) {
			double x = (i + 0.5) * (1.0 / N);
			pi += ((4.0 / (1.0 + x * x)) * (1.0 / N));
		}
	
	unsigned int end = GetTickCount();

	cout << " pi: " << setprecision(10) << pi << endl;
	cout << " Time: " << end - start << " ms" << endl;
}

int main() {
	int choose = 0;

	do {
		cout << "\n Number threads: ";
		cin >> nThreads;

		openMp();

		cout << "\n Continue (1 - Yes, 0 - No): ";
		cin >> choose;
		cout << "-------------------------------" << endl;
	} while (choose);

	return 0;
}


