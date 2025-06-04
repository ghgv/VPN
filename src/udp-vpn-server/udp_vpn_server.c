// server program for udp connection 
#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <pthread.h>


#define IFNAMSIZ 16
#define PORT 5000           //Change this accoring to the client
#define MAXLINE 1800


char buffer[15000]; 
char tun_buffer[15000];
int tun_fd=0;
struct ifreq ifr;

int listenfd, len; 
struct sockaddr_in servaddr, cliaddr; 

void *receptor(void * arg){
	printf("Receiver started\n");
	while(1){


	int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr,&len); //receive message from server 
/*	printf("\nRead sock %d bytes from client ", n);
	for (int i =0; i< n;i++)
    	printf("%02hhX ", buffer[i]);*/
	write(tun_fd,buffer,n);
	}

	return NULL;

}

void *sender(void * arg){
	printf("Sender started\n");
	while(1)
	{
	//Read the tun interface
   	int nread = read(tun_fd, tun_buffer, sizeof(tun_buffer));
   	//printf("\n\nRead tun %d bytes from device %s\n", nread, ifr.ifr_name);
    //for (int i =0; i< nread;i++)
    //printf("%02hhX ", tun_buffer[i]);
	int s=sendto(listenfd, tun_buffer, nread, 0 , (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
    //printf("Sent bytes: %d\n",s);
	}
return NULL;
}

void *init(void *arg){

	char cmd[42];
    strcpy(cmd,"ip tuntap add dev tun0 mode tun");
    system(cmd);
	strcpy(cmd,"ip addr add 10.0.0.1/24 dev tun0");
	system(cmd);
	strcpy(cmd,"ip link set up dev tun0");
	system(cmd);
	while(1);
	/*char *file = "/usr/bin/ip";
    char *arg1 = "tuntap add dev tun0 mode tun";
    int e = execl(file, file, arg1, NULL);
	if (e ==-1)
		printf("Error occured\n");*/
}

int main()
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



	bzero(&servaddr, sizeof(servaddr)); 
	listenfd = socket(AF_INET, SOCK_DGRAM,0);
	printf("Server listens FD: %d with Port: %i \n",listenfd,PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;


	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

	len = sizeof(cliaddr); 

	pthread_t thread_1,thread_2,thread_3;

	pthread_create(&thread_1,NULL,sender,NULL);
	pthread_create(&thread_2,NULL,receptor,NULL);
	//pthread_create(&thread_3,NULL,init,NULL);

	pthread_join(thread_1,NULL);
	pthread_join(thread_2,NULL);
	//pthread_join(thread_3,NULL);


  close(listenfd);
  close(tun_fd);
  return 0;

}