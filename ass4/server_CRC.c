#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>


char* crc(char* data, char* key) {
    int n_key = strlen(key);
    int n_data = strlen(data);
    char* new_data = malloc(32 * sizeof(char));

    snprintf(new_data, 32, "%s%0*d", data, n_key - 1, 0);

    for (int i = 0; i < n_data;  i++) {
        int msb = new_data[0] - '0';
        if(msb == 1)
            for(int j = 0; j < n_key; j++)
                new_data[j] = new_data[j] == key[j] ? '0' : '1'; 
        new_data++;
    }
    strcat(data,new_data);
    return data;
}

int main()
{
    struct sockaddr_un saddr, caddr;
    saddr.sun_family = AF_UNIX;
    unlink("mysocket");
    strcpy(saddr.sun_path,"mysocket");
    
    int n = 5;

    int ss = socket( AF_UNIX , SOCK_STREAM , 0);
	int success = bind (ss , (struct sockaddr*)&saddr, sizeof(saddr));
    listen(ss,n);
	socklen_t len = sizeof(caddr);
	int ts = accept(ss, (struct sockaddr*)&caddr, &len);
    char data1[32] = {0};
    char data2[32] = {0};
    long word,divisor;
    char* end;
    while(1)
	{
		printf("\nWaiting for data from client\n");
		read(ts,(void*)data1,sizeof(data1));
		read(ts,(void*)data2,sizeof(data2));
        
        if(strncmp(data1, "exit", 4) == 0 || strncmp(data2, "exit", 4) == 0 )    break;
        
        data1[strcspn(data1, "\n")] = '\0';
        data2[strcspn(data2, "\n")] = '\0';

        char result[32];
        strcpy(result,crc(data1,data2));
	    printf("Sending result\n");
	    write(ts,(void*)result,strlen(result)+1);
	}

    unlink("mysocket");
    puts("Exiting...");
	close(ts);
	close(ss);
	return 0;
}