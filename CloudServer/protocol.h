#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pto{
    unsigned int totalSize;//total size of the protocol
    unsigned int msgType;//type of the message
    unsigned int msgSize;//size of the message
    char preData[64];//preData stores information used for authentication or data exchange of small size
    char data[];
}pto;


pto* makePTO(unsigned int msgLen){
    pto* newPto = (pto*)malloc(sizeof (pto) + sizeof (char)*msgLen);

    if(newPto){
        memset(newPto, 0, sizeof (pto));
        return newPto;
    }else{
        return nullptr;
    }
}

#endif // PROTOCOL_H
