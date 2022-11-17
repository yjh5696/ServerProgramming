#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define SIZE 300

int pid;


void intHandler(int signo){
		
	if(pid!=0){	
		printf("SIGNAL NUM%d\n", signo);
		printf("Chatting Program is END\n");}
//	else { wait(NULL); }
		exit(0);

}

int main(int argc, char* argv[]){

	char *message;
	char buf[300], usrname[30];
	int fd;


	caddr_t addr;
	struct stat sbuf;
	
/*	if(argc < 2){
		fprintf(stderr, "WRONG INPUT");
		exit(1);
	}*/

	if((fd = open("mchat_test",O_CREAT|O_RDWR,0777)) == -1){
			perror("FAILED _ CONNECTING");
			exit(1);
	}



	if(fstat(fd,&sbuf) == -1){
			perror("FAILED _ FSTAT");
			exit(1);
	}

	message = (char*)malloc(SIZE);
	message = (char*)mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED, fd, (off_t)0);

	printf("input username... ");
	scanf("%s", usrname);
	
	signal(SIGINT,intHandler);

	pid = fork();
	if(pid == 0){
		while(1){
			if(strcmp(message, buf) != 0){
				strcpy(buf, message);
				puts(buf);
			}
		}
		exit(1);
	}
	while(1){
		fgets(buf, sizeof(buf), stdin);
		sprintf(message, "%s: %s", usrname, buf);
		strcpy(buf, message);
	}

	close(fd);
	if(munmap(message,SIZE) == -1){
		perror("munmap");
		exit(1);
	}	
	
}
