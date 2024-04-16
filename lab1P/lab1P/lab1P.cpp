#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <cstdlib> 
#include <ctime> 
#include <random>

using namespace std;

void bubbleSort(vector<int>& arr) {

    bool isSorted = false;
    
    while (!isSorted) {
        isSorted = true;
        for (int i = 0; i < arr.size() - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }
    }
}


void parallelbubbleSort(vector<int>& arr) {
    omp_set_num_threads(8);

    bool isSorted = false;

    while (!isSorted) {
        isSorted = true;

        #pragma omp parallel shared(arr, isSorted)
        {
            bool localIsSorted = true;
            #pragma omp for
            for (int i = 0; i < arr.size() - 1; ++i) {
                if (i % 2 == 0 && arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                    localIsSorted = false;
                }
                else if (i % 2 != 0 && arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                    localIsSorted = false;
                }
            }
            if (!localIsSorted) {
            #pragma omp critical
                isSorted = false;
            }
        }
    }
}



int main() {
    //int n = 50000;

    int n;
    cout << "Enter n: ";
    cin >> n;

    int min = 0; 
    int max = 100000; 
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(min, max);

    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = dist(gen);
        //cout << arr[i] << " ";
    }
    cout << endl;

    vector<int> arr_parallel = arr;
    vector<int> arr_sequential = arr;

    auto _start = chrono::high_resolution_clock::now();;
    parallelbubbleSort(arr_parallel);
    auto _stop = chrono::high_resolution_clock::now();

    auto _duration_seconds = chrono::duration_cast<chrono::seconds>(_stop - _start);
    auto _duration_milliseconds = chrono::duration_cast<chrono::milliseconds>(_stop - _start);
    cout << endl << "Parallel operating time: " << _duration_seconds.count() << " seconds ";
    cout << _duration_milliseconds.count() % 1000 << " milliseconds" << endl << endl;

    /*
    cout << "Parallel sorted array: ";
    for (int num : arr_parallel) {
        cout << num << " ";
    }
    cout << endl;
    */

    auto start = chrono::high_resolution_clock::now();;
    bubbleSort(arr_sequential);
    auto stop = chrono::high_resolution_clock::now();

    auto duration_seconds = chrono::duration_cast<chrono::seconds>(stop - start);
    auto duration_milliseconds = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << endl << "Sequential operating time: " << duration_seconds.count() << " seconds ";
    cout << duration_milliseconds.count() % 1000 << " milliseconds" << endl << endl;

    /*
    cout << "Sequential sorted array: ";
    for (int num : arr_sequential) {
        cout << num << " ";
    }
    cout << endl;
    */

    return 0;
}
