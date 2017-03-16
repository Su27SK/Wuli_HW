#include <string>
#include <math.h>
#include "geneticAL.h"
#include <algorithm>

#define INIT_Min 0x7fffffff

using namespace std;

bool comparison(Chrom a, Chrom b){
return a.fit<b.fit;
}

vector<vector<int>> GA::getBestServersPos(int initNum){
	chrom* popcurrent = new struct Chrom[initNum];
	chrom* popnext = new struct Chrom[initNum];

	int i ,j, l,Min;
	Min=INIT_Min;                                      // 函数最大值
	vector<int> k;	

	//cout <<  this->nodeNum << endl;
	//cout <<  this->iteration << endl;

	srand(time(0));
	evpop(popcurrent, initNum);	//随机产生初始种子群
	sort(popnext, popnext+initNum, comparison);
	
	Min = popcurrent[0].fit;//对Min值进行初始化
	
	
	/*****这里可能需要修改，增加收敛就停止*****/
	for(i =0;i< this->iteration;i++)                          // 开始迭代；
	{

		//printf("\ni = %d\n" ,i);                 // 输出当前迭代次数；

		for(j =0;j<initNum; j++)
		{
			popnext[j]=popcurrent[j];           // 更新种群；
		}
		//pickchroms(popnext, initNum);
		//pickchroms_new(popcurrent, popnext, initNum);                    // 挑选优秀个体；
		
		crossover(popnext, initNum);                     // 交叉得到新个体；
		sort(popnext, popnext+initNum, comparison);
		mutation(popnext, initNum);                      // 变异得到新个体；
		sort(popnext, popnext+initNum, comparison);

		for(j =0;j<initNum; j++) 
		{
			//cout << "popnext " << j << " \nbit is " << x(popnext[j]) << "\nfit is " << popnext[j].fit << endl;
			popcurrent[j]=popnext[j];              // 种群更替；
		}

	}  // 等待迭代终止；

	//对于真正随机数是需要注意取较大的迭代次数
	for(l =0;l<initNum; l++)
	{
		//cout << "popcurrent " << l << " \nbit is " << x(popcurrent[l]) << "\nfit is " << popcurrent[l].fit << endl;
		if(popcurrent[l].fit < Min)
		{
			Min=popcurrent[l].fit;
			k = x(popcurrent[l]);//此时的value即为所求的x值，之后会删掉
		}

	}
	cout << "k size is " << k.size() << endl;
	cout << "\n 当x等于 ";
	for(int i = 0; i < k.size(); i++){
		cout << k[i] << "\t";
	}
	cout << " 时，函数得到最小值为： " << y(k) << endl;
	cout << "\nPress any key to end ! " << endl;


	vector<vector<int>> route;
	vector<vector<int>>& path = route;

	//cout << "cumpute cost is " << T.minCostFlow(k, path) << endl;

	T.minCostFlow(k, path);

	return route;
}

void *GA::evpop(chrom* popcurrent, int initNum)   // 函数：随机生成初始种群；
{
	int i ,j;
	int random ;
	double sum=0;
	int pro = T.GetVNum() / T.GetCNum();
	int middle = pro / 2;
	//cout << "V num is " << T.GetVNum() << "\nc num is " << T.GetCNum() << "\npro is " << pro << "\nmiddle is " << middle << endl;
	for(j =0;j<initNum; j++)                            // 从种群中的第1个染色体到第initNum个染色体
	{	
		//cout << "init num is " << j << endl;
		for(i =0;i<this->nodeNum; i++)                       // 从染色体的第1个基因位到第nodeNum个基因位
		{
			random=rand ();                     // 产生一个随机值
			random=(random % pro);                 // 随机产生0或者1，每当
			//cout << "random is " << random << endl;
			popcurrent[j].bit[i]= random == middle?1:0;       // 随机产生染色体上每一个基因位的值，或；
		}  

		popcurrent[j].fit= y(x(popcurrent[j])); 		// 计算染色体的适应度值，*********************这个地方需要替换成最小最大流算法
		sum = sum + popcurrent[j].fit;
		//cout << "popcurrent " << j << " \nbit is " << x(popcurrent[j ]) << "\nfit is " << popcurrent[j ].fit << endl;
		// 输出整条染色体的编码情况
	}
	//计算适应值得百分比，该参数是在用轮盘赌选择法时需要用到的
	for (j = 0; j < initNum; j++)
	{
		popcurrent[j].rfit = popcurrent[j].fit/sum;
		popcurrent[j].cfit = 0;//将其初始化为0
	}
	return(0);                
}            

vector<int> GA::x(chrom popcurrent)  // 将编码转换成编号
{
	
	vector<int> selectNode;
	for(int i = 0; i < this->nodeNum; i++){
		if(popcurrent.bit[i]){
			selectNode.push_back(i);
			//cout << i << "\t";
		}
	}
	//cout << "\n*******************************************************************************" << endl;
	return selectNode;                           
}                                     
//需要能能够从外部直接传输函数，加强鲁棒性


int GA::y(vector<int> x)// 函数：求个体的适应度；*****************这个地方可以建一个hash表，优化时间
{
	if(x.empty()){
		return INIT_Min;
	}
	int cost = INIT_Min;
	vector<vector<int>> route;
	vector<vector<int>>& path = route;
	if(hashFit.find(x) == hashFit.end()) {	
		//如果这个节点之前没有算过，那么重新选择最小最大路径，如果算过，则直接在hash表中找
		//cout << "server cost is " << T.GetServerCost() << endl;
		//cout << "start find min cost flow" << endl;
        	cost = T.minCostFlow(x, path);
		//cout << "end find min cost flow" << cost << endl;
		if(cost != INIT_Min){
			cost += T.GetServerCost() * x.size();
		}
		
		hashFit[x] = cost;
		//hashPath[x] = route;
		
	}
	else  {
		cost = 	hashFit[x];
	}
	//cout << "\n \n x size is " << x.size() << "   cost is " << cost << endl;
	return(cost);             
} 

//基于轮盘赌选择方法，进行基因型的选择
void *GA::pickchroms_new (chrom* popcurrent, chrom* popnext, int initNum)//计算概率
{
	int men;
	int i;int j;
	double p;
	double sum=0.0;
	//find the total fitness of the population
	for (men = 0; men < initNum; men++ )
	{
		sum = sum + 1 / popnext[men].fit;
	}
	//calculate the relative fitness of each member
	for (men = 0; men < initNum; men++ )
	{
		popnext[men].rfit = (1/popnext[men].fit) / sum;
	}
	//calculate the cumulative fitness,即计算积累概率
	popcurrent[1].cfit = popcurrent[1].rfit;
	for ( men = 1; men < initNum; men++)
	{
		popnext[men].cfit = popnext[men-1].cfit + popnext[men].rfit;
	}
	
	for ( i = 1; i < initNum; i++ )
	{//产生0~1之间的随机数
		//p = r8_uniform_ab ( 0, 1, seed );//通过函数生成0~1之间均匀分布的数字
		p =rand()%10;//
		p = p/10;
		if ( p < popnext[0].cfit )
		{
			popcurrent[i] = popnext[0];      
		}
		else
		{
			for ( j = 0; j < initNum; j++ )
			{ 
				if ( popnext[j].cfit <= p && p < popnext[j+1].cfit )
				{
					popcurrent[i] = popcurrent[j+1];
				}
			}
		}
	}
	//  Overwrite the old population with the new one.
	//
	for ( i = 0; i < initNum; i++ )
	{
		popnext[i] = popcurrent[i]; 
	}
	return(0);
}
void *GA::pickchroms (chrom* popnext, int initNum)          // 函数：选择个体；
{
	int i ,j;
	chrom temp ;                                // 中间变量
	//因此此处设计的是个个体，所以参数是
	for(i =0;i<initNum-1; i++)                           // 根据个体适应度来排序；（冒泡法）
	{
		for(j =0;j<initNum - 1 - i; j++)
		{
			if(popnext [j+1].fit<popnext [j].fit)
			{
				temp=popnext[j+1];
				popnext[j +1]=popnext[j];
				popnext[j ]=temp;

			}  
		}               
	}
	/*for(i =0;i<initNum; i++)
	{
		printf("\nSorting:popnext[%d] fitness=%d" ,i, popnext[i ].fit);
		printf("\n" );                     
	} */                    
	return(0);
}   

void *GA::crossover (chrom* popnext, int initNum)              // 函数：交叉操作；
{

	int random ;
	int i ;
	//srand(time(0)); 
	random=rand ();                             // 随机产生交叉点；
	random=(random %this->nodeNum);                     // 交叉点；
	for(i =0;i< random;i ++)                   
	{
		popnext[initNum - 1].bit [i]= popnext[0].bit [i];   // child 1 cross over
		popnext[initNum - 2].bit [i]= popnext[1].bit [i];   // child 2 cross over
		popnext[initNum - 3].bit [i]= popnext[0].bit [i];   // child 1 cross over
		popnext[initNum - 4].bit [i]= popnext[1].bit [i];   // child 2 cross over
	}

	for(i =random; i<this->nodeNum;i ++)                      // crossing the bits beyond the cross point index
	{
		popnext[initNum - 1].bit [i]= popnext[1].bit [i];    // child 1 cross over
		popnext[initNum - 2].bit [i]= popnext[0].bit [i];    // chlid 2 cross over
		popnext[initNum - 3].bit [i]= popnext[1].bit [i];   // child 1 cross over
		popnext[initNum - 4].bit [i]= popnext[0].bit [i];   // child 2 cross over
	}  

	for(i =initNum - 2;i<initNum; i++)
	{
		popnext[i].fit= y(x(popnext[i]));        // 为新个体计算适应度值；
	}

	/*for(i =0;i<initNum; i++)
	{
		cout << "popcurrent " << i << " \nbit is " << x(popcurrent[i]) << "\nfit is " << popcurrent[i].fit << endl;
		// 输出新个体；
	}*/
	return(0);
}                                          

void *GA::mutation (chrom* popnext, int initNum)               // 函数：变异操作；
{
	int random ;
	int row ,col;
	//srand(time(0)); 
	random=rand()%50;  // 随机产生到之间的数；
	//变异操作也要遵从一定的概率来进行，一般设置为0到0.5之间
	//
	if(random == 25)                              // random==25的概率只有2%，即变异率为，所以是以小概率进行变异！！
	{
		col=rand()%this->nodeNum;                            // 随机产生要变异的基因位号；
		row=rand()%(initNum - 1) + 1;                            // 随机产生要变异的染色体号；

		popnext[row].bit[col] = popnext[row].bit[col]==0?1:0;
		popnext[row].fit= y(x(popnext[row]));     // 计算变异后的适应度值；
		// 输出变异后的新个体；
	}                                          
	
	

	//每次最优的前四个变异，保存在最后一个
	col=rand()%this->nodeNum;                            // 随机产生要变异的基因位号；
	row=rand()%4;                            // 随机产生要变异的染色体号；
	popnext[initNum-1].bit[col] = popnext[initNum-1].bit[col]==0?1:0;     
	popnext[initNum-1].fit = y(x(popnext[initNum-1]));
	
	if(random > 40)                              // 每次有20的几率变异变异从第四个开始到最后一个，保存在倒数第二个
	{
		col=rand()%this->nodeNum;                            // 随机产生要变异的基因位号；
		row=rand()%(initNum - 4) + 4;                            // 随机产生要变异的染色体号；

		popnext[initNum-2].bit[col] = popnext[row].bit[col]==0?1:0;
		popnext[initNum-2].fit= y(x(popnext[row]));     // 计算变异后的适应度值；
	}

	return(0);
}   
