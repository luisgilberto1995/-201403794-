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
#include <math.h>

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
    /*..............................*/
    int GUID[10];
    char grupos[10][10];
    int UID[20];
    char usuarios_usr[20][10];
    char usuarios_pwd[20][10];
    char usuarios_grp[20][10];
    /*..............................*/
    /*----------------------------*/
    /*      Banderas reservadas   */
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

    int bool_mkfs = 0;

    int inicio_publico = 0;

    int bool_filesystem = 0;

    int bool_mkdir = 0;

    int bool_cargar_disco_2 = 0;

    int bool_cargar_superbloque = 0;

    int bool_cont = 0;

    int bool_mkfile = 0;

    int bool_cat = 0;

    int bool_login = 0;
    int bool_logueado = 0;

    int bool_mkgrp = 0;
    int bool_mkusr = 0;
    int bool_rmgrp = 0;
    int bool_rmusr = 0;

    int bool_journaling = 0;

    int bool_loss = 0;

    int bool_recovery = 0;

    int bool_edit = 0;

    int bool_plus_r = 0;

    int bool_chmod = 0;

    int bool_file_rename = 0;
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
    char val_filesystem = '3';
    char *val_partname;
    char *folder_direccion;
    char *val_direccion_a_buscar;
    char *val_cont;
    char *buffer_store;
    char *val_cat;
    char *val_cat_path;
    int buffer_size=0;
    char *total_recuperado="";
    char *total_armado="";
    char *user;
    char *password;
    int bool_uno=0;
    /*********************/
    char *user_group;
    int user_id;
    int user_group_id;
    /**********************/
    char *nombre_USER_COMMAND;
    char *pwd_USER_COMMAND;
    char *grp_USER_COMMAND;
    int inicio_particion_publico = 0;
    int tamano_particion_publico = 0;
    int journal_permiso_escritura = 1;
    int block_count = 0;
    char ajuste_PUBLICO;
    int contador_grafico_bloques = 0;
    int bool_ls_i =0;
    char *ruta_char;
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
        char part_name[16];/**/
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
/************************************************/
/*                      FASE 2                  */
/************************************************/
 struct super_bloque
 {
     int s_filesystem_type;/*numero para el sistema de archivos*/
     int s_inodes_count;
     int s_blocks_count;
     int s_free_blocks_count;
     int s_free_inodes_count;
     char s_mtime[32];
     char s_umtime[32];
     int s_mnt_count;
     int s_magic;
     int s_inode_size;
     int s_block_size;
     int s_first_ino;
     int s_first_blo;
     int s_bm_inode_start;
     int s_bm_block_start;
     int s_inode_start;
     int s_block_start;
 }s_b;
 struct super_bloque *super_bloque_principal = NULL;

 struct j_data
 {
     int journal_tipo_operacion;
     char journal_tipo;
     char journal_nombre[100];
     int journal_tipo_contenido;
     char journal_contenido[64];
     char journal_fecha[20];
     int journal_propietario;
     int journal_grupo;
     int journal_permisos;
     int ultimo;
 }j_d;
struct j_data *journaling_data = NULL;

 /*------------------------------------------------*/
/*BITMAP DE INODOS*/
/*-------------------------------------------------*/
 typedef struct inodo
{
    int i_nlink;
    char i_pathlink[100];
    int i_uid;
    int i_gid;
    int i_size;
    char i_atime[32];
    char i_ctime[32];
    char i_mtime[32];
    int i_block[15];
    char i_type;
    int i_perm;
}inode;
struct inodo *inodo_principal = NULL;
/*BITMAP DE BLOQUES*/
typedef struct content
{
    char b_name[12];
    int b_inodo;
}content;

struct bloque_carpeta
{
    content b_content[4];
};
struct bloque_carpeta *bloque_carpeta_principal = NULL;

struct bloque_archivo
{
    char b_content[64];
};
struct bloque_archivo *bloque_archivo_principal = NULL;

struct bloque_apuntador
{
    int b_pointer[16];
};
struct bloque_apuntador *bloque_apuntador_principal = NULL;
/***********************************************/

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
                fseek(file, start ,SEEK_SET);
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
            printf("\n Encontrada extendida!");
            break;
        }
    }
    /*******************LECTURA Y RECOLECCION DE DATOS DE INTERES******************/
    int default_size = mbr_p->mbr_partition[pos].part_size;
    int default_start = mbr_p->mbr_partition[pos].part_start;
    int start =         mbr_p->mbr_partition[pos].part_start;
    /**/
    printf("\nParticion extendida, fisicamente inicia en: %d de tamano: %d", start, default_size);
    struct ebr_extended *ebr_actual=malloc(sizeof(struct ebr_extended));
    FILE * file= fopen(val_direccion, "rb");
    if (file != NULL) {
        fseek(file, start ,SEEK_SET);
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
                printf("\nActualizando fisicamente EBR en: %d", start);
                /**/
                FILE * file_update = fopen(val_direccion, "rb+");
                if (file_update != NULL) {
                    fseek(file_update, start ,SEEK_SET);
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
                    printf("\nActualizando fisicamente EBR en: %d", default_start);
                    /**/
                    FILE * file_update = fopen(val_direccion, "rb+");
                    if (file_update != NULL) {
                        fseek(file_update, start ,SEEK_SET);
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
                printf("\n part_next = %d.",ebr_actual->part_next );
                if(ebr_actual->part_next == -1 && t_bytes <= ((default_start + default_size) - (ebr_actual->part_start + ebr_actual->part_size)))
                {
                    confirmacion2 = 1;
                    printf("\nEs el ultimo EBR.");
                    /************************************************************************/
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
                            printf("\nActualizando EBR en:%d su nuevo part_start: %d" , ebr_actual->part_start, ebr_actual->part_next);
                            FILE * file_update= fopen(val_direccion, "rb+");
                            if (file_update != NULL) {
                                fseek(file_update, ebr_actual->part_start ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                                printf("\n???");
                            }
                            printf("\nEscribiendo fisicamente ultimo EBR en:%d", ebr_nuevo->part_start);
                            FILE * file= fopen(val_direccion, "rb+");
                            if (file != NULL) {
                                fseek(file, ebr_nuevo->part_start ,SEEK_SET);
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
                                fseek(file_update, ebr_actual->part_start ,SEEK_SET);
                                fwrite(ebr_actual, sizeof(struct ebr_extended), 1, file_update);
                                fclose(file_update);
                            }
                            FILE * file= fopen(val_direccion, "rb+");
                            if (file != NULL) {
                                fseek(file, ebr_nuevo->part_start ,SEEK_SET);
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
                            fseek(file5, ebr_actual->part_next ,SEEK_SET);
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
                        if(EspacioLibre_min[i] == 0)
                        {
                            EspacioLibre_min[i] = EspacioLibre_min[i]+152;
                        }
                        mbr_p->mbr_partition[a].part_start = EspacioLibre_min[i];
                        mbr_p->mbr_partition[a].part_fit = val_fit;
                        printf("\nEsta particion extendida inicia en: %d", EspacioLibre_min[i]);
                        printf("\nTamano de la particion: %d", valor_size);
                        FILE *file = fopen(val_direccion, "rb+");
                        size_t tam = sizeof(struct mbr_DISCO);
                        printf("\nVALOR EN BYTES[%zu]\n", tam);
                        if(file != NULL)
                        {
                            fwrite(mbr_p, sizeof(struct mbr_DISCO), 1, file);
                            fclose(file);
                            printf("\nMBR actualizada[extendida].\n");
                            s_flag = 1;
                        }
                        struct ebr_extended *ebr_inicial = malloc(sizeof(struct ebr_extended));
                        ebr_inicial->part_status = '0';
                        /*ebr_inicial->part_fit='f';*/
                        ebr_inicial->part_start= EspacioLibre_min[i];
                        printf("\nEBR inicia en: %d", ebr_inicial->part_start);
                        ebr_inicial->part_size = 0;
                        ebr_inicial->part_next = -1;
                        int byte_pos = EspacioLibre_min[i];
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
        printf("\Disco [%d] cargado.[%d]", mbr_p->mbr_disk_signature, mbr_p->mbr_partition[0].part_start);
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
    printf("\n val_un_id %s", val_un_id);
    char letra = *(val_un_id +2);
    int fila = *(val_un_id +3) - '0';
    printf("\n letra %c fila %d", letra, fila);
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
            val_partname = part_name1[x];
        }
         else if(fila == 1)
        {
            nuevoPath =path2[x];
            val_partname = part_name2[x];
        }
        else if(fila == 2)
        {
            nuevoPath =path3[x];
            val_partname = part_name3[x];
        }
        else if(fila == 3)
        {
            nuevoPath =path4[x];
            val_partname = part_name4[x];
        }
    }
    val_direccion = nuevoPath;
    printf("\n Nuevo path = %s", val_direccion);
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
                                mbr_p->mbr_partition[0].part_start = sizeof(struct mbr_DISCO);
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
    /*FILE *fp = fopen("DISCO", "ab+");

    FILE *f = fopen("DISCO", "w");
    if (f == NULL)
    {
        printf("\nError");
    }else
    {
         //print some text
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
    }*/
    char blockbitmap[5];
    blockbitmap[0]='1';
    blockbitmap[1]='0';
    blockbitmap[2]='1';
    int i;
    for( i = 3; i< 5; i++)
    {
        blockbitmap[i] = '0';
    }
    FILE *file= fopen("prueba.bin", "wb");
    if (file != NULL)
    {
        fwrite(blockbitmap, 5, 1, file);
        fclose(file);
    }
    char bitmap_bloques[5];
    file = fopen("prueba.bin", "rb");
    if(file != NULL)
    {
        /*fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET)*/
        fread(bitmap_bloques, 5, 1, file);
        fclose(file);
    }
    for(i = 0; i<5; i++)
    {
        printf("\n Prueba [%c]", bitmap_bloques[i]);
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

char* concatenacion(char *parametro1, char *parametro2)
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
    char *char_inode = "inode";
    char *char_sb = "sb";
    char *char_bm_inode = "bm_inode";
    char *char_bm_block = "bm_block";
    char *char_journaling = "journaling";
    char *char_tree = "tree";
    char *char_block= "block";
    char *lsl = "ls+l";
    char *lsi = "ls+i";
    char *filee= "file";
    printf("[Reporte seleccionado: %s]", val_tipo_reporte);
    if(strcmp(val_tipo_reporte, char_disk)==0)
    {
        reporte_disk(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_mbr)==0)
    {
        reporte_mbr(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_inode)==0)
    {
        inode_report_interfaz(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_sb)==0)
    {
        reporte_sb(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_bm_inode)==0)
    {
        reporte_bm_inode(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_bm_block)==0)
    {
        reporte_bm_block(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_journaling)==0)
    {
        reporte_journaling(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_tree)==0)
    {
        reporte_tree_interfaz(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, char_block)==0)
    {
        rep_block(direccionActual, extension, ubicacion_original);
    }
    else if(strcmp(val_tipo_reporte, filee)==0)
    {
        reporte_file();
    }
    else if(strcmp(val_tipo_reporte, lsl)==0)
    {
        reporte_ls(direccionActual, extension, ubicacion_original);
    }
    else if( strcmp(val_tipo_reporte, lsi)==0)
    {
        bool_ls_i = 1;
        reporte_ls(direccionActual, extension, ubicacion_original);
        bool_ls_i = 0;
    }
}

void reporte_bm_inode(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte bm_inode.");

    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion_original, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .txt creado!");
    }
    char inode_bitmap[super_bloque_principal->s_inodes_count];
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
        fread(inode_bitmap, super_bloque_principal->s_inodes_count, 1, file);
        fclose(file);
    }
    int contador = 1;
    int i;
    for(i = 0; i < super_bloque_principal->s_inodes_count; i++)
    {
        if(contador %20 == 0)
        {
            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%c", '\n');
            fclose(reporte);
        }
        reporte = fopen(ubicacion_original, "a");
        fprintf(reporte, "%c", inode_bitmap[i]);
        fprintf(reporte, "%s", "    ");
        fclose(reporte);
        ++contador;
    }

}

void reporte_tree_interfaz(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte tree.");
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
    char *nombreArchivo = "/reportetree.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);
        printf("\n Ubicacion %s", ubicacion);
    char *cab1 ="digraph inodo {rankdir=LR\n";
    char *cab2="node [shape=rectangle, style=\"filled\"];\n";
    char *endf ="}";
    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .dot creado!");
    }
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", cab1);
            fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab2);
            fprintf(reporte, "%s", "\n");
    fclose(reporte);
    bool_uno = 0;
    tree_report_recursivo(0 ,0 ,ubicacion);
    bool_uno = 0;
    reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", endf);
    fclose(reporte);
    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s\n", comandoFinal);
    system(comandoFinal);
    printf("\n fin reporte.");
}

void reporte_indirecto_recursivo(char *ubicacion, int grado, int pos_actual, int pos_anterior, int tipo, int b_inodo)
{
    printf("\n indirecto recursivo!");
    if(grado == 1)
    {
        int actual = block_count;
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*pos_actual),SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        /*---------------------------------------------*/
        FILE *reporte = fopen(ubicacion, "a");
        fprintf(reporte, "%s", "block");
        fprintf(reporte, "%d", block_count);
        fprintf(reporte, "%s", "[label=\"bloque apuntador ");
        fprintf(reporte, "%c", '\n');
        int i;
        for(i = 0; i<16; i++)
        {
            fprintf(reporte, "%s", "\\n ptr");
            fprintf(reporte, "%d", i);
            fprintf(reporte, "%s", ":");
            fprintf(reporte, "%d", b_ptr->b_pointer[i]);
            fprintf(reporte, "%c", '\n');
        }
        fprintf(reporte, "%s", "\"];");
        if(b_inodo == 1)
        {
            fprintf(reporte, "%s", "nodo");
            fprintf(reporte, "%d", pos_anterior);
            fprintf(reporte, "%s", "->");
            fprintf(reporte, "%s", "block");
            fprintf(reporte, "%d", block_count);
            fprintf(reporte, "%s", ";");
        }
        else
        {
            fprintf(reporte, "%s", "block");
            fprintf(reporte, "%d", pos_anterior);
            fprintf(reporte, "%s", "->");
            fprintf(reporte, "%s", "block");
            fprintf(reporte, "%d", block_count);
            fprintf(reporte, "%s", ";");
        }
        fclose(reporte);
        ++block_count;
        /*---------------------------------------------*/
        for(i = 0; i<16; i++)
        {
            if(tipo == 1)
            {
                /*carpeta*/
                if(b_ptr->b_pointer[i] != -1)
                {
                    struct bloque_carpeta *b_folder = malloc(sizeof(struct bloque_carpeta));
                    file = fopen(val_direccion, "rb");
                    if(file != NULL)
                    {
                        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*b_ptr->b_pointer[i]),SEEK_SET);
                        fread(b_folder, sizeof(struct bloque_carpeta), 1, file);
                        fclose(file);
                    }
                    reporte = fopen(ubicacion, "a");
                    fprintf(reporte, "%s", "block");
                    fprintf(reporte, "%d", block_count);
                    fprintf(reporte, "%s", "[label=\"Bloque ");
                    fprintf(reporte, "%d", i);
                    fprintf(reporte, "%c", '\n');
                    int b;
                    for(b = 0; b < 4; b++)
                    {
                        fprintf(reporte, "%s", "\\n name");
                        fprintf(reporte, "%s",b_folder->b_content[b].b_name);
                        fprintf(reporte, "%c", '\n');
                        fprintf(reporte, "%s", "\\n inodo");
                        fprintf(reporte, "%d",b_folder->b_content[b].b_inodo);
                        fprintf(reporte, "%c", '\n');
                    }
                    fprintf(reporte, "%s", "\"fillcolor=darkorange];\n");
                    fprintf(reporte, "%s", "block");
                    fprintf(reporte, "%d", actual);
                    fprintf(reporte, "%s", "->");
                    fprintf(reporte, "%s", "block");
                    fprintf(reporte, "%d", block_count);
                    fprintf(reporte, "%s", ";");
                    fclose(reporte);
                    int actual2 = block_count;
                    block_count++;
                    for(b=0; b<4; b++)
                    {
                        if(b_folder->b_content[b].b_inodo != -1)
                        {
                            printf("\n de carpeta %d %d",b_folder->b_content[b].b_inodo, actual2);
                            printf("\n de carpeta");
                            tree_report_recursivo(b_folder->b_content[b].b_inodo, actual2, ubicacion);
                        }
                    }
                }
            }
            else
            {
                /*Archivo*/
                if(b_ptr->b_pointer[i]  != -1)
                {
                    struct bloque_archivo *b_file = malloc(sizeof(struct bloque_archivo));
                    file = fopen(val_direccion, "rb");
                    if(file != NULL)
                    {
                        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*b_ptr->b_pointer[i]),SEEK_SET);
                        fread(b_file, sizeof(struct bloque_archivo), 1, file);
                        fclose(file);
                    }
                    reporte = fopen(ubicacion, "a");
                    fprintf(reporte, "%s", "block");
                    fprintf(reporte, "%d", block_count);
                    fprintf(reporte, "%s", "[label=\"Bloque ");
                    fprintf(reporte, "%d", i);
                    fprintf(reporte, "%c", '\n');
                    char *c = b_file->b_content;
                    fprintf(reporte, "%s", c);
                    fprintf(reporte, "%s", "\"];");
                    fprintf(reporte, "%s", "block");
                    fprintf(reporte, "%d", actual);
                    fprintf(reporte, "%s", "->");
                    fprintf(reporte, "%s", "block");
                    fprintf(reporte, "%d", block_count);
                    fprintf(reporte, "%s", ";");
                    fclose(reporte);
                    block_count++;
                }

            }
        }
    }
    else
    {
        int actual = block_count;
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*pos_actual),SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        /*---------------------------------------------*/
        FILE *reporte = fopen(ubicacion, "a");
        fprintf(reporte, "%s", "bloque");
        fprintf(reporte, "%d", actual);
        fprintf(reporte, "%s", "[label=\"bloque apuntador ");
        fprintf(reporte, "%c", '\n');
        int i;
        for(i = 0; i<16; i++)
        {
            fprintf(reporte, "%s", "\\n ptr");
            fprintf(reporte, "%d", i);
            fprintf(reporte, "%s", ":");
            fprintf(reporte, "%d", b_ptr->b_pointer[i]);
            fprintf(reporte, "%c", '\n');
        }
        fprintf(reporte, "%s", "\"fillcolor=gold];\n");
        if(b_inodo == 1)
        {
            fprintf(reporte, "%s", "nodo");
            fprintf(reporte, "%d", pos_anterior);
            fprintf(reporte, "%s", "->");
            fprintf(reporte, "%s", "block");
            fprintf(reporte, "%d", actual);
            fprintf(reporte, "%s", ";");
        }
        else
        {
            fprintf(reporte, "%s", "block");
            fprintf(reporte, "%d", pos_anterior);
            fprintf(reporte, "%s", "->");
            fprintf(reporte, "%s", "block");
            fprintf(reporte, "%d", actual);
            fprintf(reporte, "%s", ";");
        }
        ++block_count;
        for(i = 0; i<16; i++)
        {
            reporte_indirecto_recursivo(ubicacion, grado-1, b_ptr->b_pointer[i], actual, tipo, 0);
        }

    }
}

void tree_report_recursivo(int pos_inodo_actual, int pos_anterior, char *ubicacion)
{
    printf("\n TREE REPORTE pos actual: [%d][%d]", pos_inodo_actual, pos_anterior);
    struct inodo *inodo_lectura = malloc(sizeof(struct inodo));
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_inode_start+(sizeof(struct inodo)*pos_inodo_actual),SEEK_SET);
        fread(inodo_lectura, sizeof(struct inodo), 1, file);
        fclose(file);
    }
    char *nlink = inodo_lectura->i_nlink;
    char *pathlink = inodo_lectura->i_pathlink;
    char *atime = inodo_lectura->i_atime;
    char *ctime = inodo_lectura->i_ctime;
    char *mtime = inodo_lectura->i_mtime;
    char *enter = "\\n";
    char salto = '\n';

    FILE *reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", "nodo");
    fprintf(reporte, "%d", pos_inodo_actual);
    fprintf(reporte, "%s", "[label=\"Inodo ");
    fprintf(reporte, "%d", pos_inodo_actual);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\npathlink: ");
    fprintf(reporte, "%s", pathlink);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\nuid: ");
    fprintf(reporte, "%d", inodo_lectura->i_uid);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\ngid: ");
    fprintf(reporte, "%d", inodo_lectura->i_gid);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\nsize: ");
    fprintf(reporte, "%d", inodo_lectura->i_size);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\natime: ");
    fprintf(reporte, "%s", atime);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\nctime: ");
    fprintf(reporte, "%s", ctime);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\nmtime: ");
    fprintf(reporte, "%s", mtime);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\ntype: ");
    fprintf(reporte, "%c", inodo_lectura->i_type);
    fprintf(reporte, "%c", salto);

    fprintf(reporte, "%s", "\\nperm: ");
    fprintf(reporte, "%d", inodo_lectura->i_perm);
    fprintf(reporte, "%c", salto);
    fclose(reporte);

    int i;
    for(i = 0; i<15; i++)
    {
        reporte = fopen(ubicacion, "a");
        if(i < 12)
        {
            fprintf(reporte, "%s", "\\nAD");
            fprintf(reporte, "%d", i);
            fprintf(reporte, "%s", ":");
        }
        else
        {
            fprintf(reporte, "%s", "\\nAI");
            fprintf(reporte, "%d", i);
            fprintf(reporte, "%s", ":");
        }
        fprintf(reporte, "%d", inodo_lectura->i_block[i]);
        fprintf(reporte, "%c", salto);
        fclose(reporte);
    }
    reporte = fopen(ubicacion, "a");
    if(inodo_lectura->i_type == '1')
    {
        /*Carpeta*/
        fprintf(reporte, "%s", "\"fillcolor=cadetblue1];\n");
        fclose(reporte);
        int d;
        for(d = 0; d<12; d++)
        {
            if(inodo_lectura->i_block[d] != -1)
            {
                int desplazamiento = 0;
                if(d == 0)
                {
                    desplazamiento = 2;
                }
                struct bloque_carpeta *b_folder = malloc(sizeof(struct bloque_carpeta));
                file = fopen(val_direccion, "rb");
                if(file != NULL)
                {
                    fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*inodo_lectura->i_block[d]),SEEK_SET);
                    fread(b_folder, sizeof(struct bloque_carpeta), 1, file);
                    fclose(file);
                }
                reporte = fopen(ubicacion, "a");
                fprintf(reporte, "%s", "block");
                fprintf(reporte, "%d", block_count);
                fprintf(reporte, "%s", "[label=\"Bloque ");
                fprintf(reporte, "%d", d);
                fprintf(reporte, "%c", salto);
                fprintf(reporte, "%s", "\\n_______________");
                fprintf(reporte, "%s", "\\n_______________");
                int b;
                for(b = 0; b < 4; b++)
                {
                    fprintf(reporte, "%s", "\\n name: ");
                    fprintf(reporte, "%s",b_folder->b_content[b].b_name);
                    fprintf(reporte, "%c", salto);
                    fprintf(reporte, "%s", "\\n inodo: ");
                    fprintf(reporte, "%d",b_folder->b_content[b].b_inodo);
                    fprintf(reporte, "%c", salto);
                    fprintf(reporte, "%s", "\\n_______________");
                }
                fprintf(reporte, "%s", "\"fillcolor=darkorange];\n");
                fprintf(reporte, "%c", salto);
                fprintf(reporte, "%s", "nodo");
                fprintf(reporte, "%d", pos_inodo_actual);
                fprintf(reporte, "%s", "->");
                fprintf(reporte, "%s", "block");
                fprintf(reporte, "%d", block_count);
                fprintf(reporte, "%s", ";");
                fprintf(reporte, "%c", salto);
                fclose(reporte);
                b = desplazamiento;
                int anterior = block_count;
                block_count++;
                printf("\n DESPLAZAMIENTO: %d" ,desplazamiento);
                for(; b<4; b++)
                {
                    if(b_folder->b_content[b].b_inodo != -1)
                    {
                        printf("\n [%d][%d]b[%d]",b_folder->b_content[b].b_inodo, anterior,b);
                        tree_report_recursivo(b_folder->b_content[b].b_inodo, anterior, ubicacion);
                    }
                }
            }
        }
        for(d = 12; d<15; d++)
        {
            if(inodo_lectura->i_block[d] != -1)
            {
                reporte_indirecto_recursivo(ubicacion, d-11, inodo_lectura->i_block[d], pos_inodo_actual, 1, 1);
            }
        }
    }
    else
    {
        /*Archivo*/
        fprintf(reporte, "%s", "\"fillcolor=greenyellow];\n");
        fclose(reporte);
        int d;
        for(d = 0; d<12; d++)
        {
            if(inodo_lectura->i_block[d] != -1)
            {
                int desplazamiento = 0;
                if(d == 0)
                {
                    desplazamiento = 2;
                }
                struct bloque_archivo *b_file = malloc(sizeof(struct bloque_archivo));
                file = fopen(val_direccion, "rb");
                if(file != NULL)
                {
                    fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*inodo_lectura->i_block[d]),SEEK_SET);
                    fread(b_file, sizeof(struct bloque_archivo), 1, file);
                    fclose(file);
                }
                reporte = fopen(ubicacion, "a");
                fprintf(reporte, "%s", "block");
                fprintf(reporte, "%d", block_count);
                fprintf(reporte, "%s", "[label=\"");
                fprintf(reporte, "%d", d);
                fprintf(reporte, "%c", salto);
                char *c = b_file->b_content;
                fprintf(reporte, "%s", c);
                fprintf(reporte, "%s", "\"fillcolor=gold];\n");
                fprintf(reporte, "%c", salto);
                fprintf(reporte, "%s", "nodo");
                fprintf(reporte, "%d", pos_inodo_actual);
                fprintf(reporte, "%s", "->");
                fprintf(reporte, "%s", "block");
                fprintf(reporte, "%d", block_count);
                fprintf(reporte, "%s", ";");
                fprintf(reporte, "%c", salto);
                fclose(reporte);
                block_count++;
            }
        }
        for(d = 12; d<15; d++)
        {
            if(inodo_lectura->i_block[d] != -1)
            {
                reporte_indirecto_recursivo(ubicacion, d-11, inodo_lectura->i_block[d], pos_inodo_actual, 0, 1);
            }
        }
    }

    if(pos_anterior == 0 && pos_inodo_actual == 0)
    {
    }else
    {
        reporte = fopen(ubicacion, "a");
        fprintf(reporte, "%s", "\n block");
        fprintf(reporte, "%d", pos_anterior);
        fprintf(reporte, "%s", "->");
        fprintf(reporte, "%s", "nodo");
        fprintf(reporte, "%d", pos_inodo_actual);
        fprintf(reporte, "%s", ";");
        fclose(reporte);
    }
}

void cast2to3()
{
    cargar_superbloque();
    super_bloque_principal->s_filesystem_type = 3;
    FILE *file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, inicio_particion_publico ,SEEK_SET);
        fwrite(super_bloque_principal, sizeof(struct super_bloque), 1, file);
        fclose(file);
    }
}

void reporte_journaling(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte journaling.");

    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion_original, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .txt creado!");
    }
    int inicio_jjj = inicio_particion_publico + (sizeof(struct super_bloque)) ;
    int contador = 0;
    int ultimo = 0;
    FILE *file;
    struct j_data *journaling = malloc(sizeof(struct j_data));
    while (ultimo == 0)
    {
        file = fopen(val_direccion, "rb");
        printf("\n contador :%d", contador);
        if(file != NULL)
        {
            fseek(file, inicio_jjj + (contador * sizeof(struct j_data)), SEEK_SET);
            fread(journaling, sizeof(struct j_data), 1, file);
            fclose(file);
        }
        if(contador != 0)
        {
            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "TIPO OPERACION: ");
            fclose(reporte);
            if(journaling->journal_tipo_operacion == 1)
            {
                reporte = fopen(ubicacion_original, "a");
                fprintf(reporte, "%s", "CREAR CARPETA\n");
                fclose(reporte);
            }
            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "NOMBRE:");
            fprintf(reporte, "%s", journaling->journal_nombre);
            fprintf(reporte, "%c", '\n');
            fclose(reporte);

            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "CONTENIDO:");
            fprintf(reporte, "%s", journaling->journal_contenido);
            fprintf(reporte, "%c", '\n');
            fclose(reporte);

            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "FECHA:");
            fprintf(reporte, "%s", journaling->journal_fecha);
            fprintf(reporte, "%c", '\n');
            fclose(reporte);

            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "PROPIETARIO:");
            fprintf(reporte, "%d", journaling->journal_propietario);
            fprintf(reporte, "%c", '\n');
            fclose(reporte);

            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "GRUPO:");
            fprintf(reporte, "%d", journaling->journal_grupo);
            fprintf(reporte, "%c", '\n');
            fclose(reporte);

            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%s", "PERMISO:");
            fprintf(reporte, "%d", journaling->journal_permisos);
            fprintf(reporte, "%c", '\n');
            fprintf(reporte, "%c", '\n');
            fclose(reporte);
        }
        ultimo = journaling->ultimo;
        ++contador;
    }
}

void reporte_bm_block(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte bm_block.");

    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion_original, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .txt creado!");
    }
    char inode_bitmap[super_bloque_principal->s_blocks_count];
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
        fread(inode_bitmap, super_bloque_principal->s_blocks_count, 1, file);
        fclose(file);
    }
    int contador = 1;
    int i;
    for(i = 0; i < super_bloque_principal->s_blocks_count; i++)
    {
        if(contador %20 == 0)
        {
            reporte = fopen(ubicacion_original, "a");
            fprintf(reporte, "%c", '\n');
            fclose(reporte);
        }
        reporte = fopen(ubicacion_original, "a");
        fprintf(reporte, "%c", inode_bitmap[i]);
        fprintf(reporte, "%s", "    ");
        fclose(reporte);
        ++contador;
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

void reporte_sb(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte superblock.");
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
    char *nombreArchivo = "/reportesb.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);

    char *cab1 ="digraph G {\n";
    char *cab2="node [shape=plaintext]\n";
    char *cab3="a [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\" 0 \" WIDTH=\"50%\">\n";
    char *endf ="</table>>];}";
    char *ope_tr = "\n<tr>";
    char *clo_tr = "</tr>\n";
    char *ope_td = "\n<td WIDTH=\"100\" HEIGHT=\"50\">";
    char *clo_td = "</td>\n";
    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .dot creado!");
    }
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", cab1);
    fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab2);
    fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab3);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);
    /*                  ESCRIBIENDO DATOS                   */
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Nombre");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Valor");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_inodes_count");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_inodes_count);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_blocks_count");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_blocks_count);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_free_blocks_count");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_free_blocks_count);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_free_inodes_count");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_free_inodes_count);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    char *d = super_bloque_principal->s_mtime;
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_mtime");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", d);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    d = super_bloque_principal->s_umtime;
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_umtime");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", d);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_mnt_count");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_mnt_count);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_magic");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_magic);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_inode_size");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_inode_size);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_inode_size");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_inode_size);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_block_size");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_block_size);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_first_ino");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_first_ino);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_first_blo");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_first_blo);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_bm_inode_start");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_bm_inode_start);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_bm_block_start");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_bm_block_start);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_inode_start");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_inode_start);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    reporte = fopen(ubicacion, "a");

    fprintf(reporte, "%s", ope_tr);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "s_block_start");
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%d", super_bloque_principal->s_block_start);
    fprintf(reporte, "%s", clo_td);

    fprintf(reporte, "%s", clo_tr);

    fclose(reporte);
    /*------------------------------------------------------*/
    /*                  ESCRIBIENDO DATOS                   */
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", endf);
    fclose(reporte);
    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s\n", comandoFinal);
    system(comandoFinal);
    printf("\n fin reporte.");
}

void print_indirecto(int actual_ptr, int grado)
{
    if(grado == 1)
    {
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*actual_ptr),SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        /*---------------------------------------------*/
        int i;
        for(i = 0; i<16; i++)
        {
            if(b_ptr->b_pointer[i] != -1)
            {
                struct bloque_archivo *b_file = malloc(sizeof(struct bloque_archivo));
                file = fopen(val_direccion, "rb");
                if(file != NULL)
                {
                    fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*b_ptr->b_pointer[i]),SEEK_SET);
                    fread(b_file, sizeof(struct bloque_archivo), 1, file);
                    fclose(file);
                }
                char *cntt = b_file->b_content;
                printf("%s", cntt);
            }
        }
    }else
    {
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*actual_ptr),SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        /*---------------------------------------------*/
        int i;
        for(i = 0; i<16; i++)
        {
            if(b_ptr->b_pointer[i]!=-1)
            {
                print_indirecto(b_ptr->b_pointer[i], grado -11);
            }
        }
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

void rep_block_indirecto(int indirecto_actual, int grado, int bool_carpeta, char *ubicacion)
{
    if(grado == 1)
    {
        int actual = block_count;
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*indirecto_actual),SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        FILE *reporte = fopen(ubicacion, "a");
        fprintf(reporte, "%s", "nodo");
        fprintf(reporte, "%d", contador_grafico_bloques);
        fprintf(reporte, "%s", "[label=\"Bloque indirecto\\n___________\\n___________");
        int i;
        for( i = 0; i<16; i++)
        {
            fprintf(reporte, "%s", "\\n ");
            fprintf(reporte, "%d", b_ptr->b_pointer[i]);
        }
        fprintf(reporte, "%s", "\"fillcolor=khaki1];\n");
        if(contador_grafico_bloques != 0)
        {
            fprintf(reporte, "%s", "nodo");
            fprintf(reporte, "%d", contador_grafico_bloques-1);
            fprintf(reporte, "%s", "->nodo");
            fprintf(reporte, "%d", contador_grafico_bloques);
            fprintf(reporte, "%s", ";\n");
        }
        contador_grafico_bloques++;
        fclose(reporte);
        if(bool_carpeta == 1)/*CARPETA*/
        {
            int abc;
            for(abc = 0; abc < 16; abc++)
            {
                if(b_ptr->b_pointer[abc] != -1)
                {
                    struct bloque_carpeta *b_carpeta = malloc(sizeof(struct bloque_carpeta));
                    FILE *file_reader = fopen(val_direccion, "rb");
                    if(file_reader != NULL)
                    {
                        fseek(file_reader, super_bloque_principal->s_block_start+(b_ptr->b_pointer[abc]*sizeof(struct bloque_carpeta)),SEEK_SET);
                        fread(b_carpeta, sizeof(struct bloque_carpeta), 1, file_reader);
                        fclose(file_reader);
                    }
                    reporte = fopen(ubicacion, "a");
                    fprintf(reporte, "%s", "nodo");
                    fprintf(reporte, "%d", contador_grafico_bloques);
                    fprintf(reporte, "%s", "[label=\"Bloque carpeta ");
                    fprintf(reporte, "%s", "\\n________________\\n________________");
                    fclose(reporte);
                    int y;
                    for(y = 0; y < 4;  y++)
                    {
                        if(b_carpeta->b_content[y].b_inodo == -1)
                        {
                            reporte = fopen(ubicacion, "a");
                            fprintf(reporte, "%s", " \\n ");
                            fprintf(reporte, "%d", y);
                            fprintf(reporte, "%s", ") name: (!)");
                            fclose(reporte);
                        }else
                        {
                            reporte = fopen(ubicacion, "a");
                            fprintf(reporte, "%s", " \\n ");
                            fprintf(reporte, "%d", y);
                            fprintf(reporte, "%s", ") name:");
                            char *c = b_carpeta->b_content[y].b_name;
                            fprintf(reporte, "%s", c);
                            fclose(reporte);
                        }
                    }
                    reporte = fopen(ubicacion, "a");
                    fprintf(reporte, "%s", " \\n inode:");
                    fprintf(reporte, "%d", b_carpeta->b_content[y].b_inodo);
                    fprintf(reporte, "%s", "\"fillcolor=deepskyblue2];\n");
                    fclose(reporte);
                    if(contador_grafico_bloques != 0)
                    {
                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", "nodo");
                        fprintf(reporte, "%d", contador_grafico_bloques-1);
                        fprintf(reporte, "%s", "->nodo");
                        fprintf(reporte, "%d", contador_grafico_bloques);
                        fprintf(reporte, "%s", ";\n");
                        fclose(reporte);
                    }
                    contador_grafico_bloques++;
                }
            }
        }
        else
        {
            int abc;
            for(abc = 0; abc < 16; abc++)
            {
                if(b_ptr->b_pointer[abc] != -1)
                {
                    struct bloque_archivo *b_file = malloc(sizeof(struct bloque_archivo));
                    FILE *file_reader = fopen(val_direccion, "rb");
                    if(file_reader != NULL)
                    {
                        fseek(file_reader, super_bloque_principal->s_block_start+(b_ptr->b_pointer[abc]*sizeof(struct bloque_archivo)),SEEK_SET);
                        fread(b_file, sizeof(struct bloque_archivo), 1, file_reader);
                        fclose(file_reader);
                    }
                    reporte = fopen(ubicacion, "a");
                    fprintf(reporte, "%s", "nodo");
                    fprintf(reporte, "%d", contador_grafico_bloques);
                    fprintf(reporte, "%s", "[label=\"Bloque archivo ");
                    fprintf(reporte, "%s", "\\n________________\\n________________\\n ");
                    char *c = b_file->b_content;;
                    fprintf(reporte, "%s", c);
                    fclose(reporte);

                    reporte = fopen(ubicacion, "a");
                    fprintf(reporte, "%s", "\"fillcolor=springgreen2];\n");
                    fclose(reporte);
                    if(contador_grafico_bloques != 0)
                    {
                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", "nodo");
                        fprintf(reporte, "%d", contador_grafico_bloques -1);
                        fprintf(reporte, "%s", "->nodo");
                        fprintf(reporte, "%d", contador_grafico_bloques);
                        fprintf(reporte, "%s", ";\n");
                        fclose(reporte);
                    }
                    contador_grafico_bloques++;
                }
            }
        }
    }
    else
    {
        int actual = block_count;
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*indirecto_actual),SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        FILE *reporte = fopen(ubicacion, "a");
        fprintf(reporte, "%s", "nodo");
        fprintf(reporte, "%d", contador_grafico_bloques);
        fprintf(reporte, "%s", "[label=\"Bloque indirecto\\n___________\\n___________");
        int i;
        for( i = 0; i<16; i++)
        {
            fprintf(reporte, "%s", "\\n ");
            fprintf(reporte, "%d", b_ptr->b_pointer[i]);
        }
        fprintf(reporte, "%s", "\"fillcolor=khaki1];\n");
        if(contador_grafico_bloques != 0)
        {
            fprintf(reporte, "%s", "nodo");
            fprintf(reporte, "%d", contador_grafico_bloques-1);
            fprintf(reporte, "%s", "->nodo");
            fprintf(reporte, "%d", contador_grafico_bloques);
            fprintf(reporte, "%s", ";\n");
        }
        contador_grafico_bloques++;
        fclose(reporte);
        for(i = 0; i<16; i++)
        {
            if(b_ptr->b_pointer[i]!= -1)
            {
                rep_block_indirecto(b_ptr->b_pointer[i], grado-1, bool_carpeta, ubicacion);
            }
        }
    }
}

void rep_block(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte block.");
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
    char *nombreArchivo = "/reporteblock.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);
    char *cab1 ="digraph inodo {rankdir=LR\n";
    char *cab2="node [shape=rectangle, style=\"rounded, filled\"];\n";
    char *endf ="}";
    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .dot creado!");
    }
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", cab1);
            fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab2);
            fprintf(reporte, "%s", "\n");
    fclose(reporte);
    /*************************************************/
    printf("\n Reporte recursivo...");
    char inodebitmap[super_bloque_principal->s_inodes_count];
    FILE *file_reader = fopen(val_direccion, "rb");
    if(file_reader != NULL)
    {
        fseek(file_reader, super_bloque_principal->s_bm_inode_start,SEEK_SET);
        fread(inodebitmap, super_bloque_principal->s_inodes_count, 1, file_reader);
        fclose(file_reader);
    }
    contador_grafico_bloques = 0;
    int fillcolor = 1;
    int anterior = 0;
    int d;
    printf("\n Cantidad de inodos: %d", super_bloque_principal->s_inodes_count);
    for(d = 0; d< super_bloque_principal->s_inodes_count; d++)
    {
        if(inodebitmap[d] == '1')
        {
            printf("\n >>>>>>>>>INODO");
            struct inodo *inodo_r = malloc(sizeof(struct inodo));
            file_reader = fopen(val_direccion, "rb");
            if(file_reader != NULL)
            {
                fseek(file_reader, super_bloque_principal->s_inode_start+(d*sizeof(struct inodo)),SEEK_SET);
                fread(inodo_r, sizeof(struct inodo), 1, file_reader);
                fclose(file_reader);
            }
            if(inodo_r->i_type == '1')/*CARPETA*/
            {
                int abc;
                for(abc = 0; abc < 12; abc++)
                {
                    if(inodo_r->i_block[abc] != -1)
                    {
                        struct bloque_carpeta *b_carpeta = malloc(sizeof(struct bloque_carpeta));
                        FILE *file_reader = fopen(val_direccion, "rb");
                        if(file_reader != NULL)
                        {
                            fseek(file_reader, super_bloque_principal->s_block_start+(inodo_r->i_block[abc]*sizeof(struct bloque_carpeta)),SEEK_SET);
                            fread(b_carpeta, sizeof(struct bloque_carpeta), 1, file_reader);
                            fclose(file_reader);
                        }
                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", "nodo");
                        fprintf(reporte, "%d", contador_grafico_bloques);
                        fprintf(reporte, "%s", "[label=\"Bloque carpeta ");
                        fprintf(reporte, "%s", "\\n________________\\n________________");
                        fclose(reporte);
                        int y;
                        for(y = 0; y < 4;  y++)
                        {
                            if(b_carpeta->b_content[y].b_inodo == -1)
                            {
                                reporte = fopen(ubicacion, "a");
                                fprintf(reporte, "%s", " \\n ");
                                fprintf(reporte, "%d", y);
                                fprintf(reporte, "%s", ") name: (!)");
                                fclose(reporte);
                            }else
                            {
                                reporte = fopen(ubicacion, "a");
                                fprintf(reporte, "%s", " \\n ");
                                fprintf(reporte, "%d", y);
                                fprintf(reporte, "%s", ") name:");
                                char *c = b_carpeta->b_content[y].b_name;
                                fprintf(reporte, "%s", c);
                                fclose(reporte);
                            }
                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", " \\n inode:");
                        fprintf(reporte, "%d", b_carpeta->b_content[y].b_inodo);
                        fclose(reporte);
                        }
                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", "\"fillcolor=deepskyblue2];\n");
                        fclose(reporte);
                        if(contador_grafico_bloques != 0)
                        {
                            reporte = fopen(ubicacion, "a");
                            fprintf(reporte, "%s", "nodo");
                            fprintf(reporte, "%d", contador_grafico_bloques-1);
                            fprintf(reporte, "%s", "->nodo");
                            fprintf(reporte, "%d", contador_grafico_bloques);
                            fprintf(reporte, "%s", ";\n");
                            fclose(reporte);
                        }
                        contador_grafico_bloques++;
                    }
                }
                for(abc = 12; abc < 15; abc++)
                {
                    if(inodo_r->i_block[abc] != -1)
                    {
                        rep_block_indirecto(inodo_r->i_block[abc], abc-11, 1, ubicacion);
                    }
                }
            }
            else
            {
                int abc;
                for(abc = 0; abc < 12; abc++)
                {
                    if(inodo_r->i_block[abc] != -1)
                    {
                        struct bloque_archivo *b_file = malloc(sizeof(struct bloque_archivo));
                        FILE *file_reader = fopen(val_direccion, "rb");
                        if(file_reader != NULL)
                        {
                            fseek(file_reader, super_bloque_principal->s_block_start+(inodo_r->i_block[abc]*sizeof(struct bloque_archivo)),SEEK_SET);
                            fread(b_file, sizeof(struct bloque_archivo), 1, file_reader);
                            fclose(file_reader);
                        }
                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", "nodo");
                        fprintf(reporte, "%d", contador_grafico_bloques);
                        fprintf(reporte, "%s", "[label=\"Bloque archivo ");
                        fprintf(reporte, "%s", "\\n________________\\n________________\\n ");
                        char *c = b_file->b_content;;
                        fprintf(reporte, "%s", c);
                        fclose(reporte);

                        reporte = fopen(ubicacion, "a");
                        fprintf(reporte, "%s", "\"fillcolor=springgreen2];\n");
                        fclose(reporte);
                        if(contador_grafico_bloques != 0)
                        {
                            reporte = fopen(ubicacion, "a");
                            fprintf(reporte, "%s", "nodo");
                            fprintf(reporte, "%d", contador_grafico_bloques -1);
                            fprintf(reporte, "%s", "->nodo");
                            fprintf(reporte, "%d", contador_grafico_bloques);
                            fprintf(reporte, "%s", ";\n");
                            fclose(reporte);
                        }
                        contador_grafico_bloques++;
                    }
                }
                for(abc = 12; abc < 15; abc++)
                {
                    if(inodo_r->i_block[abc] != -1)
                    {
                        rep_block_indirecto(inodo_r->i_block[abc], abc-11, 0, ubicacion);
                    }
                }
            }
        }
    }

    /*************************************************/
    reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", endf);
    fclose(reporte);
    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s\n", comandoFinal);
    system(comandoFinal);
    printf("\n fin reporte.");
}

void inode_report_interfaz(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte inode.");
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
    char *nombreArchivo = "/reporteinode.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);
    char *cab1 ="digraph inodo {rankdir=LR\n";
    char *cab2="node [shape=rectangle, style=\"rounded, filled\"];\n";
    char *endf ="}";
    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .dot creado!");
    }
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", cab1);
            fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab2);
            fprintf(reporte, "%s", "\n");
    fclose(reporte);
    inode_report_recursivo(ubicacion);
    reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", endf);
    fclose(reporte);
    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s\n", comandoFinal);
    system(comandoFinal);
    printf("\n fin reporte.");
}

void inode_report_recursivo(char *ubicacion)
{
    cargar_superbloque();
    printf("\n Reporte recursivo...");
    char inodebitmap[super_bloque_principal->s_inodes_count];
    FILE *file_reader = fopen(val_direccion, "rb");
    if(file_reader != NULL)
    {
        fseek(file_reader, super_bloque_principal->s_bm_inode_start,SEEK_SET);
        fread(inodebitmap, super_bloque_principal->s_inodes_count, 1, file_reader);
        fclose(file_reader);
    }
    int fillcolor = 1;
    int anterior = 0;
    int d;
    printf("\n Cantidad de inodos: %d", super_bloque_principal->s_inodes_count);
    for(d = 0; d< super_bloque_principal->s_inodes_count; d++)
    {
        if(inodebitmap[d] == '1')
        {
            struct inodo *inodo_r = malloc(sizeof(struct inodo));
            file_reader = fopen(val_direccion, "rb");
            if(file_reader != NULL)
            {
                fseek(file_reader, super_bloque_principal->s_inode_start+(d*sizeof(struct inodo)),SEEK_SET);
                fread(inodo_r, sizeof(struct inodo), 1, file_reader);
                fclose(file_reader);
            }
            int inodo_id = d;
            /*CONVERSIONES*/
            char inode_id[20];
            sprintf(inode_id, "%d", inodo_id);
            char inode_size[20];
            sprintf(inode_size, "%d", inodo_r->i_size);
            /*DECLARACIONES*/
            char *id = d;
            char *path_link = inodo_r->i_pathlink;
            char *atime = inodo_r->i_atime;
            char *ctime = inodo_r->i_ctime;
            char *mtime = inodo_r->i_mtime;
            char tipo = inodo_r->i_type;
            /*ESCRITURA*/
            FILE *reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "nodo");
            fprintf(reporte, "%d", inodo_id);
            fprintf(reporte, "%s", "[label=\"Inodo ");
            fprintf(reporte, "%d", id);
            fprintf(reporte, "%s", "  \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_pathlink: ");
            fprintf(reporte, "%s", path_link);
            fprintf(reporte, "%s", "  \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_size: ");
            fprintf(reporte, "%d", inodo_r->i_size);
            fprintf(reporte, "%s", "  \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_atime: ");
            fprintf(reporte, "%s", atime);
            fprintf(reporte, "%s", "  \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_ctime: ");
            fprintf(reporte, "%s", ctime);
            fprintf(reporte, "%s", " \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_mtime: ");
            fprintf(reporte, "%s", mtime);
            fprintf(reporte, "%s", " \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "user: ");
            fprintf(reporte, "%d", inodo_r->i_uid);
            fprintf(reporte, "%s", " \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "group: ");
            fprintf(reporte, "%d", inodo_r->i_gid);
            fprintf(reporte, "%s", " \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "permiso: ");
            fprintf(reporte, "%d", inodo_r->i_perm);
            fprintf(reporte, "%s", " \\n");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[0]: ");
            fprintf(reporte, "%d", inodo_r->i_block[0]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[1]: ");
            fprintf(reporte, "%d", inodo_r->i_block[1]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[2]: ");
            fprintf(reporte, "%d", inodo_r->i_block[2]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[3]: ");
            fprintf(reporte, "%d", inodo_r->i_block[3]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[4]: ");
            fprintf(reporte, "%d", inodo_r->i_block[4]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[5]: ");
            fprintf(reporte, "%d", inodo_r->i_block[5]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[6]: ");
            fprintf(reporte, "%d", inodo_r->i_block[6]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[7]: ");
            fprintf(reporte, "%d", inodo_r->i_block[7]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[8]: ");
            fprintf(reporte, "%d", inodo_r->i_block[8]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[9]: ");
            fprintf(reporte, "%d", inodo_r->i_block[9]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[10]: ");
            fprintf(reporte, "%d", inodo_r->i_block[10]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[11]: ");
            fprintf(reporte, "%d", inodo_r->i_block[11]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[12]: ");
            fprintf(reporte, "%d", inodo_r->i_block[12]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[13]: ");
            fprintf(reporte, "%d", inodo_r->i_block[13]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_block[14]: ");
            fprintf(reporte, "%d", inodo_r->i_block[14]);
            fprintf(reporte, "%s", " \\n");
            fclose(reporte);
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", "\n");
            fprintf(reporte, "%s", "i_type: ");
            fprintf(reporte, "%c", tipo);
            if(fillcolor == 1)
            {
                fillcolor = 2;
                fprintf(reporte, "%s", "\"fillcolor=cadetblue1];\n");
            }else if (fillcolor == 2)
            {
                fillcolor = 3;
                fprintf(reporte, "%s", "\" fillcolor=lightpink1];\n");
            }else if (fillcolor == 3)
            {
                fillcolor = 1;
                fprintf(reporte, "%s", "\" fillcolor=gold1];\n");
            }
            fclose(reporte);
            if(anterior == 0 && d == 0)
            {

            }else
            {
                reporte = fopen(ubicacion, "a");
                fprintf(reporte, "%s", "nodo");
                fprintf(reporte, "%d", anterior);
                fprintf(reporte, "%s", "->");
                fprintf(reporte, "%s", "nodo");
                fprintf(reporte, "%d", d);
                fprintf(reporte, "%s", ";\n");
                fclose(reporte);
                anterior = d;
            }
            /*ESCRITURA*/
        }
    }
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
    printf("\n Ejecutando llamada a graphviz = %s\n", comandoFinal);
    system(comandoFinal);
    printf("\n fin reporte.");
}

void chmod_u()
{
    cargar_superbloque();
    printf("\n chmod  recursivo...");
    char inodebitmap[super_bloque_principal->s_inodes_count];
    FILE *file_reader = fopen(val_direccion, "rb");
    if(file_reader != NULL)
    {
        fseek(file_reader, super_bloque_principal->s_bm_inode_start,SEEK_SET);
        fread(inodebitmap, super_bloque_principal->s_inodes_count, 1, file_reader);
        fclose(file_reader);
    }
    int i;
    for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
    {
        if(inodebitmap[i]=='1')
        {
            struct inodo *inodo_l = malloc(sizeof(struct inodo));
            file_reader = fopen(val_direccion, "rb");
            if(file_reader != NULL)
            {
                fseek(file_reader, super_bloque_principal->s_inode_start + (i*sizeof(struct inodo)),SEEK_SET);
                fread(inodo_l, sizeof(struct inodo), 1, file_reader);
                fclose(file_reader);
            }
            char *d =inodo_l->i_pathlink;
            if(strcmp(folder_direccion,d) == 0)
            {
                printf("\n Se encontro la carpeta/archivo buscado.");
                int modificar_permiso = 0;
                if(user_id == 1 || user_id == inodo_l->i_uid)
                {
                    printf("\n permiso aceptado user_id: %d inode->user: %d", user_id, inodo_l->i_uid);
                    modificar_permiso = 1;
                }
                if(modificar_permiso == 1)
                {
                    printf("\n Asignando permiso %d", val_size);
                    inodo_l->i_perm = val_size;
                    file_reader = fopen(val_direccion, "rb+");
                    if (file_reader != NULL)
                    {
                        fseek(file_reader, super_bloque_principal->s_inode_start + (i*sizeof(struct inodo)),SEEK_SET);
                        fwrite(inodo_l, sizeof(struct inodo), 1, file_reader);
                        fclose(file_reader);
                    }
                    if(bool_plus_r == 1)
                    {
                        printf("\n Iniciando comparacion recursiva.");
                        int j;
                        for(j = 0; j< super_bloque_principal->s_inodes_count; j++)
                        {
                            if(inodebitmap[j]=='1')
                            {
                                struct inodo *inodo_lr = malloc(sizeof(struct inodo));
                                file_reader = fopen(val_direccion, "rb");
                                if(file_reader != NULL)
                                {
                                    fseek(file_reader, super_bloque_principal->s_inode_start + (j*sizeof(struct inodo)),SEEK_SET);
                                    fread(inodo_lr, sizeof(struct inodo), 1, file_reader);
                                    fclose(file_reader);
                                }
                                char *dr =inodo_lr->i_pathlink;
                                printf("\n Contiene? %s %s", dr, folder_direccion);
                                if(strstr(dr, folder_direccion)!=NULL)
                                {
                                    printf("\n si esta contenido");
                                    int modificar_permiso2 = 0;
                                    if(user_id == 1 || user_id == inodo_lr->i_uid)
                                    {
                                        printf("\n permiso aceptado user_id: %d inode->user: %d", user_id, inodo_lr->i_uid);
                                        modificar_permiso2 = 1;
                                    }
                                    if(modificar_permiso2 == 1)
                                    {
                                            printf("\n Asignando permiso %d", val_size);
                                            inodo_lr->i_perm = val_size;
                                            file_reader = fopen(val_direccion, "rb+");
                                            if (file_reader != NULL)
                                            {
                                                fseek(file_reader, super_bloque_principal->s_inode_start + (j*sizeof(struct inodo)),SEEK_SET);
                                                fwrite(inodo_lr, sizeof(struct inodo), 1, file_reader);
                                                fclose(file_reader);
                                            }
                                        }
                                    }
                            }
                        }
                    }
                }
                else
                {
                    printf("\n NO SE MODIFICO, PERMISOS INSUFICIENTES.");
                }
            }
        }
    }
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
        for(i = 0; i < 4; i++)
        {
            if(mbr_p->mbr_partition[i].part_status == '1')
            {
                if(mbr_p->mbr_partition[i].part_type == 'e')
                {
                   int next = mbr_p->mbr_partition[i].part_start;
                   struct ebr_extended *ebr_l = malloc(sizeof(struct ebr_extended));
                   while(next != -1)
                   {
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, next ,SEEK_SET);
                            fread(ebr_l, sizeof(struct ebr_extended), 1, file);
                            fclose(file);
                            next = ebr_l->part_next;
                        }
                        char *name = ebr_l->part_name;
                        if(strcmp(nombre_fdisk, name)==0)
                        {
                            int k;
                            int x = 0;
                            int y = 0;
                            int guardado = 0;
                            for(k = 0; k<4; k++)
                            {
                                int j;
                                for(j = 0; j<4; j++)
                                {
                                    if(k == 0)
                                    {
                                        if(strcmp(fila1[j], no)==0 && guardado==0)
                                        {
                                            printf("\n Montando particion en x:%d y:%d", j, k);
                                            next = -1;
                                            x = j;
                                            y = k;
                                            fila1[j] = id;
                                            part_name1[j] = nombre_fdisk;
                                            path1[j] = val_direccion;
                                            guardado = 1;
                                        }
                                    }
                                    /*222222222222222222222222222222222222222222222222222222222*/
                                    if(k == 1)
                                    {
                                        if(strcmp(fila2[j], no)==0 && guardado==0)
                                        {
                                            printf("\n Montando particion en x:%d y:%d", j, k);
                                            next = -1;
                                            x = j;
                                            y = k;
                                            fila2[j] = id;
                                            part_name2[j] = nombre_fdisk;
                                            path2[j] = val_direccion;
                                            guardado = 1;
                                        }
                                    }
                                    /*333333333333333333333333333333333333333333333333333333333333*/
                                    if(k == 2)
                                    {
                                        if(strcmp(fila3[j], no)==0 && guardado==0)
                                        {
                                            printf("\n Montando particion en x:%d y:%d", j, k);
                                            next = -1;
                                            x = j;
                                            y = k;
                                            fila3[j] = id;
                                            part_name3[j] = nombre_fdisk;
                                            path3[j] = val_direccion;
                                            guardado = 1;
                                        }
                                    }
                                    /*444444444444444444444444444444444444444444444444444444444444*/
                                    if(k == 3)
                                    {
                                        if(strcmp(fila4[j], no)==0 && guardado==0)
                                        {
                                            printf("\n Montando particion en x:%d y:%d", j, k);
                                            next = -1;
                                            x = j;
                                            y = k;
                                            fila4[j] = id;
                                            part_name4[j] = nombre_fdisk;
                                            path4[j] = val_direccion;
                                            guardado = 1;
                                        }
                                    }
                                }
                            }
                       }
                       printf("\n Existe la particion en el disco!");
                    }
                }
            }
        }
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

void cargar_superbloque()
{
    cargarArchivoDisco2();
    printf("\n Cargando superbloque...");
    int inicio_particion = 0;
    int bool_encontrado = 0;
    int i;
    for(i = 0; i<4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status = '1')
        {
            char *part_name_char = mbr_p->mbr_partition[i].part_name;
            printf("\n Buscando particion [carga_sb]: [[%s]] -> [[%s]]", part_name_char, val_partname);
            if(strcmp(part_name_char, val_partname)==0)
            {
                printf("\n Encontrado con fit %c.", mbr_p->mbr_partition[i].part_fit);
                inicio_particion = mbr_p->mbr_partition[i].part_start;
                inicio_particion_publico = inicio_particion;
                tamano_particion_publico = mbr_p->mbr_partition[i].part_size;
                ajuste_PUBLICO = mbr_p->mbr_partition[i].part_fit;
                printf("\n Desplazando al inicio de particion [%d]", mbr_p->mbr_partition[i].part_start);
                bool_encontrado = 1;
                break;
            }
        }
    }
    if(bool_encontrado == 0)
    {
        printf("\n Verificar logicas, no es p/e");
        for(i = 0; i < 4; i++)
        {
            if(mbr_p->mbr_partition[i].part_status == '1')
            {
                if(mbr_p->mbr_partition[i].part_type == 'e')
                {
                   int next = mbr_p->mbr_partition[i].part_start;
                   struct ebr_extended *ebr_l = malloc(sizeof(struct ebr_extended));
                   while(next != -1)
                   {
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, next ,SEEK_SET);
                            fread(ebr_l, sizeof(struct ebr_extended), 1, file);
                            fclose(file);
                            next = ebr_l->part_next;
                        }
                        char *name = ebr_l->part_name;
                        printf("\n Comparando %s %s", nombre_fdisk, name);
                        if(strcmp(val_partname, name)==0)
                        {
                            next = -1;
                            printf("\n Encontrado con fit %c.", ebr_l->part_fit);
                            inicio_particion = ebr_l->part_start+ sizeof(struct ebr_extended);
                            inicio_particion_publico = inicio_particion ;
                            tamano_particion_publico = ebr_l->part_size;
                            ajuste_PUBLICO = ebr_l->part_fit;
                            printf("\n Desplazando al inicio de particion [%d]", ebr_l->part_start);
                            bool_encontrado = 1;
                        }
                   }
                }
            }
        }
    }
        if(bool_encontrado == 1)
        {
            struct super_bloque *sup_b = malloc(sizeof(struct super_bloque));
            FILE * file = fopen(val_direccion, "rb");
            if(file != NULL)
            {
                fseek(file, inicio_particion ,SEEK_SET);
                fread(sup_b, sizeof(struct super_bloque), 1, file);
                fclose(file);
            }
            super_bloque_principal = sup_b;
            printf("\n *******************************************************");
            printf("\n [Carga del super_bloque exitosa]");
            printf("\n filesystem type:    %d", super_bloque_principal->s_filesystem_type);
            printf("\n total de inodos:    %d", super_bloque_principal->s_inodes_count);
            printf("\n total de bloques:   %d", super_bloque_principal->s_blocks_count);
            printf("\n inodos libres:      %d", super_bloque_principal->s_free_inodes_count);
            printf("\n bloques libres:     %d", super_bloque_principal->s_free_blocks_count);
            printf("\n inodo size:         %d", super_bloque_principal->s_inode_size);
            printf("\n block size:         %d", super_bloque_principal->s_block_size);
            printf("\n primer inodo libre: %d", super_bloque_principal->s_first_ino);
            printf("\n primer boque libre: %d", super_bloque_principal->s_first_blo);
            printf("\n inode bitmap:       %d", super_bloque_principal->s_bm_inode_start);
            printf("\n block bitmap:       %d", super_bloque_principal->s_bm_block_start);
            printf("\n inodo tabla:        %d", super_bloque_principal->s_inode_start);
            printf("\n bloque tabla:       %d", super_bloque_principal->s_block_start);
            printf("\n *******************************************************\n\n");
            struct inodo *i_inicial = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            total_recuperado = "";
            if(file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start ,SEEK_SET);
                fread(i_inicial, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            struct bloque_carpeta *i_bloque = malloc(sizeof(struct bloque_carpeta));
            file = fopen(val_direccion, "rb");
            if(file != NULL)
            {
                fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*i_inicial->i_block[0]) ,SEEK_SET);
                fread(i_bloque, sizeof(struct bloque_carpeta), 1, file);
                fclose(file);
            }
            printf("\n LEYENDO USUARIOS EN %d",i_bloque->b_content[2].b_inodo);
            struct inodo *i_user = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            if(file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start+(sizeof(struct inodo)*i_bloque->b_content[2].b_inodo) ,SEEK_SET);
                fread(i_user, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            int t;
            for(t = 0; t< 12; t++)
            {
                struct bloque_archivo *lectura = malloc(sizeof(struct bloque_archivo));
                /*printf("\n ! index[%d] = %d, ",t ,i_user->i_block[t]);*/
                if(i_user->i_block[t] != -1)
                {

                    file = fopen(val_direccion, "rb");
                    if(file != NULL)
                    {
                        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*i_user->i_block[t]) ,SEEK_SET);
                        fread(lectura, sizeof(struct bloque_archivo), 1, file);
                        fclose(file);
                    }
                    char piv[65];
                    int vip;
                    /*strcpy(piv, lectura->b_content);*/
                    /*printf("\n*-*-*-*-****-*-*-*-*-*-*-*-*--*-*-*-");*/
                    for(vip = 0; vip<64; vip++)
                    {
                        piv[vip] = lectura->b_content[vip];
                        printf("\n > %c", lectura->b_content[vip]);
                    }
                    piv[64]='\0';
                    /*printf("\n*-*-*-*-****-*-*-*-*-*-*-*-*--*-*-*-");*/
                    char *d = piv;
                    printf("\n >>>>>>>>>>>>>>>>>>>>>LEYENDO->%s\n", d);
                    total_recuperado = concatenacion(total_recuperado, piv);
                }
            }
            printf("\n Total recuperado en LECTURA: ------------------\n%s\n----------------------", total_recuperado);
        }else
        {
            printf("\n No se encontró la particion deseada.");
        }
}

int getGroup(char *groupname)
{
    int grupo = 0;
    int i;
    for(i = 0; i<10; i++)
    {
        char *p = grupos[i];
        printf("\n Comparando %s %s", groupname, p);
        if(strcmp(groupname, p)==0)
        {
            grupo = GUID[i];
            break;
        }
    }
    return grupo;
}

char* intTobinary(int number)
{
    printf("\n int to binary %d", number);
    char respuesta[3];
    respuesta[0] = '-';
    respuesta[1] = '-';
    respuesta[2] = '-';
    if(number == 0)
    {
        respuesta[0] = '0';
        respuesta[1] = '0';
        respuesta[2] = '0';
    }else if(number == 1)
    {
        respuesta[0] = '0';
        respuesta[1] = '0';
        respuesta[2] = '1';
    }else if(number == 2)
    {
        respuesta[0] = '0';
        respuesta[1] = '1';
        respuesta[2] = '0';
    }else if(number == 3)
    {
        respuesta[0] = '0';
        respuesta[1] = '1';
        respuesta[2] = '1';
    }else if(number == 4)
    {
        respuesta[0] = '1';
        respuesta[1] = '0';
        respuesta[2] = '0';
    }else if(number == 5)
    {
        respuesta[0] = '1';
        respuesta[1] = '0';
        respuesta[2] = '1';
    }else if(number == 6)
    {
        respuesta[0] = '1';
        respuesta[1] = '1';
        respuesta[2] = '0';
    }else if(number == 7)
    {
        respuesta[0] = '1';
        respuesta[1] = '1';
        respuesta[2] = '1';
    }
    return respuesta;
}

char* getGroupChar(int groupId)
{
    char *grupo = "-";
    int i;
    for(i = 0; i<10; i++)
    {
        char *p ;
        printf("\n Comparando GroupChar %d %d", GUID[i], groupId);
        if(GUID[i] == groupId)
        {
            p = grupos[i];
            return p;
            break;
        }
    }
    return grupo;
}

char* getUserChar(int userId)
{
    char *grupo = "-";
    int i;
    for(i = 0; i<20; i++)
    {
        char *p ;
        printf("\n UserChar Comparando %d %d", UID[i], userId);
        if(UID[i] == userId)
        {
            p = usuarios_usr[i];
            return p;
            break;
        }
    }
    return grupo;
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
void reporte_ls(char *direccion, char *extension, char *ubicacion_original)
{
    cargar_superbloque();
    printf("\n Reporte LS(i/l).");
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
    char *nombreArchivo = "/reporteLS.dot";
    char *ubicacion = concatenacion(direccion, nombreArchivo);
    char *cab1 ="digraph G {\n";
    char *cab2="node [shape=plaintext]\n";
    char *cab3="a [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\" 0 \" WIDTH=\"50%\">\n";
    char *endf ="</table>>];}";
    char *ope_tr = "\n<tr>";
    char *clo_tr = "</tr>\n";
    char *ope_td = "\n<td WIDTH=\"100\" HEIGHT=\"50\">";
    char *clo_td = "</td>\n";
    char nada[1];
    nada[0] = ' ';
    FILE *reporte = fopen(ubicacion, "w");
    if(reporte != NULL)
    {
        fputc(nada[0], reporte);
        fclose(reporte);
        printf("\n Archivo .dot creado!");
    }
    /*-------------CABECERA DOT------------*/
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", cab1);
    fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab2);
    fprintf(reporte, "%s", "\n");
    fprintf(reporte, "%s", cab3);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);
    /*-------------CABECERA TABLA------------*/
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_tr);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);
    if(bool_ls_i == 1)
    {
        reporte = fopen(ubicacion, "a");
        fprintf(reporte, "%s", ope_td);
        fprintf(reporte, "%s", "Inodo");
        fprintf(reporte, "%s", clo_td);
        fprintf(reporte, "%s", "\n");
        fclose(reporte);
    }
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Permisos");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Propietario");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Links");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Grupo");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Size");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Fecha-Hora");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", ope_td);
    fprintf(reporte, "%s", "Nombre");
    fprintf(reporte, "%s", clo_td);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);

    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", clo_tr);
    fprintf(reporte, "%s", "\n");
    fclose(reporte);
    /*-------------CABECERA TABLA------------*/
    char inode_bitmap[super_bloque_principal->s_inodes_count];
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
        fread(inode_bitmap, super_bloque_principal->s_inodes_count, 1, file);
        fclose(file);
    }
    int i;
    for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
    {
        if(inode_bitmap[i] == '1')
        {
            struct inodo *inodo_l = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            if(file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start + (sizeof(struct inodo) * i) ,SEEK_SET);
                fread(inodo_l, sizeof(struct inodo), 1, file);
                fclose(file);
            }

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_tr);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            if(bool_ls_i == 1)
            {
                reporte = fopen(ubicacion, "a");
                fprintf(reporte, "%s", ope_td);
                fprintf(reporte, "%d", i);
                fprintf(reporte, "%s", clo_td);
                fprintf(reporte, "%s", "\n");
                fclose(reporte);
            }
            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            fprintf(reporte, "%d", inodo_l->i_perm);
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            char *p = getUserChar(inodo_l->i_uid);
            fprintf(reporte, "%s", p);
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            fprintf(reporte, "%s", "0");
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            char *g = getGroupChar(inodo_l->i_gid);
            fprintf(reporte, "%s", g);
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            fprintf(reporte, "%d", inodo_l->i_size);
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            char *fh = inodo_l->i_ctime;
            fprintf(reporte, "%s", fh);
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", ope_td);
            char *n = inodo_l->i_pathlink;
            printf("\n NOMBRE: %s", n);
            fprintf(reporte, "%s", n);
            fprintf(reporte, "%s", clo_td);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);

            reporte = fopen(ubicacion, "a");
            fprintf(reporte, "%s", clo_tr);
            fprintf(reporte, "%s", "\n");
            fclose(reporte);
        }
    }
    /*---------------DATOS TABLA-------------*/
    /*------------------------------------------------------*/
    /*                  ESCRIBIENDO DATOS                   */
    reporte = fopen(ubicacion, "a");
    fprintf(reporte, "%s", endf);
    fclose(reporte);
    /*################REALIZANDO EL COMANDO################*/
    char *comando = concatenacion(extensionDot, ubicacion);
    char *comando2 = concatenacion(comando, output);
    char *comandoFinal = concatenacion(comando2, ubicacion_original);
    printf("\n Ejecutando llamada a graphviz = %s\n", comandoFinal);
    system(comandoFinal);
    printf("\n fin reporte.");
}

int permiso(int user, int userGroup, int group, int permiso_i, int operacion)
{
    /*OPERACION(1-LECTURA, 2-ESCRITURA, 3-EJECUCION)*/
    printf("\n          PERMISOS");
    printf("\n inode_user: %d inode_group: %d actual_group: %d inode_permiso: %d operacion:%d usuario_loggeado: %d", user, userGroup, group, permiso_i, operacion, user_id);
    int resultado = 0;
    double permiso_d = (double)permiso_i;
    double piso_usuario = floor(permiso_d/100);
    double decena = permiso_d-(piso_usuario*100);
    double piso_grupo = floor(decena/10);
    double piso_otro = decena - piso_grupo*10;
    printf("\n Calculos: usuario:%f grupo:%f otro:%f", piso_usuario, piso_grupo, piso_otro);
    int permiso_usuario = (int)piso_usuario;
    int permiso_grupo = (int)piso_grupo;
    int permiso_otro = (int)piso_otro;
    printf("\n Calculos_int: usuario:%d grupo:%d otro:%d", permiso_usuario, permiso_grupo, permiso_otro);
    char *usuario_bin = strdup(intTobinary(permiso_usuario));
    char *grupo_bin = strdup(intTobinary(permiso_grupo));
    char *otro_bin = strdup(intTobinary(permiso_otro));
    printf("\n Calculos_char: usuario:%s grupo:%s otro:%s", usuario_bin, grupo_bin, otro_bin);
    printf("\n Usuario loggeado:%d del grupo%d", user_id, userGroup);
    if(user_id == 1)
    {
        printf("\n USUARIO ROOT!!!!");
        return 1;
    }
    if(operacion == 1)
    {
        printf("\n LECTURA.");
        if(user == user_id)/*USUARIO AUTOR*/
        {
            printf("\n USUARIO AUTOR.");
            if(*(usuario_bin + 0) == '1')
            {
                return 1;
            }
        }
        else if(userGroup == group)/*MISMO GRUPO*/
        {
            printf("\n MISMO GRUPO.");
            if(*(grupo_bin + 0) == '1')
            {
                return 1;
            }
        }
        else /*OTROS*/
        {
            printf("\n OTRO");
            if(*(otro_bin + 0) == '1')
            {
                return 1;
            }
        }
    }
    else if(operacion == 2)
    {
        printf("\n ESCRITURA.");
        if(user == user_id)/*USUARIO AUTOR*/
        {
            printf("\n USUARIO AUTOR. [%c]", *(usuario_bin + 1));
            if(*(usuario_bin + 1) == '1')
            {
                return 1;
            }
        }
        else if(userGroup == group)/*MISMO GRUPO*/
        {
            printf("\n MISMO GRUPO.");
            if(*(grupo_bin + 1) == '1')
            {
                return 1;
            }
        }
        else /*OTROS*/
        {
                        printf("\n OTRO");

            if(*(otro_bin + 1) == '1')
            {
                return 1;
            }
        }
    }
    else if(operacion == 3)
    {
        printf("\n EJECUCION.");
        if(user == user_id)/*USUARIO AUTOR*/
        {
            printf("\n USUARIO AUTOR.");
            if(*(usuario_bin + 2) == '1')
            {
                return 1;
            }
        }
        else if(userGroup == group)/*MISMO GRUPO*/
        {
            printf("\n MISMO GRUPO.");
            if(*(grupo_bin + 2) == '1')
            {
                return 1;
            }
        }
        else /*OTROS*/
        {            printf("\n OTRO");

            if(*(otro_bin + 2) == '1')
            {
                return 1;
            }
        }
    }
    return resultado;
}

char* permisoChar(int permiso_i)
{
    /*OPERACION(1-LECTURA, 2-ESCRITURA, 3-EJECUCION)*/
    printf("\n          PERMISOS");
    int resultado = 0;
    double permiso_d = (double)permiso_i;
    double piso_usuario = floor(permiso_d/100);
    double decena = permiso_d-(piso_usuario*100);
    double piso_grupo = floor(decena/10);
    double piso_otro = decena - piso_grupo*10;
    printf("\n Calculos: usuario:%f grupo:%f otro:%f", piso_usuario, piso_grupo, piso_otro);
    int permiso_usuario = (int)piso_usuario;
    int permiso_grupo = (int)piso_grupo;
    int permiso_otro = (int)piso_otro;
    printf("\n Calculos_int: usuario:%d grupo:%d otro:%d", permiso_usuario, permiso_grupo, permiso_otro);
    char *usuario_bin = intTobinary(permiso_usuario);
    char *grupo_bin = intTobinary(permiso_grupo);
    char *otro_bin = intTobinary(permiso_otro);
    printf("\n Calculos_char: usuario:%s grupo:%s otro:%s", usuario_bin, grupo_bin, otro_bin);
    char respuesta="";
    char *readd = "r";
    char *writee = "w";
    char *exec = "x";
    if(*(usuario_bin) == '1')
    {
        respuesta = concatenacion(respuesta, readd);
    }
    if(*(usuario_bin+1) == '1')
    {
        respuesta = concatenacion(respuesta, writee);
    }
    if(*(usuario_bin+2) == '1')
    {
        respuesta = concatenacion(respuesta, exec);
    }
    respuesta = concatenacion(respuesta, "-");
    if(*(grupo_bin) == '1')
    {
        respuesta = concatenacion(respuesta, readd);
    }
    if(*(grupo_bin+1) == '1')
    {
        respuesta = concatenacion(respuesta, writee);
    }
    if(*(grupo_bin+2) == '1')
    {
        respuesta = concatenacion(respuesta, exec);
    }
    respuesta = concatenacion(respuesta, "-");
    if(*(otro_bin) == '1')
    {
        respuesta = concatenacion(respuesta, readd);
    }
    if(*(otro_bin+1) == '1')
    {
        respuesta = concatenacion(respuesta, writee);
    }
    if(*(otro_bin+2) == '1')
    {
        respuesta = concatenacion(respuesta, exec);
    }
    respuesta = concatenacion(respuesta, "-");
    return respuesta;
}

void iniciar_sesion2()
{
    cargar_superbloque();
    char **usuariosYgrupos = str_split(total_recuperado, '\n');
    int arreglo_size = tamano(usuariosYgrupos);
    printf("\n usuarios y grupos: %d para %s", arreglo_size, total_recuperado);
    int i;
    char *g = "G";
    char *u = "U";
    printf("\n Cargando usuarios y grupos.");
    for(i=0; i<10; i++)
    {
        GUID[i]=0;
    }
    for(i=0; i<20; i++)
    {
        UID[i]=0;
    }
    for(i = 0; i< arreglo_size; i++)
    {
        char **informacion = str_split(*(usuariosYgrupos+i), ',');
        printf("\n TIPO:%s", *(informacion+1));
        if(strcmp(*(informacion+1),g)==0)
        {
            printf("\n Recuperando grupo.");
            int grp;
            for(grp=0; grp < 10; grp++)
            {
                if(GUID[grp]==0)
                {
                    GUID[grp] = atoi(*(informacion+0));
                    strcpy(grupos[grp], *(informacion+2));
                    break;
                }
            }
        }
        else
        {
            printf("\n Recuperando usuario.");
            int uusr;
            for(uusr=0; uusr < 20; uusr++)
            {
                if(UID[uusr]==0)
                {
                    UID[uusr] = atoi(*(informacion+0));
                    strcpy(usuarios_grp[uusr], *(informacion+2));
                    strcpy(usuarios_usr[uusr], *(informacion+3));
                    strcpy(usuarios_pwd[uusr], *(informacion+4));
                    break;
                }
            }
        }
    }
}

void iniciar_sesion()
{
    cargar_superbloque();
    char **usuariosYgrupos = str_split(total_recuperado, '\n');
    int arreglo_size = tamano(usuariosYgrupos);
    printf("\n usuarios y grupos: %d para %s", arreglo_size, total_recuperado);
    int i;
    char *g = "G";
    char *u = "U";
    printf("\n Cargando usuarios y grupos.");
    for(i = 0; i< arreglo_size; i++)
    {
        char **informacion = str_split(*(usuariosYgrupos+i), ',');
        printf("\n TIPO:%s", *(informacion+1));
        if(strcmp(*(informacion+1),g)==0)
        {
            printf("\n Recuperando grupo.");
            int grp;
            for(grp=0; grp < 10; grp++)
            {
                if(GUID[grp]==0)
                {
                    GUID[grp] = atoi(*(informacion+0));
                    strcpy(grupos[grp], *(informacion+2));
                    break;
                }
            }
        }
        else
        {
            printf("\n Recuperando usuario.");
            int uusr;
            for(uusr=0; uusr < 20; uusr++)
            {
                if(UID[uusr]==0)
                {
                    UID[uusr] = atoi(*(informacion+0));
                    strcpy(usuarios_grp[uusr], *(informacion+2));
                    strcpy(usuarios_usr[uusr], *(informacion+3));
                    strcpy(usuarios_pwd[uusr], *(informacion+4));
                    break;
                }
            }
        }
    }
    printf("\n Usuarios y grupos cargados.Intentando loggear %s contrasena %s", user, password);
    int existe = 0;
    for(i = 0; i<20; i++)
    {
        char *user_cmp = usuarios_usr[i];
        printf("\n Comparando [%s][%s]", user, user_cmp);
        if(strcmp(user, user_cmp) == 0)
        {
            printf("\n ID:%d", UID[i]);
            user_id = UID[i];
            user_group = usuarios_grp[i];
            existe = 1;
            break;
        }
    }
    user_group_id = getGroup(user_group);
    if(existe == 0)
    {
        user = "";
        password = "";
        bool_logueado = 0;
        printf("\n No se pudo iniciar sesión...");
    }else
    {
        bool_logueado = 1;
        printf("\n Sesión iniciada");
    }
}

void escribir_en_journaling(int operacion,
                            char tipo,
                            char *nombre,
                            int tipo_contenido,
                            char *journal_contenido,
                            char *journal_fecha,
                            int propietario,
                            int grupo,
                            int permisos,
                            int file_system_type
                            )
{
    /*
    * 1: crear carpeta
    * 2: crear archivo
    * 3: borrar carpeta
    * 4: borrar archivo
    * 5: modificar...
    * */
    printf("\n Escribir en journaling: nombre = %s , direccion = %s", nombre, journal_contenido);
    if(file_system_type == 3 && journal_permiso_escritura == 1)
    {
        struct j_data *journal = malloc(sizeof(struct j_data));
        struct j_data *journal_lectura = malloc(sizeof(struct j_data));
        journal->journal_tipo_operacion = operacion;
        journal->journal_tipo = tipo;
        strcpy(journal->journal_nombre, nombre);
        journal->journal_tipo_contenido = tipo_contenido;
        strcpy(journal->journal_contenido, journal_contenido);
        strcpy(journal->journal_fecha, journal_fecha);
        journal->journal_propietario = propietario;
        journal->journal_grupo = grupo;
        journal->journal_permisos = permisos;
        journal->ultimo = 1;
        int journal_pos_start = sizeof(struct super_bloque)+inicio_particion_publico;
        printf("\n Inicio particion publico: %d", inicio_particion_publico);
        int bool_escrito = 0;
        int desplazamiento = 0;
        FILE *file;
        while(bool_escrito == 0)
        {
            printf("\n despllazamiento : %d", desplazamiento);
            file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, journal_pos_start + (desplazamiento*sizeof(struct j_data)), SEEK_SET);
                fread(journal_lectura, sizeof(struct j_data), 1, file);
                fclose(file);
                /*printf("\n journal->ultimo = %d", journal_lectura->ultimo);*/
            }
            if(journal_lectura->ultimo == 1)
            {
                journal_lectura->ultimo = 0;
                file= fopen(val_direccion, "rb+");
                if (file != NULL)
                {
                    fseek(file, journal_pos_start + (desplazamiento*sizeof(struct j_data)) ,SEEK_SET);
                    fwrite(journal_lectura, sizeof(struct j_data), 1, file);
                    fclose(file);
                }
                file= fopen(val_direccion, "rb+");
                if (file != NULL)
                {
                    fseek(file, journal_pos_start + ((desplazamiento+1)*sizeof(struct j_data)) ,SEEK_SET);
                    fwrite(journal, sizeof(struct j_data), 1, file);
                    fclose(file);
                }
                bool_escrito = 1;
            }
            ++desplazamiento;
        }
    }
}

void formatear_Particion()
{
    cargarArchivoDisco2();
    printf("\n Formateando particion...");
    if(bool_unit == 0)
    {
        val_unit[0]='k';
    }
    if(bool_filesystem == 0)
    {
        val_filesystem = '3';
    }
    int bool_encontrado = 0;
    int tamano_particion;
    char tipo_particion;
    int inicio_particion;
    size_t tamano_superbloque;
    size_t tamano_nodos;
    size_t tamano_bloques;
    int i;
    for(i = 0; i < 4; i++)
    {
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            char *part_name_char = mbr_p->mbr_partition[i].part_name;
            printf("\n Buscando particion: [[%s]] -> [[%s]]", part_name_char, val_partname);
            if(strcmp(part_name_char, val_partname)==0)
            {
                printf("\n Encontrado.");
                tamano_particion = mbr_p->mbr_partition[i].part_size;
                tipo_particion = mbr_p->mbr_partition[i].part_type;
                inicio_particion = mbr_p->mbr_partition[i].part_start;
                bool_encontrado = 1;
                break;
            }
        }
    }
    if(bool_encontrado == 0)
    {
        printf("\n Verificar logicas, no es p/e");
        for(i = 0; i < 4; i++)
        {
            if(mbr_p->mbr_partition[i].part_status == '1')
            {
                if(mbr_p->mbr_partition[i].part_type == 'e')
                {
                   int next = mbr_p->mbr_partition[i].part_start;
                   struct ebr_extended *ebr_l = malloc(sizeof(struct ebr_extended));
                   while(next != -1)
                   {
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, next ,SEEK_SET);
                            fread(ebr_l, sizeof(struct ebr_extended), 1, file);
                            fclose(file);
                            next = ebr_l->part_next;
                        }
                        char *name = ebr_l->part_name;
                        if(strcmp(nombre_fdisk, name)==0)
                        {
                            next = -1;
                            printf("\n Encontrado.");
                            tamano_particion = ebr_l->part_size;
                            tipo_particion = 'l';
                            inicio_particion = ebr_l->part_start + sizeof(struct ebr_extended);
                            bool_encontrado = 1;
                        }
                   }
                }
            }
        }
    }
    if(bool_encontrado == 1)
    {
        struct j_data *journaling_struct = malloc(sizeof(struct j_data));
        struct super_bloque *super_block = malloc(sizeof(struct super_bloque));
        struct bloque_archivo *file_block = malloc(sizeof(struct bloque_archivo));
        struct bloque_carpeta *folder_block = malloc(sizeof(struct bloque_carpeta));
        /*Calcular bloques e inodos*************************************************/
        size_t superbloque = sizeof(struct super_bloque);
        size_t block = 64;
        size_t journaling = sizeof(struct j_data);
        size_t inodos = sizeof(struct inodo);
        double bloques = (tamano_particion - sizeof(struct super_bloque))/(1 + journaling + 3 + inodos + 3*block);
        int numero_estructuras = floor(bloques);
        printf("\n Numero de estructuras[%d] tam_part", numero_estructuras);
        /*Asignando valores*********************************************************/
        super_block->s_filesystem_type = val_filesystem - '0';
        super_block->s_inodes_count = numero_estructuras;
        super_block->s_blocks_count = numero_estructuras*3;
        super_block->s_free_blocks_count = numero_estructuras*3 - 1;
        super_block->s_free_inodes_count = numero_estructuras - 1;
        super_block->s_mnt_count = 1;
        super_block->s_magic= 0xEF53;
        super_block->s_inode_size = inodos;
        super_block->s_block_size = block;
        super_block->s_first_ino = 1;
        super_block->s_first_blo = 1;/*FECHA*/
        time_t t1 = time(NULL);
        struct tm *tm1 = localtime(&t1);
        strftime(super_block->s_mtime, sizeof(super_block->s_mtime), "%Y/%m/%d %H:%M", tm1);
        strftime(super_block->s_umtime, sizeof(super_block->s_umtime), "%Y/%m/%d %H:%M", tm1);
        /*FECHA*/
        /**************************************************************************/
        /*Escribiendo estructuras**************************************************/
        /*Escribiendo superbloque & journaling y calculando bitmap pos*/
        size_t inicio_inode_bitmap;
        /*if(super_block->s_filesystem_type == 3)
        {*/
            inicio_inode_bitmap = inicio_particion + superbloque + (journaling*numero_estructuras);
            printf("\n Despues del journaling %d", inicio_inode_bitmap);
        /*}else
        {
            inicio_inode_bitmap = inicio_particion + superbloque;
        }*/
        super_block->s_bm_inode_start = inicio_inode_bitmap;
        super_block->s_bm_block_start = inicio_inode_bitmap + numero_estructuras;
        super_block->s_inode_start = super_block->s_bm_block_start  + numero_estructuras*3;
        super_block->s_block_start = super_block->s_inode_start + numero_estructuras*inodos;

        printf("\n superblock_partition_start: [%d]", inicio_particion);
        printf("\n s_bitmap_inode_start: [%d]", super_block->s_bm_inode_start);
        printf("\n s_bitmap_block_start: [%d]", super_block->s_bm_block_start);
        printf("\n s_inodetable_start: [%d]", super_block->s_inode_start);
        printf("\n s_blocktable_start: [%d]", super_block->s_block_start);
        /*---------------------ESCRITURA DEL SUPERBLOQUE---------------------*/

        FILE * file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, inicio_particion ,SEEK_SET);
            fwrite(super_block, sizeof(struct super_bloque), 1, file);
            fclose(file);
            printf("\n Se escribió el super bloque en: %d",inicio_particion);
        }
        /*-------------------------------------------------------------------*/
        /*---------------ESCRITURA DEL JOURNALING---------------*/
        /*if(super_block->s_filesystem_type == 3)
        {*/
            journaling_struct->journal_tipo_operacion=0;
            journaling_struct->ultimo=1;
            /*/si es ext3 se debe escribir el journaling*/
            file= fopen(val_direccion, "rb+");
            if (file != NULL)
            {
                fseek(file, inicio_particion + sizeof(struct super_bloque) ,SEEK_SET);
                fwrite(journaling_struct, sizeof(struct j_data), 1, file);
                fclose(file);
            }
        /*}*/
        /*-------------------------------------------------------------------*/
        /*-------------------ESCRITURA DEL BITMAP DE INODOS------------------*/
        char inodebitmap[numero_estructuras];
        inodebitmap[0] = '1';
        inodebitmap[1] = '1';
        int d;
        for(d = 2; d< numero_estructuras; d++)
        {
            inodebitmap[d] = '0';
        }
        file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_block->s_bm_inode_start ,SEEK_SET);
            fwrite(inodebitmap, sizeof(inodebitmap), 1, file);
            fclose(file);
        }
        /*-------------------------------------------------------------------*/
        /*------------------ESCRITURA DEL BITMAP DE BLOQUES------------------*/
        char blockbitmap[numero_estructuras*3];
        blockbitmap[0] = '1';
        blockbitmap[1] = '1';
        for(d = 2; d< numero_estructuras*3; d++)
        {
            blockbitmap[d] = '0';
        }
        file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_block->s_bm_block_start ,SEEK_SET);
            fwrite(blockbitmap, sizeof(blockbitmap), 1, file);
            fclose(file);
        }
        /*-------------------------------------------------------------------*/
        /*----------------------ESCRITURA DEL INODO ROOT---------------------*/
        struct inodo *root = malloc(sizeof(struct inodo));
        root->i_perm = 777;
        root->i_gid = 1;
        root->i_uid = 1;
        root->i_nlink = 0;
        strncpy(root->i_pathlink, "/", 100);
        root->i_size = 0;
        /*FECHA*/
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(root->i_atime, sizeof(root->i_atime), "%Y/%m/%d %H:%M", tm);
        strftime(root->i_ctime, sizeof(root->i_ctime), "%Y/%m/%d %H:%M", tm);
        strftime(root->i_mtime, sizeof(root->i_mtime), "%Y/%m/%d %H:%M", tm);
        /*FECHA*/
        root->i_block[0]=0;
        for(d=1; d<15; d++)
        {
            root->i_block[d]=-1;
        }
        root->i_type = '1';
        size_t tam123 = sizeof(root);
        int digito = tam123;
        printf("\n Tamano del inodo: %d  Lugar del inodo, %d", digito, super_block->s_inode_start);
        file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_block->s_inode_start ,SEEK_SET);
            fwrite(root, sizeof(struct inodo), 1, file);
            fclose(file);
        }
        int inodo_users_pos = 1;
        /*-------------------------------------------------------------------*/
        /*----------------------ESCRITURA DEL BLOQUE ROOT--------------------*/
        strncpy(folder_block->b_content[0].b_name, "/", 12);
        folder_block->b_content[0].b_inodo = 0;
        strncpy(folder_block->b_content[1].b_name, "/", 12);
        folder_block->b_content[1].b_inodo = 0;
        folder_block->b_content[2].b_inodo = 1;
        strncpy(folder_block->b_content[2].b_name, "users.txt", 12);
        folder_block->b_content[3].b_inodo = -1;
        file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_block->s_block_start ,SEEK_SET);
            fwrite(folder_block, sizeof(struct bloque_carpeta), 1, file);
            fclose(file);
            printf("\n El bloque se escribió en %d   %d", super_block->s_block_start, sizeof(struct bloque_carpeta));
        }
        /*-------------------------------------------------------------------*/
        /*-----------------ESCRITURA INODO ARCHIVO USUARIOS------------------*/
        /*USERS Inicializacion*/
        char *users_txt = "1,G,root\n1,U,root,root,201403794\n";

        struct inodo *users = malloc(sizeof(struct inodo));
        int u;
        for(u = 0; u < 15; u++)
        {
            users->i_block[u]=-1;
        }
        users->i_uid = 1;
        users->i_gid = 1;
        int int_len = strlen(users_txt);
        users->i_size = int_len;
        users->i_type = '0';
        users->i_perm = 777;
        time_t t2 = time(NULL);
        struct tm *tm2 = localtime(&t2);
        strftime(users->i_atime, sizeof(users->i_atime), "%Y/%m/%d %H:%M", tm2);
        strftime(users->i_ctime, sizeof(users->i_ctime), "%Y/%m/%d %H:%M", tm2);
        strftime(users->i_mtime, sizeof(users->i_mtime), "%Y/%m/%d %H:%M", tm2);

        strncpy(users->i_pathlink, "/users.txt", 100);
        double double_len = (double)int_len;
        double double_block_quantity = ceil(double_len/60.0);
        int int_block_quantity = (int)double_block_quantity;
        printf("\n Cantidad de bloques para separar el users.txt = %d  largo cadena = %d", int_block_quantity, int_len);
        int ix = 0;
        int b;
        for(b = 0; b< int_block_quantity; b++)
        {
            struct bloque_archivo *user_block = malloc(sizeof(struct bloque_archivo));
            int pos_user = 1;
            int seis;
            if(ix < int_len)
            {
                for(seis = 0; seis < 64; seis++)
                {
                    user_block->b_content[seis] = *(users_txt + ix);
                    ++ix;
                }
                /*ESCRIBIENDO EL BLOQUE ARCHIVO */
                FILE *fileE = fopen(val_direccion, "rb+");
                if(fileE != NULL)
                {
                    fseek(fileE, super_block->s_block_start+(sizeof(struct bloque_archivo)*pos_user),SEEK_SET);
                    fwrite(user_block, sizeof(struct bloque_archivo), 1, fileE);
                    fclose(fileE);
                }
                /*ESCRIBIENDO EL BLOQUE ARCHIVO */
                users->i_block[b] = pos_user;
                if(b<12)
                {
                    printf("\n Asignando bloque archivo al apuntador directo: %d", b);
                }else
                {
                    printf("\n Buscando en apuntadores indirectos espacio para el bloque");
                    int f;
                    for(f = 12; f<15 ; f++)
                    {
                        if(users->i_block[f] == -1)
                        {
                            /*Inicializar indirectos*/
                            printf("\n Inicializando Apuntador_indirecto %d", f);
                            int bloque_indirecto_pos = primer_ajuste_bloque(1);
                            crear_BloqueIndirecto_Archivo(bloque_indirecto_pos, f-11);
                        }else
                        {
                            /*LEER EL BLOQUE DE APUNTADORES QUE ESTA EN ESE APUNTADOR INDIRECTO*/
                            struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
                            FILE * file= fopen(val_direccion, "rb");
                            if (file != NULL)
                            {
                                fseek(file, super_block->s_block_start+(sizeof(struct bloque_apuntador)*users->i_block[f]) ,SEEK_SET);
                                fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
                                fclose(file);
                            }
                            /*------------------------------------------------------------------*/
                            if(indirecto_vacio(b_ptr, f-11) == -1)
                            {
                                /*Ocupar apuntador*/
                                ocupar_AIndirecto_archivo(f-11, users->i_block[f], pos_user);
                                break;
                            }
                        }

                    }
                }
            }
        }
        file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_block->s_inode_start+(sizeof(struct inodo)*1) ,SEEK_SET);
            fwrite(users, sizeof(struct inodo), 1, file);
            fclose(file);
        }
        /*-------------------------------------------------------------------*/
        printf("\n Fin del formateo con ext%c", val_filesystem);
    }

}

int crear_BloqueIndirecto(int nuevo_pos_bloque, int grado)
{
    int resultado = 0;
    if(grado == 1)
    {
        struct bloque_apuntador *nuevo_bloque_apuntador = malloc(sizeof(struct bloque_apuntador));
        int i;
        for(i = 0; i < 16; i++)
        {
            nuevo_bloque_apuntador->b_pointer[i]=-1;
        }
        FILE * file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*nuevo_pos_bloque) ,SEEK_SET);
            fwrite(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        resultado = 1;
    }
    else if (grado > 1)
    {
        struct bloque_apuntador *nuevo_bloque_apuntador = malloc(sizeof(struct bloque_apuntador));
        int i;
        for(i = 0; i < 16; i++)
        {
            nuevo_bloque_apuntador->b_pointer[i]=primer_ajuste_bloque(1);
        }
        FILE * file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*nuevo_pos_bloque) ,SEEK_SET);
            fwrite(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        for(i = 0; i<16; i++)
        {
            crear_BloqueIndirecto(nuevo_bloque_apuntador->b_pointer[i], grado -1);
        }
    }
    return resultado;
}

int crear_BloqueIndirecto_Archivo(int nuevo_pos_bloque, int grado)
{
    int resultado = 0;
    if(grado == 1)
    {
        struct bloque_apuntador *nuevo_bloque_apuntador = malloc(sizeof(struct bloque_apuntador));
        int i;
        for(i = 0; i < 16; i++)
        {
            nuevo_bloque_apuntador->b_pointer[i]=-1;
        }
        FILE * file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*nuevo_pos_bloque) ,SEEK_SET);
            fwrite(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        resultado = 1;
    }
    else if (grado > 1)
    {
        struct bloque_apuntador *nuevo_bloque_apuntador = malloc(sizeof(struct bloque_apuntador));
        int i;
        for(i = 0; i < 16; i++)
        {
            nuevo_bloque_apuntador->b_pointer[i]=primer_ajuste_bloque(1);
        }
        FILE * file= fopen(val_direccion, "rb+");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*nuevo_pos_bloque) ,SEEK_SET);
            fwrite(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        for(i = 0; i<16; i++)
        {
            crear_BloqueIndirecto(nuevo_bloque_apuntador->b_pointer[i], grado -1);
        }
    }
    return resultado;
}

void utilizar_Adirecto_Carpeta(struct bloque_carpeta *bloque_carpeta_pivote, struct inodo *inodo_i,
                                       struct bloque_carpeta *bloque_i,
                                       char *direccion_total,
                                       int padre_id, char *padre, char *direccion, int pos_inodo_actual, int pos_bloque_pivote)
{

    bloque_carpeta_pivote->b_content[0].b_inodo = pos_inodo_actual;
    strcpy(bloque_carpeta_pivote->b_content[0].b_name, direccion);
    bloque_carpeta_pivote->b_content[1].b_inodo = -1;
    bloque_carpeta_pivote->b_content[2].b_inodo = -1;
    bloque_carpeta_pivote->b_content[3].b_inodo = -1;
    /*ESCRIBIENDO EL BLOQUE DE CARPETAS DE PIVOTE------------------------------------------------------------------*/
    FILE *file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*pos_bloque_pivote) ,SEEK_SET);
        fwrite(bloque_carpeta_pivote, sizeof(struct bloque_carpeta), 1, file);
        fclose(file);
    }
    /*ESCRIBIENDO EL BLOQUE DE CARPETAS DE PIVOTE------------------------------------------------------------------*/
    inicializarInodo_y_bloque_Archivo(inodo_i, bloque_i, direccion_total, padre_id, padre, direccion, pos_inodo_actual);
}

void utilizar_Adirecto_Archivo(struct bloque_carpeta *bloque_carpeta_pivote,
                               struct inodo *inodo_i,
                                char *direccion_total,
                                char *direccion,
                                int pos_inodo_actual,
                                int pos_bloque_pivote)
{
    bloque_carpeta_pivote->b_content[0].b_inodo = pos_inodo_actual;
    strcpy(bloque_carpeta_pivote->b_content[0].b_name, direccion);
    bloque_carpeta_pivote->b_content[1].b_inodo = -1;
    bloque_carpeta_pivote->b_content[2].b_inodo = -1;
    bloque_carpeta_pivote->b_content[3].b_inodo = -1;
    /*ESCRIBIENDO EL BLOQUE DE CARPETAS DE PIVOTE------------------------------------------------------------------*/
    FILE *file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*pos_bloque_pivote) ,SEEK_SET);
        fwrite(bloque_carpeta_pivote, sizeof(struct bloque_carpeta), 1, file);
        fclose(file);
    }
    /*ESCRIBIENDO EL BLOQUE DE CARPETAS DE PIVOTE------------------------------------------------------------------*/
    inicializarInodo_y_bloque_File(inodo_i, direccion_total, direccion, pos_inodo_actual);
}

int indirecto_vacio(struct bloque_apuntador *bloque_apuntadores, int grado)
{
    int resultado = 0;
    if(grado == 1)
    {
        int i;
        for(i = 0; i< 16; i++)
        {
            if(bloque_apuntadores->b_pointer[i] == -1)
            {
                return -1;
            }
        }
    }
    else
    {
        int i;
        for(i = 0; i < 16; i++)
        {
            struct bloque_apuntador *bloque_apuntador_recursivo = malloc(sizeof(struct bloque_apuntador));
            FILE * file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*bloque_apuntadores->b_pointer[i]) ,SEEK_SET);
                fread(bloque_apuntador_recursivo, sizeof(struct bloque_apuntador), 1, file);
                fclose(file);
            }
            int d = indirecto_vacio(bloque_apuntador_recursivo, grado-1);
            if(d == -1)
            {
                return -1;
            }
        }
    }
    return resultado;
}

int crearCarpeta_AIndirecto(struct bloque_apuntador *bloque_apuntadores, int grado, int pos_block_ptr,
                            struct bloque_carpeta *bloque_carpeta_pivote,
                            struct inodo *inodo_i,
                            struct bloque_carpeta *bloque_i,
                            char *direccion_total,
                            int padre_id,
                            char *padre,
                            char *direccion,
                            int pos_inodo_nuevo,
                            int pos_bloque_pivote)
{
    int resultado = 0;
    if(grado == 1)
    {
        int i;
        for(i = 0; i< 16; i++)
        {
            if(bloque_apuntadores->b_pointer[i] == -1)
            {
                /*ACTUALIZANDO BLOQUE APUNTADOR, DEPENDE DE LOS 3 PRIMEROS PARAMETROS*/
                bloque_apuntadores->b_pointer[i] =pos_bloque_pivote;
                FILE * file_update= fopen(val_direccion, "rb+");
                if (file_update != NULL)
                {
                    fseek(file_update, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*pos_block_ptr) ,SEEK_SET);
                    fwrite(bloque_apuntadores, sizeof(struct bloque_apuntador), 1, file_update);
                    fclose(file_update);
                }
                /*ACTUALIZANDO BLOQUE APUNTADOR, DEPENDE DE LOS 3 PRIMEROS PARAMETROS*/
                utilizar_Adirecto_Carpeta(bloque_carpeta_pivote,
                                          inodo_i,
                                          bloque_i,
                                          direccion_total,
                                          padre_id,
                                          padre,
                                          direccion,
                                          pos_inodo_nuevo,
                                          pos_bloque_pivote);
                return -1;
            }
        }
    }
    else if (grado == 3 || grado == 2)
    {
        int i;
        for(i = 0; i< 16; i++)
        {
            struct bloque_apuntador *bloque_apuntador_recursivo = NULL;
            FILE * file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*bloque_apuntadores->b_pointer[i]) ,SEEK_SET);
                fread(bloque_apuntador_recursivo, sizeof(struct bloque_apuntador), 1, file);
                fclose(file);
            }
            int d = crearCarpeta_AIndirecto(bloque_apuntador_recursivo,
                                            grado-1,
                                            bloque_apuntadores->b_pointer[i],
                                            bloque_carpeta_pivote,
                                            inodo_i,
                                            bloque_i,
                                            direccion_total,
                                            padre_id,
                                            padre,
                                            direccion,
                                            pos_inodo_nuevo,
                                            pos_bloque_pivote);
            if(d == -1)
            {
                return -1;
            }
        }
    }
    return resultado;
}

int crearArchivo_AIndirecto(struct bloque_apuntador *bloque_apuntadores,
                            int grado,
                            int pos_block_ptr,
                            struct bloque_carpeta *bloque_carpeta_pivote,
                            struct inodo *inodo_i,
                            char *direccion_total,
                            char *direccion,
                            int pos_inodo_nuevo,
                            int pos_bloque_pivote)
{
    int resultado = 0;
    if(grado == 1)
    {
        int i;
        for(i = 0; i< 16; i++)
        {
            if(bloque_apuntadores->b_pointer[i] == -1)
            {
                /*ACTUALIZANDO BLOQUE APUNTADOR, DEPENDE DE LOS 3 PRIMEROS PARAMETROS*/
                bloque_apuntadores->b_pointer[i] =pos_bloque_pivote;
                FILE * file_update= fopen(val_direccion, "rb+");
                if (file_update != NULL)
                {
                    fseek(file_update, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*pos_block_ptr) ,SEEK_SET);
                    fwrite(bloque_apuntadores, sizeof(struct bloque_apuntador), 1, file_update);
                    fclose(file_update);
                }
                /*ACTUALIZANDO BLOQUE APUNTADOR, DEPENDE DE LOS 3 PRIMEROS PARAMETROS*/
                utilizar_Adirecto_Archivo(bloque_carpeta_pivote,
                                          inodo_i,
                                          direccion_total,
                                          direccion,
                                          pos_inodo_nuevo,
                                          pos_bloque_pivote);
                return -1;
            }
        }
    }
    else if (grado > 1)
    {
        int i;
        for(i = 0; i< 16; i++)
        {
            struct bloque_apuntador *bloque_apuntador_recursivo = malloc(sizeof(struct bloque_apuntador));
            FILE * file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*bloque_apuntadores->b_pointer[i]) ,SEEK_SET);
                fread(bloque_apuntador_recursivo, sizeof(struct bloque_apuntador), 1, file);
                fclose(file);
            }
            int d = crearArchivo_AIndirecto(bloque_apuntador_recursivo,
                                            grado-1,
                                            bloque_apuntadores->b_pointer[i],
                                            bloque_carpeta_pivote,
                                            inodo_i,
                                            direccion_total,
                                            direccion,
                                            pos_inodo_nuevo,
                                            pos_bloque_pivote);
            if(d == -1)
            {
                return -1;
            }
        }
    }
    return resultado;
}


int ocupar_AIndirecto_archivo(int grado, int posbloque_lectura, int posbloque_archivo)
{
    int guardado = 0;
    if(grado == 1)
    {
        struct bloque_apuntador *nuevo_bloque_apuntador = malloc(sizeof(struct bloque_apuntador));
        FILE * file= fopen(val_direccion, "rb");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*posbloque_lectura), SEEK_SET);
            fread(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        int c = 0;
        int i;
        for(i = 0; i < 16; i++)
        {
            if(nuevo_bloque_apuntador->b_pointer[i] == -1)
            {
                nuevo_bloque_apuntador->b_pointer[i] = posbloque_archivo;
                printf("\n posbloque_archivo asignado al indirecto: %d  en[%d]", posbloque_archivo, i);
                c = 1;
                break;
            }
        }
        if(c == 1)
        {
            file= fopen(val_direccion, "rb+");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*posbloque_lectura) ,SEEK_SET);
                fwrite(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
                fclose(file);
            }
            guardado = 1;
        }
    }
    else
    {
        struct bloque_apuntador *nuevo_bloque_apuntador = malloc(sizeof(struct bloque_apuntador));
        int i;
        FILE * file= fopen(val_direccion, "rb");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*posbloque_lectura) ,SEEK_SET);
            fread(nuevo_bloque_apuntador, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        for(i = 0; i < 16; i++)
        {
            int d = ocupar_AIndirecto_archivo(grado -1, nuevo_bloque_apuntador->b_pointer[i], posbloque_archivo);
            if( d == 1)
            {
                guardado = 1;
                break;
            }
        }
    }
    return guardado;

}

int primer_ajuste_bloque(int opcion)
{
    int resultado = 0;
    if(ajuste_PUBLICO == 'f' || ajuste_PUBLICO == 'F')
    {
        printf("\n PRIMER AJUSTE");
        opcion = 1;
    }else if(ajuste_PUBLICO == 'w' || ajuste_PUBLICO == 'W')
    {
        printf("\n PEOR AJUSTE");
        opcion = 3;
    }else if(ajuste_PUBLICO == 'b' || ajuste_PUBLICO == 'B')
    {
        printf("\n MEJOR AJUSTE");
        opcion = 2;
    }
    if(opcion == 1)
    {
        printf("\n -------------------------------------");
        char bitmap_bloques[super_bloque_principal->s_blocks_count];
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fread(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
        }
        int i;
        for(i = 0; i< super_bloque_principal->s_blocks_count; i++)
        {
            if(bitmap_bloques[i] == '0')
            {
                resultado = i;
                bitmap_bloques[i] = '1';
                break;
            }
        }
        file = fopen(val_direccion, "rb+");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fwrite(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
            printf("\n Bitmap de bloques actualizado");
        }
        printf("\n Primer Ajuste Bloque(%d)", resultado);
        printf("\n -------------------------------------");
    }else if (opcion == 4)
    {
        char bitmap_bloques[super_bloque_principal->s_blocks_count];
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fread(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
        }
        int i;
        for(i = 0; i< super_bloque_principal->s_blocks_count; i++)
        {
            if(bitmap_bloques[super_bloque_principal->s_blocks_count] == '0')
            {
                resultado = i;
                break;
            }
        }
        printf("\n Primer Bloque Disponible(%d)", resultado);
    }else if(opcion == 2)/*MEJOR AJUSTE*/
    {
        printf("\n -------------------------------------");
        char bitmap_bloques[super_bloque_principal->s_blocks_count];
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fread(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
        }
        int bool_encero=0;
        int contador_resultado = 0;
        int primer_bandera = 0;
        int primer_bandera_global = 0;
        int contador = 0;
        int inicio = 0;
        int inicio_global = 0;
        int i;
        for(i = 0; i< super_bloque_principal->s_blocks_count; i++)
        {
            if(bitmap_bloques[i] == '0' && primer_bandera == 1)
            {
                ++contador;
                bool_encero = 1;
            }
            if(bitmap_bloques[i] == '0' && primer_bandera == 0)
            {
                bool_encero=1;
                primer_bandera = 1;
                inicio = i;
                inicio_global = i;
                ++contador;
            }
            if(bitmap_bloques[i] == '1' && bool_encero == 1 && primer_bandera_global ==0)
            {
                primer_bandera = 0;
                primer_bandera_global = 1;
                bool_encero = 0;
                contador_resultado = contador;
                contador = 0;
            }
            if(bitmap_bloques[i] == '1' && bool_encero == 1 && primer_bandera_global == 1)
            {
                if(contador < contador_resultado)
                {
                    contador_resultado = contador;
                    inicio_global = inicio;
                }
                primer_bandera = 0;
                bool_encero = 0;
                contador = 0;
            }
        }
        bitmap_bloques[inicio_global] = '1';
        resultado = inicio_global;
        file = fopen(val_direccion, "rb+");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fwrite(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
            printf("\n Bitmap de bloques actualizado");
        }
        printf("\n Mejor Ajuste Bloque(%d)", resultado);
        printf("\n -------------------------------------");
    }else if(opcion == 3)/*PEOR AJUSTE*/
    {
        printf("\n -------------------------------------");
        char bitmap_bloques[super_bloque_principal->s_blocks_count];
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fread(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
        }
        int bool_encero=0;
        int contador_resultado = 0;
        int primer_bandera = 0;
        int primer_bandera_global = 0;
        int contador = 0;
        int inicio = 0;
        int inicio_global = 0;
        int i;
        for(i = 0; i< super_bloque_principal->s_blocks_count; i++)
        {

            if(bitmap_bloques[i] == '0' && primer_bandera == 1)
            {
                ++contador;
                bool_encero = 1;
            }
            if(bitmap_bloques[i] == '0' && primer_bandera == 0)
            {
                bool_encero=1;
                primer_bandera = 1;
                inicio = i;
                inicio_global = i;
                printf("\n inicio global %d", inicio_global);
                ++contador;
            }
            if(bitmap_bloques[i] == '1' && bool_encero == 1 && primer_bandera_global ==0)
            {
                primer_bandera = 0;
                primer_bandera_global = 1;
                bool_encero = 0;
                contador_resultado = contador;
                contador = 0;
            }
            if(bitmap_bloques[i] == '1' && bool_encero == 1 && primer_bandera_global == 1)
            {
                if(contador > contador_resultado)
                {
                    contador_resultado = contador;
                    inicio_global = inicio;
                }
                primer_bandera = 0;
                bool_encero = 0;
                contador = 0;
            }
        }
        bitmap_bloques[inicio_global] = '1';
        resultado = inicio_global;
        file = fopen(val_direccion, "rb+");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
            fwrite(bitmap_bloques, super_bloque_principal->s_blocks_count, 1, file);
            fclose(file);
            printf("\n Bitmap de bloques actualizado");
        }
        printf("\n Peor Ajuste Bloque(%d)", resultado);
        printf("\n -------------------------------------");
    }
    return resultado;
}

int primer_ajuste_inodo(int opcion)
{
    int resultado = 0;
    FILE *file;
    char bitmap_inodos[super_bloque_principal->s_inodes_count];
    if(ajuste_PUBLICO == 'f' || ajuste_PUBLICO == 'F')
    {
        printf("\n PRIMER AJUSTE");
        opcion = 1;
    }else if(ajuste_PUBLICO == 'w' || ajuste_PUBLICO == 'W')
    {
        printf("\n PEOR AJUSTE");
        opcion = 3;
    }else if(ajuste_PUBLICO == 'b' || ajuste_PUBLICO == 'B')
    {
        printf("\n MEJOR AJUSTE");
        opcion = 2;
    }else
    {
        printf("\n sin ajuste [%c]", ajuste_PUBLICO);
    }
    if(opcion == 1)
    {
        printf("\n -------------------------------");
        printf("\n Primer ajuste inodo.");
        file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fread(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
        }
        int i;
        for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
        {
            printf("\n Revisando bitmap_inodos[%d] = '%c'", i, bitmap_inodos[i]);
            if(bitmap_inodos[i] == '0')
            {
                resultado = i;
                bitmap_inodos[i] = '1';
                break;
            }
        }
        file = fopen(val_direccion, "rb+");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fwrite(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
            printf("\n Bitmap de inodos actualizado: %c %d",bitmap_inodos[i], i);
        }
        printf("\n -------------------------------");
    }
    else if (opcion == 4)
    {
        FILE * file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fread(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
        }
        int i;
        for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
        {
            if(bitmap_inodos[super_bloque_principal->s_inodes_count] == '0')
            {
                resultado = i;
                break;
            }
        }
        printf("\n Primer Inodo Disponible(%d)", resultado);
    }
    else if(opcion == 2)/*MEJOR AJUSTE*/
    {
        printf("\n -------------------------------------");
        file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fread(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
        }
        int bool_encero=0;
        int contador_resultado = 0;
        int primer_bandera = 0;
        int primer_bandera_global = 0;
        int contador = 0;
        int inicio = 0;
        int inicio_global = 0;
        int i;
        for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
        {
            if(bitmap_inodos[i] == '0' && primer_bandera == 1)
            {
                ++contador;
                bool_encero = 1;
            }
            if(bitmap_inodos[i] == '0' && primer_bandera == 0)
            {
                bool_encero=1;
                primer_bandera = 1;
                inicio = i;
                inicio_global = i;
                ++contador;
            }
            if(bitmap_inodos[i] == '1' && bool_encero == 1 && primer_bandera_global ==0)
            {
                primer_bandera = 0;
                primer_bandera_global = 1;
                bool_encero = 0;
                contador_resultado = contador;
                contador = 0;
            }
            if(bitmap_inodos[i] == '1' && bool_encero == 1 && primer_bandera_global == 1)
            {
                if(contador < contador_resultado)
                {
                    contador_resultado = contador;
                    inicio_global = inicio;
                }
                primer_bandera = 0;
                bool_encero = 0;
                contador = 0;
            }
        }
        bitmap_inodos[inicio_global] = '1';
        resultado = inicio_global;
        file = fopen(val_direccion, "rb+");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fwrite(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
            printf("\n Bitmap de bloques actualizado");
        }
        printf("\n Mejor Ajuste Inodo(%d)", resultado);
        printf("\n -------------------------------------");
    }
    else if(opcion == 3)/*PEOR AJUSTE*/
    {
        printf("\n -------------------------------------");
        file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fread(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
        }
        int bool_encero=0;
        int contador_resultado = 0;
        int primer_bandera = 0;
        int primer_bandera_global = 0;
        int contador = 0;
        int inicio = 0;
        int inicio_global = 0;
        int i;
        for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
        {

            if(bitmap_inodos[i] == '0' && primer_bandera == 1)
            {
                ++contador;
                bool_encero = 1;
            }
            if(bitmap_inodos[i] == '0' && primer_bandera == 0)
            {
                bool_encero=1;
                primer_bandera = 1;
                inicio = i;
                inicio_global = i;
                printf("\n inicio global %d para %c", inicio_global, bitmap_inodos[i]);
                ++contador;
            }
            if(bitmap_inodos[i] == '1' && bool_encero == 1 && primer_bandera_global ==0)
            {
                primer_bandera = 0;
                primer_bandera_global = 1;
                bool_encero = 0;
                contador_resultado = contador;
                contador = 0;
            }
            if(bitmap_inodos[i] == '1' && bool_encero == 1 && primer_bandera_global == 1)
            {
                if(contador > contador_resultado)
                {
                    contador_resultado = contador;
                    inicio_global = inicio;
                }
                primer_bandera = 0;
                bool_encero = 0;
                contador = 0;
            }
        }
        bitmap_inodos[inicio_global] = '1';
        resultado = inicio_global;
        printf("\n Peor Ajuste Bloque(%d)", resultado);
        printf("\n -------------------------------------");
        file = fopen(val_direccion, "rb+");
        if(file != NULL)
        {
            fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
            fwrite(bitmap_inodos, super_bloque_principal->s_inodes_count, 1, file);
            fclose(file);
            printf("\n Bitmap de inodos actualizado: %c %d",bitmap_inodos[i], i);
        }
    }
    printf("\n Primer Ajuste Inodos(%d)", resultado);
    return resultado;
}

void inicializarInodo_y_bloque_Archivo(struct inodo *inodo_i,
                                       struct bloque_carpeta *bloque_i,
                                       char *direccion_total,
                                       int padre_id,
                                       char *padre,
                                       char *direccion,
                                       int pos_inodo_actual)
/*----------------------------------------------------------------------------------------------------------------------------------*/
{
    strcpy(inodo_i->i_pathlink, direccion_total);
    inodo_i->i_size = 0;
    inodo_i->i_perm = 664;
    inodo_i->i_uid = user_id;
    inodo_i->i_gid = user_group_id;
    /*FECHA*/
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(inodo_i->i_atime, sizeof(inodo_i->i_atime), "%Y/%m/%d %H:%M", tm);
    strftime(inodo_i->i_ctime, sizeof(inodo_i->i_ctime), "%Y/%m/%d %H:%M", tm);
    strftime(inodo_i->i_mtime, sizeof(inodo_i->i_mtime), "%Y/%m/%d %H:%M", tm);
    /*FECHA*/
    /*Apuntar bitacora*/
    escribir_en_journaling(1, '1', direccion_total, 0, "none", inodo_i->i_ctime, user_id, inodo_i->i_gid, 664, super_bloque_principal->s_filesystem_type);
    /*Apuntar bitacora*/
    int pos_bloque = primer_ajuste_bloque(1);
    inodo_i->i_block[0] = pos_bloque;
    int i;
    for(i = 1;i < 15; i++)
    {
        inodo_i->i_block[i] = -1;
    }
    inodo_i->i_type = '1';
    FILE * file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_inode_start+(sizeof(struct inodo)*pos_inodo_actual) ,SEEK_SET);
        fwrite(inodo_i, sizeof(struct inodo), 1, file);
        fclose(file);
    }
    printf("\n [inicializarInodo_y_bloque_Carpeta : %s]", direccion);
    printf("\n NUEVO INODO ESCRITO******************************");
    printf("\n Posicion de escritura:         %d", pos_inodo_actual);
    printf("\n Inodo pathlink:                %s", direccion_total);
    printf("\n Posicion bloque[home, actual]: %d", pos_bloque);
    printf("\n *************************************************");
    char *direccion2 = strdup(direccion);
    inicializarBloqueArchivo(bloque_i, pos_inodo_actual, padre_id, padre, direccion2, pos_bloque);
}

void inicializarInodo_y_bloque_File(struct inodo *inodo_i,
                                       char *direccion_total,
                                       char *direccion,
                                       int pos_inodo_actual)
/*----------------------------------------------------------------------------------------------------------------------------------*/
{
    strcpy(inodo_i->i_pathlink, direccion_total);
    /*FECHA*/
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(inodo_i->i_atime, sizeof(inodo_i->i_atime), "%Y/%m/%d %H:%M", tm);
    strftime(inodo_i->i_ctime, sizeof(inodo_i->i_ctime), "%Y/%m/%d %H:%M", tm);
    strftime(inodo_i->i_mtime, sizeof(inodo_i->i_mtime), "%Y/%m/%d %H:%M", tm);
    inodo_i->i_perm = 664;
    inodo_i->i_uid = user_id;
    inodo_i->i_gid = user_group_id;
    /*FECHA*/
    /*Apuntar bitacora*/
    if(bool_cont == 1)
    {
        escribir_en_journaling(2, '0', direccion_total, bool_cont, val_cont, inodo_i->i_ctime, user_id, inodo_i->i_gid, 664, super_bloque_principal->s_filesystem_type);
    }
    else
    {
        char str[15];
        sprintf(str, "%d", val_size);
        escribir_en_journaling(2, '0', direccion_total, bool_cont, str, inodo_i->i_ctime, user_id, inodo_i->i_gid, 664, super_bloque_principal->s_filesystem_type);
    }
    /*Apuntar bitacora*/
    int i;
    for(i = 1;i < 15; i++)
    {
        inodo_i->i_block[i] = -1;
    }
    inodo_i->i_type = '0';
    double bloques_archivo_d=0;
    int bloques_archivo=0;
    if(bool_cont == 1)
    {
        file_reader();
        printf("\n buffer: %s", buffer_store);
        int cadena_tam = strlen(buffer_store);
        inodo_i->i_size = cadena_tam;
        double ddd = (double)cadena_tam;
        float f = ddd/64.0;
        bloques_archivo_d = ceil(f);
        bloques_archivo = (int) bloques_archivo_d;
        printf("\n bloques f:%f buffer_size:%d cadena_tam:%d ddd:%f", f, buffer_size, cadena_tam,ddd );
        int apuntador = 0;
        int contador = 0;
        int i;
        int j;
        for( i = 0; i < bloques_archivo; i++)
        {
            /*ASIGNAR CADENA AL BLOQUE-ARCHIVO*/
            struct bloque_archivo *bloque_archivo_nuevo = malloc(sizeof(struct bloque_archivo));
            for(j = 0; j < 64; j++)
            {
                if(contador < cadena_tam)
                {
                    bloque_archivo_nuevo->b_content[j] = *(buffer_store + contador);
                    printf("\n Asignando %c", bloque_archivo_nuevo->b_content[j]);
                    ++contador;
                }
            }
            char *cad = bloque_archivo_nuevo->b_content;
            printf("\n contenido bloque %d: %s", i, cad);
            int pos_block = primer_ajuste_bloque(1);
            /*--------------------------------*/
            /*ESCRIBIENDO EL BLOQUE ARCHIVO */
            FILE *fileE = fopen(val_direccion, "rb+");
            if(fileE != NULL)
            {
                fseek(fileE, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*pos_block),SEEK_SET);
                fwrite(bloque_archivo_nuevo, sizeof(struct bloque_archivo), 1, fileE);
                fclose(fileE);
            }
            /*  ESCRIBIENDO EL BLOQUE ARCHIVO */
            /*--------------------------------*/
            if(apuntador < 12)
            {
                inodo_i->i_block[apuntador] = pos_block;
                printf("\n Asignando bloque archivo al apuntador directo: %d", apuntador);
            }else
            {
                printf("\n Buscando en apuntadores indirectos espacio para el bloque");
                int f;
                for(f = 12; f<15 ; f++)
                {
                    if(inodo_i->i_block[f] == -1)
                    {
                        /*Inicializar indirectos*/
                        int bloque_indirecto_pos = primer_ajuste_bloque(1);
                        printf("\n Inicializando Apuntador_indirecto %d con el valor: %d", f, bloque_indirecto_pos);
                        crear_BloqueIndirecto_Archivo(bloque_indirecto_pos, f-11);
                    }else
                    {
                        /*LEER EL BLOQUE DE APUNTADORES QUE ESTA EN ESE APUNTADOR INDIRECTO*/
                        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
                        FILE * file= fopen(val_direccion, "rb");
                        if (file != NULL)
                        {
                            fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*inodo_i->i_block[f]) ,SEEK_SET);
                            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
                            fclose(file);
                        }
                        /*------------------------------------------------------------------*/
                        if(indirecto_vacio(b_ptr, f-11) == -1)
                        {
                            /*Ocupar apuntador*/
                            ocupar_AIndirecto_archivo(f-11, inodo_i->i_block[f], pos_block);
                            break;
                        }
                    }

                }
            }
            ++apuntador;
        }
    }
    else
    {
        printf("\n SIZE:%d!", val_size);
        inodo_i->i_size = val_size;
        char string[val_size];
        int counter = 1;
        int n;
        for(n = 0; n < val_size; n++)
        {
            string[n] = counter + '0';
            if(counter == 9)
            {
                counter = 1;
            }else
            {
                ++counter;
            }
        }
        char *s = string;
        bloques_archivo_d = ceil(((double)val_size)/64);
        bloques_archivo = (int)bloques_archivo_d;
        printf("\n BLOQUES ARCHIVO %d y cadena:size [%s]", bloques_archivo,string);
        int apuntador = 0;
        int contador = 0;
        int i;
        int j;
        for(i = 0; i < bloques_archivo; i++)
        {
            /*ASIGNAR CADENA AL BLOQUE-ARCHIVO*/
            struct bloque_archivo *bloque_archivo_nuevo = malloc(sizeof(struct bloque_archivo));
            for(j = 0; j < 64; j++)
            {
                if(contador < val_size)
                {
                    bloque_archivo_nuevo->b_content[j] = string[contador];
                    ++contador;
                }
            }
            char *cad = bloque_archivo_nuevo->b_content;
            printf("\n contenido bloque %d: %s", i, cad);

            int pos_block = primer_ajuste_bloque(1);
            /*--------------------------------*/
            /*ESCRIBIENDO EL BLOQUE ARCHIVO */
            FILE *fileE = fopen(val_direccion, "rb+");
            if(fileE != NULL)
            {
                fseek(fileE, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*pos_block),SEEK_SET);
                fwrite(bloque_archivo_nuevo, sizeof(struct bloque_archivo), 1, fileE);
                fclose(fileE);
            }
            /*--------------------------------*/
            /*  ESCRIBIENDO EL BLOQUE ARCHIVO */
            /*--------------------------------*/
            /*ESCRIBIENDO/MODIFICANDO EN EL INODO-ARCHIVO */
            if(i < 12)
            {
                inodo_i->i_block[i] = pos_block;
                printf("\n Asignando bloque archivo al apuntador directo: %d valor de i %d",pos_block,  i);
            }else
            {
                printf("\n Buscando en apuntadores indirectos espacio para el bloque");
                int f;
                for(f = 12; f<15 ; f++)
                {
                    if(inodo_i->i_block[f] == -1)
                    {
                        /*Inicializar indirectos*/
                        int bloque_indirecto_pos = primer_ajuste_bloque(1);
                        inodo_i->i_block[f] = bloque_indirecto_pos;
                        printf("\n Inicializando Apuntador_indirecto %d %d", f, inodo_i->i_block[f]);
                        crear_BloqueIndirecto_Archivo(bloque_indirecto_pos, f-11);
                    }
                    /*LEER EL BLOQUE DE APUNTADORES QUE ESTA EN ESE APUNTADOR INDIRECTO*/
                    struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
                    FILE * file= fopen(val_direccion, "rb");
                    if (file != NULL)
                    {
                        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*inodo_i->i_block[f]) ,SEEK_SET);
                        fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
                        fclose(file);
                    }
                    /*------------------------------------------------------------------*/
                    if(indirecto_vacio(b_ptr, f-11) == -1)
                    {
                        printf("\n Indirecto vacio!!!![%d][%d][%d]", f-11, inodo_i->i_block[f], pos_block);
                        /*Ocupar apuntador*/
                        ocupar_AIndirecto_archivo(f-11, inodo_i->i_block[f], pos_block);
                        break;
                    }
                }
            }
            ++apuntador;
        }
    }
    FILE * file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_inode_start+(sizeof(struct inodo)*pos_inodo_actual) ,SEEK_SET);
        fwrite(inodo_i, sizeof(struct inodo), 1, file);
        fclose(file);
    }

    printf("\n [inicializarInodo_y_bloque_Archivo : %s]", direccion);
    printf("\n NUEVO INODO ESCRITO******************************");
    printf("\n Posicion de escritura:         %d", pos_inodo_actual);
    printf("\n Inodo pathlink:                %s", direccion_total);
    printf("\n *************************************************");
}

void inicializarBloqueArchivo(struct bloque_carpeta *bloque_i, int inodo_id, int padre_id, char *padre, char *direccion, int pos_bloque)
{
    /*# . */
    bloque_i->b_content[0].b_inodo = inodo_id;
    strcpy(bloque_i->b_content[0].b_name, direccion);
    /*# .. */
    bloque_i->b_content[1].b_inodo = padre_id;
    strcpy(bloque_i->b_content[1].b_name, padre);
    /*# 2*/
    bloque_i->b_content[2].b_inodo = -1;

    /*# 3*/
    bloque_i->b_content[3].b_inodo = -1;

    FILE * file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*pos_bloque) ,SEEK_SET);
        fwrite(bloque_i, sizeof(struct bloque_carpeta), 1, file);
        fclose(file);
    }
    printf("\n\n [inicializarBloqueArchivo : %s]", direccion);
    printf("\n NUEVO BLOQUE ESCRITO******************************");
    printf("\n Posicion de escritura: %d", pos_bloque);
    printf("\n Posicion actual:       %d", inodo_id);
    printf("\n Direccion actual:      %s", direccion);
    printf("\n Posicion padre:        %d", padre_id);
    printf("\n Direccion padre:       %s", padre);
    printf("\n *************************************************");
}

char* modificar_direccion_carpeta(char *valor_direccion)
{
    int tam_resultado = strlen(valor_direccion);
    char resultado[tam_resultado + 2];
    int contador = 2;
    int i;
    resultado[0] = '/';
    resultado[1] = '?';
    for(i = 1; i < tam_resultado + 1; i++)
    {
        if(*(valor_direccion + i) == '/')
        {
            resultado[contador] = '?';
        }else
        {
            resultado[contador] = *(valor_direccion + i);
        }
        ++contador;
    }
    char *r = resultado;
    printf("\n Cadena de la carpeta [%s]", r);
    return resultado;
}

char* modificar_direccion_a_buscar(char *valor_direccion)
{
    int tam = strlen(valor_direccion)+1;
    int i;/*val_direccion_a_buscar*/
    int nuevo_tam = 0;
    int cantidad_diagonal = 0;
    char resp[4] ;
    resp[0]='1';
    resp[1]='2';
    resp[2]='\0';
    for(i = 0; i < tam; i++)
    {
        if(*(valor_direccion + i) == '/')
        {
            ++cantidad_diagonal;
        }
    }
    if(cantidad_diagonal > 1)
    {
        for(i = tam-1; i > -1; i--)
        {
            if(*(valor_direccion + i) == '/')
            {
                ++nuevo_tam;
                break;
            }
            ++nuevo_tam;
        }
        char respuesta[tam-nuevo_tam+1];
        respuesta[tam-nuevo_tam] = '\0';
        for(i = 0; i < tam-nuevo_tam; i++)
        {
            respuesta[i] = *(valor_direccion + i);
        }
        char *d = strdup(respuesta);
        printf("\n -> modificar_direccion_a_buscar /////: %s", d);
        return d;
    }else
    {
        char respuesta[2];
        respuesta[0] = '/';
        respuesta[1] = '\0';
        char *d = respuesta;
        printf("\n modificar_direccion_a_buscar /: %s", d);
        return respuesta;
    }
    return resp;
}
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
void reporte_file()
{
    printf("\n file [%s]", ruta_char);
    cargar_superbloque();
    char *dir = strdup(modificar_direccion_carpeta(ruta_char));
    char *dir2 = strdup(modificar_direccion_a_buscar(ruta_char));
    printf("\n Recuperando el primer inodo en: dir2[%s] dir(%s)", dir2, dir);
    char **split_folder = str_split(dir, '?');
    int cantidad = tamano(split_folder);
    char inode_bitmap[super_bloque_principal->s_inodes_count];
    FILE *file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
        fread(inode_bitmap, super_bloque_principal->s_inodes_count, 1, file);
        fclose(file);
    }
    int i;
    for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
    {
        if(inode_bitmap[i] == '1')
        {
            struct inodo *inodo_archivo = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            if(file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start+(i*sizeof(struct inodo)) ,SEEK_SET);
                fread(inodo_archivo, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            printf("\n Tipo inodo %c", inodo_archivo->i_type);
            if(inodo_archivo->i_type == '0')
            {
                char *token = inodo_archivo->i_pathlink;
                printf("\n comparando %s %s", token, ruta_char);
                if(strcmp(token, ruta_char)==0)
                {

                        printf("\n file: %s -------------------\n\n\n", ruta_char);
                        int j;
                        for(j = 0; j < 12; j++)
                        {
                            if(inodo_archivo->i_block[j] != -1)
                            {
                                struct bloque_archivo *bloque_letras = malloc(sizeof(struct bloque_archivo));
                                file = fopen(val_direccion, "rb");
                                if(file != NULL)
                                {
                                    fseek(file, super_bloque_principal->s_block_start+(inodo_archivo->i_block[j]*sizeof(struct bloque_archivo)) ,SEEK_SET);
                                    fread(bloque_letras, sizeof(struct bloque_archivo), 1, file);
                                    fclose(file);
                                }
                                char *contentt = bloque_letras->b_content;
                                printf("%s", contentt);
                            }
                        }
                        for(j = 12; j<15; j++)
                        {
                            if(inodo_archivo->i_block[j] != -1)
                            {
                                print_indirecto(inodo_archivo->i_block[j], j-11);
                            }
                        }
                        printf("\n\n\n Fin archivo\n");
                }
            }
        }
    }
}

char* renombrar(char *original, char *nuevo)
{
    printf("\n original %s, nuevo %s", original, nuevo);
    int original_len = strlen(original);
    int nuevo_len = strlen(nuevo);
    int contador = 0;
    int i;
    for(i = original_len-1; i > -1; i--)
    {
        if(*(original + i) == '/')
        {
            break;
        }
        ++contador;
    }
    printf("\n original %d nuevo %d contador %d", original_len, nuevo_len, contador);
    int tamanooo = original_len - contador + nuevo_len;
    char resultado[tamanooo];
}

void crear_carpeta_mkdir()
{
    printf("\n Creando carpeta [%s]", folder_direccion);
    cargar_superbloque();
    char *dir = strdup(modificar_direccion_carpeta(folder_direccion));
    char *dir2 = strdup(modificar_direccion_a_buscar(folder_direccion));
    printf("\n Recuperando el primer inodo en: [%s] (%s)", dir2, dir);
    char **split_folder = str_split(dir, '?');
    int cantidad = tamano(split_folder);
    struct inodo *inodo_inicial = malloc(sizeof(struct inodo));
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_inode_start ,SEEK_SET);
        fread(inodo_inicial, sizeof(struct inodo), 1, file);
        fclose(file);
    }
    crear_carpeta_recursivo(inodo_inicial, *(split_folder + cantidad-1), folder_direccion, dir2);
}
void crear_carpeta_recursivo(struct inodo *inodo_r, char *direccion, char *direccion_total, char *direccion_a_buscar)
/*---------------------------------------------------------------------------------------------------------*/
{
    printf("\n Crear Carpeta recursivo, %c, %s",inodo_r->i_type ,direccion_a_buscar);
    char *pathlink = inodo_r->i_pathlink;
    int tam_inodo = sizeof(struct inodo);
    int tam_bloque = 64;
    int inicio_tabla_block = super_bloque_principal->s_block_start;
    int inicio_tabla_inodo = super_bloque_principal->s_inode_start;
    if(inodo_r->i_type == '1')/*Es una carpeta...*/
    {
        printf("\n Comparando [%s][%s]", pathlink, direccion_a_buscar);
        if(strcmp(pathlink, direccion_a_buscar)==0)
        {
            printf("\n Direccion encontrada! se realizarán los procedimientos para crear carpetas...[%s] D:[%s]", pathlink, direccion);

            int grupo = user_group_id;
            int permisos = permiso(inodo_r->i_uid, grupo, inodo_r->i_gid, inodo_r->i_perm, 2);
            if(permisos == 1)
            {
                printf("\n Tiene permiso de crear la carpeta");
                int bool_bloque = 0;
                int k;
                int padre_i = 0;
                for(k = 0; k< 12; k++)/*Revisando los primeros 12 bloques directos*/
                {
                    printf("\n Revisando bloque[%d] = %d",k , inodo_r->i_block[k]);
                    if(inodo_r->i_block[k] != -1)
                    {
                        struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_archivo));
                        /*LEYENDO BLOQUE DEL INODO-----------------*/
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, inicio_tabla_block+(inodo_r->i_block[k]*tam_bloque) ,SEEK_SET);
                            fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                            fclose(file);
                        }
                        /*LEYENDO BLOQUE DEL INODO-----------------*/
                        char *padre;
                        int d;
                        if(k == 0)
                        {
                            d = 2;
                            padre_i = bloque_carpeta_temporal->b_content[0].b_inodo;
                            padre = bloque_carpeta_temporal->b_content[0].b_name;
                        }
                        else
                        {
                            d = 0;
                            struct bloque_carpeta *bloque_carpeta_temporal2 = malloc(sizeof(struct bloque_carpeta));
                            /*LEYENDO BLOQUE DEL INODO-----------------*/
                            file = fopen(val_direccion, "rb");
                            if(file != NULL)
                            {
                                fseek(file, inicio_tabla_block+(inodo_r->i_block[0]*tam_bloque) ,SEEK_SET);
                                fread(bloque_carpeta_temporal2, sizeof(struct bloque_carpeta), 1, file);
                                fclose(file);
                            }
                            padre_i = bloque_carpeta_temporal2->b_content[0].b_inodo;
                            padre = bloque_carpeta_temporal2->b_content[0].b_name;
                            /*LEYENDO BLOQUE DEL INODO-----------------*/
                        }
                        for(; d< 4; d++)
                        {
                            if(bloque_carpeta_temporal->b_content[d].b_inodo == -1)
                            {
                                printf("\n Creando carpeta en posicion de bloque %d...", d);
                                bool_bloque = 1;
                                /**************************/
                                /*ACTUALIZAR BLOQUE ACTUAL*/
                                /**************************/
                                int pos_inodo_nuevo = primer_ajuste_inodo(1);
                                bloque_carpeta_temporal->b_content[d].b_inodo = pos_inodo_nuevo;
                                FILE * file_update= fopen(val_direccion, "rb+");
                                if (file_update != NULL)
                                {
                                    fseek(file_update, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*inodo_r->i_block[k]) ,SEEK_SET);
                                    fwrite(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file_update);
                                    fclose(file_update);
                                }
                                printf("\n Actualizando bloque %d con el valor %d", k, pos_inodo_nuevo);
                                /**************************/
                                /*ACTUALIZAR BLOQUE ACTUAL*/
                                /**************************/

                                /**************************/
                                /*CREAR INODO Y BLOQUE*/
                                /**************************/
                                struct inodo *nuevo_inodo = malloc(sizeof(struct inodo));
                                struct bloque_carpeta *nuevo_bloque_carpeta = malloc(sizeof(struct bloque_carpeta));
                                inicializarInodo_y_bloque_Archivo(nuevo_inodo,
                                                                  nuevo_bloque_carpeta,
                                                                  direccion_total,
                                                                  padre_i, padre,
                                                                  direccion,
                                                                  pos_inodo_nuevo);
                                break;
                                /**************************/
                                /*CREAR INODO Y BLOQUE*/
                                /**************************/
                            }
                        }
                        if(bool_bloque == 1)
                        {
                            break;
                        }
                    }
                }
                if(bool_bloque == 0)
                {
                    /*-------------------------------------------------------*/
                    /*Buscando bloques directos sin utilizar...*/
                    int directo_vacio = 0;
                    int dir;
                    for(dir = 0; dir < 12; dir++)
                    {
                        if(inodo_r->i_block[dir] == -1)
                        {
                            directo_vacio = 1;
                            break;
                        }
                    }
                    if(directo_vacio == 1)
                    {
                        struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_carpeta));
                        /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, inicio_tabla_block+(inodo_r->i_block[0]*tam_bloque) ,SEEK_SET);
                            fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                            fclose(file);
                        }
                        /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                        /*...................................................................*/

                        /*...........................................*/
                        /*ACTUALIZAR INODO ACTUAL*******-------------*/
                        int inodo_actual_posicion = bloque_carpeta_temporal->b_content[0].b_inodo;
                        int bloque_pivote_posicion = primer_ajuste_bloque(1);
                        inodo_r->i_block[dir] = bloque_pivote_posicion;
                        file= fopen(val_direccion, "rb+");
                        if (file != NULL)
                        {
                            fseek(file, super_bloque_principal->s_inode_start+(sizeof(struct inodo)*inodo_actual_posicion) ,SEEK_SET);
                            fwrite(inodo_r, sizeof(struct inodo), 1, file);
                            fclose(file);
                        }
                        /*ACTUALIZAR INODO ACTUAL*******-------------*/
                        /*...........................................*/
                        /*CREANDO BLOQUE PIVOTE, INODO, Y BLOQUE NUEVOS*/
                        int grupo = getGroup(user_group);
                        int permisos = permiso(inodo_r->i_uid, grupo, inodo_r->i_gid, inodo_r, 2);
                        if(permisos == 1)
                        {
                            printf("\n Tiene permiso de crear la carpeta");
                        }else

                        {
                            printf("\n NO tiene permisos suficientes para crear la carpeta");
                        }
                        int pos_inodo_nuevo = primer_ajuste_inodo(1);
                        char *padre_nombre = bloque_carpeta_temporal->b_content[0].b_name;
                        struct bloque_carpeta *bloque_carpeta_pivote = malloc(sizeof(struct bloque_carpeta));
                        struct bloque_carpeta *bloque_carpeta_nuevo = malloc(sizeof(struct bloque_carpeta));
                        struct inodo *inodo_nuevo = malloc(sizeof(struct inodo));
                        utilizar_Adirecto_Carpeta(bloque_carpeta_pivote,
                                                  inodo_nuevo,
                                                  bloque_carpeta_nuevo,
                                                  direccion_total,
                                                  inodo_actual_posicion,
                                                  padre_nombre,
                                                  direccion,
                                                  pos_inodo_nuevo,
                                                  bloque_pivote_posicion);
                    }
                    else/*Revisando en indirectos para poder guardar carpeta*/
                    {
                        int ind;
                        int indirectos = 0;
                        int guardadoFinal = 0;
                        for(ind = 12; ind < 15; ind++)
                        {
                            if(inodo_r->i_block[ind] != -1)
                            {
                                struct bloque_apuntador *bloque_apuntador_temporal = malloc(sizeof(struct bloque_apuntador));
                                FILE * file = fopen(val_direccion, "rb");
                                if(file != NULL)
                                {
                                    fseek(file, inicio_tabla_block+(inodo_r->i_block[ind]*sizeof(struct bloque_apuntador)) ,SEEK_SET);
                                    fread(bloque_apuntador_temporal, sizeof(struct bloque_apuntador), 1, file);
                                    fclose(file);
                                }
                                if(indirecto_vacio(bloque_apuntador_temporal, ind -11) == -1)
                                {
                                    guardadoFinal = 1;
                                    struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_carpeta));
                                    /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                                    FILE * file = fopen(val_direccion, "rb");
                                    if(file != NULL)
                                    {
                                        fseek(file, inicio_tabla_block+(inodo_r->i_block[0]*tam_bloque) ,SEEK_SET);
                                        fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                                        fclose(file);
                                    }
                                    int inodo_actual_posicion = bloque_carpeta_temporal->b_content[0].b_inodo;
                                    char *inodo_actual_nombre = bloque_carpeta_temporal->b_content[0].b_name;
                                    int bloque_pivote_posicion = primer_ajuste_bloque(1);
                                    /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                                    /*...................................................................*/
                                    int pos_inodo_nuevo = primer_ajuste_inodo(1);
                                    struct bloque_carpeta *bloque_carpeta_pivote = malloc(sizeof(struct bloque_carpeta));
                                    struct bloque_carpeta *bloque_carpeta_nuevo = malloc(sizeof(struct bloque_carpeta));
                                    struct inodo *inodo_nuevo = malloc(sizeof(struct inodo));
                                    /*-----------------------------------------------------------------------------------f*/
                                    /*___________________________________________________________________________________*/
                                    crearCarpeta_AIndirecto(bloque_apuntador_temporal,
                                                            ind -11,
                                                            inodo_r->i_block[ind],/**/
                                                            bloque_carpeta_pivote,
                                                            inodo_nuevo,
                                                            bloque_carpeta_nuevo,
                                                            direccion_total,
                                                            inodo_actual_posicion,
                                                            inodo_actual_nombre,
                                                            direccion,
                                                            pos_inodo_nuevo,
                                                            bloque_pivote_posicion);
                                    break;
                                }
                            }
                            else
                            {
                                ++indirectos;
                            }
                        }
                        if(guardadoFinal == 0 && indirectos > 0)
                        {
                            for(ind = 12; ind < 15; ind++)
                            {
                                if(inodo_r->i_block[ind] == -1)
                                {
                                    int new_ptr_block = primer_ajuste_bloque(1);
                                    crear_BloqueIndirecto(new_ptr_block, ind-11);
                                    break;
                                }
                            }
                            struct bloque_apuntador *bloque_apuntador_temporal = malloc(sizeof(struct bloque_apuntador));
                            FILE * file = fopen(val_direccion, "rb");
                            if(file != NULL)
                            {
                                fseek(file, inicio_tabla_block+(inodo_r->i_block[ind]*sizeof(struct bloque_apuntador)) ,SEEK_SET);
                                fread(bloque_apuntador_temporal, sizeof(struct bloque_apuntador), 1, file);
                                fclose(file);
                            }
                            if(indirecto_vacio(bloque_apuntador_temporal, ind -11) == -1)
                            {
                                guardadoFinal = 1;
                                struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_carpeta));
                                /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                                FILE * file = fopen(val_direccion, "rb");
                                if(file != NULL)
                                {
                                    fseek(file, inicio_tabla_block+(inodo_r->i_block[0]*tam_bloque) ,SEEK_SET);
                                    fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                                    fclose(file);
                                }
                                int inodo_actual_posicion = bloque_carpeta_temporal->b_content[0].b_inodo;
                                char *inodo_actual_nombre = bloque_carpeta_temporal->b_content[0].b_name;
                                int bloque_pivote_posicion = primer_ajuste_bloque(1);
                                /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                                /*...................................................................*/
                                int pos_inodo_nuevo = primer_ajuste_inodo(1);
                                struct bloque_carpeta *bloque_carpeta_pivote = malloc(sizeof(struct bloque_carpeta));
                                struct bloque_carpeta *bloque_carpeta_nuevo = malloc(sizeof(struct bloque_carpeta));
                                struct inodo *inodo_nuevo = malloc(sizeof(struct inodo));
                                /*-----------------------------------------------------------------------------------*/
                                /*___________________________________________________________________________________*/
                                crearCarpeta_AIndirecto(bloque_apuntador_temporal,
                                                        ind -11,
                                                        inodo_r->i_block[ind],/**/
                                                        bloque_carpeta_pivote,
                                                        inodo_nuevo,
                                                        bloque_carpeta_nuevo,
                                                        direccion_total,
                                                        inodo_actual_posicion,
                                                        inodo_actual_nombre,
                                                        direccion,
                                                        pos_inodo_nuevo,
                                                        bloque_pivote_posicion);
                            }
                        }

                    }
                    /*Buscando bloques directos sin utilizar...*/
                    /*-------------------------------------------------------*/
                }
            }else

            {
                printf("\n NO tiene permisos suficientes para crear la carpeta");
            }

        }else
        {
            printf("\n Son diferentes. Leyendo apuntadores");
            int i;
            for(i=0; i<12; i++)
            {
                if(inodo_r->i_block[i] != -1)
                {
                    /*printf("\n inodo_r->i_block[%d] = %d", i, inodo_r->i_block[i]);*/
                    int desplazamiento;
                    if(i == 0)
                    {
                        desplazamiento = 2;
                    }else
                    {
                        desplazamiento = 0;
                    }
                    int pos_block = inodo_r->i_block[i];
                    struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_carpeta))/*malloc(sizeof(struct bloque_archivo))*/;
                    FILE * file = fopen(val_direccion, "rb");
                    if(file != NULL)
                    {
                        fseek(file, inicio_tabla_block+(pos_block*sizeof(struct bloque_carpeta)) ,SEEK_SET);
                        fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                        fclose(file);
                    }
                    int j = desplazamiento;
                    printf("\n Leyendo inodos desde el bloque.");
                    for(; j<4; j++)
                    {
                        printf("\n bloque_carpeta_temporal->b_content[j].b_inodo = %d", bloque_carpeta_temporal->b_content[j].b_inodo);
                        if(bloque_carpeta_temporal->b_content[j].b_inodo != -1)
                        {
                            printf("\n Yendo al inodo->bloque->inodo %d", bloque_carpeta_temporal->b_content[j].b_inodo);
                            struct inodo *inodo_pivote = malloc(sizeof(struct inodo));
                            int inodo_num = bloque_carpeta_temporal->b_content[j].b_inodo;
                            file = fopen(val_direccion, "rb");
                            if(file != NULL)
                            {
                                fseek(file, inicio_tabla_inodo+(inodo_num*tam_inodo) ,SEEK_SET);
                                fread(inodo_pivote, sizeof(struct inodo), 1, file);
                                fclose(file);
                            }
                            printf("\n Recursividad.");
                            crear_carpeta_recursivo(inodo_pivote, direccion, direccion_total, direccion_a_buscar);
                        }
                    }
                }
            }
        }
    }else
    {
        /*DO NOTHING*/
    }
}

void crear_archivo_mkfile()
{
    printf("\n Creando archivo [%s]", folder_direccion);
    cargar_superbloque();
    if(bool_cont == 1)
    {
        file_reader();
    }
    char *dir = strdup(modificar_direccion_carpeta(folder_direccion));
    char *dir2 = strdup(modificar_direccion_a_buscar(folder_direccion));
    printf("\n Recuperando el primer inodo en: [%s] (%s)", dir2, dir);
    char **split_folder = str_split(dir, '?');
    int cantidad = tamano(split_folder);
    struct inodo *inodo_inicial = malloc(sizeof(struct inodo));
    FILE * file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_inode_start ,SEEK_SET);
        fread(inodo_inicial, sizeof(struct inodo), 1, file);
        fclose(file);
    }
    crear_archivo_recursivo(inodo_inicial, *(split_folder + cantidad-1), folder_direccion, dir2);
}

void crear_archivo_recursivo(struct inodo *inodo_r, char *direccion, char *direccion_total, char *direccion_a_buscar)
/*---------------------------------------------------------------------------------------------------------*/
{
    printf("\n Crear Archivo recursivo, %c, %s",inodo_r->i_type ,direccion_a_buscar);
    char *pathlink = inodo_r->i_pathlink;
    int tam_inodo = sizeof(struct inodo);
    int tam_bloque = 64;
    int inicio_tabla_block = super_bloque_principal->s_block_start;
    int inicio_tabla_inodo = super_bloque_principal->s_inode_start;
    if(inodo_r->i_type == '1')/*Es una carpeta...*/
    {
        printf("\n Comparando [%s][%s]", pathlink, direccion_a_buscar);
        if(strcmp(pathlink, direccion_a_buscar)==0)
        {
            printf("\n Direccion encontrada! se realizarán los procedimientos para crear archivo...[%s] D:[%s]", pathlink, direccion);
            int grupo =user_group_id;
            int permisos = permiso(inodo_r->i_uid, grupo, inodo_r->i_gid, inodo_r->i_perm, 2);
            if(permisos == 1)
            {
                printf("\n Tiene permiso de escritura en la carpeta padre.");
                int bool_bloque = 0;
                int k;
                int padre_i = 0;
                for(k = 0; k< 12; k++)/*Revisando los primeros 12 apuntadores directos*/
                {
                    /*printf("\n Revisando apuntador[%d] = bloque(%d)",k , inodo_r->i_block[k]);*/
                    if(inodo_r->i_block[k] != -1)
                    {
                        struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_archivo));
                        /*LEYENDO BLOQUE AL QUE ESTA APUNTANDO EL APUNTADOR DEL INODO-----------------*/
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, inicio_tabla_block+(inodo_r->i_block[k]*tam_bloque) ,SEEK_SET);
                            fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                            fclose(file);
                        }
                        /*LEYENDO BLOQUE AL QUE ESTA APUNTANDO EL APUNTADOR DEL INODO-----------------*/
                        int d;
                        if(k == 0)
                        {
                            d = 2;
                        }
                        else
                        {
                            d = 0;
                        }
                        for(; d< 4; d++)/*Revisando en cada una de las 4 casillas del bloque*/
                        {
                            if(bloque_carpeta_temporal->b_content[d].b_inodo == -1)
                            {
                                printf("\n Creando archivo en posicion de bloque %d...", d);
                                bool_bloque = 1;
                                /**************************/
                                /*ACTUALIZAR BLOQUE ACTUAL*/
                                /**************************/
                                int pos_inodo_nuevo = primer_ajuste_inodo(1);
                                bloque_carpeta_temporal->b_content[d].b_inodo = pos_inodo_nuevo;
                                FILE * file_update= fopen(val_direccion, "rb+");
                                if (file_update != NULL)
                                {
                                    fseek(file_update, super_bloque_principal->s_block_start+(sizeof(struct bloque_carpeta)*inodo_r->i_block[k]) ,SEEK_SET);
                                    fwrite(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file_update);
                                    fclose(file_update);
                                }
                                printf("\n Actualizando bloque %d con el valor %d", k, pos_inodo_nuevo);
                                /**************************/
                                /*ACTUALIZAR BLOQUE ACTUAL*/
                                /**************************/

                                /**************************/
                                /*CREAR INODO Y BLOQUE*/
                                /**************************/
                                struct inodo *nuevo_inodo = malloc(sizeof(struct inodo));
                                inicializarInodo_y_bloque_File(nuevo_inodo,
                                                                  direccion_total,
                                                                  direccion,
                                                                  pos_inodo_nuevo);


                                break;
                                /**************************/
                                /*CREAR INODO Y BLOQUE*/
                                /**************************/
                            }
                        }
                        if(bool_bloque == 1)
                        {
                            break;
                        }
                    }
                }
                if(bool_bloque == 0)
                {
                    /*no fue creado el archivo*/
                    /*-------------------------------------------------------*/
                    /*Buscando bloques directos sin utilizar...*/
                    int directo_vacio = 0;
                    int dir;
                    for(dir = 0; dir < 12; dir++)
                    {
                        if(inodo_r->i_block[dir] == -1)
                        {
                            directo_vacio = 1;
                            break;
                        }
                    }
                    if(directo_vacio == 1)
                    {
                        struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_carpeta));
                        /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                        FILE * file = fopen(val_direccion, "rb");
                        if(file != NULL)
                        {
                            fseek(file, inicio_tabla_block+(inodo_r->i_block[0]*tam_bloque) ,SEEK_SET);
                            fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                            fclose(file);
                        }
                        /*LEYENDO BLOQUE DEL INODO PADRE CON INFO DEL ACTUAL->PADRE----------*/
                        /*...................................................................*/
                        /*                          ACTUALIZAR INODO ACTUAL                  */
                        /*...................................................................*/
                        int inodo_actual_posicion = bloque_carpeta_temporal->b_content[0].b_inodo;
                        int bloque_pivote_posicion = primer_ajuste_bloque(1);
                        inodo_r->i_block[dir] = bloque_pivote_posicion;
                        file= fopen(val_direccion, "rb+");
                        if (file != NULL)
                        {
                            fseek(file, super_bloque_principal->s_inode_start+(sizeof(struct inodo)*inodo_actual_posicion) ,SEEK_SET);
                            fwrite(inodo_r, sizeof(struct inodo), 1, file);
                            fclose(file);
                        }
                        /*ACTUALIZAR INODO ACTUAL*******-------------*/
                        /*...........................................*/
                        /*CREANDO BLOQUE PIVOTE, INODO, Y BLOQUE NUEVOS*/
                        int pos_inodo_nuevo = primer_ajuste_inodo(1);
                        struct bloque_carpeta *bloque_carpeta_pivote = malloc(sizeof(struct bloque_carpeta));
                        struct inodo *inodo_nuevo = malloc(sizeof(struct inodo));
                        utilizar_Adirecto_Archivo(bloque_carpeta_pivote,
                                                  inodo_nuevo,
                                                  direccion_total,
                                                  direccion,
                                                  pos_inodo_nuevo,
                                                  bloque_pivote_posicion);
                    }
                    else/*Revisando en indirectos para poder guardar carpeta*/
                    {
                        int ind;
                        int indirectos = 0;
                        int guardadoFinal = 0;
                        for(ind = 12; ind < 15; ind++)
                        {
                            if(inodo_r->i_block[ind] != -1)
                            {
                                struct bloque_apuntador *bloque_apuntador_temporal = malloc(sizeof(struct bloque_apuntador));
                                FILE * file = fopen(val_direccion, "rb");
                                if(file != NULL)
                                {
                                    fseek(file, inicio_tabla_block+(inodo_r->i_block[ind]*sizeof(struct bloque_apuntador)) ,SEEK_SET);
                                    fread(bloque_apuntador_temporal, sizeof(struct bloque_apuntador), 1, file);
                                    fclose(file);
                                }
                                if(indirecto_vacio(bloque_apuntador_temporal, ind -11) == -1)
                                {
                                    guardadoFinal = 1;
                                    /*___________________________________________________________________________________*/
                                    int bloque_pivote_posicion = primer_ajuste_bloque(1);
                                    int pos_inodo_nuevo = primer_ajuste_inodo(1);
                                    struct bloque_carpeta *bloque_carpeta_pivote = malloc(sizeof(struct bloque_carpeta));
                                    struct inodo *inodo_nuevo = malloc(sizeof(struct inodo));
                                    /*___________________________________________________________________________________*/
                                    crearArchivo_AIndirecto(bloque_apuntador_temporal,
                                                            ind -11,
                                                            inodo_r->i_block[ind],/**/
                                                            bloque_carpeta_pivote,
                                                            inodo_nuevo,
                                                            direccion_total,
                                                            direccion,
                                                            pos_inodo_nuevo,
                                                            bloque_pivote_posicion);
                                    break;
                                }
                            }
                            else
                            {
                                ++indirectos;
                            }
                        }
                        if(guardadoFinal == 0 && indirectos > 0)
                        {
                            for(ind = 12; ind < 15; ind++)
                            {
                                if(inodo_r->i_block[ind] == -1)
                                {
                                    int new_ptr_block = primer_ajuste_bloque(1);
                                    crear_BloqueIndirecto(new_ptr_block, ind-11);
                                    inodo_r->i_block[ind] = new_ptr_block;
                                    break;
                                }
                            }
                            struct bloque_apuntador *bloque_apuntador_temporal = malloc(sizeof(struct bloque_apuntador));
                            FILE * file = fopen(val_direccion, "rb");
                            if(file != NULL)
                            {
                                fseek(file, inicio_tabla_block+(inodo_r->i_block[ind]*sizeof(struct bloque_apuntador)) ,SEEK_SET);
                                fread(bloque_apuntador_temporal, sizeof(struct bloque_apuntador), 1, file);
                                fclose(file);
                            }
                            if(indirecto_vacio(bloque_apuntador_temporal, ind -11) == -1)
                            {
                                guardadoFinal = 1;

                                int bloque_pivote_posicion = primer_ajuste_bloque(1);

                                int pos_inodo_nuevo = primer_ajuste_inodo(1);
                                struct bloque_carpeta *bloque_carpeta_pivote = malloc(sizeof(struct bloque_carpeta));
                                struct inodo *inodo_nuevo = malloc(sizeof(struct inodo));
                                /*-----------------------------------------------------------------------------------*/
                                /*___________________________________________________________________________________*/
                                crearArchivo_AIndirecto(bloque_apuntador_temporal,
                                                        ind -11,
                                                        inodo_r->i_block[ind],/**/
                                                        bloque_carpeta_pivote,
                                                        inodo_nuevo,
                                                        direccion_total,
                                                        direccion,
                                                        pos_inodo_nuevo,
                                                        bloque_pivote_posicion);
                            }
                        }

                    }
                    /*Buscando bloques directos sin utilizar...*/
                    /*-------------------------------------------------------*/
                }

            }
            else
            {
                printf("\n NO tiene permisos suficientes para crear la carpeta");
            }
        }else
        {
            printf("\n Son diferentes. Leyendo apuntadores");
            int i;
            for(i=0; i<12; i++)
            {
                /*printf("\n inodo_r->i_block[%d] = %d", i, inodo_r->i_block[i]);*/
                if(inodo_r->i_block[i] != -1)
                {
                    int desplazamiento;
                    if(i == 0)
                    {
                        desplazamiento = 2;
                    }else
                    {
                        desplazamiento = 0;
                    }
                    int pos_block = inodo_r->i_block[i];
                    struct bloque_carpeta *bloque_carpeta_temporal = malloc(sizeof(struct bloque_carpeta))/*malloc(sizeof(struct bloque_archivo))*/;
                    FILE * file = fopen(val_direccion, "rb");
                    if(file != NULL)
                    {
                        fseek(file, inicio_tabla_block+(pos_block*sizeof(struct bloque_carpeta)) ,SEEK_SET);
                        fread(bloque_carpeta_temporal, sizeof(struct bloque_carpeta), 1, file);
                        fclose(file);
                    }
                    int j = desplazamiento;
                    /*printf("\n Leyendo inodos.");*/
                    for(; j<4; j++)
                    {
                        /*printf("\n bloque_carpeta_temporal->b_content[j].b_inodo = %d", bloque_carpeta_temporal->b_content[j].b_inodo);*/
                        if(bloque_carpeta_temporal->b_content[j].b_inodo != -1)
                        {
                            /*printf("\n Yendo al espacio de bloque %d", bloque_carpeta_temporal->b_content[j].b_inodo);*/
                            struct inodo *inodo_pivote = malloc(sizeof(struct inodo));
                            int inodo_num = bloque_carpeta_temporal->b_content[j].b_inodo;
                            file = fopen(val_direccion, "rb");
                            if(file != NULL)
                            {
                                fseek(file, inicio_tabla_inodo+(inodo_num*tam_inodo) ,SEEK_SET);
                                fread(inodo_pivote, sizeof(struct inodo), 1, file);
                                fclose(file);
                            }
                            printf("\n Recursividad.");
                            crear_archivo_recursivo(inodo_pivote, direccion, direccion_total, direccion_a_buscar);
                        }
                    }
                }
            }
        }
    }else
    {
        /*DO NOTHING*/
    }
}

void file_system_loss()
{
    cargar_superbloque();
    printf("\n File System Loss t_p %d i_p %d val_direccion %s", tamano_particion_publico, inicio_particion_publico, val_direccion);
    char formato[1];
    formato[0] = '\0';
    int posicion = inicio_particion_publico + sizeof(struct super_bloque) + (sizeof(struct j_data)*super_bloque_principal->s_inodes_count) ;
    int t_bytes = tamano_particion_publico - (sizeof(struct super_bloque) + (sizeof(struct j_data)*super_bloque_principal->s_inodes_count) );
    FILE *fp = fopen(val_direccion, "rb+");
    fseek(fp, posicion, SEEK_SET);
    printf("\n Borrando desde %d la cantidad de %d t_bytes.", posicion, t_bytes);
    int i;
    for(i=0; i< t_bytes-1; i++)
    {
        fputc('\0', fp);
    }
    fclose(fp);
}

void recuperar_EXT3()
{
    cargarArchivoDisco2();
    if(bool_unit == 0)
    {
        val_unit[0]='k';
    }
    if(bool_filesystem == 0)
    {
        val_filesystem = '3';
    }
    int bool_encontrado = 0;
    int tamano_particion;
    char tipo_particion;
    int inicio_particion;
    size_t tamano_superbloque;
    size_t tamano_nodos;
    size_t tamano_bloques;
    printf("\n Buscando particion...");
    int i;
    for(i = 0; i < 4; i++)
    {
        printf("\n status %c", mbr_p->mbr_partition[i].part_status);
        if(mbr_p->mbr_partition[i].part_status == '1')
        {
            char *part_name_char = mbr_p->mbr_partition[i].part_name;
            printf("\n Buscando particion: [[%s]] -> [[%s]]", part_name_char, val_partname);
            if(strcmp(part_name_char, val_partname)==0)
            {
                printf("\n Encontrado.");
                tamano_particion = mbr_p->mbr_partition[i].part_size;
                tipo_particion = mbr_p->mbr_partition[i].part_type;
                inicio_particion = mbr_p->mbr_partition[i].part_start;
                inicio_particion_publico = inicio_particion;
                bool_encontrado = 1;
                break;
            }
        }
    }


    printf("\n Recovery...");
    struct super_bloque *super_block = malloc(sizeof(struct super_bloque));
    FILE *file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, inicio_particion_publico, SEEK_SET);
        fread(super_block, sizeof(struct super_bloque), 1, file);
        fclose(file);
    }
    super_bloque_principal = super_block;
    struct j_data *journaling_struct = malloc(sizeof(struct j_data));
    struct bloque_carpeta *folder_block = malloc(sizeof(struct bloque_carpeta));
    int numero_estructuras = super_block->s_inodes_count;
    printf("\n inicio_particion %d", inicio_particion);
    printf("\n bitmap inode %d", super_bloque_principal->s_bm_inode_start);
    printf("\n bitmap block %d", super_bloque_principal->s_bm_block_start);

    /*-------------------------------------------------------------------*/
    /*----------------------ESCRITURA DEL JOURNALING---------------------*/
    /*journaling_struct->journal_tipo_operacion=0;
    journaling_struct->ultimo=1;
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, inicio_particion_publico + sizeof(struct super_bloque) ,SEEK_SET);
        fwrite(journaling_struct, sizeof(struct j_data), 1, file);
        fclose(file);
    }*/
    /*-------------------------------------------------------------------*/
    /*-------------------ESCRITURA DEL BITMAP DE INODOS------------------*/
    char inodebitmap[numero_estructuras];
    inodebitmap[0] = '1';
    inodebitmap[1] = '1';
    int d;
    for(d = 2; d< numero_estructuras; d++)
    {
        inodebitmap[d] = '0';
    }
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_block->s_bm_inode_start ,SEEK_SET);
        fwrite(inodebitmap, sizeof(inodebitmap), 1, file);
        fclose(file);
    }
    /*-------------------------------------------------------------------*/
    /*------------------ESCRITURA DEL BITMAP DE BLOQUES------------------*/
    char blockbitmap[numero_estructuras*3];
    blockbitmap[0] = '1';
    blockbitmap[1] = '1';
    for(d = 2; d< numero_estructuras*3; d++)
    {
        blockbitmap[d] = '0';
    }
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_block->s_bm_block_start ,SEEK_SET);
        fwrite(blockbitmap, sizeof(blockbitmap), 1, file);
        fclose(file);
    }
    /*-------------------------------------------------------------------*/
    /*----------------------ESCRITURA DEL INODO ROOT---------------------*/
    struct inodo *root = malloc(sizeof(struct inodo));
    root->i_perm = 777;
    root->i_gid = 1;
    root->i_uid = 1;
    root->i_nlink = 0;
    strncpy(root->i_pathlink, "/", 100);
    root->i_size = 0;
    /*FECHA*/
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(root->i_atime, sizeof(root->i_atime), "%Y/%m/%d %H:%M", tm);
    strftime(root->i_ctime, sizeof(root->i_ctime), "%Y/%m/%d %H:%M", tm);
    strftime(root->i_mtime, sizeof(root->i_mtime), "%Y/%m/%d %H:%M", tm);
    /*FECHA*/
    root->i_block[0]=0;
    for(d=1; d<15; d++)
    {
        root->i_block[d]=-1;
    }
    root->i_type = '1';
    size_t tam123 = sizeof(root);
    int digito = tam123;
    /*printf("\n Tamano del inodo: %d  Lugar del inodo, %d", digito, super_block->s_inode_start);*/
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_block->s_inode_start ,SEEK_SET);
        fwrite(root, sizeof(struct inodo), 1, file);
        fclose(file);
    }
    int inodo_users_pos = 1;
    /*-------------------------------------------------------------------*/
    /*----------------------ESCRITURA DEL BLOQUE ROOT--------------------*/
    strncpy(folder_block->b_content[0].b_name, "/", 12);
    folder_block->b_content[0].b_inodo = 0;
    strncpy(folder_block->b_content[1].b_name, "/", 12);
    folder_block->b_content[1].b_inodo = 0;
    folder_block->b_content[2].b_inodo = 1;
    strncpy(folder_block->b_content[2].b_name, "users.txt", 12);
    folder_block->b_content[3].b_inodo = -1;
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_block->s_block_start ,SEEK_SET);
        fwrite(folder_block, sizeof(struct bloque_carpeta), 1, file);
        fclose(file);
        /*printf("\n El bloque se escribió en %d   %d", super_block->s_block_start, sizeof(struct bloque_carpeta));*/
    }
    /*-------------------------------------------------------------------*/
    /*-----------------ESCRITURA INODO ARCHIVO USUARIOS------------------*/
    /*USERS Inicializacion*/
    char *users_txt = "1,G,root\n1,U,root,root,201403794\n";
    struct inodo *users = malloc(sizeof(struct inodo));
    int u;
    for(u = 0; u < 15; u++)
    {
        users->i_block[u]=-1;
    }
    users->i_uid = 1;
    users->i_gid = 1;
    int int_len = strlen(users_txt);
    users->i_size = int_len;
    users->i_type = '0';
    users->i_perm = 777;
    time_t t2 = time(NULL);
    struct tm *tm2 = localtime(&t2);
    strftime(users->i_atime, sizeof(users->i_atime), "%Y/%m/%d %H:%M", tm2);
    strftime(users->i_ctime, sizeof(users->i_ctime), "%Y/%m/%d %H:%M", tm2);
    strftime(users->i_mtime, sizeof(users->i_mtime), "%Y/%m/%d %H:%M", tm2);

    strncpy(users->i_pathlink, "/users.txt", 100);

    double double_len = (double)int_len;
    double double_block_quantity = ceil(double_len/60.0);
    int int_block_quantity = (int)double_block_quantity;
    printf("\n Cantidad de bloques para separar el users.txt = %d  largo cadena = %d", int_block_quantity, int_len);
    int ix = 0;
    int b;
    for(b = 0; b< int_block_quantity; b++)
    {
        struct bloque_archivo *user_block = malloc(sizeof(struct bloque_archivo));
        int pos_user = 1;
        int seis;
        if(ix < int_len)
        {
            /*Asignando el contenido del archivo*/
            for(seis = 0; seis < 64; seis++)
            {
                user_block->b_content[seis] = *(users_txt + ix);
                ++ix;
            }
            /*ESCRIBIENDO EL BLOQUE ARCHIVO */
            FILE *fileE = fopen(val_direccion, "rb+");
            if(fileE != NULL)
            {
                fseek(fileE, super_block->s_block_start+(sizeof(struct bloque_archivo)*pos_user),SEEK_SET);
                fwrite(user_block, sizeof(struct bloque_archivo), 1, fileE);
                fclose(fileE);
            }
            /*FIN*/
            users->i_block[b] = pos_user;
            if(b<12)
            {
                printf("\n Asignando bloque archivo al apuntador directo: %d", b);
            }else
            {
                printf("\n Buscando en apuntadores indirectos espacio para el bloque");
                int f;
                for(f = 12; f<15 ; f++)
                {
                    if(users->i_block[f] == -1)
                    {
                        /*Inicializar indirectos*/
                        printf("\n Inicializando Apuntador_indirecto %d", f);
                        int bloque_indirecto_pos = primer_ajuste_bloque(1);
                        crear_BloqueIndirecto_Archivo(bloque_indirecto_pos, f-11);
                    }else
                    {
                        /*LEER EL BLOQUE DE APUNTADORES QUE ESTA EN ESE APUNTADOR INDIRECTO*/
                        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
                        FILE * file= fopen(val_direccion, "rb");
                        if (file != NULL)
                        {
                            fseek(file, super_block->s_block_start+(sizeof(struct bloque_apuntador)*users->i_block[f]) ,SEEK_SET);
                            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
                            fclose(file);
                        }
                        /*------------------------------------------------------------------*/
                        if(indirecto_vacio(b_ptr, f-11) == -1)
                        {
                            /*Ocupar apuntador*/
                            ocupar_AIndirecto_archivo(f-11, users->i_block[f], pos_user);
                            break;
                        }
                    }

                }
            }
        }
    }
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_block->s_inode_start+(sizeof(struct inodo)*1) ,SEEK_SET);
        fwrite(users, sizeof(struct inodo), 1, file);
        fclose(file);
    }
    iniciar_sesion2();
    /*-----------------------------------------------------------------------*/
    /********************************RECUPERANDO******************************/
    /*-----------------------------------------------------------------------*/
    int ultimo = 0;
    int journal_start = inicio_particion_publico + sizeof(struct super_bloque);
    struct j_data *journal_l = malloc(sizeof(struct j_data));
    int contador = 0;
    journal_permiso_escritura = 0;
    while(ultimo == 0)
    {
        printf("\n --------------------------Contador %d ultimo %d", contador, ultimo);
        file = fopen(val_direccion, "rb");
        if(file != NULL)
        {
            fseek(file, journal_start + (contador * sizeof(struct j_data)), SEEK_SET);
            fread(journal_l, sizeof(struct j_data), 1, file);
            fclose(file);
            ultimo = journal_l->ultimo;
        }
        if(contador != 0)
        {
            /*Usuario y grupo*/
            user_id = journal_l->journal_propietario;
            user_group_id = journal_l->journal_grupo;
            /*Usuario y grupo*/
            /*Recuperar[ejecutar accion]*/
            if(journal_l->journal_tipo_operacion == 1)
            {
                /*void crear_carpeta_recursivo(struct inodo *inodo_r, char *direccion, char *direccion_total, char *direccion_a_buscar)*/
                printf("\n Creando carpeta.");
                struct inodo *inodo_inicial = malloc(sizeof(struct inodo));
                file = fopen(val_direccion, "rb");
                if(file != NULL)
                {
                    fseek(file, super_bloque_principal->s_inode_start, SEEK_SET);
                    fread(inodo_inicial, sizeof(struct inodo), 1, file);
                    fclose(file);
                }
                int x;
                for(x = 0; x < 15; x++)
                {
                    printf("\n %d", inodo_inicial->i_block[x]);
                }
                /*-------------------------------------------*/
                folder_direccion = journal_l->journal_nombre;
                /*-------------------------------------------*/
                printf("\n *****************************************");
                printf("\n Recuperando carpeta [%s]", folder_direccion);
                cargar_superbloque();
                char *dir = strdup(modificar_direccion_carpeta(folder_direccion));
                char *dir2 = strdup(modificar_direccion_a_buscar(folder_direccion));
                printf("\n ------->  dir2[%s] dir(%s)", dir2, dir);
                char **split_folder = str_split(dir, '?');
                int cantidad = tamano(split_folder);

                crear_carpeta_recursivo(inodo_inicial, *(split_folder + cantidad-1), folder_direccion, dir2);
                printf("\n *****************************************");
            }
            else if(journal_l->journal_tipo_operacion == 2)
            {
                /*(struct inodo *inodo_i,
                                       char *direccion_total,
                                       char *direccion,
                                       int pos_inodo_actual)*/
                bool_cont = journal_l->journal_tipo_contenido;
                folder_direccion = journal_l->journal_nombre;
                if(bool_cont == 1)
                {
                    val_cont = journal_l->journal_contenido;
                }
                else
                {
                    val_size = atoi(journal_l->journal_contenido);
                }
                printf("\n Recuperando archivo [%s]", folder_direccion);
                cargar_superbloque();
                if(bool_cont == 1)
                {
                    file_reader();
                }
                char *dir = strdup(modificar_direccion_carpeta(folder_direccion));
                char *dir2 = strdup(modificar_direccion_a_buscar(folder_direccion));
                printf("\n dir2[%s] dir(%s) val_size(%d), bool_cont %d", dir2, dir, val_size, bool_cont);
                char **split_folder = str_split(dir, '?');
                int cantidad = tamano(split_folder);

                struct inodo *inodo_inicial = malloc(sizeof(struct inodo));
                file = fopen(val_direccion, "rb");
                if(file != NULL)
                {
                    fseek(file, super_bloque_principal->s_inode_start, SEEK_SET);
                    fread(inodo_inicial, sizeof(struct inodo), 1, file);
                    fclose(file);
                }
                crear_archivo_recursivo(inodo_inicial, *(split_folder + cantidad-1), folder_direccion, dir2);
                bool_cont = 0;
            }
        }
        ++contador;
    }
    journal_permiso_escritura = 1;
}

void armarCadena()
{
    int i;
    char *comma = ",";
    char *enter = "\n";
    for(i = 0; i < 10; i++)
    {
        if(GUID[i]!=0)
        {
            char id[10];
            sprintf(id, "%d", GUID[i]);
            char *id_ptr = id;
            char *tipo = "G";
            char *nombre_grupo = grupos[i];
            printf("\n Concatenando %s,%s,%s", id_ptr, tipo, nombre_grupo);
            total_armado = concatenacion(total_armado, id_ptr);
            total_armado = concatenacion(total_armado, comma);
            total_armado = concatenacion(total_armado, tipo);
            total_armado = concatenacion(total_armado, comma);
            total_armado = concatenacion(total_armado, nombre_grupo);
            total_armado = concatenacion(total_armado, enter);
        }
    }
    printf("\n Grupos recuperados: %s", total_armado);
    for(i = 0; i < 20; i++)
    {
        if(UID[i]!=0)
        {
            char id[10];
            sprintf(id, "%d", UID[i]);
            char *id_ptr = id;
            char *tipo = "U";
            char *nombre_grupo = usuarios_grp[i];
            char *nombre_usuario = usuarios_usr[i];
            char *contrasena = usuarios_pwd[i];
            printf("\n Concatenando %s,%s,%s,%s,%s", id_ptr, tipo, nombre_grupo, nombre_usuario, contrasena);
            total_armado = concatenacion(total_armado, id_ptr);
            total_armado = concatenacion(total_armado, comma);
            total_armado = concatenacion(total_armado, tipo);
            total_armado = concatenacion(total_armado, comma);
            total_armado = concatenacion(total_armado, nombre_grupo);
            total_armado = concatenacion(total_armado, comma);
            total_armado = concatenacion(total_armado, nombre_usuario);
            total_armado = concatenacion(total_armado, comma);
            total_armado = concatenacion(total_armado, contrasena);
            total_armado = concatenacion(total_armado, enter);
        }
    }
    printf("\n Grupos y usuarios recuperados: %s", total_armado);
}

void cat_interfaz()
{
    printf("\n CAT [%s]", val_cat);
    cargar_superbloque();
    char *dir = strdup(modificar_direccion_carpeta(val_cat));
    char *dir2 = strdup(modificar_direccion_a_buscar(val_cat));
    printf("\n Recuperando el primer inodo en: dir2[%s] dir(%s)", dir2, dir);
    char **split_folder = str_split(dir, '?');
    int cantidad = tamano(split_folder);
    char inode_bitmap[super_bloque_principal->s_inodes_count];
    FILE *file = fopen(val_direccion, "rb");
    if(file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
        fread(inode_bitmap, super_bloque_principal->s_inodes_count, 1, file);
        fclose(file);
    }
    int i;
    for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
    {
        if(inode_bitmap[i] == '1')
        {
            struct inodo *inodo_archivo = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            if(file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start+(i*sizeof(struct inodo)) ,SEEK_SET);
                fread(inodo_archivo, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            printf("\n Tipo inodo %c", inodo_archivo->i_type);
            if(inodo_archivo->i_type == '0')
            {
                char *token = inodo_archivo->i_pathlink;
                printf("\n comparando %s %s", token, val_cat);
                if(strcmp(token, val_cat)==0)
                {
                    int permisos = permiso(inodo_archivo->i_uid, user_group_id, inodo_archivo->i_gid, inodo_archivo->i_perm, 1);
                    if(permisos == 1)
                    {
                        printf("\n CAT: %s -------------------\n", val_cat);
                        int j;
                        for(j = 0; j < 12; j++)
                        {
                            if(inodo_archivo->i_block[j] != -1)
                            {
                                struct bloque_archivo *bloque_letras = malloc(sizeof(struct bloque_archivo));
                                file = fopen(val_direccion, "rb");
                                if(file != NULL)
                                {
                                    fseek(file, super_bloque_principal->s_block_start+(inodo_archivo->i_block[j]*sizeof(struct bloque_archivo)) ,SEEK_SET);
                                    fread(bloque_letras, sizeof(struct bloque_archivo), 1, file);
                                    fclose(file);
                                }
                                char *contentt = bloque_letras->b_content;
                                printf("%s", contentt);
                            }
                        }
                        for(j = 12; j<15; j++)
                        {
                            if(inodo_archivo->i_block[j] != -1)
                            {
                                print_indirecto(inodo_archivo->i_block[j], j-11);
                            }
                        }
                        printf("\n Fin archivo\n");
                    }
                    else
                    {
                        printf("\n No tiene permisos de lectura suficientes.");
                    }
                }
            }
        }
    }
}

void eliminar_bloque(int bloque)
{

    char bitmap_block[super_bloque_principal->s_blocks_count];
    FILE * file= fopen(val_direccion, "rb");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
        fread(bitmap_block, super_bloque_principal->s_blocks_count, 1, file);
        fclose(file);
    }
    bitmap_block[bloque]='0';
    file= fopen(val_direccion, "rb+");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_block_start ,SEEK_SET);
        fwrite(bitmap_block, super_bloque_principal->s_blocks_count, 1, file);
        fclose(file);
    }
}

void eliminar_bloque_indirecto(int grado, int pos_ptr)
{
    if(grado == 1)
    {
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file= fopen(val_direccion, "rb");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(pos_ptr*sizeof(struct bloque_apuntador)) ,SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        int i;
        for(i = 0; i<16; i++)
        {
            if(b_ptr->b_pointer[i]!=-1)
            {
                eliminar_bloque(b_ptr->b_pointer[i]);
                b_ptr->b_pointer[i]=-1;
            }
        }
    }
    else
    {
        struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
        FILE * file= fopen(val_direccion, "rb");
        if (file != NULL)
        {
            fseek(file, super_bloque_principal->s_block_start+(pos_ptr*sizeof(struct bloque_apuntador)) ,SEEK_SET);
            fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
            fclose(file);
        }
        int i;
        for(i = 0; i<16; i++)
        {
            if(b_ptr->b_pointer[i]!=-1)
            {
                eliminar_bloque_indirecto(grado-1,b_ptr->b_pointer[i]);
            }
        }
    }
}

void actualizar_contenido(char *archivo, char *contenido)
{
    printf("\n Actualizar Contenido.");
    char bitmap_inode[super_bloque_principal->s_inodes_count];
    FILE *file= fopen(val_direccion, "rb");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
        fread(bitmap_inode, super_bloque_principal->s_inodes_count, 1, file);
        fclose(file);
    }
    int i;
    for(i=0; i<super_bloque_principal->s_inodes_count; i++)
    {
        if(bitmap_inode[i]=='1')
        {
            struct inodo *inodo_lectura = malloc(sizeof(struct inodo));
            file= fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start+(i*sizeof(struct inodo)) ,SEEK_SET);
                fread(inodo_lectura, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            char *d = inodo_lectura->i_pathlink;
            printf("\n Comparando %s %s", d, archivo);
            if(strcmp(d, archivo) == 0)/*ES EL ARCHIV0*/
            {
                int permL = permiso(inodo_lectura->i_uid, user_group_id, inodo_lectura->i_gid, inodo_lectura->i_perm, 1);
                int permE = permiso(inodo_lectura->i_uid, user_group_id, inodo_lectura->i_gid, inodo_lectura->i_perm, 2);
                printf("\n permiso lectura: %d", permL);
                printf("\n permiso escritura: %d", permE);
                if(permL == 1 && permE == 1)
                {
                    inodo_lectura->i_size = strlen(contenido);
                    /*printf("\n COINCIDE EN %d !!!!!!!!!!!!!!!!!!!!", i);*/
                    int d;
                    for(d = 0; d<12; d++)
                    {
                        if(inodo_lectura->i_block[d] != -1)
                        {
                            printf("\n LIBERANDO EL BLOQUE %d", inodo_lectura->i_block[d] );
                            eliminar_bloque(inodo_lectura->i_block[d]);
                            inodo_lectura->i_block[d] = -1;
                        }
                    }
                    for(d = 12; d<15; d++)
                    {
                        if(inodo_lectura->i_block[d] != -1)
                        {
                            eliminar_bloque_indirecto(d-11, inodo_lectura->i_block[d]);
                        }
                    }
                    /************************************************************************/
                    /*                      ACTUALIZANDO CONTENIDO                          */
                    /************************************************************************/
                    printf("\n NUEVA CADENA: %s", contenido);
                    int cadena_tam = strlen(contenido);
                    double cadena_tam_d = (double)cadena_tam;
                    float f = cadena_tam_d/64.0;
                    double bloques_archivo_d = ceil(f);
                    int bloques_archivo = (int) bloques_archivo_d;
                    printf("\n bloques f:%f buffer_size:%d cadena_tam:%d cadena_tam_d:%f bloques:%d", f, buffer_size, cadena_tam, cadena_tam_d, bloques_archivo);
                    int apuntador = 0;
                    int contador = 0;
                    int i2;
                    int j;
                    for(i2 = 0; i2 < bloques_archivo; i2++)
                    {
                        /*ASIGNAR CADENA AL BLOQUE-ARCHIVO*/
                        struct bloque_archivo *bloque_archivo_nuevo = malloc(sizeof(struct bloque_archivo));
                        for(j = 0; j < 64; j++)
                        {
                            if(contador < cadena_tam)
                            {
                                if(*(contenido + contador) != '\0')
                                {
                                    bloque_archivo_nuevo->b_content[j] = *(contenido + contador);
                                    /*printf("\n Asignando %c", bloque_archivo_nuevo->b_content[j]);*/
                                }
                                ++contador;
                            }
                        }
                        char *cad = bloque_archivo_nuevo->b_content;
                        printf("\n contenido bloque actualizado %d: *\n%s\n*", i2, cad);
                        int pos_block = primer_ajuste_bloque(1);
                        /*--------------------------------*/
                        /*ESCRIBIENDO EL BLOQUE ARCHIVO */
                        FILE *fileE = fopen(val_direccion, "rb+");
                        if(fileE != NULL)
                        {
                            fseek(fileE, super_bloque_principal->s_block_start+(sizeof(struct bloque_archivo)*pos_block),SEEK_SET);
                            fwrite(bloque_archivo_nuevo, sizeof(struct bloque_archivo), 1, fileE);
                            fclose(fileE);
                        }
                        /*ESCRIBIENDO EL BLOQUE ARCHIVO */
                        /*--------------------------------*/
                        if(apuntador < 12)
                        {
                            inodo_lectura->i_block[apuntador] = pos_block;
                            printf("\n Asignando bloque:%d archivo al apuntador directo: %d", pos_block, apuntador);
                        }else
                        {
                            printf("\n Buscando en apuntadores indirectos espacio para el bloque");
                            int f;
                            for(f = 12; f<15 ; f++)
                            {
                                if(inodo_lectura->i_block[f] == -1)
                                {
                                    /*Inicializar indirectos*/
                                    printf("\n Inicializando Apuntador_indirecto %d", f);
                                    int bloque_indirecto_pos = primer_ajuste_bloque(1);
                                    crear_BloqueIndirecto_Archivo(bloque_indirecto_pos, f-11);
                                }else
                                {
                                    /*LEER EL BLOQUE DE APUNTADORES QUE ESTA EN ESE APUNTADOR INDIRECTO*/
                                    struct bloque_apuntador *b_ptr = malloc(sizeof(struct bloque_apuntador));
                                    FILE * file= fopen(val_direccion, "rb");
                                    if (file != NULL)
                                    {
                                        fseek(file, super_bloque_principal->s_block_start+(sizeof(struct bloque_apuntador)*inodo_lectura->i_block[f]) ,SEEK_SET);
                                        fread(b_ptr, sizeof(struct bloque_apuntador), 1, file);
                                        fclose(file);
                                    }
                                    /*------------------------------------------------------------------*/
                                    if(indirecto_vacio(b_ptr, f-11) == -1)
                                    {
                                        /*Ocupar apuntador*/
                                        ocupar_AIndirecto_archivo(f-11, inodo_lectura->i_block[f], pos_block);
                                        break;
                                    }
                                }

                            }
                        }
                        ++apuntador;
                    }
                    printf("\n Actualizando...");
                    /*int y;
                    for(y = 0; y<12; y++)
                    {
                        printf("\n inodo_lectura[%d] = %d", y, inodo_lectura->i_block[y]);
                    }*/
                    file = fopen(val_direccion, "rb+");
                    if(file != NULL)
                    {
                        fseek(file, super_bloque_principal->s_inode_start+(i*sizeof(struct inodo)), SEEK_SET);
                        fwrite(inodo_lectura, sizeof(struct inodo), 1, file);
                        fclose(file);
                    }
                    /************************************************************************/
                    /*                      ACTUALIZANDO CONTENIDO                          */
                    /************************************************************************/
                }else
                {
                    printf("\n No se tienen los permisos de escritura y lectura para modificar el archivo.");
                }
            }
        }
    }
}

void mkgrp_procedure()
{
    cargar_superbloque();
    iniciar_sesion2();
    armarCadena();
    /*nombre_USER_COMMAND*/
    int mayor=0;
    int i;
    for(i = 0; i<10; i++)
    {
        if(GUID[i]>mayor)
        {
            mayor = GUID[i];
        }
    }
    ++mayor;
    char *comma = ",";
    char *enter = "\n";
    char id[15];
    sprintf(id, "%d", mayor);
    char *id_ptr = id;
    char *Type = "G";
    /*CONCATENANDO*/
    total_armado = concatenacion(total_armado, id_ptr);
    total_armado = concatenacion(total_armado, comma);
    total_armado = concatenacion(total_armado, Type);
    total_armado = concatenacion(total_armado, comma);
    total_armado = concatenacion(total_armado, nombre_USER_COMMAND);
    total_armado = concatenacion(total_armado, enter);
    printf("\n Luego de agregar el grupo este es el resultado: %s", total_armado);
    char *archivo = "/users.txt";
    actualizar_contenido(archivo, total_armado);
}

void rmgrp_procedure()
{
    cargar_superbloque();
    iniciar_sesion2();
    /*nombre_USER_COMMAND*/
    int mayor=0;
    int i;
    for(i = 0; i<10; i++)
    {
        char *d = grupos[i];
        if(strcmp(d,nombre_USER_COMMAND)==0)
        {
            GUID[i]=0;
        }
    }
    armarCadena();
    printf("\n Luego de eliminar el grupo este es el resultado: %s", total_armado);
    char *archivo = "/users.txt";
    actualizar_contenido(archivo, total_armado);
}

void mkuser_procedure()
{
    cargar_superbloque();
    iniciar_sesion2();
    armarCadena();
    /*nombre_USER_COMMAND*/
    int mayor=0;
    int i;
    for(i = 0; i<20; i++)
    {
        if(UID[i]>mayor)
        {
            mayor = GUID[i];
        }
    }
    ++mayor;
    char *comma = ",";
    char *enter = "\n";
    char id[15];
    sprintf(id, "%d", mayor);
    char *id_ptr = id;
    char *Type = "U";
    /*CONCATENANDO*/
    total_armado = concatenacion(total_armado, id_ptr);
    total_armado = concatenacion(total_armado, comma);
    total_armado = concatenacion(total_armado, Type);
    total_armado = concatenacion(total_armado, comma);
    total_armado = concatenacion(total_armado, grp_USER_COMMAND);
    total_armado = concatenacion(total_armado, comma);
    total_armado = concatenacion(total_armado, nombre_USER_COMMAND);
    total_armado = concatenacion(total_armado, comma);
    total_armado = concatenacion(total_armado, pwd_USER_COMMAND);
    total_armado = concatenacion(total_armado, enter);
    printf("\n Luego de agregar el grupo este es el resultado: %s", total_armado);
    char *archivo = "/users.txt";
    actualizar_contenido(archivo, total_armado);
}

void rmusr_procedure()
{
    cargar_superbloque();
    iniciar_sesion2();
    /*nombre_USER_COMMAND*/
    int mayor=0;
    int i;
    for(i = 0; i<20; i++)
    {
        char *d = usuarios_usr[i];
        if(strcmp(d,user)==0)
        {
            UID[i]=0;
        }
    }
    armarCadena();
    printf("\n Luego de eliminar el usuario este es el resultado: %s", total_armado);
    char *archivo = "/users.txt";
    actualizar_contenido(archivo, total_armado);
}

void file_edit()
{
    printf("\n FILE-EDIT %d", bool_cont);
    if(bool_cont == 1)
    {
        /*path direccion = val_direccion  ;  val_cont*/
        cargar_superbloque();
        file_reader();
        actualizar_contenido(folder_direccion, buffer_store);
    }
    else
    {
        cargar_superbloque();
        char arr_contenido[val_size+1];
        arr_contenido[val_size]='\0';
        int contador = 1;
        int i;
        for(i = 0; i<val_size; i++)
        {
            arr_contenido[i] = contador + '0';
            if(contador == 9)
            {
                contador = 1;
            }
            else
            {
                ++contador;
            }
        }
        actualizar_contenido(folder_direccion, arr_contenido);
    }
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
void quitarInterrogacion(char *cadena)
{
    int tamano = strlen(cadena);
    int i;
    for(i = 0; i < tamano; i++)
    {
        if(*(cadena + i) == '?')
        {
            *(cadena + i) = ' ';
        }
    }
}
void automata(char** entradaTotal, char* entradaUnica, int posicion)
{
    printf("\n---------------\n");
    printf("Metodo automata: %d\n", tamanoArreglo);
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
        char *siz = "+size";
        char *unit = "+unit";
        char *path = "-path";
        char *name = "-name";

        char *type = "+type";
        char *type_n = "-type";
        char *fit = "+fit";
        char *deletee = "+delete";
        char *add = "+add";
        char *rmdisk = "rmdisk";
        char *full = "full";
        char *fast = "fast";
        char *full2 = "full\n";
        char *fast2 = "fast\n";
        char *mount= "mount";
        char *unmount= "unmount";
        char *un_id= "-id1";
        char *rep = "rep";
        char *id = "-id";
        char *exec = "exec";
        char *mkdirr = "mkdir";
        char *filesystem = "+fs";
        char *bs = "\\";
        char *bs2 = "\\\n";
        char *mkfs = "mkfs";
        char *fdisk = "fdisk";
        char *mkfile = "mkfile";
        char *cont = "+cont";
        char *cat = "cat";
        char *file1 = "-file1";
        char *file2 = "-file2";
        char *file3 = "-file3";
        char *file4 = "-file4";
        char *usr = "-usr";
        char *pwd = "-pwd";
        char *login = "login";
        char *logout = "logout";
        char *mkgrp = "mkgrp";
        char *mkusr = "mkusr";
        char *rmgrp = "rmgrp";
        char *rmusr = "rmusr";
        char *grp = "-grp";
        char *loss = "loss";
        char *recovery = "recovery";
        char *jjj = "journaling";

        char *edit = "edit";

        char *lsl = "ls+l";
        char *lsi = "ls+i";
        char *ruta = "+ruta";

        char *ugo = "-ugo";
        char *chmod_c = "chmod";
        char *plus_r = "+R";
        char *plus_r_m = "+r";
        char *rename_c = "ren";

        printf("TamanoArreglo: %d Token", tamanoArreglo, token);

        if(strcmp(token, mkdisk)==0)
        {
            bool_mkdisk = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, sizee)==0 || strcmp(token, siz)==0)
        {
            bool_sizee = 1;
            val_size = getValorEntero(*(entradaTotal + posicion));
            printf("\n ->2[%d]", val_size);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, unit)==0)
        {
            bool_unit = 1;
            char* unidad = getTexto(*(entradaTotal + posicion));
            val_unit[0] = unidad[0];
            printf(" UNIDAD: %c\n", val_unit[0]);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, path)==0)
        {
            bool_path = 1;
            quitarInterrogacion(*(entradaTotal + posicion));
            val_direccion = getValorCadena(*(entradaTotal + posicion));
            folder_direccion = strdup(val_direccion);;
            printf("\n PATH = %s", val_direccion);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, type)==0 || strcmp(token, type_n)==0)
        {
            bool_type = 1;
            printf(entradaUnica);
            char **split = str_split(entradaUnica, ':');
            if(strcmp(*(split+1), fast)==0 || strcmp(*(split+1), fast2)==0)
            {
                val_type[0]='0';
            }
            else if(strcmp(*(split+1), full)==0 || strcmp(*(split+1), full2)==0)
            {
                val_type[0]='1';
            }else
            {
                val_type[0] = *(entradaUnica + 6);
            }
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, name)==0)
        {
            bool_name = 1;
            quitarInterrogacion(*(entradaTotal + posicion));
            nombre = getValorDisco(*(entradaTotal + posicion));
            nombre_fdisk = getValorCadena(*(entradaTotal + posicion));
            nombre_USER_COMMAND = strdup(nombre_fdisk);
            printf("\n----NAME:\n");printf(nombre_USER_COMMAND);printf("\n----\n");
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
            printf("\n RECUPERADO UN ID:%s", val_un_id);
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
            /*printf("\n VINO UN BS!");*/
        }
        else if(strcmp(token, mkfs)==0)
        {
            bool_mkfs = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
            /*val_direccion = *(entradaTotal + posicion+1);*/
        }
        else if(strcmp(token, filesystem)==0)
        {
            char **split = str_split(entradaUnica, ':');
            char *valchar = *(split +1);
            val_filesystem = *(valchar);
            bool_filesystem = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, mkdirr)==0)
        {
            bool_mkdir = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, cont)==0)
        {
            bool_cont = 1;
            quitarInterrogacion(*(entradaTotal + posicion));
            val_cont = getValorCadena(*(entradaTotal + posicion));
            printf("\n VAL_CONT = %s", val_cont);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, mkfile)==0)
        {
            bool_mkfile = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, cat)==0)
        {
            bool_cat = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, file1)==0)
        {
            bool_cat = 1;
            quitarInterrogacion(*(entradaTotal + posicion));
            val_cat = getValorCadena(*(entradaTotal + posicion));
            printf("\n CAT = %s", val_cat);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, login)==0)
        {
            bool_login = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, pwd)==0)
        {
            char **split = str_split(entradaUnica, ':');
            password = *(split +1);
            pwd_USER_COMMAND = *(split +1);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, grp)==0)
        {
            char **split = str_split(entradaUnica, ':');
            grp_USER_COMMAND = *(split +1);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, usr)==0)
        {
            char **split = str_split(entradaUnica, ':');
            user = *(split +1);
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, logout)==0)
        {
            if(bool_logueado == 1)
            {
                printf("\n Sesion cerrada.");
                bool_logueado = 0;
            }
            else
            {
                printf("\n ERROR: no hay usuarios loggeados");
            }
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, mkgrp)==0)
        {
            bool_mkgrp = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, mkusr)==0)
        {
            bool_mkusr = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, rmgrp)==0)
        {
            bool_rmgrp = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, rmusr)==0)
        {
            bool_rmusr = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, jjj)==0)
        {
            bool_journaling = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, loss)==0)
        {
            bool_loss = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, recovery)==0)
        {
            bool_recovery = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, edit)==0)
        {
            bool_edit = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, lsl)==0)
        {
            bool_ls_i = 0;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, lsi)==0)
        {
            bool_ls_i = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, ruta)==0)
        {
            quitarInterrogacion(*(entradaTotal + posicion));
            ruta_char = getValorCadena(*(entradaTotal + posicion));
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, ugo)==0)
        {
            val_size = getValorEntero(*(entradaTotal + posicion));
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, plus_r)==0 || strcmp(token, plus_r_m)==0)
        {
            printf("\n PLUS R!!!!!\n");
            bool_plus_r = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, chmod_c)==0)
        {
            bool_chmod = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
        else if(strcmp(token, rename_c)==0)
        {
            bool_file_rename = 1;
            automata(entradaTotal,*(entradaTotal + posicion+1), posicion+1);
        }
    }else
    {
        printf("\nDesborde\n");
    }
}

void file_rename()
{
    /*folder_direccion*/
    cargar_superbloque();
    printf("\n Cambiar nombre de [%s] a [%s].", folder_direccion, nombre_USER_COMMAND);
    int tam_original = strlen(folder_direccion);
    int tam_pedazo_nuevo = strlen(nombre_USER_COMMAND);
    int i;
    int contador = 0;
    for(i = tam_original-1; i>-1; i--)
    {
        if(*(folder_direccion + i) =='/')
        {
            break;
        }
        else
        {
            ++contador;
        }
    }
    int resta = tam_original - contador;
    int tam_nuevo = resta + tam_pedazo_nuevo;
    printf("\n nuevo tamano %d", tam_nuevo);
    int iterador = 0;
    char nuevo_prenombre[resta+1];
    nuevo_prenombre[resta] = '\0';
    char nuevo_nombre[tam_nuevo + 1];
    for(i = 0; i< resta; i++)
    {
        nuevo_nombre[i] = *(folder_direccion + i);
        nuevo_prenombre[i] = *(folder_direccion + i);
        ++iterador;
    }
    for(i = 0; i<tam_pedazo_nuevo; i++)
    {
        printf("\n %c", *(nombre_USER_COMMAND + i));
        nuevo_nombre[iterador] = *(nombre_USER_COMMAND + i);
        ++iterador;
    }
    nuevo_nombre[tam_nuevo] = '\0';
    char *nm = nuevo_nombre;
    char *pn = nuevo_prenombre;
    printf("\n Nuevo nombre: [%s] prenombre [%s]", nm, pn);
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    char bitmap_inode[super_bloque_principal->s_inodes_count];
    FILE *file= fopen(val_direccion, "rb");
    if (file != NULL)
    {
        fseek(file, super_bloque_principal->s_bm_inode_start ,SEEK_SET);
        fread(bitmap_inode, super_bloque_principal->s_inodes_count, 1, file);
        fclose(file);
    }
    for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
    {
        if(bitmap_inode[i] == '1')
        {
            struct inodo *inodo_l = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start + (i*sizeof(struct inodo)) ,SEEK_SET);
                fread(inodo_l, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            char *d = inodo_l->i_pathlink;
            if(strcmp(folder_direccion, d)==0)
            {
                char *cpy = nuevo_nombre;
                strcpy(inodo_l->i_pathlink, cpy);
                file = fopen(val_direccion, "rb+");
                if(file != NULL)
                {
                    fseek(file, super_bloque_principal->s_inode_start + (i*sizeof(struct inodo)) ,SEEK_SET);
                    fwrite(inodo_l, sizeof(struct inodo), 1, file);
                    fclose(file);
                }
            }
        }
    }
    for(i = 0; i< super_bloque_principal->s_inodes_count; i++)
    {
        if(bitmap_inode[i] == '1')
        {
            struct inodo *inodo_l = malloc(sizeof(struct inodo));
            file = fopen(val_direccion, "rb");
            if (file != NULL)
            {
                fseek(file, super_bloque_principal->s_inode_start + (i*sizeof(struct inodo)) ,SEEK_SET);
                fread(inodo_l, sizeof(struct inodo), 1, file);
                fclose(file);
            }
            char *d = inodo_l->i_pathlink;
            if(strcmp(folder_direccion, d)==0)
            {

            }
            else
            {
                if(strstr(d, folder_direccion) != NULL)
                {
                    char *t = inodo_l->i_pathlink;
                    int actual_size = strlen(t);
                    int new_size = actual_size - tam_original + tam_nuevo;
                    printf("\n nuevo tamano %d", new_size);
                    char nuevo_de_reemplazo[new_size];
                    int j;
                    for(j = 0; j<tam_nuevo; j++)
                    {
                        nuevo_de_reemplazo[j] = nuevo_nombre[j];
                    }
                    int contador = tam_nuevo;
                    for(j = tam_original+1; j<actual_size; j++)
                    {
                        nuevo_de_reemplazo[contador] = *(t+j);
                        ++contador;
                    }
                    printf("\n nuevo reemplazo = [%s]", nuevo_de_reemplazo);
                    strcpy(inodo_l->i_pathlink, nuevo_de_reemplazo);
                    file = fopen(val_direccion, "rb+");
                    if(file != NULL)
                    {
                        fseek(file, super_bloque_principal->s_inode_start + (i*sizeof(struct inodo)) ,SEEK_SET);
                        fwrite(inodo_l, sizeof(struct inodo), 1, file);
                        fclose(file);
                    }
                }
            }
        }
    }
}

void file_reader()
{
    FILE *fp;
    long lSize;

    fp = fopen ( val_cont , "rb" );
    if( fp != NULL )
    {
        printf("\n FILE READER!");
        fseek( fp , 0L , SEEK_END);
        lSize = ftell( fp );
        rewind( fp );
        buffer_size = lSize;
        /* allocate memory for entire content */
        buffer_store = calloc( 1, lSize+1 );
        printf("\n Tamano recuperado %d", buffer_size);
        if( buffer_store == NULL )
        {
            fclose(fp);fputs("memory alloc fails",stderr);exit(1);
        }
        else
        {
            /* copy the file into the buffer */
            if( 1!=fread( buffer_store , lSize, 1 , fp) )
            {
                fclose(fp); free(buffer_store); fputs("entire read fails",stderr); exit(1);
            }
            /* do your work here, buffer is a string contains the whole text */
            fclose(fp);
            printf("\n Buffer store: %s", buffer_store);
        }
    }

}
void master_Driver()
{
    printf("\n MasterDriver");
    if(bool_mkdisk == 1)
    {
        /*Comando mkdisk utilizado*/
        if(bool_sizee==1 && bool_path==1 && bool_name==1)
        {
            if(bool_unit == 0)
            {
                val_unit[0] = 'm';
            }
            crearDisco();
        }
    }else if(bool_rmdisk == 1)
    {
        eliminarDisco();
    }else if(bool_fdisk == 1)
    {
        if(bool_unit == 0)
        {
            val_unit[0] = 'k';
        }
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
    }
    else if(bool_exec == 1)
    {
        execute_LBL();
    }
    else if(bool_mkfs == 1)
    {
        formatear_Particion();
    }
    else if(bool_mkdir == 1)
    {
        crear_carpeta_mkdir();
    }
    else if(bool_mkfile == 1)
    {
        crear_archivo_mkfile();
    }
    else if(bool_cat == 1)
    {
        cat_interfaz();
    }
    else if(bool_login == 1)
    {
        iniciar_sesion();
    }
    else if(bool_mkgrp == 1)
    {
        if(user_id == 1)
        {
        mkgrp_procedure();
        }else
        {
            printf("\n ERROR debe ser root");
        }
    }
    else if(bool_mkusr == 1)
    {
        if(user_id == 1)
        {
        mkuser_procedure();
        }else
        {
            printf("\n ERROR debe ser root");
        }
    }
    else if(bool_rmgrp == 1)
    {
        if(user_id == 1)
        {
        rmgrp_procedure();
        }else
        {
            printf("\n ERROR debe ser root");
        }
    }
    else if(bool_rmusr == 1)
    {
        if(user_id == 1)
        {
        rmusr_procedure();
        }else
        {
            printf("\n ERROR debe ser root");
        }
    }
    else if(bool_loss == 1)
    {
        file_system_loss();
    }
    else if(bool_recovery == 1)
    {
        recuperar_EXT3();
    }
    else if(bool_edit == 1)
    {
        file_edit();
    }
    else if(bool_chmod == 1)
    {
        chmod_u();
    }
    else if(bool_file_rename == 1)
    {
        file_rename();
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
                printf("\nFase2 ->%s", entradaUsuario2);
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
        bool_cont = 0;
        bool_mkfile = 0;
        bool_mkdir = 0;
        bool_cat = 0;
        bool_login = 0;
        bool_mkgrp = 0;
        bool_mkusr = 0;
        bool_rmgrp = 0;
        bool_rmusr = 0;
        bool_mkfs = 0;
        total_armado = "";
        total_recuperado = "";
        bool_loss = 0;
        bool_recovery = 0;
        bool_edit = 0;
        bool_ls_i =0;
        bool_plus_r = 0;
        bool_chmod = 0;
        bool_edit = 0;
        bool_file_rename = 0;

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

void user_starter()
{
    int i;
    for(i = 0; i<20; i++)
    {
        UID[i] = 0;
    }
    for(i = 0; i<10; i++)
    {
        GUID[i] = 0;
    }
}

int main()
{
    /*tamanoArreglo = 0;
    int y = 5;
    y = y * 9;
    printf("%d", y);*/
    srand(time(NULL));
    escrituraPrueba();
    estado = 0;
    printf("Universidad de San Carlos de Guatemala\n");
    printf("Ingenieria en Ciencias y Sistemas\n");
    printf("Proyecto unico: Manejo e Implementacion de Archivos\n");
    printf("                        [File System ext2/ext3]\n");
    /*---------------------------------------------------------------*/
    int exit = 0;
    int crash_b = 0;
    while(exit == 0)
    {
        char entradaUsuario[256];
        printf("\n Fase2 ->     ");
        fgets(entradaUsuario, sizeof(entradaUsuario), stdin);
        int puntos = contadorPuntos(entradaUsuario);/*regresa la cant de :'s/2*/
        int tamanoarray = tamano2(entradaUsuario);
        char entrada[sizeof(entradaUsuario)-puntos];
        if(crash_b == 0)
        {
            partition_Starter();
            user_starter();
            crash_b = 1;
        }
        /*Habiendo definido un nuevo arreglo para la entrada, a la que se le
        convertira sustituyendo cada par de dos puntos por uno solo*/
        /*-------------------------------------------------------------------*/
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
        /**********************************************************************/
        /*Fin reescritura de la entrada*/
        /**********************************************************************/
        /*Sustituyendo los espacios dentro de las comillas por otros simbolos*/
        int bool_comilla = 0;
        int i;
        for(i = 0; i< contador; i++)
        {
            if(entrada[i] == '"' && bool_comilla == 0)
            {
                bool_comilla = 1;
            }else if (entrada[i] == '"' && bool_comilla == 1)
            {
                bool_comilla = 0;
            }
            if(entrada[i] == ' ' && bool_comilla == 1)
            {
                entrada[i] = '?';
            }
        }
        /**********************************************************************/
        /*Iniciando el pseudoautomata y el analisis realizado por el mismo*/
        /**********************************************************************/
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
        bool_cont = 0;
        bool_mkfile = 0;
        bool_mkdir = 0;
        bool_cat = 0;
        bool_login = 0;
        bool_mkgrp = 0;
        bool_mkusr = 0;
        bool_rmgrp = 0;
        bool_rmusr = 0;
        bool_mkfs = 0;
        total_armado = "";
        total_recuperado = "";
        bool_loss = 0;
        bool_recovery = 0;
        bool_edit = 0;
        bool_ls_i =0;
        bool_plus_r = 0;
        bool_chmod = 0;
        bool_edit = 0;
        bool_file_rename = 0;

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

        val_type[0] = "p";
        val_unit[0] = "m";
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
