#include <stdio.h>

#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "msocket.h"
#include "dohttp.h"
#include "tpool.h"

int main()
{
    int serverSockfd = createTcpSocket();
    if(serverSockfd < 0) {
        fprintf(stderr, "failed to create tcp socket");
        return -1;
    }
    if(!bindSocketAddr(serverSockfd, "0.0.0.0", 80)) {
        fprintf(stderr, "failed to bind addr");
        return -1;
    }

    if(listen(serverSockfd, 10) < 0) {
        fprintf(stderr, "failed to listen");
        return -1;
    }

    if(tpool_create(10) != 0){
        fprintf(stderr, "failed to tpool_create");
        return -1;
    }

    printf("wait.....\n");

    while(1) {
        char clientIp[40];
        int clientPort;
        int clientSockfd = acceptClient(serverSockfd, clientIp, &clientPort);
        if(clientSockfd < 0) {
            fprintf(stderr, "failed to accept client");
            return -1;
        }

        //do_request(&clientSockfd);
        tpool_add_work(do_request, &clientSockfd);

    }
}
