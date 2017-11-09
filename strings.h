#ifndef STRINGS_H
#define STRINGS_H

typedef struct {
    int length; // aktualni delka 
    int size;   // alokovano
    char *data;
} Tstring;

// vytvori string (naalokuje 15 bytu, pote pridava)      
int str_create(Tstring *str);  

// vytvori string a alokuje mu velikost size
int str_create_spec(Tstring *str, int size); 

// uvolni string z pameti. Pote neni potreba nic dalsiho delat.
void str_destroy(Tstring *str);

// nastavi hodnoty na pocatecni, misto je stale alokovano stejne
void str_clear(Tstring *str);

// appenduje char do retezce
int str_add_char(Tstring *str, char c);

// appenduje retezec do retezce
int str_append_str(Tstring *target, Tstring *to_append);

// alokuje a inicializuje string tak, aby hned obsahoval data
int str_create_init(Tstring *str, const char *data);

#endif