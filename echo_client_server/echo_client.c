/*--------------------------------------------------------------------------------------------------------
 파일명 : echo_client.c 
 기  능 : echo 서비스를 요구하는 TCP(연결형) 클라이언트
 컴파일 : cc -o echo_client echo_client.c -lsocket -lnsl 
 사용법 : echo_client host port
--------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <unistd.h>
#define BUF_LEN 128

int main(int argc, char *argv[]) {
	int s, n, len_in, len_out;
	struct sockaddr_in server_addr;
	char *haddr;
	char buf[BUF_LEN+1];

	if(argc < 3) {
		printf("usage: %s host port\n", argv[0]);
		exit(0);
	}
	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
	/* echo 서버의 소켓주소 구조체 작성 */
	bzero((char *)&server_addr, sizeof(server_addr));
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
	printf("Input any string : ");
	if (fgets(buf, BUF_LEN, stdin)) {
		buf[BUF_LEN] = '\0';
		len_out = strlen(buf);
	} else {
		printf("fgets error\n");
		exit(0);
	}
	/* echo 서버로 메시지 송신 */
	if (send(s, buf, len_out, 0) < 0) {
		printf("write error\n");
		exit(0);
	}
	
	if((n = recv(s, buf, len_out, 0)) < 0) {
			printf("read error\n");
			exit(0);
	}
	/* 수신된 echo 메시지 화면출력 */
	//for(len_in=0,n = 0; len_in < len_out; len_in += n) {
	//	if((n = recv(s, &buf[len_in], len_out - len_in, 0)) < 0) {
	//		printf("read error\n");
	//		exit(0);
	//	}
	//}
	printf("%d bytes: %s\n", strlen(buf), buf);
	close(s);
	return(0);
}

