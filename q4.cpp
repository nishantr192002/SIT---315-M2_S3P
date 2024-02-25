#include <chrono>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    unsigned long size = 100000000;
    srand(time(0));
    int* v1, * v2, * v3;
    auto start = high_resolution_clock::now();
    v1 = (int*)malloc(size * sizeof(int));
    v2 = (int*)malloc(size * sizeof(int));
    v3 = (int*)malloc(size * sizeof(int));
    randomVector(v1, size);
    randomVector(v2, size);

#pragma omp parallel default(none) shared(v1, v2, v3, size)
    {
#pragma omp for
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i];
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
