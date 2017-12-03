#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lex.h"
#include "strings.h"
#include "parser.h"
#include "symbol.h"
#include "symtable.h"
#include "errors.h"
#include "code_gen_expres.h"

htab_t *global_table;
function_data *global_data;
int p = 0;
int ar_count = 0;
int if_counter = 1;
int while_counter = 1;
int whole_div_cntr = 0;

int main() {
    int error = 0;
    if ((error = str_create(&(token.t_str))) != 0) {
        return error;
    }

    token.t_line = 1;

    global_table = htab_init(HTAB_SIZE);
    global_data = create_global_data();

    //str_create(&gen_str);

    int parser;
    parser = rule_start_state();

    if (parser != 0){
    	error = parser;
      switch (error) {
        case 1:
          fprintf(stderr, "ERROR - 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).\n");
          break;
        case 2:
          fprintf(stderr, "ERROR - 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu).\n");
          break;
        case 3:
          fprintf(stderr, "ERROR - 3 - sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atd.\n");
          break;
        case 4:
          fprintf(stderr, "ERROR -  4 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech, příp. špatný počet či typ parametrů u volání funkce.\n");
          break;
        case 6:
          fprintf(stderr, "ERROR -  6 - ostatní sémantické chyby.\n");
          break;
        case 99:
          fprintf(stderr, "ERROR -  99 - interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.).\n");
          break;
      }
    	printf("ERROR on line %d\n", token.t_line);

    }
    else {
      //printf("%s",gen_str.data);
      //printf("\n\n\nPARSER PROBEHL USPESNE\n");
    }

    str_destroy(&(token.t_str));
    htab_free(global_table);
    free(global_data);

    return error;
}
