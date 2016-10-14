// file_client.c : ������ �о echo ��û
#include <winsock.h>
#include <signal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

WSADATA wsadata;
int	main_socket;

void exit_callback(int sig)
{
	closesocket(main_socket);
	WSACleanup();
	exit(0);
}

void init_winsock()
{
	WORD sversion;
	u_long iMode=1;

	// winsock ����� ���� �ʼ�����
	signal(SIGINT, exit_callback);
	sversion = MAKEWORD(1,1);
	WSAStartup(sversion, &wsadata);
}

#define BUF_LEN 128

int main(int argc, char *argv[]) {
	int s, n, len_out;
	struct sockaddr_in server_addr;
	char buf[BUF_LEN+1];
	char filename[BUF_LEN];
	FILE *fp;

	init_winsock();

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("can't create socket\n");
		exit(0);
	}
	/* echo ������ �����ּ� ����ü �ۼ� */
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);


	// ���ϸ� �Է�
	printf("Enter file name : ");
	scanf("%s", filename);
	if ((fp=fopen(filename,"r"))==NULL) {
		printf("Can't open file %s\n", filename);
		exit(0);
	}
	/* �����û */
	if(connect(s, (struct sockaddr *)&server_addr, 
		sizeof(server_addr)) < 0) {
			printf("can't connect.\n");
			exit(0);
	}
	while (1) {
		//printf("Input any string : ");
		if (fgets(buf, BUF_LEN, fp)) {
			buf[BUF_LEN] = '\0';
			//len_out = strlen(buf);
		} else {
			//printf("fgets error\n");
			//exit(0);
			break;
		}
		/* echo ������ �޽��� �۽� */
		if (send(s, buf, BUF_LEN, 0) <= 0) {
			printf("write error\n");
			exit(0);
		}
		//if (strcmp(buf,"exit\n")==0)
		//	break;

		if((n = recv(s, buf, BUF_LEN, 0)) <= 0) {
			printf("read error\n");
			break;
		}
		printf("%d bytes: %s\n", n, buf);
	}
	closesocket(s);
	return(0);
}