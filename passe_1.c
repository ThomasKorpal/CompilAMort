#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "passe_1.h"
#include "utils/miniccutils.h"

extern int trace_level;
extern int flagVerif;
bool global = true;

void DFS(node_t root);
node_t make_node_global(node_nature nature, int64_t val, int32_t lineno);
void verifprint(node_t root);

void analyse_passe_1(node_t root)
{
    //parcours_arbre_offset_ident(root,trace_level);
    push_global_context();
    DFS(root);
}

void DFS(node_t root)
{
    //printf("Node : line n° %d : %d\n",root->lineno,root->node_num);
    if(root!=NULL)
    {    
        if(root->nature == NODE_BLOCK)
        {
            push_context();
        }
        if(root->nature == NODE_FUNC)
        {
            reset_env_current_offset();
            global = false;
        }
        if(root->nature == NODE_DECLS)
        {
            propaType(root->opr[1], root->opr[0]->type);
        }
        if(root->nature == NODE_DECL)
        {   //ajout à l'environnement
            int test = env_add_element(root->opr[0]->ident,root->opr[0]);
            if(test >= 0)
            {
                root->opr[0]->offset = test;
            }
            else
            {
                printf("Error line %d: variable already declared\n",root->lineno);
                exit(EXIT_FAILURE);
            }
        }
        //Parcours en profondeur
        for (int i = 0; i<root->nops; i++)
        {
            DFS(root->opr[i]);
        }

        // traitement des verifications
        if(root->nature == NODE_BLOCK)
        {
            pop_context();
        }    
        if(root->nature == NODE_DECLS)
        {
            if(root->opr[0]->type == TYPE_VOID)
            {
                printf("Error line %d: variable can not be of type void\n",root->lineno);
                flagVerif = 1;
            }
        }
        if(root->nature == NODE_DECL)
        {
            if(root->opr[1]!=NULL)
            {
                if((root->opr[0]->type==root->opr[1]->type) && (root->opr[0]->type==TYPE_INT || root->opr[0]->type==TYPE_BOOL))
                {
                    root->type = root->opr[0]->type;
                    root->global_decl = global;
                }
                else
                {
                    printf("Error line %d: wrong type declared\n",root->lineno);
                    flagVerif = 1;
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
                root->global_decl = true;
            }
        }
        if(root->nature == NODE_IDENT)
        {
            node_t node_pt = get_decl_node(root->ident);
            if((node_pt != root) && (node_pt != NULL))
            {
                root->decl_node = node_pt;
                root->type = root->decl_node->type;
            }
            else if(strcmp(root->ident, "main"))
            {
                if(node_pt != root)
                {
                    printf("Error line %d: non existing variable\n",root->lineno);
                    exit(EXIT_FAILURE);
                }
            }
        }
        if(root->nature == NODE_STRINGVAL)
        {
            root->offset = add_string(root->str);
        }
        if(root->nature == NODE_FUNC)
        {
            if(root->opr[0]->type==TYPE_VOID)
            {
                if(strcmp(root->opr[1]->ident, "main"))
                {
                    printf("Error line %d: function's name is not main\n",root->lineno);
                    flagVerif = 1;
                }
            }
            else
            {
                printf("Error line %d: function's type is not void\n",root->lineno);
                flagVerif = 1;
            }
            root->offset = get_env_current_offset();
        }
        if(root->nature == NODE_IF || root->nature == NODE_WHILE)
        {
            if(root->opr[0]->type!=TYPE_BOOL)
            {
                printf("Error line %d: false condition\n",root->lineno);
                flagVerif = 1;
            }
        }
        if(root->nature == NODE_FOR || root->nature == NODE_DOWHILE)
        {
            if(root->opr[1]->type != TYPE_BOOL)
            {
                printf("Error line %d: false condition\n",root->lineno);
                flagVerif = 1;
                //Verif autres fils   
            }
        }
        if(root->nature == NODE_PLUS || root->nature == NODE_MINUS || root->nature == NODE_MUL || root->nature == NODE_DIV || root->nature == NODE_MOD)
        {
            if(!(root->opr[0]->type == TYPE_INT && root->opr[1]->type == TYPE_INT))
            {
                printf("Error line %d: operator not working on non-int variables\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_INT;
        }
        if(root->nature == NODE_LT || root->nature == NODE_GT || root->nature == NODE_LE || root->nature == NODE_GE)
        {
            if(root->opr[0]->type != TYPE_INT || root->opr[1]->type != TYPE_INT)
            {
                printf("Error line %d: operator not working on non-int variables\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_BOOL;
        }
        if(root->nature == NODE_EQ || root->nature == NODE_NE)
        {
            if((root->opr[0]->type!=root->opr[1]->type) || (root->opr[0]->type == TYPE_VOID) || (root->opr[0]->type == TYPE_NONE))
            {
                printf("Error line %d: false condition\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_BOOL;
        }
        if(root->nature == NODE_AND || root->nature == NODE_OR)
        {
            if(!(root->opr[0]->type == TYPE_BOOL && root->opr[1]->type == TYPE_BOOL))
            {
                printf("Error line %d: wrong type (operator requires boolean)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_BOOL;
        }
        if(root->nature == NODE_BAND || root->nature == NODE_BOR || root->nature == NODE_BXOR)
        {
            if(root->opr[0]->type != TYPE_INT || root->opr[1]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_INT;
        }
        if(root->nature == NODE_NOT)
        {
            if(root->opr[0]->type != TYPE_BOOL)
            {
                printf("Error line %d: wrong type (operator requires boolean)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_BOOL;
        }
        if(root->nature == NODE_BNOT)
        {
            if(root->opr[0]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_INT;
        }
        if(root->nature == NODE_SLL || root->nature == NODE_SRL || root->nature == NODE_SRA)
        {
            if((root->opr[0]->type != TYPE_INT) || (root->opr[1]->type != TYPE_INT))
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_INT;
        }
        if(root->nature == NODE_UMINUS)
        {
            if(root->opr[0]->type != TYPE_INT)
            {
                printf("Error line %d: wrong type (operator requires integer)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = TYPE_INT;
        }
        if(root->nature == NODE_AFFECT)
        {
            if(root->opr[0]->type != root->opr[1]->type)
            {
                printf("Error line %d: wrong type (operator requires two operands of the same type)\n",root->lineno);
                flagVerif = 1;
            }
            root->type = root->opr[0]->type;
        }
        if(root->nature == NODE_PRINT)
        {
            verifprint(root->opr[0]);
        }
    }
}

node_t make_node_global(node_nature nature, int64_t val, int32_t lineno)
{
    node_t new_node=malloc(sizeof(node_s));
    new_node->nops=0;
    new_node->nature=nature;
    new_node->type = TYPE_NONE;
    new_node->offset = 0;
    new_node->global_decl = false;
    new_node->opr = NULL;
    new_node->value=val; 
    new_node->ident = NULL;
    new_node->str = NULL;
    new_node->lineno=lineno;
    //new_node->node_num=nbNode++;
    return new_node;
}

void propaType(node_t root, node_type type)
{
    for(int i = 0; i < root->nops; i++)
    {
        if(root->opr[i]!=NULL)
        {
            propaType(root->opr[i],type);
        }
    }
    if((root->nature)==NODE_IDENT)
    {
        root->type=type;
    }
}

void verifprint(node_t root)
{
    if(root->nature==(NODE_LIST))
    {
        verifprint(root->opr[0]);
        verifprint(root->opr[1]);
    }
    else
    {
        if(root->nature!=NODE_IDENT && root->nature!=NODE_STRINGVAL)
        {
            printf("Error line %d: wrong type in print, only string or ident\n",root->lineno);
            flagVerif = 1;
        }
    }
}
