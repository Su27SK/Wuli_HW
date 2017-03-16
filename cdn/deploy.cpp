#include "deploy.h"
//#include "Topology.h"
#include "geneticAL.h"
#include <stdio.h>

char* vec_to_int(vector<vector<int >> path)
{
//	int nValue=80;
	char* in=(char*)malloc(sizeof(int)+1);
	string str;	
//	char in[10];
	int pi = path.size();
	sprintf(in, "%d", pi);
	str += in;
	str += "\n\n";
	for (int i = 0; i< path.size() ;i++)
	{
	for (int j = 0; j < path[i].size(); j++)
		{
			pi = path[i][j];
			sprintf(in, "%d", pi);
			str += in;
			str += " ";
		}
	str += "\n";
	}
	int len = str.length();
	char *topo = (char *)malloc((len + 1)*sizeof(char));
	strcpy(topo, str.c_str());
	free(in);
	 return topo;
};

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	//GraphLinkedTable G;
	//G.GraphInit(topo,line_num);
	//vector<int> in;

	//G.PrintNetListInfo();
	//G.PrintConNodeInfo();
	
	Topology T;
	T.GraphInit(topo, line_num);
	T.init();
	//T.PrintHeader();
/*	vector<int> test;
	test.push_back(0);
    	test.push_back(6);
    	test.push_back(8);
    	test.push_back(9);
    	test.push_back(12);
    	test.push_back(26);
    	test.push_back(33);
    	test.push_back(36);
    	test.push_back(42);
    	test.push_back(47);
    	test.push_back(49);
	vector<vector<int>> route;
	vector<vector<int>>& path = route;
	cout<<"minCost:"<<T.minCostFlow(test, path)<<endl;
	for (size_t i = 0; i < path.size(); i++) {
		for (size_t j = 0; j < path[i].size(); j++) {
			cout<<path[i][j]<<" ";
		}
		cout<<endl;
	}
*/

	//Topology T;
	//T.GraphInit(topo, line_num);

	GA ga(T, 10000);
	vector<vector<int>> route = ga.getBestServersPos(60);
	
	

	//T.PrintHeader();
	// 需要输出的内容
	char * topo_file = vec_to_int(route);//(char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}
