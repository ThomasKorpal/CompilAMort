#ifndef _MINICCUTILS_H_
#define _MINICCUTILS_H_

#include <stdint.h>
#include <stdbool.h>

#include "../defs.h"



/* For checking the tree constructed */

bool check_program_tree(node_t n);


/* Context related functions */

typedef struct _context_s context_s;
typedef context_s * context_t;

context_t create_context();
void free_context(context_t context);
bool context_add_element(context_t context, char * idf, void * data);
void * get_data(context_t context, char * idf);


/* Environment related functions */

void push_global_context();
void push_context();
void pop_context();
void * get_decl_node(char * ident);
int32_t env_add_element(char * ident, void * node);
void reset_env_current_offset();
int32_t get_env_current_offset();
int32_t add_string(char * str);
int32_t get_global_strings_number();
char * get_global_string(int32_t index);
void free_global_strings();


/* Register allocation related functions */

void push_temporary(int32_t reg);
void pop_temporary(int32_t reg);
bool reg_available();
int32_t get_current_reg();
int32_t get_restore_reg();
void allocate_reg();
void release_reg();
int32_t get_new_label();
void set_temporary_start_offset(int32_t offset);
void set_max_registers(int32_t n);
void reset_temporary_max_offset();
int32_t get_temporary_max_offset();
int32_t get_temporary_curr_offset(); // for debug


/* Program creation related functions */

void inst_data_sec_create();
void inst_text_sec_create();
void inst_word_create(char * label, int32_t init_value);
void inst_asciiz_create(char * label_str, char * str);
void inst_label_create(int32_t label);
void inst_label_str_create(char * label);
void inst_comment_create(char * comment);
void inst_lui_create(int32_t r_dest, int32_t imm);
void inst_addu_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_subu_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_slt_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_sltu_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_and_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_or_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_xor_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_nor_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_mult_create(int32_t r_src_1, int32_t r_src_2);
void inst_div_create(int32_t r_src_1, int32_t r_src_2);
void inst_sllv_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_srav_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_srlv_create(int32_t r_dest, int32_t r_src_1, int32_t r_src_2);
void inst_addiu_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void inst_andi_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void inst_ori_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void inst_xori_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void inst_slti_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void inst_sltiu_create(int32_t r_dest, int32_t r_src_1, int32_t imm);
void inst_lw_create(int32_t r_dest, int32_t imm, int32_t r_src_1);
void inst_sw_create(int32_t r_src_1, int32_t imm, int32_t r_src_2);
void inst_beq_create(int32_t r_src_1, int32_t r_src_2, int32_t label);
void inst_bne_create(int32_t r_src_1, int32_t r_src_2, int32_t label);
void inst_mflo_create(int32_t r_dest);
void inst_mfhi_create(int32_t r_dest);
void inst_j_create(int32_t label);
void inst_teq_create(int32_t r_src_1, int32_t r_src_2);
void inst_syscall_create();
void inst_stack_allocation_create();
void inst_stack_deallocation_create(int32_t val);

void create_program();
void free_program();
void dump_mips_program(char * filename);




#endif

