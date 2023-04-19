
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "passe_2.h"
#include "utils/miniccutils.h"
#include "arch.h"

int numLabel = 1;
int flag = 0;
bool prem = true;

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
    inst_data_sec_create();
    addToData();
    DFSp2(root);
}

/*int registerAllocator()
{
    if(reg_available())
    {
        int reg = get_current_reg();
        allocate_reg();
        //printf("-l Allocating register %d\n",get_current_reg());
        return reg;
    }
    else
    {
        int reg = get_current_reg();
        flag = true;
        push_temporary(reg);
        return reg-1;                                               //???????????????????????????????????????????????????????????
    }
}*/

void DFSp2(node_t root)
{
    int reg1, reg2;
    if(root!=NULL)
    {    
        if(root->nature == NODE_PROGRAM)
        {
            
        }
        if(root->nature == NODE_BLOCK)
        {
            
        }
        if(root->nature == NODE_FUNC)
        {
            set_temporary_start_offset(root->offset);
            reset_temporary_max_offset();
            inst_text_sec_create();
            inst_label_str_create("main");
            inst_stack_allocation_create();
            //printf("avDFS reg %d\n",get_current_reg());
        }
        if(root->nature == NODE_DECLS)
        {
            
        }
        if(root->nature == NODE_DECL)
        {   
            //ajout dans le .data
            if(root->opr[0]->type == TYPE_INT || root->opr[0]->type == TYPE_BOOL) 
            {
                if(root->global_decl)
                {
                    inst_word_create(NULL,root->opr[0]->value);
                }
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
            int reg1 = get_current_reg();
            inst_addu_create(reg1-1,reg1-1,reg1);
            if(flag){
                pop_temporary(11);
            }
            else{
                //release_reg();
            }
        }
        if(root->nature == NODE_MINUS)
        {
                  
        }
        if(root->nature == NODE_MUL)
        {
            
        }
        if(root->nature == NODE_DIV)
        {
            DFSp2(root->opr[0]);
            //reg1 = registerAllocator();
            DFSp2(root->opr[1]);
            inst_div_create(reg1,get_current_reg());
            release_reg();
        }
        if(root->nature == NODE_MOD)
        {
            DFSp2(root->opr[0]);
            //reg1 = registerAllocator();
            DFSp2(root->opr[1]);
            
            release_reg();
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
        if(root->nature == NODE_INTVAL)
        {
            if(reg_available())
            {
                int reg1=get_current_reg();
                if(prem){
                    prem = false;
                }
                else{
                    allocate_reg();
                }
                inst_addiu_create(reg1,0,root->value);
            }
            else
            {
                int reg1 = get_current_reg();
                printf("ici %d\n", reg1);
                flag++;
                push_temporary(reg1);
                inst_addiu_create(reg1,0,root->value);
            }
            
        }
        if(root->nature == NODE_BOOLVAL)
        {
            
        }
    }
}

void create_inst(node_t root, int reg_dest, int reg_src1, int reg_src2, int label)
{
    switch(root->nature)
    {
        case NONE:
        case NODE_PROGRAM:
        case NODE_BLOCK:
        case NODE_LIST:
        case NODE_DECLS:
        case NODE_DECL:
        case NODE_IDENT:
        case NODE_TYPE:
        case NODE_INTVAL:
            inst_addiu_create(reg_dest,reg_src1,root->value);
            break;
        case NODE_BOOLVAL:
        case NODE_STRINGVAL:
        case NODE_FUNC:
        case NODE_IF:
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_DOWHILE:
        case NODE_PLUS:
            inst_addu_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_MINUS:
            inst_subu_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_MUL:
        case NODE_DIV:
            inst_div_create(reg_src1,reg_src2);
            break;
        case NODE_MOD:
            inst_div_create(reg_src1,reg_src2);
            inst_mfhi_create(reg_src1);
            break;
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_NOT:
        case NODE_BNOT:
        case NODE_SLL:
        case NODE_SRA:
        case NODE_SRL:
        case NODE_UMINUS:
        case NODE_AFFECT:
        case NODE_PRINT:
            if(root->opr[0]->nature != NODE_IDENT || root->opr[0]->nature != NODE_STRINGVAL)
            {
                
            }
            else
            {
                switch(root->opr[0]->nature)
                {
                    case NODE_STRINGVAL:
                        inst_lui_create(4,get_data_sec_start_addr());
                        inst_ori_create(4,4,root->opr[0]->offset);
                        inst_ori_create(2,0,4);
                        inst_syscall_create();
                        break;
                    case NODE_IDENT:
                        inst_lw_create(4,root->opr[0]->offset,29);
                        inst_ori_create(2,0,1);
                        inst_syscall_create();
                        break;
                    case NODE_INTVAL:
                        printf("Error line %d : Wrong type given (int) in print function\n",root->lineno);
                        exit(EXIT_FAILURE);
                    default:
                        printf("Error line %d : Wrong type given in print function\n",root->lineno);
                        exit(EXIT_FAILURE);
                }
            }
        default:
    }
}