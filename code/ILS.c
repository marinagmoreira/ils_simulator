/* 
    g++ -o ILS ILS.c
    ./ILS

The server waits for a c
onnection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>



bool visual_flag=false;


//*********** ILS *****************
// 
//
void ILS(int* pos_p, int* att_p, int* pos_r, int* att_r){

float vertical_sep	=0;
float horizontal_sep=0;

	// Calculations ILS obtaing angles






	// Deviations ILS make responsa as in ILS





	// Set Flag for Visual

	visual_flag=true;



	return;
}






void error( char* msg ) {
  perror(  msg );
  exit(1);
}


//*********** getData *****************
// 
//
void getData( int sockfd, char* data ) {
  int n;

  if ( (n = read(sockfd,data,31) ) < 0 )
    error( const_cast<char *>( "ERROR reading from socket") );
  data[n] = '\0';
  return ;
}

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno = 51717, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	char data[32];
	int pos_p[3], att_p[3];
	int pos_r[3], att_r[3];

	//Replace this by something else!!!

	pos_r[0]=0;
	pos_r[1]=0;
	pos_r[2]=0;

	att_r[0]=0;
	att_r[1]=0;
	att_r[2]=0;

	printf( "using port #%d\n", portno );
   
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error( const_cast<char *>("ERROR opening socket") );
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( portno );
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0)
	error( const_cast<char *>( "ERROR on binding" ) );
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
 
	//--- infinite wait on a connection ---
	while ( 1 ) {
		printf( "waiting for new client...\n" );
		if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
			error( const_cast<char *>("ERROR on accept") );
		printf( "opened new communication with client\n" );
		
		while ( 1 ) 
		{
			//---- wait for data from client ---
			memset(data, 0, sizeof(data));
			getData( newsockfd , data);
			sscanf(data, "%d %d %d %d %d %d", &pos_p[0], &pos_p[1], &pos_p[2], &att_p[0], &att_p[1], &att_p[2]);

			printf( "got %s\n", data );

			if ( atoi(data) < 0 )
				break;
              
			ILS(pos_p, att_p, pos_r, att_r);

		}

		close( newsockfd );

		//--- if -2 sent by client, we can quit ---
		if ( atoi(data) == -2 )
			break;
	}
	return 0;
}
