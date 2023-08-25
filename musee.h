#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>


#define CHEMIN "/etc"
#define NB_NSEM 5
#define SEM1 0
#define SEM2 1
#define SEM3 2
#define MUT1 3
#define MUT2 4

typedef struct s_organisation 
{
	int capacite; // capacite max.
	int capacite_courante; // capacite actuelle du musee
	int file; // file max.
	int file_courante; // file actuelle du musee
	int semid; // descripteur du semaphore
	int statut; // statut du musee defini : 0 pour ouvert et 1 pour fermé
} org;



void raler(char * msg);
int creer_debug_musee(void);
int creer_shm(char * chemin_fichier);
int recup_shm(char * chemin_fichier);
void supprimer_shm(int shmid);
int sem_create(char * chemin_fichier, int nb_semaphore);
int sem_getid(char * chemin_fichier, int nb_semaphore);
void P(int id, int nsem);
void V(int id, int nsem);
int getsem_val(int sem, int num);
void sem_destroy(int sem);
