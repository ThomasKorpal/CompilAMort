
#include <stdio.h>

#include "defs.h"
#include "passe_2.h"
#include "utils/miniccutils.h"

int numLabel = 1;

void DFSp2(node_t root);
void addToData();

void addToData()
{
    int nb = get_global_strings_number();
    for(int i = 0; i < nb; i++)
    {
        inst_asciiz_create(NULL,get_global_string(i));
    }
    
}

void gen_code_passe_2(node_t root) 
{
    /*if(reg_available())
    {
        int reg = get_current_reg();
        allocate_reg();
        gen_code_passe_2(root->opr[1]);
        create_inst();
    }*/
}

void analyse_passe2(node_t root)
{
    create_program();
    inst_data_sec_create();
    addToData();
    DFSp2(root);
}

void DFSp2(node_t root)
{
    if(root!=NULL)
    {    
        if(root->nature == NODE_PROGRAM)
        {
            inst_stack_allocation_create();
        }
        if(root->nature == NODE_BLOCK)
        {
            
        }
        if(root->nature == NODE_FUNC)
        {
            set_temporary_start_offset(root->offset);
            inst_text_sec_create();
        }
        if(root->nature == NODE_DECLS)
        {
            //ajout dans le .data 
        }
        if(root->nature == NODE_DECL)
        {   
            //ajout dans le .data
        }

        //Parcours en profondeur
        for (int i = 0; i<root->nops; i++)
        {
            DFSp2(root->opr[i]);
        }

        // traitement des verifications
        if(root->nature == NODE_BLOCK)
        {
            
        }    
        if(root->nature == NODE_DECLS)
        {
            
        }
        if(root->nature == NODE_DECL)
        {
            
        }
        if(root->nature == NODE_IDENT)
        {
            
        }
        if(root->nature == NODE_STRINGVAL)
        {
            //inst_asciiz_create(NULL,root->str);
        }
        if(root->nature == NODE_FUNC)
        {
            inst_stack_deallocation_create(root->offset + get_temporary_max_offset());
        }
        if(root->nature == NODE_IF || root->nature == NODE_WHILE)
        {
            
        }
        if(root->nature == NODE_FOR || root->nature == NODE_DOWHILE)
        {
            
        }
        if(root->nature == NODE_PLUS || root->nature == NODE_MINUS || root->nature == NODE_MUL || root->nature == NODE_DIV || root->nature == NODE_MOD)
        {
            
        }
        if(root->nature == NODE_LT || root->nature == NODE_GT || root->nature == NODE_LE || root->nature == NODE_GE)
        {
            
        }
        if(root->nature == NODE_EQ || root->nature == NODE_NE)
        {
            
        }
        if(root->nature == NODE_AND || root->nature == NODE_OR)
        {
            
        }
        if(root->nature == NODE_BAND || root->nature == NODE_BOR || root->nature == NODE_BXOR)
        {
            
        }
        if(root->nature == NODE_NOT)
        {
            
        }
        if(root->nature == NODE_BNOT)
        {
            
        }
        if(root->nature == NODE_SLL || root->nature == NODE_SRL || root->nature == NODE_SRA)
        {
            
        }
        if(root->nature == NODE_UMINUS)
        {
            
        }
        if(root->nature == NODE_AFFECT)
        {
            
        }
        if(root->nature == NODE_PRINT)
        {
            
        }
    }
}