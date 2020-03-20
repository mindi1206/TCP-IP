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

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char **argv){

    int fd, w_fd; //
    char c;
    char r_filename[50]="";
    char w_filename[50]="";

    char* txt = "";
    int file_len;
    off_t file_sz;

    int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);

	for(i=0; i<5; i++)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		if(clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i+1);
	
		r_filename[0] = '\0';
	
		// 파일이름 읽어오기
        read(clnt_sock, r_filename, sizeof(r_filename));

		printf("%s\n", r_filename);

        if((fd = open(r_filename, O_RDONLY))<0) {
                printf("THERE ARE NO FILE IN HERE..\nCLOSE THE SOCKET\n");
                write(clnt_sock, "NO" , sizeof("NO"));

                close(clnt_sock);
                close(serv_sock);

                return 0;
        }
        else{
            write(clnt_sock, "YES" , sizeof("YES"));
            printf("file discriptor number: %d\n", fd);
            
			// TO GET FILE SIZE
            // MOVE FILE POINTER TO END
            file_sz = lseek(fd, 0, SEEK_END);
            printf("file size: %d\n", (int)file_sz);

            // TO COPY content  FROM fd to buffer, ,
            // MOVE FILE POINTER TO FRONT
            lseek(fd, 0, SEEK_SET);

            file_len = (int)file_sz * sizeof(char) + 1;

            printf("assigned size for buffer: %d\n", file_len);

            txt =  (char*)malloc(file_len);

            read(fd, txt, file_len);

            printf("text content: %s\n", txt);

			// 클라이언트 소켓에 파일 크기 전송
            write(clnt_sock, (int*) &file_len, sizeof(file_len));
			// 클라이언트 소켓에 파일 내용 쓰기
            write(clnt_sock, txt , file_len);
        }

        close(clnt_sock);
    }


        close(fd);
        close(w_fd);


		close(serv_sock);

    return 0;
}







