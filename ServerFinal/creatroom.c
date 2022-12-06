#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

struct Room{
	int roomno;
	char name[30];
	char passwrd[20];
};

void connectRoom(){

	FILE* fp;

	if((fp = fopen("room_data.dat", "rb")) == NULL){
		printf("방이 아직 만들어지지 않았습니다!\n");
		return;
	}

	int noroom = 1, num;
	struct Room rec, tmp;
	while(fread(&tmp, sizeof(tmp), 1, fp) > 0)
		if(tmp.roomno != 0){
			noroom = 0;
			printf("%d, %s", tmp.roomno, tmp.name);
		}

	if(noroom)
		printf("방이 존재하지 않습니다!\n");
	else{
		printf("들어갈 방의 번호를 입력하세요(0을 입력하면 종료)>>");
		scanf("%d", &num);
		getchar();
		if(num == 0){
			fclose(fp);
			return;
		}
		fseek(fp, (num - 1) * sizeof(rec), SEEK_SET);
		fread(&rec, sizeof(rec), 1, fp);
		fclose(fp);
		chatStart(rec.name, rec.passwrd);
	}
}

void creatRoom(){

	FILE *read, *write;
	struct Room rec, tmp;
	char roomname[30], paswrd[20];

	printf("채팅방을 만듭니다.....\n");
	rec.roomno = 1;

	if((read = fopen("room_data.dat", "rb")) != NULL){
		while(fread(&tmp, sizeof(tmp), 1, read) > 0)
			if(tmp.roomno != 0) rec.roomno = tmp.roomno + 1;
		fclose(read);
	}

	write = fopen("room_data.dat", "ab");
	printf("방 이름 입력(30자 이내로 입력)>> ");
	fgets(roomname, 30, stdin);
	printf("비밀번호 입력(아무것도 입력하지 않으면 생략됨, 비밀번호는 20자 이내)>>");
	scanf("%[^\n]s", paswrd);
	int fd;
	caddr_t addr;
	struct stat statbuf;
	stat(roomname, &statbuf);
	fd = open(roomname, O_RDWR | O_CREAT, 0777);
	ftruncate(fd, 65536);
	addr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);
	munmap(addr, statbuf.st_size);
	close(fd);
	
	strcat(rec.passwrd, paswrd);
	strcat(rec.name, roomname);
	printf("방 번호: %d, 방 이름: %s\n", rec.roomno, rec.name);
	fseek(write, (rec.roomno - 1) * sizeof(rec), SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, write);
	fclose(write);

}

int chatStart(const char* roomname, const char* password){
	
	char pass[20];
	if(strcmp(password, "") != 0){
		printf("비밀번호가 존재합니다!\n");
		printf("비밀번호 입력>>");
		system("stty -echo");
		scanf("%s", pass);
		system("stty echo");
		printf("\n");
		if(strcmp(pass, password) != 0){
			printf("비밀번호가 틀렸습니다!\n");
			return 0;
		}
	}
	
	printf("%s입장합니다...\n", roomname);

	int fd;
	char usrname[20], buf[300];
	struct stat statbuf;
	stat(roomname, &statbuf);
	fd = open(roomname, O_RDWR);
	char* message = (char*)malloc(statbuf.st_size);
	message = (char*)mmap(0, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);
	printf("input username. . .");
	scanf("%s", usrname);

	int pid = fork();
	if(pid == 0){
		while(1){
			if(strcmp(message, buf) != 0){
				strcpy(buf,message);
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
	if(munmap(message, statbuf.st_size) == -1){
		perror("munmap");
		exit(1);
	}

	close(fd);
}

int main(){

	int num, roop = 1;
	while(roop){

		printf("-------------환영합니다---------------\n");
		printf("1. 채팅방 들어가기 | 2. 채팅방 생성 | 그 외 숫자를 눌러 종료>>");
		scanf("%d", &num);
		getchar();

		switch(num){
			case 1:
				printf("채팅방에 들어갑니다.....\n");
				connectRoom();
				break;
			case 2:
				creatRoom();
				break;
			default:
				roop = 0;
				break;
		}
	}

}
