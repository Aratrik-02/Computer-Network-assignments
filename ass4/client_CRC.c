#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int cs = socket(AF_UNIX, SOCK_STREAM , 0);
	struct sockaddr_un saddr;
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path,"mysocket");
    int success = connect(cs,(struct sockaddr*)&saddr,sizeof(saddr));
    perror("connect");
    char data1[32] = {0};
    char data2[32] = {0};

    while(1)
    {
        printf("Enter word or type 'exit' to exit: ");
        fgets(data1,32,stdin);
        write(cs,(void*)data1,strlen(data1)+1);
        if(strncmp(data1, "exit", 4) == 0) break;
        printf("Enter divisor or type 'exit' to exit: ");
        fgets(data2,32,stdin);
        printf("Sending data\n");
        write(cs,(void*)data2,strlen(data2)+1);

        if(strncmp(data2, "exit", 4) == 0) break;
        
        printf("Waiting for data from server\n");
        read(cs,(void*)data1,sizeof(data1));
        printf("Modified data: %s \n\n",data1);
    }
    puts("Exiting...");
    close(cs);
    return 0;
}
