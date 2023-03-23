#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "passe_1.h"
#include "utils/miniccutils.h"

extern int trace_level;
bool global = true;

void DFS(node_t root);
node_t make_node_global(node_nature nature, int64_t val, int32_t lineno);

void analyse_passe_1(node_t root)
{
    //parcours_arbre_offset_ident(root,trace_level);
    printf("COUCOU 1\n");
    DFS(root);
}

void DFS(node_t root)
{
    if(root!=NULL)
    {    
        if(root->nature == NODE_DECLS)
        {
            propaType(root->opr[1], root->opr[0]->type);
        }
        
        //Parcours en profondeur
        for (int i = 0; i<root->nops; i++)
        {
            DFS(root->opr[i]);
        }
        
        if(root->nature == NODE_DECLS)
        {
            if(root->opr[0]->type != TYPE_VOID)
            {
                propaType(root->opr[1], root->opr[0]->type);
            }
            else
            {
                printf("Error line %d: variable can not be of type void\n",root->lineno);
            }
        }
        else if(root->nature == NODE_DECL)
        {
            if(root->opr[1]!=NULL)
            {
                if(root->opr[0]->type==root->opr[1]->type && root->opr[0]->type == (TYPE_INT || TYPE_BOOL))
                {
                    root->type = root->opr[0]->type;
                }
                else
                {
                    printf("Error line %d: wrong type declared\n",root->lineno);
                }
            }
            else if(global)
            {
                if(root->opr[0]->type==TYPE_INT)
                {
                    root->opr[1] = make_node_global(NODE_INTVAL, 0, root->opr[0]->lineno);
                }
                else if(root->opr[0]->type==TYPE_BOOL)
                {
                    root->opr[1] = make_node_global(NODE_BOOLVAL, 0, root->opr[0]->lineno);
                }
            }
        }
        else if(root->nature == NODE_FUNC)
        {
            if(root->opr[0]->type==TYPE_VOID)
            {
                if(!strcmp(root->opr[1]->ident, "main"))
                {
                    global = false;
                }
                else
                {
                    printf("Error line %d: function's name is not main\n",root->lineno);
                }
            }
            else
            {
                printf("Error line %d: function's type is not void\n",root->lineno);
            }
        }
        else if(root->nature == (NODE_IF || NODE_WHILE))
        {
            if(root->opr[0]->type!=TYPE_BOOL)
            {
                printf("Error line %d: false condition\n",root->lineno);
            }
        }
        else if(root->nature == (NODE_FOR || NODE_DOWHILE))
        {
            if(root->opr[1]->type != TYPE_BOOL)
            {
                printf("Error line %d: false condition\n",root->lineno);
                //Verif autres fils   
            }
        }
        else if(root->nature == (NODE_PLUS || NODE_MINUS || NODE_MUL || NODE_DIV || NODE_MOD))
        {
            if(!(root->opr[0]->type == TYPE_INT && root->opr[1]->type == TYPE_INT))
            {
                printf("Error line %d: operator not working on non-int variables\n",root->lineno);
            }
            root->type = TYPE_INT;
        }
        else if(root->nature == (NODE_LT || NODE_GT || NODE_LE || NODE_GE))
        {
            if(root->opr[0]->type != TYPE_INT || root->opr[1]->type != TYPE_INT)
            {
                printf("Error line %d: false condition\n",root->lineno);
            }
            root->type = TYPE_BOOL;
        }
        else if(root->nature == (NODE_EQ || NODE_NE))
        {
            if(root->opr[0]->type!=root->opr[1]->type || root->opr[0]->type ==(TYPE_VOID || TYPE_NONE))
            {
                printf("Error line %d: false condition\n",root->lineno);
            }
            root->type = TYPE_BOOL;
        }
        else if(root->nature == (NODE_AND || NODE_OR))
        {
            if(!(root->opr[0]->type == TYPE_BOOL && root->opr[1]->type == TYPE_BOOL))
            {
                printf("Error line %d: wrong type (operator requires boolean)\n",root->lineno);
            }
            root->type = TYPE_BOOL;
        }
        else if(root->nature == (NODE_BAND || NODE_BOR || NODE_BXOR))
        {
            if(root->opr[0]->type != TYPE_INT || root->opr[1]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
            }
            root->type = TYPE_INT;
        }
        else if(root->nature == NODE_NOT)
        {
            if(root->opr[0]->type != TYPE_BOOL)
            {
                printf("Error line %d: wrong type (operator requires boolean)\n",root->lineno);
            }
            root->type = TYPE_BOOL;
        }
        else if(root->nature == NODE_BNOT)
        {
            if(root->opr[0]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
            }
            root->type = TYPE_INT;
        }
        else if(root->nature == (NODE_SLL || NODE_SRL || NODE_SRA))
        {
            if(root->opr[0]->type != TYPE_INT || root->opr[1]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
            }
            root->type = TYPE_INT;
        }
        else if(root->nature == NODE_UMINUS)
        {
            if(root->opr[0]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
            }
            root->type = TYPE_INT;
        }
        else if(root->nature == NODE_AFFECT)
        {
            if(root->opr[0]->type != root->opr[1]->type)
            {
                printf("Error line %d: wrong type (operator requires two operands of the same type)\n",root->lineno);
            }
            root->type = root->opr[0]->type;
        }
        else if(root->nature == NODE_PRINT)
        {
            if(root->opr[0]->type != root->opr[1]->type)
            {
                printf("Error line %d: wrong type (operator requires two operands of the same type)\n",root->lineno);
            }
            root->type = root->opr[0]->type;
        }
    }
}

node_t make_node_global(node_nature nature, int64_t val, int32_t lineno)
{
    node_t new_node=malloc(sizeof(node_s));
    new_node->nops=0;
    new_node->nature=nature;
    new_node->value=val;
    new_node->lineno=lineno;
    //new_node->node_num=nbNode++;
    return new_node;
}

void propaType(node_t root, node_type type)
{
    for(int i = 0; i < root->nops; i++)
    {
        propaType(root->opr[i],type);
    }
    if((root->nature)==NODE_IDENT)
    {
        root->type=type;
    }
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

