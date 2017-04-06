#include "deploy.h"
#include "geneticAL.h"
#include "cluster.h"
#include <stdio.h>
#include <time.h>
vector<int> testMaxflow()
{
	FILE* handle = fopen("../bin/output.txt", "r");
	vector<int> deploy;
	while (!feof(handle)) {
		char s[50];
		fgets(s, 50, handle);
		string messages(s);
		int i = atoi(messages.substr(0, messages.find(" ")).c_str());
		deploy.push_back(i);
	}
	return deploy;
}

char* vec_to_int(vector<vector<int >> path)
{
	char* in=(char*)malloc(sizeof(int)+1);
	string str;	
	int pi = path.size();
	sprintf(in, "%d", pi);
	str += in;
	str += "\n\n";
	for (size_t i = 0; i< path.size() ;i++)
	{
		for (size_t j = 0; j < path[i].size(); j++)
		{
			pi = path[i][j];
			sprintf(in, "%d", pi);
			str += in;
			if (j != path[i].size() - 1) {
				str += " ";
			}
		}
		if (i != path.size() - 1) {
			str += "\n";
		}
	}
	int len = str.length();
	char *topo = (char *)malloc((len + 1)*sizeof(char));
	strcpy(topo, str.c_str());
	free(in);
	return topo;
};

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	double start, stop;
	int minCost = INT_MAX;
	start = clock();
	Topology T;
	T.GraphInit(topo, line_num);
	T.init();
	//T.PrintEdgeInfo();
	int k = 100;
	cluster* pCluster = new cluster(T);
	pCluster->split_flood(8);
	vector<Topology> clusters = pCluster->getClusters();
	//cluster* pSubCluster1 = new cluster(clusters[0]);
	//pSubCluster1->split_flood(2);
	//cluster* pSubCluster2 = new cluster(clusters[1]);
	//pSubCluster2->split_flood(2);
	//pCluster->split(100);
	//int allNode = 0;
	//vector<int> consumes = pCluster->getClustersConsume(0);
	//vector<int> datanodes = pCluster->getClusterNode(0);
	//vector<int> deploy_test;
	//vector<int> deploy;
	//cout<<"cluster_size:"<<clusters.size()<<endl;
	//for (int i = 0; i < consumes.size(); i++) {
		//deploy_test.push_back(clusters[1].GetConLinkNode(consumes[i]));
	//}
	//for (size_t i = 0; i < datanodes.size(); i++) {
		//bool flag = true;
		 //for (size_t j = 0; j < deploy_test.size(); j++) {
			//if (datanodes[i] == deploy_test[j]) {
				//flag = false;
				//break;
			//}
		//}
		//if (flag) {
			//deploy.push_back(datanodes[i]);
		//}
	//}
	//for (int i = 0; i < consumes.size(); i++) {
		//deploy.push_back(clusters[0].GetConLinkNode(consumes[i]));
	//}
	//cout<<"size:"<<datanodes.size()<<endl;
	//for (int i = 0; i < deploy.size(); i++) {
		//cout<<"i:"<<deploy[i]<<endl;
	//}
	
	//vector<vector<int>> route;
	//vector<int> existed;
	//clusters[1].init();
	//cout<<"minCostFlow:"<<clusters[1].minCostFlow(deploy, route, existed)<<endl;
	//for (int i = 0; i < clusters.size(); i++) {
		////cout<<"indexClusters:"<<i<<endl;
		//vector<v_consum> consumer = clusters[i].getConsume();
		//int count = 0;
		//for (int j  = 0; j < consumer.size(); j++) {
			//if (consumer[j].con_id != -1) {
				//count++;
			//}
		//}
		//allNode += count;
		////cout<<"allNode:"<<allNode<<endl;
		////cout<<"count:"<<count<<endl;
	//}

	//vector<vector<int>> route;
	//vector<int> existed;
	
	int sumConsume = 0;
	int sumNode = 0;
	cout<<"cluster_nums:"<<pCluster->getClustersNumber()<<endl;
	for (int i = 0; i < pCluster->getClustersNumber(); i++) {
		vector<int> ids = pCluster->getClusterNode(i);
		vector<int> consumeIds = pCluster->getClustersConsume(i);
		cout<<"i:"<<i<<" node_ids:"<<ids.size()<<endl;
		unordered_map<int, int> test;
		//cout<<"cluster_index:"<<i<<" ";
		cout<<"consume_ids:"<<consumeIds.size()<<endl;
		sumConsume += consumeIds.size();
		sumNode += ids.size();
		for (int i = 0; i < ids.size(); i++) {
			if (test.count(ids[i]) == 0) {
				test[ids[i]] = 1;
			//} else {
				//cout<<"Error !\n"<<endl;
			}
			//cout<<ids[i]<<" ";
		}
		//cout<<endl;
	}
	cout<<"sumConsume:"<<sumConsume<<endl;
	cout<<"sumNode:"<<sumNode<<endl;

	//int sumConsume1 = 0;
	//int sumNode1 = 0;
	//cout<<"cluster_nums:"<<pSubCluster1->getClustersNumber()<<endl;
	//for (int i = 0; i < pSubCluster1->getClustersNumber(); i++) {
		//vector<int> ids = pSubCluster1->getClusterNode(i);
		//vector<int> consumeIds = pSubCluster1->getClustersConsume(i);
		//cout<<"i:"<<i<<" node_ids:"<<ids.size()<<endl;
		//unordered_map<int, int> test;
		////cout<<"cluster_index:"<<i<<" ";
		//cout<<"consume_ids:"<<consumeIds.size()<<endl;
		//sumConsume += consumeIds.size();
		//sumNode += ids.size();
		//for (int i = 0; i < ids.size(); i++) {
			//if (test.count(ids[i]) == 0) {
				//test[ids[i]] = 1;
			//} else {
				//cout<<"Error !\n"<<endl;
			//}
			////cout<<ids[i]<<" ";
		//}
		////cout<<endl;
	//}
	//cout<<"sumConsume1:"<<sumConsume1<<endl;
	//cout<<"sumNode1:"<<sumNode1<<endl;

	//cluster* pCluster2 = new cluster(clusters[0]);
	//pCluster2->split_flood(2);
	
	//for (int i = 0; i < pCluster2->getClustersNumber(); i++) {
		//vector<int> ids = pCluster2->getClusterNode(i);
		//vector<int> consumeIds = pCluster2->getClustersConsume(i);
		//cout<<"node_ids:"<<ids.size()<<endl;
		////cout<<"cluster_index:"<<i<<" ";
		//cout<<"consume_ids:"<<consumeIds.size()<<endl;
	//}

	//vector<int> test;
	//test.push_back(38);
	//test.push_back(29);
	////test.push_back(31);
	//test.push_back(48);
	//test.push_back(41);
	////for (int i = 0; i  < ids.size() / 3; i++) {
		////test.push_back(ids[i]);
	////}
	//clusters[0].init();
	//cout<<"origin:";
	//for (int i = 0; i < test.size(); i++) {
		//cout<<test[i]<<" ";
	//}
	//cout<<endl;

	//clusters[0].PrintEdgeInfo();
	//cout<<"cost:"<<clusters[0].minCostFlow(test, route, existed)<<endl;
	//cout<<"cost:"<<clusters[0].minCostFlow(test, route, existed)<<endl;

	//vector<int> test;
	//vector<int> existed;
	//vector<vector<int>> route;
	//vector<vector<int>> route2;
	//test.push_back(38);
	//test.push_back(23);
	//test.push_back(18);
	////test.push_back(29);
	//test.push_back(12);
	////test.push_back(31);
	////test.push_back(48);
	//test.push_back(4);
	//test.push_back(13);
	//cout<<"cost:"<<T.minCostFlow(test, route,  existed)<<endl;
	//vector<int> existed2;
	//cout<<"cost:"<<T.minCostFlow(existed, route2, existed2)<<endl;
	//cout<<"test_size:"<<test.size()<<endl;
	//cout<<"existed_size:"<<existed.size()<<endl;
	//cout<<"existed2_size:"<<existed2.size()<<endl;
	//for (int i = 0; i < existed.size(); i++) {
		//cout<<existed[i]<<" ";
	//}
	//cout<<endl;
	//cout<<"existed_size:"<<existed2.size()<<endl;
	//for (int i = 0; i < existed2.size(); i++) {
		//cout<<existed2[i]<<" ";
	//}
	//cout<<endl;
	//for (int i = 0; i < existed.size(); i++) {
		//cout<<existed[i]<<" ";
	//}
	//cout<<endl;
	//cout<<"node_size:"<<T.GetVNum()<<endl;

	//unordered_map<int, int> order;
	//vector<int> position = testMaxflow();
	//vector<int> truePosition;
	//for (int i = 0; i < position.size(); i++) {
		//if (order.count(position[i]) == 0) {
			//truePosition.push_back(position[i]);
			//order[position[i]] = 1;
		//}
	//}
	//cout<<"size:"<<truePosition.size()<<endl;
	
	//vector<vector<int>> route;
	//vector<int> existed;
	//cout<<T.minCostFlow(truePosition, route, existed)<<endl; 
	//cout<<"existed:"<<existed.size()<<endl;
	
	//int initNum = T.GetVNum()>100?(T.GetVNum() > 300?200:T.GetVNum()):100;
	//GA ga(T, 10000, 1 *  initNum, 1.5 *  initNum);
	
	//vector<vector<int>> route;

	//cout << "vertex number is " << T.GetVNum() << endl;
	//for(int i = 0; i < 1; i++){
		//int cost;
		//vector<vector<int>> buf = ga.getBestServersPos(cost);	
		//if(cost < minCost){
			//minCost = cost;
			//route.swap(buf);
		//}
	//}

	//cout << "finally cost is " << minCost << endl;
	//char * topo_file = vec_to_int(route2);

	//char* topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	//stop = clock();
	//cout << "程序耗时:" << ((double)(stop-start))/CLOCKS_PER_SEC << "s\n\n\n" << endl;	

	//write_result(topo_file, filename);
}
