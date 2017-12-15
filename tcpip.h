#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
//#include <cstdlib.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

#define PORTNUM 15000
const int CONNECT = 17, P1 = 19, P2 = 21, CONNECTED = 23, BEGIN_REQUEST = 25, 
          READY = 26, TABLE = 27, ACCEPT = 28, DECLINE = 29, WIN = 30, LOSE = 31, NEW_PORT = 33, DRAW = 34;

class Server{
    int ppid; // родительский pid
    int nport; // номер порта
    struct sockaddr_in serv_addr; // ip сервера
    // массив адресов клиентов
    struct sockaddr_in clnt_addr[2];
// для получения IP-адреса по имени хоста
    struct hostent *hp;
    int nclients = 0;
    char * field;
    int win_val = 1;
    char buf[80], hname[80];
// когда подключились два клиента
// запускаем дочерний процесс для
// обслуживания игры
// далее сервер продолжает слушать
   void new_game();
// проверка правильности хода
   int check_turn(int x);
public:
//создает порт чтобы слушать подключения
// заполняет информацию serv_addr
   Server(int port_num);
// запускает бесконечный цикл приема запросов
   void start();
// останавливает сервер
// для остановки запускается еще один процесс 
// сервера с ключом -stop
// и посылает ему сообщение terminate 
// сервер может получить сообщение terminate 
// также от клиента
   void stop();   
};

class Client
{
    struct sockaddr_in serv_addr; // ip сервера
// для получения IP-адреса по имени хоста
    struct hostent *hp;
    char buf[80];
    char player_id;    
    int nport;// номер порта
public:
// запускает сервер и подключается к хосту
// с адресом addr
   Client (std::string addr, int port_num);
// сообщает серверу о выходе из игры
// закрывает соединение
   ~Client ();
// предоставляет возможность делать ходы
// получает и обрабатывает сообщения от 
// сервера
   void play();
};
