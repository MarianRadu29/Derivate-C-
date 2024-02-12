#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
char expresie[500], EXPRESIE[500][500] = { "" }, postfix[500][500] = { "" }, derivata[1000];
char litera[53] = "abcdefghijklmnopqrstuvwyzABCDEFGHIJKLMNOPQRSTUVWYZ"/*litera fara x , x este parametrul functiei*/, cifra[] = "0123456789", operatori[] = "+-*/^()";
int nr_cuvinte_expresie = 0, nr_cuvinte_postfixat, i;
bool Operatori(char s[500]);
struct nod
{
    char info[10];
    nod* urm;
};
////////////////////////////////////////definim stiva
struct stiva
{
    nod* varf;
    int nrElemente;
};
void initializeazaStiva(stiva& S)
{
    S.varf = NULL; S.nrElemente = 0;
}
void pop(stiva& S)
// scoate elementul din varful stivei
{
    if (S.nrElemente != 0)
    {

        nod* varf_nou;
        varf_nou = S.varf->urm;
        delete S.varf;
        S.varf = varf_nou;
        S.nrElemente--;

    }
}
void push(stiva& S, char element[50])
// adauga un element in varful stivei
{
    nod* nod_nou;
    if (S.nrElemente == 0)
    {
        S.nrElemente = 1;
        S.varf = new nod;
        strcpy(S.varf->info, element);
        S.varf->urm = NULL;
    }
    else
    {
        S.nrElemente++;
        nod_nou = new nod;
        strcpy(nod_nou->info, element);
        nod_nou->urm = S.varf;
        S.varf = nod_nou;
    }
}
void afiseazaStiva(stiva S)
{
    nod* nod_curent;
    nod_curent = S.varf;
    while (nod_curent != NULL)
    {
        cout << nod_curent->info << ",";
        nod_curent = nod_curent->urm;
    }
    cout << endl;
}
//////////////////////////////////////////////////////////////////////////////////definim coada  
struct coada
{
    nod* prim;
    nod* ultim;
    int nrElemente;
};
coada infixat, postfixat;
bool esteVida(coada C)
{
    return C.nrElemente == 0;
}
void initializeazaCoada(coada& C)
{
    C.prim = NULL; C.ultim = NULL; C.nrElemente = 0;
}
void primulCoada(coada C, char element[50])
// ia informatia din primul nod al cozii, fara a-l elimina
{
    if (!esteVida(C))
        strcpy(element, C.prim->info);

}
void elCoada(coada& C)
{
    if (!esteVida(C))
    {
        nod* primul;
        primul = C.prim;
        C.prim = C.prim->urm;
        delete primul;
        C.nrElemente--;
    }
}
void addCoada(coada& C, char element[50])
{
    nod* nod_nou;
    if (esteVida(C))
    {
        C.nrElemente = 1;
        C.prim = new nod;
        strcpy(C.prim->info, element);
        C.prim->urm = NULL;
        C.ultim = C.prim;
    }
    else
    {
        C.nrElemente++;
        nod_nou = new nod;
        strcpy(nod_nou->info, element);
        nod_nou->urm = NULL;
        C.ultim->urm = nod_nou;
        C.ultim = nod_nou;
    }
}
void afiseazaCoada(coada C)
{
    nod* nod_curent;
    nod_curent = C.prim;
    while (nod_curent != NULL)
    {
        cout << nod_curent->info << ' ';
        nod_curent = nod_curent->urm;
    }
    cout << endl;
}
///////////////////////////////////////////////////////////////////////////////////////determinam postfixata expresiei date
bool Operatori(char s[500])
{
    if (strcmp(s, "+") == 0 || strcmp(s, "-") == 0 || strcmp(s, "*") == 0 || strcmp(s, "/") == 0 || strcmp(s, "^") == 0 || strcmp(s, "(") == 0 || strcmp(s, ")") == 0 ||
        strcmp(s, "sin") == 0 || strcmp(s, "cos") == 0 || strcmp(s, "tg") == 0 || strcmp(s, "ctg") == 0 || strcmp(s, "log") == 0 || strcmp(s, "ln") == 0 ||
        strcmp(s, "arcsin") == 0 || strcmp(s, "arccos") == 0 || strcmp(s, "arctg") == 0 || strcmp(s, "arcctg") == 0)
        return 1;
    return 0;
}
int Prioritate(char x[500])
{
    if (strcmp(x, "+") == 0 || strcmp(x, "-") == 0) return 1;
    if (strcmp(x, "*") == 0 || strcmp(x, "/") == 0) return 2;
    if (strcmp(x, "^") == 0) return 3;
    if (strcmp(x, "sin") == 0 || strcmp(x, "cos") == 0 || strcmp(x, "tg") == 0 || strcmp(x, "ctg") == 0 || strcmp(x, "ln") == 0 || strcmp(x, "log") == 0 ||
        strcmp(x, "arcsin") == 0 || strcmp(x, "arccos") == 0 || strcmp(x, "arctg") == 0 || strcmp(x, "arcctg") == 0) return 4;
    if (strcmp(x, "(") == 0) return 5;
    return 0;
}
void expresie_cuvinte(char expresie[500])
{
    for (i = 0; i < strlen(expresie); i++)
    {
        if (expresie[i] == 'x' || strchr(operatori, expresie[i]) || expresie[i]=='e') //avem parametru sau este operator unar sau paranteza rotunda
        {
            EXPRESIE[nr_cuvinte_expresie][0] = expresie[i];
            nr_cuvinte_expresie++;
        }
       
        if (strchr(cifra, expresie[i]))//avem un numar 
        {
            int nr_cifre = 0;
            EXPRESIE[nr_cuvinte_expresie][nr_cifre++] = expresie[i++];
            while (strchr(cifra, expresie[i]) != 0 && i < strlen(expresie))
                EXPRESIE[nr_cuvinte_expresie][nr_cifre++] = expresie[i++];
            i--;
            nr_cuvinte_expresie++;
        }
        if (strchr(litera, expresie[i]) && expresie[i]!='e')//avem o functie trigonometrica sau log , ln
        {
            int nr_litere = 0;
            while (strchr(litera, expresie[i]))
                EXPRESIE[nr_cuvinte_expresie][nr_litere++] = expresie[i++];
            i--;
            nr_cuvinte_expresie++;
        }
    }
    for (i = 0; i < nr_cuvinte_expresie; i++) //de exemplu pt 2+3*5 ,nr_cuvinte_expresie se incrementeaza prost
        if (strcmp(EXPRESIE[i], "") == 0)
        {
            for (int j = i; j < nr_cuvinte_expresie; j++)
                strcpy(EXPRESIE[j], EXPRESIE[j + 1]);
            nr_cuvinte_expresie--;
        }
    //lipim semnul la un numar,de exemplu vor fii 2 string uri - si 1 dar dupa va fi -1
    i = 0;
    while (i < nr_cuvinte_expresie)
    {
        if (EXPRESIE[i][0] == '-' && strchr(cifra, EXPRESIE[i + 1][0]) && i == 0)
        {
            for (int j = 0; j < nr_cuvinte_expresie - 1; j++)
                strcpy(EXPRESIE[j], EXPRESIE[j + 1]);
            nr_cuvinte_expresie--;
            char aux1[600];
            strcpy(aux1, EXPRESIE[i]);
            strcpy(EXPRESIE[i], "-");
            strcat(EXPRESIE[i], aux1);
        }
        if (EXPRESIE[i][0] == '-' && strchr(cifra, EXPRESIE[i + 1][0]) && EXPRESIE[i - 1][0] == '(')
        {
            for (int j = i; j < nr_cuvinte_expresie - 1; j++)
                strcpy(EXPRESIE[j], EXPRESIE[j + 1]);
            nr_cuvinte_expresie--;
            char aux1[600];
            strcpy(aux1, EXPRESIE[i]);
            strcpy(EXPRESIE[i], "-");
            strcat(EXPRESIE[i], aux1);
        }
        i++;
    }
}
void infixata(char EXPRESIE[500][500], int nr_cuvinte_expresie)
{
    for (i = 0; i < nr_cuvinte_expresie; i++)
        addCoada(infixat, EXPRESIE[i]);
}
void postfixata(char EXPRESIE[500][500], int nr_cuvinte_expresie)
{
    stiva S;
    initializeazaCoada(infixat);
    initializeazaCoada(postfixat);
    infixata(EXPRESIE, nr_cuvinte_expresie);
    initializeazaStiva(S);
    while (!esteVida(infixat))
    {
        char aux[50];
        primulCoada(infixat, aux);
        elCoada(infixat);
        if (Operatori(aux) == 0)//nu este o operatie
            addCoada(postfixat, aux);
        else

            if (strcmp(aux, ")") == 0)
            {
                while (strcmp(S.varf->info, "(") != 0)
                {
                    addCoada(postfixat, S.varf->info);
                    pop(S);
                }
                pop(S);
            }
            else
            {
                while (S.nrElemente != 0 && strcmp(S.varf->info, "(") != 0 && Prioritate(S.varf->info) >= Prioritate(aux))
                {
                    addCoada(postfixat, S.varf->info);
                    pop(S);
                }
                push(S, aux);
            }
    }
    while (S.nrElemente != 0)
    {
        addCoada(postfixat, S.varf->info);
        pop(S);
    }
}
////////////////////////////////////////////////////////////
//definire structuri pentru a creea arborele expresiei
struct arbore
{
    arbore* stanga;
    arbore* dreapta;
    char info[500];
};
typedef arbore* adresa_arbore;
adresa_arbore a;
struct radacina_arbore//un fel de struct nod,ca mai sus
{
    adresa_arbore info;
    radacina_arbore* urm;
};
struct stiva_arbore
{
    radacina_arbore* varf;
    int nrElemente;
};
void push_arbore(stiva_arbore& S, adresa_arbore a)
{
    radacina_arbore* p;
    if (S.nrElemente == 0)
    {
        S.nrElemente = 1;
        S.varf = new radacina_arbore;
        S.varf->info = a;
        S.varf->urm = NULL;
    }
    else
    {
        S.nrElemente++;
        p = new radacina_arbore;
        p->info = a;
        p->urm = S.varf;
        S.varf = p;
    }
}
void pop_arbore(stiva_arbore& S)
{
    if (S.nrElemente != 0)
    {
        radacina_arbore* p;
        p = S.varf->urm;
        delete S.varf;
        S.varf = p;
        S.nrElemente--;
    }
}
void Initializare_arbore(adresa_arbore& a)
{
    a = new arbore;
    a->dreapta = NULL;
    a->stanga = NULL;
}
void determinare_arbore_expresie(adresa_arbore& a, coada postfixat, int nr_cuvinte_postfixat)
{
    stiva_arbore S;
    S.nrElemente = 0;
    S.varf = NULL;
    adresa_arbore b, c, d;
    
    for (int i = 0; i < nr_cuvinte_postfixat; i++)
    {
        char OP[50];
        primulCoada(postfixat, OP);
        elCoada(postfixat);
       
        if (Operatori(OP) == 0)//este operand
        {
            Initializare_arbore(b);
            strcpy(b->info, OP);
            push_arbore(S, b);
        }
        else if (strchr(operatori, OP[0]))//verificam daca este unul dintre acesti operatori: + - * / ^ ( )
        {
            Initializare_arbore(b);
            Initializare_arbore(c);
            if (S.varf != NULL)
            {
                b = S.varf->info;
                pop_arbore(S);
            }
            if (S.varf != NULL)
            {
                c = S.varf->info;
                pop_arbore(S);
            }
            Initializare_arbore(d);
            strcpy(d->info, OP);
            d->dreapta = b;
            d->stanga = c;
            push_arbore(S, d);
        }
        else//este functie trigonometrica sau log
        {
            if (strcmp(OP, "log"))
            {
                Initializare_arbore(c);
                c = S.varf->info;
                pop_arbore(S);
                Initializare_arbore(d);
                d->dreapta = c;//normal ar fii stanga cum scrie in carte doar ca la parcurgerea inordine a arborelui ,rezultatul nu e la fel ca expresia initiala
                strcpy(d->info, OP);
                push_arbore(S, d);
            }
            else
            {
                Initializare_arbore(c);
                c = S.varf->info;
                pop_arbore(S);
                Initializare_arbore(d);
                d = S.varf->info;
                pop_arbore(S);
                adresa_arbore e;
                Initializare_arbore(e);
                strcpy(e->info, OP);
                e->stanga = d;
                e->dreapta = c;
                push_arbore(S, e);
            }
        }
    }
    a = S.varf->info;
}
void parcurgereInInordine(adresa_arbore a)
{
    if (a != NULL)
        if(strcmp(a->info,"log")!=0)
        {
            parcurgereInInordine(a->stanga);
            cout<<a->info<<' ';
            parcurgereInInordine(a->dreapta);
        }
        else
            {
                cout<<a->info<<' ';
                parcurgereInInordine(a->stanga);
                parcurgereInInordine(a->dreapta);
            }
}
void deriv_paranteze(adresa_arbore a)/////!!!!!!!!!!!!!!!!!!!!!!
{
    int sw = 0;
    if (a != NULL)
    {   if(strcmp(a->info,"log")==0)
        {
            strcat(derivata,a->info);
            strcat(derivata,a->stanga->info);
            strcat(derivata,"(");
            deriv_paranteze(a->dreapta);
            strcat(derivata,")");
        }
        else
        {
        if (Operatori(a->info) == 1 && a->stanga != NULL && a->dreapta != NULL)
            if (Prioritate(a->info) >= Prioritate(a->stanga->info) && Operatori(a->stanga->info) == 1)
            {   
                sw = 1;
                strcat(derivata, "(");
            }
        if (strcmp(a->info, "/") == 0 && a->stanga != NULL && a->dreapta != NULL)
            if (strcmp(a->stanga->info, "^") == 0)
            {   
                sw = 1;
                strcat(derivata, "(");
            }
        deriv_paranteze(a->stanga);
        if (sw == 1)
        {   
            sw = 0;
            strcat(derivata, ")");
        }

        strcat(derivata, a->info);

        if (Operatori(a->info) == 1 && a->stanga != NULL && a->dreapta != NULL)
            if (Prioritate(a->info) >= Prioritate(a->dreapta->info) && Operatori(a->dreapta->info) == 1)
            {   
                sw = 1;
                strcat(derivata, "(");
            }
        if (strcmp(a->info, "/") == 0 && a->stanga != NULL && a->dreapta != NULL)
            if (strcmp(a->dreapta->info, "^") == 0)
            {   
                sw = 1;
                strcat(derivata, "(");
            }
        if (strcmp(a->info, "sin") == 0 || strcmp(a->info, "cos") == 0 || strcmp(a->info, "tg") == 0 || strcmp(a->info, "ctg") == 0 || strcmp(a->info, "ln") == 0
            || strcmp(a->info, "arcsin") == 0 || strcmp(a->info, "arccos") == 0 || strcmp(a->info, "arctg") == 0 || strcmp(a->info, "arcctg") == 0)
        {   
            sw = 1;
            strcat(derivata, "(");
        }
        deriv_paranteze(a->dreapta);
        }
    }
    if (sw == 1)  strcat(derivata, ")");
}
/////////////////////////////afisare arbore
int height(adresa_arbore a)//inaltimea arborelui
{
    if (a != NULL)
        return 1 + max(height(a->dreapta), height(a->stanga));
    else
        return 0;
}
int size(adresa_arbore a)//numarul de noduri din arbore
{
    if (a != NULL)
        return 1 + size(a->dreapta) + size(a->stanga);
    else
        return 0;
}
//////////////////////////////////////////////////rezolvare derivata
int arbore_numar(adresa_arbore a)//verifica daca toate operatiile din arbore contin numere
{
    if (a)
    {
        if (strchr(cifra, a->info[0]) || strchr("-/*+^", a->info[0]))//este numar/cifra sau este o operatie
            return 1 + arbore_numar(a->dreapta) + arbore_numar(a->stanga);
        else
            return arbore_numar(a->stanga) + arbore_numar(a->dreapta);
    }
    else
        return 0;
}
void simplificare_operatii_numere(adresa_arbore a, adresa_arbore& b)
{
    if (size(a) > 1 && arbore_numar(a) == size(a))
    {
        Initializare_arbore(b);
        if (strcmp(a->info,"+")==0)
        {
            adresa_arbore c, d;
            simplificare_operatii_numere(a->stanga, c);
            simplificare_operatii_numere(a->dreapta, d);
            if (size(d) == 1 && size(c) == 1)//ambele sunt numere
            {
                int x, y, z;
                x = atoi(c->info);
                y = atoi(d->info);
                z = x + y;
                itoa(z, b->info, 10);
            }
            if (size(c) == 1 && size(d) > 1)//avem de  exemplu 1+2/3
            {
                int x, y, z;
                x = atoi(c->info);
                y = atoi(d->stanga->info);
                z = atoi(d->dreapta->info);
                b->info[0] = '/';
                Initializare_arbore(b->stanga);
                Initializare_arbore(b->dreapta);
                itoa(z, b->dreapta->info, 10);//numitorul fractiei finale
                y = x * z + y;
                itoa(y, b->stanga->info, 10);
            }
            if (size(c) > 1 && size(d) == 1)
            {
                int x, y, z;
                x = atoi(d->info);
                y = atoi(c->stanga->info);
                z = atoi(c->dreapta->info);
                b->info[0] = '/';
                Initializare_arbore(b->stanga);
                Initializare_arbore(b->dreapta);
                itoa(z, b->dreapta->info, 10);//numitorul fractiei finale
                y = x * z + y;
                itoa(y, b->stanga->info, 10);
            }
            if (size(c) > 1 && size(d) > 1)//de exemplu 2/3+4/5 = 2*5/3*5 + 4*3/3*5
            {
                int x1, y1, x2, y2, X, Y;
                x1 = atoi(c->stanga->info);
                y1 = atoi(c->dreapta->info);
                x2 = atoi(d->stanga->info);
                y2 = atoi(d->dreapta->info);
                Y = y1 * y2;
                X = x1 * y2 + x2 * y1;
                int x, y;
                x = X;
                y = Y;
                while (x != y)
                    if (x > y)
                        x -= y;
                    else
                        y -= x;
                X = X / x;
                Y = Y / y;
                if (X % Y != 0)
                {
                    b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    itoa(X, b->stanga->info, 10);
                    itoa(Y, b->dreapta->info, 10);
                }
                else
                    itoa(X / Y, b->info, 10);
            }
        }
        if (strcmp(a->info, "-") == 0)
        {
            if (a->stanga == NULL)
            {   
                char aux[500];
                simplificare_operatii_numere(a->dreapta,a->dreapta);
                if(size(a->dreapta)==1)
                {   
                    strcpy(aux, a->dreapta->info);
                    strcpy(a->dreapta->info, "-");
                    strcat(a->dreapta->info, aux);
                    Initializare_arbore(b);
                    strcpy(b->info,a->dreapta->info);
                }
                else
                {   Initializare_arbore(b);
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    strcpy(b->stanga->info,"-1");
                    strcpy(b->info,"*");
                    b->dreapta = a->dreapta;
                    simplificare_operatii_numere(b,b);
                }
            }
            else
            {
                adresa_arbore c, d;
                Initializare_arbore(b);
                simplificare_operatii_numere(a->stanga, c);
                simplificare_operatii_numere(a->dreapta, d);
                int x, y, z;
                if (size(c) == 1 && size(d) == 1)
                {
                    x = atoi(c->info);
                    y = atoi(d->info);
                    z = x - y;
                    itoa(z, b->info, 10);
                }
                if (size(c) == 1 && size(d) > 1)//avem de  exemplu 1-2/3
                {
                    int x, y, z; // x-y/z = (x*z-y)/z
                    x = atoi(c->info);
                    y = atoi(d->stanga->info);
                    z = atoi(d->dreapta->info);
                    b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    itoa(z, b->dreapta->info, 10);//numitorul fractiei finale
                    y = x * z - y;
                    itoa(y, b->stanga->info, 10);
                }
                if (size(c) > 1 && size(d) == 1)//   3/2-1   x/y-z = (x-y*z)/y
                {
                    int x, y, z;
                    z = atoi(d->info);
                    x = atoi(c->stanga->info);
                    y = atoi(c->dreapta->info);
                    b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    itoa(y, b->dreapta->info, 10);//numitorul fractiei finale
                    x = x - z * y;
                    itoa(x, b->stanga->info, 10);
                }
                if (size(c) > 1 && size(d) > 1)//de exemplu 2/3 - 4/5 = 2*5/3*5 - 4*3/3*5 = (2*5-4*3)/3*5
                {  
                    int x1, y1, x2, y2, X, Y;
                x1 = atoi(c->stanga->info);
                y1 = atoi(c->dreapta->info);
                x2 = atoi(d->stanga->info);
                y2 = atoi(d->dreapta->info);
                Y = y1 * y2;
                X = x1 * y2 - x2 * y1;
                int x, y;
                x = abs(X);
                y = abs(Y);
                while (x != y)
                    if (x > y)
                        x -= y;
                    else
                        y -= x;
                X = X / x;
                Y = Y / y;
                
                if (X % Y != 0)
                {
                    b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    itoa(X, b->stanga->info, 10);
                    itoa(Y, b->dreapta->info, 10);
                }
                else
                    itoa(X / Y, b->info, 10);
                }
            }
        }
        if (a->info[0] == '*')
        {
            adresa_arbore c, d;
            Initializare_arbore(c);
            Initializare_arbore(d);
            simplificare_operatii_numere(a->stanga, c);
            simplificare_operatii_numere(a->dreapta, d);
            if (size(c) == 1 && size(d) == 1)
            {
                int x, y, z;
                x = atoi(c->info);
                y = atoi(d->info);
                z = x * y;
                itoa(z, b->info, 10);
            }
            if (size(c) > 1 && size(d) == 1)//de ex 2/3*5
            {
                int x, y, z, aux1, aux2;
                x = atoi(c->stanga->info);
                y = atoi(c->dreapta->info);
                z = atoi(d->info);
                x = x * z;
                if (x % y != 0)
                {
                    aux1 = x;
                    aux2 = y;
                    while (aux1 != aux2)
                        if (aux1 > aux2)
                            aux1 -= aux2;
                        else
                            aux2 -= aux1;
                    x /= aux1;
                    y /= aux2;
                    b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    itoa(x, b->stanga->info, 10);
                    itoa(y, b->dreapta->info, 10);
                }
                else
                    itoa(x / y, b->info, 10);
            }
            if (size(c) == 1 && size(d) > 1)//de ex 3*(5/4)
            {   
                int x, y, z, aux1, aux2;
                x = atoi(c->info);
                y = atoi(d->dreapta->info);
                z = atoi(d->stanga->info);
                x = x * z;
                if (x % y != 0)
                {
                    aux1 = x;
                    aux2 = y;
                    while (aux1 != aux2)
                        if (aux1 > aux2)
                            aux1 -= aux2;
                        else
                            aux2 -= aux1;
                    x /= aux1;
                    y /= aux2;
                    b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                    Initializare_arbore(b->dreapta);
                    itoa(x, b->stanga->info, 10);
                    itoa(y, b->dreapta->info, 10);


                }
                else
                    itoa(x / y, b->info, 10);
            }
            if (size(c) > 1 && size(d) > 1)
            {   
                int x1, x2, y1, y2, X, Y;//    x1/y1 * x2/y2
                x1 = atoi(c->stanga->info);
                y1 = atoi(c->dreapta->info);
                x2 = atoi(d->stanga->info);
                y2 = atoi(d->dreapta->info);
                X = x1 * x2;
                Y = y1 * y2;
                if((X>0 && Y>0)||(X<0 && Y<0))
                {   X = abs(X);
                    Y = abs(Y);
                    if (X % Y != 0)
                    {
                        x1 = X;
                        x2 = Y;
                        while (x1 != x2)
                            if (x1 > x2)
                                x1 -= x2;
                            else
                                x2 -= x1;
                        X /= x1;
                        Y /= x2;
                        b->info[0] = '/';
                        Initializare_arbore(b->stanga);
                        Initializare_arbore(b->dreapta);
                        itoa(X, b->stanga->info, 10);
                        itoa(Y, b->dreapta->info, 10);
                    }
                    else
                        itoa(X / Y, b->info, 10);
                }
                else
                {      b->info[0] = '/';
                    Initializare_arbore(b->stanga);
                        Initializare_arbore(b->dreapta);
                    itoa(X, b->stanga->info, 10);
                        itoa(Y, b->dreapta->info, 10);
                }
            }
        }
        if (a->info[0] == '/')//trebuie pt cazul size(c)>1 si size(d)>1
        {
            Initializare_arbore(b);
            adresa_arbore c, d;
            Initializare_arbore(c);
            Initializare_arbore(d);
            simplificare_operatii_numere(a->stanga, c);
            simplificare_operatii_numere(a->dreapta, d);
            if (size(c) == 1 && size(d) == 1)
            {   
                double x, y;
                x = atoi(c->info);
                y = atoi(d->info);
                double z;
                z = x / y;
                if (fmod(z, 1.0) == 0)
                    itoa(int(z), b->info, 10);
                else
                {   
                    int aux1 , aux2;
                    aux1 = x;
                    aux2 = y;
                    while (aux1 != aux2)
                        if (aux1 > aux2)
                            aux1 -= aux2;
                        else
                            aux2 -= aux1;
                    x /= aux1;
                    y /= aux2;
                    b->info[0] = '/';
                    Initializare_arbore(b->dreapta);
                    Initializare_arbore(b->stanga);
                    itoa(x, c->info, 10);
                    itoa(y,d->info,10);
                    b->dreapta = d;
                    b->stanga = c;
                }
            }
            else
            if (size(c) > 1 && size(d) == 1)// de ex 3/4/5    3/4  /  5  
            {
                int x, y, z;
                x = atoi(d->info);
                y = atoi(c->stanga->info);
                z = atoi(c->dreapta->info);
                x = x * z;//numitorul final
                b->info[0] = '/';
                Initializare_arbore(b->stanga);
                Initializare_arbore(b->dreapta);
                if (y % x != 0)
                {
                    int m, n;
                    m = y;
                    n = x;
                    while (m != n)
                        if (m > n)
                            m -= n;
                        else
                            n -= m;
                    y /= m;
                    x /= m;
                    itoa(y, b->stanga->info, 10);
                    itoa(x, b->dreapta->info, 10);
                }
                else
                    itoa(y / x, b->info, 10);
            }
            else
            if (size(c) == 1 && size(d) > 1) //  3/(4/5)   3  /  4/5 = (3*5)/4
            {
                int x, y, z;      //      x   /  y/z = (x*z)/y
                x = atoi(c->info);
                y = atoi(d->stanga->info);
                z = atoi(d->dreapta->info);
                x = x * z;//numaratorul final
                if (x % y != 0)
                {
                    int m, n;
                    m = y;
                    n = x;
                    while (m != n)
                        if (m > n)
                            m -= n;
                        else
                            n -= m;
                    y /= m;
                    x /= m;
                    b->info[0] = '/';
                    itoa(x, b->stanga->info, 10);
                    itoa(y, b->dreapta->info, 10);

                }
                else
                    itoa(x / y, b->info, 10);
            }
            else
            if (size(c) > 1 && size(d) > 1) // de ex (1/2)/(1/3)
            {   
                int x1, y1, x2, y2;
                x1 = atoi(c->stanga->info);
                x2 = atoi(d->stanga->info);
                y1 = atoi(c->dreapta->info);
                y2 = atoi(d->dreapta->info);
                x1 = x1 * y2;
                y1 = y1 * x2;
                if(x1>0 && x2>0)
                {
                    if (x1 % y1 != 0)
                    {
                        int m, n;
                        m = y1;
                        n = x1;
                        while (m != n)
                            if (m > n)
                                m -= n;
                            else
                                n -= m;
                        y1 /= m;
                        x1 /= m;
                        b->info[0] = '/';
                        itoa(x1, b->stanga->info, 10);
                        itoa(y1, b->dreapta->info, 10);

                    }
                    else
                        itoa(x1 / y1, b->info, 10);
                }
                else
                {
                    itoa(x1, b->stanga->info, 10);
                    itoa(y1, b->dreapta->info, 10);
                }
            }
        }
        if (a->info[0] == '^')
        {   
            adresa_arbore c, d;
            simplificare_operatii_numere(a->stanga, c);
            simplificare_operatii_numere(a->dreapta, d);
            if (size(c) == 1 && size(d) == 1)
            {
                int N, nr1, nr2;
                nr1 = atoi(c->info);
                nr2 = atoi(d->info);
                N = pow(nr1, nr2);
                itoa(N, b->info, 10);
            }
            if (size(c) > 1 && size(d) == 1)
            {
                int N1, N2, nr1, nr2, nr3;
                nr1 = atoi(c->stanga->info);
                nr2 = atoi(c->dreapta->info);
                nr3 = atoi(d->info);
                N1 = pow(nr1, nr3);
                N2 = pow(nr2, nr3);
                Initializare_arbore(b->stanga);
                Initializare_arbore(b->dreapta);
                b->info[0] == '/';
                itoa(N1, b->stanga->info, 10);
                itoa(N2, b->dreapta->info, 10);
            }
        }
    }
    if (size(a) == 1)// a.info[0]='2' (de ex)
    {
        Initializare_arbore(b);
        strcpy(b->info, a->info);
    }
    if (arbore_numar(a) != size(a) && size(a) > 1)
    {  
        if(a->info[0]=='-' && strcmp(a->dreapta->info,"*")==0  && arbore_numar(a->dreapta->stanga)>0 && atoi(a->dreapta->stanga->info)<0  )//strchr("*/",a->dreapta->info[0])
                        {   
                            Initializare_arbore(b);
                            strcpy(a->info,"+");
                            int x = atoi(a->dreapta->stanga->info);
                            x = abs(x);
                            itoa(x , a->dreapta->stanga->info,10);
                            b = a;
                        }
        else
            if(strcmp(a->info,"-")==0 && strcmp(a->dreapta->info,"/")==0  && arbore_numar(a->dreapta->stanga)==1 && atoi(a->dreapta->stanga->info)<0)
            { 
                Initializare_arbore(b);
                            strcpy(a->info,"+");
                            int x = atoi(a->dreapta->stanga->info);
                            x = abs(x);
                            itoa(x , a->dreapta->stanga->info,10);
                            b = a;
            }
        else
            if (a->info[0] == '+' && size(a->stanga->dreapta) == arbore_numar(a->stanga->dreapta) && size(a->dreapta) == arbore_numar(a->dreapta) && strcmp(a->stanga->info, "+") == 0)
            {
                simplificare_operatii_numere(a->stanga->dreapta, a->stanga->dreapta);
                simplificare_operatii_numere(a->dreapta, a->dreapta);
                adresa_arbore a1, a2, a3;
                Initializare_arbore(a3);
                Initializare_arbore(a2);
                Initializare_arbore(a1);
                a2 = a->stanga->dreapta;
                a3 = a->dreapta;
                a1->info[0] = '+';
                a1->stanga = a2;
                a1->dreapta = a3;
                simplificare_operatii_numere(a1, a1);
                strcpy(b->info, "+");
                Initializare_arbore(b->dreapta);
                Initializare_arbore(b->stanga);
                b->stanga = a->stanga->stanga;
                b->dreapta = a1;
                Initializare_arbore(a1);
                a1 = b;
                Initializare_arbore(b);
                simplificare_operatii_numere(a1, b);

            }
            else
                if (a->info[0] == '*' && size(a->stanga->dreapta) == arbore_numar(a->stanga->dreapta) && size(a->dreapta) == arbore_numar(a->dreapta) && strcmp(a->stanga->info, "*") == 0)
                {
                    simplificare_operatii_numere(a->stanga->dreapta, a->stanga->dreapta);
                    simplificare_operatii_numere(a->dreapta, a->dreapta);
                    adresa_arbore a1, a2, a3;
                    Initializare_arbore(a3);
                    Initializare_arbore(a2);
                    Initializare_arbore(a1);
                    a2 = a->stanga->dreapta;
                    a3 = a->dreapta;
                    a1->info[0] = '*';
                    a1->stanga = a2;
                    a1->dreapta = a3;
                    simplificare_operatii_numere(a1, a1);
                    strcpy(b->info, "*");
                    b = a->stanga;
                    b->stanga = a1;
                    Initializare_arbore(a1);
                    a1 = b;
                    Initializare_arbore(b);
                    simplificare_operatii_numere(a1, b);
                }
                else
                    if (a->info[0] == '*' && size(a->dreapta->stanga) == arbore_numar(a->dreapta->stanga) && size(a->stanga) == arbore_numar(a->stanga) && strcmp(a->dreapta->info, "*") == 0)
                    {   
                        simplificare_operatii_numere(a->dreapta->stanga, a->dreapta->stanga);
                        simplificare_operatii_numere(a->stanga, a->stanga);
                        adresa_arbore a1, a2, a3;
                        Initializare_arbore(a1);
                        Initializare_arbore(a2);
                        Initializare_arbore(a3);
                        a2 = a->dreapta->stanga;
                        a3 = a->stanga;
                        a1->info[0] = '*';
                        a1->stanga = a3;
                        a1->dreapta = a2;
                        Initializare_arbore(a2);
                        simplificare_operatii_numere(a1, a2);
                        strcpy(b->info, "*");
                        b->stanga = a2;
                        b->dreapta = a->dreapta->dreapta;
                        a1 = b;
                        Initializare_arbore(b);
                        simplificare_operatii_numere(a1, b);
                    }
                    else
                        if (a->info[0] == '^' && a->stanga->info[0] == '^' && arbore_numar(a->dreapta) == size(a->dreapta) && arbore_numar(a->stanga->dreapta) == size(a->stanga->dreapta))
                        {
                            simplificare_operatii_numere(a->dreapta, a->dreapta);
                            simplificare_operatii_numere(a->stanga->dreapta, a->stanga->dreapta);
                            adresa_arbore a1, a2, a3;
                            Initializare_arbore(a1);
                            Initializare_arbore(a2);
                            Initializare_arbore(a3);
                            a2 = a->dreapta;
                            a3 = a->stanga->dreapta;
                            a1->info[0] = '*';
                            a1->stanga = a2;
                            a1->dreapta = a3;
                            simplificare_operatii_numere(a1, a1);
                            strcpy(b->info, "^");
                            b->stanga = a->stanga->stanga;
                            b->dreapta = a1;
                            a1 = b;
                            Initializare_arbore(b);
                            simplificare_operatii_numere(a1, b);
                        }
                            else
                        {   
                            strcpy(b->info, a->info);
                            if (a->stanga)
                            {
                                Initializare_arbore(b->stanga);
                                simplificare_operatii_numere(a->stanga, b->stanga);
                            }
                            if (a->dreapta)
                            {
                                Initializare_arbore(b->dreapta);
                                simplificare_operatii_numere(a->dreapta, b->dreapta);
                            }
                        }
    }
}
void simplificare_1_0(adresa_arbore a,adresa_arbore &b)
{
    if(strcmp(a->info,"+")==0)
    {
        if(strcmp(a->stanga->info,"0")==0)
            {
                Initializare_arbore(b);
                b = a->dreapta;
                simplificare_1_0(b,b);
            }
        else
            if(strcmp(a->dreapta->info,"0")==0)
                {
                    Initializare_arbore(b);
                    b = a->stanga;
                    simplificare_1_0(b,b);
                }
            else
                {
                    Initializare_arbore(b);
                    b = a;
                    simplificare_1_0(a->dreapta,b->dreapta);
                    simplificare_1_0(a->stanga,b->stanga);
                }

    }
    else
        if(strcmp(a->info,"-")==0 && a->stanga!=NULL)
        {
            if(strcmp(a->dreapta->info,"0")==0)
            {
                Initializare_arbore(b);
                b = a->stanga;
                simplificare_1_0(b,b);
            }
            else
                if(strcmp(a->stanga->info,"0")==0)
                {
                    Initializare_arbore(b);
                    strcpy(b->info,"-");
                    b->dreapta = a->dreapta;
                    simplificare_1_0(b,b);
                }
                else
                {
                    Initializare_arbore(b);
                    b = a;
                    simplificare_1_0(a->dreapta,b->dreapta);
                    simplificare_1_0(a->stanga,b->stanga);
                }

        }
        else
            if(strcmp(a->info,"*")==0)
        {
            if((strcmp(a->stanga->info,"0")==0) || (strcmp(a->dreapta->info,"0")==0))
            {
                Initializare_arbore(b);
                strcpy(b->info,"0");
            }
            else
                if(strcmp(a->stanga->info,"1")==0)
                {
                    Initializare_arbore(b);
                    b = a->dreapta;
                    simplificare_1_0(b,b);
                }
                else
                    if(strcmp(a->dreapta->info,"1")==0)
                    {
                        Initializare_arbore(b);
                        b = a->stanga;
                        //parcurgereInInordine(b);
                        simplificare_1_0(b,b);
                    }
                    else
                        {
                             Initializare_arbore(b);
                                strcpy(b->info,a->info);
                                simplificare_1_0(a->dreapta,b->dreapta);
                                simplificare_1_0(a->stanga,b->stanga);
                        }
        }
            else
                if(strcmp(a->info,"/")==0)
                {   
                    if((strcmp(a->stanga->info,"0")==0))
                    {
                        Initializare_arbore(b);
                        b->info[0]='0';
                    }
                    else if(strcmp(a->dreapta->info,"1")==0)
                    {
                        Initializare_arbore(b);
                        b = a->stanga;
                        simplificare_1_0(b,b);
                    }
                    else
                    {
                        Initializare_arbore(b);
                                b = a;
                                simplificare_1_0(a->dreapta,b->dreapta);
                                simplificare_1_0(a->stanga,b->stanga);
                    }
                }
                else
                    if(strcmp(a->info,"^")==0)

                    {
                        if((strcmp(a->dreapta->info,"0")==0))
                        {
                            Initializare_arbore(b);
                            strcpy(b->info,"1");
                        }
                        else
                            if(strcmp(a->dreapta->info,"1")==0)
                            {
                                Initializare_arbore(b);
                                b = a->stanga;
                                simplificare_1_0(b,b);
                            }
                            else
                                if((strcmp(a->stanga->info,"0")==0))
                                {
                                    Initializare_arbore(b);
                                    strcpy(b->info,"0");
                                }
                                else
                                    if((strcmp(a->stanga->info,"1")==0))
                                    {
                                        Initializare_arbore(b);
                                        strcpy(b->info,"1");
                                    }
                                    else
                                        {
                                             Initializare_arbore(b);
                                            b = a;
                                            simplificare_1_0(a->dreapta,b->dreapta);
                                            simplificare_1_0(a->stanga,b->stanga);
                                        }
                    }
                    else
                        b = a;
}
void derivare(adresa_arbore a, adresa_arbore& A)
{
    Initializare_arbore(A);
    if (strchr(cifra, a->info[0]) || size(a) == arbore_numar(a) || strcmp(a->info,"e")==0)//nodul este numar sau arborele este format doar din numere
    {
        strcpy(A->info, "0");
        return;
    }
    if (a->info[0] == 'x' || a->info[0] == 'X')
        A->info[0] = '1';
    if (strcmp(a->info, "ln") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        a1->info[0] = '/';
        Initializare_arbore(a3);
        a2->info[0] = '1';
        a1->stanga = a2;
        a3 = a->dreapta;//numitorul este 
        a1->dreapta = a3;
        Initializare_arbore(a4);
        a4->info[0] = '*';//arborele principal
        derivare(a->dreapta, a5);
        a4->dreapta = a5;
        a4->stanga = a1;
        A = a4;
    }
    if (strcmp(a->info, "log") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5, a6;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        derivare(a->dreapta, a1);
        A->stanga = a1;
        A->info[0] = '/';
        a2->info[0] = '*';
        a3 = a->dreapta;
        a2->stanga = a3;
        strcpy(a4->info, "ln");
        strcpy(a5->info, a->stanga->info);
        a4->dreapta = a5;
        a2->dreapta = a4;
        A->dreapta = a2;

    }
    if (strcmp(a->info, "sin") == 0)
    {
        adresa_arbore a1, a2, a3;
        Initializare_arbore(a1);
        strcpy(a1->info, "cos");
        Initializare_arbore(a2);
        a2 = a->dreapta;
        a1->dreapta = a2;
        Initializare_arbore(a3);
        derivare(a->dreapta, a3);
        strcpy(A->info, "*");
        A->stanga = a1;
        A->dreapta = a3;

    }
    if (strcmp(a->info, "cos") == 0)
    {
        adresa_arbore a1, a2, a3, a4;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        a1 = a->dreapta;
        strcpy(a2->info, "sin");
        a2->dreapta = a1;
        a3->info[0] = '*';
        Initializare_arbore(a3->stanga);
        strcpy(a3->stanga->info,"-1");
        a3->dreapta = a2;
        A->stanga = a3;
        strcpy(A->info, "*");
        derivare(a->dreapta, a4);
        A->dreapta = a4;

    }
    if (strcmp(a->info, "tg") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        a1 = a->dreapta;
        strcpy(a2->info, "cos");
        a2->dreapta = a1;
        a3->info[0] = '2';
        a4->info[0] = '^';
        a4->stanga = a2;
        a4->dreapta = a3;
        //A->info[0] = '/';
        strcpy(A->info, "/");
        derivare(a->dreapta, a5);
        A->stanga = a5;
        A->dreapta = a4;
    }
    if (strcmp(a->info, "ctg") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5, a6;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        Initializare_arbore(a6);
        a1 = a->dreapta;
        strcpy(a2->info, "sin");
        a2->dreapta = a1;
        a3->info[0] = '2';
        a4->info[0] = '^';
        a4->stanga = a2;
        a4->dreapta = a3;
        A->dreapta = a4;
        derivare(a->dreapta, a6);
        a5->info[0] = '-';
        a5->dreapta = a6;
        A->stanga = a5;
        strcpy(A->info, "/");
    }
    if (strcmp(a->info, "arcsin") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5, a6, a7, a8, a9;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        Initializare_arbore(a6);
        Initializare_arbore(a7);
        Initializare_arbore(a8);
        Initializare_arbore(a9);
        a1->info[0] = '2';
        a2->info[0] = '^';
        a2->stanga = a->dreapta;
        a2->dreapta = a1;
        a4->info[0] = '1';
        a3->info[0] = '-';
        a3->stanga = a4;
        a3->dreapta = a2;
        a5->info[0] = '2';
        a6->info[0] = '1';
        a7->info[0] = '/';
        a7->stanga = a6;
        a7->dreapta = a5;
        a8->info[0] = '^';
        a8->stanga = a3;
        a8->dreapta = a7;
        derivare(a->dreapta, a9);
        strcpy(A->info, "/");
        A->stanga = a9;
        A->dreapta = a8;
    }
    if (strcmp(a->info, "arccos") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        Initializare_arbore(a6);
        Initializare_arbore(a7);
        Initializare_arbore(a8);
        Initializare_arbore(a9);
        Initializare_arbore(a10);
        a1->info[0] = '2';
        a2->info[0] = '^';
        a2->stanga = a->dreapta;
        a2->dreapta = a1;
        a4->info[0] = '1';
        a3->info[0] = '-';
        a3->stanga = a4;
        a3->dreapta = a2;
        a5->info[0] = '2';
        a6->info[0] = '1';
        a7->info[0] = '/';
        a7->stanga = a6;
        a7->dreapta = a5;
        a8->info[0] = '^';
        a8->stanga = a3;
        a8->dreapta = a7;
        derivare(a->dreapta, a9);
        strcpy(A->info, "/");
        A->dreapta = a8;
        a10->info[0] = '-';
        a10->dreapta = a9;
        A->stanga = a10;
    }
    if (strcmp(a->info, "arctg") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        strcpy(A->info, "/");
        derivare(a->dreapta, a1);
        A->stanga = a1;
        a2->info[0] = '1';
        a3->info[0] = '+';
        a4->info[0] = '^';
        a5->info[0] = '2';
        a4->dreapta = a5;
        a4->stanga = a->dreapta;
        a3->dreapta = a2;
        a3->stanga = a4;
        A->dreapta = a3;
    }
    if (strcmp(a->info, "arcctg") == 0)
    {
        adresa_arbore a1, a2, a3, a4, a5, a6;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        Initializare_arbore(a3);
        Initializare_arbore(a4);
        Initializare_arbore(a5);
        Initializare_arbore(a6);
        strcpy(A->info, "/");
        derivare(a->dreapta, a1);
        a6->info[0] = '-';
        a6->dreapta = a1;
        A->stanga = a6;
        a2->info[0] = '1';
        a3->info[0] = '+';
        a4->info[0] = '^';
        a5->info[0] = '2';
        a4->dreapta = a5;
        a4->stanga = a->dreapta;
        a3->dreapta = a2;
        a3->stanga = a4;
        A->dreapta = a3;
    }

    if (a->info[0] == '+')
    {
        A->info[0] = '+';//(f+g)' = f' + g'
        adresa_arbore a1, a2;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        derivare(a->stanga, a1);
        derivare(a->dreapta, a2);
        A->stanga = a1;
        A->dreapta = a2;
    }
    if (a->info[0] == '-')
    {
        A->info[0] = '-';
        adresa_arbore a1, a2;
        Initializare_arbore(a1);
        Initializare_arbore(a2);
        derivare(a->stanga, a1);
        derivare(a->dreapta, a2);
        A->stanga = a1;
        A->dreapta = a2;
    }
    if (a->info[0] == '*')
    {
        if (strchr(cifra, a->stanga->info[0]) == 0 && strchr(cifra, a->dreapta->info[0]) == 0)
        {   // a.stanga = f si  a.dreapta = g 
            adresa_arbore a1, a2, a3, a4, a5;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            Initializare_arbore(a3);
            Initializare_arbore(a4);
            strcpy(A->info, "+");//(f*g)' = f'*g + f*g'
            a1->info[0] = '*';
            a2->info[0] = '*';
            derivare(a->stanga, a3);//f derivat
            a1->stanga = a3;
            a1->dreapta = a->dreapta;
            derivare(a->dreapta, a4);//g derivat
            a2->stanga = a->stanga;
            a2->dreapta = a4;
            A->stanga = a1;
            A->dreapta = a2;
        }
        else
        if (strchr(cifra, a->stanga->info[0]) && strchr(cifra, a->dreapta->info[0]))//ambele sunt constante
            strcpy(A->info, "0");
        else
        if (strchr(cifra, a->stanga->info[0]) == 0 && (strchr(cifra, a->dreapta->info[0]) || arbore_numar(a->dreapta)>1))//prima parametru si a 2 a constanta
        {   
            adresa_arbore a1, a2;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            strcpy(A->info, "*");
            A->dreapta = a->dreapta;
            derivare(a->stanga, a1);
            A->stanga = a1;
        }
        else
        if ((strchr(cifra, a->stanga->info[0]) || arbore_numar(a->stanga)>1) && strchr(cifra, a->dreapta->info[0]) == 0)//prima constanta iar a 2 a parametru
        {
            adresa_arbore a1, a2;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            strcpy(A->info, "*");
            A->stanga = a->stanga;
            derivare(a->dreapta, a1);
            A->dreapta = a1;
        }
    }
    if (a->info[0] == '/')
    {
        if (strchr(cifra, a->dreapta->info[0]) == 0)//  f/g , g este parametru
        {
            adresa_arbore a1, a2, a3, a4, a5, a6, a7, a8, a9;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            Initializare_arbore(a3);
            Initializare_arbore(a4);
            Initializare_arbore(a5);
            Initializare_arbore(a6);
            Initializare_arbore(a7);
            a5->info[0] = '-';//(f*g)' = f'*g - f*g'
            a1->info[0] = '*';
            a2->info[0] = '*';
            derivare(a->stanga, a3);//f derivat
            a1->stanga = a3;
            a1->dreapta = a->dreapta;
            derivare(a->dreapta, a4);//g derivat
            a2->stanga = a->stanga;
            a2->dreapta = a4;
            a5->stanga = a1;
            a5->dreapta = a2;//numaratorul
            strcpy(A->info, "/");
            a6->info[0] = '2';
            a7->info[0] = '^';
            a7->dreapta = a6;
            a7->stanga = a->dreapta;
            A->stanga = a5;
            A->dreapta = a7;
        }
        else
        if (strchr(cifra, a->stanga->info[0]) && strchr(cifra, a->dreapta->info[0]))
            strcpy(A->info, "0");
        else
        if (strchr(cifra, a->stanga->info[0]) == 0 && strchr(cifra, a->dreapta->info[0]))// ex (x^2)/2
        {
            adresa_arbore a1, a2;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            strcpy(A->info, "/");
            A->dreapta = a->dreapta;
            derivare(a->stanga, a1);
            A->stanga = a1;
        }

    }
    if (a->info[0] == '^')
    {
        if (strchr(cifra, a->stanga->info[0]) && strchr(cifra, a->dreapta->info[0]) == 0) //a ^ x
        {
            adresa_arbore a1, a2, a3, a4, a5;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            Initializare_arbore(a3);
            Initializare_arbore(a4);
            Initializare_arbore(a5);
            strcpy(a1->info, "ln");
            Initializare_arbore(a1->dreapta);
            strcpy(a1->dreapta->info, a->stanga->info);
            a2->info[0] = '^';
            Initializare_arbore(a2->stanga);
            Initializare_arbore(a2->dreapta);
            strcpy(a2->stanga->info, a->stanga->info);
            a2->dreapta = a->dreapta;
            a3->info[0] = '*';
            a3->stanga = a2;
            a3->dreapta = a1;
            derivare(a->dreapta, a4);
            strcpy(A->info, "*");
            A->stanga = a3;
            A->dreapta = a4;
        }
        else
        if (strcmp(a->stanga->info, "e") == 0)
        {
            adresa_arbore a1, a2, a3;
            Initializare_arbore(a1);
            Initializare_arbore(a2);
            Initializare_arbore(a3);
            a1->info[0] = '^';
            Initializare_arbore(a1->dreapta);
            Initializare_arbore(a1->stanga);
            a1->stanga->info[0] = 'e';
            a1->dreapta = a->dreapta;
            derivare(a->dreapta, a2);
            strcpy(A->info, "*");
            A->dreapta = a2;
            A->stanga = a1;
        }
        else
        if (strchr(cifra, a->stanga->info[0]) && strchr(cifra, a->dreapta->info[0])) // de ex 3^2
            strcpy(A->info, "0");
        else
        if (strchr(cifra, a->stanga->info[0]) == 0) //x este baza sau o alta functie matematica
        {
            if (arbore_numar(a->dreapta)==1 && size(a->dreapta) == 1 && strcmp(a->dreapta->info, "0") != 0)//exponentul este numar natural nenul
            {
                adresa_arbore a1, a2, a3;
                Initializare_arbore(a1);
                Initializare_arbore(a2);
                Initializare_arbore(a3);
                strcpy(a1->info, a->dreapta->info);
                int x = atoi(a->dreapta->info);
                strcpy(A->info, "*");
                a2->info[0] = '^';
                Initializare_arbore(a2->stanga);
                a2->stanga = a->stanga;
                x--;
                Initializare_arbore(a2->dreapta);
                itoa(x, a2->dreapta->info, 10);
                a3->info[0] = '*';
                a3->stanga = a1;
                a3->dreapta = a2;
                Initializare_arbore(A->dreapta);
                derivare(a->stanga, A->dreapta);
                A->stanga = a3;
            }
            if (strchr(cifra, a->dreapta->info[0]) && strcmp(a->dreapta->info, "0") == 0)//exponentul este 0
                strcpy(A->info, "0");
            if (size(a->dreapta) == arbore_numar(a->dreapta) && size(a->dreapta) > 1)//numar rational sau operatii de numere rationale
            {
                adresa_arbore A1;
                simplificare_operatii_numere(a->dreapta, A1);
                adresa_arbore a1, a2, a3;
                Initializare_arbore(a1);
                Initializare_arbore(a2);
                Initializare_arbore(a3);
                if (size(A1) == 1)
                {
                    int x = atoi(A1->info);
                    strcpy(a1->info, A1->info);
                    strcpy(A->info, "*");
                    a2->info[0] = '^';
                    Initializare_arbore(a2->stanga);
                    a2->stanga = a->stanga;
                    x--;
                    Initializare_arbore(a2->dreapta);
                    itoa(x, a2->dreapta->info, 10);
                    a3->info[0] = '*';
                    a3->stanga = a1;
                    a3->dreapta = a2;
                    Initializare_arbore(A->dreapta);
                    derivare(a->stanga, A->dreapta);
                    A->stanga = a3;
                }
                else//de exemplu pt 1/2 - 1 o sa devina -1/2 sau pt 11/7 - 1 o sa devina 4/7
                {//in A1 o sa fie exponentul din functia data,nu din derivata
                    int x, y, z;
                    x = atoi(A1->dreapta->info);//numitorul
                    y = atoi(A1->stanga->info);//numaratorul
                    a1 = A1;
                    y = y - x;
                    Initializare_arbore(a2->dreapta);
                    Initializare_arbore(a2->stanga);
                    a2->info[0] = '/';
                    itoa(y, a2->stanga->info, 10);
                    itoa(x, a2->dreapta->info, 10);
                    Initializare_arbore(a3->stanga);
                    Initializare_arbore(a3->dreapta);
                    a3->dreapta = a2;
                    a3->stanga = a->stanga;
                    a3->info[0] = '^';
                    adresa_arbore a4, a5;
                    Initializare_arbore(a4);
                    Initializare_arbore(a4->stanga);
                    Initializare_arbore(a4->dreapta);
                    a4->stanga = a1;
                    a4->dreapta = a3;
                    a4->info[0] = '*';
                    Initializare_arbore(a5);
                    Initializare_arbore(a5->stanga);
                    Initializare_arbore(a5->dreapta);
                    a5->info[0] = '*';
                    a5->stanga = a4;
                    derivare(a->stanga, a5->dreapta);
                    A = a5;
                    //parcurgereInInordine(A);
                }
            }
            if (strchr(cifra, a->stanga->info[0]) == 0 && strchr(cifra, a->dreapta->info[0]) == 0 && arbore_numar(a->dreapta) != size(a->dreapta) && arbore_numar(a->stanga) != size(a->stanga)) //de ex x^x
            {
                adresa_arbore a1, a2, a3, a4, a5, a6, a7, a8, a9, st, dr;
                Initializare_arbore(a1);
                Initializare_arbore(a2);
                Initializare_arbore(a3);
                Initializare_arbore(a4);
                Initializare_arbore(a5);
                Initializare_arbore(a6);
                Initializare_arbore(a7);
                Initializare_arbore(a8);
                Initializare_arbore(a9);
                Initializare_arbore(st);
                Initializare_arbore(dr);
                st = a->stanga;
                dr = a->dreapta;
                a1 = a;
                a2->info[0] = '*';
                a2->stanga = a1;
                Initializare_arbore(a2->dreapta);
                strcpy(a2->dreapta->info, "ln");
                Initializare_arbore(a2->dreapta->dreapta);
                a2->dreapta->dreapta = st;
                a3->info[0] = '*';
                a3->stanga = a2;
                Initializare_arbore(a3->dreapta);
                derivare(dr, a3->dreapta);
                Initializare_arbore(A->stanga);
                A->stanga = a3;

                strcpy(A->info, "+");

                a4->info[0] = '-';
                Initializare_arbore(a4->stanga);
                Initializare_arbore(a4->dreapta);
                a4->stanga = dr;
                strcpy(a4->dreapta->info, "1");
                a5->dreapta = a4;
                a5->info[0] = '^';
                a5->stanga = st;
                a6->info[0] = '*';
                a6->stanga = a5;
                Initializare_arbore(a6->dreapta);
                derivare(st, a6->dreapta);
                a7->info[0] = '*';
                a7->stanga = dr;
                a7->dreapta = a6;
                A->dreapta = a7;
            }
        }

    }
}
void DERIVATA(char expresie[] , char derivata[] , adresa_arbore &AA)
{  
    expresie_cuvinte(expresie);
    postfixata(EXPRESIE, nr_cuvinte_expresie);
    adresa_arbore a;
    Initializare_arbore(a);
    determinare_arbore_expresie(a, postfixat, postfixat.nrElemente);
    simplificare_1_0(a,a);
    adresa_arbore A, A_simplificat;//aici punem derivata arborelui a care contine expresia data
    Initializare_arbore(A_simplificat);
    Initializare_arbore(A);
    derivare(a, A);
    simplificare_1_0(A,A);//1/2*x^(-1/2)
   simplificare_operatii_numere(A, A_simplificat);
    simplificare_1_0(A_simplificat,A_simplificat);
    deriv_paranteze(A_simplificat);
    for (int i = 0; i < nr_cuvinte_expresie; i++)
        memset(EXPRESIE[i], 0, sizeof(EXPRESIE[i]));
    nr_cuvinte_expresie=0;
    Initializare_arbore(AA);
    AA = A_simplificat;
}

int main()//simplify -> ((6*4)/4^2)*x
{   
    strcpy(derivata,"");
    strcpy(expresie,"");
    adresa_arbore AA;
    while(1)
    {   cout<<endl<<"f(x)=";
        cin>>expresie;
        DERIVATA(expresie,derivata,AA);
        cout<<"f'(x)="<<derivata<<endl;
        strcpy(derivata,"");
        strcpy(expresie,"");
        D:
        cout<<'\v'<<"e - exit"<<endl<<"c - continue"<<endl<<endl;
        char e;
        cin>>e;
        if(e=='e')
            break;
        else
            if(e=='c')
                continue;
            else
                 goto D;
    }
    return 0;
}
