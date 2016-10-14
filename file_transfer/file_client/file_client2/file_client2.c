// file_client2.c : ���� ����
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
	int filesize;

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

	/* �����û */
	if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Client : Can't connect to server.\n");
		exit(0);
	}
	// ���� ���ϸ��� �Է�
	printf("Enter file name : ");
	scanf("%s", filename);
	fp = fopen(filename, "r"); // read mode open
	if (fp==NULL) {
		printf("File open error\n");
		exit(0);
	}
	//fseek(fp, 0, 2);
	//filesize = ftell(fp);
	//rewind(fp);
	//printf("filesize = %d\n", filesize);
	// file name �� ������.
	if (send(s, filename, 128, 0)<=0) { // ���ϸ��� 128 byte�� ����
		printf("send error\n");
		exit(0);
	}
	// file�� ��� �о ������.
	while(1) {
		int n;
		n = fread(buf, 1, 128, fp); // ������ �о
		if (n <= 0) // End of file
			break;
		if (send(s, buf, n, 0)<=0) { // ��Ʈ��ũ�� ������.
			printf("send error\n");
			break;
		}
	}
	fclose(fp);
	closesocket(s);
}