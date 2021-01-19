#include <vector>
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <string>
#include <queue>
#include <deque>
#include <utility>
#include <iostream>
#include <set>
#include <cmath>
#include "BitMask.hpp"

#ifndef GRAPHEHEADERDEF
#define GRAPHEHEADERDEF
using namespace std;

class Graphe
{
public:
    int N; // nb de noeuds
    std::vector<bool> prefill;                                //stocker les cases où ils sont pré définies
    unordered_map<int,int> result;                            //résultat des coleurs choisit des sommets
    unordered_map<int,std::vector<int>> info_ngbs;            //stocker les informations de voisins
    unordered_map<int,BitMask> info_importance;               //stocker les informations d'importance pour trier(indice <-> nb de coleurs disponible ; nb de voisins ; coleurs occupés)
    Graphe(){}
    Graphe(const char * filename);                            // constructeur a partir d'un fichier
    void init(int &);
    bool estValid(const int node,const int color,int k);
    ~Graphe(){
        std::vector<bool>().swap(prefill);
        result.clear();
        info_ngbs.clear();
        info_importance.clear();
    }
};

class Sudoku:public Graphe{
public:
    int cases;  //taille réelle de nombre de cas, normalement N*N 
    int colored;
    std::vector<std::vector<int>> cubicIdx(const int &);           // fonction outil pour regrouper les block des indices
    std::vector<std::vector<int>> permutation2Unique(const int &); // fonction outil pour lier les coordonnées de sudoku
    Sudoku(const char * filename);
    bool estValid_(const int node,const int color,int k);
    void solve();
    void print();
    ~Sudoku(){
        std::vector<bool>().swap(prefill);
        result.clear();
        info_ngbs.clear();
        info_importance.clear();
    }
};

#endif
