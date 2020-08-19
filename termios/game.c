#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#define RIGHT COLS-1   /*球所能到达的当前屏幕最大水平范围*/
#define BOTTOM LINES-1 /*球所能到达的当前屏幕最大垂直范围*/
#define BOARD_LENGTH   10  /*挡板长度*/
#define LEFT 0  /*当前屏幕的最左边*/
#define TOP 0   /*当前屏幕的最上边*/

char BALL= 'O';  /*球的形状*/
char BLANK= ' ';  /*覆盖球走过的轨迹*/
int left_board; /*挡板左侧坐标*/
int right_board; /*挡板右侧坐标*/
int is_lose=0;
int hdir;   /*控制球水平运动的变量*/
int vdir;   /*控制球垂直运动的变量*/
int pos_X;  /*球的横坐标*/
int pos_Y;  /*球的纵坐标*/
int delay=100;
void moveBall();
void init();
void control();
int set_ticker(int);

int main()
{
    //初始化 curses
    initscr();
    crmode();  /*中断模式*/
    noecho();  /*关闭回显*/
    move(6,28);
    attron(A_BOLD);
    addstr("Welcome to the BallGame!");
    move(8,20);
    attroff(A_BOLD);
    addstr("Help:");
    move(9,23);
    addstr("'N':Start a new game.");
    move(10,23);
    addstr("'Q':Quit game.");
    move(11,23);
    addstr("'KEY_LEFT' :Control baffle left  shift.");
    move(12,23);
    addstr("'KEY_RIGHT':Control baffle right shift.");
    move(13,23);
    addstr("'KEY_UP'   :Control of the ball speed.");
    move(14,23);
    addstr("'KEY_DOWN' :Control of the ball reducer.");
    int flag=1;
    char choice;
    move(16,24);
    addstr("Please choose your choice!(n/q):");
    refresh();
    choice=getch();
    while(flag){
        if(choice=='q'||choice=='Q'||choice=='n'||choice=='N')
             flag=0;
        else  choice=getch();
    }
    if(choice=='n'||choice=='N'){    /*开始游戏*/
        clear();
        move(10,25);
        addstr("BallGame will start! Are you read?");
        refresh();
        sleep(3);
        control();
    }
    else if(choice=='q'||choice=='Q'){   /*退出游戏*/
        clear();
        move(10,25);
        addstr("You quit the game successfully!");
        refresh();
        sleep(3);
        endwin();
    }
    endwin();  /*结束 curses*/
    return 0;
}
 
void init(){
    int i,j;
    clear();
    if(start_color()==OK){  /*改变球和挡板的颜色*/
        attron(A_BOLD);  /*打开粗体*/
        init_pair(1,COLOR_YELLOW,COLOR_BLACK);
        attron(COLOR_PAIR(1));
    }
    //初始球
    pos_X =22;  /*球初始的横坐标*/
    pos_Y = BOTTOM-1;  /*球初始的纵坐标*/
    //初始化球的运动方向,朝右上方运动
    hdir=1; 
    vdir=-1;
 
    //初始挡板
    left_board=20;
    right_board=left_board+BOARD_LENGTH;
    for(i=left_board;i<=right_board;i++){  /*显示挡板*/
        move(BOTTOM,i);
        addch('-');
    }
 
    //初始刷新时间
    signal(SIGALRM,moveBall);
    set_ticker(delay);
    keypad(stdscr,TRUE);  /*打开 keypad 键盘响应*/
    attroff(A_BLINK);     /*关闭 A_BLINK 属性*/
    is_lose=0;
    move(pos_Y,pos_X);
    addch(BALL);
    move(LINES-1, COLS-1);
    refresh();
    usleep(100000);  /*睡眠*/
    move(LINES-1,COLS-1);
    refresh();
}
 
void moveBall(){
    if(is_lose) return;
    signal(SIGALRM,moveBall);
    move(pos_Y,pos_X);
    addch(BLANK);
    pos_X += hdir;
    pos_Y += vdir;
    //改变球的方向时
    if(pos_X >= RIGHT) { /*当球横坐标大于右边边缘时，球反弹朝左运动*/
        hdir = -1;
        beep();   /*球撞墙时，发出声音*/
    }
    if(pos_X <= LEFT)  { /*当球横坐标大于左边边缘时，球反弹朝右运动*/
        hdir = 1;
        beep();  /*球撞墙时，发出声音*/
    }
    if(pos_Y <= TOP)   { /*当球纵坐标大于顶部边缘时，球反弹朝下运动*/
        vdir = 1;
        beep();  /*球撞墙时，发出声音*/
    }
 
    //当球在底部的时候进行额外的处理
    if(pos_Y >= BOTTOM-1){
        if(pos_X>=left_board&&pos_X<=right_board)  /*球在挡板处*/
            vdir=-1;
        else{    /*球不在挡板处*/
            is_lose=1;
            move(pos_Y,pos_X);
            addch(BALL);
            move(LINES-1, COLS-1);
            refresh();
            usleep(delay*1000);  /*睡眠*/
            move(pos_Y,pos_X);
            addch(BLANK);
            pos_X += hdir;
            pos_Y += vdir;
            move(pos_Y,pos_X);
            addch(BALL);
            move(LINES-1, COLS-1);
            refresh();
        }
    }
    //不改变球的方向时
    move(pos_Y,pos_X);
    addch(BALL);
    move(LINES-1, COLS-1);
    refresh();
}

void control(){
    init();
    int cmd;
    while (1)
    {     
        if(!is_lose){
            cmd=getch();
            if(cmd=='q'||cmd=='Q'||cmd==27) break;  //强制退出游戏
            //挡板左移
            if(cmd==KEY_LEFT){
                if(left_board>0){
                    move(BOTTOM,right_board);
                    addch(' ');
                    right_board--;
                    left_board--;                     
                    move(BOTTOM,left_board);
                    addch('-');
                    move(BOTTOM,RIGHT);
                    refresh();
                }
            }     
            //挡板右移
            else if(cmd==KEY_RIGHT){
                if(right_board<RIGHT){
                    move(BOTTOM,left_board);
                    addch(' ');
                    right_board++;
                    left_board++;
                    move(BOTTOM,right_board);
                    addch('-');
                    move(BOTTOM,RIGHT);
                    refresh();
                }
            }
            //给球加速
            else if(cmd==KEY_UP){   
                delay/=2;     
                set_ticker(delay);
            }
            //给球减速
            else if(cmd==KEY_DOWN){  
                delay*=2;
                set_ticker(delay);
            }
         
        }
        else{
            //输掉球后的处理
            int flag=1;
            char choice;
            move(8,15);
            addstr("Game Over!try again?(y/n):");
            refresh();
            choice=getch();
 
            while(flag){
                if(choice=='y'||choice=='Y'||choice=='n'||choice=='N')
                     flag=0;
                else  choice=getch();
            }
            if(choice=='y'||choice=='Y'){  /*游戏重新开始*/
                    delay=100; /*恢复球的初始速度*/
                    init();
                    continue;
            }
            else if(choice=='n'||choice=='N'){   /*结束游戏*/
                    break;
            }
        }
    }
}

//设置定时器
int set_ticker(int n_msecs){
    struct itimerval new_timeset;     
    long n_sec,n_usecs;
    n_sec=n_msecs/1000;
    n_usecs=(n_msecs%1000)*1000L;
    new_timeset.it_interval.tv_sec=n_sec;
    new_timeset.it_interval.tv_usec=n_usecs;
    new_timeset.it_value.tv_sec=n_sec;
    new_timeset.it_value.tv_usec=n_usecs;
    return setitimer(ITIMER_REAL,&new_timeset,NULL); 
}
