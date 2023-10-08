#include <iostream>
#include<unistd.h>
#include<sstream>
#include <string>
#include <cstring>  // For memset
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../include/wuziqi.h"



int main() {
    // 创建 socket
    int listen_socket = socket(AF_INET6, SOCK_STREAM, 0);

    // 绑定地址和端口
    sockaddr_in6 server_addr{};
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(8888);  // 设置端口号
    server_addr.sin6_addr = in6addr_any;  // 使用任意可用网络地址

    bind(listen_socket, (sockaddr*)&server_addr, sizeof(server_addr));

    // 监听连接请求
    listen(listen_socket, 1);

    std::cout << "等待玩家加入..." << std::endl;

    // 接受客户端连接
    sockaddr_in6 client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(listen_socket, (sockaddr*)&client_addr, &client_addr_len);

    std::cout << "玩家已加入，可以开始游戏！" << std::endl;

    // 创建棋盘对象
    Board board;

    // 创建 TCP 连接对象
    TCPConnection connection(client_socket);
	do {
        int choice = 0;
        std::cout << "请输入你的选择：1.开始/继续游戏   2.退出" << std::endl;
        std::cin >> choice;

        if (choice == 1) {
          board = Board();
            while (true) {
        // 显示棋盘
        board.print();

        // 等待玩家落子
        std::cout << "请玩家输入落子坐标（行 列）：";
        int x, y;
        std::cin >> x >> y;

        // 检查落子是否有效
        if (!board.isValidMove(x - 1, y - 1)) {
            std::cout << "无效的落子位置，请重新输入！" << std::endl;
            continue;
        }

        // 更新棋盘状态
        board.makeMove(x - 1, y - 1, 'X');
        board.print();

        // 发送落子信息给对手
        std::string move = std::to_string(x) + " " + std::to_string(y);
        connection.send(move);

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
        std::istringstream iss(opponent_move);
        iss >> x >> y;

        // 更新棋盘状态
        board.makeMove(x - 1, y - 1, 'O');
        board.print();

        // 检查胜负
        if (board.checkWin(x - 1, y - 1)) {
            std::cout << "对手获胜！游戏结束。" << std::endl;
            break;
        }
}
        } else if (choice == 2) {
            close(listen_socket);
            close(client_socket);
            return 0;
        } else {
            std::cout << "你的选择有误，请重新输入！" << std::endl;
        }
    } while (true);

    return 0;
}

