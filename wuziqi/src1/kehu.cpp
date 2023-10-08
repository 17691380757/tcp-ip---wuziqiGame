#include<iostream>
#include <string>
#include <cstring>  // For memset
#include <sys/socket.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<sstream>
#include "../include/wuziqi.h"


int main() {
    // 创建 socket
    int client_socket = socket(AF_INET6, SOCK_STREAM, 0);

    // 设置服务器地址和端口
    sockaddr_in6 server_addr{};
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(8888);
    if (inet_pton(AF_INET6, "::1", &(server_addr.sin6_addr)) <= 0) {
        std::cerr << "Invalid address/Address not supported!" << std::endl;
        return -1;
    }

    // 连接到服务器
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server!" << std::endl;
        return -1;
    }

    std::cout << "已连接到服务器，可以开始游戏！" << std::endl;

    // 创建棋盘对象
    Board board;

    // 创建 TCP 连接对象
    TCPConnection connection(client_socket);

		
    // 游戏逻辑
    		bool gameOver = false;
   	 		while (!gameOver) {
    // 等待服务器发送数据
    int choice = 0;
    std::cout << "请输入你的选择：1.开始/继续游戏  2.退出" << std::endl;
    std::cin >> choice;
    std::cin.ignore();  // 清除输入缓冲区中的换行符

    if (choice == 1) {
        std::string receiveData = connection.receive();
        if (receiveData.empty()) {
            std::cerr << "Failed to receive data from server!" << std::endl;
            break;
        }
        board = Board();
        while (true) {
            // 显示棋盘
            board.print();

            // 处理服务器发送的数据等
            // 处理对手落子信息
            int x, y;
            std::istringstream iss(receiveData);
            if (!(iss >> x >> y)) {
                std::cerr << "Invalid move received from the opponent!" << std::endl;
                break;
            }
            std::cout << "对手落子：" << x << "," << y << std::endl;

            // 更新棋盘状态
            board.makeMove(x - 1, y - 1, 'X');
            board.print();

            // 等待玩家落子
            std::cout << "请玩家输入落子坐标（行 列）：";
            std::cin >> x >> y;

            // 检查落子是否有效
            if (!board.isValidMove(x - 1, y - 1)) {
                std::cout << "无效的落子位置，请重新输入！" << std::endl;
                continue;
            }

            // 更新棋盘状态
            board.makeMove(x - 1, y - 1, 'O');
            board.print();

            // 发送落子信息给对手
            std::string move = std::to_string(x) + " " + std::to_string(y);

            
                try {
                    connection.send(move);
                    std::cout << "已发送落子信息：" << move << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Failed to send move information to the server: " << e.what() << std::endl;
                    break;
                }
            
             
            // 检查胜负
            if (board.checkWin(x - 1, y - 1)) {
                std::cout << "恭喜你获胜了！游戏结束。" << std::endl;
                break;
            }

            // 判断平局
            if (board.isFull()) {
                std::cout << "平局！游戏结束。" << std::endl;
                break;
            }

            // 等待对手落子
            std::cout << "等待对手落子..." << std::endl;
            std::string opponent_move = connection.receive();
            std::istringstream issOpponent(opponent_move);
            
            if (!(issOpponent >> x >> y)) {
                std::cerr << "Invalid move received from the opponent!" << std::endl;
                break;
            }

            // 更新棋盘状态
            board.makeMove(x - 1, y - 1, 'X');
            board.print();

            // 检查胜负
            if (board.checkWin(x - 1, y - 1)) {
                std::cout << "对手获胜！游戏结束。" << std::endl;
                break;
            }
        }
    } else if (choice == 2) {
        // 关闭连接
        close(client_socket);
        return 0;
    } else {
        std::cout << "你的选择有误" << std::endl;
    }
}
}
