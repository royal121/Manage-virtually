#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[])
{	

	char phrase[30]="";
	int i = 1;
	for(i = 1;i<argc;i++){
		strcat(phrase, argv[i]);
		strcat(phrase," ");
	}
	if (execlp("python", "python", "geocode.py",phrase, NULL) == -1) {
			printf("Can't run script");
	}


	return 1;
}
