#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>
#include <vector>
#include "Topology.h"
//#include <boost/functional/hash.hpp>

using namespace std;

typedef struct Chrom{
    short int bit[1000]; 
    int fit;            
    double rfit;        
    double cfit;        
                       
}chrom;

//template <typename Container> // we can make this generic for any container [1]
/*struct container_hash {
    std::size_t operator()(const vector<int> & c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};*/

class GA{
    public:
	Topology T;
        int nodeNum;
        int iteration;
        GA(Topology T, int iteration){
		this->T = T;
		this->nodeNum = T.GetVNum();
		this->iteration = iteration;
		cout << this->nodeNum << endl;
		cout << this->iteration << endl;	
	};
	//unordered_map<vector<int>, int, container_hash> hashFit;
	//unordered_map<vector<int>, vector<int>, container_hash> hashPath;
        vector<vector<int>> getBestServersPos(int num);
    private:

        void *evpop(chrom* popcurrent, int num);         
        vector<int> x(chrom popcurrent);                
        int y(vector<int> x);
        //void *pickchroms(chrom* popcurrent, int initNum);    
        void *pickchroms_new(chrom* popcurrent, chrom* popnext, int initNum);
        void *crossover(chrom* popnext, int initNum);        
        void *mutation(chrom* popnext, int initNum);         
        //chrom* popcurrent;                      
        //chrom* popnext;                         
};
