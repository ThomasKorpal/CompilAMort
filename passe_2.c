
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
    if(root!=NULL)
    {    
        if(root->nature == NODE_PROGRAM)
        {
            DFSp2(root->opr[0]);
            DFSp2(root->opr[1]);
        }
        if(root->nature == NODE_BLOCK)
        {
            DFSp2(root->opr[0]);
            DFSp2(root->opr[1]);
        }
        if(root->nature == NODE_LIST)
        {
            DFSp2(root->opr[0]);
            DFSp2(root->opr[1]);
        }
        if(root->nature == NODE_DECLS)
        {
            DFSp2(root->opr[0]);
            DFSp2(root->opr[1]);
        }
        if(root->nature == NODE_DECL)
        {   
            if(root->global_decl)//ajout dans le .data
            {
                //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!opérations!!!!!!!!!!
                inst_word_create(root->opr[0]->ident,root->opr[1]->value);
            }
            else
            {
                inst_ori_create(get_current_reg(),get_r0(),root->opr[1]->value);
                inst_sw_create(get_current_reg(), root->opr[0]->offset, get_stack_reg());
            }
        }
        if(root->nature == NODE_IDENT)
        {
            
        }
        if(root->nature == NODE_TYPE)
        {
            
        }
        if(root->nature == NODE_INTVAL)
        {
            
        }
        if(root->nature == NODE_BOOLVAL)
        {
            
        }
        if(root->nature == NODE_STRINGVAL)
        {
            //inst_asciiz_create(NULL,root->str);
        }
        if(root->nature == NODE_FUNC)
        {
            set_temporary_start_offset(root->offset);
            reset_temporary_max_offset();
            inst_text_sec_create();
            inst_label_str_create("main");
            inst_stack_allocation_create();
            //printf("%d\n",get_current_reg());

            DFSp2(root->opr[2]);

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
        if(root->nature == NODE_PLUS){
            printf("Node+\n");
            for(int i=0; i<2; i++){
                if(root->opr[i]->nature == NODE_INTVAL){
                    if(reg_available()){
                        inst_ori_create(get_current_reg(),get_r0(),root->opr[i]->value);
                        allocate_reg();
                    }
                    else{
                        flag++;
                        release_reg();
                        push_temporary(get_current_reg());
                        inst_ori_create(get_current_reg(),get_r0(),root->opr[i]->value);
                        allocate_reg();
                    }
                }
                if(root->opr[i]->nature == NODE_IDENT){
                    if(reg_available()){
                        if(root->opr[i]->decl_node->global_decl){
                            inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_data_sec_start_addr());
                        }
                        else{
                            inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_stack_reg());
                        }
                        allocate_reg();
                    }
                    else{
                        flag++;
                        release_reg();
                        push_temporary(get_current_reg());
                        if(root->opr[i]->decl_node->global_decl){
                            inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_data_sec_start_addr());
                        }
                        else{
                            inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_stack_reg());
                        }
                        allocate_reg();
                    }
                }
                else{
                    DFSp2(root->opr[i]);
                }
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else{
                release_reg();
            }
            inst_addu_create(get_current_reg()-1, get_current_reg(), get_current_reg()-1);
            printf("%d\n",flag);
        }
        if(root->nature == NODE_MINUS)
        {
                  
        }
        if(root->nature == NODE_MUL)
        {
            
        }
        if(root->nature == NODE_DIV)
        {
            
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
        if(root->nature == NODE_AFFECT){
            DFSp2(root->opr[1]);
            release_reg();
            if(root->opr[0]->decl_node!=NULL){
                if(root->opr[0]->decl_node->global_decl){
                    inst_sw_create(get_current_reg(), root->opr[0]->decl_node->offset, get_data_sec_start_addr());
                }
                else{
                    inst_sw_create(get_current_reg(), root->opr[0]->decl_node->offset, get_stack_reg());
                }
            }
            if(get_current_reg()!=8)
                release_reg(); 
        }
        if(root->nature == NODE_PRINT)
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
            if(root->opr[0]->type == TYPE_INT || root->opr[0]->type == TYPE_BOOL) 
            {
                if(root->global_decl)
                {
                    inst_word_create(NULL,root->opr[0]->value);
                }
            }
            break;
        case NODE_IDENT:
        case NODE_TYPE:
        case NODE_INTVAL:
            inst_addiu_create(reg_dest,reg_src1,root->value);
            break;
        case NODE_BOOLVAL:
        case NODE_STRINGVAL:
        case NODE_FUNC:
            int firstTimeFunc=1;
            if(firstTimeFunc)
            {
                set_temporary_start_offset(root->offset);
                reset_temporary_max_offset();
                inst_text_sec_create();
                inst_label_str_create("main");
                inst_stack_allocation_create();
                firstTimeFunc = 0;
            }
            else
            {
                inst_stack_deallocation_create(root->offset + get_temporary_max_offset());
            }
            break;
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
            inst_mult_create(reg_src1,reg_src2);
            inst_mflo_create(reg_src1);
            break;
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
            inst_or_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_BXOR:
            inst_xor_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_NOT:
            if(root->opr[0]->type == TYPE_BOOL)
            {
                inst_xori_create(reg_dest,reg_src1,0x00000001);
            }
            else
            {
                printf("Error line %d : the not operator only work on boolean\n",root->lineno);
                exit(EXIT_FAILURE);
            }
            break;
        case NODE_BNOT:
            inst_xori_create(reg_dest,reg_src1,0xFFFFFFFF);
            break;
        case NODE_SLL:
            inst_sllv_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_SRA:
            inst_srav_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_SRL:
            inst_srlv_create(reg_dest,reg_src1,reg_src2);
            break;
        case NODE_UMINUS:
            if(root->opr[0]->type == TYPE_INT)
            {
                inst_xori_create(reg_dest,reg_src1,0x80000000);
            }
            else
            {
                printf("Error line %d : the not operator only work on integers\n",root->lineno);
                exit(EXIT_FAILURE);
            }
            break;
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
            break;
        default:
    }
}