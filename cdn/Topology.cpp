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
		int consume = GetNetLinkNode(temp.back());
		temp.push_back(consume);
		temp.push_back(f);
		path.push_back(temp);
	}
	if (flow < _maxDemand) {
		cost = 0;
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
	for (int i = 0; i < GetCNum(); i++) {
		_maxDemand += GetConDemand(i);
	}
}

/**
 * @brief minCostFlow 
 *
 * @param {vector<int>} deploy
 * @param {vector<vector<int>>} path
 * @param {vector<int>} exist
 *
 * @return {interge}
 */
int Topology::minCostFlow(vector<int> deploy, vector<vector<int>>& path, vector< int>& exist)
{
	_init(deploy);
	if (_virtualSource == -1 || _virtualSink == -1) {
		cout<<"Must init() first"<<endl;
		return -1;
	}
	int cost = _minCostFlow(_virtualSource, _virtualSink, path);
	unordered_map<int, int> existed;
	for (int i = 0; i < path.size(); i++) {
		if (existed.count(path[i][0]) == 0) {
			existed[path[i][0]]  = 1;
			exist.push_back(path[i][0]);
		}
	}
	_reset(deploy);
	if (cost == 0) {
		return INT_MAX;
	} else {
		//return cost + GetServerCost() * count;
		return cost;
	}
}

/**
 * @brief initializePreflow 
 *
 * @param {interge} s
 */
void Topology::initializePreflow(int s)
{
	memset(h, 0, sizeof(int) * n);
	h[s] = n;
	memset(e, 0, sizeof(int) * n);
	for (int i = n - 1; i >= 0; i--) {
		memset(f[i], 0, sizeof(int) * n);
	}
	for (int eId = gHead[s]; eId != -1; eId = vec_edge[eId].next) {
		int v = vec_edge[eId].out;
		e[v] = vec_edge[eId].bandwith;
		e[s] -= vec_edge[eId].bandwith;
	}
}

/**
 * @brief push 
 *
 * @param {interge} u
 * @param {interge} v
 */
void Topology::push(int u, int v) 
{
	for (int eId = gHead[u]; eId != -1; eId = vec_edge[eId].next) {
		if (vec_edge[eId].out == v) {
			int temp = min(e[u], vec_edge[eId].bandwith);
			e[u] = e[u] - temp;
			e[v] = e[v] + temp;
			vec_edge[eId].bandwith -=temp;
			vec_edge[eId ^ 1].bandwith += temp;
			break;
		}
	}
}

/**
 * @brief relabel 
 *
 * @param {interge} u
 */
void Topology::relabel(int u)
{
	int temp = -1;
	for (int eId = gHead[u]; eId != -1; eId = vec_edge[eId].next) {
		if (vec_edge[eId].bandwith > 0) {
			if (temp == -1 || temp > h[vec_edge[eId].out]) {
				temp = h[v];
			}
		}
	}
	h[u] = 1 + temp;
}

/**
 * @brief maxflow 
 *
 * @param {interge} s
 * @param {interge} t
 */
void Topology::maxflow(int s, int t)
{
	initializePreflow();
	queue<int> q;
	char *l = new char[n];
	int u, v, m;
	memset(l, 0, sizeof(char) * n);
	for (int eId = gHead[s]; eId != -1; eId = vec_edge[eId].next) {
		if (vec_edge[eId].out != t) {
			q.push(vec_edge[eId].out);
			l[vec_edge[eId].out] = 1;
		}
	}
	while (q.size() != 0) {
		u = q.front();
		m = -1;
		for (int eId = gHead[u]; eId != -1; eId = vec_edge[eId].next) {
			v = vec_edge[eId].out;
			if (vec_edge[eId].bandwith > 0) {
				if (h[u] > h[v]) {
					push(u, v);
					if (l[v] == 0 && v != s && v != t) {
						l[v] = 1;					
						q.push(v);
					}
				} else if (m == -1) {
					m = h[v];
				} else {
					m = min(m, h[v]);
				}
			}
		}
		if (e[u] != 0) {
			h[u] = 1 + m;
		} else {
			l[u] = 0;
			q.pop();
		}
	}
}
