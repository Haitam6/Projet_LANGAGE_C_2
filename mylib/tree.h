#include<string.h>

// structure de données pour stocker des informations que un seul fichier

struct TreeNode {
    char *fname;  // fname est le nom du fichier
    struct TreeNode *contents;   // contents est la liste des fichiers contenus dans le repertoire
    struct TreeNode *next;  // next est le pointeur vers le prochain element de la liste ce qui représente le fichier suivant
};