#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define CAP 32

typedef struct MessageQueue {
        long mtype;
        char mdata[CAP];
} MQ;
int main()
{
        int mqid = msgget(1234,0666|IPC_CREAT);
        MQ mq;
        msgrcv(mqid,&mq,CAP,2,0);
        printf("Octal value = %s\n",mq.mdata);
        return 0;
}
