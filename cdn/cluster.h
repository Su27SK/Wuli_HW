#ifndef _CLUSTER_H_
#define _CLUSTER_H_
#include "Topology.h"
struct node
{
	int id;
	bool isConsume;
	int index;
};

class cluster
{
	private:
		vector<Topology> _clusters;
		int _mClusters;
		vector<vector<int> > _consumeIndex;
		vector<vector<int> > _nodeIndex;
		vector<unordered_map<int, int> >  _uMap;
		GraphLinkedTable table;
		vector<int> _area[MAX_NODE_NUM];
		static bool _consumeNode[MAX_NODE_NUM];
		static bool _dataNode[MAX_NODE_NUM];
		void _init();
		void _dispreadIntoCluster(vector<int> extraIndex);
	public:
		cluster(GraphLinkedTable& vtable) {
			table = vtable;
			_mClusters = 0;
		}
		void split(int n);	
		void split_flood(int n);
		cluster& setClustersNumber(int n);
		vector<Topology> getClusters();
		int getClustersNumber();
		vector<int> getClusterNode(int k);
		vector<int> getClustersConsume(int k);
};
#endif
