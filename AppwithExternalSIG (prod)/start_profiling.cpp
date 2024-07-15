#include <iostream>
#include <string>
#include <array>
#include <stdexcept>
#include <csignal>
#include <unistd.h>
#include <memory>

// Function to execute shell command and get the output
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
    try {
        // Find the PID of the "testcode" executable
        std::string pid = exec("pidof testcode");
        pid.erase(pid.find_last_not_of(" \n\r\t") + 1);

        if (pid.empty()) {
            std::cerr << "No running process found for 'testcode'." << std::endl;
            return 1;
        }

        // Print the PID found
        std::cout << "Found PID: " << pid << std::endl;

        // Send the SIGUSR1 signal to the process using -s option
        std::string command = "kill -s USR1 " + pid;
        if (system(command.c_str()) == -1) {
            std::cerr << "Failed to send SIGUSR1 signal." << std::endl;
            return 1;
        }

        std::cout << "Profiling has started." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
