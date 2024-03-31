// #include <iostream>
// #include <thread>    // Add this line for thread-related functionality
// #include <chrono>

// // Function to measure
// void myFunction() {
//     // Your function logic goes here
//     // For example, let's simulate some processing time
//     std::this_thread::sleep_for(std::chrono::seconds(2));
// }

// int main() {
//     // Record the start time
//     auto startTime = std::chrono::high_resolution_clock::now();

//     // Call your function
//     myFunction();

//     // Record the end time
//     auto endTime = std::chrono::high_resolution_clock::now();

//     // Calculate the elapsed time
//     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

//     // Output the elapsed time
//     std::cout << "Function execution time: " << duration.count() / 1000.0 << " ms" << std::endl;

//     return 0;
// }

#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>

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
     //while(true){
     //    // Perform some computation on the specified core
     //    std::cout << "Thread running on CPU Core " << coreNumber << std::endl;
     //    std::this_thread::sleep_for(std::chrono::microseconds(100000));
     //}
    
        std::cout << "Thread running on CPU Core " << coreNumber << std::endl;

    // Your parallel code goes here
}

int main() {

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
    
    
    uint32_t val = 0;
    size_t val2 = val;
    
    std::cout<<"Val : "<<val<<std::endl;
    std::cout<<"Val2 : "<<val2<<std::endl;
    return 0;
}
