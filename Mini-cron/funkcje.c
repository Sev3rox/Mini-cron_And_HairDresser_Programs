#include "funkcje.h"
int d=0;
int bb=1;
static int child=0;
potoki_t *podziel(char komenda[])
{
	potoki_t * pot=NULL;
	pot = (potoki_t *) malloc(sizeof(potoki_t));
	if(pot==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	potoki_t * pocz=pot;
	pot->next=NULL;
	char *pos;
	char *pos2;
	char *ch;
	char cha[100];
	char charr[100];
	strcpy(charr,komenda);
	ch=strtok_r(charr,"|",&pos);
	strcpy(cha,ch);
	char *chh;
	int i=0;
	chh=strtok_r(cha," ",&pos2);
	pot->kom[i] = (char *) malloc(20*sizeof(char));
	strcpy(pot->kom[i],chh);
	if(pot->kom[i]==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	do
	{	
		i++;
		pot->kom[i] = (char *) malloc(20*sizeof(char));
		if(pot->kom[i]==NULL)
		{
			syslog(LOG_INFO,"Blad alokacji pamieci");
			exit(1);
		}
		chh=strtok_r(NULL," ",&pos2);
		if(chh==NULL)break;
		strcpy(pot->kom[i],chh);
		pot->kom[i+1]=NULL;
	}while(chh!=NULL);
	pot->kom[i]=NULL;
	pot->next=(potoki_t *) malloc(sizeof(potoki_t));
	if(pot->next==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pot=pot->next;
	pot->next=NULL;
	do
	{	
		pot->kom[i]=NULL;
		i=0;
		d++;
		pot->kom[i] = (char *) malloc(20*sizeof(char));
		if(pot->kom[i]==NULL)
		{
			syslog(LOG_INFO,"Blad alokacji pamieci");
			exit(1);
		}
		ch=strtok_r(NULL,"|",&pos);
		if(ch==NULL)break;
		strcpy(cha,ch);
		chh=strtok_r(cha," ",&pos2);
		if(chh==NULL)break;
		strcpy(pot->kom[i],chh);
		do
		{	
			i++;
			pot->kom[i] = (char *) malloc(20*sizeof(char));	
			if(pot->kom[i]==NULL)
			{
				syslog(LOG_INFO,"Blad alokacji pamieci");
				exit(1);
			}
			chh=strtok_r(NULL," ",&pos2);
			if(chh==NULL){break;}
			strcpy(pot->kom[i],chh);
			i++;
		}while(chh!=NULL);
		pot->kom[i]=NULL;
		pot->next=(potoki_t *) malloc(sizeof(potoki_t));
		if(pot->next==NULL)
		{
			syslog(LOG_INFO,"Blad alokacji pamieci");
			exit(1);
		}
		pot=pot->next;
		pot->kom[i+1]=NULL;
	}while(ch!=NULL);
	return pocz;
}


void read2(node_t *n,char* nazwa)
{
	char ch[30]="";
	char *pos;
	int i=1;
	FILE *f = fopen(nazwa, "r");
	if(f==NULL)
	{
		syslog(LOG_INFO,"Blad otwarcia pliku wejsciowego");
		exit(1);
	}
	node_t *pom = (node_t *) malloc(sizeof(node_t));
	if(pom==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pom->next=NULL;
	node_t *pom1 = (node_t *) malloc(sizeof(node_t));
	if(pom1==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pom1->next=NULL;
	node_t *pom2 = (node_t *) malloc(sizeof(node_t));
	if(pom2==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pom2->next=NULL;
	pom=n;
	int jkl=0;
	while ( fgets(ch,30, f)!=NULL && ch[0]!='\n' && ch[0]!=' ')
        {    
		if(jkl==0){}
		else
		{
			n->next = (node_t *) malloc(sizeof(node_t));
			if(n->next==NULL)
			{
				syslog(LOG_INFO,"Blad alokacji pamieci");
				exit(1);
			}
			n=n->next;
    			n->next = NULL;  
		} 
		n->godzina=atoi(strtok_r(ch,":",&pos));
		n->minuta=atoi(strtok_r(NULL,":",&pos));
		char* ca=strtok_r(NULL,":",&pos);
		strcpy(n->komenda,ca);
		n->mode=atoi(strtok_r(NULL,"",&pos));
		pom1=pom;
		int ip=0;
		while(pom1!=NULL&&ip==0)
		{
			if(((n->godzina<pom1->godzina)||(n->godzina==pom1->godzina&&n->minuta<pom1->minuta))&&jkl!=0)
			{
				pom2->godzina=pom1->godzina;
				pom2->minuta=pom1->minuta;
					strcpy(pom2->komenda,pom1->komenda);
				pom2->mode=pom1->mode;
				pom1->godzina=n->godzina;
				pom1->minuta=n->minuta;
				strcpy(pom1->komenda,n->komenda);
				pom1->mode=n->mode;
				n->godzina=pom2->godzina;
				n->minuta=pom2->minuta;
				strcpy(n->komenda,pom2->komenda);
				n->mode=pom2->mode;
				ip=1;
			}
		pom1=pom1->next;
		}
	jkl=1;
	}
	n=pom;
	if(fclose(f)!=0)
	{
		syslog(LOG_INFO,"Blad zamykania pliku wejsciowego");
		exit(1);
	};
}

int spanie(node_t *n)
{
	struct tm *tm_struct;
 	time_t rawtime;
  	time ( &rawtime );
 	tm_struct=localtime(&rawtime );
	int hour = tm_struct->tm_hour;
	int min=tm_struct->tm_min;
	int a,b;
	a=n->godzina-hour;
	b=n->minuta-min;
	int sum;
	sum=60*60*a+60*b;
	return sum;
}









node_t* sorttime(node_t *n)
{
	node_t *pomm= (node_t *) malloc(sizeof(node_t));
	if(pomm==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pomm->next=NULL;
	node_t *pomm1= (node_t *) malloc(sizeof(node_t));
	if(pomm1==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pomm1->next=NULL;
	node_t *pomm2= (node_t *) malloc(sizeof(node_t));
	if(pomm2==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pomm2->next=NULL;
	pomm=n;
	pomm1=n;
	if(spanie(pomm)<0)
	{
		while(spanie(pomm->next)<0)
		{
			pomm=pomm->next;
		}
		while(pomm1->next!=NULL)
		{
			pomm1=pomm1->next;
		}
		pomm2=pomm->next;
		pomm->next=NULL;
		pomm1->next=n;
		n=pomm2;
	}
	return n;
}



void wczytanie(int signum){
 	 bb=0;
}



void wypisanie(int signum){
	node_t * pocza=(node_t *) malloc(sizeof(node_t));
	if(pocza==NULL)
	{
		syslog(LOG_INFO,"Blad alokacji pamieci");
		exit(1);
	}
	pocza=poczo;
	do
	{
		syslog(LOG_INFO,"%s\n",pocza->komenda);
		pocza=pocza->next;
	}while(pocza!=NULL);
}



void koniec(int signum){
	exit(0);
}



void error_exit(const char *kom){
    perror(kom);
    (child ? _exit : exit)(EXIT_FAILURE);
}


void pipeline(potoki_t *pot, int in_fd,FILE *wy,char * komend,int mod){
    	if(d<=1)
	{ 
        	if(in_fd != STDIN_FILENO)
		{
            		if(dup2(in_fd, STDIN_FILENO) != -1)
                	close(in_fd); 
            		else error_exit("dup2");
        	}
		int fds[2];
		int fdss[2];
		if(pipe(fds)<0)
		{
			syslog(LOG_INFO,"Blad tworzenia pipe'a");
			exit(1);
		};
		if(pipe(fdss)<0)
		{
			syslog(LOG_INFO,"Blad tworzenia pipe'a");
			exit(1);
		};
		pid_t h;
		h=fork();
		if(h<0)
		{
	  	  	syslog(LOG_INFO,"Blad tworzenia podprocesu");
			exit(1);
		};
		if(h==0)
		{	
			if(close(fds[0])<0)
					{
	    					syslog(LOG_INFO,"Blad zamykania strunmienia");
						exit(1);
					};
					if(close(fdss[0])<0)
					{
	    					syslog(LOG_INFO,"Blad zamykania strunmienia");
						exit(1);
					};
					if(mod==0)
						{if(dup2(fdss[1], 2)<0)
						{
	    					syslog(LOG_INFO,"Blad duplikowania strumienia");
						exit(1);
						};
						if(dup2(fds[1], 1)<0)
						{
	    						syslog(LOG_INFO,"Blad duplikowania strumienia");
							exit(1);
						};
						}

					if(mod==1)
						{if(dup2(fds[1], 2)<0)
						{
	    					syslog(LOG_INFO,"Blad duplikowania strumienia");
						exit(1);
						};
						if(dup2(fdss[1], 1)<0)
						{
	    						syslog(LOG_INFO,"Blad duplikowania strumienia");
							exit(1);
						};
						}

					if(mod==2)
						{if(dup2(fds[1], 2)<0)
						{
	    					syslog(LOG_INFO,"Blad duplikowania strumienia");
						exit(1);
						};
						if(dup2(fds[1], 1)<0)
						{
	    						syslog(LOG_INFO,"Blad duplikowania strumienia");
							exit(1);
						};
						}


					if(close(fds[1])<0)
					{
	    					syslog(LOG_INFO,"Blad zamykania strunmienia");
						exit(1);
					};
					if(close(fdss[1])<0)
					{
	    					syslog(LOG_INFO,"Blad zamykania strunmienia");
						exit(1);
					};
			if(execvp(pot->kom[0],pot->kom)==-1)
					{
					syslog(LOG_INFO,"Blad wykonywania programu");
						exit(1);
					}
		}
		else
		{	
			if(close(fds[1])<0)
			{
	    			syslog(LOG_INFO,"Blad zamykania strunmienia");
				exit(1);
			};
			char ss[1024]="";
			if(read(fds[0],ss,sizeof(char) * 1024)<0)
			{
	    			syslog(LOG_INFO,"Blad przekazania danych");
				exit(1);
			};
			if(fprintf(wy,"Polecenie:%s wygenerowalo:",komend)<0)
			{
	    			syslog(LOG_INFO,"Blad wypisywania danych");
				exit(1);
			};
			if(fprintf(wy,"%s\n",ss)<0)
			{
	    			syslog(LOG_INFO,"Blad wypisywania danych");
				exit(1);
			};
			syslog(LOG_INFO,"Zadanie:%s wygenerowalo kod wyjscia:",komend);
			syslog(LOG_INFO,"%s",ss);
    		}
	}
    	else
	{
        	int fd[2];
        	pid_t childpid;
        	if ((pipe(fd) == -1) || ((childpid = fork()) == -1)) 
		{
         	   	syslog(LOG_INFO,"Blad tworzenia pipe'a");
			error_exit("Failed to setup pipeline");
        	}
        	if (childpid == 0)
		{ 
           		child = 1;
           		if(close(fd[0])<0)
			{
	    			syslog(LOG_INFO,"Blad zamykania strumienia");
				exit(1);
			};
            		if (dup2(in_fd, STDIN_FILENO) == -1) 
                	{
				syslog(LOG_INFO,"Blad duplikowania strumienia");
				exit(1);
			}
            		if (dup2(fd[1], STDOUT_FILENO) == -1)   
                	{
				syslog(LOG_INFO,"Blad duplikowania strumienia");
				exit(1);
			}
            	else 
			if ((close(fd[1]) == -1))
                	{
				syslog(LOG_INFO,"Blad zamykania strumienia");exit(1);
			}
            		else 
			{
                		execvp(pot->kom[0],pot->kom);
                		error_exit("Failed to execlp");
            		}
        	}
        	if(close(fd[1])<0)
		{
	    		syslog(LOG_INFO,"Blad zamykania strumienia");
			exit(1);
		};   
        	if(close(in_fd)<0)
		{
	    		syslog(LOG_INFO,"Blad zamykania strumienia");
			exit(1);
		};
		d--;
    		pipeline(pot->next, fd[0],wy,komend,mod);
	}
}


