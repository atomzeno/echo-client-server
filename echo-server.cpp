#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <set>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <signal.h>
using namespace std;
#define BUFFERSIZE 1024
set <int> client_fd;
bool broadcast = false;
bool echo = false;
mutex mut;
void recvThread(int cli_sd){
	printf("Connected and recvThread is working!\n");
	char message[BUFFERSIZE];
	while(true){
		ssize_t res = recv(cli_sd, message, BUFFERSIZE - 1, 0);
		if(res==0 || res==-1){
			perror("recv failed");
            break;
		}
		message[res] = '\0';
		printf("message : %s\n",message);
		if(echo == true && broadcast == false){
			res = send(cli_sd, message, res, 0);
			if(res == 0 || res == -1){
				perror("send failed");
	        	break;
			}
		}
		if(echo == true && broadcast == true){
			mut.lock();
			printf("broadcast send to %d number of clients\n",(int)client_fd.size());
			set<int>::iterator it;
			for(it=client_fd.begin();it!=client_fd.end();){
				res = send((*it), message, res, 0);
				if(res == 0 || res == -1){
					perror("send failed");
					it = client_fd.erase(it);
	            	continue;
				}
				it++;
			}
			mut.unlock();
			if(client_fd.find(cli_sd)==client_fd.end()){
				break;
			}
		}
	}
	if(broadcast==true){
		mut.lock();
		client_fd.erase(cli_sd);
		mut.unlock();
	}
	close(cli_sd);
}
int main(int argc, char **argv) {
	//./server 60000 과 같이 입력이 들어옵니다.
	//argv[0]=./server,argv[1]=port 번호
	if (argc < 2 || argc > 4) {
		printf("echo-server:\n");
		printf("syntax : echo-server <port> [-e[-b]]\n");
		printf("sample 1 : echo-server 1234 \n");
		printf("sample 2 : echo-server 1234 -e \n");
		printf("sample 3 : echo-server 1234 -e -b \n");	
		return -1;//./server 60000 과 같은 형태가 입력이 아니면 안 됩니다.
	}
	if(argc >= 3 && (strcmp(argv[2], "-e") != 0) ){
		printf("no -e option!\n");
		return -1;
	}
	if(argc == 4 && (strcmp(argv[3], "-b")!=0)){
		printf("It should be -e -b option!\n");
		return -1;
	}
	if(argc >= 3){
		echo = true;
	}
	if(argc == 4){
		broadcast = true;
	}

	printf("Echo server program begun\n");
	int server_socket;//서버 소켓의 file descrypter입니다.
	int opt = 1;
	struct sockaddr_in server_address;//서버 소켓의 file descrypter입니다.
	server_socket = socket(AF_INET, SOCK_STREAM, 0);// 서버의 TCP 소켓을 생성하고, 파일 디스크립터를 반환해줍니다.
	if (server_socket == -1) {
		perror("socket");
		return -1;
	}
	//SOCK_STREAM은 TCP 연결 때 사용한다.
	if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))==-1){
		perror("setsockopt");
		return -1;
	}
	//setsocketopt 함수에서는 server_socket에 주소를 재 사용해도 문제가 없게 설정해줍니다.
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(atoi(argv[1]));//입력해준 argv[1] 포트 번호를 써줍니다
	if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address))==-1){
		perror("bind");
		return -1;
	}
	// 소켓에 클라이언트가 통신하는데 필요한 고정 주소인 서버 주소를  할당해 줍니다.
	if(listen(server_socket, 5)==-1){
		perror("listen");
		return -1;
	}

	while(true){
		struct sockaddr_in client_address;//클라이언트 소켓의 file descrypter입니다.
		socklen_t client_address_size = sizeof(client_address);//client_address의 크기를 저장해줍니다.
		int cli_sd = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
		if (cli_sd == -1) {
			perror("accept");
			break;
		}
		if(broadcast==true){
			mut.lock();
			client_fd.insert(cli_sd);
			mut.unlock();
		}
		thread* cl_th = new thread(recvThread, cli_sd);
		cl_th->detach();
	}
	close(server_socket);
	return 0;
}

