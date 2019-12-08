#include "common.h"

struct sm_msg {
    int flag;
    int sm_time;
    pthread_mutex_t sm_mutex;
    pthread_cond_t sm_cond;
};

bool heart(char *ip, int port) {
    bool ret = true;
    int fd;
    if ((fd = socket_connect(port, ip)) < 0) {
        ret = false;
    }
    close(fd);
    return ret;
}

int main() {
    int shmid;
    int heat_listen;
    int port_heart, port_master;
    char temp_ctrl[5] = {0};
    char temp_inter[5] = {0};
    char temp_client[5] = {0};
    char temp_master[5] = {0};
    char ip_master[20] = {0};
    char port_temp[5] = {0};
    port_heart = atoi(temp_client);
    port_master = atoi(temp_master);
    Max_KeepAlive_Interval = atoi(temp_inter);
    ctrl_port = atoi(temp_ctrl);
    if ((shmid = shmget(IPC_PRIVATE, sizeof(struct sm_msg), 0666|IPC_CREAT)) == -1) {
        DBG("Error in shmget: %s\n", strerror(errno));
        return -1;
    }
    if ((share_memory = (char *)shmat(shmid, 0, 0)) == NULL) {
        DBG("shmat : %s\n", strerror(errno));
        return -1;
    }
    msg = (struct sm_msg *)share_memory;
    msg->flag = 0;
    msg->sm_time = 0;
    pthread_mutexattr_init(&m_attr);
    pthread_condattr_init(&c_attr);
    pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&msg->sm_mutex, &m_attr);
    pthread_cond_init(&msg->sm_ready, &c_attr);
    int pid;
    connect_nonblock(port_master, ip_master, 1);
    if ((pid = fork()) < 0) {
        DBG("fork error\n");
        return -1;
    }
    if (pid != 0) {
        if ((heat_listen = socket_create(port_heart)) < 0) {
            DBG("Error in socket_create!");
            return -1;
        }
        while (1) {
            int fd;
            if ((fd = accept(heat_listen, NULL, NULL)) < 0) {
                DBG("accept error on heart_listen!\n");
                close(fd);
            }
            DBG("\033[35m*\033[0m ");
            fflush(stdout);
            close(fd);
        }
    } else {
        int pid1;
        if ((pid1 = fork()) < 0) {
            DBG("fork error pid1!\n");
            return -1;
        }
        if (pid1 == 0) {
            DBG("子进程监听中...%d\n", port_master);
            while (1) {
                pthread_mutex_lock(&msg->sm_mutex);
                DBG("子进程等待信号开启心跳!\n");
                fflush(stdout);
                pthread_cond_wait(&msg->sm_ready, &msg->sm_mutex);
                DBG("获得心跳信号，开始心跳 ❤　\n");
                pthread_mutex_unlock(&msg->sm_mutex);
                while (1) {
                    if (client_heart(ip_master, port_master)) {
                        DBG("\n第%d次心跳成功!\n", msg->sm_time);
                        pthread_mutex_lock(&msg->sm_mutex);
                        msg->sm_time = 0;
                        msg->flag = 0;
                        pthread_mutex_unlock(&msg->sm_mutex);
                        fflush(stdout);
                        break;       
                    } else {
                        DBG("\n第%d次心跳失败\n", msg->sm_time);
                        pthread_mutex_lock(&msg->sm_mutex);
                        msg->sm_time++;
                        pthread_mutex_unlock(&msg->sm_mutex);
                        fflush(stdout);
                    }
                    sleep(6 * msg->sm_time);
                    if (msg->sm_time > Max_KeepAlive_Interval) msg->sm_time = Max_KeepAlive_Interval;
                    pthread_mutex_unlock(&msg->sm_mutex);
                }
            } 
        } else {
            DBG("孙子进程\n");
            int x = 0;
            int pid2;
            for (int i = 100; i < 106; i++) {
                x = i;
                if ((pid2 = fork()) < 0) {
                    DBG("error fork pid2\n");
                    continue;
                }
                if (pid2 == 0) break;
            }
            if (pid2 == 0) {
                sys_detect(x);
            } else {
                DBG("Father!\n");
                int client_listen;
                if ((client_listen = socket_create(ctrl_port)) < 0) {
                    DBG("create client_listen error : %s\n", strerror(errno));
                }
                DBG("client_listen <<----->>\n");
                while (1) {
                    DBG("输出sock_crtl\n");
                    if ((sock_ctrl = accept(client_listen, NULL, NULL)) < 0) {
                        DBG("Error in accept client_listen : %s\n", strerror(errno));
                        continue;
                    }
                    DBG("sock_ctrl connect\n");
                    fflush(stdout);
                    DBG("\033[35maccept success!\033[0m ");
                    fflush(stdout);
                    pthread_mutex_lock(&msg->sm_mutex);
                    msg->flag = 0;
                    pthread_mutex_unlock(&msg->sm_mutex);
                    close(sock_ctrl);
                }
            }
        }
    }
    return 0;
}
