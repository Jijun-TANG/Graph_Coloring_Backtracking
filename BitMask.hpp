#include<stdexcept>
#include<cstring>
#ifndef __BITMASK__
#define __BITMASK__

class BitMask{
private:
int O; // stocker les couleurs disponibles à colorer
int extra_; //stocker les nombres de voisins, juste pour trier, il ne bouge pas dès qu'un BitMask est initialisé, donc est appelé extra
int N; // le nombre maximum des couleurs possibles
bool *Data;//les masques pour indiquer la quelle couleur on peut utiliser. Par exemple si Data[i] = true, ça veut dire que i ième couleur est occupée
//en faisant cette structure on aura plus besoin de fonction 'cols' et boucler chaque fois quand on met à jour une couleur au noeud
public:
    //constructeur par défaut
    BitMask():O(0),extra_(0),N(0){
        Data=nullptr;
    }

    //constructeur qui va être utiliser dans l'algo principale
    BitMask(int n_):O(n_>0?n_:throw std::domain_error("Invalid bits initialise value (N>=0)\n")),extra_(0),N(n_){
        Data=new bool[n_];
        std::memset(Data,false,n_);
    }

    //constructeur par copie
    BitMask(const BitMask & b):O(b.O),extra_(b.extra_),N(b.N){
        Data=new bool[N];
        for(int i=0;i<N;++i){
            Data[i]=(b.Data)[i];
        }
    }
    void setExtra(int const & a_){ // mettre à jour le nombre de voisins
        extra_=a_;
    }

    //accès aux n_ ieme masque
    bool operator[](int const & n_) const{
        if(n_<0 || n_ >= N){
            throw std::domain_error("Invalid indice value '(i>=0) and (i<=N-1)' \n");
        }
        return *(Data+n_);
    }

    //modifier le n_ ième masque, on la modifie que dans le cas où une couleur de sommet voisin est attribuée
    bool & operator()(int const & n_){
        if(n_<0 || n_ >= N){
            throw std::domain_error("Invalid indice value '(i>=0) and (i<=N-1)' \n");
        }
        O--;
        return *(Data+n_);
    }

    //les opérateurs permettant de faire la comparaison dans le conteneur priority_queue
    bool operator<(BitMask const & b) const{
        if(O<b.O){
            return true;
        }
        return extra_>b.extra_;
    }
    bool operator>(BitMask const & b) const{
        if(O>b.O){
            return true;
        }
        return extra_<b.extra_;
    }
    bool operator==(BitMask const & b) const{
        return O==b.O&&extra_==b.extra_;
    }

    //les méthodes outils
    int ON() const{
        return O;
    }
    int size() const{
        return N;
    }
    int extra() const{
        return extra_;
    }
    void recalibrate(){
        O=N;
        for(int i=0;i<N;++i){
            O-=Data[i];
        }
    }
    bool verify() const{
        int a=0;
        for(int i=0;i<N;++i){
            a+=Data[i];
        }
        return a==O;
    }
    
    //le destructeur
    ~BitMask(){
        delete[] Data;
    } 
};



#endif