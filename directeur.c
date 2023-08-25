#include "musee.h"

int main (int argc, char * argv[])
{
    if ( (argc == 0) || (argc == 1) || (argc > 5))
    {
        fprintf(stderr, "usage: <PARAM>\n");
        exit(EXIT_FAILURE);
    }
    int ordre, ret;
    int capacite;
    int file;
    int shmid;
    org * s;
    char * endptr;
    s = NULL;

    /* on initialise ret (dans chaque programme) par la 
     * par la valeur de creer_debug_musee
     * si celle-ci est égal ou superieur à 1 
     * on se permettra d'écrire certaines informations
     */ 
    ret = creer_debug_musee();

    if ( (argc == 4) && ((ordre = strncmp(argv[1], "creer", 5)) == 0)) 
    {
        if (ret >= 1)
            printf("Creation du meilleur musee du monde \n");

        // recuperation de la capacite max.
        capacite = strtol(argv[2], &endptr, 10);
        if (errno > 0)
            raler("strtol");
        if (*endptr == '\0')
        {
            if ( capacite <= 0 )
            {
                fprintf(stderr, "usage: capacite\n" );
                exit(EXIT_FAILURE);
            }
        }

        // recuperation de la file max.
        file = strtol(argv[3], &endptr, 10);
        if (errno > 0)
            raler("strtol");
        if (*endptr == '\0')
        {
            if ( file < 0 )
            {
                fprintf(stderr, "usage: file\n" );
                exit(EXIT_FAILURE);
            }
        }

        /* on crée la mémoire partagé ou on stockera 
         * toutes les informations nécessaires et 
         * on crée également les sémaphores et/ou mutex
         * on initialise le musee a fermé 
         */
        shmid = creer_shm(CHEMIN);
        s = shmat(shmid, NULL, 0);
        s->capacite=capacite;
        s->capacite_courante = 0;
        s->file = file;//mise en place de la file
        s->file_courante = 0;
        s->statut = 1;//musee ferme
        s->semid = sem_create(CHEMIN, NB_NSEM);
    }
    else if ((argc == 4) && (ordre = strncmp(argv[1], "creer", 5)) == -1)
        raler("strncmp");

    else if((argc == 2) && ((ordre = strncmp(argv[1], "ouvrir", 6))== 0))
    {
        if (ret >= 1)
            printf("Ouverture du meilleur musee du monde \n");

        /* pour recuperer les informations, on doit recuperer la 
         * memoire partage, ainsi si l'on recupere celle-ci
         * on a accès à la structure donnant toutes les informations
         * nécessaires, on fera le même processus pour récupérer les
         * informations dans les differents programmes 
         */
        shmid = recup_shm(CHEMIN);
        s = shmat(shmid, NULL, 0);
        // on passe le musee en mode ouvert / permet de rentrer
        // dans la boucle while du controleur si il recoit la semaphore
        s->statut = 0;
        // le directeur autorise l'accès a son musée
        V(s->semid, SEM1);   
    }
  
    else if((argc == 2) && ((ordre = strncmp(argv[1], "fermer", 6)) == 0) )
    {
        if (ret >= 1)
            printf("Fermeture du meilleur musee du monde \n");
        
        shmid = recup_shm(CHEMIN);
        s = shmat(shmid, NULL, 0);
        if (shmid == -1)
        {
            fprintf(stderr, "usage: musee d'athena introuvable\n" );
            exit(EXIT_FAILURE);
        }
        s->semid = sem_getid(CHEMIN, NB_NSEM);
        /* on appelle une semaphore ici : si le musee est ouvert, 
         * cela permet de sortir de la boucle while
         * et de stopper l'attente du controleur 
         */
        s->statut = 1; 
        V(s->semid,SEM3); 
    }

    else if((argc == 2) && ((ordre = strncmp(argv[1], "supprimer", 9)) == 0))
    {
        if ( ret >= 1)
            printf("Suppression du meilleur musee du monde\n");
        shmid = recup_shm(CHEMIN);
        s = shmat(shmid, NULL, 0);
        if (shmid == -1)
        {
            fprintf(stderr, "usage: musee d'athena introuvable\n" );
            exit(EXIT_FAILURE);
        }
        // on supprime la memoire partage et le descripteur du semaphore
        sem_destroy(s->semid); // non ce n'est pas une semaphore POSIX
        supprimer_shm(shmid);
    }
    else if((argc == 2) && ( (ordre = strncmp(argv[1], "fermer", 6))  == -1
        || (ordre = strncmp(argv[1], "ouvrir", 6)) == -1 
        || (ordre = strncmp(argv[1], "supprimer", 9)) == -1))
        raler("strncmp");

    else 
    {
        // si le programme n'est rentré dans aucune des conditions
        // c'est que les arguments n'étaient pas corrects
        fprintf(stderr, "usage: arg\n" );
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
