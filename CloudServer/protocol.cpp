#include "protocol.h"

pto* makePTO(unsigned int msgLen){
    pto* newPto = (pto*)malloc(sizeof (pto) + sizeof (char)*msgLen);

    if(newPto){
        memset(newPto, 0, sizeof (pto) + sizeof (char)*msgLen);
        newPto->totalSize = sizeof (pto) + sizeof (char)*msgLen;
        newPto->msgSize = sizeof (char)*msgLen;
        return newPto;
    }else{
        return nullptr;
    }
}
