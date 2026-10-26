#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// ===================
// Parsowanie
// ===================

struct WynikParsowania {
    bool czySukces;
    bool czyZaDuza;
    int grosze;
};

WynikParsowania parsujKwote(string ciagZnakow) {
    
    int zlotowki = 0;
    int grosze = 0;
    
    bool kropkaZnaleziona = false;
    
    int licznikMiejscPoPrzecinku = 0;
    
    // Przecinek na kropka
    
    for (int i = 0; i < (int)ciagZnakow.length(); i++) {
        if (ciagZnakow[i] == ',') {
            ciagZnakow[i] = '.';
        }
    }

    // Walidacja
    
    for (int i = 0; i < (int)ciagZnakow.length(); i++) {
        
        char znak = ciagZnakow[i];
        
        if (znak == '.') {
            
            if (kropkaZnaleziona) { 
                
                return {false, false, 0}; 
            }
            
            kropkaZnaleziona = true;
            
        } else if (isdigit(znak)) {
            
            int cyfra = znak - '0';
            
            if (!kropkaZnaleziona) {
                
                // Przekroczenie int
                if (zlotowki > (21474836 - cyfra) / 10) {
                    
                    return {false, true, 0};
                }
                
                zlotowki = zlotowki * 10 + cyfra;
                
            } else {
                
                licznikMiejscPoPrzecinku++;
                
                // Max 2 miejsca po przecinku
                if (licznikMiejscPoPrzecinku > 2) { return {false, false, 0}; }
                
                if (licznikMiejscPoPrzecinku == 1) grosze += cyfra * 10;
                if (licznikMiejscPoPrzecinku == 2) grosze += cyfra;
            }
        } else {
            
            // Inne znaki
            return {false, false, 0};
        }
    }

    // Końcowa walidacja int (max 2147483647)
    if (zlotowki == 21474836 && grosze > 47) {
        
        return {false, true, 0};
    }

    int sumaGroszy = zlotowki * 100 + grosze;
    
    if (sumaGroszy < 0) {
        
        return {false, true, 0};
    }

    return {true, false, sumaGroszy};
}

// ===================
// Kwota
// ===================

void wypiszKwote(int grosze) {
    
    int zl = grosze / 100;
    int gr = grosze % 100;

    if (gr == 0) {
        
        cout << zl;
    
    } else {
        
        cout << zl << ",";
        
        if (gr < 10) {
            
            cout << "0";
        }
        
        cout << gr;
    }
}

// ===================
// Nonimaly
// ===================

vector<int> wybierzNominaly() {
    
    cout << "?> Wybierz źródło nominałów:" << endl;
    cout << "1> Użyj domyślnej tablicy nominałów" << endl;
    cout << "2> Wprowadź własne nominały z klawiatury" << endl;
    cout << "?> ";
    
    int wybor;
    
    while (!(cin >> wybor) || (wybor != 1 && wybor != 2)) {
        
        cout << endl << "!> [ERROR] Niepoprawny wybór." << endl << "?> Podaj 1 lub 2:" << endl << "?> ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    vector<int> nominaly;

    if (wybor == 1) {
        
        nominaly = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10, 5, 2, 1};
        cout << endl << ">> Wybrano domyślny zestaw nominałów." << endl;

    } else {
        
        cout << endl << "?> Podaj liczbe dostepnych nominalow: " << endl << "?> ";
        
        int iloscNominalow;
        
        while (!(cin >> iloscNominalow) || iloscNominalow < 0) {
            
            cout << endl << "!> [ERROR] Niepoprawna liczba." << endl << "?> Podaj liczbe calkowita >= 0: " << endl << "?> ";
            
            cin.clear();
            cin.ignore(10000, '\n');
        }

       if (iloscNominalow > 0) {
            
        cout << endl << "?> Podaj wartosci nominalow: " << endl;
            
            for (int i = 0; i < iloscNominalow; i++) {
                
                string wpisNominalu;
                int groszeNominalu = -1;
                
                while (true) {
                    
                    cout << "?> Nominal " << i + 1 << ": ";
                    cin >> wpisNominalu;
                    
                    WynikParsowania wynik = parsujKwote(wpisNominalu);
                    
                    if (wynik.czySukces) {
                        
                        if (wynik.grosze <= 0) {
                            
                            cout << endl << "!> [ERROR] Wartość nominału musi być większa od 0!" << endl;
                        
                        } else {
                            
                            groszeNominalu = wynik.grosze;
                            break;
                        }
                    } else if (wynik.czyZaDuza) {
                        
                        cout << endl << "!> [ERROR] Podana wartość nominału jest za duża!" << endl;
                    
                    } else {
                        
                        cout << endl << "!> [ERROR] Błędny format nominału!" << endl;
                    }
                }

                nominaly.push_back(groszeNominalu);
            }
        }
    }

    return nominaly;
}

// ===================
// Tablica
// ===================

void wypiszTablice(vector<int> nominaly) {
    
    cout << ">> Aktualna tablica nominalow: " << endl << ">> ";
    
    for (int i = 0; i < (int)nominaly.size(); i++) {
        
        wypiszKwote(nominaly[i]);
        
        if (i + 1 < (int)nominaly.size()) {
            
            cout << " ";
        }
    }

    cout << endl;
}

// ===================
// Algorytm zachłanny
// ===================

void wydajReszte(int reszta, vector<int> nominaly) {
    
    if (reszta == 0) {
        
        cout << endl << ">> Brak reszty do wydania (kwota wynosi 0)." << endl;
        return;
    }

    cout << endl << ">> Wydawana reszta: " << endl << ">> ";
    
    int indeksNominalu = 0;

    while (reszta > 0 && indeksNominalu < (int)nominaly.size()) {

        if (reszta >= nominaly[indeksNominalu]) {
            
            wypiszKwote(nominaly[indeksNominalu]);
                
            cout << " ";
            
            reszta -= nominaly[indeksNominalu];

        } else {

            indeksNominalu++;
        }
    }
    
    cout << endl;

    if (reszta > 0) {
        
        cout << endl << "!> [ERROR] Nie udalo sie wydac calej reszty. Pozostalo do wydania: " << endl << "!> ";
        
        wypiszKwote(reszta);

        cout << endl;
    }
}

// ===================
// Program
// ===================

int main() {

    vector<int> nominaly = wybierzNominaly();

    if (nominaly.empty()) {
        
        cout << endl << "!> [ERROR] Tablica nominalow jest pusta! Nie mozna wydac reszty." << endl;
        return 0;
    }

    sort(nominaly.rbegin(), nominaly.rend());

    cout << endl;
    
    wypiszTablice(nominaly);

    string wpisKlawiatury;
    
    int reszta = -1;

    cout << endl << "?> Podaj kwote reszty: " << endl << "?> ";

    while (true) {
        
        cin >> wpisKlawiatury;
        
        WynikParsowania wynik = parsujKwote(wpisKlawiatury);
        
        if (wynik.czySukces) {
            
            reszta = wynik.grosze;
            break;
            
        } else if (wynik.czyZaDuza) {
            
            cout << endl << "!> [ERROR] Podana liczba jest za duza!" << endl;
            cout << "?> Podaj ponownie kwote: " << endl << "?> ";
            
        } else {
            
            cout << endl << "!> [ERROR] Bledna kwota (niepoprawny format)!" << endl;
            cout << "?> Podaj ponownie kwote: " << endl << "?> ";
        }
    }

    wydajReszte(reszta, nominaly);

    cout << endl << ">> Dziekujemy, to wszystko." << endl << ">> Przyjdź jutro znowu bo mamy promocje!";

    return 0;
}