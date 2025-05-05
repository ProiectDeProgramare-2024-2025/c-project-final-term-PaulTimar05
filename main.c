#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUME 100
#define MAX_SALI 100
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

typedef struct {
    int id;
    char nume[MAX_NUME];
    int capacitate;
    int rezervata;
} Sala;

Sala sali[MAX_SALI];
int numarSali = 0;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void asteaptaEnter() {
    printf("\nApasa Enter ca sa continui!");
    while (getchar() != '\n');
}

void afiseazaMeniu() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== MENIU PRINCIPAL ===\n" COLOR_RESET);
    printf(COLOR_YELLOW "1. " COLOR_RESET "Adauga sala\n");
    printf(COLOR_YELLOW "2. " COLOR_RESET "Sterge sala\n");
    printf(COLOR_YELLOW "3. " COLOR_RESET "Afiseaza sali\n");
    printf(COLOR_YELLOW "4. " COLOR_RESET "Cauta sala\n");
    printf(COLOR_YELLOW "5. " COLOR_RESET "Rezerva sala\n");
    printf(COLOR_YELLOW "6. " COLOR_RESET "Anuleaza rezervare\n");
    printf(COLOR_YELLOW "0. " COLOR_RESET "Iesire\n");
}

void incarcaSaliDinFisier() {
    FILE *f = fopen("sali.txt", "r");
    if (!f) return;

    numarSali = 0;
    while (fscanf(f, "%d,%99[^,],%d,%d\n", &sali[numarSali].id, sali[numarSali].nume,
                  &sali[numarSali].capacitate, &sali[numarSali].rezervata) == 4) {
        numarSali++;
    }

    fclose(f);
}

void salveazaSaliInFisier() {
    FILE *f = fopen("sali.txt", "w");
    if (!f) {
        printf(COLOR_RED "Eroare la deschiderea fisierului pentru scriere.\n" COLOR_RESET);
        return;
    }

    for (int i = 0; i < numarSali; i++) {
        fprintf(f, "%d,%s,%d,%d\n", sali[i].id, sali[i].nume, sali[i].capacitate, sali[i].rezervata);
    }

    fclose(f);
}

void adaugaSala() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== Submeniu: Adaugare Sala ===\n" COLOR_RESET);

    if (numarSali >= MAX_SALI) {
        printf(COLOR_RED "Numar maxim de sali atins!\n" COLOR_RESET);
        asteaptaEnter();
        return;
    }

    Sala s;
    s.id = (numarSali > 0) ? sali[numarSali - 1].id + 1 : 1;
    s.rezervata = 0;

    do {
        printf("Nume sala: " COLOR_BOLD COLOR_CYAN "(ex: Sala Mare) " COLOR_RESET);
        fgets(s.nume, MAX_NUME, stdin);
        s.nume[strcspn(s.nume, "\n")] = '\0';
        if (strlen(s.nume) == 0) {
            printf(COLOR_RED "Numele nu poate fi gol. Reincercati.\n" COLOR_RESET);
        }
    } while (strlen(s.nume) == 0);

    do {
        printf("Capacitate: " COLOR_BOLD COLOR_CYAN "(ex: 100) " COLOR_RESET);
        if (scanf("%d", &s.capacitate) != 1 || s.capacitate <= 0) {
            printf(COLOR_RED "Capacitate invalida. Introduceti un numar pozitiv.\n" COLOR_RESET);
            while (getchar() != '\n');
        } else {
            while (getchar() != '\n');
            break;
        }
    } while (1);

    sali[numarSali++] = s;
    salveazaSaliInFisier();

    printf(COLOR_GREEN "Sala adaugata cu succes!\n" COLOR_RESET);
    asteaptaEnter();
}

void afiseazaSali() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== Submeniu: Lista Salilor ===\n" COLOR_RESET);

    if (numarSali == 0) {
        printf(COLOR_YELLOW "Nu exista sali inregistrate.\n" COLOR_RESET);
    } else {
        for (int i = 0; i < numarSali; i++) {
            printf(COLOR_GREEN "ID: %d | Nume: %s | Capacitate: %d | Status: %s\n" COLOR_RESET,
                   sali[i].id, sali[i].nume, sali[i].capacitate,
                   sali[i].rezervata ? "REZERVATA" : "LIBERA");
        }
    }

    asteaptaEnter();
}

void stergeSala() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== Submeniu: Stergere Sala ===\n" COLOR_RESET);

    int id;
    printf("Introduceti ID-ul salii de sters: " COLOR_BOLD COLOR_CYAN "(ex: 3) " COLOR_RESET);
    if (scanf("%d", &id) != 1) {
        printf(COLOR_RED "ID invalid!\n" COLOR_RESET);
        while (getchar() != '\n');
        asteaptaEnter();
        return;
    }
    while (getchar() != '\n');

    int gasit = 0;
    for (int i = 0; i < numarSali; i++) {
        if (sali[i].id == id) {
            gasit = 1;
            for (int j = i; j < numarSali - 1; j++) {
                sali[j] = sali[j + 1];
            }
            numarSali--;
            salveazaSaliInFisier();
            printf(COLOR_GREEN "Sala stearsa cu succes.\n" COLOR_RESET);
            break;
        }
    }

    if (!gasit) {
        printf(COLOR_RED "Sala cu ID-ul %d nu a fost gasita.\n" COLOR_RESET, id);
    }

    asteaptaEnter();
}

void cautaSala() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== Submeniu: Cautare Sala ===\n" COLOR_RESET);

    char numeCautat[MAX_NUME];
    printf("Introduceti numele sau parte din nume: " COLOR_BOLD COLOR_CYAN "(ex: Sala M) " COLOR_RESET);
    fgets(numeCautat, MAX_NUME, stdin);
    numeCautat[strcspn(numeCautat, "\n")] = '\0';

    int gasit = 0;
    for (int i = 0; i < numarSali; i++) {
        if (strstr(sali[i].nume, numeCautat) != NULL) {
            printf(COLOR_GREEN "ID: %d | Nume: %s | Capacitate: %d | Status: %s\n" COLOR_RESET,
                   sali[i].id, sali[i].nume, sali[i].capacitate,
                   sali[i].rezervata ? "REZERVATA" : "LIBERA");
            gasit = 1;
        }
    }

    if (!gasit) {
        printf(COLOR_YELLOW "Nu a fost gasita nicio sala cu acest nume.\n" COLOR_RESET);
    }

    asteaptaEnter();
}

void rezervaSala() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== Submeniu: Rezervare Sala ===\n" COLOR_RESET);

    int id;
    printf("Introduceti ID-ul salii de rezervat: " COLOR_BOLD COLOR_CYAN "(ex: 5) " COLOR_RESET);
    if (scanf("%d", &id) != 1) {
        printf(COLOR_RED "ID invalid!\n" COLOR_RESET);
        while (getchar() != '\n');
        asteaptaEnter();
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < numarSali; i++) {
        if (sali[i].id == id) {
            if (sali[i].rezervata) {
                printf(COLOR_YELLOW "Sala este deja rezervata.\n" COLOR_RESET);
            } else {
                sali[i].rezervata = 1;
                salveazaSaliInFisier();
                printf(COLOR_GREEN "Sala a fost rezervata cu succes!\n" COLOR_RESET);
            }
            asteaptaEnter();
            return;
        }
    }

    printf(COLOR_RED "Sala cu ID-ul %d nu a fost gasita.\n" COLOR_RESET, id);
    asteaptaEnter();
}

void anuleazaRezervare() {
    clearScreen();
    printf(COLOR_BOLD COLOR_CYAN "=== Submeniu: Anulare Rezervare ===\n" COLOR_RESET);

    int id;
    printf("Introduceti ID-ul salii pentru care doriti anularea rezervarii: " COLOR_BOLD COLOR_CYAN "(ex: 5) " COLOR_RESET);
    if (scanf("%d", &id) != 1) {
        printf(COLOR_RED "ID invalid!\n" COLOR_RESET);
        while (getchar() != '\n');
        asteaptaEnter();
        return;
    }
    while (getchar() != '\n');

    for (int i = 0; i < numarSali; i++) {
        if (sali[i].id == id) {
            if (!sali[i].rezervata) {
                printf(COLOR_YELLOW "Sala nu este rezervata.\n" COLOR_RESET);
            } else {
                sali[i].rezervata = 0;
                salveazaSaliInFisier();
                printf(COLOR_GREEN "Rezervarea a fost anulata cu succes!\n" COLOR_RESET);
            }
            asteaptaEnter();
            return;
        }
    }

    printf(COLOR_RED "Sala cu ID-ul %d nu a fost gasita.\n" COLOR_RESET, id);
    asteaptaEnter();
}

int main() {
    incarcaSaliDinFisier();

    int optiune;
    do {
        afiseazaMeniu();
        printf("Optiune: ");
        if (scanf("%d", &optiune) != 1) {
            printf(COLOR_RED "Optiune invalida! Introduceti un numar.\n" COLOR_RESET);
            while (getchar() != '\n');
            asteaptaEnter();
            continue;
        }
        while (getchar() != '\n');

        switch(optiune) {
            case 1: adaugaSala(); break;
            case 2: stergeSala(); break;
            case 3: afiseazaSali(); break;
            case 4: cautaSala(); break;
            case 5: rezervaSala(); break;
            case 6: anuleazaRezervare(); break;
            case 0:
                clearScreen();
                printf(COLOR_GREEN "Iesire din aplicatie.\n" COLOR_RESET);
                break;
            default:
                printf(COLOR_RED "Optiune invalida! Va rugam sa alegeti o optiune valida.\n" COLOR_RESET);
                asteaptaEnter();
        }
    } while (optiune != 0);

    return 0;
}
