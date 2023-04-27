
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "common.h"
#include "arch.h"

extern char * infile;
extern char * outfile;
int32_t trace_level = DEFAULT_TRACE_LEVEL;
extern bool stop_after_syntax;
extern bool stop_after_verif;
extern int flagb;
int flagVerif = 0;

void printf_lvl(int level, char* to_print)
{
    if(level <= trace_level)
    {
        printf("%s",to_print);
    }
}

void fctUsage()
{
    printf("Usage : ./minicc [OPTIONS]\n\n");
    printf("Options list :\n");
    printf("\t<filename.c> : \t\tdefines the .c file to compile\n");
    printf("\t-b : \t\t\tprints the project banner on the terminal, including the compiler's name, version and authors' names\n");
    printf("\t-o <filename.s> : \tdefines the outfile's name (default : out.s)\n");
    printf("\t-t <int> : \t\tdefines the trace level used by the compiler (between 0 and 5) (0 = no trace printed, 5 = all traces printed, default : 0)\n");
    printf("\t-r <int> : \t\tdefines the maximum number of registers used in the assembly code (between 4 and 8) (default : 8)\n");
    printf("\t-s : \t\t\tstops compilation after syntax verification (default : no)\n");
    printf("\t-v : \t\t\tstops compilation after verification phase (default : no)\n");
    printf("\t-h : \t\t\tprints the options list\n\n");
}

void analyseArgs(int argc, char ** argv)
{
    int i = 1, cpt = 1;
    char* fichierEntree = "";
    /*printf("%d\n",argc);
    for(int j = 0; j < argc; j++)
    {   
        printf("%d %s\n",j,argv[j]);
    }*/
    if(argc > 1) //si on passe des arguments
    {
        if(argc == 2)
        {
            printf_level(4,"One argument detected\n");
            if(!strcmp(argv[1],"-b"))
            {
                printf("\n*****************************************************************************************\n");
                printf("*****************************************************************************************\n");
                printf("*****                               Compilateur minicc                              *****\n");
                printf("*****                                 version KiMarch                               *****\n");
                printf("*****                                                                               *****\n");
                printf("*****                                                                               *****\n");
                printf("*****                         par Thomas Korpal & Hugo Vouaux                       *****\n");
                printf("*****                            EISE 4 - Polytech Sorbonne                         *****\n");
                printf("*****************************************************************************************\n");
                printf("*****************************************************************************************\n");
                printf("\nPS: Une bonne note svp ? :)\n");
                flagb = 1;
            }
            else if(!strcmp(argv[1],"-h"))
            {
                fctUsage();
                //flagb = 1;
                exit(EXIT_FAILURE);
            }
            else
            {
                if(strlen(argv[1])>2)
                {
                    if(argv[1][strlen(argv[1])-1]=='c' && argv[1][strlen(argv[1])-2]=='.')
                    {
                        printf_level(5,"Giving infile the name of the file\n");
                        fichierEntree = argv[1];
                    }
                    else
                    {
                        printf("File given isn't a C file\n");
                        //flagb = 1;
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    printf("Argument given is not a file\n");
                    fctUsage();
                    //flagb = 1;
                    exit(EXIT_FAILURE);
                }
            }
        }
        else
        {
            for(int i = 1; i > argc; i++)
            {
                if(!strcmp(argv[i],"-h"))
                {
                    fctUsage();
                    exit(EXIT_SUCCESS);
                }
                if(!strcmp(argv[i],"-t"))
                {
                    if(i+1 < argc)
                    {
                        if(atoi(argv[i+1]) >= 0 && atoi(argv[i+1]) <= 5)
                        {
                            trace_level = atoi(argv[i+1]);
                        }
                    }
                }
            }
            printf_level(4,"Multiple arguments detected\n");
            while(i < argc)
            {
                if(!strcmp(argv[i],"-o"))
                {
                    printf_level(4,"-o argument detected\n");
                    if(i+1 < argc)
                    {
                        if(strlen(argv[i+1]) > 2)
                        {
                            if(argv[i+1][strlen(argv[i+1])-1]=='s' && argv[i+1][strlen(argv[i+1])-2]=='.')
                            {
                                outfile = argv[i+1];
                                if(trace_level != 0)
                                {
                                    printf_level(5,"Giving the variable outfile the outfile's name\n");
                                }
                            }
                            else
                            {
                                printf("File given isn't a .s file\n");
                                //flagb = 1;
                                //break;
                                exit(EXIT_FAILURE);
                            }
                        }
                        else
                        {
                            printf("Argument given is not a file\n");
                            fctUsage();
                            //flagb = 1;
                            //break;
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        printf("Option -o needs an argument\n");
                        //flagb = 1;
                        //break;
                        exit(EXIT_FAILURE);
                    }
                    i += 2;
                    continue;
                }
                if(!strcmp(argv[i],"-t"))
                {
                    printf_level(4,"-t argument detected\n");
                    if(i+1 < argc)
                    {
                        if(atoi(argv[i+1]) >= 0 && atoi(argv[i+1]) <= 5)
                        {
                            trace_level = atoi(argv[i+1]);
                            printf_level(5,"Updating trace level to %d\n",atoi(argv[i+1]));
                        }
                        else
                        {
                            printf("Argument given for -t out of range (between 0 and 5 included)\n");
                            //flagb = 1;
                            //break;
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        printf("Option -t needs an argument\n");
                        //flagb = 1;
                        //break;
                        exit(EXIT_FAILURE);
                    }
                    i+=2;
                    continue;
                }
                if(!strcmp(argv[i],"-r"))
                {
                    printf_level(4,"-r argument detected\n");
                    if(i+1 < argc)
                    {
                        if(atoi(argv[i+1]) >= 4 && atoi(argv[i+1]) <= 7)
                        {
                            set_max_registers(atoi(argv[i+1])+1);
                            printf_level(5,"Updating maximum registers to %d\n",atoi(argv[i+1]));
                        }
                        else if (atoi(argv[i+1]) == 8)
                        {
                            set_max_registers(atoi(argv[i+1]));
                            printf_level(5,"Updating maximum registers to %d\n",atoi(argv[i+1]));
                        }
                        else
                        {
                            printf("Argument given for -r out of range (between 4 and 8 included)\n");
                            //flagb = 1;
                            //break;
                            exit(EXIT_FAILURE);
                        }   
                    }
                    else
                    {
                        printf("Option -r needs an argument\n");
                        //flagb = 1;
                        //break;
                        exit(EXIT_FAILURE);
                    }
                    i+=2;
                    continue;
                }
                if(!strcmp(argv[i],"-s"))
                {
                    printf_level(4,"-s argument detected\n");
                    if(!stop_after_verif)
                    {
                        stop_after_syntax = true;
                        printf_level(5,"Parameter stop_after_syntax updated to true\n");
                    }
                    else
                    {
                        //erreur
                        printf("Options -s and -v are incompatible\n");
                        //flagb = 1;
                        //break;
                        exit(EXIT_FAILURE);
                    }
                    i++;
                    continue;
                }
                if(!strcmp(argv[i],"-v"))
                {
                    printf_level(4,"-v argument detected\n");
                    if(!stop_after_syntax)
                    {
                        stop_after_verif = true;
                        printf_level(5,"Parameter stop_after_verif updated to true\n");
                    }
                    else
                    {
                        //erreur
                        printf("Options -s and -v are incompatible\n");
                        //flagb = 1;
                        //break;
                        exit(EXIT_FAILURE);
                    }
                    i++;
                    continue;
                }
                if(!strcmp(argv[i],"-b"))
                {
                    printf("Option -b can't be activated when more than 1 argument given\n");
                    //flagb = 1;
                    //break;
                    exit(EXIT_FAILURE);
                }
                else
                {
                    printf_level(4,".c file found\n");
                    if(strlen(argv[i])>2)
                    {
                        if(cpt == 1)
                        {
                            if(argv[i][strlen(argv[i])-1] == 'c' && argv[i][strlen(argv[i])-2] == '.')
                            {
                                fichierEntree = argv[i];
                                printf_level(4,"Updating infile with the .c file's name\n");
                                cpt++;
                            }
                            else
                            {
                                printf("File given isn't a C file\n");
                                fctUsage();
                                //flagb = 1;
                                //break;
                                exit(EXIT_FAILURE);
                            }
                            // Poser question fichiers inexistant ou autre argument rentré
                            i++;
                            continue;
                        }
                        else
                        {
                            printf("Only one .c file is required\n");
                            fctUsage();
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        if(strcmp(argv[i],"-h"))
                        {
                            printf("Argument given is not a file\n");
                            exit(EXIT_FAILURE);
                        }
                        fctUsage();
                        exit(EXIT_SUCCESS);
                    }
                }
            }
        }
    }
    else //si on a pas d'arguments
    {
        printf("Aucun argument passe a ./minicc [OPTIONS]\n\n");
        fctUsage();
        //flagb = 1;
        exit(EXIT_FAILURE);
    }
    infile = fichierEntree;
    fclose(fopen(outfile,"w"));
}

void free_nodes(node_t n) {
    // A implementer
}


char * strdupl(char * s) {
    char * r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}


static int32_t dump_tree2dot_rec(FILE * f, node_t n, int32_t node_num) {

    if (n == NULL) {
        fprintf(f, "    N%d [shape=record, label=\"{{NULL}}\"];\n", node_num);
        return node_num;
    }

    switch (n->nature) {
        case NODE_IDENT:
            {
                node_t decl_node = n->decl_node;
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{<decl>Decl      |Ident: %s|Offset: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->ident, n->offset);
                if (decl_node != NULL && decl_node != n) {
                    fprintf(f, "    edge[tailclip=false];\n");
                    fprintf(f, "    \"N%d\":decl:c -> \"N%d\" [style=dashed]\n", node_num, decl_node->node_num);
                }
                break;
            }
        case NODE_INTVAL:
        case NODE_BOOLVAL:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{Value: %" PRId64 "}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->value);
            break;
        case NODE_STRINGVAL:
            {
                char str[32];
                int32_t i = 1;
                while (true) {
                    str[i - 1] = n->str[i];
                    i += 1;
                    if (n->str[i] == '"') {
                        str[i - 1] = '\0';
                        break;
                    }
                }
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{val: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), str);
            }
            break;
        case NODE_TYPE:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type));
            break;
        case NODE_LIST:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE LIST}}\"];\n", node_num);
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK:
        case NODE_DECLS:
        case NODE_DECL:
        case NODE_IF:
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_DOWHILE:
        case NODE_PRINT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops);
            break;
        case NODE_FUNC:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}|{offset: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops, n->offset);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
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
        case NODE_SRA:
        case NODE_SRL:
        case NODE_SLL:
        case NODE_NOT:
        case NODE_BNOT:
        case NODE_UMINUS:
        case NODE_AFFECT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->nops);
            break;
        default:
            printf("*** Error in %s: unknow nature : %s\n", __func__, node_nature2string(n->nature));
            assert(false);
    }

    n->node_num = node_num;

    int32_t curr_node_num = node_num + 1;
    for (int32_t i = 0; i < n->nops; i += 1) {
        int32_t new_node_num = dump_tree2dot_rec(f, n->opr[i], curr_node_num);

        fprintf(f, "    edge[tailclip=true];\n");
        fprintf(f, "    N%d -> N%d\n", node_num, curr_node_num);
        curr_node_num = new_node_num + 1;
    }

    return curr_node_num - 1;
}



static void dump_tree2dot(FILE * f, node_t root) {
    assert(root->nature == NODE_PROGRAM);

    int32_t curr_node_num = 1;
    dump_tree2dot_rec(f, root, curr_node_num);
}


void dump_tree(node_t prog_root, const char * dotname) {

    FILE * f;

    f = fopen(dotname, "w");
    fprintf(f, "digraph global_vars {\n");
    dump_tree2dot(f, prog_root);
    fprintf(f, "}");    
    fclose(f);
}


const char * node_type2string(node_type t) {
    switch (t) {
        case TYPE_NONE:
            return "TYPE NONE";
        case TYPE_INT:
            return "TYPE INT";
        case TYPE_BOOL:
            return "TYPE BOOL";
        case TYPE_VOID:
            return "TYPE VOID";
        default:
            assert(false);
    }
}


const char * node_nature2string(node_nature t) {
    switch (t) {
        case NONE:
            return "NONE";
        case NODE_PROGRAM:
            return "PROGRAM";
        case NODE_BLOCK:
            return "BLOCK";
        case NODE_LIST:
            return "LIST";
        case NODE_DECLS:
            return "DECLS";
        case NODE_DECL:
            return "DECL";
        case NODE_IDENT:
            return "IDENT";
        case NODE_TYPE:
            return "TYPE";
        case NODE_INTVAL:
            return "INTVAL";
        case NODE_BOOLVAL:
            return "BOOLVAL";
        case NODE_STRINGVAL:
            return "STRINGVAL";
        case NODE_FUNC:
            return "FUNC";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_DOWHILE:
            return "DOWHILE";
        case NODE_PLUS:
            return "PLUS";
        case NODE_MINUS:
            return "MINUS";
        case NODE_MUL:
            return "MUL";
        case NODE_DIV:
            return "DIV";
        case NODE_MOD:
            return "MOD";
        case NODE_LT:
            return "LT";
        case NODE_GT:
            return "GT";
        case NODE_LE:
            return "LE";
        case NODE_GE:
            return "GE";
        case NODE_EQ:
            return "EQ";
        case NODE_NE:
            return "NE";
        case NODE_AND:
            return "AND";
        case NODE_OR:
            return "OR";
        case NODE_BAND:
            return "BAND";
        case NODE_BOR:
            return "BOR";
        case NODE_BXOR:
            return "BXOR";
        case NODE_SLL:
            return "SLL";
        case NODE_SRA:
            return "SRA";
        case NODE_SRL:
            return "SRL";
        case NODE_NOT:
            return "NOT";
        case NODE_BNOT:
            return "BNOT";
        case NODE_UMINUS:
            return "UMINUS";
        case NODE_AFFECT:
            return "AFFECT";
        case NODE_PRINT:
            return "PRINT";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




const char * node_nature2symb(node_nature t) {
    switch (t) {
        case NONE:
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_MUL:
            return "*";
        case NODE_DIV:
            return "/";
        case NODE_MOD:
            return "%";
        case NODE_LT:
            return "<";
        case NODE_GT:
            return ">";
        case NODE_LE:
            return "<=";
        case NODE_GE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NE:
            return "!=";
        case NODE_AND:
            return "&&";
        case NODE_OR:
            return "||";
        case NODE_BAND:
            return "&";
        case NODE_BOR:
            return "|";
        case NODE_BXOR:
            return "^";
        case NODE_SRA:
            return ">>";
        case NODE_SRL:
            return ">>>";
        case NODE_SLL:
            return "<<";
        case NODE_NOT:
            return "!";
        case NODE_BNOT:
            return "~";
        case NODE_UMINUS:
            return "-";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




