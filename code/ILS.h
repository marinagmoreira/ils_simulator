#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <math.h>
#include <pthread.h>

#define PI 		3.14159265
#define ID_ATT	1
#define ID_POS	2



extern bool visual_flag_pos;
extern bool visual_flag_att;
