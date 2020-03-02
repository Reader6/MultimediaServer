#ifndef DOHTTP_H
#define DOHTTP_H

#include <stdio.h>

void *do_request(void* client);


int sendFile(int sock, FILE *file);
void response405(int sock);
void response404(int sock);
#endif // DOHTTP_H
