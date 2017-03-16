#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<stack>
using namespace std;
#define MAX_EDGE_NUM    (2000 * 20)
#define MAX_NODE_NUM    1600

struct edge            //与某一点相连的边信息
{
	int v_next;      //进入
	int bandwith;   //带宽 
	int cost;        //单位成本
};

struct edgev_v            //边信息
{
	int in;      
	int out;      
	int bandwith;   //带宽 
	int cost;        //单位成本
	int next=-1;       //
};
struct v_net
{
	int v_id;
	bool is_consumer = false;         //是否连接消费节点
	vector<edge>  edge_list;          //连接的边链表
};
struct v_consum
{
	int con_id;
	int v_id;                    //连接的网络节点
	int demand;                  //消费需求
};

class GraphLinkedTable
{
private:
	vector<v_net> vertex;
	vector<v_consum> consume;
	int v_count;               //网络节点数量
	int e_count;               //边数量
	int c_count;               //消费节点数量
	int ServerCost;            //服务器成本
protected:
	vector<edgev_v> vec_edge;      //所有边信息数组
	vector<int> headEdge;           //每个节点的第一条边编号
	int gHead[MAX_NODE_NUM];
	int gEdgeCount;            //边数量自增
public:
	GraphLinkedTable();
	~GraphLinkedTable(){}
	void SetGraphPara(int v_n, int e_n, int c_n,int sc);  
	void InsertEdge(int v1, int v2, int b, int c);       //插入边信息
	void InsertEdgev_v(int v1, int v2, int b, int c);       //插入所有的边信息
	void InsertConsume(int c, int v, int dem);           //插入消费节点信息
	void PrintNetListInfo();                             //打印网络节点信息
	void  PrintConNodeInfo();                             //打印消费节点信息
	void PrintEdgeInfo();                                 //打印所有边信息
	int GetEdgebw(int v1, int v2);                       //返回两节点之间的边带宽，如果两点之间不存在边，则返回0
	int GetEdgecost(int v1, int v2);                     //返回两节点之间的单位成本，如果两点之间不存在边，则返回无穷大
	vector<int> GetNeiborNode(int v);                    //返回相邻节点列表
	int GetConLinkNode(int cv);                          //返回与消费节点相邻的网络节点，如果不存在该消费节点，则返回无穷大
	int GetConDemand(int cv);                            //返回与消费节点的需求，如果不存在该节点，返回无穷大
	int GetServerCost();                                 //返回服务器成本
	int GetVNum();                                       //返回网络节点数量
	int GetCNum();                                       //返回消费节点数量
	int GetVEdge(int v);                                 //返回该网络节点的度
	void GraphInit(char * topo[MAX_EDGE_NUM], int line_num);
	stack<int> str_to_int(char *s);
	int ReturnEdgeNo(int in,int out);                     //fanhui wangluojiedian d diyitiaobian bianhao
	void PrintHeader();                                    //dayin suoyou jiedian de diyitiao bian bianhao
	void SetNextEdgeNo();
};



