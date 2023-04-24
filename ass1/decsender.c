#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define CAP 32

typedef struct MessageQueue {
    long mtype;
    char mdata[CAP];
} MQ;

char* decToBase(int number, int base) {
    char* converted = malloc(CAP+1);
    for(int i = CAP - 1; i >= 0; i--){
        int digit = number & (1 << base) - 1;
        number >>= base;
        if(digit > 9)
            converted[i] = 'A' + (digit - 10);
        else
            converted[i] = '0' + digit;
    }
    converted[CAP] = '\0';
    return converted;
}
int main()
{
    int num;MQ mq1,mq2,mq3;
    printf("Enter a positive decimal number: ");
    scanf("%d", &num);
    int mqid = msgget(1234,0666|IPC_CREAT);
    strcpy(mq1.mdata, decToBase(num, 1));
    mq1.mtype = 1; msgsnd(mqid,&mq1,CAP,0);
    strcpy(mq2.mdata, decToBase(num, 3));
    mq2.mtype = 2; msgsnd(mqid,&mq2,CAP,0);
    strcpy(mq3.mdata, decToBase(num, 4));
    mq3.mtype = 3; msgsnd(mqid,&mq3,CAP,0);
    msgctl(mqid,IPC_RMID,0);
    return 0;
}
