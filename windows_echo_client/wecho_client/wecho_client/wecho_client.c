// wecho_client windows version
// 
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void init_winsock();
#define BUF_LEN 128

int main(int argc, char *argv[]) {
	int s, n, len_out;
	struct sockaddr_in server_addr;
	char buf[BUF_LEN+1];

	init_winsock();

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
	/* echo 서버의 소켓주소 구조체 작성 */
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	/* 연결요청 */
	if(connect(s, (struct sockaddr *)&server_addr, 
	    sizeof(server_addr)) < 0) {
		printf("can't connect.\n");
		exit(0);
	}
	/* 키보드 입력을 받음 */
 while (1) {
	printf("Input any string : ");
	if (fgets(buf, BUF_LEN, stdin)) {
		buf[BUF_LEN] = '\0';
		len_out = strlen(buf);
	} else {
		printf("fgets error\n");
		exit(0);
	}
	/* echo 서버로 메시지 송신 */
	if (send(s, buf, len_out, 0) <= 0) {
		printf("write error\n");
		exit(0);
	}
	
	if((n = recv(s, buf, len_out, 0)) <= 0) {
			printf("read error\n");
			exit(0);
	}
	printf("%d bytes: %s\n", strlen(buf), buf);
 }
	closesocket(s);
	return(0);
}