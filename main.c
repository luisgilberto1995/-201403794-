#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int estado;

void minusculas(char **entrada)
{
    printf("minusculas");
}

void automata(char** entradaTotal, char* entradaUnica)
{
    printf("---------------\n");
    printf("Metodo automata:\n\n\n");
if (entradaTotal)
    {
        int i;
        for (i = 0; *(entradaTotal + i); i++)
        {
            /*printf("Palabra =[%s]\n", *(entradaTotal + i));*/
            free(*(entradaTotal + i));
            if(strcmp(*(entradaTotal + i), entradaUnica)==0)
                {
                    printf("Encontrado!\n");
                }else
                {
                    printf("NoEncontrado!\n");
                }
        }
        printf("\n");
        free(entradaTotal);
    }
    printf("\n\n");
}
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Conteo de los elementos a extraer */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
int main()
{
    estado = 0;
    /*int y = 5;
    y = y * 9;
    printf("%d", y);*/
    printf("Universidad de San Carlos de Guatemala\n");
    printf("Ingenieria en Ciencias y Sistemas\n");
    printf("Proyecto: Fase 1\n");
    printf("                    [File System ext2/ext3]\n\n\n");
    char entrada[256];
    fgets(entrada, sizeof(entrada), stdin);
    char** tokens;
    printf("Se ingresó:[%s]\n\n", entrada);
    tokens = str_split(entrada, ' ');
    automata(tokens, *(tokens + 0));
    /*if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("month=[%s]\n", *(tokens + i));
            free(*(tokens + i));
        }
        printf("\n");
        free(tokens);
    }*/
    return 0;
}
