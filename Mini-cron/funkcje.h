#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
typedef struct potoki{
	char *kom[100];
	struct potoki * next;
}potoki_t;
typedef struct node {
    	char komenda[1000];
	int godzina;
	int minuta;
	int mode;
    	struct node * next;
} node_t;
potoki_t *podziel(char komenda[]);
void read2(node_t *n,char* nazwa);
int spanie(node_t *n);
node_t* sorttime(node_t *n);
void wczytanie(int signum);
void wypisanie(int signum);
void koniec(int signum);
void pipeline( potoki_t *pot, int in_fd, FILE *wy,char* komend,int mod);
void error_exit(const char*);



node_t * poczo;
int d;
int bb;
static int child;
#endif
