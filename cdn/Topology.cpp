#include "Topology.h"
int Topology::gPre[MAX_NODE_NUM];
int Topology::gDist[MAX_NODE_NUM];
int Topology::gPath[MAX_NODE_NUM];

/**
 * @brief _init 
 *
 * @param {vector<int>} deploy
 */
void Topology::_init(vector<int> deploy)
{
	_virtualSource = GetVNum();
	_virtualSink = GetVNum() + 1;
	for (size_t i = 0; i < deploy.size(); i++) {  //增加到超级节点到虚拟节点的链路
		InsertEdgev_v(_virtualSource, deploy[i], INT_MAX, 0);
		InsertEdgev_v(deploy[i], _virtualSource, 0, 0);
	}
	for (int i = 0; i < GetCNum(); i++) {  //增加消费节点到虚拟终点的链路
		int nearId = GetConLinkNode(i);
		int bandwith = GetConDemand(i);
		InsertEdgev_v(nearId, _virtualSink, bandwith, 0);
		InsertEdgev_v(_virtualSink, nearId, 0, 0);
	}
}

/**
 * @brief _reset 
 *
 * @param {vector<int>} deploy
 */
void Topology::_reset(vector<int> deploy)
{
	gHead[_virtualSource] = -1;
	gHead[_virtualSink] = -1;
	vec_edge.assign(gEdge.begin(), gEdge.end());
	for (size_t i = 0; i < deploy.size(); i++) {
		while (gHead[deploy[i]] >= _originEdgeNums) {
			gHead[deploy[i]] = vec_edge[gHead[deploy[i]]].next;
		}
	}
	for (int i = 0; i < GetCNum(); i++) {  //增加消费节点到虚拟终点的链路
		int nearId = GetConLinkNode(i);
		while (gHead[nearId] >= _originEdgeNums) {
			gHead[nearId] = vec_edge[gHead[nearId]].next;
		}
	}
	gEdgeCount = _originEdgeNums;
}

/**
 * @brief _spfa 
 *
 * @param {interge} s
 * @param {interge} t
 *
 * @return {boolean}
 */
bool Topology::_spfa(int s, int t) 
{
	memset(gPre, -1, sizeof(int) * MAX_NODE_NUM);
	memset(gDist, 0x7F, sizeof(int) * MAX_NODE_NUM);
	gDist[s] = 0;
	queue<int> Q;
	Q.push(s);
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop();

		for (int e = gHead[u]; e != -1; e = vec_edge[e].next) {
			int v = vec_edge[e].out;
			if (vec_edge[e].bandwith > 0 && gDist[u] + vec_edge[e].cost < gDist[v]) {
				gDist[v] = gDist[u] + vec_edge[e].cost;
				gPre[v] = u; //前一个节点
				gPath[v] = e; //改点连接的前一条边
				Q.push(v);
			}
		}
	}

	if (gPre[t] == -1) {
		return false;
	}
	return true;
}

/**
 * @brief _minCostFlow 
 *
 * @param {interge} s
 * @param {interge} t
 * @param {vector<vector<int>>} path
 *
 * @return {interge}
 */
int Topology::_minCostFlow(int s, int t, vector<vector<int>>& path) 
{
	int cost = 0;
	int flow = 0;
	while (_spfa(s, t)) {
		int f = INT_MAX;
		for (int u = t; u != s; u = gPre[u]) {
			if (vec_edge[gPath[u]].bandwith  < f) {
				f = vec_edge[gPath[u]].bandwith;
			}
		}
		flow += f;
		cost += gDist[t] * f;
		vector<int> temp;
		for (int u = t; u != s; u = gPre[u]) {
			temp.push_back(u);
			vec_edge[gPath[u]].bandwith -= f;   //正向边容量减少
			vec_edge[gPath[u]^1].bandwith += f; //反向边容量增加
		}
		reverse(temp.begin(), temp.end());
		temp.pop_back();
		temp.push_back(f);
		path.push_back(temp);
	}
	return cost;
}

/**
 * @brief init 
 */
void Topology::init()
{
	gEdge.assign(vec_edge.begin(), vec_edge.end());
	_originEdgeNums = gEdge.size();
}

/**
 * @brief minCostFlow 
 *
 * @param {vector<int>} deploy
 * @param {vector<vector<int>>} path
 *
 * @return {interge}
 */
int Topology::minCostFlow(vector<int> deploy, vector<vector<int>>& path)
{
	_init(deploy);
	if (_virtualSource == -1 || _virtualSink == -1) {
		cout<<"Must init() first"<<endl;
		return -1;
	}
	int cost = _minCostFlow(_virtualSource, _virtualSink, path);
	//cout<<"minCost:"<<cost<<endl;
	_reset(deploy);
	return cost == 0 ? INT_MAX: cost;
}
