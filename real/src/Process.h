#ifndef PROCESS_H
#define PROCESS_H

#define DISCONNECTED 1
#define DISCONNECTED2 2

int ProcessPacket(unsigned char *buf, int len, bool &move);

#endif