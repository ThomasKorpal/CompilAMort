%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"
#include "common.h"
#include "passe_1.h"
#include "passe_2.h"

#include "y.tab.h"

int yyparse(node_t * program_root);
void analyse_tree(node_t root);

int flagb = 0;
char * infile = NULL;
char * outfile = DEFAULT_OUTFILE;
bool stop_after_syntax = false;
bool stop_after_verif = false;

#if YYDEBUG
extern int yydebug;
#endif

#if LEX_DEBUG
#define RETURN(token) ({ printf("%s \t\"%s\"\n", #token, yytext); return token; })
#else
#define RETURN(token) ({ return token; })
#endif


%}

%option yylineno


LETTRE          [A-Za-z]
/* A completer ; fausses definitions pour passer la compilation */
CHIFFRE         [0-9]
IDF             {LETTRE}({LETTRE}|{CHIFFRE}|_)*
CHIFFRE_NON_NUL [1-9]
ENTIER_DEC      0|({CHIFFRE_NON_NUL}{CHIFFRE}*)
LETTRE_HEXA     [a-fA-F]
ENTIER_HEXA     0x({CHIFFRE}|{LETTRE_HEXA})+
ENTIER          {ENTIER_DEC}|{ENTIER_HEXA}
CHAINE_CAR      [\x20-\x21\x23-\x5B\x5D-\x7E]
CHAINE          \"({CHAINE_CAR}|\\\"|\\n)*\"
COMMENTAIRE     "//"{CHAINE_CAR}*
/*SEPARATEUR      {\x20 | \t | \n }*/


%%

"void"          RETURN(TOK_VOID);
"int"           RETURN(TOK_INT);
"bool"          RETURN(TOK_BOOL);
"true"          RETURN(TOK_TRUE);
"false"         RETURN(TOK_FALSE);
"if"            RETURN(TOK_IF);
"else"          RETURN(TOK_ELSE);
"while"         RETURN(TOK_WHILE);
"for"           RETURN(TOK_FOR);
"do"            RETURN(TOK_DO);
"print"         RETURN(TOK_PRINT);
"+"             RETURN(TOK_PLUS);
"-"             RETURN(TOK_MINUS); /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
"*"             RETURN(TOK_MUL);
"/"             RETURN(TOK_DIV);
"%"             RETURN(TOK_MOD);
">"             RETURN(TOK_GT);
"<"             RETURN(TOK_LT);
"!"             RETURN(TOK_NOT);
"~"             RETURN(TOK_BNOT);
"&"             RETURN(TOK_BAND);
"|"             RETURN(TOK_BOR);
"^"             RETURN(TOK_BXOR);
"="             RETURN(TOK_AFFECT);
";"             RETURN(TOK_SEMICOL);
","             RETURN(TOK_COMMA);
"("             RETURN(TOK_LPAR);
")"             RETURN(TOK_RPAR);
"{"             RETURN(TOK_LACC);
"}"             RETURN(TOK_RACC);
">>"            RETURN(TOK_SRA);
">>>"           RETURN(TOK_SRL);
"<<"            RETURN(TOK_SLL);
">="            RETURN(TOK_GE);
"<="            RETURN(TOK_LE);
"=="            RETURN(TOK_EQ);
"!="            RETURN(TOK_NE);
"&&"            RETURN(TOK_AND);
"||"            RETURN(TOK_OR);

{IDF} {
                #if !LEX_DEBUG
                yylval.strval = strdupl(yytext);
                #endif
                RETURN(TOK_IDENT);
}

{ENTIER} {
                #if !LEX_DEBUG
                /* A completer : affecter yylval.intval et faire des verifications */
                if(strlen(yytext)>=3 && yytext[1]=='x')
                {
                    if(strlen(yytext)>10)
                    {
                        printf("int out of 32 bits range\n");
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        yylval.intval = (int)strtol(yytext,NULL,0);
                    }
                }
                else
                {
                    if(atoi(yytext)<2147483648)
                    {
                        yylval.intval = atoi(yytext);
                    }
                    else
                    {
                        printf("int out of 32 bits range\n");
                        yylval.intval = 0xFFFFFFFF;
                        //flagb=1;
                        exit(EXIT_FAILURE);
                        //comment rendre erreur bloquante
                    }
                }
                #endif
                RETURN(TOK_INTVAL);
}

{CHAINE} {
                #if !LEX_DEBUG
                /* A completer : affecter yylval.strval */
                yylval.strval = strdupl(yytext);
                #endif
                RETURN(TOK_STRING);
}

{COMMENTAIRE} {
}

[ \t\n\r]+

. {
                fprintf(stderr, "Error line %d: Lexical error\n", yylineno);
                exit(1);
}


%%

int yywrap(void) {
    return 1;
}

int main(int argc, char ** argv) { 
    node_t program_root;
    analyseArgs(argc, argv);
    if(!flagb)
    {
        // Poser question fichiers inexistant ou autre argument rentré
        if(!access(infile,F_OK))
        {
            yyin = fopen(infile, "r");
        #if LEX_DEBUG
            while(yylex());
            fclose(yyin);
        #else
            #if YYDEBUG
            yydebug = 1;
            #endif
            yyparse(&program_root);
            analyse_tree(program_root);
            if(!stop_after_syntax)
            {
                analyse_passe_1(program_root);
                if(!stop_after_verif)
                {
                    //reste du code
                    //gen_code_passe_2(program_root);
                }
            }
            fclose(yyin);
        #endif
            yylex_destroy();
        }
        else
        {
            printf("File given is inexistant\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}



