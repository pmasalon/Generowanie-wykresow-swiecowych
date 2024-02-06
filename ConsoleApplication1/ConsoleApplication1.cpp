#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>

using namespace std;

double konw_do_double(char* wsk) // wskaźnik umożliwia dostęp do kolejnych znaków w ciągu znaków które są przekazywane bez koniecz tworzenia kopii danych
{
    double wynik = 0.0;
    int calkowita = 0; //będzie przechowywać część całkowitą liczby
    double dziesietna = 0.0; //będzie przechowywać część dziesiętną liczby
    int m_dziesietnej = 0; //będzie przechowywać liczbę miejsc dziesiętnych

    // konwersja części całkowitej
    while (*wsk != '.' && *wsk != '\0') //wykonuje się do az nie jest kropką lub znakiem końca ciągów
    {
        calkowita = calkowita * 10 + (*wsk - '0');//przesunięcie wartości calkowita o jedną pozycję dziesiętną w lewo i dodanie aktualnej cyfry (zamienionej na liczbę) z ciągu znaków
        wsk++;//przesuniecie wskaznika na nastepny znak
    }

    // konwersja części dziesiętnej
    if (*wsk == '.') // czy znak to kropka
    {
        wsk++;//przesuniecie wskaznika na nastepny znak
        while (*wsk != '\0') // wykonuje się az nie jest znakiem końca znaków
        {
            dziesietna = dziesietna * 10 + (*wsk - '0'); //przesunięcie wartości dziesietna o jedną pozycję dziesiętną w lewo i dodanie aktualnej cyfry (zamienionej na liczbę) z ciągu znaków
            wsk++;//przesuniecie wskaznika na nastepny znak
            m_dziesietnej++;//dodanie +1 do licznika
        }
    }

    // sklejanie wyniku
    wynik = static_cast<double>(calkowita) + dziesietna / pow(10, m_dziesietnej); //część całkowita jest dodawana do części dziesiętnej podzielonej przez 10^ liczba miejsc dziesiętnych

    return wynik;
}

double zaokr(double l)
{
    double k = l;
    k = k * 100;
    int s = round(k);
    s = s % 100;
    if (s > 25 || s < 75) l = floor(l) + 0.5;
    else l = round(l);
    return l;
}

int main() {
    cout << "pobierz dane - g -\n";
    cout << "zamknij program - q -\n";
    char znak;
    cin >> znak;
    if (znak == 'q') return 0;

    ifstream plik("intc_us_data.csv");

    if (!plik.is_open())
    {
        cout << "BLAD PLIKU" << endl;
        return 0;
    }

    string date[200];
    int id[200];
    double open[200], high[200], low[200], close[200]; //deklar zmiennych do przech liczb

    for (int i = 0; i < 5816; i++) // ominięcie niepotrzebnych rekordow
    {
        char temp[200];
        plik.getline(temp, 200, '\n');
    }

    // wczytanie danych
    for (int i = 0; i < 200; i++)
    {
        char date_k[20], open_k[20], high_k[20], low_k[20], close_k[20], volume[20];

        // wczytanie linii z pliku
        plik.getline(date_k, 20, ',');
        plik.getline(open_k, 20, ',');
        plik.getline(high_k, 20, ',');
        plik.getline(low_k, 20, ',');
        plik.getline(close_k, 20, ',');
        plik.getline(volume, 20, '\n');


        id[i] = i + 1;
        date[i] = date_k;
        // konwersja pól na double
        open[i] = konw_do_double(open_k);
        high[i] = konw_do_double(high_k);
        low[i] = konw_do_double(low_k);
        close[i] = konw_do_double(close_k);

        cout << "id: " << id[i] << " Date: " << date[i] << ", Open : " << open[i] << ", High : " << high[i]
            << ", Low: " << low[i] << ", Close: " << close[i] << endl;
    }

    plik.close();

    char tablica[50][200];
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 200; j = j++)
        {
            tablica[i][j] = ' ';
        }
    }
    for (int i = 0; i < 200; i++)
    {
        double k = 50;
        for (int j = 0; j < 50; j = j++)
        {
            if (zaokr(high[i]) >= k && zaokr(low[i]) <= k) // czy k <= od high i czy k >= od low
            {
                //cout << i << " " << k << " " << zaokr(high[i]) << " " << zaokr(low[i]) << "\n";
                if (zaokr(high[i]) == k) //czy k = high
                {
                    if (zaokr(high[i]) != zaokr(close[i]) && zaokr(high[i]) != zaokr(open[i])) tablica[j][i] = '|';
                    else if (zaokr(high[i]) == zaokr(close[i])) tablica[j][i] = 'O';
                    else if (zaokr(high[i]) == zaokr(open[i])) tablica[j][i] = '#';
                }
                else
                if (zaokr(low[i]) == k) // czy k = low
                {
                    if (zaokr(low[i]) != zaokr(close[i]) && zaokr(low[i]) != zaokr(open[i])) tablica[j][i] = '|';
                    else if (zaokr(low[i]) == zaokr(close[i])) tablica[j][i] = '#';
                    else if (zaokr(low[i]) == zaokr(open[i])) tablica[j][i] = 'O';
                }
                else
                if (k > zaokr(close[i]) && k > zaokr(open[i])) tablica[j][i] = '|'; // k > od close i open
                else if (k <= zaokr(close[i]) && zaokr(close[i]) > zaokr(open[i]) && k >= zaokr(open[i])) tablica[j][i] = 'O';
                else if (k <= zaokr(open[i]) && zaokr(close[i]) < zaokr(open[i]) && k >= zaokr(close[i])) tablica[j][i] = '#';
                else if (k < zaokr(close[i]) && k < zaokr(open[i])) tablica[j][i] = '|';              
            }
            else tablica[j][i] = ' ';
            k = k - 0.5;
            
        }
        
    }
    ofstream zapis;
    zapis.open("chart.txt", ios::out | ios::trunc);
    if (zapis.good() == true)
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 200; j = j++)
        {
            zapis << tablica[i][j];
            zapis << " ";
        }
        zapis << "\n";
    }
    return 0;
}
