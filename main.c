#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>

    int estado;
    int tamanoArreglo;
    char** arreglo;
    int particionado_disco_actual = 0;
    char *pathDisco;
    int load_disk_flag = 0;
    int bytesize_mbr = 136;
    int num_particiones = 0;
    /****************************/
    int minimos1[4];
    int superior[4];
    char tipos[4];
    char nombres[4][16];
    /****************************/
    int LimSuperior[4];
    int LimInferior[4];
    char LimNombres[4][16];
    char LimTipos[4];
    /****************************/
    int EspacioLibre[5];
    int EspacioLibre_min[5];
    int EspacioLibre_max[5];
    char EspacioLibre_nombres[4][16];
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

    int bool_mount = 0;
    int bool_unmount = 0;

    int bool_rep = 0;
    int bool_id = 0;

    int bool_exec = 0;

    int bool_bs = 0;
    /*--------------------rep -id::vda0 -path::"/home/luis/Escritorio/rep1/rep2/rep3/reporte1.png" -name::mbr --------*/
    /*     Banderas reservadas   */
    /*----------------------------*/
    int val_add = 0;
    int val_delete = 0;
    int val_size = 0;
    char *val_tipo_reporte ;/*0 = mbr,  1 = disk*/
    char val_fit;/*BF 1, FF 2, WF 3*/
    char val_unit[1] = "m";
    char* val_direccion;
    char* nombre;
    char* nombre_fdisk;
    char val_type[1] = "p";
    char* val_un_id;
    char *val_folderPath;
    /*----------------------------*/
    /*Valores reservados*/
    /*----------------------------*/
    /*________________________________*/
    /*TABLA DE PARTICIONES DEL SISTEMA*/
    /*________________________________*/


    char *fila1[4];
    char *fila2[4];
    char *fila3[4];
    char *fila4[4];

    char *path1[4];
    char *path2[4];
    char *path3[4];
    char *path4[4];

    char *part_name1[4];
    char *part_name2[4];
    char *part_name3[4];
    char *part_name4[4];

    /*____________________________*/
    /*____________________________*/
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
        char mbr_fecha_creacion[32];
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
                start = mbr_p->mbr_partition[i].part_start;
            }
        }
        printf("\nREPORTE LOGICAS-------------------------\nstart:%d\n", start);
        struct ebr_extended *ebr_actual=malloc(sizeof(struct ebr_extended));
        int status = 0;
        int c = 0;
        while(status != -1)
        {
            FILE * file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, start + 152 ,SEEK_SET);
                fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
                fclose(file);
            }
            status = ebr_actual->part_next;
            start = ebr_actual->part_next;
            char *name = ebr_actual->part_name;
            printf("\n[DATO]start: %d tamano: %d next:%d nombre:%s", ebr_actual->part_start, ebr_actual->part_size, status, name);
            c++;
            if(c == 25){break;}
        }
    }
}

void reporteMinimos()
{
    printf("\n--------------REPORTE MINIMOS---------------");
    int i;
    for(i = 0; i<4; i++)
    {
        char *imprimir = LimNombres[i];
        printf("\nLimiteInferior[%d], LimiteSuperior[%d], LimNombres[%s]", LimInferior[i], LimSuperior[i], imprimir);
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
                        mbr_p->mbr_partition[a].part_fit = val_fit;
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
        printf("\n ERROR :(\n");
    }else
    {
        printf("\n Particion primaria creada.\n");
    }
    /*---------------------------------------------------------------------------*/
}

void partition_Starter()
{
    int i;
    char *no = "no";
    for(i = 0; i<4; i++)
    {
        int j = 0;
        for(j = 0; j<4; j++)
        {
            if(i == 0)
            {
                fila1[j] = no;
            }else if(i ==1)
            {
                fila2[j] = no;

            }else if(i == 2)
            {
                fila3[j] = no;

            }else if(i == 3)
            {
                fila4[j] = no;

            }
        }
    }
    printf("\n Tabla de particiones cargada...");
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
    int default_start = mbr_p->mbr_partition[pos].part_start;
    int start =         mbr_p->mbr_partition[pos].part_start ;
    printf("\nParticion extendida, fisicamente inicia en: %d de tamano: %d", start+152, default_size);
    struct ebr_extended *ebr_actual=malloc(sizeof(struct ebr_extended));
    FILE * file= fopen(val_direccion, "rb");
    if (file != NULL) {
        fseek(file, start+152 ,SEEK_SET);
        fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
        fclose(file);
    }
    /******************************************************************************/
    printf("\n//%c, %d\n", ebr_actual->part_status, ebr_actual->part_next);
    if(ebr_actual->part_status == '0'&& ebr_actual->part_next == -1)
    {
        /***********************Es la primera lógica********************************/
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
                printf("\nActualizando fisicamente EBR en: %d", start+152);
                /**/
                FILE * file_update = fopen(val_direccion, "rb+");
                if (file_update != NULL) {
                    fseek(file_update, start+152 ,SEEK_SET);
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
        /************************************************************************************/
    }else
    {
        int confirmacion2 = 0;
        if(ebr_actual->part_status == '0'&& ebr_actual->part_next != -1)
        {
            printf("\nPrimer EBR vacio, pero existen mas particiones logicas");/*******************************/
            if(t_bytes <= (ebr_actual->part_next - default_start))
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

                    ebr_actual->part_status ='1';
                    /**/
                    printf("\nActualizando fisicamente EBR en: %d", default_start +152);
                    /**/
                    FILE * file_update = fopen(val_direccion, "rb+");
                    if (file_update != NULL) {
                        fseek(file_update, start+152 ,SEEK_SET);
                        fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                        fclose(file_update);
                        confirmacion = 1;
                    }

                }else
                {
                    printf("\nERROR:El nombre excede el limite.");
                }
            }
            /**************************************************************************************/
        }

        if(confirmacion2 == 0)
        {
            while(confirmacion2 == 0)
            {
                if(ebr_actual->part_next == -1 && t_bytes <= ((default_start + default_size) - (ebr_actual->part_start + ebr_actual->part_size)))
                {
                    confirmacion2 = 1;
                    printf("\nEs el ultimo EBR.");/************************************************************************/
                    printf("\nParticion:%d Espacio", t_bytes);
                    if(t_bytes <= ((default_start + default_size) - (ebr_actual->part_start + ebr_actual->part_size)))
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

                            /*start = ebr_nuevo->part_start;*/
                            printf("\nActualizando EBR en:%d su nuevo part_start: %d" , ebr_actual->part_start+152, ebr_actual->part_next);
                            FILE * file_update= fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start+152 ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                                printf("\n???");
                            }
                            printf("\nEscribiendo fisicamente ultimo EBR en:%d", ebr_nuevo->part_start);
                            FILE * file= fopen(val_direccion, "rb+");
                            if (file != NULL) {
                                fseek(file, ebr_nuevo->part_start+152 ,SEEK_SET);
                                fwrite(ebr_nuevo, sizeof(struct ebr_extended), 1, file);
                                fclose(file);
                                confirmacion = 1;
                            }
                        }else
                        {
                            printf("\nERROR:El nombre excede el limite.");
                        }
                    }/*************************************************************************************/
                }else
                {
                    printf("\nLogica en medio.");/*********************************************************/
                    int operacion = 0;
                    operacion = ebr_actual->part_next - (ebr_actual->part_start + ebr_actual->part_size);
                    if(t_bytes <= operacion)
                    {
                        confirmacion2 = 1;
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
                            ebr_nuevo->part_next = ebr_actual->part_next;
                            ebr_nuevo->part_size = t_bytes;
                            ebr_nuevo->part_start = ebr_actual->part_start + ebr_actual->part_size;
                            ebr_nuevo->part_status = '1';
                            ebr_actual->part_next = ebr_nuevo->part_start;

                            printf("\nEscribiendo en medio EBR en:%d", ebr_nuevo->part_start);

                            FILE * file_update= fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start+152 ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                            }
                            FILE * file= fopen(val_direccion, "rb+");
                            if (file != NULL) {
                                fseek(file, ebr_nuevo->part_start +152,SEEK_SET);
                                fwrite(ebr_nuevo, sizeof(struct ebr_extended), 1, file);
                                fclose(file);
                                confirmacion = 1;
                            }
                        }else
                        {
                            printf("\nERROR:El nombre excede el limite.");
                        }
                    }
                    /*********************************************************************************/
                }
                if(confirmacion2 == 0)
                {
                    if(ebr_actual->part_next != -1)
                    {
                        FILE * file5= fopen(val_direccion, "rb");
                        if (file5 != NULL) {
                            fseek(file5, ebr_actual->part_next+152 ,SEEK_SET);
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

void eliminarParticion()
{
    if(val_delete != -1)
    {
        printf("\n Desea eliminar la particion? si[s] no[n]\n");
        char entradaUsuario[10];
        fgets(entradaUsuario, sizeof(entradaUsuario), stdin);
        if(entradaUsuario[0]=='s' || entradaUsuario[0]=='S')
        {
            printf("\n Eliminando particion.");
            cargarArchivoDisco();
            int p_e = 0;
            /*Primero se va a buscar en las p/e*/
            int i;
            for(i = 0; i<4 ; i++)
            {
                if(mbr_p->mbr_partition[i].part_status == '1')
                {
                    char *comp = mbr_p->mbr_partition[i].part_name;
                    printf("\n(%s, %s)", comp, nombre_fdisk);
                    if(strcmp(nombre_fdisk, comp)==0)
                    {
                        printf("\n Encontrado");
                        p_e = 1;
                        char type = mbr_p->mbr_partition[i].part_type;
                        int start = mbr_p->mbr_partition[i].part_start;
                        int size = mbr_p->mbr_partition[i].part_size;
                        mbr_p->mbr_partition[i].part_fit='\0';
                        mbr_p->mbr_partition[i].part_name[0] = '\0';
                        mbr_p->mbr_partition[i].part_size = 0;
                        mbr_p->mbr_partition[i].part_start = 0;
                        mbr_p->mbr_partition[i].part_status = '0';
                        mbr_p->mbr_partition[i].part_type = '\0';
                        if(type == 'e' && val_delete == 1)
                        {
                            char cero[1];
                            cero[0] = '\0';
                            FILE *fp = fopen(val_direccion, "rb+");
                            fseek(fp, start, SEEK_SET);
                            int z;
                            for(z=0; z< size; z++)
                            {
                                fputc('\0', fp);
                            }
                            fclose(fp);

                        }
                        FILE * file_update = fopen(val_direccion, "rb+");
                        if (file_update != NULL) {
                            fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file_update);
                            fclose(file_update);
                        }
                    }
                }
            }/*****************************************************/
            /*----------------------------------------------------*/
            if(p_e == 0)
            {
                printf("\n No se encontro en las p/e, buscando en logicas");
                if(getExtendidas() == 1)
                {
                    int k;
                    int pos;
                    int primero = 0;
                    for(k = 0; k<4; k++)
                    {
                        if(mbr_p->mbr_partition[k].part_status == '1')
                        {
                            if(mbr_p->mbr_partition[k].part_type == 'e')
                            {
                                pos = k;
                                break;
                            }
                        }
                    }
                    int start = mbr_p->mbr_partition[pos].part_start;
                    int size = mbr_p->mbr_partition[pos].part_size;
                    struct ebr_extended *ebr_actual=malloc(sizeof(struct ebr_extended));
                    FILE * file= fopen(val_direccion, "rb");
                    if (file != NULL)
                    {
                        fseek(file, start + 152 ,SEEK_SET);
                        fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
                        fclose(file);
                    }
                    char *name = ebr_actual->part_name;
                    /*________________VERIFICANDO EL PRIMERO______________*/
                    if(strcmp(nombre_fdisk, name)==0)
                    {
                        printf("\n Es el primero");
                        primero = 1;
                        ebr_actual->part_fit ='\0';
                        ebr_actual->part_name[0]='\0';
                        ebr_actual->part_size = 0;
                        ebr_actual->part_status = '0';

                        FILE * file_update = fopen(val_direccion, "rb+");
                        if (file_update != NULL) {
                            fseek(file_update, start+152 ,SEEK_SET);
                            fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                            fclose(file_update);
                        }
                        printf("\n Primera particion logica eliminada.");
                    }
                    /*____________________________________________________*/
                    /*________________VERIFICANDO SI FUE ELIMINADO EL PRIMERO______________*/
                    if(primero == 0 && ebr_actual->part_next != -1)
                    {
                        /************************INICIALIZANDO*********************************/
                        struct ebr_extended *ebr_anterior=malloc(sizeof(struct ebr_extended));
                        FILE * file_anterior= fopen(val_direccion, "rb");
                        if (file_anterior != NULL)
                        {
                            fseek(file_anterior, start + 152 ,SEEK_SET);
                            fread(ebr_anterior, sizeof(struct ebr_extended), 1, file_anterior);
                            fclose(file_anterior);
                        }
                        FILE * file_presente= fopen(val_direccion, "rb");
                        if (file_presente != NULL)
                        {
                            fseek(file_presente, ebr_actual->part_next + 152 ,SEEK_SET);
                            fread(ebr_actual, sizeof(struct ebr_extended), 1, file_presente);
                            fclose(file_presente);
                        }
                        /*********************************************************************/
                        int status=ebr_actual->part_next;
                        while(primero == 0)
                        {
                            name = ebr_actual->part_name;
                            printf("\n (%s, %s)", name, nombre_fdisk);
                            if(strcmp(nombre_fdisk, name)==0)
                            {
                                printf("\n Encontrado!........................");
                                primero = 1;
                                int extremo = 0;
                                if(ebr_actual->part_next == -1)
                                {
                                    printf("\n Al final");
                                    ebr_anterior->part_next = -1;
                                    FILE * file_update = fopen(val_direccion, "rb+");
                                    if (file_update != NULL) {
                                            printf("\n Actualizando anterior en: %d", ebr_anterior->part_start+152 );
                                        fseek(file_update, ebr_anterior->part_start+152 ,SEEK_SET);
                                        fwrite(ebr_anterior, sizeof(struct ebr_extended), 1, file_update);
                                        fclose(file_update);
                                    }
                                    if(val_delete == 1)
                                    {
                                        char cero[1];
                                        cero[0] = '\0';
                                        FILE *fp = fopen(val_direccion, "rb+");
                                        fseek(fp, ebr_actual->part_start+152 , SEEK_SET);
                                        int z;
                                        int q = ebr_actual->part_size;
                                        for(z=0; z< q; z++)
                                        {
                                            fputc('\0', fp);
                                        }
                                        fclose(fp);
                                    }
                                }else
                                {
                                    printf("\n En medio........................");
                                    extremo = ebr_actual->part_next;
                                    ebr_anterior->part_next = extremo;
                                    ebr_actual->part_fit ='\0';
                                    ebr_actual->part_name[0]='\0';
                                    ebr_actual->part_size = 0;
                                    ebr_actual->part_status = '0';
                                    FILE * file_update = fopen(val_direccion, "rb+");
                                    if (file_update != NULL) {
                                            printf("\n Actualizando anterior en: %d", ebr_anterior->part_start+152 );
                                        fseek(file_update, ebr_anterior->part_start+152 ,SEEK_SET);
                                        fwrite(ebr_anterior, sizeof(struct ebr_extended), 1, file_update);
                                        fclose(file_update);
                                    }
                                    if(val_delete == 1)
                                    {
                                        char cero[1];
                                        cero[0] = '\0';
                                        FILE *fp = fopen(val_direccion, "rb+");
                                        fseek(fp, ebr_actual->part_start+152 , SEEK_SET);
                                        int z;
                                        int q = ebr_actual->part_size;
                                        for(z=0; z< q; z++)
                                        {
                                            fputc('\0', fp);
                                        }
                                        fclose(fp);
                                    }
                                }
                                printf("\n Particion logica eliminada.");
                            }
                            if(status == -1)
                            {
                                primero = 1;
                            }else
                            {
                                /*.....................DESPLAZANDO PUNTERO.................*/
                                FILE * file11= fopen(val_direccion, "rb");
                                if (file11 != NULL)
                                {
                                    fseek(file11, ebr_actual->part_next + 152 ,SEEK_SET);
                                    fread(ebr_actual, sizeof(struct ebr_extended), 1, file11);
                                    fclose(file11);
                                }
                                FILE * file22= fopen(val_direccion, "rb");
                                if (file22 != NULL)
                                {
                                    fseek(file22, ebr_anterior->part_next + 152 ,SEEK_SET);
                                    fread(ebr_anterior, sizeof(struct ebr_extended), 1, file22);
                                    fclose(file22);
                                }
                                status = ebr_actual->part_next;
                                /*...........................................................*/
                            }
                        }
                    }
                    /*_____________________________________________________________________*/

                }else
                {printf("\n ERROR: no se encontro la particion. ");}
            }
            reporteLogicas();
            setMinimos();
            reporteMinimos();
        }else
        {
            printf("\n Particion intacta.");
        }
    }else
    {
        printf("ERROR: comando no reconocido");
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
        nombres[i][0] = '\0';
        /*-------------------------------------*/
        /*aqui van ordenadas las respuestas*/
        LimInferior[i] = 0;
        LimSuperior[i] = 0;
        LimNombres[i][0] = '\0';
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
            tipos[contador] = mbr_p->mbr_partition[i].part_type;
            int j;
            for(j = 0; j < 16; j++)
            {
                nombres[contador][j] = mbr_p->mbr_partition[i].part_name[j];
            }
            printf("\nHay una particion que inicia en: %d y de tamano: %d nombre:%c %c %c\n", mbr_p->mbr_partition[i].part_start, mbr_p->mbr_partition[i].part_size, nombres[i][0], nombres[i][1], nombres[i][2]);
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
    char tipi = tipos[0];
    char pivote_nombre[16];
    int u;
    for(u = 0; u< 16; u++)
    {
        pivote_nombre[u] = nombres[0][u];
    }
    /*------*/
    if(mini == 0)
    {
        LimInferior[count2] = mini;
        LimSuperior[count2] = max;
        LimTipos[count2] = tipi;
        for(u = 0; u<16; u++)
        {
            LimNombres[count2][u] = pivote_nombre[u];
        }
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
                tipi = tipos[y];
                int j;
                for(j = 0; j < 16; j++)
                {
                    pivote_nombre[j] = nombres[y][j];
                }
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
                tipi = nombres[y];
                for(u =0; u<16; u++)
                {
                    pivote_nombre[u] = nombres[y][u];
                }
                pos = y;
                printf("\nfor1 %d < %d - %d != 0\n", minimos1[y], mini, minimos1[y]);
            }
        }
        /*if(flag ==1)
        {*/
            LimInferior[count2] = mini;
            LimSuperior[count2] = max;
            LimTipos[count2] = tipi;
            for(u = 0; u<16; u++)
            {
                LimNombres[count2][u] = pivote_nombre[u];
            }
            minimos1[pos] = 0;
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
        char *imprimir = LimNombres[y];
        printf("\nRESULTADO-Minimo[%d]-Maximo[%d] %s", LimInferior[y], LimSuperior[y], imprimir);
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

void modificarParticion()
{
    cargarArchivoDisco();
    setMinimos();
    printf("\nModificando particion...\nBuscando en primarias y extendidas");
    /************************************************************************/
    val_unit[0] == 'k';
    if(val_unit[0] == 'k' || val_unit[0] == 'K')
    {
        val_add = 1024 * val_add ;
    }
    else if(val_unit[0] == 'm' || val_unit[0] == 'M')
    {
        val_add = 1024 * 1024 * val_add ;
    }
    else if(val_unit[0] == 'b' || val_unit[0] == 'B')
    {

    }

    /************************************************************************/
    int i;
    int pos = 0;
    int pe_encontrada = 0;
    for(i=0; i< 4; i++)
    {
        char *comp = LimNombres[i];
        printf("\n(%s , %s)", nombre_fdisk, comp);
        if(strcmp(nombre_fdisk, comp)==0)
        {
            pos = i;
            pe_encontrada = 1;
            break;
        }
    }

    if(pe_encontrada == 1)
    {
        if(pos+1 != num_particiones)
        {
            if(val_add > 0)
            {
                printf("\nEn medio expansion");
                int tam_disponible = LimInferior[pos+1] - LimSuperior[pos];
                printf("\nTam disponible: %d", tam_disponible);
                if(val_add <= tam_disponible)
                {
                    for(i = 0; i<4; i++)
                    {
                        if(mbr_p->mbr_partition[i].part_start == LimInferior[pos] && LimSuperior[pos] != 0)
                        {
                            printf("\nTamano anterior: %d Nuevo tamano: %d ",mbr_p->mbr_tamano, mbr_p->mbr_tamano + val_add);
                            mbr_p->mbr_partition[i].part_size = mbr_p->mbr_partition[i].part_size + val_add;
                            FILE *file = fopen(val_direccion, "rb+");
                            if(file != NULL)
                            {
                                fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                                fclose(file);
                                printf("\nParticion p/e actualizada[expansion]1.\n");
                            }
                        }
                    }
                }else
                {
                    printf("\nERROR: no hay suficiente espacio.");
                }
            }else if (val_add < 0)
            {
                printf("\nEn medio reduccion: %d", val_add);
                int resultado = (LimSuperior[pos]-LimInferior[pos]) + val_add;
                if(resultado >= (1024 * 1024 * 2))
                {
                    for(i = 0; i<4; i++)
                    {
                        if(mbr_p->mbr_partition[i].part_start == LimInferior[pos] && LimSuperior[pos] != 0)
                        {
                            mbr_p->mbr_partition[i].part_size = resultado;
                            printf("\nNuevo tamano: %d Distancia:", resultado);
                            FILE *file = fopen(val_direccion, "rb+");
                            if(file != NULL)
                            {
                                fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                                fclose(file);
                                printf("\nParticion p/e actualizada[reduccion]1.\n");
                            }
                        }
                    }
                }
                else
                {
                    printf("\nNo se puede reducir tanto espacio.");
                }
            }
        }else if (pos + 1 == num_particiones)
        {
            printf("\nAl final");
            if(val_add > 0)
            {
                printf("\nExpansion");
                int libre = mbr_p->mbr_tamano - LimSuperior[pos];
                if(val_add <= libre)
                {
                    for(i = 0; i<4; i++)
                    {
                        if(mbr_p->mbr_partition[i].part_start == LimInferior[pos] && LimSuperior[pos] != 0)
                        {
                            printf("\nTamano anterior: %d Nuevo tamano: %d",mbr_p->mbr_tamano, mbr_p->mbr_tamano + val_add );
                            mbr_p->mbr_partition[i].part_size = mbr_p->mbr_partition[i].part_size + val_add;
                            FILE *file = fopen(val_direccion, "rb+");
                            if(file != NULL)
                            {
                                fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                                fclose(file);
                                printf("\nParticion p/e actualizada[expansion]2.\n");
                            }
                        }
                    }
                }else
                {
                    printf("\nERROR: no hay tamano suficiente.");
                }
            }else
            {
                printf("\nReduccion: %d", val_add);
                int resultado = (LimSuperior[pos] - LimInferior[pos]) + val_add;
                if(resultado >= (1024 * 1024 * 2))
                {
                    for(i = 0; i < 4; i++)
                    {
                        if(mbr_p->mbr_partition[i].part_start == LimInferior[pos] && LimSuperior[pos] != 0)
                        {
                            mbr_p->mbr_partition[i].part_size = resultado;
                            printf("\nNuevo tamano: %d", resultado);
                            FILE *file = fopen(val_direccion, "rb+");
                            if(file != NULL)
                            {
                                fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                                fclose(file);
                                printf("\nParticion p/e actualizada[reduccion].\n");
                            }
                        }
                    }
                }else
                {
                    printf("\nERROR: No se puede reducir tanto espacio");
                }
            }
        }
    }else/**************************************************************************************/
    {
        /*Buscar en las logicas*/
        printf("\nBuscando en logicas...");/*****************************************************/
        if(getExtendidas() == 1)
        {
            int k;
            int default_size = 0;
            int default_start = 0;
            for(k = 0; k < 4; k++)
            {
                if(mbr_p->mbr_partition[k].part_type == 'e')
                {
                    pos = k;
                    default_size=mbr_p->mbr_partition[k].part_size;
                    default_start=mbr_p->mbr_partition[k].part_start;
                    break;
                }
            }
            struct ebr_extended *ebr_actual = malloc(sizeof(struct ebr_extended));
            FILE * file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, default_start+152 ,SEEK_SET);
                fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
                fclose(file);
            }
            int comprobante = 0;
            char *name;
            if(ebr_actual->part_status == '1')
            {
                name = ebr_actual->part_name;
                if(strcmp(nombre_fdisk, name)==0)
                {
                    comprobante = 1;
                    printf("\nEncontrado en el primero");
                    if(val_add>0)
                    {
                        printf("\nAumentando");
                        if(val_add <= (ebr_actual->part_next - (ebr_actual->part_start + ebr_actual->part_size)))
                        {
                            ebr_actual->part_size = ebr_actual->part_size + val_add;
                            FILE * file_update = fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start+152 ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                            }
                        }else {printf("\ ERROR: no se pudo realizar el aumento");}

                    }else if(val_add < 0)
                    {
                       printf("\nDisminuyendo");
                       int resultado = ebr_actual->part_size + val_add;
                       if(resultado >= (1024 * 1024 * 2))
                       {
                           ebr_actual->part_size = resultado;
                           FILE * file_update = fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start+152 ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                            }
                       }else{printf("\n ERROR: no se puede reducir: %d / %d" , resultado,1024*1024*2 );}
                    }
                }
            }
            if(comprobante ==0 && ebr_actual->part_next != -1)
            {
                while(comprobante == 0 )
                {
                    name = ebr_actual->part_name;
                    printf("\n(%s, %s)", name, nombre_fdisk);
                    if(strcmp(nombre_fdisk, name)==0)
                    {
                            comprobante = 1;
                            printf("\n Encontrado!");
                            if(val_add > 0)
                            {
                                printf("\n Aumento");
                                int extremo;
                                if(ebr_actual->part_next == -1)
                                {
                                    extremo = default_size;
                                }else
                                {
                                    extremo = ebr_actual->part_next;
                                }
                                if(val_add <= extremo - (ebr_actual->part_start + ebr_actual->part_size))
                                {
                                    ebr_actual->part_size = ebr_actual->part_size + val_add;
                                    FILE * file_update = fopen(val_direccion, "rb+");
                                    if (file_update != NULL) {
                                        fseek(file_update, ebr_actual->part_start+152 ,SEEK_SET);
                                        fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                        fclose(file_update);
                                    }
                                }else
                                {
                                    printf("\n ERROR: no se puede aumentar el tamano");
                                }
                            }else if(val_add < 0)
                            {
                                printf("\n Reduccion");
                                int resultado = ebr_actual->part_size + val_add;
                                if(resultado >= 1024*1024*2)
                                {
                                    ebr_actual->part_size = resultado;
                                    FILE * file_update = fopen(val_direccion, "rb+");
                                    if (file_update != NULL) {
                                        fseek(file_update, ebr_actual->part_start+152 ,SEEK_SET);
                                        fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                        fclose(file_update);
                                    }
                                }else{printf("\n ERROR: no se pudo reducir");}
                            }
                    }
                    if(ebr_actual->part_next == -1)
                    {
                        comprobante = 1;
                    }else
                    {
                        FILE * file= fopen(val_direccion, "rb");
                        if (file != NULL)
                        {
                            fseek(file, ebr_actual->part_next+152 ,SEEK_SET);
                            fread(ebr_actual, sizeof(struct ebr_extended), 1, file);
                            fclose(file);
                        }
                    }
                    /*printf("\n While");*/
                }
            }
        }
        else
        {
            printf("\nERROR");
        }
    }
    reporteMinimos();
    reporteLogicas();
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
                        mbr_p->mbr_partition[a].part_fit = val_fit;
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
                        printf("\nEBR inicia en: %d", ebr_inicial->part_start);
                        ebr_inicial->part_size = 0;
                        ebr_inicial->part_next = -1;
                        int byte_pos = EspacioLibre_min[i] + 152;
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
}

void cargarArchivoDisco2()
{
    printf("\n Cargando el disco de la particion seleccionada: %s.", val_un_id);
    char *nuevoPath;
    char cabecera_array[5] = "abcd";
    printf("\n Buscando disco de la particion.");
    int rango = 0;
    char letra = *(val_un_id +2);
    int fila = *(val_un_id +3) - '0';
    if(letra == 'a' || letra == 'b' || letra == 'c' || letra == 'd')
    {
        rango = 1;
    }
    if(fila >= 0 && fila < 4)
    {
        rango = rango +1;
    }else
    {
        rango = 0;
    }
    int x = 0;
    if(rango == 2)
    {
        if(letra == 'a')
        {
            x = 0;
        }else if(letra == 'b')
        {
            x = 1;
        }else if(letra == 'c')
        {
            x = 2;
        }else if(letra == 'd')
        {
            x = 3;
        }
        printf("\nx: %d y:%d", x, fila);
        if(fila == 0)
        {
            nuevoPath =path1[x];
        }
         else if(fila == 1)
        {
            nuevoPath =path2[x];
        }
        else if(fila == 2)
        {
            nuevoPath =path3[x];
        }
        else if(fila == 3)
        {
            nuevoPath =path4[x];
        }
    }
    val_direccion = nuevoPath;
    printf("\n Nuevo path= %s", val_direccion);
    cargarArchivoDisco();
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
        int min_byte = 1024 * 1024 * 2 ;
        int t_bytes = 0;
        /*printf("!!!%c!!!" ,val_unit[0]);*/
        val_unit[0] = 'k';
        if(val_unit[0] == 'k' || val_unit[0] == 'K')
        {
            t_bytes = 1024 * val_size ;
            /*printf("\nKiloBytes;");*/
        }
        else if(val_unit[0] == 'm' || val_unit[0] == 'M')
        {
            t_bytes = 1024 * 1024 * val_size ;
            /*printf("\nMegaBytes;");*/
        }
        else if(val_unit[0] == 'b' || val_unit[0] == 'B')
        {

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
                if(extendidas+primarias < 4)
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
                                printf("\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1EL FIT DEL DISCO ES: %c", val_fit);
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
                    printf("ERROR1: particion primaria[Primaria:%d][Extendida:%d]", primarias, extendidas);
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
    return i+1;
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

char *arregloDireccion2(char *comando)
{
    int tamanio = contadorDiagonales(comando);
    int tamanioTotal = tamano3(comando);
    char *respuesta;
    respuesta =(char*)malloc(tamanio +tamanioTotal);
    int d;
    int count = 0;
    int bul = 0;
    for(d = 0; d < tamanio +tamanioTotal -1; d++)
    {
        if(*(comando+d) == '/')
        {
            if(bul == 0)
            {
                bul = 1;
                *(respuesta + count) = *(comando +d);
                ++count;
            }else
            {
                *(respuesta + count) = ',';
                ++count;
                *(respuesta + count) = *(comando +d);
                ++count;
            }
        }
        else
        {
            *(respuesta + count) = *(comando +d);
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
        return *(arregloDirecciones +0);
    }else
    {
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
        respuesta =(char*)malloc(tamanio+1);
        for(i = 0; i < direcciones; i++)
        {
            char *actual = *(arregloDirecciones + i);
            for(j = 0; j< tamanos[i]; j++)
            {
                if(*(actual+j) != '\0')
                {
                    *(respuesta + count) = *(actual+j);
                    ++count;
                }
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

void asignar_FolderPath(char *extension, char *ubicacion_original)
{
    char *direccion = val_direccion;
    /*printf("\n asignar_FolderPath: %s", direccion);*/
    int tam = tamano3(val_direccion);
    int i;
    int size = 0;
    for(i = tam-1; i< tamano3; i--)
    {
        if(*(val_direccion + i) == '/')
        {
            break;
        }else
        {
            size = size +1;
        }
    }
    size = size;
    char respuesta_archivo[size];
    char respuesta_carpeta[tam -size];
    int cont = 0;
    /*printf("\n resta: %d", size);*/
    for(i = tam-size; i< tam; i++)
    {
        respuesta_archivo[cont] = *(val_direccion +i);
        /*printf("\n %c: ", *(val_direccion +i));*/
        ++cont;
    }
    for(i = 0; i < tam-size-1; i++)
    {
        respuesta_carpeta[i] = *(val_direccion +i);

    }
    respuesta_carpeta[tam-size-1] = '\0';
    val_folderPath = respuesta_carpeta;
    /*printf("\n ValFolderPath: %s", val_folderPath);*/
    crearCarpetas(respuesta_carpeta, extension, ubicacion_original);
}

void crearCarpetas(char val_folderPath1[], char *extension,char *ubicacion_original)
{
    char *direccionActual = val_folderPath1;
    struct stat st = {0};
    if(stat(val_folderPath1, &st) == -1)
    {
        printf("\nCreando carpetas[%s]...\n", val_folderPath1);
        char *paraSplit;
        paraSplit = arregloDireccion2(val_folderPath1);
        char** carpetas = str_split(paraSplit ,',');
        int carpetas_count = tamano(carpetas);
        int b = 0;
        while(b < carpetas_count)
        {
            direccionActual = concatenarDireccion(carpetas, b+1);
            printf("[[%d]", b);
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
    }
    char *char_mbr = "mbr";
    char *char_disk = "disk";
    printf("[char: %s, usuario: %s]", char_mbr, val_tipo_reporte);
    if(strcmp(val_tipo_reporte, char_disk)==0)
    {
        reporte_disk(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_mbr)==0)
    {
        reporte_mbr(direccionActual, extension, ubicacion_original);
    }
}

void reporte_mbr(char *direccion, char *extension, char *ubicacion_original)
{
    printf("\n REPORTE MBR\n Extension: %s", extension);
    char *jpg = "jpg";
    char *png = "png";
    char *extensionDot = "dot -Tpng ";
    char *output = " -o ";
    if(strcmp(extension, jpg)==0)
    {
        extensionDot = "dot -Tjpg ";
    }if(strcmp(extension, png)==0)
    {
        extensionDot = "dot -Tpng ";
    }
    cargarArchivoDisco2();
    char nada[1] ;
    nada[0] = ' ';
    char *nombreArchivo = "/reportembr.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);
    /*printf("\n [%s]", ubicacion);*/
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
    }
    char *cab1 ="digraph G {\n";
    char *cab2="node [shape=plaintext]\n";
    char *cab3="a [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\" 0 \" WIDTH=\"50%\">\n";
    char *endf ="</table>>];}";
    char *mbrTop = "MBR";
    char *ebrTop = "EBR";
    char *ope_tr = "\n<tr>";
    char *clo_tr = "</tr>\n";
    char *ope_td = "\n<td WIDTH=\"100\" HEIGHT=\"50\">";
    char *clo_td = "</td>\n";
    char intTOchar[20];
    sprintf(intTOchar, "%d", mbr_p->mbr_tamano);
    char *fecha = mbr_p->mbr_fecha_creacion;
    char *palabra;
    FILE *lapiz = fopen(ubicacion, "a");
    if(lapiz != NULL)
    {
        fprintf(lapiz, "%s", cab1);
        fprintf(lapiz, "%s", cab2);
        fprintf(lapiz, "%s", cab3);
        fprintf(lapiz, "%s", ope_tr);
        fprintf(lapiz, "%s", ope_td);
        fprintf(lapiz, "%s", mbrTop);
        fprintf(lapiz, "%s", clo_td);
        fprintf(lapiz, "%s", clo_tr);


        fprintf(lapiz, "%s", ope_tr);

        fprintf(lapiz, "%s", ope_td);
        fprintf(lapiz, "%s", "mbr_tamaño");
        fprintf(lapiz, "%s", clo_td);

        fprintf(lapiz, "%s", ope_td);
        palabra = intTOchar;
        fprintf(lapiz, "%s", palabra);
        fprintf(lapiz, "%s", clo_td);

        fprintf(lapiz, "%s", clo_tr);
        /*************************************************/
        fprintf(lapiz, "%s", ope_tr);

        fprintf(lapiz, "%s", ope_td);
        fprintf(lapiz, "%s", "mbr_fecha_creacion");
        fprintf(lapiz, "%s", clo_td);

        fprintf(lapiz, "%s", ope_td);
        fprintf(lapiz, "%s", fecha);
        fprintf(lapiz, "%s", clo_td);

        fprintf(lapiz, "%s", clo_tr);
    /*****************************************************/
    /*************************************************/
        fprintf(lapiz, "%s", ope_tr);

        fprintf(lapiz, "%s", ope_td);
        fprintf(lapiz, "%s", "mbr_disk_signature");
        fprintf(lapiz, "%s", clo_td);

        fprintf(lapiz, "%s", ope_td);
        fprintf(lapiz, "%d", mbr_p->mbr_disk_signature);
        fprintf(lapiz, "%s", clo_td);

        fprintf(lapiz, "%s", clo_tr);
        fclose(lapiz);
    /*****************************************************/
    /*************************************************/
    int y ;
    for(y = 0; y<4; y++)
    {
        if(mbr_p->mbr_partition[y].part_status == '1')
        {
            lapiz = fopen(ubicacion, "a");
            printf("\nIterando.");
            char *label="partition_";
            sprintf(intTOchar, "%d", y);
            char *total_label = concatenacion(label, intTOchar);
            /*---------------------------------------------------*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", total_label);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);
            /*-----------------------------------------------------*/
            /*=====================================================*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", "part_status");
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%c", mbr_p->mbr_partition[y].part_status);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);
            /*=====================================================*/
            /*=====================================================*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", "part_type");
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%c", mbr_p->mbr_partition[y].part_type);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);
            /*=====================================================*/

            /*=====================================================*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", "part_fit");
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%c", mbr_p->mbr_partition[y].part_fit);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);
            /*=====================================================*/
            /*=====================================================*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", "part_start");
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%d", mbr_p->mbr_partition[y].part_start);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);
            /*=====================================================*/
            /*=====================================================*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", "part_size");
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%d", mbr_p->mbr_partition[y].part_size);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);
            /*=====================================================*/
            /*=====================================================*/
            fprintf(lapiz, "%s", ope_tr);

            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", "part_name");
            fprintf(lapiz, "%s", clo_td);

            palabra = mbr_p->mbr_partition[y].part_name;
            fprintf(lapiz, "%s", ope_td);
            fprintf(lapiz, "%s", palabra);
            fprintf(lapiz, "%s", clo_td);

            fprintf(lapiz, "%s", clo_tr);

            fclose(lapiz);
            /*=====================================================*/
            if(mbr_p->mbr_partition[y].part_type == 'e')
            {
                struct ebr_extended *ebr_actual = malloc(sizeof(struct ebr_extended));
                FILE * lupalogica= fopen(val_direccion, "rb");
                if (lupalogica != NULL)
                {
                    fseek(lupalogica, mbr_p->mbr_partition[y].part_start + 152 ,SEEK_SET);
                    fread(ebr_actual, sizeof(struct ebr_extended), 1, lupalogica);
                    fclose(lupalogica);
                }
                int contador = 1;
                char *label2="EBR_";
                sprintf(intTOchar, "%d", contador);
                char *total_label2 = concatenacion(label2, intTOchar);
                if(ebr_actual->part_status == '1')
                {/*                 SI ESTA ACTIVO EL PRIMER EBR    */
                /*---------------------------------------------------*/
                lapiz = fopen(ubicacion, "a");
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", total_label2);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                /*-----------------------------------------------------*/
                /*=====================================================*/
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", "ebr_status");
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%c", ebr_actual->part_status);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                /*=====================================================*/
                /*=====================================================*/
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", "ebr_fit");
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%c", ebr_actual->part_fit);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                /*=====================================================*/
                /*=====================================================*/
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", "ebr_start");
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%d", ebr_actual->part_start);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                /*=====================================================*/
                /*=====================================================*/
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", "ebr_size");
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%d", ebr_actual->part_size);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                /*=====================================================*/
                /*=====================================================*/
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", "ebr_next");
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%d", ebr_actual->part_next);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                /*=====================================================*/
                /*=====================================================*/
                fprintf(lapiz, "%s", ope_tr);

                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", "ebr_name");
                fprintf(lapiz, "%s", clo_td);
                palabra = ebr_actual->part_name;
                fprintf(lapiz, "%s", ope_td);
                fprintf(lapiz, "%s", palabra);
                fprintf(lapiz, "%s", clo_td);

                fprintf(lapiz, "%s", clo_tr);
                fclose(lapiz);
                /*=====================================================*/
                }
                ++contador;
                int stat = ebr_actual->part_next;
                printf("\n ESTATUS:%d", stat);
                if(stat != -1)
                {
                    lupalogica= fopen(val_direccion, "rb");
                    if (lupalogica != NULL)
                    {
                        fseek(lupalogica, ebr_actual->part_next + 152 ,SEEK_SET);
                        fread(ebr_actual, sizeof(struct ebr_extended), 1, lupalogica);
                        fclose(lupalogica);
                    }
                }
                while(stat != -1)
                {
                    sprintf(intTOchar, "%d", contador);
                    total_label2 = concatenacion(label2, intTOchar);
                    ++contador;
                    /*---------------------------------------------------*/
                    lapiz = fopen(ubicacion, "a");
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", total_label2);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    /*-----------------------------------------------------*/
                    /*=====================================================*/
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", "ebr_status");
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%c", ebr_actual->part_status);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    /*=====================================================*/
                    /*=====================================================*/
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", "ebr_fit");
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%c", ebr_actual->part_fit);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    /*=====================================================*/
                    /*=====================================================*/
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", "ebr_start");
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%d", ebr_actual->part_start);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    /*=====================================================*/
                    /*=====================================================*/
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", "ebr_size");
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%d", ebr_actual->part_size);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    /*=====================================================*/
                    /*=====================================================*/
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", "ebr_next");
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%d", ebr_actual->part_next);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    /*=====================================================*/
                    /*=====================================================*/
                    fprintf(lapiz, "%s", ope_tr);

                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", "ebr_name");
                    fprintf(lapiz, "%s", clo_td);
                    palabra = ebr_actual->part_name;
                    fprintf(lapiz, "%s", ope_td);
                    fprintf(lapiz, "%s", palabra);
                    fprintf(lapiz, "%s", clo_td);

                    fprintf(lapiz, "%s", clo_tr);
                    fclose(lapiz);
                    /*=====================================================*/
                    if(ebr_actual->part_next != -1)
                    {
                        lupalogica= fopen(val_direccion, "rb");
                        if (lupalogica != NULL)
                        {
                            fseek(lupalogica, ebr_actual->part_next + 152 ,SEEK_SET);
                            fread(ebr_actual, sizeof(struct ebr_extended), 1, lupalogica);
                            fclose(lupalogica);
                        }

                    }else
                    {
                        stat = -1;
                    }
            }
        }
    }
    /*****************************************************/
    }
        lapiz = fopen(ubicacion, "a");
        fprintf(lapiz, "%s", endf);
        fclose(lapiz);
    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s", comandoFinal);
    system(comandoFinal);
    printf("\n fin");
}
}

void printExtendida(char *ubicacion)
{
    printf("\n Extendida...");
    int o ;
    int pos = 0;
    for(o = 0; o<4; o++)
    {
        if(mbr_p->mbr_partition[o].part_status == '1')
        {
            if(mbr_p->mbr_partition[o].part_type == 'e')
            {
                /*printf("\n Encontré la extendida!!!!!!!!!!!!!!!!!!!!!!!!!!");*/
                pos = o;
                break;
            }
        }
    }
    int default_size = mbr_p->mbr_partition[pos].part_size;
    int default_start = mbr_p->mbr_partition[pos].part_start;
    struct ebr_extended *ebr_actual = malloc(sizeof(struct ebr_extended));
    /*printf("\n Direccion ebr: %s", val_direccion);*/
    FILE * lupalogica= fopen(val_direccion, "rb");
    if (lupalogica != NULL)
    {
        fseek(lupalogica, mbr_p->mbr_partition[pos].part_start + 152 ,SEEK_SET);
        fread(ebr_actual, sizeof(struct ebr_extended), 1, lupalogica);
        fclose(lupalogica);
       /* printf("\n Lectura exitosa!!!");
        printf("\n SEGVAL: %d  %d", ebr_actual->part_start, mbr_p->mbr_partition[pos].part_start);
        printf("\n SEGVAL: %c", ebr_actual->part_status);*/
    }else{printf("\n Error!!!");}

    FILE *lapiz;
    /*if(ebr_actual->part_next != -1)
    {*/
        int status = ebr_actual->part_next;
        lapiz = fopen(ubicacion, "a");
        fprintf(lapiz, "%s", "EBR");
        fprintf(lapiz, "%s", "\\nStatus:");
        fprintf(lapiz, "%c", ebr_actual->part_status);
        fprintf(lapiz, "%s", " \\nNext_ebr:");
        fprintf(lapiz, "%d", ebr_actual->part_next);
        fclose(lapiz);
        if(ebr_actual->part_status == '1')
        {
            lapiz = fopen(ubicacion, "a");
            fprintf(lapiz, "%s", " | ");
            fprintf(lapiz, "%s", "LOGICA");
            fprintf(lapiz, "%s", "\\nTamano:");
            fprintf(lapiz, "%d", ebr_actual->part_size);
            fprintf(lapiz, "%s", "\\nNombre:");
            char *nombre = ebr_actual->part_name;
            fprintf(lapiz, "%s", nombre);
            fclose(lapiz);
        }else
        {
            lapiz = fopen(ubicacion, "a");
            fprintf(lapiz, "%s", " | ");
            fprintf(lapiz, "%s", "LIBRE");
            fprintf(lapiz, "%s", "\\nTamano:");
            fprintf(lapiz, "%d", ebr_actual->part_next);
            fclose(lapiz);
        }
        int result = ebr_actual->part_next - (ebr_actual->part_start + ebr_actual->part_size);
        if(result != 0)
        {
        lapiz = fopen(ubicacion, "a");
            fprintf(lapiz, "%s", " | ");
            fprintf(lapiz, "%s", "LIBRE");
            fprintf(lapiz, "%s", "\\nTamano:");
            fprintf(lapiz, "%d", result);
            fclose(lapiz);
        }
        if(status != -1)
        {
            lupalogica= fopen(val_direccion, "rb");
            if (lupalogica != NULL)
            {
                fseek(lupalogica, ebr_actual->part_next + 152 ,SEEK_SET);
                fread(ebr_actual, sizeof(struct ebr_extended), 1, lupalogica);
                fclose(lupalogica);
            }
            status = 0;
            while(status != -1)
            {
                int ultimo = 0;
                lapiz = fopen(ubicacion, "a");
                fprintf(lapiz, "%s", " | ");
                fprintf(lapiz, "%s", "EBR");
                fprintf(lapiz, "%s", "\\nStatus:");
                fprintf(lapiz, "%c", ebr_actual->part_status);
                fprintf(lapiz, "%s", "\\nNext:");
                fprintf(lapiz, "%d", ebr_actual->part_next);
                fclose(lapiz);
                if(ebr_actual->part_status == '1')
                {
                    lapiz = fopen(ubicacion, "a");
                    fprintf(lapiz, "%s", " | ");
                    fprintf(lapiz, "%s", "LOGICA");
                    fprintf(lapiz, "%s", "\\nTamano:");
                    fprintf(lapiz, "%d", ebr_actual->part_size);
                    fprintf(lapiz, "%s", "\\nNombre:");
                    char *nombre = ebr_actual->part_name;
                    fprintf(lapiz, "%s", nombre);
                    fclose(lapiz);
                }
                if(ebr_actual->part_next == -1)/*---------------------------SI ES EL ULTIMO------------------------*/
                {
                    ultimo = 1;
                    int resultado = (default_start + default_size) - (ebr_actual->part_start + ebr_actual->part_size);
                    if(resultado != 0)
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "LIBRE");
                        fprintf(lapiz, "%s", "\\nTamano:");
                        fprintf(lapiz, "%d", resultado);
                        fclose(lapiz);
                    }
                }/*---------------------------------------------------------------------------------------------------*/
                int resultado = ebr_actual->part_next - (ebr_actual->part_start + ebr_actual->part_size);
                if(resultado != 0 && ultimo == 0)
                {
                    lapiz = fopen(ubicacion, "a");
                    fprintf(lapiz, "%s", " | ");
                    fprintf(lapiz, "%s", "LIBRE");
                    fprintf(lapiz, "%s", "\\nTamano:");
                    fprintf(lapiz, "%d", resultado);
                    fclose(lapiz);
                }
                if(ultimo == 1)
                {
                    status = -1;
                }else
                {
                    lupalogica= fopen(val_direccion, "rb");
                    if (lupalogica != NULL)
                    {
                        fseek(lupalogica, ebr_actual->part_next + 152 ,SEEK_SET);
                        fread(ebr_actual, sizeof(struct ebr_extended), 1, lupalogica);
                        fclose(lupalogica);
                    }
                }
            }
        }
        else
        {
            if(ebr_actual->part_status == '1')
            {
                int resultado = (default_start + default_size) - (ebr_actual->part_start + ebr_actual->part_size);
                if(resultado != 0)
                {
                    lapiz = fopen(ubicacion, "a");
                    fprintf(lapiz, "%s", " | ");
                    fprintf(lapiz, "%s", "LIBRE");
                    fprintf(lapiz, "%s", "\\nTamano:");
                    fprintf(lapiz, "%d", resultado);
                    fclose(lapiz);
                }
            }else
            {
                int resultado = default_size;
                lapiz = fopen(ubicacion, "a");
                fprintf(lapiz, "%s", " | ");
                fprintf(lapiz, "%s", "LIBRE");
                fprintf(lapiz, "%s", "\\nTamano:");
                fprintf(lapiz, "%d", resultado);
                fclose(lapiz);
            }
        }
    /*}
    else
    {*/
        /*printf("\n \n No habian logicas para imprimir.");
        FILE *lapiz;
        lapiz = fopen(ubicacion, "a");
        fprintf(lapiz, "%s", "EXTENDIDA\\n");
        fprintf(lapiz, "%s", "Tamano:");
        fprintf(lapiz, "%d", mbr_p->mbr_partition[pos].part_size);
        fprintf(lapiz, "%s", "\\nNombre:");
        char *nombre = mbr_p->mbr_partition[pos].part_name;
        fprintf(lapiz, "%s", nombre);
        fclose(lapiz);*/
    /*}*/

    /*lapiz = fopen(ubicacion, "a");
    fprintf(lapiz, "%s", "PRIMARIA\\n");
    fprintf(lapiz, "%s", "Tamano:");
    fprintf(lapiz, "%d", LimSuperior[i]-LimInferior[i]);
    fprintf(lapiz, "%s", "\\nNombre:");
    char *nombre = LimNombres[i];
    fprintf(lapiz, "%s", nombre);
    fclose(lapiz);*/
}

void reporte_disk(char *direccion, char *extension, char *ubicacion_original)
{
    printf("\n REPORTE DISK\n Extension: %s", extension);
    char *jpg = "jpg";
    char *png = "png";
    char *extensionDot = "dot -Tpng ";
    char *output = " -o ";
    if(strcmp(extension, jpg)==0)
    {
        extensionDot = "dot -Tjpg ";
    }if(strcmp(extension, png)==0)
    {
        extensionDot = "dot -Tpng ";
    }
    cargarArchivoDisco2();
    setMinimos();
    char nada[1] ;
    nada[0] = ' ';
    char *nombreArchivo = "/reportedisk.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);
    /*printf("\n [%s]", ubicacion);*/
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
    }
    char *cab1 ="digraph disco {";
    char *cab2="node [shape=record];\n";
    char *estructura = "\nstruct1 [label =  \"";
    char *endf ="\"];}";
    char intTOchar[20];
    sprintf(intTOchar, "%d", mbr_p->mbr_tamano);
    char *palabra;
    FILE *lapiz = fopen(ubicacion, "a");
    /*num_particiones*/
    if(lapiz != NULL)
    {
        fprintf(lapiz, "%s", cab1);
        fprintf(lapiz, "%s", cab2);
        fprintf(lapiz, "%s", estructura);
        fclose(lapiz);
        lapiz = fopen(ubicacion, "a");
        fprintf(lapiz, "%s", "MBR");
        fprintf(lapiz, "%s", "\\nmbr_tamano: ");
        fprintf(lapiz, "%d", mbr_p->mbr_tamano);
        fprintf(lapiz, "%s", "\\nmbr_disk_signature: ");
        fprintf(lapiz, "%d", mbr_p->mbr_disk_signature);
        fclose(lapiz);
        printf("\n NUMERO DE PARTICIONES: %d", num_particiones);
        if(num_particiones != 0)
        {
            int i;
            for(i=0; i<num_particiones; i++)
            {
                if(i == 0)
                {
                    if(LimInferior[0] != 0)
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "LIBRE");
                        fprintf(lapiz, "%s", "\\nTamano:");
                        fprintf(lapiz, "%d", LimInferior[0]);
                        fclose(lapiz);
                    }
                    if(LimTipos[0]=='p')
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "PRIMARIA");
                        fprintf(lapiz, "%s", "\\nTamano:");
                        fprintf(lapiz, "%d", LimSuperior[0]-LimInferior[0]);
                        fprintf(lapiz, "%s", "\\nNombre:");
                        char *nombre = LimNombres[0];
                        fprintf(lapiz, "%s", nombre);
                        fclose(lapiz);
                    }else
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "{ EXTENDIDA | {");
                        fclose(lapiz);
                        printExtendida(ubicacion);
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", "} } ");
                        fclose(lapiz);
                    }
                }
                else
                {
                    if(LimInferior[i] != LimSuperior[i-1])
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "LIBRE");
                        fprintf(lapiz, "%s", "\\nTamano:");
                        fprintf(lapiz, "%d", LimInferior[i]-LimSuperior[i-1]);
                        printf("\n EXTREMOS DEL ESPACIO LIBRE: %d %d", LimInferior[i], LimSuperior[i-1]);
                        fclose(lapiz);
                    }
                    if(LimTipos[i]=='p')
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "PRIMARIA");
                        fprintf(lapiz, "%s", "\\nTamano:");
                        fprintf(lapiz, "%d", LimSuperior[i]-LimInferior[i]);
                        fprintf(lapiz, "%s", "\\nNombre:");
                        char *nombre = LimNombres[i];
                        fprintf(lapiz, "%s", nombre);
                        fclose(lapiz);
                    }else
                    {
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", " | ");
                        fprintf(lapiz, "%s", "{ EXTENDIDA | {");
                        fclose(lapiz);
                        printExtendida(ubicacion);
                        lapiz = fopen(ubicacion, "a");
                        fprintf(lapiz, "%s", "} } ");
                        fclose(lapiz);
                    }
                }
            }
            if(LimSuperior[num_particiones-1] != mbr_p->mbr_tamano)
            {
                printf("\n ESPACIO VACIO EN EL REPORTE DE: %d  %d", mbr_p->mbr_tamano, LimSuperior[num_particiones-1]);
                lapiz = fopen(ubicacion, "a");
                fprintf(lapiz, "%s", " | ");
                fprintf(lapiz, "%s", "LIBRE22");
                fprintf(lapiz, "%s", "\\nTamano:");
                fprintf(lapiz, "%d", mbr_p->mbr_tamano - LimSuperior[num_particiones-1]);
                fclose(lapiz);
            }
            lapiz = fopen(ubicacion, "a");
            fprintf(lapiz, "%s", endf);
            fclose(lapiz);
        }else
        {
            lapiz = fopen(ubicacion, "a");
            fprintf(lapiz, "%s", "LIBRE");
            fprintf(lapiz, "%s", "\\nTamano:");
            fprintf(lapiz, "%d", mbr_p->mbr_tamano);
            fclose(lapiz);
            lapiz = fopen(ubicacion, "a");
            fprintf(lapiz, "%s", endf);
            fclose(lapiz);
        }

    }

    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s", comandoFinal);
    system(comandoFinal);
    printf("\n fin");
}

void generarReporte()
{
    printf("\n Ubicacion del reporte: %s", val_direccion);
    char **extension = str_split(val_direccion, '.');
    char *ubicacion_original = strdup(val_direccion);
    asignar_FolderPath(*(extension +1), ubicacion_original);
}

void montarParticion()
{
    char *id2= concatenacion(val_direccion, nombre);
    char *id = strdup(id2);
    char *no = "no";
    printf("\n id: %s", id);
    cargarArchivoDisco();
    /*+++++++++++++++++++BUSCAR PARTICION EN DISCO+++++++++++++++++++++++*/
    int i;
    int existe = 0;
    int pos = 0;
    for(i = 0; i < 4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            char *name = mbr_p->mbr_partition[i].part_name;
            if(strcmp(nombre_fdisk, name)==0)
            {
                existe = 1;
                pos = i;
                printf("\n Existe la particion en el disco!");
            }
        }
    }
    /*              EXISTE LA PARTICION EN EL DISCO */
    if(existe == 1)
    {
        int x = 0;
        int y = 0;
        int guardado = 0;
        for(i = 0; i<4; i++)
        {
            int j;
            for(j = 0; j<4; j++)
            {
                if(i == 0)
                {
                    if(strcmp(fila1[j], no)==0 && guardado==0)
                    {
                        printf("\n Espacio vacio en x:%d y:%d", j, i);
                        x = j;
                        y = i;
                        fila1[j] = id;
                        part_name1[j] = nombre_fdisk;
                        path1[j] = val_direccion;
                        guardado = 1;
                    }
                }
                /*222222222222222222222222222222222222222222222222222222222*/
                if(i == 1)
                {
                    if(strcmp(fila2[j], no)==0 && guardado==0)
                    {
                        printf("\n Espacio vacio en x:%d y:%d", j, i);
                        x = j;
                        y = i;
                        fila2[j] = id;
                        part_name2[j] = nombre_fdisk;
                        path2[j] = val_direccion;
                        guardado = 1;
                    }
                }
                /*333333333333333333333333333333333333333333333333333333333333*/
                if(i == 2)
                {
                    if(strcmp(fila3[j], no)==0 && guardado==0)
                    {
                        printf("\n Espacio vacio en x:%d y:%d", j, i);
                        x = j;
                        y = i;
                        fila3[j] = id;
                        part_name3[j] = nombre_fdisk;
                        path3[j] = val_direccion;
                        guardado = 1;
                    }
                }
                /*444444444444444444444444444444444444444444444444444444444444*/
                if(i == 3)
                {
                    if(strcmp(fila4[j], no)==0 && guardado==0)
                    {
                        printf("\n Montando particion en x:%d y:%d", j, i);
                        x = j;
                        y = i;
                        fila4[j] = id;
                        part_name4[j] = nombre_fdisk;
                        path4[j] = val_direccion;
                        guardado = 1;
                    }
                }
            }
        }
    }else
    {
        printf("\n Verificar logicas, no es p/e");
    }
    printf("\nFin montar particion.");
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
}

void desmontarParticion()
{
    char cabecera_array[5] = "abcd";
    char *no = "no";
    printf("\n Desmontando particion.");
    int rango = 0;
    char letra = *(val_un_id +2);
    int fila = *(val_un_id +3) - '0';
    if(letra == 'a' || letra == 'b' || letra == 'c' || letra == 'd')
    {
        rango = 1;
    }
    if(fila >= 0 && fila <= 4)
    {
        rango = rango +1;
    }else
    {
        rango = 0;
    }
    int x = 0;
    if(rango == 2)
    {
        if(letra == 'a')
        {
            x = 0;
        }else if(letra == 'b')
        {
            x = 1;
        }else if(letra == 'c')
        {
            x = 2;
        }else if(letra == 'd')
        {
            x = 3;
        }
        printf("\nx: %d y:%d", x, fila);
        if(fila == 0)
        {
            fila1[x] = no;
        } else if(fila == 1)
        {

            fila2[x] = no;
        }else if(fila == 2)
        {

            fila3[x] = no;
        }else if(fila == 3)
        {

            fila4[x] = no;
        }
    }
}

void imprimirParticiones()
{
    char cabecera_array[5] = "abcd";
    printf("\n Imprimiendo Particiones");
    char *no = "no";
    int i;
    for(i = 0; i< 4; ++i)
    {
        int j;
        for(j = 0; j < 4; j++)
        {
            if(i == 0)
            {
                if(strcmp(fila1[j], no) != 0)
                {
                    char let = cabecera_array[j];
                    printf("\n -%c -%d -%c", let, j, cabecera_array[j]);
                    printf("\n#id:: vd%c%d -path::\" %s\" -name::\" %s\" ", let, i, path1[j], part_name1[j]);
                }
            }
            if(i == 1)
            {
                if(strcmp(fila2[j], no) != 0)
                {
                    char let = cabecera_array[j];
                    /*sprintf(num, "%d", i);*/
                    printf("\n#id:: vd%c%d -path::\" %s\" -name::\" %s\" ", let, i, path2[j], part_name2[j]);
                }
            }
            if(i == 2)
            {
                if(strcmp(fila3[j], no) != 0)
                {
                    char let = cabecera_array[j];
                    /*sprintf(num, "%d", i);*/
                    printf("\n#id:: vd%c%d -path::\" %s\" -name::\" %s\" ", let, i, path3[j], part_name3[j]);
                }
            }
            if(i == 0)
            {
                if(strcmp(fila4[j], no) != 0)
                {
                    char let = cabecera_array[j];
                    /*sprintf(num, "%d", i);*/
                    printf("\n#id:: vd%c%d -path::\" %s\" -name::\" %s\" ", let, i, path4[j], part_name4[j]);
                }
            }
        }
    }
}
void crearDisco()
{
    printf("\nCreando disco...\n");
    int t_bytes = 0;
    struct stat st = {0};
    char string[32];
    val_unit[0] = 'm';
    if(val_unit[0] == 'k' || val_unit[0] == 'K')
    {
        t_bytes = 1024 * val_size;
    }
    else if(val_unit[0] == 'm' || val_unit[0] == 'M')
    {
        t_bytes = 1024 * 1024 * val_size ;
    }
    else if(val_unit[0] == 'B' || val_unit[0] == 'b')
    {
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
        /*primerDisco->mbr_fecha_creacion = tm;*/
        strftime(primerDisco->mbr_fecha_creacion, sizeof(primerDisco->mbr_fecha_creacion), "%Y/%m/%d %H:%M", tm);
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
        /*primerDisco->mbr_fecha_creacion = tm;*/
        strftime(primerDisco->mbr_fecha_creacion, sizeof(primerDisco->mbr_fecha_creacion), "%Y/%m/%d %H:%M", tm);
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
    printf("\n¿Desea eliminar el archivo? si[si+ENTER] no[no+ENTER]\n");
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

void minusculas(char *entrada)
{
    printf("\n Convirtiendo a minusculas...");
    int tamanoo = strlen(entrada);
    int i;
    for(i = 0; i< tamanoo; i++)
    {
        if(*(entrada +i) == 'A')
        {
            *(entrada +i) = 'a';
        }else if(*(entrada +i) == 'B')
        {

            *(entrada +i) = 'b';
        }else if(*(entrada +i) == 'C')
        {

            *(entrada +i) = 'c';
        }else if(*(entrada +i) == 'D')
        {
            *(entrada +i) = 'd';

        }else if(*(entrada +i) == 'E')
        {
            *(entrada +i) = 'e';

        }else if(*(entrada +i) == 'F')
        {
            *(entrada +i) = 'f';

        }else if(*(entrada +i) == 'G')
        {
            *(entrada +i) = 'g';

        }else if(*(entrada +i) == 'H')
        {
            *(entrada +i) = 'h';

        }else if(*(entrada +i) == 'I')
        {
            *(entrada +i) = 'i';

        }else if(*(entrada +i) == 'J')
        {
            *(entrada +i) = 'j';

        }else if(*(entrada +i) == 'K')
        {
            *(entrada +i) = 'k';

        }else if(*(entrada +i) == 'L')
        {
            *(entrada +i) = 'l';

        }else if(*(entrada +i) == 'M')
        {
            *(entrada +i) = 'm';

        }else if(*(entrada +i) == 'N')
        {
            *(entrada +i) = 'n';

        }else if(*(entrada +i) == 'O')
        {
            *(entrada +i) = 'o';

        }else if(*(entrada +i) == 'P')
        {
            *(entrada +i) = 'p';

        }else if(*(entrada +i) == 'Q')
        {
            *(entrada +i) = 'q';

        }else if(*(entrada +i) == 'R')
        {
            *(entrada +i) = 'r';

        }else if(*(entrada +i) == 'S')
        {
            *(entrada +i) = 's';

        }else if(*(entrada +i) == 'T')
        {
            *(entrada +i) = 't';

        }else if(*(entrada +i) == 'U')
        {
            *(entrada +i) = 'u';

        }else if(*(entrada +i) == 'V')
        {
            *(entrada +i) = 'v';

        }else if(*(entrada +i) == 'W')
        {
            *(entrada +i) = 'w';

        }else if(*(entrada +i) == 'X')
        {
            *(entrada +i) = 'x';

        }else if(*(entrada +i) == 'Y')
        {
            *(entrada +i) = 'y';

        }else if(*(entrada +i) == 'Z')
        {

            *(entrada +i) = 'z';
        }
    }
    printf("\n Resultado: %s\n-----------------------------", entrada);
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
char* getComando(char *comando)/*Enviar comando completo, devuelve valor del comando*/
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
    val_tipo_reporte =*(ArregloComando+1);
    /*printf("\nlllllll[%s]",*(ArregloComando+1));*/
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
    if (posicion < tamanoArreglo)/*se realizo el split correctamente*/
    {
    char* token = getComando(*(entradaTotal+posicion));
    minusculas(token);
    printf("\n!-----!\n");
    printf("[%s]",*(entradaTotal + posicion));
    printf("\n!-----!\n");
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
    char *full = "full";
    char *fast = "fast";
    char *mount= "mount";
    char *unmount= "unmount";
    char *un_id= "-id1";
    char *rep = "rep";
    char *id = "-id";
    char *exec = "exec";
    char *bs = "\\";
    char *bs2 = "\\\n";

    char *fdisk = "fdisk";
    printf("TamanoArreglo: %d Token", tamanoArreglo, token);

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
            printf("\n----NAME:\n");printf(nombre_fdisk);printf("\n----\n");
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
            char **split = str_split(entradaUnica, ':');
            if(strcmp(full, *(split +1))==0)
            {
                val_delete = 1;
                printf("\n full");
            }else if(strcmp(fast, *(split +1))==0)
            {
                printf("\n fast");
                val_delete = 0;
            }else
            {
                printf("\n delete error");
                val_delete = -1;
            }
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, add)==0)
        {
            bool_add = 1;
            val_add = getValorEntero(*(entradaTotal + posicion));
            printf("3[%d]", val_add);
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
        else if(strcmp(token, mount)==0)
        {
            bool_mount = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, unmount)==0)
        {
            bool_unmount = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, un_id)==0)
        {
            char **split = str_split(entradaUnica, ':');
            val_un_id = *(split +1);
            printf("\n RECUPERADO:%s", val_un_id);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, rep)==0)
        {
            bool_rep = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, id)==0)
        {
            /*bool_rep = 1;*/
            char **split = str_split(entradaUnica, ':');
            val_un_id = *(split +1);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, exec)==0)
        {
            bool_exec = 1;
            val_direccion = *(entradaTotal + posicion+1);
        }
        else if(strcmp(token, bs)==0 || strcmp(token, bs2)==0)
        {
            bool_bs = 1;
            printf("\n VINO UN BS!");
        }

    }else
    {
        printf("\nDesborde\n");
    }
}

void master_Driver()
{
    printf("\n MasterDriver");
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
        if(bool_sizee==1 && bool_path==1 && bool_name==1)
        {
            crearParticion();
        }else if(bool_add == 1)
        {
            modificarParticion();
        }else if(bool_deletee == 1)
        {
            eliminarParticion();

        }
        else
        {
            printf("Error, comando no reconocido.");
        }
    }else if(bool_mount == 1)
    {
        if(bool_mount == 1 && bool_path == 0 && bool_name == 0)
        {
            imprimirParticiones();
        }else
        {
            montarParticion();
        }
    }else if(bool_unmount == 1)
    {
        desmontarParticion();
    }else if(bool_rep == 1)
    {
        generarReporte();
    }else if(bool_exec == 1)
    {
        execute_LBL();
    }
}

char* quitarBackSlash(char *entrada)
{
    int pos = 0;
    size_t tamanoo = strlen(entrada);
    int i;
    char respuesta[tamanoo-1];
    int contador = 0;
    for(i = 0; i< tamanoo; i++)
    {
        if(*(entrada+i)!='\\')
        {
            respuesta[contador] = *(entrada+i);
            ++contador;
        }else
        {
            respuesta[contador] = ' ';
            ++contador;
        }
    }
    char *ptr = respuesta;
    printf("\nSin el BS: %s" ,ptr);
    return respuesta;

}

int tieneBS(char *entrada)
{

    int tienebs = 0;
    size_t tamanoo = strlen(entrada);
    int i;
    printf("\n Tamano cadena %d", tamanoo);
    for(i = 0; i< tamanoo; i++)
    {
        if(*(entrada + i) == '\\')
        {
            tienebs = 1;
            break;
        }
    }
    return tienebs;
}

int tieneWS(char *entrada)
{
    int i;
    int tamanoo = strlen(entrada);
    int respuesta = 0;
    for(i = 0; i<tamanoo; i++)
    {
        if(*(entrada + i) == ' ')
        {
            respuesta = 1;
        }
    }
    return respuesta;
}

void execute_LBL()
{
    printf("\n exec!: %s\n\n", val_direccion);

    FILE * fp;
    char * entradaUsuario2 = NULL;
    size_t len = 0;
    ssize_t read;
    int contador_bs = 0;
    fp = fopen(val_direccion, "r");
    int crash_b = 0;
    char *conctncn;
    if (fp != NULL)
    {
        while ((read = getline(&entradaUsuario2, &len, fp)) != -1)
        {

                /*printf("Linea de largo %zu :\n", read);*/
                printf("\nFase1 ->%s", entradaUsuario2);
                if(*(entradaUsuario2) != '#')
                {
                    int puntos = contadorPuntos(entradaUsuario2);/*regresa la cant de :'s/2*/
                    size_t tamanoarray = strlen(entradaUsuario2);
                    char entrada[tamanoarray-puntos];
                    if(crash_b == 0)
                    {
                        partition_Starter();
                        crash_b = 1;
                    }
                    /*Habiendo definido un nuevo arreglo para la entrada, a la que se le
                    convertira sustituyendo cada par de dos puntos por uno solo*/
                    int d;
                    int paridad = 0;/*simulador de booleano*/
                    int contador = 0;
                    for(d = 0; d<tamanoarray+1; d++)
                    {
                        if(entradaUsuario2[d] ==':'){
                            if(paridad == 1)
                            {
                                paridad = 0;
                                entrada[contador] = entradaUsuario2[d];
                                contador = contador+1;
                            }else
                            {
                                paridad = 1;
                            }
                        }else
                        {
                            entrada[contador] = entradaUsuario2[d];
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
                    if(tieneWS(entrada) == 1)
                    {
                        automata(tokens, *(tokens + 0), 0);
                    }else
                    {
                        automata(tokens, entrada, 0);
                    }
                    if(bool_bs != 1)
                    {
                    printf("\n bool_bs::%d", bool_bs);
                        master_Driver();
                        /*---------------------------------------------------------*/
                        bool_add = 0;
                        bool_deletee = 0;
                        bool_fdisk = 0;
                        bool_fit = 0;
                        bool_mkdisk =  0;
                        bool_name =  0;
                        bool_path = 0;
                        bool_rmdisk = 0;
                        bool_sizee = 0;
                        bool_type = 0;
                        bool_unit = 0;
                        bool_mount = 0;
                        bool_unmount = 0;
                        tamanoArreglo = 0;
                        bool_id = 0;
                        particionado_disco_actual = 0;
                        val_add = 0;
                        val_delete = 0;
                        val_size = 0;
                        bool_rep = 0;
                        bool_exec = 0;

                        int indice;
                        for(indice = 0; indice < 4; indice++)
                        {
                            minimos1[indice] = 0;
                            superior[indice] = 0;
                            LimSuperior[indice] = 0;
                            LimSuperior[indice] = 0;
                            EspacioLibre[indice] = 0;
                            EspacioLibre_max[indice] = 0;
                            EspacioLibre_min[indice] = 0;
                        }
                        EspacioLibre[indice+1] = 0;
                        EspacioLibre_max[indice+1] = 0;
                        EspacioLibre_min[indice+1] = 0;

                        val_type[1] = "p";
                        val_unit[1] = "m";
                    }/*IF del backslash*/
                    bool_bs = 0;
                }
            }/*While linea por linea*/



        fclose(fp);
        if (entradaUsuario2)
        {
            free(entradaUsuario2);
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
    printf("                        [File System ext2/ext3]\n");
    /*---------------------------------------------------------------*/
    int exit = 0;
    int crash_b = 0;
    while(exit == 0)
    {
        char entradaUsuario[256];
        printf("\n Fase1 ->     ");
        fgets(entradaUsuario, sizeof(entradaUsuario), stdin);
        int puntos = contadorPuntos(entradaUsuario);/*regresa la cant de :'s/2*/
        int tamanoarray = tamano2(entradaUsuario);
        char entrada[sizeof(entradaUsuario)-puntos];
        if(crash_b == 0)
        {
            partition_Starter();
            crash_b = 1;
        }
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
        /*---------------------------------------------------------*/
        bool_add = 0;
        bool_deletee = 0;
        bool_fdisk = 0;
        bool_fit = 0;
        bool_mkdisk =  0;
        bool_name =  0;
        bool_path = 0;
        bool_rmdisk = 0;
        bool_sizee = 0;
        bool_type = 0;
        bool_unit = 0;
        bool_mount = 0;
        bool_unmount = 0;
        tamanoArreglo = 0;
        bool_id = 0;
        particionado_disco_actual = 0;
        val_add = 0;
        val_delete = 0;
        val_size = 0;
        bool_rep = 0;
        bool_exec = 0;

        int indice;
        for(indice = 0; indice < 4; indice++)
        {
            minimos1[indice] = 0;
            superior[indice] = 0;
            LimSuperior[indice] = 0;
            LimSuperior[indice] = 0;
            EspacioLibre[indice] = 0;
            EspacioLibre_max[indice] = 0;
            EspacioLibre_min[indice] = 0;
        }
        EspacioLibre[indice+1] = 0;
        EspacioLibre_max[indice+1] = 0;
        EspacioLibre_min[indice+1] = 0;

        val_type[1] = "p";
        val_unit[1] = "m";
    }
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
