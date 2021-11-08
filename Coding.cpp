#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

char kodowanie(int num)
{
    if(num < 0 || num >= 64)
        return '\0';

    if(num == 63)
        return '/';
    if(num == 62)
        return '+';
    if(num >= 52)
        return (char)(num - 4);
    if(num >= 26)
        return (char)(num - 26 + 'a');

    return (char)(num + 'A');
}

int main()
{
    cout << "Witam w programie kodujacym" << endl << endl;
    cout << "Podaj nazwe pliku z wiadomoscia do zakodowania: ";
    char nazwa[64];
    cin >> nazwa;
    cout << endl;
    fstream wiad(nazwa, ios::in);
    if(!wiad.good())
    {
        cout << "Dany plik nie istnieje. Zamykanie programu." << endl;
        system("pause");
        return 0;
    }

    cout << "Podaj nazwe pliku do ktorego chcesz zapisac powstaly kod:";
    cin >> nazwa;
    cout << endl;
    fstream kod(nazwa, ios::out);
    if(!kod.good())
    {
        cout << "Nie mozna utowrzyc pliku. Zamykanie programu." << endl;
        wiad.close();
        system("pause");
        return 0;
    }

    wiad.seekg(0, ios::end);
    int dlugosc = wiad.tellg();
    wiad.seekg(0);
    wiad.clear();

    int braki = dlugosc % 3;
    int n = (dlugosc - braki)/3;
    if(braki != 0)
        braki = 3 - braki;

    char c1, c2, c3;
    char jednostki[4];
    for(int i = 0; i < n; i++)
    {
       c1 = wiad.get();
       c2 = wiad.get();
       c3 = wiad.get();

       jednostki[0] = kodowanie((c1 & 0b11111100) >> 2);
       jednostki[1] = kodowanie(((c1 & 0b00000011) << 4) + ((c2 & 0b11110000) >> 4));
       jednostki[2] = kodowanie(((c2 & 0b00001111) << 2) + ((c3 & 0b11000000) >> 6));
       jednostki[3] = kodowanie(c3 & 0b00111111);

       kod.write(jednostki, 4);
       kod.flush();
    }

    if(braki == 1)
    {
        c1 = wiad.get();
        c2 = wiad.get();

        jednostki[0] = kodowanie((c1 & 0b11111100) >> 2);
        jednostki[1] = kodowanie(((c1 & 0b00000011) << 4) + ((c2 & 0b11110000) >> 4));
        jednostki[2] = kodowanie((c2 & 0b00001111) << 2);
        jednostki[3] = '=';

        kod.write(jednostki, 4);
        kod.flush();
    }
    else if(braki == 2)
    {
        c1 = wiad.get();
        jednostki[0] = kodowanie((c1 & 0b11111100) >> 2);
        jednostki[1] = kodowanie((c1 & 0b00000011) << 4);
        jednostki[2] = '=';
        jednostki[3] = '=';

        kod.write(jednostki, 4);
        kod.flush();
    }

    cout << "Wiadomosc zostala zakodowana" << endl;
    kod.close();
    wiad.close();
    system("pause");
    return 0;
}
