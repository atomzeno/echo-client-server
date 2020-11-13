#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <thread>
#include <stdlib.h>
#include <signal.h>
using namespace std;
#define BUFFERSIZE 1024
int close_fd;
void recvThread(int client_fd) {
	printf("RecvThread is working!\n");
	char buf[BUFFERSIZE];
	while (true) {
		ssize_t res = recv(client_fd, buf, BUFFERSIZE - 1, 0);
		if (res == 0 || res == -1) {
			perror("recv failed!");
			break;
		}
		buf[res] = '\0';
		printf("message sent from server : %s\n",buf);
	}
	printf("disconnected!\n");
	close(client_fd);
	exit(0);
}
int main(int argc, char **argv) {
	//usage : echo-client <ip> <port>
	//./echo-client 127.0.0.1 60000 과 같이 입력이 들어옵니다.
	//argc[0]="./echo-client", argc[1]=ip 주소, argc[2]=port 번호
	int client_fd;
	struct sockaddr_in server_address;
	if (argc != 3) {
		printf("echo-client:\n");
		printf("syntax : echo-client <ip> <port>\n");
		printf("sample : echo-client 127.0.0.1 1234\n");
		return 0;//./client, 127.0.0.1, 90000 과 같은 형태가 입력이 아니면 안 됩니다.
	}
	printf("Client program begun\n");
	client_fd = socket(AF_INET, SOCK_STREAM, 0);//서버 접속을 위한 소켓 생성
	//socket() 함수는 성공 시에 파일 디스크립터를 반환하고, 실패 시 -1을 반환합니다.
	//AF_INET은 IPv4 인터넷 프로토콜이며, SOCK_STREAM은 TCP 연결때 사용합니다.
	if(client_fd == -1){
		printf("socket failed!\n");
		return 0;//socket 생성이 실패하였다!
	}
	memset(&server_address, 0, sizeof(server_address));//server_address 구조체에 있는 값들을 초기화 해 줍니다
	server_address.sin_family = AF_INET;//AF_INET은 IPv4 인터넷 프로토콜입니다.
	//server_address.sin_addr.s_addr = inet_addr(argv[1]);//인터넷 주소 cp 즉 argc[1]을 32비트 바이너리 주소로 변환해 줍니다.
	if(!inet_aton(argv[1], &server_address.sin_addr)){
		printf("Invalid Server Ip address!\n");
		return 0;
	}
	server_address.sin_port = htons(atoi(argv[2]));
	//먼저 char 배열 형태인 argv[2]를 atoi 함수를 통하여 숫자로 바꿔주고,
	//이후에 u_short(0~65535까지 저장 가능한) 형태인 sin_port에 맞춰주기 위하여 htons 함수를 사용합니다.
	int flag=connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address));
	if(flag==-1){
		printf("Connection disabled!\n");
		return 0;
	}
	//socket() 함수에 의해서 만들어진 클라이언트 소켓지정자 client_fd를
	//connect 함수를 통해서 클라이언트의 소켓을 특정하는 file descrypter client_fdserver_address 와
	//sizeof(server_address))로 결정되는 서버의 listening socket을 연결해줍니다.
	printf("Client program connection established\n\n");
	close_fd=client_fd;
	thread thread_client_recv(recvThread, client_fd);
	thread_client_recv.detach();
	while(true){
		char message[BUFFERSIZE];
		int message_length;
		scanf("%s",message);
		message_length=strlen(message);
		ssize_t res = send(client_fd, message, message_length, 0);
		if (res == 0 || res == -1) {
			perror("send failed!");
			break;
		}
	}
	close(client_fd);
	return 0;
}

