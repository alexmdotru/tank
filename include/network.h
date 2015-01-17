#pragma once

#include "tank.h"

TCPsocket openTCPSocket(char *host, int port);
void sendTankStruct(tank_t *tank, TCPsocket socket);
void recvTankStruct(tank_t *tank, TCPsocket socket);
