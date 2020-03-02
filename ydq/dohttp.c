#include "dohttp.h"
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>

void *do_request(void* client)
{
    char buf[1000] = {0};
    int* socket = (int*)client;

    ssize_t recvLen = recv(*socket, buf, 10000, 0);
    if(recvLen <= 0)
        fprintf(stderr, "recv failed");
    buf[recvLen] = '\0';

    char method[16] = {0};
    char url[256] = ".";
    sscanf(buf, "%s %s *", method, url+1);

    if(strcmp(method, "GET") != 0){
        fprintf(stderr, "not provided method: %s\n", method);
        response405(*socket);
        return NULL;
    }

    FILE *fp = fopen(url, "r");
    long filesize = -1;//文件大小
    char tail[10];    //文件后缀
    if(fp == NULL){
        fprintf(stderr, "file open failed: %s\n", url);
        response404(*socket);
        return NULL;
    }
    else {
        //获取文件大小
        struct stat statbuf;
        if(stat(url, &statbuf)!=0)
            fprintf(stderr, "Get file information error\n");
        else {
            filesize = statbuf.st_size;
        }
        //获取后缀
        sscanf(url, ".%*[^.].%s", tail);
    }

    if(strcmp(tail, "m3u8") == 0){
        int sendSize = snprintf(buf, 1024,
                                "HTTP/1.1 200 OK\r\n"
                                "Server: Server\r\n"
                                "Content-Type: application/vnd.apple.mpegurl\r\n"
                                "Content-Length: %lu\r\n"
                                "Connection: keep-alive\r\n"
                                "Accept-Ranges: bytes\r\n\r\n",
                                filesize);
        write(*socket, buf, (size_t)sendSize);
        sendFile(*socket, fp);
    }else if(strcmp(tail, "ts") == 0){
        int sendSize = snprintf(buf, 1024,
                                "HTTP/1.1 200 OK\r\n"
                                "Server: Serve\r\nr"
                                "Content-Type: video/mp2t\r\n"
                                "Content-Length: %lu\r\n"
                                "Connection: keep-alive\r\n"
                                "Accept-Ranges: bytes\r\n\r\n",
                                filesize);
        write(*socket, buf, (size_t)sendSize);
        sendFile(*socket, fp);
    }else {
        fprintf(stderr, "file open failed: %s\n", url);
        response404(*socket);
    }

    return NULL;
}

int sendFile(int sock, FILE *file)
{
    size_t sendSize;
    int fd = fileno(file);
    char send_buf[1024] = {0};
    while ((sendSize = (size_t)read(fd, send_buf, 1024)) > 0) {
        write(sock, send_buf, sendSize);
    }
    return 0;
}

void response405(int sock)
{
    const char* message = "HTTP/1.1 405 Not Allowed\r\n"
                              "Server: Server\r\n"
                              "Content-Type: text/html\r\n"
                              "Content-Length: 122\r\n"
                              "Connection: keep-alive\r\n\r\n"
                              "<html>\r\n"
                              "<head><title>405 Not Allowed</title></head>\r\n"
                              "<body>\r\n"
                              "<center><h1>405 Not Allowed</h1></center>\r\n"
                              "</body>\r\n"
                              "</html>\r\n";
    write(sock, message, strlen(message));
}

void response404(int sock)
{
    const char* message = "HTTP/1.1 404 Not Found\r\n"
                              "Server: Server\r\n"
                              "Content-Type: text/html\r\n"
                              "Content-Length: 118\r\n"
                              "Connection: keep-alive\r\n\r\n"
                              "<html>\r\n"
                              "<head><title>404 Not Found</title></head>\r\n"
                              "<body>\r\n"
                              "<center><h1>404 Not Found</h1></center>\r\n"
                              "</body>\r\n"
                              "</html>\r\n";
    write(sock, message, strlen(message));
}

