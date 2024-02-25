#include <chrono>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <omp.h>
#include <time.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

void* parallelVectorAddition(void* arg) {
    int* v1 = (int*)arg;
    int* v2 = (int*)arg;
    int* v3 = (int*)arg;
    unsigned long size = *((unsigned long*)arg);

    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }

    pthread_exit(NULL);
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

    // Pthread implementation
    pthread_t thread;
    pthread_create(&thread, NULL, parallelVectorAddition, (void*)v3);
    pthread_join(thread, NULL);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by pthread implementation: " << duration.count() << " microseconds" << endl;

    // OpenMP implementation
    start = high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by OpenMP implementation: " << duration.count() << " microseconds" << endl;

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
