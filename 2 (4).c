//============================================================================
// Zadanie 5_02
// 
//  Stosy i Kolejki
//
//  WCY23KY3S1   Dmytro Stefko
//============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 3 // Liczba dostepnych linii telefonicznych

// Struktura klienta
typedef struct Klient {
    char typ;          // P, G, S
    int numer;         // numer klienta
    int czas;          // czas trwania rozmowy w sekundach
    struct Klient* next; // wskaznik na kolejny element listy
} Klient;

// Struktura dla kolejki priorytetowej
typedef struct priorytowaKolejka {
    Klient* head; // wskaznik na poczatek listy
} priorytowaKolejka;

// Funkcja do inicjalizacji kolejki
void inicjujKolejke(priorytowaKolejka* queue) {
    queue->head = NULL;
}

// Funkcja do okreslenia priorytetu klienta
int priorytet(char t) {
    if (t == 'P') return 1; // Platinum - najwyzszy priorytet
    if (t == 'G') return 2; // Gold - sredni priorytet
    return 3;               // Silver - najnizszy priorytet
}

// Funkcja do dodawania klienta do kolejki (priorytetowej)
void dodajDoPrior(priorytowaKolejka* queue, char typ, int numer, int czas) {
    // Tworzenie nowego klienta
    Klient* newKlient = (Klient*)malloc(sizeof(Klient));
    if (!newKlient) {
        printf("Blad alokacji pamieci\n");
        return;
    }
    newKlient->typ = typ;
    newKlient->numer = numer;
    newKlient->czas = czas;
    newKlient->next = NULL;

    // Wstawianie klienta w odpowiednie miejsce wedlug priorytetu
    if (!queue->head || priorytet(typ) < priorytet(queue->head->typ)) {
        // Nowy klient ma wyzszy priorytet lub lista jest pusta
        newKlient->next = queue->head;
        queue->head = newKlient;
    } else {
        // Szukanie odpowiedniego miejsca w liscie
        Klient* current = queue->head;
        while (current->next && priorytet(typ) >= priorytet(current->next->typ)) {
            current = current->next;
        }
        newKlient->next = current->next;
        current->next = newKlient;
    }
}

// Funkcja do usuwania klienta z kolejki
Klient* usun(priorytowaKolejka* queue) {
    if (!queue->head) {
        printf("Kolejka jest pusta\n");
        return NULL;
    }
    Klient* usunietyKlient = queue->head;
    queue->head = queue->head->next;
    return usunietyKlient;
}

// Funkcja do symulacji obslugi klientow
void symulacja(priorytowaKolejka* queue, FILE* output) {
    int lines[MAX_LINES] = {0}; 
    int aktualnyCzas = 0; // Aktualny czas symulacji

    while (queue->head || lines[0] > 0 || lines[1] > 0 || lines[2] > 0) {
        // Sprawdzenie dostepnych linii
        for (int i = 0; i < MAX_LINES; ++i) {
            if (lines[i] == 0 && queue->head) {
                // Przydzielenie klienta do wolnej linii
                Klient* Klient = usun(queue);
                lines[i] = Klient->czas;

                // Zapisanie informacji do pliku
                fprintf(output, "Klient %c %d uzyskal polaczenie na linii %d\n", Klient->typ, Klient->numer, i + 1);
                printf("Klient %c %d uzyskal polaczenie na linii %d\n", Klient->typ, Klient->numer, i + 1);

                // Zwolnienie pamieci dla obsluzonego klienta
                free(Klient);
            }
        }

        // Zmniejszenie czasu trwania polaczen 
        for (int i = 0; i < MAX_LINES; ++i) {
            if (lines[i] > 0) {
                lines[i]--;
            }
        }

        aktualnyCzas++;
    }
}

// Funkcja do wyswietlania zawartosci pliku na ekranie
void wypisz(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Nie mozna otworzyc pliku %s\n", filename);
        return;
    }

    printf("\nZawartosc pliku %s:\n", filename);

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

int main() {
    // Otwieranie pliku wejsciowego
    FILE* input = fopen("we.txt", "r");
    if (input == NULL) {
        perror("Nie mozna otworzyc pliku we.txt");
        return 1;
    }

    // Otwieranie pliku wyjsciowego
    FILE* output = fopen("wy.txt", "w");
    if (output == NULL) {
        perror("Nie mozna otworzyc pliku wy.txt");
        fclose(input);
        return 1;
    }

    priorytowaKolejka queue;
    inicjujKolejke(&queue);

    // Wczytanie danych z pliku wejsciowego
    char typ;
    int numer, czas;
    while (fscanf(input, " %c %d %d", &typ, &numer, &czas) == 3) {
        dodajDoPrior(&queue, typ, numer, czas);
    }

    fclose(input);

    // Wyswietlenie zawartosci pliku wejsciowego
    wypisz("we.txt");

    // Symulacja polaczen
    printf("\nWynik: \n");
    symulacja(&queue, output);

    fclose(output);

    // Wyswietlenie zawartosci pliku wyjsciowego
    wypisz("wy.txt");

    return 0;
}


