#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

// Build_path qui permet de construire le chemin d'accès à un fichier à partir de son nom et du chemin d'accès à son répertoire root

char *build_path(const char *root, const char *filename)
{
    char *path = malloc(strlen(root) + strlen(filename) + 2); // On alloue la mémoire nécessaire pour stocker le chemin d'accès
    strcpy(path, root); // On copie le chemin d'accès du répertoire root dans path
    strcat(path, "/"); // On ajoute un / à la fin du chemin d'accès
    strcat(path, filename); // On ajoute le nom du fichier à la fin du chemin d'accès
    strcat(path, "\0"); // On ajoute un caractère de fin de chaîne
    return path; // On retourne le chemin d'accès
}


// Fonction load qui charge l'arborscence d'un répertoire dans une structure de donnée TreeNode

struct TreeNode *load(const char *fname)  // fname est le répertoire dont on souhaite charger l'arborescence
{
     struct TreeNode *root = (struct TreeNode*) malloc(sizeof(struct TreeNode));
     struct stat s;
	 if (lstat(fname, &s) < 0) 
	 {
		 perror("lstat"); // Affiche une erreur si le répertoire n'est pas accessible
		 exit(EXIT_FAILURE);
	 }

	if (S_ISREG(s.st_mode) || S_ISLNK(s.st_mode)) { // Vérifie si le chemin spécifié correspond à un fichier ou un lien symbolique
		root->fname = (char *) fname; // On stocke le nom du fichier dans root->fname pour affecter le chemin du fichier au noeud racine
		root->contents = NULL; // On initialise root->contents à NULL pour dire qu'il n'y a pas de contenu
		root->next = NULL; // On initialise root->next à NULL pour dire qu'il n'y a pas de répertoire suivant
		
	}
    else if(S_ISDIR(s.st_mode)){ // Vérifie si le chemin spécifié correspond à un répertoire
        DIR *dir;
		struct dirent *direntry;
		struct TreeNode *prev = NULL;
		root->fname = (char *)fname;
		root->contents = NULL;
		root->next = NULL;
		dir = opendir(fname);

        if (dir == NULL){
			perror("Impossible d'ouvir le répertoire");
		}
		else
		{

while ( (direntry = readdir(dir)) != NULL){ // Parcourt les entrées du répertoire
	if ((*direntry).d_name[0] != '.')	// Vérifie si le nom du répertoire courant ne commence pas par un point (c'est-à-dire, n'est pas un répertoire caché)
	{ 
		struct TreeNode *curr = (struct TreeNode *)malloc(sizeof(struct TreeNode)); // entree du répertoire courant
		char *path = build_path(fname, (*direntry).d_name);							// On utilise désormais la fonction build_path pour construire le chemin d'accès à l'entrée du répertoire courant
		curr = load(path);															// Charge les données du répertoire courant en utilisant la fonction load et stocke le résultat dans la structure curr.
		if (prev == NULL)
		{
			prev = curr;
		}
		else
		{
			prev->next = curr;
			prev = prev->next;
		}
		if (root->contents == NULL)
		{
			root->contents = prev;
		}
	}
		}
		closedir(dir); // On ferme le répertoire

		}
        
    }
return root; // On retourne le noeud racine de l'arborescence
}