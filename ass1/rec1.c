// Receiver1.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>

typedef struct student
{
  char name[32];
  int roll;
} STUD;

typedef struct msgQueue
{
  long mtype;
  char mdata[sizeof (STUD)];
} MQ;


void selectionSort(STUD arr[], int n)
{
    int i, j, min_idx;
 
    for (i = 0; i < n-1; i++)
    {

        min_idx = i;
        for (j = i+1; j < n; j++)
          if (strcmp(arr[min_idx].name , arr[j].name) > 0)
            min_idx = j;
 

           if(min_idx != i)
           {
                STUD temp;
                strcpy(temp.name,arr[min_idx].name);
                temp.roll = arr[min_idx].roll;

                strcpy(arr[min_idx].name , arr[i].name);
                arr[min_idx].roll = arr[i].roll;

                strcpy(arr[i].name,temp.name);
                arr[i].roll = temp.roll;
           }
    }
}



int main ()
{
  int msqid = msgget (1235, 0666 | IPC_CREAT);
  MQ mq, mq1;
  int n;
  msgrcv (msqid, &mq, sizeof (STUD), 2, 0);
  sscanf (mq.mdata, "%d", &n);
  STUD *arr = (STUD *) malloc (sizeof (STUD) * n);
  int i;
  for (i = 0; i < n; i++)
    {
      msgrcv (msqid, &mq, sizeof (STUD), 2, 0);
      memcpy (&arr[i], mq.mdata, sizeof (STUD));
      printf ("name: %s\troll:%d\n", arr[i].name, arr[i].roll);
    }
    selectionSort(arr,n);
    printf("\nSending Sorted data\n");
    mq1.mtype = 1;
    for (i = 0; i < n; i++)
    {
      memcpy (mq1.mdata, &arr[i], sizeof (STUD));
      msgsnd (msqid, &mq1, sizeof (arr[i]), 0);
    }
  return 0;
}

