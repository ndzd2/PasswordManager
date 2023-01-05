/*! \brief     Menad¿er hase³
 *  \author    Patryk NiedŸwiecki
 *  \version   1.1
 *
 *  Program stanowi przyk³ad systemu zarz¹dzania has³ami.
 *  Wszystkie has³a s¹ szyfrowane jednym has³em g³ównym.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>

 /*! \brief Deklaracja funkcji
  */
class Haslo;

/*! \brief Prototypy funkcji
 */
bool pusty_plik(std::ifstream&);
std::string szyfrujHaslo(std::string);
std::string odszyfrujHaslo(std::string);
void wyszukaj(const std::vector<Haslo*>*);
void dodajHaslo(std::vector<Haslo*>*, std::vector<std::string>*);
void usunHaslo(std::vector<Haslo*>*);
void edytujHaslo(std::vector<Haslo*>*, std::vector<std::string>*);
std::string dodajKategorie(std::vector<std::string>*);
void usunKategorie(std::vector<std::string>*, std::vector<Haslo*>*);
void sortuj(std::vector<Haslo*>*);
int menu();

/*! \class Haslo
    \brief Klasa przechowuj¹ca informacje o haœle.

    Ta klasa podlega hermetyzacji stosujac pola prywatne w celu zmniejszenia do nich dostepu.
    Posiada 3 konstruktory z czego 2 parametrowe a jeden domyslny
*/
class Haslo {
private:
    std::string nazwa;               //!< Parametr obowiazkowy - nazwa hasla
    std::string haslo;               //!< Parametr obowiazkowy - haslo
    std::string kategoria;           //!< Parametr obowiazkowy - kategoria danego hasla
    std::string strona_internetowa;  //!< Parametr opcjonalny  - strona internetowa do danego hasla
    std::string nazwa_uzytkownika;   //!< Parametr opcjonalny  - nazwa uzytkownika dla danego hasla

public:
    Haslo() {
        this->nazwa = "";
        this->haslo = "";
        this->kategoria = "";
        this->strona_internetowa = "";
        this->nazwa_uzytkownika = "";
    }

    Haslo(std::string nazwa, std::string kategoria, std::string strona_internetowa, std::string nazwa_uzytkownika) {
        this->nazwa = nazwa;
        this->haslo = generatorHasla();
        this->kategoria = kategoria;
        this->strona_internetowa = strona_internetowa;
        this->nazwa_uzytkownika = nazwa_uzytkownika;
    }

    Haslo(std::string nazwa, std::string haslo, std::string kategoria, std::string strona_internetowa, std::string nazwa_uzytkownika) {
        this->nazwa = nazwa;
        this->haslo = haslo;
        this->kategoria = kategoria;
        this->strona_internetowa = strona_internetowa;
        this->nazwa_uzytkownika = nazwa_uzytkownika;
    }


    std::string getNazwa() {
        return this->nazwa;
    }

    void setNazwa(std::string nazwa) {
        this->nazwa = nazwa;
    }

    std::string getHaslo() {
        return this->haslo;
    }

    void setHaslo(std::string haslo) {
        this->haslo = haslo;
    }

    void setHasloLosowe() {
        this->haslo = generatorHasla();
    }

    std::string getKategoria() {
        return this->kategoria;
    }

    void setKategoria(std::string kategoria) {
        this->kategoria = kategoria;
    }

    std::string getStronaInternetowa() {
        return this->strona_internetowa;
    }

    void setStronaInternetowa(std::string strona_internetowa) {
        this->strona_internetowa = strona_internetowa;
    }

    std::string getNazwaUzytkownika() {
        return this->nazwa_uzytkownika;
    }

    void setNazwaUzytkownika(std::string nazwa_uzytkownika) {
        this->nazwa_uzytkownika = nazwa_uzytkownika;
    }

    /*! \fn string generatorHasla()
        \brief Generator nowego has³a na podstawie preferencji u¿ytkownika
    */
    std::string generatorHasla() {
        int dlugosc, n;
        std::string znaki = "abcdefghijklmnopqrstuvwxyz";
        std::string haslo = "";
        srand(time(0));  //losowe znaki

        std::cout << "Podaj dlugosc hasla: ";
        std::cin >> dlugosc;
        while (dlugosc < 6) {
            std::cout << "Haslo nie powinno zawierac mniej niz 6 znakow.\nPodaj dlugosc hasla: ";
            std::cin >> dlugosc;
        }

        std::cout << "Czy haslo moze zawierac duze litery:\n1. Tak\n2. Nie\nPodaj numer (1|2): ";
        std::cin >> n;
        if (n == 1) {
            znaki += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }

        std::cout << "Czy haslo moze zawierac znaki specjalne:\n1. Tak\n2. Nie\nPodaj numer (1|2): ";
        std::cin >> n;
        if (n == 1) {
            znaki += "?!@&#^+=-$*";
        }


        for (int i = 0; i < dlugosc; i++) {
            haslo += znaki[rand() % znaki.length()];
        }
        return haslo;
    }

    std::string to_string() {
        return nazwa + "\n" + haslo + "\n" + kategoria + "\n" + strona_internetowa + "\n" + nazwa_uzytkownika + "\n";
    }

    std::string to_string_zaszyfrowany() {
        return szyfrujHaslo(nazwa) + "\n" + szyfrujHaslo(haslo) + "\n" + szyfrujHaslo(kategoria) + "\n" + szyfrujHaslo(strona_internetowa) + "\n" + szyfrujHaslo(nazwa_uzytkownika);
    }
};

int main() {
    bool pierwsze_uruchomienie;
    std::vector<Haslo*>* hasla = new std::vector<Haslo*>();
    std::vector<std::string>* kategorie = new std::vector<std::string>();
    std::string nazwa_pliku, haslo, wiersz;
    std::cout << "Podaj nazwe pliku do odszyfrowania: ";
    std::cin >> nazwa_pliku;
    std::cout << "Podaj haslo aby odszyfrowac plik: ";
    std::cin >> haslo;

    std::ifstream plik_wejsciowy(nazwa_pliku);
    if (!plik_wejsciowy.is_open()) {
        std::cout << "Nie udalo sie otworzyc tego pliku." << std::endl;
        return -1;
    }

    getline(plik_wejsciowy, wiersz);
    if (szyfrujHaslo(haslo) != wiersz && !pusty_plik(plik_wejsciowy)) {
        std::cout << "B³êdne has³o." << std::endl;
        return -1;
    }
    else {
        while (getline(plik_wejsciowy, wiersz)) {
            hasla->push_back(new Haslo());
            hasla->at(hasla->size() - 1)->setNazwa(odszyfrujHaslo(wiersz));
            getline(plik_wejsciowy, wiersz);
            hasla->at(hasla->size() - 1)->setHaslo(odszyfrujHaslo(wiersz));
            getline(plik_wejsciowy, wiersz);
            hasla->at(hasla->size() - 1)->setKategoria(odszyfrujHaslo(wiersz));
            kategorie->push_back(odszyfrujHaslo(wiersz));
            getline(plik_wejsciowy, wiersz);
            hasla->at(hasla->size() - 1)->setStronaInternetowa(odszyfrujHaslo(wiersz));
            getline(plik_wejsciowy, wiersz);
            hasla->at(hasla->size() - 1)->setNazwaUzytkownika(odszyfrujHaslo(wiersz));
        }
    }

    bool flaga = true;
    while (flaga) {
        switch (menu()) {
        case 1: wyszukaj(hasla);
            break;
        case 2: sortuj(hasla);
            break;
        case 3: dodajHaslo(hasla, kategorie);
            break;
        case 4: edytujHaslo(hasla, kategorie);
            break;
        case 5: usunHaslo(hasla);
            break;
        case 6: dodajKategorie(kategorie);
            break;
        case 7: usunKategorie(kategorie, hasla);
            break;
        case 8:
        default:flaga = false;
            break;
        }
    }

    if (pusty_plik(plik_wejsciowy)) {
        pierwsze_uruchomienie = true;
    }
    else {
        pierwsze_uruchomienie = false;
    }
    plik_wejsciowy.close();

    std::ofstream plik_wynikowy(nazwa_pliku);
    if (!plik_wynikowy.is_open()) {
        std::cout << "Nie udalo sie otworzyc tego pliku." << std::endl;
        return -1;
    }

    if (pierwsze_uruchomienie) {
        plik_wynikowy << szyfrujHaslo(haslo) << "\n";
    }

    for (int i = 0; i < hasla->size(); i++) {
        plik_wynikowy << hasla->at(i)->to_string_zaszyfrowany();
    }

    plik_wynikowy.close();
    return 0;
}

/*! \fn int menu()
 *    \brief Funkcja wyœwietlaj¹ca menu g³ówne programu.
 *    umo¿liwia wybranie opcji która ma zostaæ uruchomiona.
 */
int menu() {
    int opcja;
    std::cout << "MENU G£ÓWNE" << std::endl;

    std::cout << "1. Wyszukaj Haslo" << std::endl;
    std::cout << "2. Posortuj Hasla" << std::endl;
    std::cout << "3. Dodaj nowe Haslo" << std::endl;
    std::cout << "4. Edytuj Haslo" << std::endl;
    std::cout << "5. Usun Haslo" << std::endl;
    std::cout << "6. Dodaj nowa Kategorie" << std::endl;
    std::cout << "7. Usun Kategorie" << std::endl;
    std::cout << "8. Wyjdz" << std::endl;

    std::cout << "Wybierz opcje: ";
    std::cin >> opcja;
    while (opcja < 1 || opcja > 8) {
        std::cout << "Nie ma takiej opcji...\nWybierz opcje: ";
        std::cin >> opcja;
    }
    return opcja;
}

/*! \fn bool pusty_plik(ifstream& plik)
    \brief Sprawdza czy podany plik jest pusty
    \param plik opisuje plik otwarty w trybie odczytu.
*/
bool pusty_plik(std::ifstream& plik) {
    return plik.peek() == std::ifstream::traits_type::eof();
}

/*! \fn string szyfrujHaslo(string zawartosc)
    \brief Szyfruje haslo (lub inne wyrazenie) przekazane przez zmienna wiadomosc
    \param wiadomosc opisuje stringa ktory ma zostac zaszyfrowany.
           Ten string zostanie zaszyfrowany poprzez krok zwiekszajacy sie w trakcie iteracji.
           W zaleznosci od dlugosci stringa 'wiadomosc', kazdy znak zostanie zaszyfrowany
           o pozycje zwiekszajaca sie i zalezna od wartosci zmiennej j.
*/
std::string szyfrujHaslo(std::string wiadomosc) {
    std::string wynik = "";
    // ABC -> A + 1  B + 2  C + 3  -> BDF
    for (int i = 0, j = 1; i < wiadomosc.length(); i++, j++) {
        wynik += char(wiadomosc[i] + j);
    }
    return wynik;
}

/*! \fn string odszyfrujHaslo(string wiadomosc)
    \brief Odszyfrowuje haslo (lub inne wyrazenie) przekazane przez zmienna wiadomosc
    \param wiadomosc opisuje stringa ktory ma zostac odszyfrowany.
           Ten string zostanie odszyfrowany poprzez krok zwiekszajacy sie w trakcie iteracji.
           W zaleznosci od dlugosci stringa 'wiadomosc', kazdy znak zostanie odszyfrowany
           o pozycje zwiekszajaca sie i zalezna od wartosci zmiennej j.
*/
std::string odszyfrujHaslo(std::string wiadomosc) {
    std::string wynik = "";
    // BDF -> B - 1  D - 2  F - 3  -> ABC
    for (int i = 0, j = 1; i < wiadomosc.length(); i++, j++) {
        wynik += char(wiadomosc[i] - j);
    }
    return wynik;
}

/*! \fn void wyszukaj(const vector<Haslo*>* hasla)
    \brief Pozwala wyszukiwac nowe haslo w zaleznosci od wybranego parametru wyszukiwania
    \param hasla to zbior instacji klasy Haslo ktore pozwala zebrac w intuicyjny sposob
           wszystkie wczytane hasla.
*/
void wyszukaj(const std::vector<Haslo*>* hasla) {
    int opcja;
    std::string wybor;

    std::cout << std::endl;

    std::cout << "Wybierz po czym szukac haslo:\n1. Nazwa\n2. Kategoria\n3. Strona\n4. Login\nWybierz opcje: ";
    std::cin >> opcja;
    while (opcja < 1 || opcja > 4) {
        std::cout << "Wybierz po czym szukac haslo:\n1. Nazwa\n2. Kategoria\n3. Strona\n4. Login\nWybierz opcje: ";
        std::cin >> opcja;
    }
    std::cout << "Wprowadz wartosc wybranego parametru: ";
    std::cin >> wybor;

    for (int i = 0; i < hasla->size(); i++) {
        switch (opcja) {
        case 1: if (hasla->at(i)->getNazwa() == wybor)
            std::cout << hasla->at(i)->to_string();
            break;
        case 2: if (hasla->at(i)->getKategoria() == wybor)
            std::cout << hasla->at(i)->to_string();
            break;
        case 3: if (hasla->at(i)->getStronaInternetowa() == wybor)
            std::cout << hasla->at(i)->to_string();
            break;
        case 4: if (hasla->at(i)->getNazwaUzytkownika() == wybor)
            std::cout << hasla->at(i)->to_string();
            break;
        }
    }
}

/*! \fn void dodajHaslo(vector<Haslo*>* hasla, vector<string>* kategorie)
    \brief Pozwala dodawac nowe haslo (losowe lub nie) do bazy danych
    \param hasla to zbior instacji klasy Haslo ktore pozwala zebrac w intuicyjny sposob
           wszystkie wczytane hasla.
    \param kategorie to zbior stringow kategorii miedzy ktorymi mozna wybrac kategorie dla nowego hasla.
*/
void dodajHaslo(std::vector<Haslo*>* hasla, std::vector<std::string>* kategorie) {
    std::string nazwa, nowe_haslo, kategoria, strona, login;
    int wybor;

    std::cout << "Podaj nazwe dla nowego hasla: " << std::endl;
    std::cin >> nazwa;

    std::cout << "1. Generuj losowe haslo" << std::endl;
    std::cout << "2. Wprowadz recznie haslo" << std::endl;
    std::cout << "Twoj wybor: ";
    std::cin >> wybor;
    while (wybor != 1 and wybor != 2) {
        std::cout << "Wybierz ponownie opcje (1|2)\nTwoj wybor: ";
        std::cin >> wybor;
    }

    Haslo* haslo = new Haslo();
    haslo->setNazwa(nazwa);
    if (wybor == 1) {
        haslo->setHasloLosowe();
    }
    else {
        std::cout << "Wprowadz haslo: ";
        std::cin >> nowe_haslo;
        while (nowe_haslo.length() < 4) {
            std::cout << "Haslo jest zbyt krotkie.\nWprowadz haslo: ";
            std::cin >> nowe_haslo;
        }

        if (nowe_haslo.length() < 8) {
            std::cout << "Haslo jest malo bezpieczne." << std::endl;
        }
        else if (nowe_haslo.length() < 12) {
            std::cout << "Haslo jest srednio bezpieczne." << std::endl;
        }
        else {
            std::cout << "Haslo jest bardzo bezpieczne." << std::endl;
        }
        haslo->setHaslo(nowe_haslo);
    }

    if (kategorie->size() == 0) {
        kategoria = dodajKategorie(kategorie);
    }
    else {
        for (int i = 0; i < kategorie->size(); i++) {
            std::cout << i + 1 << ". " << kategorie->at(i) << std::endl;
        }
        std::cout << "Twoj wybor: ";
        std::cin >> wybor;
        while (wybor < 1 || wybor > kategorie->size()) {
            std::cout << "Wybierz kategorie [1-" << kategorie->size() << "]: ";
            std::cin >> wybor;
        }
        kategoria = kategorie->at(wybor - 1);
    }
    haslo->setKategoria(kategoria);

    int opcja;
    std::cout << "Dodaj strone internetowa:\n1. Tak\n2. Nie\nTwoj wybor: ";
    std::cin >> opcja;
    if (opcja == 1) {
        std::cout << "Podaj strone internetowa: ";
        std::cin >> strona;
    }
    else {
        strona = "";
    }
    haslo->setStronaInternetowa(strona);

    std::cout << "Dodaj login:\n1. Tak\n2. Nie\nTwoj wybor: ";
    std::cin >> opcja;
    if (opcja == 1) {
        std::cout << "Podaj login: ";
        std::cin >> login;
    }
    else {
        login = "";
    }
    haslo->setNazwaUzytkownika(login);

    hasla->push_back(haslo);
}

/*! \fn void usunHaslo(vector<Haslo*>* hasla)
    \brief Usuwa haslo o podanej nazwie
    \param hasla to zbior instacji klasy Haslo ktore pozwala zebrac w intuicyjny sposob
           wszystkie wczytane hasla.
*/
void usunHaslo(std::vector<Haslo*>* hasla) {
    std::string nazwa_do_usuniecia;
    int wybor;

    std::cout << "Wprowadz nazwe hasla ktorego chcesz usunac: ";
    std::cin >> nazwa_do_usuniecia;
    std::cout << "Jesteœ pewien?\n1. Tak\n2. Nie\nTwoj wybor: ";
    std::cin >> wybor;
    if (wybor == 2) {
        return;
    }

    int rozmiar = hasla->size();
    int usuniete = 0;
    for (int i = 0; i < rozmiar; i++) {
        if (hasla->at(i)->getNazwa() == nazwa_do_usuniecia) {
            hasla->erase(hasla->begin() + i);
            usuniete++;
            i--;
            rozmiar--;
        }
    }

    if (usuniete > 1) {
        std::cout << "Hasla zostaly usuniete." << std::endl;
    }
    else if (usuniete == 1) {
        std::cout << "Haslo zostalo usuniete." << std::endl;
    }
    else {
        std::cout << "Nie ma hasla z taka nazwa." << std::endl;
    }
}

/*! \fn void edytujHaslo(vector<Haslo*>* hasla, vector<string>* kategorie)
    \brief Pozwala edytowac haslo ktore ma podana nazwe. Mozna wybrac parametr ktory ma podlegac edycji.
    \param hasla to zbior instacji klasy Haslo ktore pozwala zebrac w intuicyjny sposob
           wszystkie wczytane hasla.
    \param kategorie to zbior stringow kategorii miedzy ktorymi mozna wybrac kategorie dla nowego hasla.
*/
void edytujHaslo(std::vector<Haslo*>* hasla, std::vector<std::string>* kategorie) {
    std::string nazwa;
    int wybor;
    std::string nowa_nazwa, nowe_haslo, nowa_strona_internetowa, nowa_nazwa_uzytkownika;

    std::cout << "Nazwa hasla ktory ma podlegac edycji: ";
    std::cin >> nazwa;
    for (int i = 0; i < hasla->size(); i++) {
        if (hasla->at(i)->getNazwa() == nazwa) {
            std::cout << "Zmien:\n1. Nazwe\n2. Haslo\n3. Kategoria\n4. Strona\n5. Login\nTwoj wybor: ";
            std::cin >> wybor;
            while (wybor < 1 || wybor > 5) {
                std::cout << "Zmien:\n1. Nazwe\n2. Haslo\n3. Kategoria\n4. Strona\n5. Login\nTwoj wybor: ";
                std::cin >> wybor;
            }
            if (wybor == 1) {
                std::cout << "Podaj nowa nazwe: ";
                std::cin >> nowa_nazwa;
                hasla->at(i)->setNazwa(nowa_nazwa);
            }
            else if (wybor == 2) {
                std::string test_hasla;
                std::cout << "Wprowadz poprzednie haslo: ";
                std::cin >> test_hasla;
                if (test_hasla == hasla->at(i)->getHaslo()) {
                    std::cout << "Podaj nowe haslo: ";
                    std::cin >> nowe_haslo;
                    hasla->at(i)->setHaslo(nowe_haslo);
                }
                else {
                    std::cout << "Zle haslo." << std::endl;
                }
            }
            else if (wybor == 3) {
                for (int i = 0; i < kategorie->size(); i++) {
                    std::cout << i + 1 << ". " << kategorie->at(i) << std::endl;
                }
                std::cout << " > ";
                std::cin >> wybor;
                while (wybor < 1 || wybor > kategorie->size()) {
                    std::cout << "Nie ma takiej opcji.\nWybierz kategorie [1-" << kategorie->size() << "]: ";
                    std::cin >> wybor;
                }
                hasla->at(i)->setKategoria(kategorie->at(wybor - 1));
            }
            else if (wybor == 4) {
                std::cout << "Podaj nowa strone: ";
                std::cin >> nowa_strona_internetowa;
                hasla->at(i)->setStronaInternetowa(nowa_strona_internetowa);
            }
            else {
                std::cout << "Podaj nowy login: ";
                std::cin >> nowa_nazwa_uzytkownika;
                hasla->at(i)->setNazwaUzytkownika(nowa_nazwa_uzytkownika);
            }
            return;
        }
    }
    std::cout << "Nie ma hasla z taka nazwa." << std::endl;
}

/*! \fn string dodajKategorie(vector<string>* kategorie)
    \brief Ta funkcja dodaje nowa kategorie do wektora wskaznikow do kategorii
    \param kategorie to zbior stringow kategorii miedzy ktorymi mozna wybrac kategorie dla nowego hasla.
*/
std::string dodajKategorie(std::vector<std::string>* kategorie) {
    std::string kategoria;
    std::cout << "Wprowadz nowa kategorie: ";
    std::cin >> kategoria;
    kategorie->push_back(kategoria);
    return kategoria;
}

/*! \fn void usunKategorie(vector<string>* kategorie, vector<Haslo*>* hasla)
    \brief Usuwa wybrana kategorie razem ze wszystkimi haslami nalezacymi do tej kategorii
    \param hasla to zbior instacji klasy Haslo ktore pozwala zebrac w intuicyjny sposob
           wszystkie wczytane hasla.
    \param kategorie to zbior stringow kategorii miedzy ktorymi mozna wybrac kategorie dla nowego hasla.
*/
void usunKategorie(std::vector<std::string>* kategorie, std::vector<Haslo*>* hasla) {
    int wybor;
    for (int i = 0; i < kategorie->size(); i++) {
        std::cout << i + 1 << ". " << kategorie->at(i) << std::endl;
    }
    std::cout << "Kategoria do usuniecia: ";
    std::cin >> wybor;
    while (wybor < 1 || wybor > kategorie->size()) {
        std::cout << "Kategoria do usuniecia [1-" << kategorie->size() << "]: ";
        std::cin >> wybor;
    }

    int rozmiar = hasla->size();
    for (int i = 0; i < rozmiar; i++) {
        if (hasla->at(i)->getKategoria() == kategorie->at(wybor - 1)) {
            hasla->erase(hasla->begin() + i);
            i--;
            rozmiar--;
        }
    }
    kategorie->erase(kategorie->begin() + wybor - 1);
}


/*! \fn void sortuj(vector<Haslo*>* hasla)
    \brief Sortuje hasla wedlug nazwy i kategorii (np. kiedy nazwa jest taka sama)
    \param hasla to zbior instacji klasy Haslo ktore pozwala zebrac w intuicyjny sposob
           wszystkie wczytane hasla.
*/
void sortuj(std::vector<Haslo*>* hasla) {
    for (int i = 0; i < hasla->size() - 1; i++) {
        for (int j = i + 1; j < hasla->size(); j++) {
            if (hasla->at(i)->getNazwa() > hasla->at(j)->getNazwa()) {
                Haslo* temp = hasla->at(j);
                hasla->at(j) = hasla->at(i);
                hasla->at(i) = temp;
            }
            else if (hasla->at(i)->getKategoria() > hasla->at(j)->getKategoria()) {
                Haslo* temp = hasla->at(j);
                hasla->at(j) = hasla->at(i);
                hasla->at(i) = temp;
            }
        }
    }
}