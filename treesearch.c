#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "mylib/mylib.h"


//Fonction treesearch qui récupère un nom de fichier en ligne de commande, le mémorise dans le fichier spécifié dans le fichier de configuration (tree.conf), puis envoie un signal SIGHUP au programme treeload pour déclencher une recherche.

int main(int argc, char **argv)
{
    // Structure pour stocker les chemins des fichiers à manipuler
    struct config
    {
        char rootdir[BUFSIZ];
        char datafile[BUFSIZ];
        char pidfile[BUFSIZ];
    };

    // Configuration des fichiers
    struct config conf;

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

    // Écriture du nom de fichier passé en argument dans le fichier datafile
    FILE *data_file = fopen(conf.datafile, "w");
    if (data_file == NULL)
    {
        perror("Erreur pour ouvrir datafile");
        return 1;
    }

    int num_written = fprintf(data_file, "%s\n", argv[1]);
    if (num_written < 0)
    {
        perror("Erreur pour écrire à datafile");
        fclose(data_file);
        return 1;
    }

    fclose(data_file);

    // Ouverture du fichier pidfile pour obtenir le PID
    FILE *pid_file = fopen(conf.pidfile, "r");
    if (pid_file == NULL)
    {
        perror("Erreur pour ouvrir pidfile");
        return 1;
    }

    int pid;
    fscanf(pid_file, "%d", &pid);

    fclose(pid_file);

    // Affichage du PID
    printf("PID: %d\n", pid);

    // Envoi du signal SIGHUP au processus treeload avec le PID
    pid = (pid_t)pid;
    kill(pid, SIGHUP);

    return 0;
}
