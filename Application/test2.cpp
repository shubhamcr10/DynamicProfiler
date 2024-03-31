#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <valgrind/callgrind.h>
#include <signal.h>

volatile sig_atomic_t toggleProfiling = 0; // Atomic flag to toggle profiling

// Signal handler for SIGUSR1 (start profiling)
void handle_sigusr1(int sig) {
    toggleProfiling = true;
    CALLGRIND_START_INSTRUMENTATION;
    CALLGRIND_TOGGLE_COLLECT; // Start collecting data
    std::cout << "Profiling started by signal." << std::endl;
}

// Signal handler for SIGUSR2 (stop profiling)
void handle_sigusr2(int sig) {
    toggleProfiling = false;
    CALLGRIND_TOGGLE_COLLECT; // Stop collecting data
    CALLGRIND_STOP_INSTRUMENTATION;
    std::cout << "Profiling stopped by signal." << std::endl;
}

// Function to be executed in parallel
void parallelFunction(int coreNumber) {
    // Create a mask with the specified CPU core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(coreNumber, &cpuset);

    // Set the thread affinity to the specified CPU core
    pthread_t currentThread = pthread_self();
    if (pthread_setaffinity_np(currentThread, sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Failed to set thread affinity." << std::endl;
        return;
    }

    while (true) {
        // Check if profiling is requested
        if (toggleProfiling) {
            // Profiling-specific code here
            std::cout << "Profiling: Doing some computation..." << std::endl;
            std::cout << "Thread running on CPU Core " << coreNumber << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(100000));
        } else {
            // Normal code here
            std::cout << "Thread running on CPU Core " << coreNumber << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(100000));
        }
    }
}

int main() {
    // Signal handling setup for start and stop signals
    struct sigaction sa_start, sa_stop;
    sa_start.sa_handler = &handle_sigusr1;
    sa_stop.sa_handler = &handle_sigusr2;
    sigemptyset(&sa_start.sa_mask);
    sigemptyset(&sa_stop.sa_mask);
    sa_start.sa_flags = 0;
    sa_stop.sa_flags = 0;
    sigaction(SIGUSR1, &sa_start, NULL);
    sigaction(SIGUSR2, &sa_stop, NULL);

    const int numThreads = 4; // Number of CPU cores or threads
    std::vector<std::thread> threads;

    // Divide the work among threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(parallelFunction, i);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
