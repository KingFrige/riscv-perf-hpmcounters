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

// How many counters do we support? (change for your micro-architecture).
#define NUM_COUNTERS (32)
//#define NUM_COUNTERS (32) maximum amount of HPMs is 32
typedef std::array<long, NUM_COUNTERS> snapshot_t;

static char const*               counter_names[NUM_COUNTERS];
static snapshot_t                init_counters;
static snapshot_t                tmp_counters;
static snapshot_t                finish_counters;
static std::vector <snapshot_t>  counters;

enum StatState
{
   INIT,   // initialize the counters
   FINISH, // print the total CPI
   MAX
};

int topDownCntShow(snapshot_t hpmcounters){

  printf("instret:%d\n",               hpmcounters[ 0]);
  printf("cycles:%d\n",                hpmcounters[ 1]);
  printf("time:%d\n",                  hpmcounters[ 2]);

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

static int cnt_stats(int enable)
{
   long tsc_start = read_csr_safe(cycle);
   long irt_start = read_csr_safe(instret);

   sigset_t sig_set;
   sigemptyset(&sig_set);
   sigaddset(&sig_set, SIGTERM);

   if (sigprocmask(SIG_BLOCK, &sig_set, NULL) < 0) {
      perror ("sigprocmask failed");
      return 1;
   }

   static size_t step = 0; // increment every time handle_stats is called

   int i = 0;         
   snapshot_t snapshot;
#define READ_CTR(name) do { \
      if (i < NUM_COUNTERS) { \
         long csr = read_csr_safe(name); \
         tmp_counters[i] = csr; \
         if (enable == INIT)   { init_counters[i] = csr; finish_counters[i] = 0; counter_names[i] = #name; } \
         if (enable == FINISH) { finish_counters[i] = csr - init_counters[i]; } \
         i++; \
      } \
   } while (0)

   // Since most processors will not support all 32 HPMs, comment out which hpm counters you don't want to track.
   READ_CTR(cycle);
   READ_CTR(instret);
   READ_CTR(time);
   READ_CTR(hpmcounter3);
   READ_CTR(hpmcounter4);
   READ_CTR(hpmcounter5);
   READ_CTR(hpmcounter6);
   READ_CTR(hpmcounter7);
   READ_CTR(hpmcounter8);
   READ_CTR(hpmcounter9);
   READ_CTR(hpmcounter10);
   READ_CTR(hpmcounter11);
   READ_CTR(hpmcounter12);
   READ_CTR(hpmcounter13);
   READ_CTR(hpmcounter14);
   READ_CTR(hpmcounter15);
   READ_CTR(hpmcounter16);
   READ_CTR(hpmcounter17);
   READ_CTR(hpmcounter18);
   READ_CTR(hpmcounter19);
   READ_CTR(hpmcounter20);
   READ_CTR(hpmcounter21);
   READ_CTR(hpmcounter22);
   READ_CTR(hpmcounter23);
   READ_CTR(hpmcounter24);
   READ_CTR(hpmcounter25);
   READ_CTR(hpmcounter26);
   READ_CTR(hpmcounter27);
   READ_CTR(hpmcounter28);
   READ_CTR(hpmcounter29);
   READ_CTR(hpmcounter30);
   READ_CTR(hpmcounter31);

   //printf("Snapshot Time in cycles : %ld\n", read_csr_safe(cycle) - tsc_start);
   //printf("Snapshot Time in instret: %ld\n", read_csr_safe(instret) - irt_start);

#undef READ_CTR
   if (enable == FINISH) { 

     // printf("init_counter\n\n");
     // topDownCntShow(init_counters);
     // printf("====================================\n\n");
     // printf("tmp_counter\n\n");
     // topDownCntShow(tmp_counters);
     // printf("====================================\n\n");
     // printf("finish_counter\n\n");

     topDownCntShow(finish_counters);

     //printf("Print Time in cycles : %ld\n", read_csr_safe(cycle) - tsc_start);
     //printf("Print Time in instret: %ld\n", read_csr_safe(instret) - irt_start);
   }

   if (sigprocmask(SIG_UNBLOCK, &sig_set, NULL) < 0) {
      perror ("sigprocmask unblock failed");
      return 1;
   }

   return 0;
}

int main(int argc, char *argv[])
{
  cnt_stats(INIT);

  // program to monitor
  system(argv[1]);

  cnt_stats(FINISH);

  return 0;
}

