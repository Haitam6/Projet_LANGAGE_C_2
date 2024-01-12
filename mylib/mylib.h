// Bibliothèque contenant les fonctions load,search et parse

struct TreeNode *load(const char *fname);
void search(struct TreeNode *root, char *entry);
int parse(const char *src, char *v1, char *v2);