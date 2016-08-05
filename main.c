#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

    int estado;
    int tamanoArreglo;
    char** arreglo;
    /**/
    /*----------------------------*/
    /*Banderas reservadas*/
    /*----------------------------*/
    int bool_mkdisk = 0;
    int bool_sizee = 0;
    int bool_unit = 0;
    int bool_path = 0;
    int bool_name = 0;

    int bool_type = 0;
    int bool_fit = 0;
    int bool_deletee = 0;
    int bool_add = 0;
    /*----------------------------*/
    /*Banderas reservadas*/
    /*----------------------------*/
    int val_size = 0;
    char val_unit[1] = "m";
    char* val_direccion;
    char* nombre;


    /*----------------------------*/
    /*Valores reservados*/
    /*----------------------------*/

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char *s = strdup(a_str);
    char* tmp        = s;
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
    count += last_comma < (s + strlen(s) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(s, delim);

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

int tamano3(char *arreglo)
{
    int i = 0;
    while (*(arreglo+i) != '\0' && arreglo != NULL )
    {
        ++i;
    }
    return i;
}

char *arregloDireccion(char *comando)
{
    int tamanio = contadorDiagonales(val_direccion);
    int tamanioTotal = tamano3(val_direccion);
    char *respuesta;
    respuesta =(char*)malloc(tamanio +tamanioTotal);
    int d;
    int count = 0;
    int bul = 0;
    for(d = 0; d < tamanio +tamanioTotal -1; d++)
    {
        if(*(val_direccion+d) == '/')
        {
            if(bul == 0)
            {
                bul = 1;
                *(respuesta + count) = *(val_direccion +d);
                ++count;
            }else
            {
                *(respuesta + count) = ',';
                ++count;
                *(respuesta + count) = *(val_direccion +d);
                ++count;
            }
        }
        else
        {
            *(respuesta + count) = *(val_direccion +d);
            ++count;
        }
    }
    printf("\n&&&&&&&&&&&&&&&&&&&&\n");
    printf(respuesta);
    printf("\n&&&&&&&&&&&&&&&&&&&&");
    return respuesta;
}

char *concatenarDireccion(char **arregloDirecciones, int direcciones)
{
    char *respuesta;
    int maximos[direcciones];
    int tamanos[direcciones];
    if(direcciones == 1)
    {
        printf("direcciones[%d]", direcciones);
        return *(arregloDirecciones +0);
    }else
    {
        printf("direcciones[%d]", direcciones);
        int i;
        int j;
        int count = 0;
        int tamanio = 0;
        for(i=0; i < direcciones; i++)
        {
            tamanio = tamanio + tamano3(*(arregloDirecciones+i));
            tamanos[i] = tamano3(*(arregloDirecciones+i));
            maximos[i] = tamanio;
        }
        printf("tamanio:%d", tamanio);
        respuesta =(char*)malloc(tamanio+1);
        for(i = 0; i < direcciones; i++)
        {
            char *actual = *(arregloDirecciones + i);
            for(j = 0; j< tamanos[i]; j++)
            {
                *(respuesta + count) = *(actual+j);
                ++count;
            }
        }
    }
    printf("\n++++++++++++++++++++\n");
    printf(respuesta);
    printf("\n++++++++++++++++++++++");
    return respuesta;
}

char *concatenacion(char *parametro1, char *parametro2)
{
    char *respuesta;
    int char_tamano1 = tamano3(parametro1);
    int char_tamano2 = tamano3(parametro2);
    respuesta = malloc(char_tamano1 + char_tamano2 - 1);
    strcpy(respuesta, parametro1);
    strcat(respuesta, parametro2);
    printf("\nccccccccccccccccccc1\n");
    printf(respuesta);
    printf("\n-");
    return respuesta;
}

void crearDisco()
{
    printf("\nCreando disco...\n");
    int t_bytes = 0;
    struct stat st = {0};
    char string[32];
    if(val_unit[0] == 'k')
    {
        t_bytes = 1024 * val_size;
    }
    else if(val_unit[0] == 'm')
    {
        t_bytes = 1024 * 1024 * val_size;
    }

    if(stat(val_direccion, &st) == -1)
    {
        printf("\nCreando carpeta...\n");
        char *paraSplit;
        char *direccionActual;
        paraSplit = arregloDireccion(val_direccion);
        char** carpetas = str_split(paraSplit ,',');
        int carpetas_count = tamano(carpetas);
        int b = 0;
        while(b < carpetas_count)
        {
            direccionActual = concatenarDireccion(carpetas, b+1);
            printf("[");
            printf(direccionActual);
            printf("]");
            if(stat(direccionActual, &st) == -1)
            {
                printf("\nno existe\n");
                mkdir(direccionActual, 0700);
                printf("Ahora ya existe\n");
            }
            else
            {
                printf("\nsi existe\n");
            }
            ++b;
        }
        printf("\nCreando archivo\n");
        printf(nombre);
        printf("\n....\n");
        char *ubicacion = concatenacion(val_direccion, nombre);
        FILE *fp = fopen(ubicacion, "w");
        fseek(fp, t_bytes , SEEK_SET);
        fputc('\0', fp);
        fclose(fp);
    }else
    {
        printf("[Ya fue creada la carpeta]");
        printf("\nCreando archivo...\n");
        printf(nombre);
        printf("\n....\n");
        char *ubicacion = concatenacion(val_direccion, nombre);
        FILE *fp = fopen(ubicacion, "w");
        fseek(fp, t_bytes , SEEK_SET);
        fputc('\0', fp);
        fclose(fp);
    }
}
void minusculas(char **entrada)
{
    printf("minusculas");
}

/*Contador de elementos*/
int tamano(char **entradaTotal)
{
    int i;
    /*printf("Calculando tamano...\n");*/
    if (entradaTotal)/*se realizo el split correctamente*/
    {
        for (i = 0; *(entradaTotal + i); i++)
        {
            /*printf("Palabra =[%s]\n", *(entradaTotal + i));*/
            /*free(*(entradaTotal + i));*/
        }
        printf("\n");
        /*free(entradaTotal);*/
    }
    /*printf("El tamano es: ");
    printf("%d", i);*/
    return i;
}

int tamano2(char arreglo[])
{
    int i;
    int d = 1;
    /*printf("Calculando tamano2...\n");*/

        for (i = 0; d == 1; ++i)
        {
            if(arreglo[i]=='\n'){break;}
        }
        printf("\n");

    return i;
}

int contadorPuntos(char entrada[])
{
    int contadorPuntos = 0;
    int tk;
    for(tk = 0; entrada[tk]!='\0'; ++tk)
        {
            if(entrada[tk-1]==':')
             {
                contadorPuntos= contadorPuntos + 1;
             }

        }
    /*printf("\nPuntos:");
    printf("%d", contadorPuntos);
    printf("\n");*/
        return contadorPuntos/2;
}

int contadorDiagonales(char entrada[])
{
    int contadorPuntos = 0;
    int tk;
    for(tk = 0; entrada[tk]!='\0'; ++tk)
        {
            if(entrada[tk-1]=='/')
             {
                contadorPuntos= contadorPuntos + 1;
             }

        }
    /*printf("\nPuntos:");
    printf("%d", contadorPuntos);
    printf("\n");*/
        return contadorPuntos;
}
char* getComando(char comando[])/*Enviar comando completo, devuelve valor del comando*/
{
    char** ArregloComando;
    ArregloComando = str_split(comando, ':');
    return *(ArregloComando + 0);
}
char* getTexto(char comando[])/*Enviar comando completo, devuelve valor del comando*/
{
    char** ArregloComando;
    ArregloComando = str_split(comando, ':');
    return *(ArregloComando + 1);
}
int getValorEntero(char* comando)/*Enviar comando completo, devuelve valor del comando*/
{
    printf("\ngetValorEntero\n");
    int respuesta = 0;
    char** ArregloComando;
    ArregloComando = str_split(comando, ':');
    sscanf(*(ArregloComando +1), "%d", &respuesta);
    printf("[%d]", respuesta);
    return respuesta;
}
char *getValorCadena(char *comando)/*Enviar comando completo, devuelve el valor de la cadena*/
{
    char** ArregloComando = str_split(comando, ':');
    char* valor = *(ArregloComando +1);
    int i = 0;
    printf("lllllll[%s]",valor, "\n");
    int tamanio = tamano2(comando);
    for(i = 0; *(valor+i) != '\0' && *(valor+i) != NULL; i++){}
    printf("\ntamano\n");
    printf("[%d]\n", i);
    char *respuesta;
    respuesta =(char*)malloc(tamanio-1);
    int d;
    int count = 0;
    for(d = 0; d< i-1; d++)
    {
        if(*(valor +d) != '"')
        {
            *(respuesta + count) = *(valor +d);
            ++count;
        }
    }
    printf("\n*******************");
    printf(respuesta);
    printf("\n********************");
    return respuesta;
}
char *getValorDisco(char *comando)/*Enviar comando completo, devuelve el valor de la cadena*/
{
    char** ArregloComando = str_split(comando, ':');
    char* valor = *(ArregloComando +1);
    int i = 0;
    printf("lllllll[%s]",valor, "\n");
    int tamanio = tamano2(comando);
    for(i = 0; *(valor+i) != '\0' && *(valor+i) != NULL; i++){}
    printf("\ntamano\n");
    printf("[%d]\n", i);
    char *respuesta;
    respuesta =(char*)malloc(tamanio);
    int d;
    int count = 0;
    int truee=0;
    for(d = 0; d< i-1; d++)
    {
        if(*(valor +d) == '"')
        {
            if(truee == 0)
            {
                *(respuesta + count) = '/';
                ++count;
                truee = 1;
            }
        }else
        {
            *(respuesta + count) = *(valor +d);
            ++count;
        }
    }
    printf("\n*******************");
    printf(respuesta);
    printf("\n********************");
    return respuesta;
}
void automata(char** entradaTotal, char* entradaUnica, int posicion)
{
    printf("\n---------------\n");
    printf("Metodo automata:\n");
    /*-------------------------------------------------*/
    char* token = getComando(*(entradaTotal+posicion));
    printf("-----\n");
    printf(*(entradaTotal + posicion));
    printf("\n-----\n");
    /*LISTA DE PALABRAS
     RESERVADAS*/
    char *mkdisk = "mkdisk";
    char *sizee = "-size";
    char *unit = "+unit";
    char *path = "-path";
    char *name = "-name";

    char *type = "+type";
    char *fit = "+fit";
    char *deletee = "+delete";
    char *add = "+add";
    char *rmdisk = "rmdisk";
    if (posicion < tamanoArreglo)/*se realizo el split correctamente*/
    {

        if(strcmp(token, mkdisk)==0)
        {
            bool_mkdisk = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, sizee)==0)
        {
            bool_sizee = 1;
            printf(*(entradaTotal + posicion));
            val_size = getValorEntero(*(entradaTotal + posicion));
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, unit)==0)
        {
            bool_unit = 1;
            char* unidad = getTexto(*(entradaTotal + posicion));
            val_unit[0] = unidad[0];
            printf("%c\n", val_unit[0]);
            printf("\nEstado = 3\n");
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, path)==0)
        {
            bool_path = 1;
            val_direccion = getValorCadena(*(entradaTotal + posicion));
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, type)==0)
        {
            bool_type = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, name)==0)
        {
            bool_name = 1;
            nombre = getValorDisco(*(entradaTotal + posicion));
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, fit)==0)
        {
            bool_fit = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, deletee)==0)
        {
            bool_deletee = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, add)==0)
        {
            bool_add = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }

    }else
    {
        printf("\nDesborde\n");
    }
}

void master_Driver()
{

    if(bool_mkdisk == 1)
    {
        /*Comando mkdisk utilizado*/
        if(bool_sizee==1 && bool_unit==1 && bool_path==1 && bool_name==1)
        {
            crearDisco();
        }
        else if(bool_sizee==1 && bool_unit==0 && bool_path==1 && bool_name==1)
        {
            crearDisco();
        }
        else
        {
            printf("Error, comando no reconocido.");
        }
    }
}

int main()
{
    estado = 0;
    /*tamanoArreglo = 0;
    int y = 5;
    y = y * 9;
    printf("%d", y);*/
    printf("Universidad de San Carlos de Guatemala\n");
    printf("Ingenieria en Ciencias y Sistemas\n");
    printf("Proyecto: Fase 1\n");
    printf("                        [File System ext2/ext3]\n\n\n");
    /*---------------------------------------------------------------*/
    int exit = 0;
    /*while(exit == 0)
    {*/
        char entradaUsuario[256];
        fgets(entradaUsuario, sizeof(entradaUsuario), stdin);
        int puntos = contadorPuntos(entradaUsuario);/*regresa la cant de :'s/2*/
        int tamanoarray = tamano2(entradaUsuario);
        char entrada[sizeof(entradaUsuario)-puntos];

        /*Habiendo definido un nuevo arreglo para la entrada, a la que se le
        convertira sustituyendo cada par de dos puntos por uno solo*/
        int d;
        int paridad = 0;/*simulador de booleano*/
        int contador = 0;
        for(d = 0; d<tamanoarray+1; d++)
        {
            if(entradaUsuario[d] ==':'){
                if(paridad == 1)
                {
                    paridad = 0;
                    entrada[contador] = entradaUsuario[d];
                    contador = contador+1;
                }else
                {
                    paridad = 1;
                }
            }else
            {
                entrada[contador] = entradaUsuario[d];
                contador = contador +1;
            }

        }
        /*******************************/
        /*Fin reescritura de la entrada*/
        /*******************************/
        /*******************************/
        /*Iniciando el pseudoautomata y el analisis realizado por el mismo*/
        /*******************************/
        char** tokens;
        printf("Se ingresó:[%s]\n\n", entrada);
        tokens = str_split(entrada, ' ');
        printf("Despues:[%s]\n\n", entrada);
        tamanoArreglo = tamano(tokens);
        printf("\nIniciando el automata...\n");
        automata(tokens, *(tokens + 0), 0);
        master_Driver();
    /*}*/
    /*-------------------------------------------------------------*/
    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("Ultima revision, palabra [%d]=[%s]\n", i, *(tokens + i));
        }
        printf("\n");
    }
    return 0;
}
