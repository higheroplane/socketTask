#include "tcpip.h"
const int _M = 4, _N = 5, _W = 5;
Server::Server (int port_num)
{
    nport = port_num;
    nport = htons ((u_short)nport);
    bzero (&serv_addr, (size_t)sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = nport;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    field = (char*) calloc (_N *_M, sizeof (char));
    memset (field, 0, _N *_M);

}

void Server::start()
{
    int s = 0, ns1 = 0, ns2 = 0;
    if ((s = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {perror ("ERROR in socket() call"); exit (1);}

    while (bind (s, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {sleep (1);}
       //{perror ("ERROR in bind() call"); exit (1);}
    
    fprintf (stderr, "Server ready: %s\n", inet_ntoa (serv_addr.sin_addr));

    if (listen (s, 5) == -1)
        {perror ("ERROR in listen() call"); exit (1);}
    int counter = 0;
while (1)
{
    socklen_t addrlen; int pid = 0;
        bzero (&clnt_addr, sizeof (clnt_addr));
        addrlen = sizeof (clnt_addr);
        
    if ((ns1 = accept (s, (sockaddr*)&clnt_addr, &addrlen)) == -1)
            {perror ("ERROR in accept() call"); exit (1);} 
    if ((ns2 = accept (s, (sockaddr*)&clnt_addr, &addrlen)) == -1)
            {perror ("ERROR in accept() call"); exit (1);} 

int nbytes = 0, fout = 0, ifconnect = 0;
char * init = (char *) calloc (_M *_N + 1, sizeof (char));
    if((nbytes = recv(ns1, buf, 1, 0)) != 0)
    {
        if (buf [0] == CONNECT)
        {
            nclients ++;
            
            sprintf (init, "%c%c%c%c", P1, _M, _N, _W);
            send(ns1, init, 4, 0);

        } 
    }
//printf ("connect");
    ifconnect = 0;
    if((nbytes = recv(ns2, buf, 1, 0)) != 0 )
    {
        if (buf [0] == CONNECT)
        {
           // nclients ++;
            sprintf (init, "%c%c%c%c", P2, _M, _N, _W);
            send(ns2, init, 4, 0);

        } 
    }

        sockaddr_in forked_server_addr = serv_addr;
        int forkedPort = PORTNUM + 1; 
	    bool createdForkedServer = false;
	    int forkedSocket = socket (AF_INET, SOCK_STREAM, 0);
	    if (forkedSocket == -1)
	    {
                perror ("Failed to create socket for fork");
		exit (1);
	    }
        //bool createdForkedServer = 0;
        while (!createdForkedServer)
            {
               forked_server_addr.sin_port = htons (static_cast<u_short> (forkedPort));
               if (bind (forkedSocket, 
                         (struct sockaddr*)&forked_server_addr, 
			 sizeof (forked_server_addr)) != -1) break;
	       forkedPort ++;
	    }

    init[0] = NEW_PORT;
    *(reinterpret_cast<short*> (init + 1)) = static_cast <short> (forkedPort);
    send(ns1, init, 3, 0);
    send(ns2, init, 3, 0);
    
    close (ns1);
    close (ns2);

    if((pid = fork()) == -1) {perror(""); exit (1);}
    if (pid) continue;
if (!pid)
{
    if (listen (forkedSocket, 5) == -1)
        {perror ("ERROR in listen() call"); exit (1);}
    

    
        bzero (&clnt_addr, sizeof (clnt_addr));
        addrlen = sizeof (clnt_addr);
        
    if ((ns1 = accept (forkedSocket, (sockaddr*)&clnt_addr, &addrlen)) == -1)
            {perror ("ERROR in accept() call"); exit (1);} 
    if ((ns2 = accept (forkedSocket, (sockaddr*)&clnt_addr, &addrlen)) == -1)
            {perror ("ERROR in accept() call"); exit (1);} 

    
    

//printf ("connect connect");
    ifconnect = 0;
    if((nbytes = recv(ns1, buf, 2, 0)) != 0 )
    {
        if (buf [0] == CONNECTED) ifconnect ++;
 
        else 
        {
            //printf ("Problems with client1"); 
            //send(ns1, "1Please restart client, an error occured.\n", strlen("1Please restart client, an error occured.\n") + 1, 0);
            exit (1);
        }
    }

    ifconnect = 0;
    if((nbytes = recv(ns2, buf, 2, 0)) != 0)
    {
        if (buf [0] == CONNECTED) ifconnect ++;
         
        else 
        {
            //printf ("Problems with client2"); 
            //send(ns2, "1Please restart client, an error occured.\n", strlen("1Please restart client, an error occured.\n") + 1, 0);exit (1);
        }
    }

    if (buf[1] == P1) std::swap(ns1, ns2);

    sprintf (init, "%c", BEGIN_REQUEST);
    send(ns1, init, 1, 0);
    send(ns2, init, 1, 0);
    
    if((nbytes = recv(ns1, buf, 1, 0)) != 0 && buf[0] == READY) printf ("c1 is ready\n");
    if((nbytes = recv(ns2, buf, 1, 0)) != 0) printf ("c2 is ready\n");

    int game = 1;
    while (game == 1)
    {
        memset (init, TABLE, 1);
        memcpy (init + 1, field, _M*_N);
        //printf
        for (int i = 0; i < _M; i ++)
        {
            for (int j = 0; j < _N; j ++)
            {
                if (init [j + _N*i + 1] == 0) printf (" %c |", ' ');
                if (init [j + _N*i  + 1] == P1 ) printf (" %c |", 'x');
                if (init [j + _N*i + 1] == P2 ) printf (" %c |", 'o');
            }
            printf ("\n");
        }
       // while (recv(ns1, buf, sizeof (buf), MSG_DONTWAIT) != -1);
        send(ns1, init, 1 + _M*_N, 0);
        for (volatile int check = 0 ; check == 0 && game == 1;)  
        {
            if((nbytes = recv(ns1, buf, 2, 0)) > 0) printf ("c1 is ready %d %d %d\n", buf[0], buf[1], field[buf[1]]);
            if (check_turn ((int)buf[1])) {init [0] = (char) WIN; game = 0;}
            else if (buf[0] == P1 && field [buf[1]] == 0) {field [buf[1]] = (char)P1; init [0] = (char) ACCEPT; check = 1;}
            else init [0] = (char) DECLINE;
            send(ns1, init, 1, 0);
        } 
        if (game == 0) {init [0] = (char) LOSE; send(ns2, init, 1, 0);}
        else
        {
            memset (init, TABLE, 1);
            memcpy (init + 1, field, sizeof (char)*_M*_N);
            send(ns2, init, 1 + _M*_N, 0);
        }
        for (volatile int check = 0 ; check == 0 && game == 1;)  
        {
            if((nbytes = recv(ns2, buf, 2, 0)) != 0) printf ("c2 is ready %d %d% d\n", buf[0], buf[1], field[buf[1]]);
            if (check_turn ((int)buf[1])) {init [0] = (char) WIN; game = 0;}
            else if (buf[0] == P2 && field [buf[1]] == 0) {field [buf[1]] = (char)P2; init [0] = (char) ACCEPT; check = 1;}
            else init [0] = (char) DECLINE;
            send(ns2, init, 1, 0);
            if (game == 0) {init [0] = (char) LOSE; send(ns1, init, 1, 0);}
        }
//printf ("2 accepted\n");
        
             
    }

    close (forkedSocket);
    close (ns1);
    close (ns2);
}
}
close (s);
}

void Server::stop()
{

}
int Server::check_turn (int x)
{
    if ((x*17)%(_N*_M) == win_val) return 1;
    else return 0;
}

void Server:: new_game ()
{

}

Client::Client (std::string addr, int port_num)
{
    nport = port_num;
    nport = htons ((ushort) nport);
    char message [1000];
  
    if((hp = gethostbyname(addr.c_str())) == 0)
    {
        perror("Error gethostbyname()"); exit(3);
    }
    
    bzero(&serv_addr, (size_t)sizeof(serv_addr));
    bcopy(hp->h_addr, &(serv_addr.sin_addr), hp->h_length);
    serv_addr.sin_family = hp->h_addrtype;
    serv_addr.sin_port = nport;
}

Client::~Client ()
{
    printf ("client desrtucted succesfully");
}

void Client::play ()
{
    int s = 0;
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror(" Error socket()"); exit(1);
    }

    fprintf(stderr, "Server's host address: %s\n", inet_ntoa(serv_addr.sin_addr));
 
    if(connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Error connect()"); exit(1);
    }
    buf[0] = (char) CONNECT;
    send(s, buf, 1, 0);
    if (recv(s, buf, 4, 0) < 0)
        {
            perror("Error recv()"); exit(1);
        }
        printf("Received from server: %s\n", buf);
    int p1 = buf[0], m = buf[1], n = buf[2], w = buf[3];
    if (recv(s, buf, 3, 0) < 0)
        {
            perror("Error recv()"); exit(1);
        }
        printf("Received from server: %s\n", buf);

    serv_addr.sin_port = htons(*((ushort*) (buf+1)));
    buf [0] = (char) CONNECTED;
    buf [1] = (char) p1;
    close (s);
    sleep (1);

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror(" Error socket()"); exit(1);
    }

    fprintf(stderr, "Server's host address: %s\n", inet_ntoa(serv_addr.sin_addr));
 
    if(connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Error connect()"); exit(1);
    }
    
    send(s, buf, 2, 0);  

    if (recv(s, buf, 1, 0) < 0)
        {
            perror("Error recv()"); exit(1);
        }
    if (buf [0] == BEGIN_REQUEST) printf ("Let`s start!\n");
    sprintf(buf, "%c", READY);
    send(s, buf, 1, 0);

int game = 1;
//buf = realloc
while (game == 1)
{
    memset (buf, 0, m * n + 1);
    if (recv(s, buf, (m*n + 1)*sizeof(char), 0) < 0)
    {
        perror("Error recv()"); exit(1);
    }
    //printf ("%d %c\n", buf[10], buf[10]);

    if (buf[0] == TABLE)
    {
        for (int i = 0; i < m; i ++)
        {
            for (int j = 0; j < n; j ++)
            {
                //printf ("%2d ", buf [1 + j + n*i]); 
                if (buf [1 + j + n*i] == 0) printf (" %2c |", ' ');
                if (buf [1 + j + n*i] == P1 ) printf (" %2c |", 'x');
                if (buf [1 + j + n*i] == P2 ) printf (" %2c |", 'o');
            }
            printf ("\n");
        }
    }

    int check = 0;
    if (buf [0] == LOSE) {check ++; game = 0; printf ("You lost;(");}
    while (check == 0 && game == 1)
    {
        printf ("ur turn: ");
        int turn = 0;
        scanf ("%d", &turn);
        buf [0] = (char) p1;
        buf [1] = (char) turn;
        send(s, buf, 2, 0);

        if (recv(s, buf, 1, 0) < 0)
        {
            perror("Error recv()"); exit(1);
        }

        if (buf [0] == ACCEPT) check ++;
        if (buf [0] == WIN) {check ++; game = 0; printf ("Congratulations! You`re the winner!");}
        
    }
    check = 0;
}

    close(s);
    printf("Thanks for participating!\n\n");
}














