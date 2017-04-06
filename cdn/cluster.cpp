#include "cluster.h"
bool cluster::_consumeNode[MAX_NODE_NUM];
bool cluster::_dataNode[MAX_NODE_NUM];

/**
 * @brief _init 
 */
void cluster::_init()
{
	memset(_consumeNode, false, sizeof(_consumeNode));
	memset(_dataNode, false, sizeof(_dataNode));
}

/**
 * @brief split 
 *
 * @param {Interge} n
 */
void cluster::split(int n)
{
	_init();
	if (n <= 1) {
		return;
	}
	srand((unsigned) time(0));
	int index = 0;
	int nConsume = table.GetCNum();
	int costPerServer = table.GetServerCost();
	bool visited_Node[MAX_NODE_NUM];
	int consumeNodeIndexPos = rand() % nConsume;
	vector<int> extraIndex;
	while (true) {
		vector<int> nodeIndex;
		vector<int> consumeIndex;
		unordered_map<int, int> nodeExisted;
		memset(visited_Node, false, sizeof(visited_Node));
		Topology* pTopo = new Topology();
		int neighborId = -1;
		pTopo->SetGraphPara(table.GetVNum(), 0, table.GetCNum(), costPerServer);
		if (!_consumeNode[consumeNodeIndexPos]) {
			neighborId = table.GetConLinkNode(consumeNodeIndexPos);
			_area[neighborId].push_back(index);
			int demand = table.GetConDemand(consumeNodeIndexPos);
			pTopo->InsertConsume(consumeNodeIndexPos, neighborId, demand);
			_consumeNode[consumeNodeIndexPos] = true;
			consumeIndex.push_back(consumeNodeIndexPos);
		} else {
			bool flag = false;
			for (int i = 0; i < nConsume; i++) {
				if (!_consumeNode[i]) {
					consumeNodeIndexPos = i;
					flag = true;		
					break;
				}
			}
			if (flag) continue;
		}
		
		if (neighborId == -1) {
			break;
		}

		bool nodeFull = false;
		queue<int> q;
		q.push(neighborId);
		nodeIndex.push_back(neighborId);
		//nodeExisted[neighborId] = 1;
		visited_Node[neighborId] = true;
		while (!nodeFull && q.size() != 0) {
			int u = q.front();
			nodeExisted[u] = 1;
			q.pop();
			vector<int> bfs = table.GetNeiborNode(u);
			for (size_t k = 0; k < bfs.size(); k++) {
				int consumeId = table.GetNetLinkNode(bfs[k]);
				if (consumeId < nConsume) {
					if (!_consumeNode[consumeId]) {
						int demand = table.GetConDemand(consumeId);
						pTopo->InsertConsume(consumeId, bfs[k], demand);
						_consumeNode[consumeId] = true;
						consumeIndex.push_back(consumeId);
					}
				}
				if (!visited_Node[bfs[k]]) {
					q.push(bfs[k]);
					visited_Node[bfs[k]] = true;
					nodeIndex.push_back(bfs[k]);
					_area[bfs[k]].push_back(index);
					nodeExisted[bfs[k]] = 1;
				}
				if (nodeIndex.size() >= n) {
					nodeFull = true;
				}
			}
		}

		/**consider one consume cluster**/
		if (consumeIndex.size() <= 1) {
			for (size_t i = 0; i < consumeIndex.size(); i++) {
				extraIndex.push_back(consumeIndex[i]);
			}
			continue;
		}


		/**consider the nodes in many consume cluster**/
		//for (size_t p = 0; p < nodeIndex.size(); p++) {
			//vector<int> neighborIds = table.GetNeiborNode(nodeIndex[p]);
			//for (size_t w = 0; w < neighborIds.size(); w++) {
				//if (nodeExisted.count(neighborIds[w]) != 0) {
					//int cost = table.GetEdgecost(nodeIndex[p], neighborIds[w]);
					//int bandwidth = table.GetEdgebw(nodeIndex[p], neighborIds[w]);
					//pTopo->InsertOneEdge(nodeIndex[p], neighborIds[w], bandwidth, cost);
					//pTopo->InsertEdgev_v(nodeIndex[p], neighborIds[w], bandwidth, cost);
				//}
			//}
		//}

		consumeNodeIndexPos = consumeIndex.back();
		_consumeNode[consumeNodeIndexPos] = false;
		consumeIndex.pop_back();

		if (consumeIndex.size() != 0 && nodeIndex.size() != 0) {
			_nodeIndex.push_back(nodeIndex);
			_consumeIndex.push_back(consumeIndex);
			_clusters.push_back(*pTopo);
			_uMap.push_back(nodeExisted);
			index++;
		}
	}
	_mClusters = _nodeIndex.size();

	_dispreadIntoCluster(extraIndex);

	for (int i = 0; i < _mClusters; i++) {
		for (size_t p = 0; p < _nodeIndex[i].size(); p++) {
			int u = _nodeIndex[i][p];
			vector<int> neighborIds = table.GetNeiborNode(u);
			for (size_t w = 0; w < neighborIds.size(); w++) {
				if (_uMap[i].count(neighborIds[w]) != 0) {
					int cost = table.GetEdgecost(u, neighborIds[w]);
					int bandwidth = table.GetEdgebw(u, neighborIds[w]);
					_clusters[i].InsertOneEdge(u, neighborIds[w], bandwidth, cost);
					_clusters[i].InsertEdgev_v(u, neighborIds[w], bandwidth, cost);
				}
			}
		}
	}

	_consumeIndex.resize(_mClusters + 1);
	_nodeIndex.resize(_mClusters + 1);
	
}

/**
 * @brief _dispreadIntoCluster 
 *
 * @param {vector} extraConsumeIndex
 */
void cluster::_dispreadIntoCluster(vector<int> extraConsumeIndex)
{
	queue<int> temp;
	bool visited_Node[MAX_NODE_NUM];
	for (size_t i = 0; i < extraConsumeIndex.size(); i++) {
		int neighborId = table.GetConLinkNode(extraConsumeIndex[i]);
		if (_area[neighborId].size() != 0 && _mClusters != _area[neighborId][0]) {

			int area = _area[neighborId][0];
			int demand = table.GetConDemand(extraConsumeIndex[i]);
			_clusters[area].InsertConsume(extraConsumeIndex[i], neighborId, demand);

			_consumeIndex[area].push_back(extraConsumeIndex[i]);
		}
	}
}

/**
 * @brief split_flood 
 * 
 * @param {interge} n the number of cluster
 */
void cluster::split_flood(int n)
{
	_init();
	_mClusters = n;
	int *upperConsume = (int*)malloc(sizeof(int) * n);
	int *upperServer = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		upperConsume[i] = 0;
	}
	for (int i = 0; i < n; i++) {
		upperServer[i] = 0;
	}
	int costPerServer = table.GetServerCost();
	for (int i = 0; i < n; i++) {
		Topology* pTopo = new Topology();
		pTopo->SetGraphPara(table.GetVNum(), 0, table.GetCNum(), costPerServer);
		_clusters.push_back(*pTopo);
	}
	_consumeIndex.resize(n);
	_nodeIndex.resize(n);
	int count = 0;
	int nConsume = table.GetCNum();
	bool visited_Node[MAX_NODE_NUM];
	queue<node> q;
	srand((unsigned) time(0));
	vector<v_consum> consumer = table.getConsume();
	while (count < _mClusters) {
		int u = rand() %  nConsume;
		if (!_consumeNode[u] && consumer[u].v_id != -1) {
			node pNode;
			pNode.id = u;
			pNode.isConsume = true;
			pNode.index = count;
			_consumeIndex[count++].push_back(u);
			q.push(pNode);
			_consumeNode[u] = true;
		}
	}
	while (q.size()) {
		memset(_dataNode, false, sizeof(_dataNode));
		node pNode = q.front();
		q.pop();
		if (pNode.isConsume) {
			int neighborId = table.GetConLinkNode(pNode.id);
			if (!_dataNode[neighborId]) {
				node uNode;
				uNode.id = neighborId;
				uNode.isConsume = false;
				uNode.index = pNode.index;

				if (upperServer[uNode.index] <= 250) {
					q.push(uNode);
					_dataNode[neighborId] = true;
					_nodeIndex[uNode.index].push_back(neighborId);

					int demand = table.GetConDemand(pNode.id);
					_clusters[pNode.index].InsertConsume(pNode.id, neighborId, demand);
					upperServer[uNode.index]++;
				}
			}
		} else {
			vector<int> neighborIds = table.GetNeiborNode(pNode.id);
			for (size_t i = 0; i < neighborIds.size(); i++) {
				int u = neighborIds[i];
				int consumeId = table.GetNetLinkNode(u);
				if (consumeId < nConsume && !_consumeNode[consumeId]) {
					node uNode;
					uNode.id = consumeId;
					uNode.isConsume = true;
					uNode.index = pNode.index;

					if (upperConsume[uNode.index] <= 70) {
						q.push(uNode);
						_consumeNode[consumeId] = true;
				
						_consumeIndex[uNode.index].push_back(consumeId); 

						int demand = table.GetConDemand(consumeId);
						_clusters[pNode.index].InsertConsume(consumeId, u, demand);
						upperConsume[uNode.index]++;
					}
				} 
				if (!_dataNode[u]) {
					node uNode;
					uNode.id = u;
					uNode.isConsume = false;
					uNode.index = pNode.index;
					if (upperServer[uNode.index] <= 150) {
						q.push(uNode);
						_dataNode[u] = true;
						_nodeIndex[uNode.index].push_back(u);

						int cost = table.GetEdgecost(pNode.id, u);
						int bandwidth = table.GetEdgebw(pNode.id, u);
						_clusters[pNode.index].InsertEdge(pNode.id, u, bandwidth, cost);
						_clusters[pNode.index].InsertEdgev_v(pNode.id, u, bandwidth, cost);
						_clusters[pNode.index].InsertEdgev_v(u, pNode.id, bandwidth, cost);
						upperServer[uNode.index]++;
					}
				}
			}	
		}
	}
	//for (int i = 0; i < _nodeIndex[k].size(); i++) {
		 //vector<int> neibor = _clusters[k].GetNeiborNode(_nodeIndex[k][i]);
		 //cout<<"source:"<<_nodeIndex[k][i]<<endl;
		 //for (int j = 0; j < neibor.size(); j++) {
			//cout<<neibor[j]<<endl;
		 //}
	//}
	//cout<<endl;
}


/**
 * @brief setClustersNumber 
 *
 * @param {interge} n
 *
 * @return {cluster}
 */
cluster& cluster::setClustersNumber(int n)
{
	_mClusters = n;
	return *this;
}

/**
 * @brief getClusters 
 *
 * @return {vector<Topolog>}
 */
vector<Topology> cluster::getClusters()
{
	return _clusters;
}

/**
 * @brief getClustersNumber 
 *
 * @return {interge}
 */
int cluster::getClustersNumber()
{
	return _mClusters;
}

/**
 * @brief getClusterNode 
 *
 * @param {interge} k
 *
 * @return vector<int>
 */
vector<int> cluster::getClusterNode(int k)
{
	if (k > _mClusters) {
		return vector<int>();
	}
	return _nodeIndex[k];
}

/**
 * @brief getClustersConsume 
 *
 * @param {interge} k
 *
 * @return {vector<int>}
 */
vector<int> cluster::getClustersConsume(int k)
{
	if (k > _mClusters) {
		return vector<int>();
	}
	return _consumeIndex[k];
}

