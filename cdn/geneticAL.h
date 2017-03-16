#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>
#include <vector>
#include "Topology.h"
#include <boost/functional/hash.hpp>

using namespace std;

typedef struct Chrom{
    short int bit[1000]; //网络节点编码
    int fit;            //适应值，这里指成本
    double rfit;        //相对fit值，即所占的备份比
    double cfit;        //累计概率
                        //存放网络链路
}chrom;

//template <typename Container> // we can make this generic for any container [1]
struct container_hash {
    std::size_t operator()(const vector<int> & c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};

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
	unordered_map<vector<int>, int, container_hash> hashFit;
	unordered_map<vector<int>, vector<int>, container_hash> hashPath;
        vector<vector<int>> getBestServersPos(int num);
    private:

        void *evpop(chrom* popcurrent, int num);         //种群初始化
        vector<int> x(chrom popcurrent);                
        int y(vector<int> x);
        void *pickchroms(chrom* popcurrent, int initNum);    //选择操作
        void *pickchroms_new(chrom* popcurrent, chrom* popnext, int initNum);//轮盘选择
        void *crossover(chrom* popnext, int initNum);        //交叉操作
        void *mutation(chrom* popnext, int initNum);         //突变操作
        //chrom* popcurrent;                      //初始种群规模
        //chrom* popnext;                         //更新后种群规模
};
