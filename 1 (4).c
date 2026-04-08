//============================================================================
// Zadanie 5_01
// 
//  Stosy i Kolejki
//
//  WCY23KY3S1   Dmytro Stefko
//============================================================================
#include <stdio.h>
#include <stdlib.h>

// Struktura dla stosu
typedef struct Stos {
    int* array;
    int top;
    int pojemnosc;
} Stos;

// Funkcja do tworzenia stosu
Stos* stworzStos(int pojemnosc) {
    Stos* stos = (Stos*)malloc(sizeof(Stos));
    stos->pojemnosc = pojemnosc;
    stos->top = -1;
    stos->array = (int*)malloc(pojemnosc * sizeof(int));
    return stos;
}

// Funkcja do sprawdzenia, czy stos jest pusty
int czyPusty(Stos* Stos) {
    return Stos->top == -1;
}

// Funkcja do sprawdzenia, czy stos jest pelny
int czyPelny(Stos* Stos) {
    return Stos->top == Stos->pojemnosc - 1;
}

// Funkcja do dodawania elementu na stos
void dodaj(Stos* Stos, int element) {
    if (czyPelny(Stos)) {
        printf("Stos jest pelny\n");
        return;
    }
    Stos->array[++Stos->top] = element;
}

// Funkcja do zdejmowania elementu ze stosu
int usun(Stos* Stos) {
    if (czyPusty(Stos)) {
        return -1; // Wartosc specjalna dla pustego stosu
    }
    return Stos->array[Stos->top--];
}

// Funkcja do wypisywania ruchow
void przesun(char from, char to, int disk, FILE* file) {
    printf("Przenies element %d z %c do %c\n", disk, from, to);
    fprintf(file, "Przenies element %d z %c do %c\n", disk, from, to);
}

// Funkcja do wykonania ruchu miedzy dwoma stosami
void przesunMiedzy(Stos* src, Stos* dest, char s, char d, FILE* file) {
    int pole1 = usun(src);
    int pole2 = usun(dest);

    // Decyzja o ruchu na podstawie wartosci ze stosow
    if (pole1 == -1) { // Jesli zrodlo jest puste
        dodaj(src, pole2);
        przesun(d, s, pole2, file);
    } else if (pole2 == -1) { // Jesli cel jest pusty
        dodaj(dest, pole1);
        przesun(s, d, pole1, file);
    } else if (pole1 > pole2) { // Jesli krazek w zrodle jest wiekszy
        dodaj(src, pole1);
        dodaj(src, pole2);
        przesun(d, s, pole2, file);
    } else { // Jesli krazek w celu jest wiekszy
        dodaj(dest, pole2);
        dodaj(dest, pole1);
        przesun(s, d, pole1, file);
    }
}

// Glowna funkcja Hanoi iteracyjnie
void hanoi(int n, FILE* file) {
    Stos* src = stworzStos(n);
    Stos* aux = stworzStos(n);
    Stos* dest = stworzStos(n);

    char s = 'A', d = 'C', a = 'B';

    // Jesli liczba krazkow jest nieparzysta
    if (n % 2 == 0) {
        char temp = d;
        d = a;
        a = temp;
    }

    // Wypelnienie zrodlowego stosu
    for (int i = n; i >= 1; --i) {
        dodaj(src, i);
    }

    int liczbaKrokow = (1 << n) - 1; 

    for (int i = 1; i <= liczbaKrokow; ++i) {
        if (i % 3 == 1) {
            przesunMiedzy(src, dest, s, d, file);
        } else if (i % 3 == 2) {
            przesunMiedzy(src, aux, s, a, file);
        } else if (i % 3 == 0) {
            przesunMiedzy(aux, dest, a, d, file);
        }
    }

    // Zwolnienie pamieci
    free(src->array);
    free(aux->array);
    free(dest->array);
    free(src);
    free(aux);
    free(dest);
}

int main() {
    int n;

    // Wczytywanie liczby krazkow
    do {
        printf("Wpisz ilosc krazkow (do 8): ");
        scanf("%d", &n);
        if (n < 1 || n > 8) {
            printf("Liczba krazkow musi byc nie wieksza niz 8\n");
        }
    } while (n < 1 || n > 8);

    // Otwarcie pliku wyjsciowego
    FILE* file = fopen("wy.txt", "w");
    if (file == NULL) {
        perror("Nie mozna otworzyc pliku wy.txt");
        return 1;
    }

    // Wywolanie iteracyjnego algorytmu Hanoi
    printf("Rozwiazanie dla %d krazkow:\n", n);
    fprintf(file, "Rozwiazanie dla %d krazkow:\n", n);
    hanoi(n, file);

    // Zamkniecie pliku
    fclose(file);

    printf("Wynik zapisano do pliku wy.txt\n");
    return 0;
}