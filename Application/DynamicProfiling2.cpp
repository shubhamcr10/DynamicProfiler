#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to run dynamic profiling using Valgrind with Callgrind
void runDynamicProfiling(const char* exePath) {
    const char* valgrindCmd = "valgrind";
    const char* callgrindCmd = "--tool=callgrind";
    // const char* outputFile = "callgrind.out"; //old
    const char* outputFile = "--callgrind-out-file=callgrind.out";

    // valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes --callgrind-out-file=callgrind.out ./video_interface Command to use
    // valgrind --tool=callgrind --callgrind-out-file=callgrind.out Command to use

    // Construct the command to run Valgrind with Callgrind
    char command[256];
    snprintf(command, sizeof(command), "%s %s %s %s", valgrindCmd, callgrindCmd, exePath, outputFile);

    // Execute the Valgrind command
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Error: Valgrind failed with status %d\n", status);
        exit(EXIT_FAILURE);
    }
    
    // Add a delay to allow Valgrind to complete (adjust the sleep duration as needed)
    // This gives Valgrind some time to finish creating the output file
    const int delaySeconds = 5;
    printf("Waiting for Valgrind to complete... (%d seconds)\n", delaySeconds);
    sleep(delaySeconds);
}

// void runDynamicProfiling(const char* exePath) {
//     const char* valgrindCmd = "valgrind";
//     const char* callgrindCmd = "--tool=callgrind";
//     const char* outputFile = "--callgrind-out-file=callgrind.out";

//     // Fork the process
//     pid_t pid = fork();

//     if (pid == -1) {
//         // Fork failed
//         perror("Fork failed");
//         exit(EXIT_FAILURE);
//     } else if (pid == 0) {
//         // Child process
//         // Redirect stdout to callgrind.out
//         freopen("callgrind.out", "w", stdout);

//         // Execute Valgrind with Callgrind using execvp
//         execlp(valgrindCmd, valgrindCmd, callgrindCmd, exePath, outputFile, (char *)NULL);

//         // If execvp fails
//         perror("Exec failed");
//         exit(EXIT_FAILURE);
//     } else {
//         // Parent process
//         // Wait for the child to complete
//         int status;
//         waitpid(pid, &status, 0);

//         if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
//             printf("Valgrind completed successfully.\n");
//         } else {
//             fprintf(stderr, "Error: Valgrind failed.\n");
//             exit(EXIT_FAILURE);
//         }
//     }
// }

// Function to generate a graph.dot file from the Callgrind output file
void generateDotFile(const char* callgrindOutputFile) {
    const char* gprof2dot = "gprof2dot";
    const char* callgrindCMD = "callgrind";
    const char* dotFile = "graph.dot";

    // gprof2dot -f callgrind -o graph.dot callgrind.out

    // Construct the command to run cg_annotate and generate the dot file
    char command[256];
    snprintf(command, sizeof(command), "%s -f %s -o %s %s", gprof2dot, callgrindCMD, dotFile, callgrindOutputFile);

    // Execute the cg_annotate command
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Error: cg_annotate failed with status %d\n", status);
        exit(EXIT_FAILURE);
    }
}

// Function to convert the dot file to a PNG file
void convertDotToPng(const char* dotFile) {
    const char* dotCmd = "dot";
    const char* pngFile = "graph.png";

    // dot -Tpng -o graph.png graph.dot 

    // Construct the command to run dot and convert the dot file to a PNG file
    char command[256];
    snprintf(command, sizeof(command), "%s -Tpng %s -o %s", dotCmd, dotFile, pngFile);

    // Execute the dot command
    int status = system(command);
    if (status != 0) {
        fprintf(stderr, "Error: dot command failed with status %d\n", status);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_executable>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* exePath = argv[1];

    // Run dynamic profiling using Valgrind with Callgrind
    runDynamicProfiling(exePath);

    // Generate the graph.dot file from the Callgrind output
    generateDotFile("callgrind.out");

    // Convert the dot file to a PNG file
    convertDotToPng("graph.dot");

    return EXIT_SUCCESS;
}
