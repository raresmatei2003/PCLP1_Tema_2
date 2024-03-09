#include <stdio.h>
#include <stdlib.h>
// sterge bitii din m de dupa primii nr biti
// (retine cei mai putini semnificativi nr biti din m)
void stergBiti(unsigned int *m, int nr)
{
    int i = (1 << nr) - 1;
    *m = (*m) & i;
}
int xorr(int a, int b)
{
    int i = 1;
    int rez = 0;
    while (i < 9)
    {
        // daca a si b au bitul de pe pozitia log2(i) diferit, atunci acesta
        // este marcat in rezultat
        if ((a & i) != (b & i))
            rez = rez | i;
        i = i << 1;
    }
    return rez;
}
int adunare(int a, int b)
{
    int rez = 0;
    int i = 1;
    int tr = 0;
    while (i < 9)
    {
        int ai = a & i; // retin bitul de pe pozitia log2(i) din a
        int bi = b & i; // retin bitul de pe pozitia log2(i) din b
        // daca ambele numere au bitul de pe poz log2(i) nenul, atunci adaugam
        // transportul si il actualizam pentru urmatorul bit
        if (ai && bi)
        {
            rez = rez | tr;
            tr = i << 1;
        }
        else
        {
            // daca suma bitilor din ai si bi este 1 si transportul este
            // 1 atunci suma pentru acest bit va da 0, fiind astfel necesar
            // doar sa actualizam transportul pentru urmatorul bit
            if ((ai | bi) && tr)
                tr = tr << 1;
            else
            {
                // calculam suma pentru bitul de pe poz log2(i) si resetam
                // transportul
                rez = rez | ((ai | bi) | tr);
                tr = 0;
            }
        }
        i = i << 1;
    }
    return rez;
}
void interschimbareBit(int *a, int bit, int poz)
{
    if (bit == 0) // daca trebuie schimbat bitul in 0
    {
        // ~(1<<poz) = conjugatul nr format din bitul 1 pe pozitia poz
        *a = *a & (~(1 << poz));
    }
    else
    {
        // daca trebuie schimbat bitul in 1
        *a = *a | bit;
    }
}
int interschimbare(int a, int b)
{
    // b >> 2 rezulta primii 2 biti din b
    // scadem din 3 pentru a da pozitia de la stanga
    int p1 = 3 - (b >> 2);

    // 3 are reprezentarea binara 0011, astfel b & 3 returneaza
    // doar ultimii 2 biti din b (cei mai putin semnificativi)
    // scadem din 3 pentru a da pozitia de la stanga
    int p2 = 3 - (b & 3);

    // retin bitul de pe poz p1 si il mut pe poz p2
    int bit1 = ((1 << p1) & a) >> p1 << p2;
    // retin bitul de pe poz p2 si il mut pe poz p1
    int bit2 = ((1 << p2) & a) >> p2 << p1;
    // schimb bitul de pe pozitia p2 cu bitul bit1
    interschimbareBit(&a, bit1, p2);
    // schimb bitul de pe pozitia p1 cu bitul bit2
    interschimbareBit(&a, bit2, p1);
    return a;
}
int rotatiest(int a, int b)
{
    // in cazul in care b este mai mare sau egal cu 4, bitii numarului a
    // pot fi rotiti doar de b%4 = b&3 ori (prin b&3 sunt salvati doar
    // cei mai putini semnificativi 2 biti care retin un numar <= 3)
    int nr = b & 3;

    if (nr == 1) // o singura rotire
    {
        // 0 1 2 3 -> 1 2 3 0
        a = interschimbare(a, 3); // schimb 0 cu 3 (3 = 0011) -> 3 1 2 0
        a = interschimbare(a, 2); // schimb 3 cu 2 (2 = 0010) -> 2 1 3 0
        a = interschimbare(a, 1); // schimb 2 cu 1 (1 = 0001) -> 1 2 3 0
    }
    else if (nr == 2) // 2 rotiri
    {
        // 0 1 2 3 -> 2 3 0 1
        a = interschimbare(a, 2); // schimb 0 cu 2 (2 = 0010) -> 2 1 0 3
        a = interschimbare(a, 7); // schimb 1 cu 3 (7 = 0111) -> 2 3 0 1
    }
    else if (nr == 3) // 3 rotiri
    {
        // 0 1 2 3 -> 3 0 1 2
        a = interschimbare(a, 1); // schimb 0 cu 1 (1 = 0001) -> 1 0 2 3
        a = interschimbare(a, 2); // schimb 1 cu 2 (2 = 0010) -> 2 0 1 3
        a = interschimbare(a, 3); // schimb 2 cu 3 (3 = 0011) -> 3 0 1 2
    }

    return a;
}
void calcul(int *rez, int nr, int (*op)(int a, int b))
{
    *rez = op(*rez, nr);
}
int main()
{
    int (*op[4])(int, int) = {adunare, interschimbare, rotatiest, xorr};
    int n, i;
    unsigned int m;
    scanf("%d%d", &n, &m);
    stergBiti(&m, n * 6 + 4); // sterg bitii dinaintea primului numar
    int rez = m >> (n * 6);   // extrag primul numar
    stergBiti(&m, n * 6);     // sterg bitii numarului extras din m
    for (i = n - 1; i >= 0; i--)
    {
        int operatie = m >> (i * 6 + 4); // extrag operatia
        stergBiti(&m, i * 6 + 4);        // sterg bitii operatiei din m
        int nr = m >> (i * 6);           // extrag urmatorul numar
        stergBiti(&m, i * 6);            // sterg bitii numarului extras din m
        calcul(&rez, nr, op[operatie]);
    }
    printf("%d", rez);
    return 0;
}