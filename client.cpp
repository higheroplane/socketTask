#include "tcpip.h"

#define PORTNUM 15000

int main()
{
    //printf ("enter the address:\n");
    //std::string addr;
    //std::getline(std::cin, addr);     
    Client Player ("192.168.1.100", PORTNUM);
    Player.play();
}
