#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct dictionary_entry
{
    char *word;
    int priority;
};
void citireDictionar(int *n, struct dictionary_entry **dict)
{
    scanf("%d", &*n);
    getchar();
    *dict = realloc(*dict, *n * sizeof(struct dictionary_entry));
    int i;
    for (i = 0; i < *n; i++)
    {
        char aux[21];
        scanf("%s", aux);
        (*dict)[i].word = (char *)malloc((strlen(aux) + 1) * sizeof(char));
        strcpy((*dict)[i].word, aux);
        (*dict)[i].priority = 0;
    }
}
// functie ce returneaza pozitia unui cuvant in dictionar ce se potriveste cu
// cel introdus
int cautInDict(struct dictionary_entry *dict, int n, char *cuv)
{
    int poz = -1, i, cuvLen = strlen(cuv);
    for (i = 0; i < n; i++)
    {
        // verific daca cuvantul din dictionar il are ca prefix pe cuv
        if (strncmp(dict[i].word, cuv, cuvLen) == 0)
        {
            // daca nu am gasit niciun cuvant pana acum atunci il retin
            // pe acesta
            if (poz < 0)
                poz = i;
            else if (dict[i].priority > dict[poz].priority)
                poz = i;
            else if (dict[i].priority == dict[poz].priority &&
                     strcmp(dict[i].word, dict[poz].word) <= 0)
                poz = i;
        }
    }
    return poz;
}
// functie ce returneaza pozitia unui cuvant in dictionar identic cu cel
// introdus. functia este folosita pentru cuvintele introduse cu * la final
int cautInDictExact(struct dictionary_entry *dict, int n, char *cuv)
{
    int poz = -1, i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(dict[i].word, cuv) == 0)
            poz = i;
    }
    return poz;
}
// functie ce adauga un cuvant la finalul dictionarului si ii retine
// 1 la proritate
void adaugInDict(struct dictionary_entry **dict, int *n, char *cuv)
{
    (*n)++;
    *dict = realloc(*dict, *n * sizeof(struct dictionary_entry));
    (*dict)[*n - 1].word = (char *)malloc((strlen(cuv) + 1) * sizeof(char));
    strcpy((*dict)[*n - 1].word, cuv);
    (*dict)[*n - 1].priority = 1;
}
int main()
{
    int n, m, i;
    struct dictionary_entry *dict = malloc(0);
    citireDictionar(&n, &dict);
    scanf("%d", &m);
    for (i = 0; i < m; i++)
    {
        char cuv[21];
        scanf("%s", cuv);
        // verific daca am citit cuvant sau semn de punctuatie
        if (cuv[0] < 'a')
            printf("%s ", cuv);
        else
        {
            char ultcar = cuv[strlen(cuv) - 1];
            if (ultcar == '*')
            {
                // sterg * de la final
                cuv[strlen(cuv) - 1] = '\0';
                int poz = cautInDictExact(dict, n, cuv);
                if (poz < 0) // daca nu a fost gasit in dictionar
                    adaugInDict(&dict, &n, cuv);
                else
                    dict[poz].priority++;
                printf("%s ", cuv);
            }
            else
            {
                int poz = cautInDict(dict, n, cuv);
                if (poz < 0) // daca nu a fost gasit in dictionar
                {
                    adaugInDict(&dict, &n, cuv);
                    printf("%s ", cuv);
                }
                else
                {
                    dict[poz].priority++;
                    printf("%s ", dict[poz].word);
                }
            }
        }
    }
    for (i = 0; i < n; i++)
        free(dict[i].word);
    free(dict);
    return 0;
}