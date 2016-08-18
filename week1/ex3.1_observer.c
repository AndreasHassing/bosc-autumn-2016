/* 

   Observation af kernetilstand 

 */

/* API for standard input/output */
#include <stdio.h>

/* Symbolic constants */
#define DAYSEC  86400   /* seconds per day   */
#define HOURSEC 3600    /* seconds per hour  */
#define MINSEC  60      /* seconds per minut */

#define MAXBUF 512     /* lenght of largest string required to read */

/* ----  CPU model ---- */
void cpumodel(void)
{
  FILE *cpuinfofile;

  char cpumodel[MAXBUF]; 
  char line[MAXBUF];

  cpuinfofile = fopen ("/proc/cpuinfo","r");
  
  while (fgets(line,MAXBUF,cpuinfofile))
    {
      if (sscanf(line,"model name : %[^\n]",cpumodel)) 
      printf("CPU model: %s\n",cpumodel);
    }

  fclose(cpuinfofile);
}

/* ---- kernel version of the operating system ---- */
void kernelversion(void)
{
  FILE *versionfile;

  char kernelversion[MAXBUF];
  char line[MAXBUF]; 

  /* Kernel version */
  versionfile = fopen("/proc/version","r");

  fgets(line,MAXBUF,versionfile);

  if (sscanf(line,"Linux version %s",kernelversion))
    printf("Kerne version: %s\n",kernelversion);

  fclose(versionfile);
}

/* ---- uptime of the machine ---- */
void uptime(void)
{
  //FILE *cpuinfofile, *versionfile;
  FILE *uptimefile;

  int uptime;
  char line[MAXBUF]; 

  uptimefile = fopen("/proc/uptime","r");
  
  fgets(line,MAXBUF,uptimefile);

  fclose(uptimefile);
  if (sscanf(line,"%d.",&uptime))
    {
      int left,days,hours,minutes,secs;
      
      days = uptime / DAYSEC;
      left = uptime - (days * DAYSEC);
	      
      hours = left / HOURSEC;
      left = left - (hours * HOURSEC);

      minutes = left / MINSEC;
      secs = left - minutes * MINSEC;
      
      printf("Uptime: %2dd:%2dh:%2dm:%2ds\n",days,hours,minutes,secs);
    }
}

double stattime_to_hours(int unsigned long long stattime) {
    return stattime / 100.0 / 60.0 / 60.0;
}

/* ---- stats of the machine ---- */
void stats(void)
{
    FILE *statfile;

    int unsigned long long usermodetime, kernelmodetime, idletime;
    int unsigned long long contextswitches;
    int unsigned long long processes_created;

    char line[MAXBUF];

    statfile = fopen("/proc/stat", "r");

    fgets(line, MAXBUF, statfile);

    sscanf(line, "cpu %llu %*d %llu %llu", &usermodetime, &kernelmodetime, &idletime);
    printf("User mode time: %.2f hours\n", stattime_to_hours(usermodetime));
    printf("Kernel mode time: %.2f hours\n", stattime_to_hours(kernelmodetime));
    printf("Idle time: %.2f hours\n", stattime_to_hours(idletime));

    while (fgets(line, MAXBUF, statfile)) {
        if (sscanf(line, "ctxt %llu", &contextswitches)) {
            printf("Context switches: %llu\n", contextswitches);
        } else if (sscanf(line, "processes %llu", &processes_created)) {
            printf("Processes created: %llu\n", processes_created);
        }
    }
        
    fclose(statfile);
}

/* ---- main program ---- */
int main (void)
{
  cpumodel();
  kernelversion();
  uptime();
  stats();
  
  return 0;
}
