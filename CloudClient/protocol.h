#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN =0,
    ENUM_MSG_TYPE_REGISTER_REQUEST,
    ENUM_MSG_TYPE_REGISTER_RESPOND,

    ENUM_MSG_TYPE_LOGIN_REQUEST,
    ENUM_MSG_TYPE_LOGIN_RESPOND,

    ENUM_MSG_TYPE_SHOW_ONLINE_REQUEST,
    ENUM_MSG_TYPE_SHOW_ONLINE_RESPOND,

    ENUM_MSG_TYPE_SEARCH_USER_REQUEST,
    ENUM_MSG_TYPE_SEARCH_USER_RESPOND,

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,

    ENUM_MSG_TYPE_ADD_FRIEND_RESEND_REQUEST,
    ENUM_MSG_TYPE_ADD_FRIEND_RESEND_RESPOND,

    ENUM_MSG_TYPE_FRESH_FRIENDLIST_REQUEST,
    ENUM_MSG_TYPE_FRESH_FRIENDLIST_RESPOND,

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,

    ENUM_MSG_TYPE_PRIVATE_MESSAGE_REQUEST,
    ENUM_MSG_TYPE_PRIVATE_MESSAGE_RESPOND,

    ENUM_MSG_TYPE_BROADCAST_REQUEST,
    ENUM_MSG_TYPE_BROADCAST_RESPOND,
    ENUM_MSG_TYPE_MAX = 0x00ffffff
};

typedef struct pto{
    unsigned int totalSize;//total size of the protocol
    unsigned int msgType;//type of the message
    unsigned int msgSize;//size of the message
    int code;//respond code
    char preData[64];//preData stores information used for authentication or data exchange of small size
    char data[];
}pto;


pto* makePTO(unsigned int msgLen);

#endif // PROTOCOL_H
