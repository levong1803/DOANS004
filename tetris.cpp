#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cctype>  
#include <algorithm>  
#include <string> 
using namespace std;

#define H 20
#define W 15

int board[H][W] = {} ;
class Piece {
protected:
    char shape[4][4];  
public:
    Piece() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = ' ';
    }
    virtual void rotate() {

        char temp[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[i][j] = shape[i][j];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[3-j][i];
    }

    char getShape(int i, int j) const {
        return shape[i][j];
    }

    void setShape(int i, int j, char c) {
        shape[i][j] = c;
    }
    virtual ~Piece() {}
};
class PieceI : public Piece {
public:
    PieceI() {
        shape[0][1] = 'I';
        shape[1][1] = 'I';
        shape[2][1] = 'I';
        shape[3][1] = 'I';
    }
};
class PieceO : public Piece {
public:
    PieceO() {
        shape[1][1] = 'O';
        shape[1][2] = 'O';
        shape[2][1] = 'O';
        shape[2][2] = 'O';
    }
    
    void rotate() override {
    }
};
class PieceT : public Piece {
public:
    PieceT() {
        shape[1][1] = 'T';
        shape[2][0] = 'T';
        shape[2][1] = 'T';
        shape[2][2] = 'T';
    }
};
class PieceS : public Piece {
public:
    PieceS() {
        shape[1][1] = 'S';
        shape[1][2] = 'S';
        shape[2][0] = 'S';
        shape[2][1] = 'S';
    }
};
class PieceZ : public Piece {
public:
    PieceZ() {
        shape[1][0] = 'Z';
        shape[1][1] = 'Z';
        shape[2][1] = 'Z';
        shape[2][2] = 'Z';
    }
};
class PieceJ : public Piece {
public:
    PieceJ() {
        shape[1][0] = 'J';
        shape[2][0] = 'J';
        shape[2][1] = 'J';
        shape[2][2] = 'J';
    }
};
class PieceL : public Piece {
public:
    PieceL() {
        shape[1][2] = 'L';
        shape[2][0] = 'L';
        shape[2][1] = 'L';
        shape[2][2] = 'L';
    }
};
Piece* createPiece(int type) {
    switch(type) {
        case 0: return new PieceI();
        case 1: return new PieceO();
        case 2: return new PieceT();
        case 3: return new PieceS();
        case 4: return new PieceZ();
        case 5: return new PieceJ();
        case 6: return new PieceL();
        default: return new PieceI();
    }
}
int x=4,y=0,b=1;
Piece* currentPiece = nullptr;
int fallSpeed = 200; 
int linesRemoved = 0;
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
            if (i == 0 && j == 0) board[i][j] = (int)'+'; 
            else if (i == 0 && j == W-1) board[i][j] = (int)'+';
            else if (i == H-1 && j == 0) board[i][j] = (int)'+'; 
            else if (i == H-1 && j == W-1) board[i][j] = (int)'+'; 
            else if (i == 0 || i == H-1) board[i][j] = (int)'-'; 
            else if (j == 0 || j == W-1) board[i][j] = (int)'|'; 
            else board[i][j] = (int)' '; 
}
void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < H ; i++) {
        for (int j = 0 ; j < W ; j++) {
            int c = board[i][j]; 
            if (c == '+' || c == '-' || c == '|') {
                cout << "\033[37;1m" << (char)c << resetColor();
            }
            else if (c != ' ') {
                cout << getBlockColor((char)c) << "#" << resetColor();
            }
            else {
                cout << " "; 
            }
        }
        cout << endl;
    }
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
    int linesRemovedThisTurn = 0;
    for (int i = H-2; i > 0; i-- ){
        bool isFull = true;
        for (int j = 1; j < W-1; j++) {
            if (board[i][j] == (int)' ') {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            int topBorder[W];
            for (int jj = 0; jj < W; jj++) {
                topBorder[jj] = board[0][jj];
            }
            for (int ii = i; ii > 1; ii--) {
                for (int jj = 1; jj < W-1; jj++) {
                    board[ii][jj] = board[ii-1][jj];
                }
            }
            for (int jj = 1; jj < W-1; jj++) {
                board[1][jj] = (int)' ';
            }
            for (int jj = 0; jj < W; jj++) {
                board[0][jj] = topBorder[jj];
            }
            i++;
            linesRemovedThisTurn++;
            draw();
            Sleep(200);
        }
    }
}
bool canRotatePiece() {
    if (currentPiece == nullptr) return false;
    Piece* tempPiece = createPiece(b);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tempPiece->setShape(i, j, currentPiece->getShape(i, j));
    tempPiece->rotate();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (tempPiece->getShape(i, j) != ' ') {
                int tx = x + j;
                int ty = y + i;
                if (tx < 1 || tx >= W-1 || ty >= H-1) {
                    delete tempPiece;
                    return false;
                }
                int c = board[ty][tx]; 
                if (c != ' ' && c != '+' && c != '-' && c != '|') {
                    delete tempPiece;
                    return false;
                }
            }
    delete tempPiece;
    return true;
}
void rotatePiece(){
    if (currentPiece == nullptr) return;
    if (canRotatePiece()) {
        currentPiece->rotate();
    }
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

