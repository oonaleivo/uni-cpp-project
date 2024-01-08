#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>


/* Kirjasto
 *
 * Kuvaus:
 *   ohjelma, joka lukee käynnistyessään kirjastojen kokoelmiin liittyviä tietoja tiedostosta,
 * tallentaa ne sopivaan tietorakenteeseen (map) ja antaa käyttäjälle mahdollisuuden tehdä hakuja
 * kyseiseen tietorakenteeseen.
 *   Ohjelma kysyy ensin tiedostoa ja avaa sen. Tiedoston tiedot tallennetaan tietorakenteeseen.
 * Jos tiedostoa ei pysty avaamaan, tuottaa ohjelma errorin ja päättää ohjelman.
 *   Käyttäjä voi tehdä kirjastoon hakuja käyttäen eri hakusanoja, jotka ovat
 * ohjelmassa määritelty. Eri haut tulostavat tietoja käyttäjälle.
 *   Väärä hakusana tai väärä määrä hakusanoja tuottaa errorin.
 *
 * Ohjelman kirjoittaja
 * Nimi: Oona Leivo
 * Opiskelijanumero: 150872052
 * Käyttäjätunnus: s8oleivo
 * E-Mail: oona.leivo@tuni.fi
 *
 * */

using namespace std;

struct Book
{
    string author;
    string title;
    int reservations;
};

using Libraries =  map<string, vector<struct Book>>;

std::vector<std::string> split(const std::string& s, const char delimiter, bool ignore_empty = false){
    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}

int read_file(Libraries &lib, string const &tiedoston_nimi) {
    // Avaa annetun tiedoston ja lukee sen tiedot ja tallentaa ne tietorakenteeseen
    // Tuottaa errorin, jos kyseistä tiedostoa ei ole olemassa tai se on vääränlainen

    ifstream tiedosto_olio(tiedoston_nimi);
    if (!tiedosto_olio) {
          cout << "Error: input file cannot be opened" << endl;
          return 1;
        }
    string rivi;
    while (getline(tiedosto_olio, rivi)) {
        vector<string> strings = split(rivi, ';', true);

        int vector_size = strings.size();
        if (vector_size != 4) {
            cout << "Error: empty field" << endl;
            return 1;
        }
        string library = strings.at(0),
                author = strings.at(1),
                title = strings.at(2);
        int reservations;

        if (strings.at(3) == "on-the-shelf") {
            reservations = 0;
        }

        else {
            reservations = stoi(strings.at(3));
        }

        // jos kirjastoa ei ole vielä mapissa
        if (lib.find(library) == lib.end()){
            lib.insert({library, {}});
        }
        // kirjasto on jo mapissa, lisätään uusi kirja
        lib.at(library).push_back({author, title, reservations});
    }
    tiedosto_olio.close();
    return 0;
}

void print_materials(Libraries &lib, string const &library_name) {
    // Tulostaa aakkosjärjestyksessä allekkain kaikki annetun kirjaston kirjat.

    if (lib.find(library_name) == lib.end()) {
        cout << "Error: unknown library" << endl;
        return;
    }
    else {
        map<string, vector<string>> books;

        for (Book book : lib.at(library_name)) {
            books.insert({book.author, {}});
            books.at(book.author).push_back((book.title));
        }
        for (auto author : books) {
            for (auto title : author.second) {
                cout << author.first << ": " << title << endl;
            }
        }
    }
}

void print_books(Libraries &lib, string const &library_name, string const &author_name) {
    // tulostaa kirjan nimen mukaisessa aakkosjärjestyksessä kaikki kyseisen
    // tekijän kirjat, jotka löytyvät kyseisestä kirjastosta, sekä merkkijonon “---” ja kirjojen varaustiedot

    if (lib.find(library_name) == lib.end()) {
        cout << "Error: unknown library" << endl;
        return;
    }
    bool author_found = false;
    for (Book book : lib.at(library_name)) {
        if (book.author == author_name) {
            author_found = true;
            break;
        }
    }
    if (!author_found) {
        cout << "Error: unknown author" << endl;
        return;
    }
    map<string, int> book_author;
    for (Book book : lib.at(library_name)) {
        if (book.author == author_name) {
            book_author.insert({book.title, book.reservations});
        }
    }
    for (auto book : book_author) {
        if (book.second == 0) {
            cout << book.first << " --- on the shelf" << endl;
        }
        else {
        cout << book.first << " --- " << book.second << " reservations" << endl;
        }
    }
}

string get_book_title(string const &input, vector<string> inputs) {
    // erottaa input-parametristä kirjan nimen ja palauttaa sen stringinä

    string first_word = inputs.at(2);
    string title = input.substr(input.find(first_word));
    // jos kirjan nimi on ""-merkkien sisällä
    if (title.find('"') != string::npos) {
        title = title.substr(title.find('"')+1);
        title = title.substr(0, title.rfind('"'));
    }
    return title;
}

void print_reservable_books(Libraries &lib, string const &author_name, string const &book_title) {
    // Komento tulostaa lyhyimmän varausjonon halutun tekijän halutulle kirjalle
    // ja sen alle aakkosjärjestyksessä kirjastot, joista kirja löytyy tällä samanpituisella varausjonolla

    bool author_found = false;
    for (auto library : lib) {
        for (Book book : library.second) {
            if (book.author == author_name) {
                author_found = true;
                break;
            }
        }
    }
    if (!author_found) {
        cout << "Book is not a library book" << endl;
        return;
    }
    set<string> libraries;
    int shortest_line = 0;
    for (auto library_name : lib) {
        for (Book book : library_name.second) {
            if ((author_name == book.author) && (book_title == book.title)) {
                if (shortest_line == 0) {
                    shortest_line = book.reservations;
                    libraries.insert(library_name.first);
                }
                else if (shortest_line == book.reservations) {
                    libraries.insert(library_name.first);
                }
                else if (shortest_line > book.reservations) {
                    shortest_line = book.reservations;
                    libraries.clear();
                    libraries.insert(library_name.first);
                }
            }
        }
    }
    if (shortest_line >= 100) {
        cout << "Book is not reservable from any library" << endl;
        return;
    }

    else if ((shortest_line == 0) && (libraries.size() >= 1)) {
        cout << "on the shelf" << endl;
    }
    else if (libraries.size() >= 1) {
        cout << shortest_line << " reservations" << endl;
    }
    else {
        cout << "Book is not a library book" << endl;
        return;
    }
    for (string name : libraries) {
        cout << "--- " << name << endl;
    }
}

void print_loanable_books(Libraries &lib) {
    // Ohjelma tulostaa näytölle kaikkien kirjastojen kaikista kirjoista ne, jotka löytyvät hyllystä.
    // Kirjoista tulostetaan tekijä ja nimi aakkosjärjestyksessä
    // ensin tekijöiden mukaan ja sitten kirjan nimen mukaan.
    // Lisäksi jokainen kirja on listalla vain kerran, vaikka se löytyisi useammasta kirjastosta

    map<string, set<string>> loanable_books;

    for (auto library : lib) {
    for (auto book : library.second) {
        if (book.reservations == 0) {
            loanable_books.insert({book.author, {}});
            loanable_books.at(book.author).insert(book.title);
        }
      }
    }
   for (auto author : loanable_books) {
       for (auto title : author.second)
       cout << author.first << ": " << title << endl;
    }
}

int main()
{
    string rivi;
    string tiedoston_nimi;
    cout << "Input file: ";
    getline(cin, tiedoston_nimi);

    Libraries libraryInfo;

    int exit_number = read_file(libraryInfo, tiedoston_nimi);
    if (exit_number == 1) {
        return EXIT_FAILURE;
    }

   while(true) {
        string input;
        cout << "lib> ";
        getline(cin, input);

     vector<string> inputs = split(input, ' ', true);

        if (inputs.at(0) == "quit") {
                return EXIT_SUCCESS;
            }
        else if (inputs.at(0) == "libraries") {
            string library_name = inputs.at(0);
            for (auto library : libraryInfo) {
                cout << library.first << endl;
                }
             }
        else if (inputs.at(0) == "material") {
            int input_amount = inputs.size();
            if (input_amount != 2) {
                cout << "Error: wrong number of parameters" << endl;
                continue;
                }
                string library_name = inputs.at(1);
                print_materials(libraryInfo, library_name);
             }
        else if (inputs.at(0) == "books") {
            int input_amount = inputs.size();
            if (input_amount != 3) {
                cout << "Error: wrong number of parameters" << endl;
                continue;
                }
               string library_name = inputs.at(1);
               string author_name = inputs.at(2);
               print_books(libraryInfo, library_name, author_name);
             }

        else if (inputs.at(0) == "reservable") {
            int input_amount = inputs.size();
            if (input_amount < 3) {
                cout << "Error: wrong number of parameters" << endl;
                continue;
                }
                string author = inputs.at(1);
                string title = get_book_title(input, inputs);
                print_reservable_books(libraryInfo, author, title);
              }

        else if (inputs.at(0) == "loanable" ) {
            int input_amount = inputs.size();
            if (input_amount != 1) {
                cout << "Error: wrong number of parameters" << endl;
                continue;
                 }
                print_loanable_books(libraryInfo);
             }

       else {
           cout << "Error: unknown command" << endl;
       }
   }
    return 0;
}


