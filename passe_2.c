
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "passe_2.h"
#include "utils/miniccutils.h"
#include "arch.h"
#include "common.h"

int numLabel = 1;
int flag = 0;
bool prem = true;
int Warning = 0;
extern int trace_level;

void print();
void addToData();

void gen_code_passe_2(node_t root) 
{
    printf_level(3,"Creating .data section of the assembly program\n");
    inst_data_sec_create();
    printf_level(2,"Beginning code generation for every node\n");
    DFSp2(root);
}

void DFSp2(node_t root)
{
    if(root!=NULL)
    {   printf_level(4,"Generating code for NODE_%s \n",node_nature2string(root->nature));
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
            if(root->opr[1]!=NULL){
                if(root->global_decl){
                    inst_word_create(root->opr[0]->ident,root->opr[1]->value);
                    printf_level(5,"Adding global variable %s to .data section with value %ld\n",root->opr[0]->ident,root->opr[1]->value);
                }
                else{
                    DFSp2(root->opr[1]);
                    release_reg();
                    inst_sw_create(get_current_reg(), root->opr[0]->offset, get_stack_reg());
                }
            }
        }
        if(root->nature == NODE_IDENT)
        {
            if(reg_available()){
                if(root->decl_node->global_decl){
                    inst_lui_create(get_current_reg(),0x1001);
                    inst_lw_create(get_current_reg(), root->decl_node->offset,get_current_reg());
                    //inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_data_sec_start_addr());
                }
                else{
                    inst_lw_create(get_current_reg(),root->decl_node->offset, get_stack_reg());
                }
                allocate_reg();
            }
            else{
                flag++;
                release_reg();
                push_temporary(get_current_reg());
                if(root->decl_node->global_decl){
                    inst_lui_create(get_current_reg(),0x1001);
                    inst_lw_create(get_current_reg(), root->decl_node->offset,get_current_reg());
                    //inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_data_sec_start_addr());
                }
                else{
                    inst_lw_create(get_current_reg(),root->decl_node->offset, get_stack_reg());
                }
                allocate_reg();
            }
        }
        if((root->nature == NODE_INTVAL) || (root->nature == NODE_BOOLVAL) )
        {
            if(reg_available()){
                inst_ori_create(get_current_reg(),get_r0(),root->value);
                allocate_reg();
            }
            else{
                flag++;
                release_reg();
                push_temporary(get_current_reg());
                inst_ori_create(get_current_reg(),get_r0(),root->value);
                allocate_reg();
            }
        }
        if(root->nature == NODE_FUNC)
        {
            addToData();
            printf_level(3,"Setting up temporary stack\n");
            set_temporary_start_offset(root->offset);
            reset_temporary_max_offset();
            printf_level(3,"Creating .text section of the assembly program\n");
            inst_text_sec_create();
            printf_level(2,"main label added\n");
            inst_label_str_create("main");
            inst_stack_allocation_create();
            //printf("%d\n",get_current_reg());

            DFSp2(root->opr[2]);

            inst_stack_deallocation_create(root->offset + get_temporary_max_offset());
            printf_level(3,"Setting up exit syscall (end of program)\n");
            inst_ori_create(2, get_r0(), 10);
            inst_syscall_create();
        }
        if(root->nature == NODE_IF)
        {
            DFSp2(root->opr[0]);//parcours de la condition
            if(root->opr[0]->nature!=NODE_AFFECT){
                release_reg();
            }
            //si ca vaut 0 (false) jump label else
            int labelELSE = numLabel++;
            inst_beq_create(get_current_reg(), get_r0(), labelELSE);
            DFSp2(root->opr[1]);//parcours des instructions
            //jump label fin
            int labelFIN;
            if(root->nops==3){
                labelFIN = numLabel++;
                inst_j_create(labelFIN);
            }
            //label else
            inst_label_create(labelELSE);
            if(root->nops==3){
                DFSp2(root->opr[2]);
                //label fin
                inst_label_create(labelFIN);
            }
        }
        if(root->nature ==  NODE_WHILE)
        {
            //label while
            int labelWHILE = numLabel++;
            inst_label_create(labelWHILE);
            DFSp2(root->opr[0]);//parcours de la condition
            release_reg();
            //si ca vaut 0 (false) jump label fin
            int labelFIN = numLabel++;
            inst_beq_create(get_current_reg(), get_r0(), labelFIN);
            DFSp2(root->opr[1]);//parcours des instructions
            inst_j_create(labelWHILE);
            inst_label_create(labelFIN);
        }
        if(root->nature == NODE_FOR) 
        {
            if(root->opr[0]->nature == NODE_AFFECT){
                DFSp2(root->opr[0]);//parcours de l'initialisation
            }
            //label for
            int labelFOR = numLabel++;
            inst_label_create(labelFOR);
            DFSp2(root->opr[1]);//parcours de la condition
            release_reg();
            //si ca vaut 0 (false) jump label fin
            int labelFIN = numLabel++;
            inst_beq_create(get_current_reg(), get_r0(), labelFIN);
            DFSp2(root->opr[3]);//parcours du bloc
            if(root->opr[2]->nature == NODE_AFFECT){
                DFSp2(root->opr[2]);//parcours de l'instruction
            }
            inst_j_create(labelFOR);
            inst_label_create(labelFIN);
        }
        if(root->nature == NODE_DOWHILE)
        {
            //label while
            int labelWHILE = numLabel++;
            inst_label_create(labelWHILE);
            DFSp2(root->opr[0]);//parcours des instructions
            DFSp2(root->opr[1]);//parcours de la condition
            //si ca vaut 1 (true) jump label while
            release_reg();
            inst_bne_create(get_current_reg(), get_r0(), labelWHILE);
        }
        if(root->nature == NODE_PLUS)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                /*if(root->opr[i]->nature == NODE_INTVAL){
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
                else if(root->opr[i]->nature == NODE_IDENT){
                    if(reg_available()){
                        if(root->opr[i]->decl_node->global_decl){
                            printf("current reg : %d\noffset : %d", get_current_reg(),root->opr[i]->decl_node->offset);
                            inst_lui_create(get_current_reg(),0x1001);
                            inst_lw_create(get_current_reg(), root->opr[i]->decl_node->offset,get_current_reg());
                            //inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_data_sec_start_addr());
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
                            inst_lui_create(get_current_reg(),0x1001);
                            inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset,get_current_reg());
                            //inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_data_sec_start_addr());
                        }
                        else{
                            inst_lw_create(get_current_reg(),root->opr[i]->decl_node->offset, get_stack_reg());
                        }
                        allocate_reg();
                    }
                }
                else{
                    DFSp2(root->opr[i]);
                }*/
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_addu_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_MINUS)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_subu_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_MUL)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_mult_create(get_current_reg()-1, get_current_reg());
            inst_mflo_create(get_current_reg()-1);
        }
        if(root->nature == NODE_DIV)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_div_create(get_current_reg()-1, get_current_reg());
            inst_teq_create(get_current_reg(), get_r0());
            inst_mflo_create(get_current_reg()-1);
        }
        if(root->nature == NODE_MOD)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_div_create(get_current_reg()-1, get_current_reg());
            inst_teq_create(get_current_reg(), get_r0());
            inst_mfhi_create(get_current_reg()-1);
        }
        if(root->nature == NODE_LT)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_slt_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_GT)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_slt_create(get_current_reg()-1, get_current_reg(), get_current_reg()-1);
        }
        if(root->nature == NODE_LE)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_slt_create(get_current_reg()-1, get_current_reg(), get_current_reg()-1);
            inst_xori_create(get_current_reg()-1, get_current_reg()-1, 1);
        }
        if(root->nature == NODE_GE)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_slt_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
            inst_xori_create(get_current_reg()-1, get_current_reg()-1, 1);
        }
        if(root->nature == NODE_EQ)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_xor_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
            inst_sltiu_create(get_current_reg()-1, get_current_reg()-1, 1);
        }
        if(root->nature == NODE_NE)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_xor_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
            inst_sltu_create(get_current_reg()-1, get_r0(), get_current_reg()-1);
        }
        if(root->nature == NODE_AND || root->nature == NODE_BAND)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_and_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_OR || root->nature == NODE_BOR)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_or_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_BXOR)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_xor_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_NOT)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            inst_xori_create(get_current_reg()-1, get_current_reg()-1, 1);
        }
        if(root->nature == NODE_BNOT)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            inst_nor_create(get_current_reg()-1, get_r0(), get_current_reg()-1);
        }
        if(root->nature == NODE_SLL)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_sllv_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_SRL)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_srlv_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_SRA)
        {
            if(root->opr[0]->nature==NODE_AFFECT){
                Warning++;
            }
            for(int i=0; i<2; i++){
                DFSp2(root->opr[i]);
            }
            if(flag){
                flag--;
                pop_temporary(get_current_reg());
            }
            else if(root->opr[1]->nature!=NODE_AFFECT){
                release_reg();
            }
            inst_srav_create(get_current_reg()-1, get_current_reg()-1, get_current_reg());
        }
        if(root->nature == NODE_UMINUS)
        {
            DFSp2(root->opr[0]);
            inst_subu_create(get_current_reg()-1, get_r0(), get_current_reg()-1);
        }
        if(root->nature == NODE_AFFECT)
        {
            //printf("%d\n", root->opr[1]->nature+18);
            DFSp2(root->opr[1]);
            if(Warning){
                Warning--;
                if(root->opr[0]->decl_node!=NULL){
                    if(root->opr[0]->decl_node->global_decl){
                        inst_lui_create(4,0x1001);
                        inst_sw_create(get_current_reg()-1, root->opr[0]->decl_node->offset, 4);
                    }
                    else{
                        inst_sw_create(get_current_reg()-1, root->opr[0]->decl_node->offset, get_stack_reg());
                    }
                }
            }
            else{
                release_reg();
                if(root->opr[0]->decl_node!=NULL){
                    if(root->opr[0]->decl_node->global_decl){
                        inst_lui_create(4,0x1001);
                        inst_sw_create(get_current_reg(), root->opr[0]->decl_node->offset, 4);
                    }
                    else{
                        inst_sw_create(get_current_reg(), root->opr[0]->decl_node->offset, get_stack_reg());
                    }
                }
            }
        }
        if(root->nature == NODE_PRINT)
        {
            print(root->opr[0]);
        }
    }
}

void print(node_t root){
    if(root->nature == NODE_STRINGVAL){
        inst_lui_create(4,0x1001);
        inst_ori_create(4,4,root->offset);
        inst_ori_create(2,0,4);
        inst_syscall_create();
    }
    else if(root->nature == NODE_IDENT){
        if(root->decl_node->global_decl){
            //printf("%d\n",get_data_sec_start_addr());
            inst_lui_create(4,0x1001);
            inst_lw_create(4,root->decl_node->offset,4);
        }
        else{
            inst_lw_create(4,root->decl_node->offset,get_stack_reg());
        }
        inst_ori_create(2,0,1);
        inst_syscall_create();
    }
    else{
        print(root->opr[0]);
        print(root->opr[1]);
    }
}

void addToData()
{
    int nb = get_global_strings_number();
    printf_level(4,"Adding %d global strings to the .data section\n",nb);
    for(int i = 0; i < nb; i++)
    {
        inst_asciiz_create(NULL,get_global_string(i));
        printf_level(5,"%s added to the .data section\n",get_global_string(i));
    }
}
