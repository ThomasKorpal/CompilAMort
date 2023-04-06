
#include <stdio.h>

#include "defs.h"
#include "passe_2.h"
#include "utils/miniccutils.h"

int numLabel = 1;

void addToData()
{
    int nb = get_global_strings_number();
    for(int i = 0; i < nb; i++)
    {
        inst_asciiz_create(NULL,get_global_string(i));
    }
}

//En gros on va faire l'allocateur de registres, qui sera appelé lors des créations
//d'instructions dans les 10000 if et l'analyse passe 2 sera appelée dans gen_code_passe_2

//Va peut etre falloir changer l'organisation des fonctions 


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

int registerAllocator()
{
    if(reg_available())
    {
        int reg = get_current_reg();
        allocate_reg();
    }
    else
    {

    }
}

void DFSp2(node_t root)
{
    int reg1, reg2;
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
            
        }
        if(root->nature == NODE_DECL)
        {   
            //ajout dans le .data
            if(root->opr[0]->type == TYPE_INT || root->opr[0]->type == TYPE_BOOL) 
            {
                inst_word_create(NULL,root->opr[0]->value);
            }
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
        if(root->nature == NODE_IF)
        {
            
        }
        if(root->nature ==  NODE_WHILE)
        {

        }
        if(root->nature == NODE_FOR) 
        {
            
        }
        if(root->nature == NODE_DOWHILE)
        {

        }
        if(root->nature == NODE_PLUS)
        {
            
        }
        if(root->nature == NODE_MINUS)
        {

        }
        if(root->nature == NODE_MUL)
        {
            
        }
        if(root->nature == NODE_DIV)
        {
            reg1 = registerAllocator();
            reg2 = registerAllocator();
            inst_div_create(reg1,reg2);
            release_reg();
        }
        if(root->nature == NODE_MOD)
        {

        }
        if(root->nature == NODE_LT)
        {
            
        }
        if(root->nature == NODE_GT)
        {

        }
        if(root->nature == NODE_LE)
        {

        }
        if(root->nature == NODE_GE)
        {

        }
        if(root->nature == NODE_EQ)
        {
            
        }
        if(root->nature == NODE_NE)
        {

        }
        if(root->nature == NODE_AND)
        {
            
        }
        if(root->nature == NODE_OR)
        {

        }
        if(root->nature == NODE_BAND)
        {
            
        }
        if(root->nature == NODE_BOR)
        {

        }
        if(root->nature == NODE_BXOR)
        {

        }
        if(root->nature == NODE_NOT)
        {
            
        }
        if(root->nature == NODE_BNOT)
        {
            
        }
        if(root->nature == NODE_SLL)
        {
            
        }
        if(root->nature == NODE_SRL)
        {

        }
        if(root->nature == NODE_SRA)
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