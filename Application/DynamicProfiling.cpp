#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <executable> [args...]" << std::endl;
        return 1;
    }

    // Prepare the command for callgrind
    std::vector<std::string> callgrindCmd = {"valgrind", "--tool=callgrind"};

    // Optionally, you can add more callgrind options here, for example:
    // callgrindCmd.push_back("--callgrind-out-file=callgrind.out.%p");
    // callgrindCmd.push_back("--cache-sim=yes");
    // callgrindCmd.push_back("--branch-sim=yes");

    // Add the executable and its arguments to the command
    for (int i = 1; i < argc; ++i) {
        callgrindCmd.push_back(argv[i]);
    }

    // Convert command vector to array of char pointers for execvp
    std::vector<char*> execArgs;
    for (auto& arg : callgrindCmd) {
        execArgs.push_back(&arg.front());
    }
    execArgs.push_back(nullptr); // execvp expects a NULL terminated array

    // Fork process
    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        std::cerr << "Failed to fork process." << std::endl;
        return 1;
    } else if (pid > 0) {
        // Parent process, wait for the child to finish
        int status;
        waitpid(pid, &status, 0);
        std::cout << "Profile completed. Analyze results with kcachegrind or similar tools." << std::endl;
    } else {
        // Child process, execute the callgrind command
        execvp("valgrind", execArgs.data());
        // If execvp returns, it failed
        std::cerr << "Failed to execute 'valgrind'." << std::endl;
        exit(1);
    }

    return 0;
}

