#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Check if a number is prime
int
is_prime(int n)
{
  if(n <= 1)
    return 0;
  if(n <= 3)
    return 1;
  if(n % 2 == 0 || n % 3 == 0)
    return 0;
  
  for(int i = 5; i * i <= n; i += 6) {
    if(n % i == 0 || n % (i + 2) == 0)
      return 0;
  }
  return 1;
}

// CPU-intensive workload: calculate prime numbers
int
main(int argc, char *argv[])
{
  int max = 15000;  // Calculate primes up to this number
  int iterations = 50;  // Repeat the calculation many times to span multiple ticks
  int count = 0;
  struct procinfo info;
  int pid = getpid();
  
  printf("CPU-bound test starting (PID: %d)\n", pid);
  printf("Calculating prime numbers up to %d (%d iterations)...\n", max, iterations);
  
  int start_time = uptime();
  
  // Calculate primes multiple times (more CPU-intensive)
  for(int iter = 0; iter < iterations; iter++) {
    for(int i = 2; i <= max; i++) {
      if(is_prime(i)) {
        count++;
      }
    }
  }
  count = count / iterations;  // Report average count per iteration
  
  int end_time = uptime();
  
  printf("\nCPU-bound test completed (PID: %d)\n", pid);
  printf("Found %d prime numbers\n", count);
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
