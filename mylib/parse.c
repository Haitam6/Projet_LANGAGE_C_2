#include<stdio.h>
#include<string.h>
#include<stdlib.h>

 // Fonction qui permet de parser une ligne du fichier config de la forme "variable = valeur" et de stocker v1 dans v1 et v2 dans v2 pour que v1 garde la variable et v2 la valeur

int parse(const char *src, char *v1, char *v2)
{
  return sscanf(src,"%s = %s",v1,v2)==2 ;
}