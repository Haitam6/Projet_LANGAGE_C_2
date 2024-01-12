#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "mylib/mylib.h"

// Fonction treeload qui charge une arborescence en mémoire et reste en attente du signal SIGHUP. Lorsqu'elle reçoit ce signal, elle lit le nom d'un fichier à rechercher depuis un fichier de configuration (tree.conf), écrit les emplacements correspondants, puis reste actif. 

void catcher(int signum)
{
    puts("Signal reçu\n");
}

int main()
{
    // Structure pour stocker les chemins des fichiers à charger
    struct config
    {
        char rootdir[BUFSIZ];
        char datafile[BUFSIZ];
        char pidfile[BUFSIZ];
    };

    // Configuration de signal
    struct config conf;
    struct sigaction sigact;

    // Initialisation du gestionnaire de signal pour SIGHUP
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction(SIGHUP, &sigact, NULL);

    FILE *fp;
    char s[BUFSIZ];
    char v1[BUFSIZ];
    char v2[BUFSIZ];

    // Lecture des valeurs de configuration depuis le fichier tree.conf
    fp = fopen("tree.conf", "r");
    if (fp == NULL)
    {
        perror("Erreur pour ouvrir tree.conf");
        return 1;
    }

    while (fgets(s, BUFSIZ, fp) != NULL)
    {
        s[strlen(s)] = '\0'; // supprime le \n final

        char *ptr = strchr(s, '#');
        if (ptr != NULL)
        {
            *ptr = '\0'; // pour ne pas tenir compte des lignes de commentaire
        }
        else
        {
            parse(s, v1, v2);

            if (strcmp(v1, "rootdir") == 0)
            {
                strcpy(conf.rootdir, v2);
            }
            else if (strcmp(v1, "datafile") == 0)
            {
                strcpy(conf.datafile, v2);
            }
            else if (strcmp(v1, "pidfile") == 0)
            {
                strcpy(conf.pidfile, v2);
            }
        }
    }

    fclose(fp);

    // Affichage des informations de configuration
    printf("rootdir: %s\n", conf.rootdir);
    printf("datafile: %s\n", conf.datafile);
    printf("pidfile: %s\n", conf.pidfile);

    // Chargement de l'arborescence à partir du répertoire racine
    struct TreeNode *root = load((const char *)conf.rootdir);

    // Écriture du PID dans le fichier pidfile
    FILE *f_pid = fopen(conf.pidfile, "w");

    if (f_pid == NULL)
    {
        perror("Erreur pour ouvrir pidfile");
        return 1;
    }

    pid_t pid = getpid();
    char pid_str[16];
    sprintf(pid_str, "%d", pid);

    int written = fprintf(f_pid, pid_str);
    if (written < 0)
    {
        perror("Erreur pour écrire à pidfile");
        fclose(f_pid);
        return 1;
    }

    fclose(f_pid);

    // Pause du programme jusqu'à ce qu'un signal SIGHUP soit reçu
    pause();

    // Ouverture du fichier de données
    FILE *data_file = fopen(conf.datafile, "r");

    if (data_file == NULL)
    {
        perror("Erreur pour ouvrir datafile");
        return 1;
    }

    // Lecture du buffer depuis le fichier de données
    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, data_file) != NULL)
    {
        buffer[strlen(buffer) - 1] = '\0'; // supprime le \n final
    }

    fclose(data_file);

    // Recherche du buffer dans l'arborescence
    search(root, buffer);

    // Boucle infinie pour maintenir le programme actif après la recherche
    while (1);
    return 0;
}
