#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// I/O-intensive workload: repeatedly write and read from a file
int
main(int argc, char *argv[])
{
  int fd;
  char buf[64];
  int iterations = 200;  // Number of I/O operations
  struct procinfo info;
  int pid = getpid();
  
  printf("I/O-bound test starting (PID: %d)\n", pid);
  printf("Performing %d I/O operations...\n", iterations);
  
  int start_time = uptime();
  
  // Perform I/O operations
  for(int i = 0; i < iterations; i++) {
    // Write to file
    fd = open("iotest.txt", O_CREATE | O_WRONLY);
    if(fd < 0) {
      printf("Error: cannot open iotest.txt for writing\n");
      exit(1);
    }
    
    // Prepare data
    for(int j = 0; j < 64; j++) {
      buf[j] = 'A' + (i % 26);
    }
    
    // Write data
    if(write(fd, buf, 64) != 64) {
      printf("Error: write failed\n");
      close(fd);
      exit(1);
    }
    close(fd);
    
    // Read from file
    fd = open("iotest.txt", O_RDONLY);
    if(fd < 0) {
      printf("Error: cannot open iotest.txt for reading\n");
      exit(1);
    }
    
    // Read data
    if(read(fd, buf, 64) != 64) {
      printf("Error: read failed\n");
      close(fd);
      exit(1);
    }
    close(fd);
    
    // Small computation between I/O operations
    int sum = 0;
    for(int k = 0; k < 100; k++) {
      sum += k;
    }
  }
  
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
