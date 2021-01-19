#include <chrono>
#include "Graphe.hpp"
#include "BitMask.hpp"



int main(int argc, const char ** argv)
{   
    using std::cout;
    using std::endl;
    /*
    Graphe G("G2.txt");

    cout << "nb nodes = " << G.N << endl;
    int start_node;
    G.init(start_node);
    cout<<"special result test\n";
    for(int i=0;i<G.N;++i){
        cout<<G.result[i]<<' ';
    }
    cout<<"special voisins test\n";
    for(int i=0;i<G.N;++i){
        cout<<i+1<<'\n';
        for(int j=0;j<(int)G.info_ngbs[i].size();++j){
            cout<<G.info_ngbs[i][j]+1<<' ';
        }
        cout<<'\n';
    }
    cout<<"\nstart from node "<<start_node+1<<'\n';
    for(int i=0;i<(int)G.info_importance[start_node].size();++i){
        if(!G.info_importance[start_node][i]){
            auto b1=G.result;
            auto b2=G.info_importance;
            if(G.estValid(start_node,i,1)){
                break;
            }
            G.result=b1;
            G.info_importance=b2;
        }
    }
    auto ans=G.result;
    for(auto it=ans.begin();it!=ans.end();++it){
        cout<<"Node : "<<it->first+1<<' '<<it->second+1<<'\n';
    }
    cout << "end prog" << endl;
    */
    Sudoku s1("09_probleme04.TXT");
    auto start_time=std::chrono::high_resolution_clock::now();
    s1.solve();
    auto end_time=std::chrono::high_resolution_clock::now();
    s1.print();
    double secondsPassed = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    cout << "end prog ... Time taken by solve() : " << secondsPassed / 1000 << " s" << endl;
    return 0;
}
