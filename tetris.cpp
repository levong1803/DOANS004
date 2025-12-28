#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define H 20
#define W 15

char board[H][W] = {} ;

char blocks[][4][4] = {
        // 0: I (dọc)
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        // 1: O (vuông)
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        // 2: T
        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},
        // 3: S
        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},
        // 4: Z
        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}},
        // 5: J
        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},
        // 6: L
        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}}
};

int x=4,y=0,b=1;

void gotoxy(int x, int y) {
    COORD c = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' && y+j < H)
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' )
                board[y+i][x+j] = blocks[b][i][j];
}
string getBlockColor(char blockType) {
    switch(blockType) {
        case 'I': return "\033[36m"; 
        case 'O': return "\033[33m"; 
        case 'T': return "\033[35m"; 
        case 'S': return "\033[32m"; 
        case 'Z': return "\033[31m"; 
        case 'J': return "\033[34m"; 
        case 'L': return "\033[38;5;208m"; 
        default: return "\033[0m";
    }
}
string resetColor() {
    return "\033[0m";
}
void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++)
            cout<<board[i][j];
}

bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

void removeLine(){
    int j;
    for (int i = H-2; i >0 ; i-- ){
        for (j = 0; j < W-1 ; j++)
            if (board[i][j] == ' ') break;
        if (j == W-1){
            for (int ii = i; ii >0 ; ii-- )
                for (int j = 0; j < W-1 ; j++ ) board[ii][j] = board[ii-1][j];
            i++;
            draw();
            _sleep(200);
        }
    }
}

void rotate(){
  
}

int main()
{
    srand(time(0));
    b = rand() % 7;
    system("cls");
    initBoard();
    
    while (1){
        boardDelBlock();
        
        if (canMove(0,1)) y++;
        else {
            block2Board();
            
            removeLine();
            x = 5; y = 0; b = rand() % 7;
        }
        
        block2Board();
        draw();
        Sleep(200);
    }
    
    return 0;
}

