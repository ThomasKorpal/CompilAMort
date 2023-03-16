%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "defs.h"
#include "common.h"
#include "miniccutils.h"
#include "passe_1.h"
#include "passe_2.h"



/* Global variables */
extern bool stop_after_syntax;
extern bool stop_after_verif;
extern char * outfile;
int32_t nbNode = 1;


/* prototypes */
int yylex(void);
extern int yylineno;

void yyerror(node_t * program_root, char * s);
void analyse_tree(node_t root);

node_t make_node(node_nature nature, int nops, node_t enf1, node_t enf2);
node_t make_node_main_if(node_nature nature, int nops, node_t enf1, node_t enf2, node_t enf3);
node_t make_node_for(node_nature nature, int nops, node_t enf1, node_t enf2, node_t enf3, node_t enf4);
node_t make_node_type(node_nature nature, node_type type);
node_t make_node_ident(node_nature nature, char* ident);
node_t make_node_string(node_nature nature, char* str);
node_t make_node_bool_int(node_nature nature, int64_t val);
/* A completer */

%}

%parse-param { node_t * program_root }

%union {
    int32_t intval;
    char * strval;
    node_t ptr;
};


/* Definir les token ici avec leur associativite, dans le bon ordre */
%token TOK_VOID TOK_INT TOK_BOOL TOK_TRUE TOK_FALSE 
%token TOK_IF TOK_ELSE TOK_WHILE TOK_FOR TOK_PRINT
%token TOK_SEMICOL TOK_COMMA TOK_LPAR TOK_RPAR TOK_LACC TOK_RACC
%token TOK_DO
/* A completer */

%nonassoc TOK_THEN
%nonassoc TOK_ELSE

%right TOK_AFFECT

%left TOK_OR
%left TOK_AND
%left TOK_BOR
%left TOK_BXOR 
%left TOK_BAND 
%left TOK_EQ TOK_NE
%nonassoc TOK_GT TOK_LT TOK_GE TOK_LE
%nonassoc TOK_SRL TOK_SRA TOK_SLL

%left TOK_PLUS TOK_MINUS
%left TOK_MUL TOK_DIV TOK_MOD

%nonassoc TOK_UMINUS TOK_NOT TOK_BNOT

%token <intval> TOK_INTVAL;
%token <strval> TOK_IDENT TOK_STRING;

%type <ptr> program listdecl listdeclnonnull vardecl ident type listtypedecl decl maindecl listinst listinstnonnull inst block expr listparamprint paramprint

%%

/* Completer les regles et la creation de l'arbre */
program:
        listdeclnonnull maindecl
        {
            $$ = make_node(NODE_PROGRAM, 2, $1, $2);
            *program_root = $$;
        }
        | maindecl
        {
            $$ = make_node(NODE_PROGRAM, 2, NULL, $1);
            *program_root = $$;
        }
        ;

listdecl:
        listdeclnonnull
        {
            $$ = $1;
        }
        |
        {
            $$ = NULL;
        }
        ;

listdeclnonnull:
        vardecl
        { 
            $$ = $1;
        }
        | listdeclnonnull vardecl
        { 
            $$ = make_node(NODE_LIST, 2, $1, $2);
        }
        ;

vardecl:
        type listtypedecl TOK_SEMICOL
        {
            $$ = make_node(NODE_DECLS, 2, $1, $2);
        }
        ;

type:
        TOK_INT
        {
            $$ = make_node_type(NODE_TYPE, TYPE_INT);
        }
        | TOK_BOOL
        {
            $$ = make_node_type(NODE_TYPE, TYPE_BOOL);
        }
        | TOK_VOID
        {
            $$ = make_node_type(NODE_TYPE, TYPE_VOID);
        }
        ;

listtypedecl:
        decl
        { 
            $$ = $1;
        }
        | listtypedecl TOK_COMMA decl
        { 
            $$ = make_node(NODE_LIST, 2, $1, $3);
        }
        ;

decl:
        ident
        { 
            $$ = make_node(NODE_DECL, 2, $1, NULL);
        }
        | ident TOK_AFFECT expr
        { 
            $$ = make_node(NODE_DECL, 2, $1, $3);
        }
        ;

maindecl:
        type ident TOK_LPAR TOK_RPAR block
        {
            $$ = make_node_main_if(NODE_FUNC, 3, $1, $2, $5);
        }
        ;

listinst:
        listinstnonnull
        {
            $$ = $1;
        }
        |
        {
            $$ = NULL;
        }
        ;

listinstnonnull:
        inst
        {
            $$ = $1;
        }
        | listinstnonnull inst
        {
            $$ = make_node(NODE_LIST, 2, $1, $2);
        }
        ;

inst:
        expr TOK_SEMICOL
        {
            $$ = $1;
        }
        | TOK_IF TOK_LPAR expr TOK_RPAR inst TOK_ELSE inst
        {
            $$ = make_node_main_if(NODE_IF, 3, $3, $5, $7);
        }
        | TOK_IF TOK_LPAR expr TOK_RPAR inst %prec TOK_THEN
        {
            $$ = make_node_main_if(NODE_IF, 2, $3, $5, NULL);                //?????????????????????????????????????????????
        }
        | TOK_WHILE TOK_LPAR expr TOK_RPAR inst
        {
            $$ = make_node(NODE_WHILE, 2, $3, $5);
        }
        | TOK_FOR TOK_LPAR expr TOK_SEMICOL expr TOK_SEMICOL expr TOK_RPAR inst
        {
            $$ = make_node_for(NODE_FOR, 4, $3, $5, $7, $9);
        }
        | TOK_DO inst TOK_WHILE TOK_LPAR expr TOK_RPAR TOK_SEMICOL
        {
            $$ = make_node(NODE_DOWHILE, 2, $2, $5);
        }
        | block
        {
            $$ = $1;
        }
        | TOK_SEMICOL
        {
            $$ = NULL;                                                        //????????????????????????????????????????????????
        }
        | TOK_PRINT TOK_LPAR listparamprint TOK_RPAR TOK_SEMICOL
        {
            $$ = make_node(NODE_PRINT, 1, $3, NULL);
        }
        ;

block:
        TOK_LACC listdecl listinst TOK_RACC
        {
            $$ = make_node(NODE_BLOCK, 2, $2, $3);
        }
        ;

expr:
        expr TOK_MUL expr
        {
            $$ = make_node(NODE_MUL, 2, $1, $3);
        }
        | expr TOK_DIV expr
        {
            $$ = make_node(NODE_DIV, 2, $1, $3);
        }
        | expr TOK_PLUS expr
        {
            $$ = make_node(NODE_PLUS, 2, $1, $3);
        }
        | expr TOK_MINUS expr
        {
            $$ = make_node(NODE_MINUS, 2, $1, $3);
        }
        | expr TOK_MOD expr
        {
            $$ = make_node(NODE_MOD, 2, $1, $3);
        }
        | expr TOK_LT expr
        {
            $$ = make_node(NODE_LT, 2, $1, $3);
        }
        | expr TOK_GT expr
        {
            $$ = make_node(NODE_GT, 2, $1, $3);
        }
        | TOK_MINUS expr %prec TOK_UMINUS
        {
            $$ = make_node(NODE_UMINUS, 1, $2, NULL);
        }
        | expr TOK_GE expr
        {
            $$ = make_node(NODE_GE, 2, $1, $3);
        }
        | expr TOK_LE expr
        {
            $$ = make_node(NODE_LE, 2, $1, $3);
        }
        | expr TOK_EQ expr
        {
            $$ = make_node(NODE_EQ, 2, $1, $3);
        }
        | expr TOK_NE expr
        {
            $$ = make_node(NODE_NE, 2, $1, $3);
        }
        | expr TOK_AND expr
        {
            $$ = make_node(NODE_AND, 2, $1, $3);
        }
        | expr TOK_OR expr
        {
            $$ = make_node(NODE_OR, 2, $1, $3);
        }
        | expr TOK_BAND expr
        {
            $$ = make_node(NODE_BAND, 2, $1, $3);
        }
        | expr TOK_BOR expr
        {
            $$ = make_node(NODE_BOR, 2, $1, $3);
        }
        | expr TOK_BXOR expr
        {
            $$ = make_node(NODE_BXOR, 2, $1, $3);
        }
        | expr TOK_SRL expr
        {
            $$ = make_node(NODE_SRL, 2, $1, $3);
        }
        | expr TOK_SRA expr
        {
            $$ = make_node(NODE_SRA, 2, $1, $3);
        }
        | expr TOK_SLL expr
        {
            $$ = make_node(NODE_SLL, 2, $1, $3);
        }
        | TOK_NOT expr
        {
            $$ = make_node(NODE_NOT, 1, $2, NULL);
        }
        | TOK_BNOT expr
        {
            $$ = make_node(NODE_BNOT, 1, $2, NULL);
        }
        | TOK_LPAR expr TOK_RPAR
        {
            $$ = $2;                                                        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        | ident TOK_AFFECT expr
        {
            $$ = make_node(NODE_AFFECT, 2, $1, $3);
        }
        | TOK_INTVAL
        {
            $$ = make_node_bool_int(NODE_INTVAL, $1);
        }
        | TOK_TRUE
        {
            $$ = make_node_bool_int(NODE_BOOLVAL, 1);
        }
        | TOK_FALSE
        {
            $$ = make_node_bool_int(NODE_BOOLVAL, 0);
        }
        | ident
        {
            $$ = $1;
        }
        ;

listparamprint:
        listparamprint TOK_COMMA paramprint
        {
            $$ = make_node(NODE_LIST, 2, $1, $3);
        }
        | paramprint
        {
            $$ = $1;
        }
        ;        

paramprint:
        ident
        {
            $$ = $1;    
        }
        | TOK_STRING
        {
            $$ = make_node_string(NODE_STRINGVAL, $1);
        }
        ;

ident:
        TOK_IDENT
        {
            $$ = make_node_ident(NODE_IDENT, $1);
        }
        ;
%%

/* A completer et/ou remplacer avec d'autres fonctions */
node_t make_node(node_nature nature, int nops, node_t enf1, node_t enf2)
{
    va_list ap;
    node_t new_node = malloc(sizeof(node_s));                         //node_s ?????????????????????????????????
    new_node->nature = nature;
    new_node->nops=nops;
    new_node->node_num=nbNode++;
    new_node->lineno=yylineno;
    new_node->opr=malloc(nops*sizeof(node_t));
    new_node->opr[0]=enf1;
    if(nops > 1)
    {
        new_node->opr[1]=enf2;
    }
    return new_node;
}

node_t make_node_main_if(node_nature nature, int nops, node_t enf1, node_t enf2, node_t enf3)
{
    va_list ap;
    node_t new_node=malloc(sizeof(node_s));                         //node_s ?????????????????????????????????
    new_node->nature=nature;
    new_node->nops=nops;
    new_node->node_num=nbNode++;
    new_node->lineno=yylineno;
    new_node->opr=malloc(nops*sizeof(node_t));
    new_node->opr[0]=enf1;
    new_node->opr[1]=enf2;
    if(nops > 2)
    {
        new_node->opr[2]=enf3;
    }
    return new_node;
}

node_t make_node_for(node_nature nature, int nops, node_t enf1, node_t enf2, node_t enf3, node_t enf4)
{
    va_list ap;
    node_t new_node=malloc(sizeof(node_s));
    new_node->nature=nature;
    new_node->nops=nops;
    new_node->node_num=nbNode++;
    new_node->lineno=yylineno;
    new_node->opr=malloc(nops*sizeof(node_t));
    new_node->opr[0]=enf1;
    new_node->opr[1]=enf2;
    new_node->opr[2]=enf3;
    new_node->opr[3]=enf4;
    return new_node;
}

node_t make_node_type(node_nature nature, node_type type)
{
    va_list ap;
    node_t new_node=malloc(sizeof(node_s));
    new_node->nature=nature;
    new_node->type=type;
    new_node->lineno=yylineno;
    new_node->node_num=nbNode++;
    return new_node;
}

node_t make_node_ident(node_nature nature, char* ident)
{
    va_list ap;
    node_t new_node=malloc(sizeof(node_s));
    new_node->nature=nature;
    new_node->ident=ident;
    new_node->lineno=yylineno;
    new_node->node_num=nbNode++;
    new_node->offset = -1;
    return new_node;
}

node_t make_node_string(node_nature nature, char* str)
{
    va_list ap;
    node_t new_node=malloc(sizeof(node_s));
    new_node->nature=nature;
    new_node->str=str;
    new_node->lineno=yylineno;
    new_node->node_num=nbNode++;
    return new_node;
}

node_t make_node_bool_int(node_nature nature, int64_t val)
{
    va_list ap;
    node_t new_node=malloc(sizeof(node_s));
    new_node->nature=nature;
    new_node->value=val;
    new_node->lineno=yylineno;
    new_node->node_num=nbNode++;
    return new_node;
}

void analyse_tree(node_t root)
{
    dump_tree(root, "apres_syntaxe.dot");
    /*if (!stop_after_syntax) 
    {
        analyse_passe_1(root);
        //dump_tree(root, "apres_passe_1.dot");
        if (!stop_after_verif) 
        {
            create_program(); 
            gen_code_passe_2(root);
            dump_mips_program(outfile);
            free_program();
        }
        free_global_strings();
    }*/
    free_nodes(root);
}



/* Cette fonction est appelee automatiquement si une erreur de syntaxe est rencontree
 * N'appelez pas cette fonction vous-meme :
 * la valeur donnee par yylineno ne sera plus correcte apres l'analyse syntaxique
 */
void yyerror(node_t * program_root, char * s) {
    fprintf(stderr, "Error line %d: %s\n", yylineno, s);
    exit(1);
}

