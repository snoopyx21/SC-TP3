#include "musee.h"

int main(int argc, char * argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "usage: argument dump\n" );
        exit(EXIT_FAILURE);
    }
    if (strncmp(argv[0], "./dump",6 ) == -1)
    {
        fprintf(stderr, "usage: argument dump\n" );
        exit(EXIT_FAILURE);
    }
	int shmid, ret;
    org * s;
    shmid = recup_shm(CHEMIN);
    s = shmat(shmid, 0, 0);
    s->semid = sem_getid(CHEMIN, NB_NSEM);

    ret = creer_debug_musee();

    if (ret >= 1)
    {
        fprintf(stdout, "Le meilleur musee du monde a comme : \n");
        fprintf(stdout, "- capacite maximum : %d\n", s->capacite);
        fflush(stdout);
        fprintf(stdout, "- capacite courante : %d\n", s->capacite_courante );
        fprintf(stdout, "- file d'attente maximum : %d\n", s->file);
        fflush(stdout);
        fprintf(stdout, "- file d'attente courante : %d\n",s->file_courante );
        if(s->statut == 1)
        {
            fprintf(stdout, "Le meilleur musee du monde est actuellement ferme\n");
            fflush(stdout);
        }
        if (s->statut == 0)
        {
            fprintf(stdout, "Le meilleur musee du monde est actuellement ouvert\n");
            fflush(stdout);
        }
        if (ret > 1)
        {
            fprintf(stdout,"semaphore numero %d = %d\n", SEM1, 
                getsem_val(s->semid, SEM1));
            fprintf(stdout,"semaphore numero %d = %d\n", SEM2, 
                getsem_val(s->semid, SEM2));
            fflush(stdout);
            fprintf(stdout,"semaphore numero %d = %d\n", SEM3, 
                getsem_val(s->semid, SEM3));
            fprintf(stdout,"semaphore numero %d = %d\n", MUT1, 
                getsem_val(s->semid, MUT1));
            fflush(stdout);
            fprintf(stdout,"semaphore numero %d = %d\n", MUT2, 
                getsem_val(s->semid, MUT2));
        }
        printf("Affichage des valeurs terminées, au revoir !\n");
        fflush(stdout);
    }
	return 0;
}
