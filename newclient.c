#include <stdio.h> /*for printf() and fprintf()*/
#include <sys/socket.h> /*for socket() connect() send() recv()*/
#include <arpa/inet.h> /*sockaddr_in and inet_addr()*/
#include <stdlib.h> /*for atoi()*/
#include <string.h> /*for memset*/
#include <unistd.h> /*for close()*/
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define FILE_TO_SEND    "file.c" /*Size of file to send*/

void DieWithError(char *errorMessage) /*Function to handle errors*/
{
	perror (errorMessage) ;
	exit(1);
}

int main(int argc, char *argv[])
{
	int sock; //Socket Descriptor
	struct sockaddr_in ServAddr; //Echo server address
	unsigned short ServPort; //Echo server port
	char *servIP; //Server IP
	int fd;
	ssize_t len;
	off_t offset = 0;
	struct stat file_stat;
	int remain_data;
	int sent_bytes = 0;
	char file_size[256];
	
	if ((argc< 3) || (argc> 4)){
		fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",argv[0]);
		exit(1);
	}
	
	servIP = argv[1] ;

	if (argc == 4)
		ServPort = atoi(argv[3]);
	else
		ServPort = 7;
	
	//Reliable Stream socket using TCP
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError(" socket () failed") ;
	
	//Zero out structure
	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = AF_INET; //Internet Address Family
	ServAddr.sin_addr.s_addr = inet_addr(servIP); //Server IP
	ServAddr.sin_port = htons(ServPort); //Server Port
	
	//Establish the connection
	if (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
		DieWithError(" connect () failed") ;
	

        if ((fd = open(FILE_TO_SEND, O_RDONLY)) <0)
        {
                DieWithError("Error opening file");

        }
        
        if (fstat(fd, &file_stat) < 0)
        {
                DieWithError( "Error fstat");
        }
        fprintf(stdout, "File Size: %d bytes\n", (int)file_stat.st_size);
        //file_size[0] = file_stat.st_size;
        snprintf(file_size,sizeof(file_size), "%d",(int)file_stat.st_size);
        //printf("%s",file_size);
        if ((len = send(sock, file_size, sizeof(file_size), 0)) < 0)
        {
              DieWithError("Error on sending greetings");
        }
        fprintf(stdout, "Server sent %d bytes for the size\n", (int)len);
	offset = 0;
    remain_data = file_stat.st_size;
    
    while (((sent_bytes = sendfile(sock, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
        {
                fprintf(stdout, "1. Server has to send %d bytes from file's data, offset is now : 0 and remaining data = %d\n", sent_bytes,  remain_data);
                remain_data -= sent_bytes;
                fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
        }
        
	printf("\n");
	close(sock);
	exit(0);
}

