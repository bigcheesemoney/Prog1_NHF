#include "debugmalloc.h"
#include "sakk.h"
#include <stdbool.h>
#include <stdlib.h>

void init_tabla(Tabla *tabla)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tabla->mezok[i][j] = URES;
        }
    }

    for (int k = 0; k < 8; k++) {
        tabla->mezok[1][k] = S_GYALOG;
        tabla->mezok[6][k] = V_GYALOG;
    }

    Babu vilagos_tisztek[8] = {V_BASTYA, V_HUSZAR, V_FUTO, V_VEZER, V_KIRALY, V_FUTO, V_HUSZAR, V_BASTYA};
    Babu sotet_tisztek[8]   = {S_BASTYA, S_HUSZAR, S_FUTO, S_VEZER, S_KIRALY, S_FUTO, S_HUSZAR, S_BASTYA};

    for (int l = 0; l < 8; l++) {
        tabla->mezok[0][l] = sotet_tisztek[l];
        tabla->mezok[7][l] = vilagos_tisztek[l];
    }
}

bool bastya_lepes(Lepes lepes, Tabla tabla)
{
    int tav_y = abs(lepes.HOVA.SOR - lepes.HONNAN.SOR);
    int tav_x = abs(lepes.HOVA.OSZLOP - lepes.HONNAN.OSZLOP);

    if (lepes.HOVA.SOR == lepes.HONNAN.SOR){
        int irany = (lepes.HOVA.OSZLOP > lepes.HONNAN.OSZLOP) ? 1 : -1;
        for (int i = 1; i < tav_x; i++){
            if (tabla.mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP + i * irany] != 0){
                return false;
            }
        }
        return true;
    }
    else if(lepes.HOVA.OSZLOP == lepes.HONNAN.OSZLOP){
        int irany = (lepes.HOVA.SOR > lepes.HONNAN.SOR) ? 1 : -1;
        for (int i = 1; i < tav_y; i++){
            if (tabla.mezok[lepes.HONNAN.SOR + i * irany][lepes.HONNAN.OSZLOP] != 0){
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

bool huszar_lepes(Lepes lepes)
{
    int tav_y = abs(lepes.HOVA.SOR - lepes.HONNAN.SOR);
    int tav_x = abs(lepes.HOVA.OSZLOP - lepes.HONNAN.OSZLOP);

    if (tav_x == 2 && tav_y == 1){
        return true;
    }
    else if (tav_x == 1 && tav_y == 2){
        return true;
    }
    else{
        return false;
    }
}

bool kiraly_lepes(Lepes lepes)
{
    int tav_y = abs(lepes.HOVA.SOR - lepes.HONNAN.SOR);
    int tav_x = abs(lepes.HOVA.OSZLOP - lepes.HONNAN.OSZLOP);

    if (tav_x < 2 && tav_y < 2){
        return true;
    }
    else{
        return false;
    }
}

bool futo_lepes(Lepes lepes, Tabla tabla)
{
    int tav_y = abs(lepes.HOVA.SOR - lepes.HONNAN.SOR);
    int tav_x = abs(lepes.HOVA.OSZLOP - lepes.HONNAN.OSZLOP);

    int irany_y = (lepes.HOVA.SOR > lepes.HONNAN.SOR) ? 1 : -1;
    int irany_x = (lepes.HOVA.OSZLOP > lepes.HONNAN.OSZLOP) ? 1 : -1;

    if (tav_x == tav_y){
        for (int i = 1; i < tav_y; i++){
            if (tabla.mezok[lepes.HONNAN.SOR + i * irany_y][lepes.HONNAN.OSZLOP + i *irany_x] != 0){
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

bool gyalog_lepes(Lepes lepes, Tabla tabla)
{
    int tav_y = abs(lepes.HOVA.SOR - lepes.HONNAN.SOR);
    int tav_x = abs(lepes.HOVA.OSZLOP - lepes.HONNAN.OSZLOP);
    int irany_y = (lepes.HONNAN.SOR > lepes.HOVA.SOR) ? -1 : 1;

    if (lepes.HONNAN.BABU == V_GYALOG){
        if(irany_y == -1){
            if (lepes.HONNAN.SOR == 6 && tav_y == 2 && tav_x == 0){
                for (int i = 1; i < 3; i++){
                    if (tabla.mezok[lepes.HONNAN.SOR - i][lepes.HONNAN.OSZLOP] != URES){
                        return false;
                    }
                return true;
                }
            }
            else if (tav_y == 1 && tav_x == 0 && lepes.HOVA.BABU == URES){
                return true;
            }
            else if (tav_y == tav_x && tav_x == 1 && lepes.HOVA.BABU != URES){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else if(lepes.HONNAN.BABU == S_GYALOG){
        if(irany_y == 1){
            if (lepes.HONNAN.SOR == 1 && tav_y == 2 && tav_x == 0){
                for (int i = 1; i < 3; i++){
                    if (tabla.mezok[lepes.HONNAN.SOR + i][lepes.HONNAN.OSZLOP] != URES){
                        return false;
                    }
                return true;
                }
            }
            else if (tav_y == 1 && tav_x == 0 && lepes.HOVA.BABU == URES){
                return true;
            }
            else if (tav_y == tav_x && tav_x == 1 && lepes.HOVA.BABU != URES){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
}

bool szabalyose(Lepes lepes, Tabla tabla)
{
    if (lepes.HONNAN.SOR == lepes.HOVA.SOR && lepes.HONNAN.OSZLOP == lepes.HOVA.OSZLOP){
        return false;
    }
    else if ((lepes.HONNAN.BABU>URES && lepes.HONNAN.BABU<S_GYALOG) && (lepes.HOVA.BABU>URES && lepes.HOVA.BABU<S_GYALOG)){
        return false;
    }
    else if ((lepes.HONNAN.BABU>V_KIRALY && lepes.HONNAN.BABU<=S_KIRALY) && (lepes.HOVA.BABU>V_KIRALY && lepes.HOVA.BABU<=S_KIRALY)){
        return false;
    }

    switch(lepes.HONNAN.BABU){
        case(V_BASTYA):
        case(S_BASTYA):
            return bastya_lepes(lepes, tabla);
            break;
        case(V_FUTO):
        case(S_FUTO):
            return futo_lepes(lepes, tabla);
            break;
        case(V_VEZER):
        case(S_VEZER):
            return (bastya_lepes(lepes, tabla) || futo_lepes(lepes, tabla));
            break;
        case(V_HUSZAR):
        case(S_HUSZAR):
            return huszar_lepes(lepes);
            break;
        case(V_GYALOG):
        case(S_GYALOG):
            return gyalog_lepes(lepes, tabla);
            break;
        case(V_KIRALY):
        case(S_KIRALY):
            return kiraly_lepes(lepes);
            break;
        case(URES):
            return false;
    }
}

bool sakke(Tabla tabla, Babu kiraly)
{
    Lepes ellenorzo = {0};

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (tabla.mezok[i][j] == kiraly){
                ellenorzo.HOVA.SOR = i;
                ellenorzo.HOVA.OSZLOP = j;
                ellenorzo.HOVA.BABU = kiraly;
            }
        }
    }

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            ellenorzo.HONNAN.SOR = i;
            ellenorzo.HONNAN.OSZLOP = j;
            ellenorzo.HONNAN.BABU = tabla.mezok[i][j];
            if(szabalyose(ellenorzo, tabla)){
                return true;
            }
        }
    }

    return false;
}

bool matte(Tabla tabla, Babu kiraly)
{
    Lepes ellenorzo = {0};

    if (!sakke(tabla, kiraly)){
        return false;
    }

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (kiraly == V_KIRALY && tabla.mezok[i][j] < S_KIRALY && tabla.mezok[i][j] != URES){
                ellenorzo.HONNAN.SOR = i;
                ellenorzo.HONNAN.OSZLOP = j;
                ellenorzo.HONNAN.BABU = tabla.mezok[i][j];

                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        ellenorzo.HOVA.SOR = k;
                        ellenorzo.HOVA.OSZLOP = l;
                        ellenorzo.HOVA.BABU = tabla.mezok[k][l];
                        Tabla kov_allapot = tabla;
                        lepes_kezel(&kov_allapot, ellenorzo);
                        if (szabalyose(ellenorzo, tabla) && !sakke(kov_allapot, kiraly)){
                            return false;
                        }
                    }
                }
            }
            else if (kiraly == S_KIRALY && tabla.mezok[i][j] > V_KIRALY){
                ellenorzo.HONNAN.SOR = i;
                ellenorzo.HONNAN.OSZLOP = j;
                ellenorzo.HONNAN.BABU = tabla.mezok[i][j];

                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        ellenorzo.HOVA.SOR = k;
                        ellenorzo.HOVA.OSZLOP = l;
                        ellenorzo.HOVA.BABU = tabla.mezok[k][l];
                        Tabla kov_allapot = tabla;
                        lepes_kezel(&kov_allapot, ellenorzo);
                        if (szabalyose(ellenorzo, tabla) && !sakke(kov_allapot, kiraly)){
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool patte(Tabla tabla, Babu kiraly)
{
    Lepes ellenorzo = {0};

    if (sakke(tabla, kiraly)){
        return false;
    }

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (kiraly == V_KIRALY && tabla.mezok[i][j] < S_KIRALY && tabla.mezok[i][j] != URES){
                ellenorzo.HONNAN.SOR = i;
                ellenorzo.HONNAN.OSZLOP = j;
                ellenorzo.HONNAN.BABU = tabla.mezok[i][j];

                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        ellenorzo.HOVA.SOR = k;
                        ellenorzo.HOVA.OSZLOP = l;
                        ellenorzo.HOVA.BABU = tabla.mezok[k][l];
                        Tabla kov_allapot = tabla;
                        lepes_kezel(&kov_allapot, ellenorzo);
                        if (szabalyose(ellenorzo, tabla) && !sakke(kov_allapot, kiraly)){
                            return false;
                        }
                    }
                }
            }
            else if (kiraly == S_KIRALY && tabla.mezok[i][j] > V_KIRALY){
                ellenorzo.HONNAN.SOR = i;
                ellenorzo.HONNAN.OSZLOP = j;
                ellenorzo.HONNAN.BABU = tabla.mezok[i][j];

                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        ellenorzo.HOVA.SOR = k;
                        ellenorzo.HOVA.OSZLOP = l;
                        ellenorzo.HOVA.BABU = tabla.mezok[k][l];
                        Tabla kov_allapot = tabla;
                        lepes_kezel(&kov_allapot, ellenorzo);
                        if (szabalyose(ellenorzo, tabla) && !sakke(kov_allapot, kiraly)){
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void promocio(Tabla *tabla, Lepes lepes)
{
    int uj = 0;
    if (lepes.HONNAN.BABU == V_GYALOG && lepes.HOVA.SOR == 0){
        printf("Gyalogpromocio!\nAdd meg a kivant babu szamat!\n2: Futo, 3: Huszar, 4: Bastya, 5: Vezer\n");
        scanf("%d", &uj);
        while (uj < 2 || uj > 5){
            scanf("%d", &uj);
        }
        tabla->mezok[lepes.HOVA.SOR][lepes.HOVA.OSZLOP] = uj;
        tabla->mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP] = URES;
    }
    else if (lepes.HONNAN.BABU == S_GYALOG && lepes.HOVA.SOR == 7){
        printf("Gyalogpromocio!\nAdd meg a kivant babu szamat!\n8: Futo, 9: Huszar, 10: Bastya, 11: Vezer\n");
        while (uj < 8 || uj > 11){
            scanf("%d", &uj);
        }
        tabla->mezok[lepes.HOVA.SOR][lepes.HOVA.OSZLOP] = uj;
        tabla->mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP] = URES;
    }
}

void lepes_kezel(Tabla *tabla, Lepes lepes){
    tabla->mezok[lepes.HOVA.SOR][lepes.HOVA.OSZLOP] = tabla->mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP];
    tabla->mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP] = URES;
}
