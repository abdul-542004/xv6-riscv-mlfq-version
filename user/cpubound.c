#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Compute cube of a number (expensive multiplication)
long long
cube(int n)
{
  long long result = (long long)n * n * n;
  return result;
}

// Complex bit manipulation with multiple operations
unsigned int
bit_chaos(unsigned int n)
{
  unsigned int result = n;
  
  // Multiple rounds of bit operations
  for(int i = 0; i < 10; i++) {
    result ^= (result << 13);
    result ^= (result >> 17);
    result ^= (result << 5);
    result = (result & 0xAAAAAAAA) | ((result & 0x55555555) << 1);
    result = ~result;
    result = (result << 7) | (result >> 25);
  }
  
  return result;
}

// Heavy arithmetic operations combining multiple calculations
long long
heavy_math(int base, int iterations)
{
  long long sum = 0;
  long long product = 1;
  
  for(int i = 1; i <= iterations; i++) {
    // Cube calculations
    long long c1 = cube(base + i);
    long long c2 = cube(base - i + 1);
    
    // Multiple arithmetic operations
    sum += (c1 * i) / (i + 1) - (c2 / (i + 2));
    product = (product * (i % 100 + 1)) % 1000000007;
    
    // More complex calculations
    long long temp = (c1 % 1000) * (c2 % 1000);
    sum += temp / (i + 1);
    
    // Comparisons and conditional arithmetic
    if(c1 > c2) {
      sum += (c1 - c2) / (i + 1);
    } else {
      sum += (c2 - c1) / (i + 1);
    }
    
    // Nested loop for extra computation
    for(int j = 0; j < 5; j++) {
      temp = (temp * (j + 1)) % 999983;
      sum += temp;
    }
  }
  
  return sum + product;
}

// CPU-intensive workload: heavy mathematical computations
// with NO I/O - should move to lower priority queues
int
main(int argc, char *argv[])
{
  int outer_iterations = 15000;  // MANY outer iterations
  int inner_iterations = 2000;   // HEAVY computation per iteration
  long long total_result = 0;
  unsigned int bit_result = 0;
  struct procinfo info;
  int pid = getpid();
  
  // Print startup message only (minimal I/O)
  printf("CPU-bound test starting (PID: %d)\n", pid);
  printf("Performing heavy computations (%d x %d iterations)...\n", 
         outer_iterations, inner_iterations);
  
  int start_time = uptime();
  
  // Pure CPU-intensive computation - NO I/O operations
  // This should cause the process to use its full time slice repeatedly
  // and get demoted to lower priority queues in MLFQ
  for(int iter = 0; iter < outer_iterations; iter++) {
    // Heavy mathematical operations
    total_result += heavy_math(iter % 1000, inner_iterations);
    
    // Bit manipulation chaos (multiple rounds)
    unsigned int seed1 = (unsigned int)iter;
    unsigned int seed2 = (unsigned int)(iter + 12345);
    bit_result ^= bit_chaos(seed1 * 0x9E3779B9U);
    bit_result ^= bit_chaos(seed2 * 0x12345678U);
    
    // Additional cube calculations with comparisons
    for(int i = 0; i < 200; i++) {  // Increased from 50 to 200
      long long c1 = cube(iter + i);
      long long c2 = cube(iter - i + 1);
      
      // Multiple comparisons and arithmetic
      if(c1 > c2) {
        total_result += (c1 - c2) % 1000000;
      } else {
        total_result -= (c2 - c1) % 1000000;
      }
      
      // More bit operations
      bit_result = (bit_result << 3) ^ (bit_result >> 5) ^ c1;
    }
    
    // Prevent overflow with modulo
    total_result = total_result % 1000000007LL;
  }
  
  int end_time = uptime();
  
  // Print results only at the end (minimal I/O)
  printf("\nCPU-bound test completed (PID: %d)\n", pid);
  printf("Computation result: %d (bit result: %u)\n", 
         (int)(total_result % 1000000), bit_result);
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
