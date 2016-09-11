#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "DieWithError.h"

void error(char *msg);
int soc(int portno);
int createSocket(int portno);
void con(int portno, char *msg);
void* listn(void* sockfd);
int closecon = 1;
char buffer2[256];


int main(int argc, char *argv[])
{
  char *servPort;
  char *clnPort;
  int portno;
  int socketDs;
  char message[256], str_portno[256];
  int menu = 0;


    if (argc != 3)    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server Port No> <Your port>\n",
               argv[0]);
       exit(1);
    }

    servPort = atoi(argv[1]);             /* Server port number*/
    clnPort = atoi(argv[2]);         /* Second arg: Clients port */

    pthread_t storelist;

   // 	printf("enter server portno: ");
  	// scanf("%d",&pno);
  	// printf("enter port no");
  	// scanf("%d",&portno);

  	// storesoc = soc(portno);
    //socketDs = soc(clnPort);
      socketDs = createSocket(clnPort);
  	pthread_create(&storelist,NULL,listn,(void*)socketDs);

  	sprintf(str_portno, "%d", clnPort);

  	con(servPort, str_portno);

    while(closecon)
    {


      printf("choose an option: \n");
      printf("1:To view available ports\n");
      printf("2:connect to peer\n");
      printf("3:close\n");
      scanf("%d",&menu);

      switch(menu){
        case 1:
        // sscanf(str_portno, "%d", &portno);
        // con(pno, str_portno);
        printf("Available clients%s\n",buffer2 );
        break;
        case 2:
        //printf("connect to peer\n");
        printf("enter message: ");
        scanf("%s", message);
        printf("enter portno");
        scanf("%d",&portno);
        con(portno,message);
        break;
        case 3:
          closecon = 0;
          break;
        break;
        default:
        printf("no option selected");

      }
    }

    shutdown(socketDs, SHUT_RDWR);
    pthread_join(storelist, (void **)NULL);



}
int createSocket(int portno)
{
    int sock;                        /* socket to create */
    struct sockaddr_in echoServAddr; /* Local address */

    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    if (portno == -1)
    	return sock;
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(portno);              /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    // if (listen(sock, MAXPENDING) < 0)
    //     DieWithError("listen() failed");

    return sock;
}
void error(char *msg)
{
	perror(msg);

}

void con(int portno, char *msg)
{
	int sockfd, n, confd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];  //server reads characters from the socket connection into this buffer.

	bzero((char*)&serv_addr,sizeof(serv_addr));

	 serv_addr.sin_family=AF_INET;
	 serv_addr.sin_addr.s_addr= inet_addr("127.0.0.1");;

	 serv_addr.sin_port=htons(portno);
	 bzero(buffer, sizeof(buffer));//sets all values in the buffer to zero
	// printf("Message:%s\n", msg);
	strcpy(buffer, msg);

	sockfd = createSocket(-1);
	 if( (confd = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) ) < 0)
	 {
		error("error connecting\n");
		return;
	 }

	n = send(sockfd,buffer,sizeof(buffer), 0);
	if(n<0)
	{
		error("error writing to socket\n");
		return;
	}

	bzero(buffer,sizeof(buffer));
	n=read(sockfd,buffer, 256);
	if(n<0)
	{
		error("error reading rom socket\n");
	}
	else {
	//	printf("server: %s\n", buffer);
	strcat(buffer2,buffer);
	strcat(buffer2," ");
	}


}

void* listn(void* sockfd)
{
	int newsockfd, n;
	int listsoc = (int)sockfd;
	char buffer[256];


	if( listen(listsoc,5)< 0)
		error("error listening\n");
	else
		printf("listening\n");

	while (1){
		newsockfd = accept(listsoc, (struct sockaddr *) NULL,NULL);
		bzero(buffer,256);
		if (closecon <1)
			break;
		if (newsockfd < 0)
		{
		  	error("ERROR on accept\n");
		}
		else
		{
			printf("accepted connection\n");
		}
		n = read(newsockfd,buffer,256);
		if (n < 0)
			error("ERROR reading from socket\n");

		printf("Here is the message: %s\n",buffer);
		bzero(buffer,256);
		strcpy(buffer, "msg received");
		n = send(newsockfd,buffer,256, 0);
		if (n < 0)
			error("ERROR writing to socket\n");
	}
	printf("stopped listening\n");

}

// int soc(int portno)
// {
// 	int sockfd;//store values returned by socket system call
// 	struct sockaddr_in addr;
// 		  struct timeval t;
//
// 	 sockfd=socket(AF_INET,SOCK_STREAM,0);
// 	if(sockfd<0)
// 	 {
// 		error("error opening socket\n");
// 	 }
//
// 	if (portno == -1)
//  		return sockfd;
// 	//AF_INET is the address domain of the socket
// 	addr.sin_family=AF_INET;
//  	addr.sin_addr.s_addr=INADDR_ANY;  //any ip address can be used
//  	addr.sin_port=htons(portno);
//
//
// 	if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
// 		error("ERROR on binding\n");
//
//
//       		return sockfd;
// }
