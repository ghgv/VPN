#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <pthread.h>
#include <netdb.h>
#include "parse_xml.h"

#define IFNAMSIZ 16
#define PORT 5000
#define MAXLINE 1800

char text[400];

char buffer[1500];
char tun_buffer[1500];
int tun_fd=0;
struct ifreq ifr;

int sockfd;
struct sockaddr_in servaddr;
struct hostent *host_entry;
char pTag[] = "server";
char pTagValue[100]= {0};
char pTimer[10]={0};
char pTarget[]="10.0.0.9";  //posibble bug here!

void *receptor(void * arg){
	printf("Receiver started\n");
	while(1){


	int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL,NULL); //receive message from server 
	printf("\nRead sock %d bytes from client ", n);
	for (int i =0; i< n;i++)
    	printf("%02hhX ", buffer[i]);
	write(tun_fd,buffer,n);
	}

	return NULL;

}

void *sender(void * arg){
	printf("Sender started\n");
	while(1)
	{
    	int nread = read(tun_fd, tun_buffer, sizeof(tun_buffer));
    	printf("\n\nRead tun %d bytes from device %s\n", nread, ifr.ifr_name);
    	for (int i =0; i< nread;i++)
    		printf("%02hhX ", tun_buffer[i]);

		int s=sendto(sockfd, tun_buffer, nread, 0 , (struct sockaddr*)&servaddr, sizeof(servaddr)); 
		printf("Sent bytes: %d\n",s);
	}
	return NULL;
}

void *ping(void * arg){

	while(1){
		printf("Target %s",arg);
  		char *file = "/usr/bin/ping";
        char *arg1 = "10.0.0.1";
        execl(file, file, arg, NULL);
		
	}
	return NULL;
}

int main(int argc, char* argv[])
{
		printf("I am parent= %i\n",getpid());
	
	char command[50];
    int error = 0;
	strcpy(command, "ip tuntap add dev tun0 mode tun");
    error = system(command);
	strcpy(command, "ip addr add 10.0.0.2/24 dev tun0");
	error = system(command);
	strcpy(command, "ip link set up dev tun0");
	error = system(command);


		FILE * f;
		printf("Path: %s\n",argv[1]);
		f=fopen(argv[1],"rb");

		if(f==NULL){
			printf("File not found\n");
			exit(1);
		}
		fseek(f, 0, SEEK_END);
        int size = ftell(f);
		fseek(f, 0L, SEEK_SET);
		printf("Size of the file: %ld bytes\n", size);

	    int bytes_read= fread(text,1,size,f);
	    text[bytes_read]='\0';
	    char pTag[] = "server";
	    char pTagValue[100]= {0};
	    GetXmlTagValue(text,pTag,pTagValue);
	    printf("Server´s name: %s\n",pTagValue);

	    GetXmlTagValue(text,"timer",pTimer);
	    printf("Waiting time %s\n",pTimer);
	
		GetXmlTagValue(text,"target",pTarget);
	    printf("Ping to %s\n",pTarget);
	
	
	if(fork()==0){
        printf("I am pinger %i\n",getpid());
		pthread_t thread_4;
		pthread_create(&thread_4,NULL,ping,pTarget);
		pthread_join(thread_4,NULL);


    }
    else
	{


	tun_fd = open("/dev/net/tun", O_RDWR);
	if (tun_fd == 0)
		{
			printf("Error could not open the tun interface\n");
			exit(0);
		}
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
	strcpy(ifr.ifr_name, "tun0");
	ioctl(tun_fd, TUNSETIFF, (void *)&ifr);


    host_entry = gethostbyname(pTagValue);
    char * IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    printf("Server´s IP: %s\n", IPbuffer);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_addr.s_addr = inet_addr(IPbuffer); 
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;

	// create datagram socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	pthread_t thread_1,thread_2,thread_3;
	pthread_create(&thread_1,NULL,sender,NULL);
	pthread_create(&thread_2,NULL,receptor,NULL);
//	pthread_create(&thread_3,NULL,ping,NULL);

	pthread_join(thread_1,NULL);
	pthread_join(thread_2,NULL);
//  pthread_join(thread_3,NULL);


	close(sockfd);
	close(tun_fd);
	return 0;
	}
}
