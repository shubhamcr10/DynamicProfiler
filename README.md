#terminal 1

g++ -g -pthread -o test test2.cpp // check to add -lpthread -lcallgrind
Exe running with profiling added


#terminal 2


pidof <exename>

kill -SIGUSR1 <pid>   // to start profiling

kill -SIGUSR2 <pid>   // to stop profiling
