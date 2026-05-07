#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono> 
using namespace std;
using namespace std::chrono;

// Parallel programs
int sum_p(int n, vector<int>& arr) {
    int total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}
int average_p(int n, vector<int>& arr) {
    int total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total / n;
}
int maximum_p(int n, vector<int>& arr) {
    int max_value = arr[0];
    #pragma omp parallel for reduction(max:max_value)
    for (int i = 1; i < n; i++) {
        if (max_value < arr[i]) {
            max_value = arr[i];
        }
    } return max_value;
}

int minimum_p(int n, vector<int>& arr) {
    int min_value = arr[0];
    #pragma omp parallel for reduction(min:min_value)
    for (int i = 1; i < n; i++) {
        if (min_value > arr[i]) {
            min_value = arr[i];
        }
    }
    return min_value;
}

// Sequential programs
int sum(int n, vector<int>& arr) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total;
}
int average(int n, vector<int>& arr) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += arr[i];
    }
    return total / n;
}
int maximum(int n, vector<int>& arr) {
    int max_value = arr[0];
    for (int i = 1; i < n; i++) {
        if (max_value < arr[i]) {
            max_value = arr[i];
        }
    }
    return max_value;
}
int minimum(int n, vector<int>& arr) {
    int min_value = arr[0];
    for (int i = 1; i < n; i++) {
        if (min_value > arr[i]) {
            min_value = arr[i];
        }
    }
    return min_value;
}

int main() {
    vector<int> arr;
    int n = 600;
    
    for (int i = 0; i < n; i++) {
        arr.push_back(rand() % n);
        cout << arr[i] << " ";
    }

    cout << "\n\n--- For Sequential Execution ---\n\n";

    auto start = high_resolution_clock::now();
    int seq_sum = sum(n, arr);
    auto end = high_resolution_clock::now();
    double seq_sum_time = duration<double, nano>(end - start).count();
    cout << "Sum : " << seq_sum << "\n";
    cout << "TIME TAKEN : " << seq_sum_time << " ns\n\n";

    start = high_resolution_clock::now();
    int seq_avg = average(n, arr);
    end = high_resolution_clock::now();
    double seq_avg_time = duration<double, nano>(end - start).count();
    cout << "Average : " << seq_avg << "\n";
    cout << "TIME TAKEN : " << seq_avg_time << " ns\n\n";

    start = high_resolution_clock::now();
    int seq_max = maximum(n, arr);
    end = high_resolution_clock::now();
    double seq_max_time = duration<double, nano>(end - start).count();
    cout << "Maximum : " << seq_max << "\n";
    cout << "TIME TAKEN : " << seq_max_time << " ns\n\n";

    start = high_resolution_clock::now();
    int seq_min = minimum(n, arr);
    end = high_resolution_clock::now();
    double seq_min_time = duration<double, nano>(end - start).count();
    cout << "Minimum : " << seq_min << "\n";
    cout << "TIME TAKEN : " << seq_min_time << " ns\n\n";

    cout << "--- For Parallel Execution ---\n\n";

    start = high_resolution_clock::now();
    int par_sum = sum_p(n, arr);
    end = high_resolution_clock::now();
    double par_sum_time = duration<double, nano>(end - start).count();
    cout << "Sum : " << par_sum << "\n";
    cout << "TIME TAKEN : " << par_sum_time << " ns\n";
    cout << "Speedup Factor: " << seq_sum_time / par_sum_time << "\n\n";

    start = high_resolution_clock::now();
    int par_avg = average_p(n, arr);
    end = high_resolution_clock::now();
    double par_avg_time = duration<double, nano>(end - start).count();
    cout << "Average : " << par_avg << "\n";
    cout << "TIME TAKEN : " << par_avg_time << " ns\n";
    cout << "Speedup Factor: " << seq_avg_time / par_avg_time << "\n\n";

    start = high_resolution_clock::now();
    int par_max = maximum_p(n, arr);
    end = high_resolution_clock::now();
    double par_max_time = duration<double, nano>(end - start).count();
    cout << "Maximum : " << par_max << "\n";
    cout << "TIME TAKEN : " << par_max_time << " ns\n";
    cout << "Speedup Factor: " << seq_max_time / par_max_time << "\n\n";

    start = high_resolution_clock::now();
    int par_min = minimum_p(n, arr);
    end = high_resolution_clock::now();
    double par_min_time = duration<double, nano>(end - start).count();
    cout << "Minimum : " << par_min << "\n";
    cout << "TIME TAKEN : " << par_min_time << " ns\n";
    cout << "Speedup Factor: " << seq_min_time / par_min_time << "\n\n";

    return 0;
}
