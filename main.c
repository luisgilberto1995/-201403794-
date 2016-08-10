#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
    int estado;
    int tamanoArreglo;
    char** arreglo;
    int particionado_disco_actual = 0;
    char val_fit;/*BF 1, FF 2, WF 3*/
    int val_add = 0;
    char *pathDisco;
    int load_disk_flag = 0;
    int bytesize_mbr = 136;
    int num_particiones = 0;
    /****************************/
    int minimos1[4];
    int superior[4];
    /****************************/
    int LimSuperior[4];
    int LimInferior[4];
    /****************************/
    int EspacioLibre[5];
    int EspacioLibre_min[5];
    int EspacioLibre_max[5];
    /**/
    /*----------------------------*/
    /*Banderas reservadas*/
    /*----------------------------*/
    int bool_mkdisk = 0;
    int bool_rmdisk = 0;
    int bool_fdisk = 0;

    int bool_sizee = 0;
    int bool_unit = 0;
    int bool_path = 0;
    int bool_name = 0;

    int bool_type = 0;
    int bool_fit = 0;
    int bool_deletee = 0;
    int bool_add = 0;
    /*----------------------------*/
    /*     Banderas reservadas   */
    /*----------------------------*/
    int val_size = 0;
    char val_unit[1] = "m";
    char* val_direccion;
    char* nombre;
    char* nombre_fdisk;
    char val_type[1] = "p";
    /*----------------------------*/
    /*Valores reservados*/
    /*----------------------------*/
    typedef struct particion
    {
        char part_status;/*1 activa 0 no*/
        char part_type;/*1 primaria 0 extendida*/
        char part_fit;/*primer mejor peor*/
        int part_start;
        int part_size;
        char part_name[16];
    }particion;

    typedef struct mbr_DISCO
    {
        int mbr_tamano;/*En KB*/
        time_t mbr_fecha_creacion;
        int mbr_disk_signature;/*numero random*/
        particion mbr_partition[4];
    }mbr_DISCO, *D_actual;

 struct ebr_extended
{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}ebr;
    struct mbr_DISCO *mbr_p = NULL;
    struct ebr_extended *ebr_p = NULL;

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

off_t filesize(const char *filename){
    struct stat st;
    if( stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}

void reporteLogicas()
{
    if (getExtendidas() == 1)
    {
        int i;
        int start;
        for(i = 0; i<4; i++)
        {
            if(mbr_p->mbr_partition[i].part_type == 'e')
            {
                start = 140 + mbr_p->mbr_partition[i].part_start;
            }
        }
        printf("\nREPORTE-------------------------\nstart:%d\n", start);
        struct ebr_extended *ebr_actual=malloc(sizeof(struct ebr_extended));
        int status = 0;
        int c = 0;
        while(status != -1)
        {
            FILE * file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, ebr_actual->part_next+140 ,SEEK_SET);
                fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
                fclose(file);
            }
            status = ebr_actual->part_next;
            printf("\n[DATO]start: %d tamano: %d next:%d", ebr_actual->part_start, ebr_actual->part_size, status);
            c++;
            if(c == 25){break;}
        }
    }
}

int getPrimarias()
{
    int primarias = 0;
    int i;
    for(i = 0; i< 4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            if(mbr_p->mbr_partition[i].part_type == 'p')
            {
                primarias = primarias + 1;
            }
        }
    }
    return primarias;
}

int getExtendidas()
{
    int extendidas = 0;
    int i;
    for(i = 0; i< 4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            if(mbr_p->mbr_partition[i].part_type == 'e')
            {
                extendidas = extendidas + 1;
            }
        }
    }
    return extendidas;
}

int getParticiones()
{
    int contador = 0;
    int i;
    for(i = 0; i<4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            ++contador;
        }
    }
    return contador;
}

void Particionar(int valor_size)
{
    /*------------------PARTICION PRIMARIA---------------------*/
    int i;
    int s_flag = 0;
    int created = 0;
    for(i = 0; i< 5; i++)
    {
        if(valor_size <= EspacioLibre[i])
        {
            int a;
            for(a = 0; a<4; a++)
            {
                if(mbr_p->mbr_partition[a].part_status == '0' && created == 0)
                {
                    int namesize = tamano3(nombre_fdisk);
                    if(namesize <= 16)
                    {
                        int c;
                        for(c = 0; c < namesize; c++)
                        {
                            mbr_p->mbr_partition[a].part_name[c] = *(nombre_fdisk + c);
                        }
                        mbr_p->mbr_partition[a].part_status = '1';
                        mbr_p->mbr_partition[a].part_type = 'p';
                        mbr_p->mbr_partition[a].part_size = valor_size;
                        mbr_p->mbr_partition[a].part_start = EspacioLibre_min[i];
                        printf("\nEsta particion inicia en: %d", EspacioLibre_min[i]);
                        printf("\nTamano de la particion: %d", valor_size);
                        FILE *file = fopen(val_direccion, "rb+");
                        size_t tam = sizeof(struct mbr_DISCO);
                        printf("\nVALOR EN BYTES[%zu]\n", tam);
                        if(file != NULL)
                        {
                            fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                            fclose(file);
                            printf("\nParticion primaria realizada.\n");
                            s_flag = 1;
                        }
                        created = 1;
                    }else{ printf("ERROR: nombre muy extenso para la particion.");}
                }
            }
        }
    }
    if(s_flag == 0)
    {
        printf("\n:(\n");
    }
    /*---------------------------------------------------------------------------*/
}

void particionLogica(int t_bytes)
{
    int i;
    int pos = 0;
    int confirmacion = 0;
    for(i = 0; i < 4; i++)
    {
        if(mbr_p->mbr_partition[i].part_type == 'e')
        {
            pos = i;
            break;
        }
    }
    /*******************LECTURA Y RECOLECCION DE DATOS DE INTERES******************/
    int default_size = mbr_p->mbr_partition[pos].part_size;
    int start = mbr_p->mbr_partition[pos].part_start ;
    printf("\nParticion extendida, fisicamente inicia en: %d de tamano: %d", start+140, default_size);
    struct ebr_extended *ebr_actual=malloc(sizeof(struct ebr_extended));
    FILE * file= fopen(val_direccion, "rb");
    if (file != NULL) {
        fseek(file, start+140 ,SEEK_SET);
        fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
        fclose(file);
    }
    /******************************************************************************/
    printf("\n//%c, %d\n", ebr_actual->part_status, ebr_actual->part_next);
    if(ebr_actual->part_status == '0'&& ebr_actual->part_next == -1)
    {
        /*Es la primera lógica*/
        printf("\nprimera particion logica\n");
        if(t_bytes <= default_size)
        {
            int namesize = tamano3(nombre_fdisk);
            if(namesize <= 16)
            {
                int c;
                for(c = 0; c < namesize; c++)
                {
                    ebr_actual->part_name[c] = *(nombre_fdisk + c);
                }
                ebr_actual->part_start = start;
                ebr_actual->part_fit = val_fit;
                ebr_actual->part_status = '1';
                ebr_actual->part_size = t_bytes;
                ebr_actual->part_next = -1;

                /**/
                printf("\nActualizando fisicamente EBR en: %d", start+140);
                /**/
                FILE * file_update = fopen(val_direccion, "rb+");
                if (file_update != NULL) {
                    fseek(file_update, start+140 ,SEEK_SET);
                    fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                    fclose(file_update);
                }
            }else
            {
                printf("\nERROR:El nombre excede el limite\n");
            }
        }else
        {
            printf("\nError: no hay espacio disponible :(\n");
        }

    }else
    {
        int confirmacion2 = 0;
        if(ebr_actual->part_status == '0'&& ebr_actual->part_next != -1)
        {
            printf("\nPrimer EBR vacio, pero existen mas particiones logicas");
            if(t_bytes <= (ebr_actual->part_start - start))
            {
                confirmacion2 = 1;
                printf("\nSi cabe en el primer EBR");
                int namesize = tamano3(nombre_fdisk);
                if(namesize <= 16)
                {

                    int c;
                    for(c = 0; c < namesize; c++)
                    {
                        ebr_actual->part_name[c] = *(nombre_fdisk + c);
                    }
                    ebr_actual->part_fit = val_fit;
                    /*ebr_actual->part_next = ebr_actual->part_next;*/
                    ebr_actual->part_size = t_bytes;
                    ebr_actual->part_start = start;
                    ebr_actual->part_status ='1';
                    /**/
                    printf("\nActualizando fisicamente EBR en: %d", start+140);
                    /**/
                    FILE * file_update = fopen(val_direccion, "rb+");
                    if (file_update != NULL) {
                        fseek(file_update, start+140 ,SEEK_SET);
                        fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                        fclose(file_update);
                        confirmacion = 1;
                    }

                }else
                {
                    printf("\nERROR:El nombre excede el limite.");
                }
            }
        }
        printf("\nConfirmacion %d ... NEXT: %d", confirmacion2, ebr_actual->part_next);
        if(confirmacion2 == 0)
        {
            while(confirmacion2 == 0)
            {
                if(ebr_actual->part_next == -1)
                {
                    confirmacion2 = 1;
                    printf("\nEs el ultimo EBR.");
                    printf("\nParticion:%d Espacio %d", t_bytes, ((default_size) - (start + ebr_actual->part_size)));
                    if(t_bytes <= ((default_size) - (start + ebr_actual->part_size)))
                    {
                        int namesize = tamano3(nombre_fdisk);
                        if(namesize <= 16)
                        {
                            struct ebr_extended *ebr_nuevo=malloc(sizeof(struct ebr_extended));
                            int c;
                            for(c = 0; c < namesize; c++)
                            {
                                ebr_nuevo->part_name[c] = *(nombre_fdisk + c);
                            }
                            ebr_nuevo->part_fit = val_fit;
                            ebr_nuevo->part_next = -1;
                            ebr_nuevo->part_size = t_bytes;
                            ebr_nuevo->part_start = ebr_actual->part_start + ebr_actual->part_size;
                            ebr_nuevo->part_status = '1';
                            ebr_actual->part_next = ebr_nuevo->part_start;

                            start = ebr_nuevo->part_start;
                            printf("\nActualizando EBR en:%d su nuevo part_start: %d" , ebr_actual->part_start+140, ebr_actual->part_next);
                            FILE * file_update= fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start+140 ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                                printf("\n???");
                            }
                            printf("\nEscribiendo fisicamente ultimo EBR en:%d", start+140);
                            FILE * file= fopen(val_direccion, "rb+");
                            if (file != NULL) {
                                fseek(file, start+140 ,SEEK_SET);
                                fwrite(ebr_nuevo, sizeof(struct ebr_extended), 1, file);
                                fclose(file);
                                confirmacion = 1;
                            }
                        }else
                        {
                            printf("\nERROR:El nombre excede el limite.");
                        }
                    }
                }else
                {
                    printf("\nebr_actual->part_next:%d", ebr_actual->part_next);
                    printf("\nebr_actual->part_start:%d", ebr_actual->part_start);
                    printf("\nebr_actual->part_size:%d", ebr_actual->part_size);
                    int operacion = 0;
                    operacion = ebr_actual->part_next - (ebr_actual->part_start + ebr_actual->part_size);
                    printf("\nt1:%d", t_bytes);
                    /*printf("\noperacion:%d", operacion);*/
                    printf("\nt2");
                    if(t_bytes <= operacion)
                    {
                        printf("\nt");
                        confirmacion2 = 1;
                        printf("\nt");
                        int namesize = tamano3(nombre_fdisk);
                        printf("\nt");
                        if(namesize <= 16)
                        {
                            struct ebr_extended *ebr_nuevo=malloc(sizeof(struct ebr_extended));
                            int c;
                            for(c = 0; c < namesize; c++)
                            {
                                ebr_nuevo->part_name[c] = *(nombre_fdisk + c);
                            }
                            ebr_nuevo->part_fit = val_fit;
                            ebr_nuevo->part_next = ebr_actual->part_next;
                            ebr_nuevo->part_size = t_bytes;
                            ebr_nuevo->part_start = ebr_actual->part_start + ebr_actual->part_size;
                            ebr_nuevo->part_status = '1';
                            ebr_actual->part_next = ebr_nuevo->part_start;
                            start = ebr_nuevo->part_start;
                            printf("\nEscribiendo en medio EBR en:%d", start);

                            FILE * file_update= fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start+140 ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                            }
                            FILE * file= fopen(val_direccion, "rb+");
                            if (file != NULL) {
                                fseek(file, start +140,SEEK_SET);
                                fwrite(ebr_nuevo, sizeof(struct ebr_extended), 1, file);
                                fclose(file);
                                confirmacion = 1;
                            }
                        }else
                        {
                            printf("\nERROR:El nombre excede el limite.");
                        }
                    }
                }
                if(confirmacion2 == 0)
                {
                    if(ebr_actual->part_next != -1)
                    {
                        FILE * file5= fopen(val_direccion, "rb");
                        if (file5 != NULL) {
                            fseek(file5, ebr_actual->part_next+140 ,SEEK_SET);
                            fread(ebr_actual, sizeof(struct ebr_extended), 1, file5);
                            fclose(file5);
                        }
                    }else
                    {
                        confirmacion2 = 1;
                    }
                }
            }
        }
    }
    if(confirmacion == 1)
    {
        printf("\nParticion logica creada correctamente\n");
    }
}

void setMinimos()
{
    int i;
    int contador = 0;
    for(i = 0; i<4; i++)
    {
        /*ordenar estos datos de menor a mayor*/
        minimos1[i] = 0;
        superior[i] = 0;
        /*-------------------------------------*/
        /*aqui van ordenadas las respuestas*/
        LimInferior[i] = 0;
        LimSuperior[i] = 0;
        /*--------------------------------------*/
        EspacioLibre[i] = 0;
        EspacioLibre_min[i] = 0;
        EspacioLibre_max[i] = 0;
    }
    EspacioLibre[4] = 0;
    EspacioLibre_min[4] = 0;
    EspacioLibre_max[4] = 0;
    for(i = 0; i < 4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            minimos1[contador] = mbr_p->mbr_partition[i].part_start;
            superior[contador] = mbr_p->mbr_partition[i].part_start + mbr_p->mbr_partition[i].part_size;
            printf("\nHay una particion que inicia en: %d y de tamano: %d\n", mbr_p->mbr_partition[i].part_start, mbr_p->mbr_partition[i].part_size);
            ++contador;
        }
    }
    int count = 0;
    int count2 = 0;
    int flag = 0;
    int pos = 0;
    int minimo_cero = 0;
    printf("\nParticiones encontradas:%d", contador);
    /******************************************************************/
    int mini = minimos1[0];
    int max = superior[0];
    if(mini == 0)
    {
        LimInferior[count2] = mini;
        LimSuperior[count2] = max;
        ++count2;
        ++count;
    }
    for(; count < contador; count++)
    {
        int y;
        /*Asignando a mini un valor diferente de 0, puesto que si existía 0 ya fue agregado*/
        for(y = 0; y< 4; y++)
        {
            if(minimos1[y] != 0)
            {
                mini = minimos1[y];
                max = superior[y];
                pos = y;
                break;
            }
        }

        printf("\nMINI:%d MAX%d\n", mini, max);
        /*buscando el verdadero minimo*/
        for(y = 0; y<4; y++)
        {
            if(minimos1[y] < mini && minimos1[y] != 0)
            {
                mini = minimos1[y];
                max = superior[y];
                flag = 1;
                pos = y;
                printf("\nfor1 %d < %d - %d != 0\n", minimos1[y], mini, minimos1[y]);
            }
        }
        /*if(flag ==1)
        {*/
            LimInferior[count2] = mini;
            LimSuperior[count2] = max;
            minimos1[pos] = 0;
            flag = 0;
            printf("\nEstableciendo Minimo:[%d] y maximo [%d]", mini, max);
       /* }*//*else if(mini == 0)
        {
            LimInferior[count2] = mini;
            LimSuperior[count2] = max;
            minimos1[0] = 0;
        }*/
        ++count2;
    }
    int y;
    /*.................................................................*/
    for(y = 0; y<4; y++)
    {
        printf("\nRESULTADO-Minimo[%d]-Maximo[%d]", LimInferior[y], LimSuperior[y]);
    }
    /*.................................................................*/
    /******************************************************************/
    num_particiones = contador;
    printf("\nExisten [%d] minimos", contador);
    for(y = 0; y< contador; y++)
    {
        if(y == 0)
        {
            int libre = LimInferior[y];
            EspacioLibre[y] = libre;
            EspacioLibre_min[y] = 0;
            EspacioLibre_max[y] = LimInferior[y];
        }else
        {
            int libre = LimInferior[y] - LimSuperior[y-1];
            EspacioLibre[y] = libre;
            EspacioLibre_min[y] = LimSuperior[y-1];
            EspacioLibre_max[y] = LimInferior[y];
        }
    }
    EspacioLibre[contador] = mbr_p->mbr_tamano - LimSuperior[contador-1];
    EspacioLibre_min[contador] = LimSuperior[contador-1];
    EspacioLibre_max[contador] = mbr_p->mbr_tamano;
    printf("\ntamano del disco:%d", mbr_p->mbr_tamano);
}

void particionExendida(int valor_size)
{
    setMinimos();
    /*------------------PARTICION EXTENDIDA---------------------*/
    int i;
    int s_flag = 0;
    int created = 0;
    for(i = 0; i< 5; i++)
    {
        if(valor_size <= EspacioLibre[i])
        {
            int a;
            for(a = 0; a<4; a++)
            {
                if(mbr_p->mbr_partition[a].part_status == '0' && created == 0)
                {
                    int namesize = tamano3(nombre_fdisk);
                    if(namesize <= 16)
                    {
                        int c;
                        for(c = 0; c < namesize; c++)
                        {
                            mbr_p->mbr_partition[a].part_name[c] = *(nombre_fdisk + c);
                        }
                        mbr_p->mbr_partition[a].part_status = '1';
                        mbr_p->mbr_partition[a].part_type = 'e';
                        mbr_p->mbr_partition[a].part_size = valor_size;
                        mbr_p->mbr_partition[a].part_start = EspacioLibre_min[i];
                        printf("\nEsta particion inicia en: %d", EspacioLibre_min[i]);
                        printf("\nTamano de la particion: %d", valor_size);
                        FILE *file = fopen(val_direccion, "rb+");
                        size_t tam = sizeof(struct mbr_DISCO);
                        printf("\nVALOR EN BYTES[%zu]\n", tam);
                        if(file != NULL)
                        {
                            fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                            fclose(file);
                            printf("\nParticion primaria actualizada[extendida].\n");
                            s_flag = 1;
                        }
                        struct ebr_extended *ebr_inicial = malloc(sizeof(struct ebr_extended));
                        ebr_inicial->part_status = '0';
                        /*ebr_inicial->part_fit='f';*/
                        ebr_inicial->part_start= EspacioLibre_min[i];
                        ebr_inicial->part_size = 0;
                        ebr_inicial->part_next = -1;
                        int byte_pos = EspacioLibre_min[i] + 140;
                        printf("\nEscribiendo EBR en: %d\n", byte_pos);
                        FILE *file_ext = fopen(val_direccion, "rb+");
                        fseek(file_ext, byte_pos ,SEEK_SET);
                        created = 1;
                        if (file_ext != NULL) {
                            fwrite(ebr_inicial, sizeof(struct ebr_extended), 1, file_ext);
                            fclose(file_ext);
                            created = 1;
                        }
                    }else{ printf("ERROR: nombre muy extenso para la particion.");}
                }
            }
        }
    }
    if(s_flag == 0)
    {
        printf("\n:(\n");
    }
    if(created == 1)
    {
        printf("\nParticion extendida creada exitosamente.\n");
    }
    /*---------------------------------------------------------------------------*/
}

int cargarArchivoDisco()
{
    printf("\ncargarArchivoDisco\n");

    mbr_DISCO *primerDisco = malloc(sizeof(mbr_DISCO));
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fread(primerDisco, sizeof(struct mbr_DISCO), 1, file);
        fclose(file);
        mbr_p = primerDisco;
        printf("\Disco [%d] cargado.", mbr_p->mbr_disk_signature);
        load_disk_flag = 1;
    }else
    {
        load_disk_flag = 0;
        printf("ERROR: No se encontró el disco");
    }
    /*primerDisco->mbr_tamano = 10 * 1024 * 1024;
    primerDisco->mbr_partition[0].part_status = 1;
    primerDisco->mbr_partition[0].part_status = 1;
    FILE * file= fopen("disc.dsk", "wb");
    if (file != NULL) {
        fwrite(primerDisco, sizeof(struct mbr_DISCO), 1, file);
        fclose(file);
    }

    mbr_DISCO *primerDiscoleido = malloc(sizeof(struct mbr_DISCO));
    FILE * file2= fopen("disco.dsk", "rb");
    if (file2 != NULL) {
        fread(primerDiscoleido, sizeof(struct mbr_DISCO), 1, file2);
        fclose(file2);
    }

    printf("\n*[%d]\n", primerDisco->mbr_disk_signature);
    /*int tamanoArchivo = filesize(val_direccion)-512;
    d_mbr.tamanoDisco = tamanoArchivo;
    if(tamanoArchivo == -513)
    {
        printf("Error, no se encontró el disco");
    }else
    {
        FILE *myfile;
        int i;
        unsigned char inicio[1];
        myfile = fopen(val_direccion, "r");
        for(i = 0; i < 1; i++){
            fread(&inicio[i], 1, 1, myfile);
        }
        printf("char[%c]", inicio[0]);
        if(inicio[0]=='\0')
        {
            printf("Disco vacío!\n");

        }else{
            printf("Disco lleno!\n");
            int paso2 = 0;
            if(val_type[0] == 'p')
            {
                t_bytes = 1024 * val_size -1;
            }
            else if(val_type[0] == 'e')
            {
                t_bytes = 1024 * 1024 * val_size -1;
            }
            else if(val_type[0] == 'l')
            {
                t_bytes = 1024 * 1024 * val_size -1;
            }
        }
    }*/
}

void crearParticion()
{
    /*leer disco*/
    cargarArchivoDisco();
    /*Carga exitosa del disco = 1, si no = 0*/
    if(load_disk_flag == 1)
    {
        /*Primera particion*/
        /*d_mbr.particiones = 1;
        d_mbr.particiones_logicas = 0;*/
        /*d_mbr.*/
        /*Escribir MBR*/
        int min_byte = 1024 * 1024 * 2 -1;
        int t_bytes = 0;
        /*printf("!!!%c!!!" ,val_unit[0]);*/
        if(val_unit[0] == 'k')
        {
            t_bytes = 1024 * val_size -1;
            /*printf("\nKiloBytes;");*/
        }
        else if(val_unit[0] == 'm')
        {
            t_bytes = 1024 * 1024 * val_size -1;
            /*printf("\nMegaBytes;");*/
        }
        /*printf("\n3[%d]", val_size);*/
        /*Convertir int a char*/
        char str_val[10];
        sprintf(str_val, "%d", t_bytes);
        /*--------------------------------*/
        if(t_bytes >= min_byte)
        {
            int primarias=0;
            int extendidas=0;
            primarias = getPrimarias();
            extendidas = getExtendidas();
            if(val_type[0] == 'p')
            {
                printf("\nParticion primaria seleccionada...\n");
                if(extendidas+primarias <4)
                {
                    printf("Particion primaria:[Primaria:%d][Extendida:%d]", primarias, extendidas);
                    if(primarias == 0 && extendidas == 0)
                    {
                        printf("\nprimera particion\n");
                        /*Es la primera*/
                        if(val_size <= mbr_p->mbr_tamano)
                        {
                            int namesize = tamano3(nombre_fdisk);
                            if(namesize <= 16)
                            {

                                mbr_p->mbr_partition[0].part_status = '1';
                                int c;
                                for(c = 0; c < namesize; c++)
                                {
                                    mbr_p->mbr_partition[0].part_name[c] = *(nombre_fdisk + c);
                                }
                                mbr_p->mbr_partition[0].part_type = 'p';
                                mbr_p->mbr_partition[0].part_fit = val_fit;
                                mbr_p->mbr_partition[0].part_start = 0;
                                mbr_p->mbr_partition[0].part_size = t_bytes;
                                printf("\nTamano:%d", t_bytes);
                                FILE *file = fopen(val_direccion, "rb+");
                                size_t tam = sizeof(struct mbr_DISCO);
                                printf("\nVALOR EN BYTES[%zu]\n", tam);
                                if(file != NULL)
                                {
                                    fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                                    fclose(file);
                                }
                            }
                            else
                            {
                                printf("ERROR: nombre de particion muy amplio");
                            }

                        }else
                        {
                            printf("ERROR: Excede la capacidad del disco");
                        }
                    }else
                    {
                        printf("\NO es la primera particion\n");
                        setMinimos();
                        Particionar(t_bytes);
                    }
                }
                else
                {
                    printf("ERROR: particion primaria[Primaria:%d][Extendida:%d]", primarias, extendidas);
                }
            }else if(val_type[0] == 'e')
            {
                printf("\nParticion extendida seleccionada...\n");
                if(extendidas == 0 && primarias < 4)
                {
                    particionExendida(t_bytes);
                }else
                {
                    printf("ERROR: particion extendida");
                }
            }else if(val_type[0] == 'l')
            {
                printf("\nParticion logica seleccionada...\n");
                if(extendidas == 1)
                {
                    particionLogica(t_bytes);
                    reporteLogicas();
                }else
                {
                    printf("ERROR: no hay particion extendida.");
                }
            }
        }
        else
        {
            printf("\nERROR: el tamano minimo de particionado es de %d:%d",min_byte, t_bytes );
        }
    }

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

void escrituraPrueba()
{
    FILE *fp = fopen("DISCO", "ab+");

    FILE *f = fopen("DISCO", "w");
    if (f == NULL)
    {
        printf("\nError");
    }else
    {
        /* print some text */
        const char *text = "0";
        fprintf(f, text);
        int mb = 1024 * 50 * 1024;
        int i = 0;
        FILE *pFile2;
        pFile2=fopen("DISCO", "a");
        for(i = 0; i < mb; i++)
        {
            fprintf(pFile2, text);
        }
    }

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
    /*printf("\nccccccccccccccccccc1\n");
    printf(respuesta);
    printf("\n-");*/
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
        t_bytes = 1024 * val_size -1;
    }
    else if(val_unit[0] == 'm')
    {
        t_bytes = 1024 * 1024 * val_size -1;
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
        printf("\nCreando archivo con carpeta recien creada...\n");
        printf(nombre);
        printf("\n....\n");
        char cero[1];
        cero[0] = '\0';
        char *ubicacion = concatenacion(val_direccion, nombre);
        FILE *fp = fopen(ubicacion, "wb");
        /*fwrite(cero , 1 , sizeof(cero) , fp );*/
        /*fseek(fp, t_bytes , SEEK_SET);*/
        /*fwrite(cero , 1 , sizeof(cero) , fp );*/
        int i;
        for(i=0; i< t_bytes; i++)
        {
            fputc('\0', fp);
        }
        fclose(fp);
        /*DECLARAR MBR*/
        mbr_DISCO *primerDisco = malloc(sizeof(mbr_DISCO));
        /*----------------------------------------*/
        /*FECHA*/
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        primerDisco->mbr_fecha_creacion = t;
        /*strftime(primerDisco->mbr_fecha_creacion, sizeof(primerDisco->mbr_fecha_creacion), "%c", tm);*/
        /*----------------------------------------*/
        /*IDENTIFICADOR UNICO*/
        int signature = rand();
        primerDisco->mbr_disk_signature = signature;
        /*----------------------------------------*/
        /*TAMANO*/
        primerDisco->mbr_tamano = t_bytes;
        /*----------------------------------------*/
        for(i = 0; i<4; i++)
        {
            primerDisco->mbr_partition[i].part_status = '0';
        }
        FILE *file = fopen(ubicacion, "rb+");
        size_t tam = sizeof(struct mbr_DISCO);
        printf("\nVALOR EN BYTES[%zu]\n", tam);
        if(file != NULL)
        {
            fwrite(primerDisco, sizeof(struct mbr_DISCO), 1, file);
            fclose(file);
        }
    }else
    {
        printf("[Ya fue creada la carpeta]");
        printf("\nCreando archivo...\n");
        printf(nombre);
        printf("\n....\n");
        char cero[1];
        cero[0] = '\0';
        char *ubicacion = concatenacion(val_direccion, nombre);
        FILE *fp = fopen(ubicacion, "wb");
        int i;
        for(i=0; i< t_bytes; i++)
        {
            fputc('\0', fp);
        }
        /*fwrite(cero , 1 , sizeof(cero) , fp );*/
        /*fseek(fp, t_bytes , SEEK_SET);*/
        /*fwrite(cero , 1 , sizeof(cero) , fp );*/
        /*fputc('\0', fp);*/
        fclose(fp);
        /*DECLARAR MBR*/
        mbr_DISCO *primerDisco = malloc(sizeof(mbr_DISCO));
        /*----------------------------------------*/
        /*FECHA*/
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        primerDisco->mbr_fecha_creacion = t;
        /*strftime(primerDisco->mbr_fecha_creacion, sizeof(primerDisco->mbr_fecha_creacion), "%c", tm);*/
        /*----------------------------------------*/
        /*IDENTIFICADOR UNICO*/
        int signature = rand();
        primerDisco->mbr_disk_signature = signature;
        /*----------------------------------------*/
        /*TAMANO*/
        primerDisco->mbr_tamano = t_bytes;
        /*----------------------------------------*/
        for(i = 0; i<4; i++)
        {
            primerDisco->mbr_partition[i].part_status = '0';
        }
        FILE *file = fopen(ubicacion, "rb+");
        size_t tam = sizeof(struct mbr_DISCO);
        printf("\nVALOR EN BYTES[%zu]\n", tam);
        if(file != NULL)
        {
            fwrite(primerDisco, sizeof(struct mbr_DISCO), 1, file);
            fclose(file);
        }
    }
}

void eliminarDisco()
{
    int status;
    printf("\n¿Desea eliminar el archivo? si[s] no[n]\n");
    char decision[10];
    fgets(decision, sizeof(decision), stdin);
    printf(decision);
    char *si = "s\n";
    if(strcmp(si, decision)==0)
    {

        status = remove(val_direccion);
        if(status==0)
        {
            printf("\nArchivo eliminado correctamente\n");
        }else
        {
            printf("\nError al eliminar el archivo\n");
        }
    }
    else
    {
        printf("\nNada fue eliminado\n");
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
    /*printf("\ngetValorEntero\n");*/
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
    /*printf("lllllll[%s]",valor, "\n");*/
    int tamanio = tamano2(comando);
    for(i = 0; *(valor+i) != '\0' && *(valor+i) != NULL; i++){}
    /*printf("\ntamano\n");
    printf("[%d]\n", i);*/
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
    printf("\n***getValorCadena***\n");
    printf(respuesta);
    printf("\n********************\n");
    return respuesta;
}
char *getValorDisco(char *comando)/*Enviar comando completo, devuelve el valor de la cadena*/
{
    char** ArregloComando = str_split(comando, ':');
    char* valor = *(ArregloComando +1);
    int i = 0;
    /*printf("lllllll[%s]",valor, "\n");*/
    int tamanio = tamano2(comando);
    for(i = 0; *(valor+i) != '\0' && *(valor+i) != NULL; i++){}
    /*printf("\ntamano\n");
    printf("[%d]\n", i);*/
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
    printf("\n__getValorDisco__\n");
    printf(respuesta);
    printf("\n_________________");
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

    char *fdisk = "fdisk";
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
            val_size = getValorEntero(*(entradaTotal + posicion));
            printf("2[%d]", val_size);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, unit)==0)
        {
            bool_unit = 1;
            char* unidad = getTexto(*(entradaTotal + posicion));
            val_unit[0] = unidad[0];
            printf("%c\n", val_unit[0]);
            /*printf("\nEstado = 3\n");*/
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
            printf(entradaUnica);
            val_type[0] = *(entradaUnica + 6);

            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, name)==0)
        {
            bool_name = 1;
            nombre = getValorDisco(*(entradaTotal + posicion));
            nombre_fdisk = getValorCadena(*(entradaTotal + posicion));
            printf("\n----\n");printf(nombre_fdisk);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, fit)==0)
        {
            bool_fit = 1;
            printf("%c", *(entradaUnica + 5));
            if(*(entradaUnica + 5) == 'b')
            {
                val_fit = 'b';
            }else if(*(entradaUnica + 5) == 'f')
            {
                val_fit = 'f';
            }else if(*(entradaUnica + 5) == 'w')
            {
                val_fit = 'w';
            }
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
            val_add = getValorEntero(*(entradaTotal + posicion));
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, rmdisk)==0)
        {
            bool_rmdisk = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, fdisk)==0)
        {
            bool_fdisk = 1;
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
    }else if(bool_rmdisk == 1)
    {
        eliminarDisco();
    }else if(bool_fdisk == 1)
    {
        if(bool_sizee==1 && bool_unit==1 && bool_path==1 && bool_name==1)
        {
            crearParticion();
        }else
        {
            printf("Error, comando no reconocido.");
        }
    }
}

int main()
{

    srand(time(NULL));
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
        /*printf("Se ingresó:[%s]\n\n", entrada);*/
        tokens = str_split(entrada, ' ');
        /*printf("Despues:[%s]\n\n", entrada);*/
        tamanoArreglo = tamano(tokens);
        printf("\nIniciando el automata...\n");
        automata(tokens, *(tokens + 0), 0);
        master_Driver();
    /*}*/
    /*-------------------------------------------------------------*/
    /*if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("Ultima revision, palabra [%d]=[%s]\n", i, *(tokens + i));
        }
        printf("\n");
    }*/
    return 0;
}
