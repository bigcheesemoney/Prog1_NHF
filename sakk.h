#ifndef SAKK_H
#define SAKK_H
#include <stdbool.h>
typedef enum Babu{
    URES,
    V_GYALOG, V_FUTO, V_HUSZAR, V_BASTYA, V_VEZER, V_KIRALY,
    S_GYALOG, S_FUTO, S_HUSZAR, S_BASTYA, S_VEZER, S_KIRALY
} Babu;

typedef struct Tabla{
    Babu mezok[8][8];
}Tabla;

typedef struct Mezo{
    int SOR; int OSZLOP;
    Babu BABU;
} Mezo;

typedef struct Lepes {
    Mezo HONNAN; Mezo HOVA;
}Lepes;

void init_tabla(Tabla *tabla);

bool bastya_lepes(Lepes lepes, Tabla tabla);

bool futo_lepes(Lepes lepes, Tabla tabla);

bool gyalog_lepes(Lepes lepes, Tabla tabla);

bool kiraly_lepes(Lepes lepes);

bool huszar_lepes(Lepes lepes);

bool szabalyose(Lepes lepes, Tabla tabla);

bool sakke(Tabla tabla, Babu kiraly);

bool matte(Tabla tabla, Babu kiraly);

bool patte(Tabla tabla, Babu kiraly);

void promocio(Tabla *tabla, Lepes lepes);

void lepes_kezel(Tabla *tabla, Lepes lepes);
#endif
