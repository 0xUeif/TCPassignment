#include <stdio.h>      /* for printf() */
#include <sys/socket.h> /* for accept() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

void DieWithError(char *errorMessage);  /* Error handling function */

int AcceptTCPConnection(int servSock)
{
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure *          /* stores the clientDetails like portno*/
    int clientLength = 0;
    //char buffer[256];
    char buffer2[256];
    char clientDetails[1000];
    FILE *fp;
    //int noofClients =0;
    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);
    strcpy(clientDetails, "");

    /* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
           &clntLen)) < 0)
        DieWithError("accept() failed");

    /* clntSock is connected to a client! */


    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
    printf("Handling portno %d\n",echoClntAddr.sin_port);

    read(clntSock,buffer2,sizeof(buffer2));
    strcat(clientDetails, buffer2);
  //  strcat(clientDetails, " ");
    fp = fopen("/home/isako/isako/assignment/addr.txt", "a");
    fprintf(fp, "%s\n",clientDetails);
    fclose(fp);
    fp = fopen("/home/isako/isako/assignment/addr.txt", "r");
  //  long inputfilesize;
  //  fseek(fp,0,SEEK_END);
  //  inputfilesize=ftell(fp);
    rewind(fp);
    //clientDetails = malloc((inputfilesize +1) * (sizeof(char)));
    //read(clientDetails ,sizeof(char),inputfilesize ,fp);
    fscanf(fp,"%s", buffer2);
    while (!feof(fp)) {
      strcat(clientDetails, buffer2);
      strcat(clientDetails, " ");
      fscanf(fp,"%s", buffer2);
    }

    fclose(fp);
    printf("Client sent: %s\n",buffer2);

  //  write(clntSock,clientDetails, strlen(clientDetails));
    return clntSock;
}
