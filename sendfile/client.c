#include "common.h" 

int main(int argc, char **argv) {
    int sockfd;
    char *server = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in server_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }
	FILE * fp = NULL;
	fp = fopen("pic.png","rb");
	if(fp == NULL){
        return -1;
	}
	fseek(fp, 0L, SEEK_END);
	value = ftell(fp);
    printf("value = %d\n", value);
	fseek(fp, 0L, SEEK_SET);
	Message hi;
	hi.size = value;
	strcpy(hi.name, "pic.png");
    memset(hi.buff, 0, sizeof(hi.buff));
	while (1){
		int rc = fread(hi.buff, sizeof(char), 1024, fp);
        printf("rc = %d\n", rc);
        if (rc <= 0) break;
		send(sockfd, (char *)&hi, sizeof(hi), 0);
        memset(hi.buff, 0, sizeof(hi.buff));
	}
    fclose(fp);
    close(sockfd);
    return 0;
}
