#pragma once

#include "tank.h"

TCPsocket openTCPSocket(char *host, int port);
void sendTankStruct(tank_t *tank, TCPsocket socket);
void recvTankStruct(tank_t *tank, TCPsocket socket);
void sendDestrUpdate(int8_t *id, TCPsocket socket);
void recvDestrUpdate(int8_t *id, TCPsocket socket);
void sendEnemiesKilled(uint8_t *enemies, TCPsocket socket);
void recvEnemiesKilled(uint8_t *enemies, TCPsocket socket);
