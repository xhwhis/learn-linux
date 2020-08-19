#include <curses.h>
#include <unistd.h>
#define RIGHT COLS-1   /*球所能到达的当前屏幕最大水平范围*/
#define BOTTOM LINES-1 /*球所能到达的当前屏幕最大垂直范围*/
#define BOARD_LENGTH   10  /*挡板长度*/
#define LEFT 0  /*当前屏幕的最左边*/
#define TOP 0   /*当前屏幕的最上边*/

char BALL= 'o';  /*球的形状*/
char BLANK= ' ';  /*覆盖球走过的轨迹*/
int left_board; /*挡板左侧坐标*/
int right_board; /*挡板右侧坐标*/
int pos_X;  /*球的横坐标*/
int pos_Y;  /*球的纵坐标*/
int delay=100;
void init();

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
        init();
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
    clear();
    if(start_color()==OK){  /*改变球和挡板的颜色*/
        attron(A_BOLD);  /*打开粗体*/
        init_pair(1,COLOR_YELLOW,COLOR_BLACK);
        attron(COLOR_PAIR(1));
    }
    //初始球
    pos_X =22;  /*球初始的横坐标*/
    pos_Y = BOTTOM-1;  /*球初始的纵坐标*/
 
    //初始挡板
    left_board=20;
    right_board=left_board+BOARD_LENGTH;
    for(int i=left_board;i<=right_board;i++){  /*显示挡板*/
        move(BOTTOM,i);
        addch('-');
    }
    move(pos_Y,pos_X);
    addch(BALL);
    move(LINES-1, COLS-1);
    refresh();
    usleep(100000);  /*睡眠*/
    move(LINES-1,COLS-1);
    refresh();
}
