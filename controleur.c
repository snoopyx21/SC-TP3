#include "musee.h"

void nouveau_visiteur(int ret)
{
    /* un nouveau visiteur se presente dans le plus beau musee du monde */
    if (ret >= 1)
        printf("Tiens, un nouveau visiteur ! \n");
}
int main(int argc, char * argv[])
{
    if (argc != 1)
    {
        fprintf(stderr, "usage: argument controleur\n" );
        exit(EXIT_FAILURE);
    }
    if (strncmp(argv[0], "./controleur",12 ) == -1)
    {
        fprintf(stderr, "usage: argument controleur\n" );
        exit(EXIT_FAILURE);
    }
  	int shmid, ret;
    org * s;
    shmid = recup_shm(CHEMIN);
    s = shmat(shmid, 0, 0);
    s->semid = sem_getid(CHEMIN, NB_NSEM);
    ret = creer_debug_musee();

    // le controleur autorise l'accès, le directeur en a donné l'ordre 
    P(s->semid,SEM1); 
    // tant que le musee est ouvert 
    while(s->statut == 0) 
    {
        P(s->semid, SEM3); // presence d'un visiteur
        /* si par malheur, dans le programme, le directeur décide de fermer
         * il faut pouvoir reussir à fermer
         */
        if (s->statut == 1) // si fermé
        {
            if(ret > 1)
                printf("Fermeture du musee, veuillez sortir !\n");
            exit(EXIT_FAILURE);
        }
        nouveau_visiteur(ret);

        // mutex pour la capacite
        P(s->semid,MUT1); 
        if (ret > 1)
            printf("capacite_courante : %d \n",s->capacite_courante);
        if(s->capacite_courante < s->capacite)
        {
            /* si un visiteur se présente, on regarde 
             * déjà si il y a de la place
             * et si c'est le cas, on augmente la capacite de 1
             */
            s->capacite_courante=s->capacite_courante+1;
            // on peut fermer le mutex de capacite, on a modifié sa valeur
            V(s->semid,MUT1);

            /* si une place se libère, le controleur a le droit
             * de le faire rentrer
             */
            V(s->semid,SEM2);

            // mutex pour la file d'attente
            P(s->semid,MUT2); 
            s->file_courante = s->file_courante - 1;
            if (ret > 1)
                printf("file_courante : %d \n",s->file_courante );
            // fermeture du mutex pour la file d'attente
            V(s->semid,MUT2); 
        }
        else // si il n'y a plus de place dans le musee
        {
            // on fermera la mutex de capacite
            V(s->semid,MUT1);
            // on dit au visiteur qu'il ne peut pas rentrer
            V(s->semid,SEM3);
            if (ret >= 1)
                printf("Désolé, je ne peux pas vous laissez rentrer\n");
        }
        if (ret >= 1)
            printf("Merci de votre visite, au plaisir de vous revoir !\n");
    }
 
    return EXIT_SUCCESS;
}
