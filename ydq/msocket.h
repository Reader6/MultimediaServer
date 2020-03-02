#ifndef MSOCKET_H
#define MSOCKET_H

/**
 * @brief 创建一个TCP或UDP套接字
 * @return false: -1
 */
int createTcpSocket(void);
int createUdpSocket(void);

/**
 * @brief 给一个TCP套接字 绑定ip地址和端口号
 */
int bindSocketAddr(int sockfd, const char* ip, int port);

/**
 * @brief 等待一个连接，并保存
 * @return 失败返回-1, 成功返回套接字
 */
int acceptClient(int sockfd, char* ip, int* port);


#endif // MSOCKET_H
