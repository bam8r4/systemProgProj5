#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


int findHighest(int x)
{
   int input =0;
   int n = x;
   int flag = 3;
   int i =0;

   if(x == 4)
   {
      return 3;
   }
   else if(x ==3)
   {
      return 2;
   }
   else if(x <= 2)
   {
      return 0;
   }

   for(input=x-1;input>2;input--)
   {
      flag=3;
      for(i=2; i<n; i++)
      {
         if(input == i)
            continue;

         if(input % i ==0)
         {
            flag = 2;
            break;
         }
         else
         {
            continue;
         }
      }
      if(flag == 2)
         continue;
      else if(flag =3)
         break;
   }
   return input;

}
int main(int argc, char *argv[])
{
   int *ptr = NULL;
   int pid =0;
   int shmid = 0;
   key_t key = 0;
   key = 0x173686;


   //Return memory identifier.
   shmid = shmget(key,sizeof(int),0666|IPC_CREAT);
   ptr = (int *) shmat(shmid,(void*)0,0);



   if (*ptr != 0)
   {
     printf("Shared memory is not 0.\n");
     return -1;
   }

   else
     printf("Shared memory is 0.\n");


   *ptr = findHighest(atoi(argv[1]));

   printf("Value assigned to shared memory is %d.\n", *ptr);


   while(*ptr != 0)
   {
      sleep(1);
   }

   if(*ptr == 0)
   {
        printf("Memory location has been set back to 0.\n");
   }
   return 0;
}


