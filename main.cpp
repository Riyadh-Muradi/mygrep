/* tekija: riyadh muradi */

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <clocale>

using namespace std;

string pieneks(string teksti)
{
    for (int i = 0; i < (int)teksti.length(); i++)
    {
        teksti[i] = tolower(teksti[i]);
    }

    return teksti;
}

bool onkoPaalla(string optiot, char kirjain)
{
    for (int i = 0; i < (int)optiot.length(); i++)
    {
        if (optiot[i] == kirjain)
        {
            return true;
        }
    }

    return false;
}

bool onkoSallittuOptio(char c)
{
    if (c == 'l')
        return true;
    if (c == 'o')
        return true;
    if (c == 'i')
        return true;
    if (c == 'r')
        return true;

    return false;
}

bool optiotOk(string optiot)
{
    if (optiot.length() < 3)
    {
        return false;
    }

    if (optiot[0] != '-' || optiot[1] != 'o')
    {
        return false;
    }

    for (int i = 2; i < (int)optiot.length(); i++)
    {
        char c = optiot[i];

        if (!onkoSallittuOptio(c))
        {
            return false;
        }
    }

    return true;
}

void kysyJaEtsi()
{
    string lause;
    string haku;

    cout << "Anna lause, josta haetaan: ";
    getline(cin, lause);

    cout << "Anna hakusana: ";
    getline(cin, haku);

    int kohta = (int)lause.find(haku);

    if (kohta >= 0 && lause.find(haku) != string::npos)
    {
        cout << "\"" << haku << "\" löytyi lauseesta \"" << lause << "\" kohdasta " << kohta << endl;
    }
    else
    {
        cout << "\"" << haku << "\" ei löytynyt lauseesta \"" << lause << "\"" << endl;
    }
}

void haeTiedostosta(string haku, string tiedostoNimi, string optiot)
{
    ifstream tiedosto;

    try
    {
        ifstream tiedostoKoko(tiedostoNimi, ios::binary | ios::ate);
        if (!tiedostoKoko)
        {
            throw -1;
        }

        streampos tiedostoKokoTavuina = tiedostoKoko.tellg();
        if (tiedostoKokoTavuina < 0)
        {
            throw -1;
        }

        tiedostoKoko.close();

        tiedosto.open(tiedostoNimi);
        if (!tiedosto)
        {
            throw -1;
        }
    }
    catch (int virhe)
    {
        cout << "virhe " << virhe << endl;
        cout << "emt \"" << tiedostoNimi << "\" kokoo " << endl;
        return;
    }

    bool rivinumerot = onkoPaalla(optiot, 'l');
    bool lukumaara = onkoPaalla(optiot, 'o');
    bool kirjainkokoPois = onkoPaalla(optiot, 'i');
    bool kaanteinen = onkoPaalla(optiot, 'r');

    string rivi;
    int rivinumero = 0;
    int osumat = 0;

    while (getline(tiedosto, rivi))
    {
        rivinumero++;

        string riviVertailu = rivi;
        string hakuVertailu = haku;

        if (kirjainkokoPois)
        {
            riviVertailu = pieneks(riviVertailu);
            hakuVertailu = pieneks(hakuVertailu);
        }

        bool loytyi = (riviVertailu.find(hakuVertailu) != string::npos);

        if (kaanteinen)
        {
            loytyi = !loytyi;
        }

        if (loytyi)
        {
            osumat++;

            if (rivinumerot)
            {
                cout << rivinumero << ":";
            }

            cout << rivi << endl;
        }
    }

    tiedosto.close();

    if (lukumaara)
    {
        cout << endl;

        if (kaanteinen)
        {
            cout << "Rivejä joilta ei löytynyt \"" << haku << "\": " << osumat << endl;
        }
        else
        {
            cout << "Rivejä joilta löytyi \"" << haku << "\": " << osumat << endl;
        }
    }
}

void ohje()
{
    cout << "Komento meni väärin, kokeile näin:" << endl;
    cout << "  mygrep" << endl;
    cout << "  mygrep <haku> <tiedosto>" << endl;
    cout << "  mygrep -olo <haku> <tiedosto>" << endl;
    cout << "  optiot l,o,i,r (voi yhdistää samaan)" << endl;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "fi_FI.UTF-8");

    if (argc == 1)
    {
        kysyJaEtsi();
        return 0;
    }

    if (argc == 3)
    {
        haeTiedostosta(argv[1], argv[2], "");
        return 0;
    }

    if (argc == 4)
    {
        string optiot = argv[1];

        if (!optiotOk(optiot))
        {
            ohje();
            return 1;
        }

        haeTiedostosta(argv[2], argv[3], optiot);
        return 0;
    }

    ohje();
    return 1;
}
