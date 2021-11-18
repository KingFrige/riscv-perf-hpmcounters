#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <array>

#define read_csr_safe(reg) ({ register long __tmp asm("a0"); \
    asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define SHOW_PERFCNT(fmt, mhpmcnt) \
  printf(fmt, (int)(read_csr_safe(mhpmcounter ## mhpmcnt)));

static int counters[32];

enum StatState
{
  INIT,   // initialize the counters
  FINISH
};

int cnt_stats(int enable){
  size_t instret, cycles;

  instret = read_csr_safe(instret);
  cycles = read_csr_safe(cycle);

  return 0;
}

int topDownCntShow(int * hpmcounters){
  size_t instret, cycles;

  instret = read_csr_safe(instret);
  cycles = read_csr_safe(cycle);

  printf("instret:%d\n",               hpmcounters[ 1]);
  printf("cycles:%d\n",                hpmcounters[ 2]);

  printf("slotsIssed:%d\n",            hpmcounters[ 3]);
  printf("fetchBubbles:%d\n",          hpmcounters[ 4]);
  printf("branchRetired:%d\n",         hpmcounters[ 5]);

  printf("badResteers:%d\n",           hpmcounters[ 6]);
  printf("recoveryCycles:%d\n",        hpmcounters[ 7]);
  printf("unknowBanchCycles:%d\n",     hpmcounters[ 8]);
  printf("brMispredRetired:%d\n",      hpmcounters[ 9]);
  printf("machineClears:%d\n",         hpmcounters[10]);
  printf("iCacheStallCycles:%d\n",     hpmcounters[11]);
  printf("iTLBStallCycles:%d\n",       hpmcounters[12]);
  printf("memStallsAnyLoad:%d\n",      hpmcounters[13]);
  printf("memStallsStores:%d\n",       hpmcounters[14]);

  printf("exeport0Utilization:%d\n",   hpmcounters[15]);
  printf("exeport1Utilization:%d\n",   hpmcounters[16]);
  printf("exeport2Utilization:%d\n",   hpmcounters[17]);
  printf("exeport3Utilization:%d\n",   hpmcounters[18]);
  printf("exeport4Utilization:%d\n",   hpmcounters[19]);
  printf("noOpsExecutedCycles:%d\n",   hpmcounters[20]);
  printf("fewOpsExecutedCycles:%d\n",  hpmcounters[21]);
  printf("arithDivider_active:%d\n",   hpmcounters[22]);

  printf("robStallCycles:%d\n",        hpmcounters[29]);
  printf("memStallsL1Miss:%d\n",       hpmcounters[30]);
  printf("fpRetired:%d\n",             hpmcounters[31]);

  printf("memLatency:%d\n", 0);
  printf("memStallsL2Miss:%d\n", 0);
  printf("memStallsL3Miss:%d\n", 0);

  return 0;
}

int main(int argc, char *argv[])
{
  // // read cycle and instruction counts in user mode
  // uint64_t csr_cycle  = read_csr_safe(cycle);
  // uint64_t csr_instr  = read_csr_safe(instret);

  // // read initial value of HPMC's in user mode
  // uint64_t start_hpmc3  = read_csr_safe(hpmcounter3);
  // uint64_t start_hpmc31 = read_csr_safe(hpmcounter31);
  topDownCntShow(counters);

  // program to monitor
  system(argv[1]);

  topDownCntShow(counters);

  // read final value of HPMC's and substract initial in user mode
  // printf("Value of Event (zero'd): %d\n", read_csr_safe(hpmcounter3) - start_hpmc3);
  return 0;
}

