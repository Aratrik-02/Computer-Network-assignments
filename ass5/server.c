#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main (int ac, char **av){
  int i, j, l, n, sid, cid;
  struct sockaddr_in sad, cad;
  char sip_addr[32], msg[32];
  if (ac == 1)
    strcpy (sip_addr, "127.0.0.1");
  else
    strcpy (sip_addr, av[1]);
  sid = socket (AF_INET, SOCK_STREAM, 0);
  sad.sin_family = AF_INET;
  inet_aton(sip_addr, &(sad.sin_addr));
  sad.sin_port = htons(1234);
  bind (sid, (struct sockaddr *) &sad, sizeof (sad));
  listen (sid, 1);
  l = sizeof (cad);
  cid = accept (sid, (struct sockaddr *) &cad, &l);
  while(1)
  {
    printf("\nWaiting for data from client\n");
    read(cid,(void*)msg,sizeof(msg));
    if(strncmp(msg, "exit", 4) == 0)break;  
    msg[strcspn(msg, "\n")] = '\0';
    char new_msg[64];
    printf("Message received : %s\n",msg);
    int count = 0,i,j=0;
    for (i = 0; i < strlen(msg); i++) {
        if (msg[i] == '1') {
            count++;
            new_msg[j++] = msg[i];
            if (count == 5) {
                new_msg[j++] = '0';
                count = 0;
            }
        } else {
            count = 0;
            new_msg[j++] = msg[i];
        }
    }
    new_msg[j]='\0';
    printf("Sending result : %s\n",new_msg);
    write(cid,(void*)new_msg,j);
  }
  puts("Exiting...");
  close(sid);
  close(cid);
  return 0;
}