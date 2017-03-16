#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<limits.h>
#include<stack>
#include"Init.h"
using namespace std;

GraphLinkedTable::GraphLinkedTable()
{
	v_count = 0;
	e_count = 0;
	c_count = 0;
	ServerCost = 0;
	gEdgeCount = 0;
	memset(gHead, -1, sizeof(int) * MAX_NODE_NUM);
}


void GraphLinkedTable::SetGraphPara(int v_n, int e_n, int c_n,int sc)
{
	v_count = v_n;
	e_count = e_n;
	c_count = c_n;
	ServerCost = sc;
	struct v_net v;
	for (int i = 0; i<v_n; i++)
	{
		v.v_id = i;
		vertex.push_back(v);
	}
}


void GraphLinkedTable::InsertEdge(int v1, int v2, int b, int c)
{
	edge ed;
	ed.v_next = v2;
	ed.bandwith = b;
	ed.cost = c;
	vertex[v1].edge_list.push_back(ed);
	ed.v_next = v1;
	vertex[v2].edge_list.push_back(ed);
}


void GraphLinkedTable::InsertEdgev_v(int v1, int v2, int b, int c)
{
	edgev_v ed;
	ed.in = v1;
	ed.out = v2;
	ed.bandwith = b;
	ed.cost = c;
	ed.next = gHead[v1];
	vec_edge.push_back(ed);
	gHead[v1] = gEdgeCount++;
}


void GraphLinkedTable::PrintEdgeInfo()
{
	int i;
	cout<<"下面打印边信息："<<endl;
	for( i=0;i<vec_edge.size();i++)
	{	cout<<vec_edge[i].in<<'\t'<<vec_edge[i].out<<'\t';
		cout<<vec_edge[i].bandwith<<'\t'<<vec_edge[i].cost<<endl;
	}
}


void GraphLinkedTable::InsertConsume(int c, int v, int dem)
{
	v_consum co;
	co.con_id = c;
	co.v_id = v;
	co.demand = dem;
	consume.push_back(co);
}


void GraphLinkedTable::PrintNetListInfo()
{
	cout << "下面打印网络节点信息：" << endl;
	for (int i = 0; i < v_count; i++)
	{
		for (int j = 0; j < vertex[i].edge_list.size(); j++)
		{
			cout << vertex[i].v_id << "\t" << vertex[i].edge_list[j].v_next << "\t";
			cout << vertex[i].edge_list[j].bandwith << "\t" << vertex[i].edge_list[j].cost << endl;
		}
	}
	cout << endl;
}


void GraphLinkedTable::PrintConNodeInfo()
{
	cout << "下面打印消费节点信息：" << endl;
	for (int i = 0; i < c_count; i++)
	  cout << consume[i].con_id << "\t" << consume[i].v_id << "\t" << consume[i].demand << endl;
	cout << endl;
}


stack<int> GraphLinkedTable::str_to_int(char *s)
{
	stack<int> num;
	char str[20];
	strcpy(str, s);
	char *rs = strtok(str, " ");

	while (rs != NULL)
	{
		num.push(atoi(rs));
		rs = strtok(NULL, " ");
	}
	return num;
}



int GraphLinkedTable::GetEdgebw(int v1, int v2)
{
	int i = 0;
	while (  i<vertex[v1].edge_list.size()&&(vertex[v1].edge_list[i].v_next != v2))
	{
		i++;
	}
	if (i<vertex[v1].edge_list.size())
	  return vertex[v1].edge_list[i].bandwith;
	else
	  return 0;
}


int GraphLinkedTable::GetEdgecost(int v1, int v2)
{
	int i = 0;
	while ( i<vertex[v1].edge_list.size()&& (vertex[v1].edge_list[i].v_next != v2))
	{
		i++;
	}
	if (i<vertex[v1].edge_list.size())
	  return vertex[v1].edge_list[i].cost;
	else
	  return INT_MAX;
}


vector<int> GraphLinkedTable::GetNeiborNode(int v)
{
	vector<int> nextNode;
	int i = 0;
	while (i < vertex[v].edge_list.size())
	{
		nextNode.push_back(vertex[v].edge_list[i].v_next);
		i++;
	}
	return nextNode;
}


int  GraphLinkedTable::GetConLinkNode(int cv)
{
	if (cv > c_count)
	  return INT_MAX;
	return consume[cv].v_id;
}


int  GraphLinkedTable::GetConDemand(int cv)
{
	if (cv >= c_count)
	  return INT_MAX;
	return consume[cv].demand;
}


int GraphLinkedTable::GetServerCost()
{
	return ServerCost;
}


int GraphLinkedTable::GetVNum()
{
	return v_count;
}


int GraphLinkedTable::GetCNum()
{
	return c_count;
}


int GraphLinkedTable::GetVEdge(int v)
{
	return vertex[v].edge_list.size();
}


void GraphLinkedTable::PrintHeader()  
{
	for (int e = gHead[51]; e != -1; e = vec_edge[e].next) {
		int v = vec_edge[e].out;
		cout<<"e:"<<e<<" v:"<<v<<endl;
	}
	//cout<<"Now headEdge:"<<endl;
	//for(int i=0;i<headEdge.size();i++)
	//cout<<i<<'\t'<<headEdge[i]<<endl;
	//cout<<"v2 Edege No:"<<endl;
	//cout<<headEdge[2]<<endl;
	//int formal=headEdge[2];
	//while(vec_edge[formal].next!=-1)
	//{
	//cout<<vec_edge[formal].next<<endl;
	//formal=vec_edge[formal].next;
	//}
}


int GraphLinkedTable::ReturnEdgeNo(int in,int out)
{
	int i=0;
	while(vec_edge.size()>i) 
	{
		if(vec_edge[i].in==in&& vec_edge[i].out==out)
		  return i;
		i++;
	}
	return -1;
}

void GraphLinkedTable::GraphInit(char * topo[MAX_EDGE_NUM], int line_num)
{
	char str[30];
	stack<int> in;
	int cur_lnum=0;
	while (cur_lnum<line_num)
	{
		strcpy(str, topo[cur_lnum]);
		in = str_to_int(str);
		int vn, en, cn;//输入GraphLinkedTable各参数信息
		cn = in.top();
		in.pop();
		en = in.top();
		in.pop();
		vn = in.top();
		in.pop();


		cur_lnum++;                  //空行
		cur_lnum++;
		strcpy(str, topo[cur_lnum]);
		SetGraphPara(vn, en, cn,atoi(str));
		cur_lnum++;                  //空行
		int bid, eid, bw, netcost;
		cur_lnum++;
		strcpy(str, topo[cur_lnum]);          //第一条边信息
		in = str_to_int(str);
		while (in.size() == 4)
		{
			netcost = in.top();
			in.pop();
			bw = in.top();
			in.pop();
			eid = in.top();
			in.pop();
			bid = in.top();
			in.pop();
			InsertEdge(bid, eid, bw, netcost);//插入边信息
			InsertEdgev_v(bid, eid, bw, netcost);
			InsertEdgev_v(eid, bid, bw, netcost);
			cur_lnum++;
			strcpy(str, topo[cur_lnum]);        //下一条边信息
			in = str_to_int(str);
		}


		int cid, lwid, dint;
		int curcv = 0;
		while (curcv<c_count)
		{
			cur_lnum++;
			strcpy(str, topo[cur_lnum]);           //第一个消费节点信息
			in = str_to_int(str);
			dint = in.top();
			in.pop();
			lwid = in.top();
			in.pop();
			cid = in.top();
			in.pop();
			InsertConsume(cid, lwid, dint);//插入消费节点信息
			curcv++;
		}
		cur_lnum++;
	}
	for(int i=0;i<v_count;i++)  
	{
		headEdge.push_back(ReturnEdgeNo(i,vertex[i].edge_list[0].v_next));
	}
}



