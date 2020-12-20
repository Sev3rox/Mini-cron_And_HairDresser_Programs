#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

int freechairs=7;  				//liczba wolnych krzesel
int chairs=7;   				//liczba wszystkich krzesel
int not_in=0;   				//liczba klientow ktorzy nie weszli
int in=-1;					//aktualnie golony klient
int ii=0;					//liczba klientow ogolonych
int clients =9;					//liczba wszystkich klientow
int next=-1;					//nastepny klient
int wret;					//zmienna pomocnicza wykorzystywana przy wypisywaniu bledow
bool debug=false;      
pthread_mutex_t drzwi; 	//mutex zapewniajacy wylacznosc przy wchodzeniu i wychodzeniu z poczekalni
pthread_cond_t cond_client;			//semafor sluzacy do wpuszczania klienta
pthread_cond_t cond_barber;			//semafor sluzacy do budzenia fryzjera
pthread_cond_t waitt;				//semafor sluzacy do informowania klienta przez poprzedniego
struct List *dropped = NULL; 			//lista klientow, ktorzy nie dostali sie do poczekalni
struct List *waiting = NULL;			//lista klientow w poczekalni

struct List
{
	int client_nr;				//numer klienta
	struct List *next;			//nastepny element
};

void wait()					//funkcja symulujaca prace/czekanie
{
	int x= rand()%20000+25000;
	int i;
	int j;
	for(i=0;i<10000;i++)
	for(j=0;j<x;j++)
	{;}
}

void write_waiting()				//funkcja wypisujaca liste klientow w poczekalni
{
	struct List *pom = waiting;
    	printf("Poczekalnia: ");
    	while(pom!=NULL)
    	{
        	printf("%d ",pom->client_nr);
        	pom = pom->next;
    	}
    	printf("\n");
}

void write_dropped()				//funkcja wypisujaca liste klientow, ktorzy nie dostali sie do poczekalni
{
    	struct List *pom = dropped;
	printf("Nie weszli: ");
    	while(pom!=NULL)
    	{
        	printf("%d ",pom->client_nr);
        	pom = pom->next;
    	}
    	printf("\n");
}

void add_dropped(int n)				//funkcja dodajaca klienta na poczatek listy klientow, ktorzy nie dostali sie do poczekalni
{
    	struct List *pom = (struct List*)malloc(sizeof(struct List));
	if (pom == NULL) 
	{
	    	printf("Blad alokacji pamieci");
		exit(EXIT_FAILURE);
	}
    	pom->client_nr = n;
    	pom->next = dropped;
    	dropped = pom;
    	if(debug==true)
    		write_dropped();
}
void add_waiting(int n)				//funkcja dodajaca klienta na poczatek listy klientow poczekalni
{
    	struct List *pom = (struct List*)malloc(sizeof(struct List));
	if (pom == NULL) 
	{
	    	printf("Blad alokacji pamieci");
		exit(EXIT_FAILURE);
	}
    	pom->client_nr = n;
    	pom->next = waiting;
    	waiting = pom;
	if(debug==true)
    		write_waiting();
}

void del_client()				//funkcja usuwajaca ostatniego klienta z listy klientow w poczekalni
{
    	struct List *pom = waiting;
    	struct List *pom2 = waiting;
    	while (pom->next!=NULL)			//petla ustawiajaca wskaznik pom na ostatni element listy
    	{
		next = pom2->client_nr;
        	pom2=pom;
        	pom=pom->next;
    	}
     	if(pom->client_nr==waiting->client_nr)	//jezeli jest to jedyny element, ustawienie pierwszego elementu listy jako NULL
     	{
          	waiting=waiting->next;
		next = -1;
     	}
      	else					//jezeli nie ustawienie elementu nastepnego po przedostatnim, czyli nowym ostatnim jako NULL
      	{
        	pom2->next=pom->next;
		
	}
	in=pom->client_nr;
	free(pom);				//zwolnienie ostatniego elementu listy
	if(debug==true)
    		write_waiting();
}

void *hairer(void *param) 			//funkcja symulujaca dzialanie fryzjera
{
	wret=pthread_mutex_lock(&drzwi);	//blokada zapewniajaca wylacznosc przy wprowadzaniu zmian do poczekalni
	if (wret) 
	{
		 	fprintf(stderr, "ERROR; return code from drzwi pthread_mutex_lock is %d\n", wret);
			exit(EXIT_FAILURE);
	}
    	while(not_in+ii<clients) 		//warunek sprawdzajacy czy fryzjer moze zakonczyc prace
	{
		//sem_wait(&sem_client);
		
		wret=pthread_cond_wait(&cond_client,&drzwi);	//usypianie fryzjera
		if (wret) 
		{
		 	fprintf(stderr, "ERROR; return code from cond_client pthread_cond_wait is %d\n", wret);
			exit(EXIT_FAILURE);
		}
		printf("Res:%d WRomm: %d/%d [in: %d]  -  client went from waiting room to hairdresser\n",not_in, chairs-freechairs, chairs, in);
		freechairs ++;			//zwiekszenie liczby wolnych miejsc
		del_client();			//usuniecie ostatniego klienta z listy klientow w poczekalni
		printf("Res:%d WRomm: %d/%d [in: %d]  -  haircut starting\n",not_in, chairs-freechairs, chairs, in);
		wait();
		ii++;
		in=-1;
		printf("Res:%d WRomm: %d/%d [in: %d] -  haircut ended\n",not_in, chairs-freechairs, chairs, in);
		//sem_post(&sem_barber);
		wret=pthread_cond_signal(&cond_barber);	//wysylaniu sygnalu klientowi informujacego, ze golenie sie zakonczylo
		if (wret) 
		{
		 	fprintf(stderr, "ERROR; return code from cond_barber pthread_cond_signal is %d\n", wret);
			exit(EXIT_FAILURE);
		}
	} 
	wret=pthread_mutex_unlock(&drzwi);	//zdjecie blokady zapewniajacej wylacznosc przy wprowadzaniu zmian do poczekalni
	if (wret) 
		{
		 	fprintf(stderr, "ERROR; return code from drzwi pthread_mutex_unlock is %d\n", wret);
			exit(EXIT_FAILURE);
		}
}

void *client(void *nr)					//funkcja symulujaca dzialanie klienta
{
   	int num=*(int*)nr;
	wret=pthread_mutex_lock(&drzwi);		//blokada zapewniajaca wylacznosc przy wprowadzaniu zmian do poczekalni
	if (wret) 
	{
		 	fprintf(stderr, "ERROR; return code from drzwi pthread_mutex_lock is %d\n", wret);
			exit(EXIT_FAILURE);
	}
	if(freechairs <= 0)				//jezeli nie ma wolnego miejsca
   	{
	    	add_dropped(num);			//dodanie klienta na liste osob, ktore nie dostaly sie do poczekalni
		not_in++;
	    	printf("Res:%d WRomm: %d/%d [in: %d]  -  client dropped\n",not_in, chairs-freechairs, chairs, in);
   	}
	else						
	{
		freechairs--;				//zmniejszenie liczby wolnych miejsc
		add_waiting(num);
		if(freechairs!=chairs-1)
		{	
			wret=pthread_cond_wait(&waitt,&drzwi);	//czekanie az aktualny klient opusci zakonczy golenie
			if (wret) 
			{
		 		fprintf(stderr, "ERROR; return code from waitt pthread_cond_wait is %d\n", wret);
				exit(EXIT_FAILURE);
			}
		}
		wret=pthread_cond_signal(&cond_client);		//obudzenie fryzjera, jezeli ten spi
		if (wret) 
		{
		 	fprintf(stderr, "ERROR; return code from cond_client pthread_cond_signal is %d\n", wret);
			exit(EXIT_FAILURE);
		}
		wret=pthread_cond_wait(&cond_barber,&drzwi);	//czekanie na zakonczenie golenia
		if (wret) 
		{
		 	fprintf(stderr, "ERROR; return code from cond_barber pthread_cond_wait is %d\n", wret);
			exit(EXIT_FAILURE);
		}
		wret=pthread_cond_signal(&waitt);		//sygnalizowanie ze zakonczono golenie
		if (wret) 
		{
		 	fprintf(stderr, "ERROR; return code from waitt pthread_cond_signal is %d\n", wret);
			exit(EXIT_FAILURE);
		}
	}
	wret=pthread_mutex_unlock(&drzwi);			//zdjecie blokady zapewniajacej wylacznosc przy wprowadzaniu zmian do poczekalni
	if (wret) 
	{
		 	fprintf(stderr, "ERROR; return code from drzwi pthread_mutex_unlock is %d\n", wret);
			exit(EXIT_FAILURE);
	}
}


int main(int argc, char *argv[])
{
 	
	if (argc >6) 
	{
	    	printf("Podano za duzo argumentow");
		return 1;
	}
	if (argc <5) 
	{
	    	printf("Podano za malo argumentow");
		return 1;
	}
	srand(time(NULL));				//funkcja od czasu uzywana przy losowaniu czasow przychodzenia i golenia
   	static struct option p[] =
   	{
      	  	{"Clients", required_argument, NULL, 'k'},
     	   	{"Chairs", required_argument, NULL, 'c'},
     	   	{"Debug", no_argument, NULL, 'd'}

    	};
	int w=0;
	while((w = getopt_long(argc, argv, "k:c:d",p,NULL)) != -1)
    	{
        	switch(w)
        	{
        		case 'k': 
            		clients=atoi(optarg);
            		break;
        		case 'c': 
            		freechairs=atoi(optarg);
            		chairs=atoi(optarg);
            		break;
        		case 'd':
            		debug=true;
            		break;
        	}
    	}

	int *array=malloc(sizeof(int)*clients);		//stworzenie tablicy klientow zawierajacej numery im odpowiadajace
	if (array == NULL) 
	{
	    	printf("Blad alokacji pamieci");
		return 1;
	}
    	int i;						//zmienna pomocnicza
    	for (i=0; i<clients; i++)
    	{
        	array[i] = i;				//ustawienie zmiennej tablicy na jej numer indeksu
    	}

	pthread_t *clientsarray = malloc(sizeof(pthread_t)*clients);	//stworzenie tablicy identyfikatorow watkow klientow
	if (clientsarray == NULL) 
	{
	    	printf("Blad alokacji pamieci");
		return 1;
	}
    	pthread_t hairdresser;				//stworzenie watku fryzjera
	wret=pthread_mutex_init(&drzwi, NULL);
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from drzwi pthread_mutex_init is %d\n", wret);
		exit(EXIT_FAILURE);
        }
	wret=pthread_cond_init(&cond_barber,NULL);	//zainicjalizowanie zmiennej warunkowej fryzjera
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from cond_barber pthread_cond_init is %d\n", wret);
		exit(EXIT_FAILURE);
        }
	wret=pthread_cond_init(&cond_client,NULL);	//zainicjalizowanie zmiennej warunkowej klienta
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from cond_client pthread_cond_init is %d\n", wret);
		exit(EXIT_FAILURE);
        }
	wret=pthread_cond_init(&waitt,NULL);		//zainicjalizowanie sluzacej do sygnalizowania
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from waitt pthread_cond_init is %d\n", wret);
		exit(EXIT_FAILURE);
        }
    	wret=pthread_create(&hairdresser, 0, hairer, 0);//zainicjalizowanie watku fryzjera
        if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from Hairdresser pthread_create() is %d\n", wret);
		exit(EXIT_FAILURE);
        }
	for (i=0; i<clients; i++)			//zainicjalizowanie watkow klientow
    	{
        	wait();
		wret=pthread_create(&clientsarray[i], 0, client, (void *)&array[i]);
 		if (wret) 
		{
 			fprintf(stderr, "ERROR; return code from Client pthread_create() is %d\n", wret);
			exit(EXIT_FAILURE);
       		}
    	}
  	for (i=0; i<clients; i++)
    	{
        	wret=pthread_join(clientsarray[i],0);	//oczekiwanie na zakonczenie kazdego z watkow klientow
		if (wret) 
		{
 			fprintf(stderr, "ERROR; return code from Client pthread_join() is %d\n", wret);
			exit(EXIT_FAILURE);
        	}
    	}
	wret=pthread_join(hairdresser, 0);		//oczekiwanie na zakonczenie watku fryzjera
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from Hairdresser pthread_join() is %d\n", wret);
		exit(EXIT_FAILURE);
        }
    	wret=pthread_mutex_destroy(&drzwi);		//zniszczenie mutexu
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from drzwi pthread_mutex_destroy is %d\n", wret);
		exit(EXIT_FAILURE);
        }
	wret=pthread_cond_destroy(&cond_barber);	//zniszczenie zmiennej warunkowej
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from cond_barber pthread_cond_destroy is %d\n", wret);
		exit(EXIT_FAILURE);
        }
    	wret=pthread_cond_destroy(&cond_client);	//zniszczenie zmiennej warunkowej
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from cond_client pthread_cond_destroy is %d\n", wret);
		exit(EXIT_FAILURE);
        }
	wret=pthread_cond_destroy(&waitt);		//zniszczenie zmiennej warunkowej
	if (wret) 
	{
 		fprintf(stderr, "ERROR; return code from waitt pthread_cond_destroy is %d\n", wret);
		exit(EXIT_FAILURE);
        }
    	free(waiting);					//zwolnienie listy klientow w poczekalni
    	free(dropped);					//zwolnienie listy klientow, ktorzy nie dostali sie do poczekalni
    	return 0;
}
