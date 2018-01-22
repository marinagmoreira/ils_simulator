/* 
    g++ -o ILS ILS.c
    ./ILS

The server waits for a c
onnection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
*/

#include "ILS.h"


// GLOBAL VARIALES -------

bool visual_flag_pos=false;
bool visual_flag_att=false;

//*********** ILS_data_pos *****************
// 
void ILS_data_pos(int* pos_p, int* pos_r){

	float vertical_sep	=0;
	float horizontal_sep=0;

	float a_glide_slope=0;


	// Calculations ILS obtaing angles------


	// Glide Slope
	a_glide_slope = atan2(pos_r[2]-pos_a[2],sqrt(pow(pos_r[0]-pos_a[0])+pow(pos_r[1]-pos_a[1])));
	a_deviation = atan2(pos_r[0]-pos_a[0],pos_r[1]-pos_a[1]);

	// Deviations ILS make responsa as in ILS



	// Set Flag for Visual

	visual_flag=true;

	return;
}

//*********** ILS_data_att *****************
// 
void ILS_data_att(att_p, att_r){

	visual_flag_att=true;
	return
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
	int id=0;

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

			// scan data from attitude
			if (data[0]==ID_ATT)
			{
				sscanf(data, "%d %d %d %d %d %d", &att_p[0], &att_p[1], &att_p[2]);
				ILS_data_att(att_p, att_r);
			}
			// scan data from position
			else if (data[0]==ID_POS)
			{
				sscanf(data, "%d %d %d %d %d %d", &pos_p[0], &pos_p[1], &pos_p[2]);
				ILS_data_pos(pos_p, pos_r);
			}


			printf( "got %s\n", data );

			if ( atoi(data) < 0 )
				break;
              
			

		}

		close( newsockfd );

		//--- if -2 sent by client, we can quit ---
		if ( atoi(data) == -2 )
			break;
	}
	return 0;
}