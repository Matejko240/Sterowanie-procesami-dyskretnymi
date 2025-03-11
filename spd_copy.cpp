#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
#include <chrono>
using namespace std;

class Zadanie{
public:
    Zadanie(int pj, int qj, int rj, int indeks)
        : m_pj(pj), m_qj(qj), m_rj(rj), m_indeks(indeks){}
    ~Zadanie() {};
    friend ostream& operator<<(ostream& os, const Zadanie& zadanie) {
        os << "rj: " << zadanie.m_rj << " pj: " << zadanie.m_pj << " qj: " << zadanie.m_qj <<" indeks: "<< zadanie.m_indeks;
        return os;
    }
    int GetQ() const { return m_qj;}
    int GetP() const { return m_pj;}
    int GetR() const { return m_rj;}
    void ZmienQ(int q) {m_qj = q;}
    void ZmienP(int p) {m_pj = p;}
    void ZmienR(int r) {m_rj = r;}
    int GetIndeks() const { return m_indeks;}
private:
    int m_pj;
    int m_qj;
    int m_rj;
    int m_indeks;
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
    vector<int> algorytmSchrage();
    void algorytmSchragezPZ();
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
    auto start = chrono::high_resolution_clock::now();
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) 
    {        indeksy[i] = i;
    }

    sort(indeksy.begin(), indeksy.end(), [&](int a, int b) {
        return m_zadania[a].GetR() < m_zadania[b].GetR();
    });
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end-start;
    double duration_ms = duration.count() * 1000.0;
    cout << "Sort po r czas: "<< duration_ms << endl;
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
    auto start = chrono::high_resolution_clock::now();
    vector<int> indeksy(m_n);
    for (int i = 0; i < m_n; ++i) {
        indeksy[i] = i;
    }

    sort(indeksy.begin(), indeksy.end(), [&](int a, int b) {
        return m_zadania[a].GetQ() < m_zadania[b].GetQ();
    });
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end-start;
    double duration_ms = duration.count() * 1000.0;
    cout << "Sort po q czas: "<< duration_ms << endl;
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
    auto start = chrono::high_resolution_clock::now();
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
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end-start;
    double duration_ms = duration.count() * 1000.0;
    cout << "Czas przeglad zupelny: "<< duration_ms << endl;

    return rozwiazanie;
}

class PorownajQ {
public:
    bool operator()(const Zadanie& zadanie1, const Zadanie& zadanie2) const {
        if (zadanie1.GetQ() < zadanie2.GetQ()) return true;
        if (zadanie1.GetQ() > zadanie2.GetQ()) return false;
        return false;
    }
};

class PorownajR {
public:
    bool operator()(const Zadanie& zadanie1, const Zadanie& zadanie2) const {
        if (zadanie1.GetR() > zadanie2.GetR()) return true;
        if (zadanie1.GetR() < zadanie2.GetR()) return false;
        return false;
    }
};

vector<int> Problem::algorytmSchrage(){
    auto start = chrono::high_resolution_clock::now();
    priority_queue<Zadanie, vector<Zadanie>, PorownajR> kolejkaR;
    priority_queue<Zadanie, vector<Zadanie>, PorownajQ> kolejkaQ;
    int t = 0;
    vector<int> kolejnosc;
    // int k = 0;
    int CMax = 0;

     for (const Zadanie& zadanie : m_zadania) {
        kolejkaR.push(zadanie);
    }
    while(!kolejkaQ.empty() || !kolejkaR.empty()){
        while(!kolejkaR.empty() && kolejkaR.top().GetR() <= t){
            
            kolejkaQ.push(kolejkaR.top());
            kolejkaR.pop();
    }
        if(kolejkaQ.empty()){
            t=kolejkaR.top().GetR();

        } else {
            Zadanie zadanie = kolejkaQ.top();
            kolejkaQ.pop();
            // k=k+1;
            kolejnosc.push_back(zadanie.GetIndeks());
            t+=zadanie.GetP();
            CMax=max(CMax, t+zadanie.GetQ());
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end-start;
    double duration_ms = duration.count() * 1000.0;
    cout << "AlgSchrage czas: "<< duration_ms <<"ms AlgSchrage CMax: " << CMax << endl;
    return kolejnosc;
}
void Problem::algorytmSchragezPZ(){
    auto start = chrono::high_resolution_clock::now();
    priority_queue<Zadanie, vector<Zadanie>, PorownajR> kolejkaR;
    priority_queue<Zadanie, vector<Zadanie>, PorownajQ> kolejkaQ;
    int t = 0;
    int CMax = 0;
    int q = numeric_limits<int>::max();
    Zadanie l(0,q,0,-1);
     for (const Zadanie& zadanie : m_zadania) {
        kolejkaR.push(zadanie);
    }
    while(!kolejkaQ.empty() || !kolejkaR.empty()){
        while(!kolejkaR.empty() && kolejkaR.top().GetR() <= t){
            Zadanie zadanie = kolejkaR.top();
            kolejkaQ.push(zadanie);
            kolejkaR.pop();
            if(zadanie.GetQ()>l.GetQ()) {
            l.ZmienP(t-zadanie.GetR());
            t=zadanie.GetR();
            if(l.GetP()>0){
                kolejkaQ.push(l);
            }
            }
    }
        if(kolejkaQ.empty()){
            t=kolejkaR.top().GetR();

        } else {
            Zadanie zadanie = kolejkaQ.top();
            kolejkaQ.pop();
            l=zadanie;
            t+=zadanie.GetP();
            CMax=max(CMax, t+zadanie.GetQ());
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end-start;
    double duration_ms = duration.count() * 1000.0;
    cout << "AlgSchragePZ czas: "<< duration_ms << "ms AlgSchragePZ CMax: " << CMax << endl;
}
int main() {
    ifstream dane ("SCHRAGE9.DAT");
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
        Zadanie zadanie(pj, qj, rj, i);
        zadania.push_back(zadanie);
    }
    Problem problem(n, zadania);

    Rozwiazanie rozwiazaniePoR(LiczCmax(problem.GetZadania(),problem.SortujPoR()), problem.SortujPoR());
    Rozwiazanie rozwiazaniePoQ(LiczCmax(problem.GetZadania(),problem.SortujPoQ()), problem.SortujPoQ());
    Rozwiazanie rozwiazaniePoC(LiczCmax(problem.GetZadania(),problem.SortujPoC()), problem.SortujPoC());
    //Rozwiazanie rozwiazaniePZupelny(LiczCmax(problem.GetZadania(),problem.PZupelny()), problem.PZupelny());
    Rozwiazanie rozwiazanieASchrage(4, problem.algorytmSchrage());
    problem.algorytmSchrage();
    cout<<"Rozw po R ";
    rozwiazaniePoR.WyswietlKolejnosc();
    cout<<"Rozw po Q ";
    rozwiazaniePoQ.WyswietlKolejnosc();
    cout<<"Rozw po C ";
    rozwiazaniePoC.WyswietlKolejnosc();
    //cout<<"Przeglad zupelny";
    //rozwiazaniePZupelny.WyswietlKolejnosc();
    cout<<"Algorytm Schrage";
    rozwiazanieASchrage.WyswietlKolejnosc();
    // problem.Poka();
    cout << "Cmax sort po R: " << rozwiazaniePoR.GetKryterium() << endl;
    cout << "Cmax sort po Q: " << rozwiazaniePoQ.GetKryterium() << endl;
    cout << "Cmax sort po C: " << rozwiazaniePoC.GetKryterium() << endl;
   // cout << "Cmax przeglad zupelny: " << rozwiazaniePZupelny.GetKryterium() << endl;
    problem.algorytmSchrage();
    problem.algorytmSchragezPZ();
    

    return 0;
}