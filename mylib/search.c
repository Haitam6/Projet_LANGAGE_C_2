#include <stdio.h>
#include <libgen.h>
#include "tree.h"
#include <string.h>

// Fonction search qui permet de chercher une entrée dans une structure de donnée et affiche les occurences trouvées

void search(struct TreeNode *root, char *entry) // Entry est le nom de l'entrée que l'on souhaite chercher les occurences
{
	 if (root->contents == NULL ) // Verifie s'il s'agit d'un fichier ou d'un repertoire vide
	 { 
		if( strcmp( basename(root->fname),entry )==0 ) // Si c'est un fichier et que son nom correspond à l'entrée recherchée
		{
			printf(">>>%s", root->fname);
			printf("\n");
		}
	}else // Sinon, si c'est un repertoire non vide
	{ 
		
        if( strcmp( basename(root->fname),entry  )==0 ) // Si c'est un repertoire et que son nom correspond à l'entrée recherchée
		{
           printf(">>>%s",root->fname);
		   printf("\n");
		}

		search(root->contents,entry); // On recherche l'entrée dans le contenu du repertoire courant
		
	}
	
	if (root->next != NULL) // Si le repertoire courant n'est pas le dernier
	{
		search(root->next,entry); // On recherche l'entrée dans le repertoire suivant	
	}
}