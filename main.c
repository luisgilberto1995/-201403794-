#include <stdio.h>
#include <stdlib.h>

int main()
{
    int c;
    int conteo;
    char entrada[4];
    c=getchar();
    conteo = 0;
    //printf(c);
    while((conteo < 4) && (c != EOF)){
        entrada[conteo] =c;
        ++conteo;
        c = getchar();
    }
    printf(entrada);
    return 0;
}
