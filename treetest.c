#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mylib/mylib.h"


// Fonction pour charger une arborescence et rechercher un fichier dans celle-ci

int main(int argc, char **argv)
{
   
    char v1[BUFSIZ];
    char v2[BUFSIZ];

    FILE *fp;

    char s[BUFSIZ];

    fp = fopen("tree.conf", "r"); // On ouvre le fichier de configuration "tree.conf"

    if (fp == NULL)
    {
        perror("tree.conf"); // Affiche une erreur si le fichier de configuration n'est pas accessible
        return 1;
}

while(fgets(s,BUFSIZ,fp)!=NULL)
{     
    s[strlen(s)]='\0' ; // Supprime le caractère de retour à la ligne
    char *ptr = strchr(s,'#'); // Recherche d'un commentaire

    if(ptr!=NULL)
    {
        *ptr='\0' ; // Pour ne pas tenir compte des lignes de commentaire
    }
    else
    {
        continue;
    }
}

fclose(fp); // On ferme le fichier de configuration "tree.conf"
parse(s,v1,v2); // Analyse la ligne lue pour extraire v1 et v2

struct TreeNode *root =load((const char *) v2); // On charge l'arborescence à partir du répertoire spécifié


if(argc !=2 )
{
    printf("Usage:\n\t./treetest file\n");
    return 0;
}

search(root,argv[1]); // Recherche le fichier spécifié dans l'arborescence

    return 0;
}

