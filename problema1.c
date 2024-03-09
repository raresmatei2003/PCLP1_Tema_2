#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
// retine linia, pozitia de inceput si numarul de spatii al fiecarei
// grupari de spatii sterse din text
typedef struct
{
    int linie, poz, nr;
} spatii;
char **citire(int *n)
{
    int i;
    scanf("%d", &*n);
    getchar();
    char **s = malloc(*n * sizeof(s));
    for (i = 0; i < *n; i++)
    {
        char linie[101];
        fgets(linie, 102, stdin);
        s[i] = (char *)malloc((strlen(linie) + 1) * sizeof(char));
        strcpy(s[i], linie);
    }
    return s;
}
// creez un vector de siruri de caractere care are randuri pentru subliniere
char **transformText(char **s, int n)
{
    char **shl = malloc(2 * n * sizeof(shl));
    int i;
    for (i = 0; i < n; i++)
    {
        // linia cu text
        shl[2 * i] = (char *)malloc((strlen(s[i]) + 1) * sizeof(char));
        strcpy(shl[2 * i], s[i]);
        // linia cu sublinierea
        shl[2 * i + 1] = (char *)malloc((strlen(s[i]) + 1) * sizeof(char));
        memset(shl[2 * i + 1], ' ', strlen(s[i]) - 1);
        strcat(shl[2 * i + 1], "\n");
    }
    return shl;
}
// functie ce retine linia, pozitia de inceput si lungimea subsirurilor
// alcatuite din cel putin 2 spatii din text, apoi le sterge
void elimSpatii(char **shl, int n, spatii *sp, int *m)
{
    int i;
    for (i = 0; i < 2 * n; i += 2)
    {
        int j;
        for (j = 0; j < strlen(shl[i]); j++)
        {
            if (shl[i][j] == ' ')
            {
                int k = j + 1;
                // merg cat timp am spatii si nu am ajuns la finalul liniei
                while (shl[i][k] == ' ' && k < strlen(shl[i]))
                    k++;
                // daca am mai mult de un spatiu
                if (k > j + 1)
                {
                    int l;
                    (*m)++;
                    sp = (spatii *)realloc(sp, *m * sizeof(spatii));
                    sp[*m - 1].linie = i; // retin linia pe care sunt spatiile
                    sp[*m - 1].poz = j + 1; // retin pozitia de la care incep
                    sp[*m - 1].nr = k - j - 1; // retin numarul spatiilor
                    // elimin spatiile
                    for (l = j + 1; l < strlen(shl[i]) - sp[*m - 1].nr; l++)
                    {
                        shl[i][l] = shl[i][l + sp[*m - 1].nr];
                        shl[i + 1][l] = shl[i + 1][l + sp[*m - 1].nr];
                    }
                    shl[i][l] = '\0';
                    shl[i + 1][l] = '\0';
                }
            }
        }
    }
}
void aplichl(char *linie, int poz, int len);
// functie ce cauta o secventa cheie in text
void cautInText(char **shl, int n, char cuv[10])
{
    int i;
    for (i = 0; i < 2 * n; i += 2)
    {
        // caut secventa cheie in linia i din text
        char *s = strstr(shl[i], cuv);
        while (s != NULL)
        {
            int poz = s - shl[i];
            // verific daca subsirul gasit este identic cu secventa cheie
            // (daca se afla intre spatii sau daca se afla la inceput sau
            // sfarsit de linie)
            if ((poz == 0 && shl[i][poz + strlen(cuv)] == ' ') ||
                (strlen(cuv) + 1 == strlen(shl[i])) ||
                (shl[i][poz - 1] == ' ' && poz + strlen(cuv) + 1 ==
                                               strlen(shl[i])) ||
                (shl[i][poz - 1] == ' ' && shl[i][poz + strlen(cuv)] == ' '))
            {
                // daca am gasit secventa cheie atunci aplic highlight-ul
                aplichl(shl[i + 1], poz, strlen(cuv));
            }
            // caut urmatoarea aparitie a secventei cheie pe linia i
            s = strstr(s + strlen(cuv), cuv);
        }
    }
}
// functie ce aplica highlight-ul pentru un subsir de lungime len care incepe
// de pe pozitia poz pe linia linie
void aplichl(char *linie, int poz, int len)
{
    int i;
    for (i = poz; i < poz + len; i++)
        linie[i] = '_';
}
// functie ce adauga spatiile sterse la inceput din textul cu sublinierile.
// in cazul in care aceste spatii fac parte dintr-o secventa cheie,
// pe randul cu highlight-ul este completat cu '_'
void adaugSpatii(char **shl, int n, spatii sp)
{
    // retin cu ce caracter trebuie completat pe randul cu sublinierea
    char c = shl[sp.linie + 1][sp.poz - 1];
    // siruri auxiliare in care construiesc textul completat
    char *aux = (char *)malloc(sp.nr + (strlen(shl[sp.linie]) - sp.poz));
    char *aux2 = (char *)malloc(sp.nr + (strlen(shl[sp.linie + 1]) - sp.poz));

    // initializes sirurile auxiliare cu ce caracter trebuie completat textul
    memset(aux, ' ', sp.nr);
    memset(aux2, c, sp.nr);

    // adaug la final restul textului ce se afla dupa caracterele introduse
    strcat(aux, shl[sp.linie] + sp.poz);
    strcat(aux2, shl[sp.linie + 1] + sp.poz);

    // copiez in text sirul completat
    strcpy(shl[sp.linie] + sp.poz, aux);
    strcpy(shl[sp.linie + 1] + sp.poz, aux2);
}
int main()
{
    char cuv[15][10] = {"first of", "for", "for each", "from", "in", "is",
                        "is a", "list of", "unit", "or", "while", "int",
                        "float", "double", "string"};
    int n, i;
    char **text = citire(&n);               // text initial
    char **texthl = transformText(text, n); // text cu sublinieri

    int m = 0; // numarul de grupari de spatii sterse sin text
    spatii *sp = malloc(0);
    elimSpatii(texthl, n, sp, &m);

    // caut in text fiecare secventa cheie
    for (i = 0; i < 15; i++)
        cautInText(texthl, n, cuv[i]);
    // parcurg structura in care am retinut gruparile de spatii sterse
    // si le adaug in textul cu highlight
    for (i = m - 1; i >= 0; i--)
        adaugSpatii(texthl, n, sp[i]);

    for (i = 0; i < 2 * n; i++)
    {
        printf("%s", texthl[i]);
        free(texthl[i]);
    }
    for (i = 0; i < n; i++)
        free(text[i]);
    free(text);
    free(texthl);
    return 0;
}