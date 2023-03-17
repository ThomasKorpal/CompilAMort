
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "passe_1.h"

extern int trace_level;

void analyse_passe_1(node_t root)
{
    //parcours_arbre_offset_ident(root,trace_level);
}

/*void parcours_arbre_offset_ident(node_t root, int trace_level)
{
    //En gros on fait un DFS ou on va reperer tous les noeuds NODE_IDENT
    //On met a jour les offset des NODE_IDENT fils d'un NODE_DECLS 
    //Si le NODE_DECLS est fils d'un NODE_LIST => initialisation des offsets global au NODE_LIST
    node_t* tab_node_ident = DFS_OFFSET_IDENT(root,trace_level);
    int nbNodeIdent = sizeof(tab_node_ident)/sizeof(tab_node_ident[0]);
    for(int i=0; i<nbNodeIdent; i++)
    {
        if(tab_node_ident[i]->offset == -1 && tab_node_ident[i]->decl_node == NULL)
        {
            for(int j=0; j<nbNodeIdent; j++)
            {
                if(!strcmp(tab_node_ident[i]->ident,tab_node_ident[j]->ident) && i!=j && tab_node_ident[j]->offset >= 0)//Identifier les noeuds NODE_IDENT qui sont des declarations de variable
                {
                    tab_node_ident[i]->decl_node=tab_node_ident[j];
                    if(trace_level == 2)
                    {
                        printf("Adding node connexion\n");
                    }
                    else if(trace_level > 3)
                    {
                        printf("Connexion : node %d to node",i,j);
                    }
                }
            }
        }
    }
}node_t* DFS_OFFSET_IDENT(node_t root, int trace_level)
{
    //DFS qui va compter le nombre de NODE_IDENT de l'arbre et retourner un tableau des noeuds
    //On initialisera les offset dans cette fonction
}*/

