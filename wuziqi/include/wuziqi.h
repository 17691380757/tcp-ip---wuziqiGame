#ifndef WUZIQI_H
#define WUZIQI_H
#include<iostream>
#include <string>
#include <cstring>  // For memset
#include <sys/socket.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<sstream>
const int BOARD_SIZE = 15;

// 棋盘类:
class Board {
private:
    char board[BOARD_SIZE][BOARD_SIZE];

public:
    Board() {
        memset(board, ' ', sizeof(board));
    }

    void print() {
        std::cout << "  ";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            std::cout << i + 1 << ' ';
        }
        std::cout << std::endl;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            std::cout << i + 1 << ' ';
            for (int j = 0; j < BOARD_SIZE; ++j) {
                std::cout << board[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }

    bool isFull() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }

    bool isValidMove(int x, int y) {
        return x >= 0 && y >= 0 && x < BOARD_SIZE&& y < BOARD_SIZE&& board[x][y] == ' ';
    }

    void makeMove(int x, int y, char player) {
        board[x][y] = player;
    }

    bool checkWin(int x, int y) {
        char player = board[x][y];

        // 水平方向
        int count = 0;
        for (int i = y - 4; i <= y + 4; ++i) {
            if (i < 0 || i >= BOARD_SIZE) {
                continue;
            }
            if (board[x][i] == player) {
                count++;
            }
            else {
                count = 0;
            }
            if (count == 5) {
                return true;
            }
        }

        // 垂直方向
        count = 0;
        for (int i = x - 4; i <= x + 4; ++i) {
            if (i < 0 || i >= BOARD_SIZE) {
                continue;
            }
            if (board[i][y] == player) {
                count++;
            }
            else {
                count = 0;
            }
            if (count == 5) {
                return true;
            }
        }

        // 左上到右下斜线
        count = 0;
        for (int i = -4; i <= 4; ++i) {
            int row = x + i;
            int col = y + i;
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
                continue;
            }
            if (board[row][col] == player) {
                count++;
            }
            else {
                count = 0;
            }
            if (count == 5) {
                return true;
            }
        }

        // 右上到左下斜线
        count = 0;
        for (int i = -4; i <= 4; ++i) {
            int row = x + i;
            int col = y - i;
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
                continue;
            }
            if (board[row][col] == player) {
                count++;
            }
            else {
                count = 0;
            }
            if (count == 5) {
                return true;
            }
        }

        return false;
    }
};

// TCP 连接类
class TCPConnection {
private:
    int socket_fd;

public:
    TCPConnection(int sock) : socket_fd(sock) {}

    std::string receive() {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(socket_fd, buffer, sizeof(buffer), 0);
        return std::string(buffer);
    }

    void send(const std::string& message) {
        ::send(socket_fd, message.c_str(), message.length(), 0);
    }
};
#endif//WUZIQI_H
