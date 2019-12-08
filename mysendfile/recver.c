#include "head.h"
#include "color.h"
#include "common.h"

struct FileMsg{
    int size;
    char name[20];
    char buff[512];
};

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage:./recver port");
        exit(1);
    }
    int server_listen, sockfd, port, packet_size;
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    D("["L_RED"Main"NONE"] : Create server listen socket...\n");
    while (1) {
        struct sockaddr_in client;
        int len = sizeof(client);
        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, (socklen_t *)&len)) < 0) {
            perror("accept");
            continue;
        }
        D("["L_GREEN"Accept"NONE"] : %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        struct FileMsg packet;
        struct FileMsg pre_packet;
        struct FileMsg packet_t;
        packet_size = sizeof(struct FileMsg);
        int recv_size, flag = 0, offset = 0, num = 0, size = 0, filesize = 0, tmp = 0;
        FILE *fp;
        while (1) {
            if (flag) {
                memcpy(&packet, &pre_packet, flag);
                offset = flag;
            }
            flag = 0;
            while ((recv_size = recv(sockfd, (char *)&packet_t, packet_size, 0)) > 0) {
                if (offset + recv_size == packet_size) {
                    memcpy((char *)&packet + offset, &packet_t, recv_size);
                    offset = 0;
                    if (!num)
                        filesize = packet_t.size;
                    D("["RED"Debug"NONE"] : 整包\n");
                    break;
                }
                else if (offset + recv_size < packet_size){
                    memcpy((char *)&packet + offset, &packet_t, recv_size);
                    D("["RED"Debug"NONE"] : 拆包\n");
                    offset += recv_size;
                } else {
                    memcpy((char *)&packet + offset, &packet_t, packet_size - offset);
                    flag = recv_size - (packet_size - offset);
                    memcpy(&pre_packet, (char *)&packet_t + packet_size - offset, flag);
                    D("["RED"Debug"NONE"] : 粘包 size = %d\n", size);
                    break;
                }
            }
            num++;
            if (num == 1) {
                D("["L_PINK"Recver"NONE"] : ---> %s   (%d)\n", packet.name, packet.size);
                fp = fopen(packet.name, "wb");
                D("["L_YELLOW"Write"NONE"] : ");
            } else {
                //D(".");
            }
            offset = 0;
            fflush(stdout);
            int write_size;
            if (packet.size - size >= packet_size) {
                write_size = fwrite(packet.buff, 1, sizeof(packet.buff), fp);
            } else {
                write_size = fwrite(packet.buff, 1, packet.size - size, fp);
            }
            size += write_size;
            if (size >= filesize) {
                D("["RED"Finish"NONE"] : recv %d packets，write %d bytes\n", num, size);
                break;
            }
        }
        fclose(fp);
        close(sockfd);
    }
    return 0;
}
