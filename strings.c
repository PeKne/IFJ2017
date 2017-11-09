#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"

#define ERR_ALLOC -1
#define STR_SIZE 15

int str_create(Tstring *str)
{
    if ((str->data = (char *) malloc(sizeof(char) * STR_SIZE)) == NULL)
        return ERR_ALLOC;

    str->length = 0;
    str->size = STR_SIZE;
    str->data[0] = '\0';
    return 0;
}

int str_create_spec(Tstring *str, int size)
{
    size++; // je potreba misto pro \0 
    str->length = 0;
    if ((str->data = (char *) malloc(sizeof(char) * size)) == NULL)
        return ERR_ALLOC;
    str->size = size;
    str->data[0] = '\0';
    return 0;
}

void str_destroy(Tstring *str)
{
    str->length = 0;
    str->size = 0;
    free(str->data);
    str->data = NULL;
}

void str_clear(Tstring *str)
{
    str->length = 0;
    str->data = '\0';
}


int str_add_char(Tstring *str, char c)
{
    if ( str->length+1 >= str->size) {
        if ((str->data = (char *) realloc(str, sizeof(char) * (str->length + STR_SIZE))) == NULL)
            return ERR_ALLOC;
        str->size += STR_SIZE; 
    }
        str->data[str->length] = c;
        str->data[++(str->length)] = '\0';
        return 0;
}

int str_append_str(Tstring *target, Tstring *to_append)
{
    int err = 0;
    for (int i = 0; i < to_append->length; ++i)
    {
        if ((err = str_add_char(target, to_append->data[i])) != 0)
            return err;
    }
    
    return 0;
}

int str_create_init(Tstring *str, const char *data)
{
    int length = strlen(data);
    if ((str->data = (char *) malloc(sizeof(char) * length+1)) == NULL)
        return ERR_ALLOC;
    strcpy(str->data, data);
    str->length = length;
    str->size = length + 1; // \0 pridan implicitne za data
    return 0;
}