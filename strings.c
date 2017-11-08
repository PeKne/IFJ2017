#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define ERR_ALLOC -1;
#define STR_SIZE 15

typedef struct {
    int length; // aktualni delka 
    int size;   // alokovano
    char *data;
}T_string;

int str_create_spec(string *str, int size+1)
{
    str->length = 0;
    if ((str->data = (char *) Malloc(sizeof(char) * size)) == NULL)
        return ERR_ALLOC;
    str->size = size;
    str->data[0] = '\0';
    return 0;
}

int str_create(string *str)
{
    if ((str->data = (char *) Malloc(sizeof(char) * size)) == NULL)
        return ERR_ALLOC;

    str->length = 0;
    str->size = STR_SIZE;
    str->data[0] = '\0';
    return 0;
}

void str_destroy(string *str)
{
    str->length = -1;
    str->size = 0;
    free(str);
    str = NULL;
}

void str_clear(string *str)
{
    str->length = 0;
    str->data = '\0';
}

int str_addChar(string *str, char c)
{
    if ( str->length+1 >= str->size) {
        if ((str->data = (char *) Realloc(str, sizeof(char) * (str->length + STR_SIZE))) == NULL)
            return ERR_ALLOC;
        str->size += STR_SIZE; 
    }
        str->data[str->length] = c;
        str->data[str->length+1] = '\0';
        return 0;
}