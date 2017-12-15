#include "tcpip.h"

#define PORTNUM 15100

int main()
{
    Server GameMaster (PORTNUM);
    GameMaster.start();
    return 0;
}

