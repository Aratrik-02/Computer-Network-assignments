#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include <pthread.h>

#define SPORT 1234
#define DPORT 1235

typedef struct connetion_info
{
    int ts;
    struct sockaddr_in caddr;
}con;
con ci;

void* sender(void* args)
{
    int ss = ci.ts;
    char data[1000];
    while(1){
        fgets(data , sizeof(data) , stdin);
        data[strlen(data) - 1] = '\0';
        sendto( ss , (void*)data , strlen(data)+1,0,(struct sockaddr*)&(ci.caddr),sizeof(ci.caddr));
        if( !strcasecmp(data,"bye") ) break;
    }
    pthread_exit(NULL);
}   

void* reciever(void*args)
{
    int ss = ci.ts;
    int len = (sizeof(ci.caddr));
    char data[1000];
    while(1)
    {
        recvfrom(ss,(void*)data,sizeof(data),0,(struct sockaddr*)&(ci.caddr),&len);
        printf("Friend:%s\n",data);
        if( !strcasecmp(data,"bye") ) break;
    }
    pthread_exit(NULL);
}
int main(int argc , char* argv[])
{
    char sip[32] , dip[32];
    pthread_t th[2];

    
    if(argc == 1)
    {
        strcpy ( sip , "127.0.0.1");
        strcpy ( dip , "127.0.0.1");
    }
    else
    {
        strcpy ( sip , argv[1]);
        strcpy ( sip , argv[2]);
    }
    int ss = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in saddr , daddr;

    saddr.sin_family = AF_INET;
    inet_aton( sip , &(saddr.sin_addr));
    saddr.sin_port = htons(SPORT);
    bind( ss , (struct sockaddr*)&saddr , sizeof(saddr));

    daddr.sin_family = AF_INET;
    inet_aton( sip , &(daddr.sin_addr));
    daddr.sin_port = htons(DPORT);

    ci.ts = ss;
    ci.caddr = daddr;
    puts("CHAT STARTED");
    pthread_create(&th[0] , NULL , &sender , (void*)&ci);
    pthread_create(&th[1] , NULL , &reciever , (void*)&ci);
    int i;
    for(i=0;i<2;i++)
        pthread_join(th[i],NULL);
    puts("CHAT ENDED");
    return 0;
}