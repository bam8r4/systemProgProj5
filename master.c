#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
   int *ptr = NULL;
   int shmid = 0;
   key_t key = 0;
   pid_t pid;
   int status =0;
   char *arg_list[2];

   if (strcmp(argv[1], "-h")==0)
   {
      printf("Usage: ExecutableName -n <integer>\n");
      return 0;
   }
   else
   {
     if(argv[2] == NULL)
     {
        printf("No arguments, defaulting to a value of 100\n");
        arg_list[0]="100";
     }
     else
     arg_list[0]=argv[2];

   }

   //Assigning a unique key to shared memory.
   key = 0x173686;


   //Return a shared memory identifier placed in shmid.
   shmid = shmget(key,sizeof(int),0666|IPC_CREAT);

   
   ptr = (int *) shmat(shmid,(void*)0,0);

   *ptr= 0;

   pid = fork();

   if (pid == -1)
   {
      printf("Error while attempting fork.\n");
      exit(EXIT_FAILURE);
   }

   else if(pid == 0)
   {
      printf("Worker has been execv'ed.\n");
      char * argv_list[] = {"work",arg_list[0],NULL};

      execv("work",argv_list);
   }
   else
   {
      while(*ptr == 0)
      {
         sleep(1);
      }

      if (*ptr == -1)
      {
         printf("Only valid arguments can have primes.\n");
      }

      if (*ptr != 0 && *ptr != -1)
      {
         printf("Largest prime smaller than the argument is %d\n" ,*ptr);
         *ptr=0;
      }


      if (waitpid(pid, &status, 0) > 0) {

         if (WIFEXITED(status) && !WEXITSTATUS(status))
         printf("\n");

         else if (WIFEXITED(status) && WEXITSTATUS(status)) {
            if (WEXITSTATUS(status) == 127) {


               printf("execv failed\n");
            }
            else
         printf("Worker program terminated properly.\n");
         }
         else
         printf("Worker program termination failure.\n");
      }
      else {

      printf("waitpid() failed\n");
      }
      shmdt((void *) ptr);
      shmctl(shmid, IPC_RMID, NULL);
      return 0;
   }
}


