#include "lib.h"
#include "globals.h"

#define NUM_OF_MINERS 4
int main(int argc, char** argv)
{    
    int serverPid, minerPid;
	/* create writer process */
	serverPid = vfork();
	if (serverPid == 0) //server 
	{
		char *argv[] = {"./Server.out", NULL};
		execv("./Server.out", argv);
	}

	/* Create reader process */
    for(int i = 0; i < NUM_OF_MINERS; i++)
    {
		minerPid = vfork();
		if(minerPid == 0) //miners
		{
			char *argv[] = {"./Miner.out", NULL};
			execv("./Miner.out", argv);
		}
    } 
   
	// Do nothing, only parent process should get here
	pause();
    return 0;
}