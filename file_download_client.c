#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void error_handling(char *message);
int main(int argc, char *argv[])
{
	char filename[100]; // 서버에서 다운받는 파일의 이름
	char w_filename[100]; // 새로 저장할 파일 이름
	ssize_t a;
	int sock, recv_cnt = 0, recv_len = 0;
	char message[BUF_SIZE];
	char* txt = "";
	int str_len = 0, fd, file_len /*파일 크기*/ = 0;
	struct sockaddr_in serv_adr;
	filename[0] = '\0';

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected...........");


	printf("enter filename to download from server : ");
	scanf("%s", filename);

	write(sock, filename, strlen(filename) + 1);

	// 서버에 파일이 있는지 없는지 여부
	str_len = read(sock, message, BUF_SIZE - 1);
	message[str_len] = 0;

	printf("Is in server?: %s\n", message);

	// 파일 없음
	if (strcmp(message, "NO") == 0) {
		close(sock);
		return 0;
	}
	// 파일 있음
	else {
		// 파일 길이 받아오기
		read(sock, (int*)&file_len, sizeof(file_len));
		printf("size of file:  %d\n", file_len);

		while (recv_len < file_len) {
			recv_len = read(sock, &message[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		printf("file content: %s", message);

		// 새로 저장할 파일 이름 입력
		printf("new file name? : ");
		scanf("%s", w_filename);

		// 새로 저장할 파일 이름 입력
		fd = open(w_filename, O_CREAT | O_WRONLY | O_TRUNC);

		if (fd == -1)
			error_handling("open() error!");
		printf("file descriptor: %d \n", fd);

		// 새 파일에 내용 저장하기
		if (write(fd, message, file_len) == -1)
			error_handling("write() error!");

		// 소켓 닫기
		close(fd);
		close(sock);
		return 0;

	}
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

