#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

// 棋盘大小
#define ROW 3
#define COL 3

// 玩家编号
enum PlayerNumber {
    PLAYER_1, // 玩家1
    PLAYER_2  // 玩家2
};

// 棋子状态
enum ChessType {
    EMPTY,    // 空
    CHESS_1,  // 玩家1落子
    CHESS_2   // 玩家2落子
};

// 游戏状态
enum GameStatus {
    GO_ON,    // 未产生胜负
    WIN_1,    // 玩家1胜
    WIN_2,    // 玩家2胜
    DRAW      // 平局
};


// 井字棋
class Game {
private:
    ChessType board[ROW][COL];  // 棋局
    GameStatus status;          // 游戏状态（GO_ON, WIN_1, WIN_2, DRAW）
    PlayerNumber nowPlaying;    // 当前回合玩家编号（PLAYER_1, PLAYER_2）
    int turn;                   // 已经进行的回合数，从0开始

    GameStatus judge();
    void switchPlayer();

public:
    Game();

    void getBoard(ChessType b[ROW][COL]);
    GameStatus getStatus();
    PlayerNumber getNowPlaying();
    int getTurn();

    static GameStatus judge(ChessType b[ROW][COL]);
    void recieveChess(int y, int x);
    void printBoard();
    GameStatus printResult();
};

// 构造函数
// 初始玩家为PLAYER_1，初始回合为0。
Game::Game() {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            board[i][j] = EMPTY;
        }
    }
    status = GO_ON;
    nowPlaying = PLAYER_1;
    turn = 0;
}

// 获取当前棋局的副本。
// @param b 3 * 3的二维数组，用于接收副本
void Game::getBoard(ChessType b[ROW][COL]) {
    memcpy(b, board, sizeof(ChessType) * ROW * COL);
}

// 获取当前游戏状态。
// @return GO_ON, WIN_1, WIN_2, DRAW之一
GameStatus Game::getStatus() {
    return status;
}

// 获取当前回合的玩家。
// @return PLAYER_1, PLAYER_2之一
PlayerNumber Game::getNowPlaying() {
    return nowPlaying;
}

// 获取已经进行的回合数。
// @return 已经进行的回合数，以int类型表示
int Game::getTurn() {
    return turn;
}

// 切换玩家。
void Game::switchPlayer() {
    nowPlaying = nowPlaying == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}

// 放置棋子。
// @param y 纵坐标，取值0/1/2
// @param x 横坐标，取值0/1/2
// @exception 若无法落子（游戏已结束/坐标超出棋盘范围/棋子被占）则抛出const char*异常。
void Game::recieveChess(int y, int x) {
    // 判断游戏是否已经结束
    if (status != GO_ON) {
        throw "游戏已经结束";
    }
    // 判断坐标合法与否
    if (x < 0 || x > 2 || y < 0 || y > 2) {
        throw "坐标超出范围";
    }
    // 判断落子处是否被占
    if (board[y][x] != EMPTY) {
        throw "已有棋子位于该位置";
    }
    board[y][x] = nowPlaying == PLAYER_1 ? CHESS_1 : CHESS_2;
    switchPlayer();
    turn++;
    status = judge();
}

// 判断任一棋局状态。
// @param b 棋局，以3 * 3的二维数组存储
// @return WIN_1, WIN_2, DRAW, GO_ON之一
GameStatus Game::judge(ChessType b[ROW][COL]) {
    // 判断行列
    for (int i = 0; i < ROW; i++) {
        if (b[i][0] == b[i][1] && b[i][1] == b[i][2]) {
            if (b[i][0] == CHESS_1)
                return WIN_1;
            if (b[i][0] == CHESS_2)
                return WIN_2;
        }
        if (b[0][i] == b[1][i] && b[1][i] == b[2][i]) {
            if (b[0][i] == CHESS_1)
                return WIN_1;
            if (b[0][i] == CHESS_2)
                return WIN_2;
        }
    }
    // 判断对角线
    if ((b[0][0] == b[1][1] && b[1][1] == b[2][2])
    || (b[2][0] == b[1][1] && b[1][1] == b[0][2])) {
        if (b[1][1] == CHESS_1)
            return WIN_1;
        if (b[1][1] == CHESS_2)
            return WIN_2;
    }
    // 判断平局
    for (int y = 0; y < ROW; y++) {
        for (int x = 0; x < COL; x++) {
            if (b[y][x] == EMPTY)
                return GO_ON;   // 不平局
        }
    }
    return DRAW;    // 平局
}

// 判断游戏状态
// @return WIN_1, WIN_2, DRAW, GO_ON之一
GameStatus Game::judge() {
    return judge(board);
}

// 打印棋局。形如：
//  X │   │
// ───┼───┼───
//    │ O │
// ───┼───┼───
//  X │   │
void Game::printBoard() {
    cout << endl;
    for (int y = 0; y < ROW; y++) {
        for (int x = 0; x < COL; x++) {
            if (board[y][x] == EMPTY) {
                cout << "   ";
            } else if (board[y][x] == CHESS_1) {
                cout << " O ";
            } else if (board[y][x] == CHESS_2) {
                cout << " X ";
            }
            if (x < 2)
                cout << "│";
        }
        if (y < 2)
            cout << endl << "───┼───┼───" << endl;
    }
    cout << "\n\n";
}

// 若游戏结束，打印游戏结果，否则不打印。
// @return GO_ON, WIN_1, WIN_2, DRAW之一
GameStatus Game::printResult() {
    if (status == WIN_1) {
        cout << "玩家1胜利！" << endl;
    } else if (status == WIN_2) {
        cout << "玩家2胜利！" << endl;
    } else if (status == DRAW) {
        cout << "平局！" << endl;
    }
    return status;
}


// 玩家。抽象类
class Player {
public:
    virtual ~Player() {}
    virtual void play(Game* game) = 0;
};


// 人类玩家
class HumanPlayer: public Player {
public:
    // 根据获取的指令，下一步棋。
    // @param game Game对象的指针
    void play(Game* game) {
        int position; // 落子位置（数字表示）
        int x, y;     // 落子位置（坐标）
        cout << "1~9对应位置如下：" << endl
             << "[7] [8] [9]" << endl
             << "[4] [5] [6]" << endl
             << "[1] [2] [3]" << endl
             << "请输入你的棋子的位置：";
        while (1) {
            cin >> position;
            if (position < 1 || position > 9) {
                cerr << "位置不合法，请重新输入：";
                continue;
            }
            y = (9 - position) / 3;
            x = (position - 1) % 3;
            try {
                game->recieveChess(y, x);
                return;
            } catch (const char* msg) {
                cerr << msg << "，请重新输入：";
            }
        }
    }
};


// AI玩家
class AiPlayer: public Player {
private:
    PlayerNumber number;        // 玩家号（PLAYER_1/PLAYER_2）
    ChessType board[ROW][COL];  // 3*3矩阵存储的棋局
    ChessType myChess;          // 我方棋子（CHESS_1/CHESS_2）
    ChessType oppChess;         // 对手棋子（CHESS_1/CHESS_2）
    GameStatus myWin;           // 我方胜利状态（WIN_1/WIN_2）
    GameStatus oppWin;          // 对手胜利状态（WIN_1/WIN_2）

    GameStatus judge();
    int eval();
    int minMaxSearch(int &idx,int turn);

public:
    void play(Game* game);
};

// 判断在成员变量board棋局下游戏的状态。
// @return GO_ON, WIN_1, WIN_2, DRAW之一
GameStatus AiPlayer::judge() {
    return Game::judge(board);
}

// 下一步棋。
// @param game Game对象的指针
void AiPlayer::play(Game* game) {
    // 获取当前棋局
    game->getBoard(board);
    number = game->getNowPlaying();
    // 获取玩家和棋子
    if (number == PLAYER_1) {
        myChess = CHESS_1;
        oppChess = CHESS_2;
        myWin = WIN_1;
        oppWin = WIN_2;
    } else {
        myChess = CHESS_2;
        oppChess = CHESS_1;
        myWin = WIN_2;
        oppWin = WIN_1;
    }
    // 获取当前轮数
    int turn = game->getTurn();
    int x;
    minMaxSearch(x, turn);
    game->recieveChess(x / 3, x % 3);
}

// 评估局面，给出分数。
// @return
// 自身赢时，返回 空格数+1；
// 对手赢时，返回 -空格数-1；
// 平局时，返回 0
int AiPlayer::eval() {
    int empty = 0;    // 空位数量
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)   
            if (board[y][x] == EMPTY) empty++;

    GameStatus status = judge();
    if (status == myWin) return empty + 1;
    if (status == oppWin) return -empty - 1;
    return 0;
}

// 极大极小值搜索。
// @param idx 最佳下棋位置，实际上相当于返回值
// @param turn 回合数
// @return 该结点的评估值极值
int AiPlayer::minMaxSearch(int &idx, int turn) {
    // 叶子结点，返回棋面评估值
    if (judge() != GO_ON) return eval();

    // 不是叶子结点，返回当前结点的子结点的评估值极值

    // 是我的回合则isMyTurn为1，否则为0
    int isMyTurn = number == PLAYER_1 ? (turn + 1) % 2 : turn % 2;

    // 极值初值
    int val = isMyTurn ? -200 : 200; // 我的回合取极大值，对手回合取极小值

    // 记录可下棋的位置
    // 以一维形式（y * 3 + x）存储
    vector<int> choices;
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            if (board[y][x] == EMPTY)
                choices.push_back(y * 3 + x);
    
    // 遍历所有选项
    for (int i = 0, position; i < (int)choices.size(); i++) {
        position = choices[i];
        board[position / 3][position % 3] = isMyTurn ? myChess : oppChess;
        int x = position;
        int sonVal = minMaxSearch(x, turn + 1);
        board[position / 3][position % 3] = EMPTY;
        
        // 更新极值
        if (isMyTurn) {
            if (val < sonVal) {
                val = sonVal;
                // 更新最优下棋位置
                idx = choices[i];
            }
        } else {
            if (val > sonVal) {
                val = sonVal;
            }
        }
    }
    return val;
}


// 主函数
int main() {
    Player* player1;    // 玩家1
    Player* player2;    // 玩家2

    // 根据对战模式创建玩家
    cout << "选择对战模式：" << endl
         << "[1] 玩家1：真人；玩家2：真人" << endl
         << "[2] 玩家1：真人；玩家2：AI" << endl
         << "[3] 玩家1：AI；玩家2：真人" << endl;
    while (1) {
        int cmd;
        cin >> cmd;
        switch (cmd) {
            case 1:
                player1 = new HumanPlayer;
                player2 = new HumanPlayer;
                break;
            case 2:
                player1 = new HumanPlayer;
                player2 = new AiPlayer;
                break;
            case 3:
                player1 = new AiPlayer;
                player2 = new HumanPlayer;
                break;
            default:
                cerr << "未知指令，请重新输入：";
                continue;
        }
        break;
    };

    // 初始化游戏
    Game game;
    cout << "游戏开始！\n棋子：[O] 玩家1；[X] 玩家2" << endl;
    game.printBoard();

    // 回合
    while (1) {
        PlayerNumber nowPlaying = game.getNowPlaying();
        cout << "--------------------------------" << endl;
        
        // 下棋
        if (nowPlaying == PLAYER_1) {
            cout << "* 玩家1（O）的回合 *" << endl;
            player1->play(&game);
        } else {
            cout << "* 玩家2（X）的回合 *" << endl;
            player2->play(&game);
        }

        // 打印棋局
        game.printBoard();

        // 判断游戏结果
        if (game.printResult() != GO_ON)
            break;
    }

    // 销毁对象
    delete player1;
    delete player2;
    system("pause");
    return 0;
}
