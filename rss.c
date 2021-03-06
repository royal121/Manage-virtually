#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{	
	FILE *f = fopen("stories.txt", "w");
	char *vars[] = {"RSS_FEED=http://feeds.bbci.co.uk/news/technology/rss.xml", NULL};
	if (!f) {
		printf("Can't open stories.txt");
	}
	pid_t pid = fork();
	if (pid == -1) {
		printf("Can't fork process");
	}
	char *phrase = argv[1];
	if (!pid) {
		if (dup2(fileno(f), 1) == -1) {
			printf("Can't redirect Standard Output");
		}
		if (execle("/usr/bin/python", "/usr/bin/python", "rssgossip.py",phrase, NULL,vars) == -1) {
			printf("Can't run script");
		}
	}
	int pid_status;
	if (waitpid(pid, &pid_status, 0) == -1) {
		printf("Error waiting for the process");
	}
	return 1;
}
