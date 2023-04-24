// Sender.c

#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<string.h>

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

int main ()
{
  int msqid = msgget (1235, 0666 | IPC_CREAT);
  MQ mq1,mq4;
  int i, n;

  printf ("Enter number of students: ");
  scanf ("%d", &n);
  STUD *arr = (STUD *) malloc (sizeof (STUD) * n);
  mq1.mtype = 2;
  sprintf (mq1.mdata, "%d", n);
  msgsnd (msqid, &mq1, sizeof (STUD), 0);
  mq4.mtype = 3;
  sprintf (mq4.mdata, "%d", n);
  msgsnd (msqid, &mq4, sizeof (STUD), 0);

  for (i = 0; i < n; i++)
    {
      printf ("Enter Roll No. for student %d: ", i + 1);
      scanf ("%d%*c", &(arr[i].roll));
      printf ("Enter student%d's name: ", i + 1);
      fgets (arr[i].name, sizeof (arr[i].name), stdin);
      arr[i].name[strlen (arr[i].name) - 1] = 0;
      memcpy (mq1.mdata, &arr[i], sizeof (STUD));
      msgsnd (msqid, &mq1, sizeof (arr[i]), 0);
      memcpy (mq4.mdata, &arr[i], sizeof (STUD));
      msgsnd (msqid, &mq4, sizeof (arr[i]), 0);
    }
  
  printf("\ndata sorted by name:\n");
  MQ mq2;
  STUD *arr2 = (STUD *) malloc (sizeof (STUD) * n);
  for (i = 0; i < n; i++)
  {
      msgrcv (msqid, &mq2, sizeof (STUD), 1, 0);
      memcpy (&arr2[i], mq2.mdata, sizeof (STUD));
      printf("name=%s \t roll=%d\n", arr2[i].name, arr2[i].roll);
  }

  printf("\n\nSorted data by roll no:\n");
  MQ mq3;
  STUD *arr3 = (STUD *) malloc (sizeof (STUD) * n);
  for (i = 0; i < n; i++)
  {
      msgrcv (msqid, &mq3, sizeof (STUD), 1, 0);
      memcpy (&arr3[i], mq3.mdata, sizeof (STUD));
      printf("name=%s \t roll=%d\n", arr3[i].name, arr3[i].roll);
  }

  msgctl(msqid,IPC_RMID,0);

  return 0;
}

