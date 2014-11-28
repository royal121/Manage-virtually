#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define FILENAME    "ab.png"   /* File to save*/

void DieWithError(char *errorMessage) /*Function to handle errors*/
{
	perror (errorMessage) ;
	exit(1);
}


int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in ServAddr; /* Local address */
    struct sockaddr_in ClntAddr; /* Client address */
    unsigned short ServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */
    FILE *received_file;
    int remain_data = 0;
    char buffer[BUFSIZ];       /* Buffer for echo string */
    int file_size;                   /* Size of received message */
    int recvMsgSize;
    ssize_t len;

    if (argc != 2)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }

    ServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));   /* Zero out structure */
    ServAddr.sin_family = AF_INET;                /* Internet address family */
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    ServAddr.sin_port = htons(ServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
        DieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(ClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &ClntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");

        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(ClntAddr.sin_addr));
		
    /* Receive file size from client */
    //recv(clntSock, buffer, BUFSIZ, 0);
      //  file_size = atoi(buffer);
    if ((recv(clntSock, buffer, BUFSIZ, 0)) < 0)
        DieWithError("recv() failed");
	file_size = atoi(buffer);
	
	received_file = fopen(FILENAME, "w");
        if (received_file == NULL)
        {
                DieWithError("Failed to open file foo");
        }

        remain_data = file_size;

        while (((len = recv(clntSock, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0))
        {		
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", (int)len, remain_data);
        }
        fclose(received_file);
        close(clntSock);
    }
    
    /* NOT REACHED */
}

