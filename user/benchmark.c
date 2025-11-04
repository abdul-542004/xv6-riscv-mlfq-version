#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Benchmark program to test MLFQ scheduler
int
main(int argc, char *argv[])
{
  int pid1, pid2;
  int status;
  
  printf("=== MLFQ Scheduler Benchmark ===\n");
  printf("Starting CPU-bound and I/O-bound tests concurrently...\n\n");
  
  int start_time = uptime();
  
  // Fork first child - CPU-bound process
  pid1 = fork();
  if(pid1 == 0) {
    // Child 1: Run CPU-bound test
    char *args[] = { "cpubound", 0 };
    exec("cpubound", args);
    printf("Error: exec cpubound failed\n");
    exit(1);
  }
  
  // Fork second child - I/O-bound process
  pid2 = fork();
  if(pid2 == 0) {
    // Child 2: Run I/O-bound test
    char *args[] = { "iobound", 0 };
    exec("iobound", args);
    printf("Error: exec iobound failed\n");
    exit(1);
  }
  
  // Parent: wait for both children
  wait(&status);
  wait(&status);
  
  int end_time = uptime();
  
  printf("\n=== Benchmark Complete ===\n");
  printf("Total time: %d ticks\n", end_time - start_time);
  printf("Both CPU-bound and I/O-bound tests finished.\n");
  
  exit(0);
}
