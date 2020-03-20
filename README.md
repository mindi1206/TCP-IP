# TCP-IP
# HOW TO USE

* file_download_server.c / file_download_client.c
	* discription\
			server 에 저장되어있는 파일 중 하나를 client에 저장하는 코드 입니다.\
			client 에서 파일이름을 입력받아 서버에 있으면 다운로드, 없으면 소켓을 닫습니다.\
			** .txt파일만 가능
			
  * usage
	```c
		gcc file_download_server.c -o file_download_server
		./file_download_server 5241
	```
	```c
		gcc file_download_client.c -o file_download_client
		./file_download_client 127.0.0.1 5241
	```
  * 실행결과
* file_upload_server.c / file_upload_client.c
	* discription\
			client 에 저장되어있는 파일 중 하나를 server에 저장하는 코드 입니다.\
			client 에서 파일이름을 입력받아 있으면 업로드, 없으면 소켓을 닫습니다.\
			** .txt파일만 가능
 
	* usage
	```c
		gcc file_upload_server.c -o file_upload_server
		./file_upload_server 5241
	```
	```c
		gcc file_upload_client.c -o file_upload_client
		./file_upload_client 127.0.0.1 5241
	```
	
* jpg_upload_server.c / jpg_upload_client.c
	* discripion\
	.txt  .jpg, .wav 등 다양한 형식의 파일을 업로드하는 파일입니다.
	
	* usage
	
