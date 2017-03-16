#ifndef _TOPOLOGY_H_
#define _TOPOLOGY_H_
#include "Init.h"
#include <queue>
#include <limits.h>
#include <algorithm>
struct Edge {
	int to;
	int vol;
	int cost;
	int next;
};

class Topology:public GraphLinkedTable
{
	private:
		static int gPre[MAX_NODE_NUM];
		static int gDist[MAX_NODE_NUM];
		static int gPath[MAX_NODE_NUM];
		vector<edgev_v> gEdge;
		int _originEdgeNums;
		int _virtualSource;
		int _virtualSink;
		bool _spfa(int s, int t);
		int _minCostFlow(int s, int t, vector<vector<int>>& path);
		void _init(vector<int> deploy);
		void _reset(vector<int> deploy);
	public:
		Topology() {
			GraphLinkedTable();
			_virtualSource = -1;
			_virtualSink = -1;
		    _originEdgeNums = 0;
		}
		void init();
		int minCostFlow(vector<int> deploy, vector<vector<int>>& path);
};
#endif
