#include "musee.h"
/* Fonction pour les erreurs
 */
void raler(char * msg)
{
    if (msg == NULL)
    {
        fprintf(stderr, "usage: > OUTPUT FATAL ERROR\n");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "usage: " );
	perror(msg);
	exit(EXIT_FAILURE);
}

/* fonction de debug 
 */
 int creer_debug_musee(void)
 {
 	char * choix;
 	int choix2;
 	choix = getenv("DEBUG_MUSEE");
 	if (choix == NULL)
 		choix2 = 0;	
 	else
 		choix2 = atoi(choix);
 	return choix2;
 }

/***************************PARTIE MEMOIRE PARTAGE************************/


/* Fonction créant une mémoire partagée et
 * et retournant son descripteur 
 */
int creer_shm(char * chemin_fichier)
{
	key_t key;
	int shmid;
	key = ftok(chemin_fichier, 'C');
	if (key == -1)
		raler("ftok");
	shmid = shmget(key, sizeof(org), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        if ( (shmctl(shmid, IPC_RMID, NULL)) == -1)
            raler("shmctl");
        raler("shmget");
    }
	return shmid;
}

/* Fonction d'accés à la mémoire partagé
 * qui retourne le descripteur de la mémoire partagé
 */
int recup_shm(char * chemin_fichier)
{
	key_t key;
	int shmid;
	key = ftok(chemin_fichier, 'C');
	if (key == -1)
		raler("ftok");
	if((shmid=shmget(key,0,0))==-1)
		raler("shmget");
	return shmid;
}

/* Fonction de suppression de mémoire partagé
 */
void supprimer_shm(int shmid)
{
	int ret;
	ret = shmctl(shmid, IPC_RMID, NULL);
	if (ret == -1)
		raler("shmctl");
}



/***************************** PARTIE SEMAPHORE************************/


/* Fonction qui crée une  semaphore 
 * et retourne son descripteur
 */
int sem_create(char * chemin_fichier, int nb_semaphore) 
{
	int semid;
	key_t key;
	key = ftok(chemin_fichier, 'C');
	if (key == -1)
		raler("ftok");
	semid = semget(key, nb_semaphore, IPC_CREAT | IPC_PRIVATE | 0666); 
	if (semid == -1)
		raler("semget");
	semctl(semid, SEM1, SETVAL, 0);
	if (errno > 0)
		raler("semctl");
	semctl(semid, SEM2, SETVAL, 0);
	if (errno > 0)
		raler("semctl");
	semctl(semid, SEM3, SETVAL, 0);
	if (errno > 0)
		raler("semctl");
	semctl(semid, MUT1, SETVAL, 1);
	if (errno > 0)
		raler("semctl");
	semctl(semid, MUT2, SETVAL, 1);
	if (errno > 0)
		raler("semctl");
    return semid;
}

/* Retourne le descripteur de la semaphore
 */
int sem_getid(char * chemin_fichier, int nb_semaphore) 
{
	int semid;
	key_t key;
	key = ftok(chemin_fichier, 'C');
	if (key == -1)
		raler("ftok");
	semid = semget(key, nb_semaphore, 0);
	if (semid == -1)
		raler("semget");
	return semid;  
}

void P ( int id, int nsem )
{
	struct sembuf s [1] = { {nsem , -1 , 0} } ;
	if ( semop ( id , s , 1) == -1)
		raler ("semop");
}

void V ( int id, int nsem)
{
	struct sembuf s [1] = { {nsem , 1 , 0} } ;
	if ( semop ( id , s , 1) == -1)
		raler ("semop") ;
}

int getsem_val(int sem, int num)
{
	int ret;
	ret = semctl(sem, num, GETVAL, 0);
	return ret;
}

/* Demande au systeme la destruction du semaphore d'identificateur sem. */
void sem_destroy(int sem) 
{
	int ret;
	ret = semctl(sem, 0, IPC_RMID);
	if (ret == -1)
		raler("semclt");
}
