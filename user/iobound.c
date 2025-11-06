#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// Pure I/O-intensive workload: repeatedly write to a file
int
main(int argc, char *argv[])
{
  int fd;
  char c = '.';
  int iterations = 500;  // Number of I/O operations
  struct procinfo info;
  int pid = getpid();
  
  printf("I/O-bound test starting (PID: %d)\n", pid);
  
  int start_time = uptime();
  
  // Open file once
  fd = open("iotest.txt", O_CREATE | O_WRONLY);
  if(fd < 0) {
    printf("Error: cannot open iotest.txt\n");
    exit(1);
  }
  
  // Perform pure I/O operations - just write dots repeatedly
  for(int i = 0; i < iterations; i++) {
    if(write(fd, &c, 1) != 1) {
      printf("Error: write failed\n");
      close(fd);
      exit(1);
    }
  }
  
  close(fd);
  
  int end_time = uptime();
  
  // Clean up
  unlink("iotest.txt");
  
  printf("\nI/O-bound test completed (PID: %d)\n", pid);
  printf("Completed %d I/O operations\n", iterations);
  printf("Time elapsed: %d ticks\n", end_time - start_time);
  
  // Get process info
  if(getprocinfo(pid, &info) == 0) {
    printf("\nProcess Statistics:\n");
    printf("  PID: %d\n", info.pid);
    printf("  Name: %s\n", info.name);
    printf("  Priority Queue: %d\n", info.priority);
    printf("  Total CPU Ticks: %d\n", (int)info.total_ticks);
    printf("  Times Scheduled: %d\n", (int)info.num_scheduled);
  } else {
    printf("Failed to get process info\n");
  }
  
  exit(0);
}