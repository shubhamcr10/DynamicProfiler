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

// Profiling thread
void profilingThread() {
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

    // Continuously perform some computation
    while (true) {
        if (toggleProfiling) {
            // Profiling-specific code here
            std::cout << "Profiling: Doing some computation..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } else {
            // Non-profiling code here
            std::cout << "Normal: Doing some computation..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main() {
    // Start profiling thread
    std::thread profilingThreadObj(profilingThread);

    // Wait for user input to stop profiling
    std::cout << "Press Enter to stop profiling." << std::endl;
    std::cin.get();

    // Stop profiling
    toggleProfiling = false;

    // Wait for the profiling thread to finish
    profilingThreadObj.join();

    return 0;
}
