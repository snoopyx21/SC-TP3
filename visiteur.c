 #include "musee.h"
void sortie_client(int ret)
{
    /* sortie du visiteur */
    if (ret >= 1)
        printf("C'etait vraiment la meilleur visite du monde \n");
}
int visite_chez_athena(int visiteur)
{
    /* visite du musee */
    sleep(visiteur / 1000);
    return 0;
}
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: arg\n" );
        exit(EXIT_FAILURE);
    }
    int shmid, ret;
    int visite = 1 ;
    int visiteur;
    char * endptr;
    org * s;
    shmid = recup_shm(CHEMIN);
    s = shmat(shmid, 0, 0);
    s->semid = sem_getid(CHEMIN, NB_NSEM);
    ret = creer_debug_musee();

    /* recuperation du temps de visite */
    visiteur = strtol(argv[1], &endptr, 10);
    if (errno > 0)
      raler("strtol");
    if (*endptr == '\0')
    {
        /* verification que le temps soit positif */
        if ( visiteur <= 0 )
        {
            fprintf(stderr, "usage: nombres de visiteurs intolerable\n" );
            exit(EXIT_FAILURE);
        }
    }

    // mutex pour la file d'attente
    P(s->semid,MUT2); 
    if(s->file_courante < s->file)
    {
        s->file_courante=s->file_courante+1;
        // fermeture de la mutex de la file d'attente
        V(s->semid,MUT2);  
    }
    else // plus de place dans la file d'attente, le visiteur n'attend pas
    {
        // fermeture de la mutex de la file d'attente
        V(s->semid,MUT2); 
        if (ret >= 1)
            printf("usage: PAR LA COLERE DE ZEUS, JE N'ATTENDRAIS PAS !\n"); 
        exit(EXIT_FAILURE);
    }
    if (ret > 1)
        printf("file_courante : %d\n",s->file_courante );

    // un visiteur se presente devant le controleur 
    V(s->semid, SEM3);
    // il attend maintenant une place dans le musee (capacite)
    P(s->semid,SEM2);

    // il fait sa petite visite
    visite = visite_chez_athena(visiteur);
    if (visite != 0)
    {
        printf("usage: La visite s'est mal passé\n");
        exit(EXIT_FAILURE);
    }

    /* sa visite s'est terminé, la capacité diminue vu 
     * que le visiteur sort du musee
     * utilisation de la mutex de capacite
     */ 
    P(s->semid,MUT1); 
    s->capacite_courante=s->capacite_courante-1;
    // fermeture de la mutex de capacite
    V(s->semid,MUT1); 

    if (ret > 1)
        printf("capacite courante : %d\n", s->capacite_courante);
    // le client sort, heureux d'avoir pu visité le meilleur musee du monde
    sortie_client(ret);
    
    return EXIT_SUCCESS;
}