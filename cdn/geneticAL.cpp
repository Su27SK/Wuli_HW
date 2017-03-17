#include <string>
#include <math.h>
#include "geneticAL.h"
#include <algorithm>

#define INIT_Min 0x7fffffff

using namespace std;

bool comparison(Chrom a, Chrom b)
{
	return a.fit<b.fit;
}

vector<vector<int>> GA::getBestServersPos(int initNum)
{
	chrom* popcurrent = new struct Chrom[initNum];
	chrom* popnext = new struct Chrom[initNum];

    T.PrintConNodeInfo();
	int i ,j, l,Min;
	Min=INIT_Min;                                      
	vector<int> k;	

	srand(time(0));
	evpop(popcurrent, initNum);	
	sort(popnext, popnext+initNum, comparison);

	Min = popcurrent[0].fit;

	
	for(i =0;i< this->iteration;i++)                        
	{

		for(j =0;j<initNum; j++)
		{
			popnext[j]=popcurrent[j];           
		}
		//pickchroms(popnext, initNum);
		//pickchroms_new(popcurrent, popnext, initNum);                    

		crossover(popnext, initNum);                     
		sort(popnext, popnext+initNum, comparison);
		mutation(popnext, initNum);                      
		sort(popnext, popnext+initNum, comparison);

		for(j =0;j<initNum; j++) 
		{
			//cout << "popnext " << j << " \nbit is " << x(popnext[j]) << "\nfit is " << popnext[j].fit << endl;
			popcurrent[j]=popnext[j];             
		}

	}  

	for(l =0;l<initNum; l++)
	{
		if(popcurrent[l].fit < Min)
		{
			Min=popcurrent[l].fit;
			k = x(popcurrent[l]);
		}

	}

	vector<vector<int>> route;
	vector<vector<int>>& path = route;

	int cost = T.minCostFlow(k, path); 
	if( cost > INT_MAX - 100){
		vector<vector<int>> a;
		return a;
	}
	else
		return route;


}

void *GA::evpop(chrom* popcurrent, int initNum)   
{
	int i ,j;
	int random ;
	double sum=0;
	int pro = T.GetVNum() / T.GetCNum();
	int middle = pro / 2;
	//cout << "V num is " << T.GetVNum() << "\nc num is " << T.GetCNum() << "\npro is " << pro << "\nmiddle is " << middle << endl;
	for(j =0;j<initNum; j++)                           
	{	
		//cout << "init num is " << j << endl;
		for(i =0;i<this->nodeNum; i++)                       
		{
			random=rand ();                     
			random=(random % pro);                 
			//cout << "random is " << random << endl;
			popcurrent[j].bit[i]= random == middle?1:0;       
		}  

		popcurrent[j].fit= y(x(popcurrent[j])); 		
		sum = sum + popcurrent[j].fit;
		//cout << "popcurrent " << j << " \nbit is " << x(popcurrent[j ]) << "\nfit is " << popcurrent[j ].fit << endl;

	}
	
	for (j = 0; j < initNum; j++)
	{
		popcurrent[j].rfit = popcurrent[j].fit/sum;
		popcurrent[j].cfit = 0;
	}
	return(0);                
}            

vector<int> GA::x(chrom popcurrent)  
{

	vector<int> selectNode;
	for(int i = 0; i < this->nodeNum; i++){
		if(popcurrent.bit[i]){
			selectNode.push_back(i);
		}
	}
	return selectNode;                           
}                                     


int GA::y(vector<int> x)
{
	if(x.empty()){
		return INIT_Min;
	}
	int cost = INIT_Min;
	vector<vector<int>> route;
	vector<vector<int>>& path = route;
	cost = T.minCostFlow(x, path);
	if(cost != INIT_Min){
		cost += T.GetServerCost() * x.size();
	}
	
	/*if(hashFit.find(x) == hashFit.end()) {	
		
		cost = T.minCostFlow(x, path);
		if(cost != INIT_Min){
			cost += T.GetServerCost() * x.size();
		}

		//hashFit[x] = cost;

	}
	else  {
		//cost = hashFit[x];
		cost = T.minCostFlow(x, path);
	}*/
	return(cost);             
} 


void *GA::pickchroms_new (chrom* popcurrent, chrom* popnext, int initNum)
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
	//calculate the cumulative fitness,
	popcurrent[1].cfit = popcurrent[1].rfit;
	for ( men = 1; men < initNum; men++)
	{
		popnext[men].cfit = popnext[men-1].cfit + popnext[men].rfit;
	}

	for ( i = 1; i < initNum; i++ )
	{
		
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


void *GA::crossover (chrom* popnext, int initNum)             
{

	int random ;
	int i;
	random=rand ();                             
	int subCross = random % initNum;
	random=(random %this->nodeNum);                   
	for(i =0;i< random;i ++)                   
	{
		popnext[initNum - 1].bit [i]= popnext[0].bit [i];   // child 1 cross over
		popnext[initNum - 2].bit [i]= popnext[1].bit [i];   // child 2 cross over
		popnext[initNum - 3].bit [i]= popnext[0].bit [i];   // child 1 cross over
		popnext[initNum - 4].bit [i]= popnext[subCross].bit [i];   // child 2 cross over
	}

	for(i =random; i<this->nodeNum;i ++)                      // crossing the bits beyond the cross point index
	{
		popnext[initNum - 1].bit [i]= popnext[1].bit [i];    // child 1 cross over
		popnext[initNum - 2].bit [i]= popnext[0].bit [i];    // chlid 2 cross over
		popnext[initNum - 3].bit [i]= popnext[subCross].bit [i];   // child 1 cross over
		popnext[initNum - 4].bit [i]= popnext[0].bit [i];   // child 2 cross over
	}  

	for(i =initNum - 2;i<initNum; i++)
	{
		popnext[i].fit= y(x(popnext[i]));        
	}

	return(0);
}                                          

void *GA::mutation (chrom* popnext, int initNum)            
{
	int random ;
	int row ,col;
	//srand(time(0)); 
	random=rand()%50;  

	//
	if(random == 25)                             
	{
		col=rand()%this->nodeNum;                            
		row=rand()%(initNum - 2) + 2;                            

		popnext[row].bit[col] = popnext[row].bit[col]==0?1:0;
		popnext[row].fit= y(x(popnext[row]));     
		
	}                                          



	
	col=rand()%this->nodeNum;                            
	row=rand()%4;                           
	popnext[initNum-1].bit[col] = popnext[initNum-1].bit[col]==0?1:0;     
	popnext[initNum-1].fit = y(x(popnext[initNum-1]));

	if(random > 40)                              
	{
		
		col=rand()%this->nodeNum;                            
		row=rand()%(initNum / 3);                            

		popnext[initNum-2].bit[col] = popnext[row].bit[col]==0?1:0;
		popnext[initNum-2].fit= y(x(popnext[row]));    
	}

	return(0);
}   
