/*--------------------------------------------------------------------------------------------------------
���ϸ� : file_server2.c 
��  �� : file ���� ���񽺸� �����ϴ� ����
������ : cc -w -o file_server2 file_server2.c
���� : file_server2 port
--------------------------------------------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#define BUF_LEN 128
int main(int argc, char *argv[]) {
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;			/* ���Ϲ�ȣ */
	int len, msg_size;
	char buf[BUF_LEN+1];
	unsigned int set = 1;

	if(argc != 2) {
		printf("usage: %s port\n", argv[0]);
		exit(0);
	}
	/* ���� ���� */
	if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Server: Can't open stream socket.");
		exit(0);
	}

	printf("server_fd = %d\n", server_fd);

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&set, sizeof(set));

	/* server_addr�� '\0'���� �ʱ�ȭ */
	bzero((char *)&server_addr, sizeof(server_addr));
	/* server_addr ���� */
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	/* bind() ȣ�� */
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Server: Can't bind local address.\n");
		exit(0);
	}

	/* ������ ���� ������ ���� */
	listen(server_fd, 5);

	/* iterative  echo ���� ���� */
	printf("Server : waiting connection request.\n");
	len = sizeof(client_addr);

	/* �����û�� ��ٸ� */
	while (1) 
	{
		char filename[128];
		FILE *fp;

		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
		printf("client_fd = %d\n", client_fd);
		if(client_fd < 0) {
			printf("Server: accept failed.\n");
			exit(0);
		}

		printf("Server : A client connected.\n");
		printf("From %s : %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		if (recv(client_fd, filename, 128, 0)<=0) {
			printf("filename recv error\n");
			exit(0);
		}
		printf("filename : %s\n", filename);
		if ((fp=fopen(filename, "w"))==NULL) {
			printf("file open error\n");
			exit(0);
		}
		while (1) {
			int i, len;
			msg_size = recv(client_fd, buf, BUF_LEN, 0);
			if (msg_size<=0) // end of file
				break;
			printf("read data = %d bytes : %s", msg_size, buf);
			if (fwrite(buf, msg_size, 1, fp)==NULL) {
				printf("fwrite error\n");
				break;
			}
		}
		fclose(fp);
		close(client_fd);
	}
	close(server_fd);
	return(0);
}

