#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Zadanie{
public:
    Zadanie(int pj, int qj, int rj)
        : m_pj(pj), m_qj(qj), m_rj(rj){}
    ~Zadanie() {};
    friend ostream& operator<<(ostream& os, const Zadanie& zadanie) {
        os << "rj: " << zadanie.m_rj << " pj: " << zadanie.m_pj << " qj: " << zadanie.m_qj;
        return os;
    }
    int GetQ() const { return m_qj;}
    int GetP() const { return m_pj;}
    int GetR() const { return m_rj;}
private:
    int m_pj;
    int m_qj;
    int m_rj;
};

class Rozwiazanie
{
public:
    Rozwiazanie(int kryterium, vector <int> solution) 
    : m_kryterium(kryterium), m_solution(solution){}
    ~Rozwiazanie() {};

    vector<int> GetKolejnosc() const {
    return m_solution;
    }
    int GetKryterium() const {
    return m_kryterium;    
    }
    void WyswietlKolejnosc()
    {
        cout << "Kolejnosc: ";
        for (int i :m_solution) {
            cout << i << " ";
        }
        cout << endl;
    }
private:
    vector <int> m_solution;
    int m_kryterium;
};
class Problem
{
public:
    Problem(int n, vector<Zadanie> zadania)
    : m_n(n), m_zadania(zadania){};
    ~Problem() {};
    void Poka(){
        for (const Zadanie& zadanie : m_zadania) {
          cout << zadanie << endl;
        }
    } 
    vector<int> SortujPoR();
    vector<int> SortujPoQ();
    vector<int> SortujPoC();
    vector<int> PZupelny();
    vector<int> GetIndeksy();
    vector<Zadanie> GetZadania(){
        return m_zadania;
    }
private:
    int m_n; //liczba zadan
    vector<Zadanie> m_zadania;
};
vector<int> Problem::GetIndeksy() {
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) 
    {        indeksy[i] = i;
    }

    return indeksy;
}
vector<int> Problem::SortujPoR() {
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) 
    {        indeksy[i] = i;
    }

    sort(indeksy.begin(), indeksy.end(), [&](int a, int b) {
        return m_zadania[a].GetR() < m_zadania[b].GetR();
    });

    return indeksy;
}

vector<int> Problem::SortujPoC(){
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) 
    {        indeksy[i] = i;
    }

    vector<int> C(m_n);
   
    for (int i = 0; i < m_n; ++i) {
    C[i] = m_zadania[i].GetR() + m_zadania[i].GetP() + m_zadania[i].GetQ();
    }
    sort(indeksy.begin(), indeksy.end(), [&](int a, int b) {
        return C[a] < C[b];
    });

    return indeksy;
}


vector<int> Problem::SortujPoQ() {
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) {
        indeksy[i] = i;
    }

    sort(indeksy.begin(), indeksy.end(), [&](int a, int b) {
        return m_zadania[a].GetQ() < m_zadania[b].GetQ();
    });

    return indeksy;
}

int LiczCmax(const vector<Zadanie>& zadania, const vector<int>& kolejnosc) {
    int Cmax = 0;
    int czas = 0;
    for (int i : kolejnosc) {
        czas = max(czas, zadania[i].GetR()) + zadania[i].GetP();
        Cmax = max(Cmax, czas + zadania[i].GetQ());
    }
    return Cmax;
}
vector<int> Problem::PZupelny() {
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) {
        indeksy[i] = i;
    }
    vector<int> rozwiazanie = indeksy;
    int najlepszyCmax = LiczCmax(m_zadania, indeksy);

    do {
    int aktualnyCmax = LiczCmax(m_zadania, indeksy);
    if(aktualnyCmax<najlepszyCmax)
    {
        rozwiazanie = indeksy;
        najlepszyCmax = aktualnyCmax;
    }
    } while (next_permutation(indeksy.begin(), indeksy.end())); // Generujemy kolejną permutację

    return rozwiazanie;
}

int main() {
    ifstream dane ("SCHRAGE1.DAT");
    if (!dane.is_open()){
    cout<<"error mordo";
    return 0;
    }
    int n;
    dane >> n;
    vector<Zadanie> zadania;
    for(int i = 0; i<n; ++i){
        int pj,qj,rj;
        dane >> rj >> pj >> qj;
        Zadanie zadanie(pj, qj, rj);
        zadania.push_back(zadanie);
    }
    Problem problem(n, zadania);

    Rozwiazanie rozwiazaniePoR(0, problem.SortujPoR());
    Rozwiazanie rozwiazaniePoQ(1, problem.SortujPoQ());
    Rozwiazanie rozwiazaniePoC(2, problem.SortujPoC());
    Rozwiazanie rozwiazaniePZupelny(3, problem.PZupelny());

    cout<<"Rozw po R ";
    rozwiazaniePoR.WyswietlKolejnosc();
    cout<<"Rozw po Q ";
    rozwiazaniePoQ.WyswietlKolejnosc();
    cout<<"Rozw po C ";
    rozwiazaniePoC.WyswietlKolejnosc();
    cout<<"Przeglad zupelny";
    rozwiazaniePZupelny.WyswietlKolejnosc();
    
    problem.Poka();
    cout << "Cmax sort po R: " << LiczCmax(problem.GetZadania(),rozwiazaniePoR.GetKolejnosc()) << " dla kryterium: " << rozwiazaniePoR.GetKryterium() << endl;
    cout << "Cmax sort po Q: " << LiczCmax(problem.GetZadania(),rozwiazaniePoQ.GetKolejnosc()) << " dla kryterium: " << rozwiazaniePoQ.GetKryterium() << endl;
    cout << "Cmax sort po C: " << LiczCmax(problem.GetZadania(),rozwiazaniePoC.GetKolejnosc()) << " dla kryterium: " << rozwiazaniePoC.GetKryterium() << endl;
    cout << "Cmax przeglad zupelny: " << LiczCmax(problem.GetZadania(),rozwiazaniePZupelny.GetKolejnosc())\
                                      << " dla kryterium: " << rozwiazaniePZupelny.GetKryterium() << endl;

    return 0;
}