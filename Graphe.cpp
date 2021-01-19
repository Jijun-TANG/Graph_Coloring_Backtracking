#include "Graphe.hpp"
//--------------------------------constructeur---------------------------------------------//

Graphe::Graphe(const char * filename) { // constructeur a partir d'un fichier texte
    std::ifstream  f(filename);
    assert( f );

    int n; // nombre de noeuds
    int m; // nombre d'arcs

    f >> n >> m;
    N = n;
    for(int i=0; i < m; i++) // remplissage du graphe
    {
        int a, b;
        f >> a >> b;
        a--; b--; // zero based notation

        assert(a < n && a >= 0);
        assert(b < n && b >= 0);
        info_ngbs[a].push_back(b);
        info_ngbs[b].push_back(a);
    }
    //prefill.clear();
    prefill.resize(N);
    f.close();
}

void Graphe::init(int & start_node){//initialiser la conteneur result, info_importance et donner le premier sommet le plus éligible à colorer
    std::priority_queue<std::pair<int,std::pair<int,int>>,std::deque<std::pair<int,std::pair<int,int>>>> deg_q;
    for(auto it=info_ngbs.begin();it!=info_ngbs.end();it++){
        int now=it->first;           //now node of graphe
        if(prefill[now]){continue;}               //no change the prefilled color
        int tmp=0;                   // colors occupied (correlation négative de colors disponible, pour faire le trie)
        auto c=it->second;           // vector ngbs of now node
        int s=(c).size();
        result[now]=-1;   //initialiser le hashmap 'result' pour stocker les résultats plus tard
        info_importance[now]=*(new BitMask(N));
        info_importance[now].setExtra(s);
        for(int j=0;j<s;++j){
            if(prefill[c[j]]){
                if(!info_importance[now][result[c[j]]]){
                    ++tmp;
                    info_importance[now](result[c[j]])=true;
                }
                
            }
        }
        deg_q.push(std::make_pair(tmp,std::make_pair(s,now)));//(colors occupied, neighbours numbers, indices)
    }
    if(deg_q.empty()){
        throw runtime_error("Error in Init: all the nodes are already painted\n");
    }
    else{start_node=(deg_q.top()).second.second;}
}

bool Graphe::estValid(const int node,const int color,int k){
    result[node]=color;
    if(k==N){       //au moin N sommets ont une coleur, on arrête la récurrsion
        cout<<"full_color success!\n";
        return true;}           

    std::vector<int> ngbs=info_ngbs[node];  //pour actualiser les informations de coleurs à ses voisins
    for(int i=0;i<(int)info_ngbs[node].size();++i){
        
        if(!prefill[info_ngbs[node][i]] && result[info_ngbs[node][i]]==-1){                  //on concerne que les nodes non colorées
            
            if(!info_importance[info_ngbs[node][i]][color]){ //on évite de diminuer le nbr de coleur disponible plusieurs fois à cause du même couleur
                
                info_importance[info_ngbs[node][i]](color)=true;  //ce coleur est occupé
                
                if(info_importance[info_ngbs[node][i]].ON()<=0){
                    //cout<<"not enough color from node "<<node+1<<" for node "<<ngbs[i]+1<<'\n';
                    return false;
                }
            }

        }
        
    }
    std::priority_queue<std::pair<int,std::pair<int,int>>,std::deque<std::pair<int,std::pair<int,int>>>> dgs; //pour trier selon le degree d'importances et trouver le prochain sommet à colorer
    
    //je choisis ce type de conteneur pour éviter d'appeler la fonction 'sort'. Une fois tous les éléments sont à la position, on peut avoir la bonne élément souhaitée.
    //Je choisis deque comme la structure interne car on a besoin que la première élément de la queue. Deque est implémenté pour l'efficacité de reprendre les éléments à deux bouts 

    for(auto it=info_importance.begin();it!=info_importance.end();++it){
        if(!prefill[it->first] && result[it->first]==-1){         //on concerne que les nodes non colorées
            dgs.push(std::make_pair(N-(it->second).ON(),std::make_pair((it->second).extra(),it->first)));
        }
    }
    if(dgs.empty()){return true;}
    auto next=dgs.top();
    int next_node=next.second.second;
    for(int i=0;i<N;++i){
        if(!info_importance[next_node][i]){
            auto bkup1=result;
            unordered_map<int,BitMask> bkup2(info_importance.begin(),info_importance.end());
            if(estValid(next_node,i,k+1)){
                return true;
            }
            result=bkup1;               //idée coeur de backtracking
            info_importance=bkup2;
        }
    }
    return false;
}

std::vector<std::vector<int>> Sudoku::cubicIdx(const int & size){
    int ss=(int)sqrt(size);
    std::vector<std::vector<int>> ans(size,std::vector<int>());
    for(int i=0;i<ss;++i){
        for(int j=0;j<ss;++j){
            int s_i=i*ss*size+j*ss;
            int k=s_i;
            while(k<=s_i+(ss-1)*size){
                for(int l=0;l<ss;++l){
                    ans[i*ss+j].push_back(k+l);
                }
                k+=size;
            }
        }
    }
    return ans;
}

std::vector<std::vector<int>> Sudoku::permutation2Unique(const int & size){
    if(size<=1){return {};}
    if(size==2){return {{0,1}};}
    std::vector<std::vector<int>> ans;
    for(int i=0;i<size-1;++i){
        for(int j=i+1;j<size;++j){
            ans.push_back({i,j});
        }
    }
    return ans;
}

Sudoku::Sudoku(const char * filename){
    std::ifstream f(filename);
    if(!f){std::cout<<"filename error\n";}
    f>>N;                              //load problem size into N
    if(N<1){
        throw std::domain_error("invalide N value\n");
    }
    cases=N*N; //numbre total des cas c'est la longeur au carré
    int i,j=0;// indices for row & columns
    
    // transform the N x N sudoko to a graph with elements in the same line, column, block connected to each other

    std::vector<std::vector<bool>> linkedmem(cases,std::vector<bool>(cases,false)); //to memorize the node connected
    
    for(;j<cases-1;++j){
        int row_max=((j/N)+1)*N;
        int col_max=(N-1-j/N)*N+j;
        for(i=1;i<N;++i){
            if(j+i<row_max){
                info_ngbs[j].push_back(j+i);
                info_ngbs[j+i].push_back(j);
                linkedmem[j][j+i]=true;
                linkedmem[j+i][j]=true;
            }
            if(j+i*N<=col_max){
                info_ngbs[j].push_back(j+i*N);
                info_ngbs[j+i*N].push_back(j);
                linkedmem[j][j+i*N]=true;
                linkedmem[j+i*N][j]=true;
            }
        }
    }

    std::vector<std::vector<int>> p_l=permutation2Unique(N);// pour donner des permutation 2 à 2 unique dans un block

    std::vector<std::vector<int>> b_i=cubicIdx(N);// pour donner des indices dans une sous block de tableau Sudoku
    int c2n=(int)p_l.size();
    for(i=0;i<N;++i){
        for(j=0;j<c2n;++j){
            if(!linkedmem[b_i[i][p_l[j][0]]][b_i[i][p_l[j][1]]]){
                info_ngbs[b_i[i][p_l[j][0]]].push_back(b_i[i][p_l[j][1]]);
                info_ngbs[b_i[i][p_l[j][1]]].push_back(b_i[i][p_l[j][0]]);
            }
        }
    }
    colored=0;
    prefill.resize(cases);
    while (f>>i>>j){
        if(i<1||i>cases){
            throw std::domain_error("index out of problem size\n");
        }
        if(j<1||j>N){
            throw std::domain_error("number prefilled out of problem size\n");
        }
        ++colored;
        prefill[i-1]=true;
        result[i-1]=j-1;
    }
    f.close();
}

bool Sudoku::estValid_(const int node,const int color,int k){
    result[node]=color;
    if(k==cases){       //au moin N sommets ont une coleur, on arrête la récurrsion
        cout<<"full_color success!\n";
        return true;
    }

    std::vector<int> ngbs=info_ngbs[node];  //pour actualiser les informations de coleurs à ses voisins
    
    for(int i=0;i<(int)info_ngbs[node].size();++i){
        
        if(!prefill[info_ngbs[node][i]] && result[info_ngbs[node][i]]==-1){                  //on concerne que les nodes non colorées
            
            if(!info_importance[info_ngbs[node][i]][color]){ //on évite de diminuer le nbr de coleur disponible plusieurs fois à cause du même couleur
                
                info_importance[info_ngbs[node][i]](color)=true;  //ce coleur est occupé
                
            }

        }
        
    }
    std::priority_queue<std::pair<int,int>,std::deque<std::pair<int,int>>> dgs;            //pour trier selon le degree d'importances et trouver le prochain sommet à colorer
    
    //importance : nb de coleurs disponibles & indices de sommets(car tous les cas de sudoku ont le même nbr de voisins)
    
    for(auto it=info_importance.begin();it!=info_importance.end();++it){
        if(!prefill[it->first] && result[it->first]==-1){         //on concerne que les nodes non colorées
            dgs.push(std::make_pair(N-(it->second).ON(),it->first));
        }
    }
    auto next=dgs.top();
    
    if(next.first>=N){
        //cout<<"Node "<<node+1<<" Non coleur disponible pour le node "<<next.second+1<<'\n';
        return false;
    }
    for(int i=0;i<N;++i){
        if(!info_importance[next.second][i]){
            auto bkup1=result;
            unordered_map<int,BitMask> bkup2(info_importance.begin(),info_importance.end());
            if(estValid_(next.second,i,k+1)){
                return true;
            }
            result=bkup1;               //méthode de backtracking
            info_importance=bkup2;      //méthode de backtracking : récuperer l'état précédent
            bkup1.clear();
            bkup2.clear();
        }
    }
    return false;   
}



void Sudoku::solve(){
    int start_node;
    init(start_node);
    for(int i=0;i<N;++i){
        if(!info_importance[start_node][i]){
            auto b1=result;
            auto b2=info_importance;
            if(estValid_(start_node,i,1+colored)){
                return;
            }
            result=b1;
            info_importance=b2;
        }
    }
}

void Sudoku::print()
{   
    cout << endl;
    
    for (int i = 0; i < cases; i++)
    {
        cout << result[i] + 1 << " ";
        if ((i + 1) % (int)sqrt(N) == 0)
            cout << "|";
        if ((i + 1) % N == 0)
        {
            cout << endl;
        }
        if ((i + 1) % (N * (int)sqrt(N)) == 0)
        {
            for (int j = 0; j < N + 1; j++)
                cout << "--";
            cout << endl;
        }
    }
}