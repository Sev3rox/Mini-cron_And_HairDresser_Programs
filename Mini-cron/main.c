#include "funkcje.h"


int main(int argc, char *argv[]) {
	time_t rawtime;
	openlog("Minicron",LOG_CONS | LOG_PID | LOG_NDELAY,LOG_USER);
	if (argc >3) 
	{
	    	syslog(LOG_INFO,"Podano za duzo argumentow");
		return 1;
	}
	if (argc <3) 
	{
	    	syslog(LOG_INFO,"Podano za malo argumentow");
		return 1;
	}
	signal(SIGINT, koniec);
	signal(SIGUSR1, wczytanie);
	signal(SIGUSR2, wypisanie);
	struct tm * timeinfo;
	node_t * pocz = NULL;
	pocz = (node_t *) malloc(sizeof(node_t));
	if (pocz == NULL) 
	{
	    	syslog(LOG_INFO,"Blad alokacji pamieci");
		return 1;
	}
	pocz->next = NULL;
	node_t * akt = pocz;
        pid_t pid, sid;
        pid = fork();
        if (pid < 0) 
	{
                syslog(LOG_INFO,"Blad tworzenia podprocesu");
		exit(EXIT_FAILURE);	
        }
        if (pid > 0) 
	{
                exit(EXIT_SUCCESS);
        }
        umask(0);
        sid = setsid();
        if (sid < 0) 
	{
		syslog(LOG_INFO,"Blad tworzenia nowego SID");
                exit(EXIT_FAILURE);
        }   
	FILE *wy = fopen(argv[2],"w");
	if (wy == NULL) 
	{
	    	syslog(LOG_INFO,"Blad otwarcia pliku wyjsciowego");return 1;
	}
	read2(akt,argv[1]);
	akt=sorttime(akt);
        pid_t child_pid;
	int i=0;


	while(akt!=NULL)
	{
		int sen=spanie(akt);
		if (sen<0)sen+=24*60*60;
		sleep(sen);
		int fds[2];
		int fdss[2];
		i++;
		int j=0;
		if(bb==0)
		{
			node_t * pom;
			while(akt!=NULL)
			{
				pom=akt;
				free(akt);
				akt=pom->next;
			}
			node_t * pocz2 = NULL;
			pocz2 = (node_t *) malloc(sizeof(node_t));
			if (pocz2 == NULL) 
			{
	    			syslog(LOG_INFO,"Blad alokacji pamieci");
				return 1;
			}
			pocz2->next = NULL;
			akt = pocz2;
			read2(akt,argv[1]);
			akt=sorttime(akt);
			bb=1;
		}
		poczo=akt;
		if(pipe(fds)!=0)
		{
	    		syslog(LOG_INFO,"Blad tworzenia pipe'a");
			return 1;
		};
		if(pipe(fdss)!=0)
		{
	    		syslog(LOG_INFO,"Blad tworzenia pipe'a");
			return 1;
		};
		pid_t ch_pid;
		ch_pid=fork();
		if(ch_pid<0)
		{
	    		syslog(LOG_INFO,"Blad tworzenia podprocesu");
			return 1;
		};
		if(ch_pid==0)
		{
			potoki_t *pot=podziel(akt->komenda);
			
			
				pipeline(pot,STDIN_FILENO,wy,akt->komenda,akt->mode);
			
			
			d=0;
			return 0;
		}
		else 
		{
			;
		}
	akt=akt->next;
	}
   exit(EXIT_SUCCESS);
}

