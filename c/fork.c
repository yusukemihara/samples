#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

static char term[256];

static void ctrl_end(int i)
{
	fprintf(stderr, "client process catch ctrl's SIGCHILD\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	int status;
	pid_t pid, ppid;

	sprintf(term, "no term");

	if ((pid = fork()) > 0) {
		fprintf(stderr, "main process\n");
	} else if (pid == 0) {
		fprintf(stderr, "start client process\n");
		sprintf(term, "term001");
		if ((ppid = fork()) > 0) {
			int i;
			struct sigaction sa;

			fprintf(stderr, "continue client process\n");
			waitpid(ppid, &status, WNOHANG);

			memset(&sa, 0, sizeof(struct sigaction));
			sa.sa_handler = ctrl_end;
			if (sigaction(SIGCHLD, &sa, NULL) != 0) {
				exit(0);
			}

			for (i = 0; i < 10; i++) {
				fprintf(stderr, "client process:%d\n", i);
				sleep(1);
			}
			exit(0);
		} else if (ppid == 0) {
			int i;

			fprintf(stderr, "start ctrl process term:%s\n", term);
			for (i = 0; i < 5; i++) {
				fprintf(stderr, "ctrl process:%d\n", i);
				sleep(1);
			}
			fprintf(stderr, "end ctrl process\n");
			exit(0);
		} else {
			fprintf(stderr, "error\n");
			exit(0);
		}
	} else {
		fprintf(stderr, "error\n");
		exit(0);
	}
	wait(&status);
	fprintf(stderr, "end\n");
	return 0;
}
