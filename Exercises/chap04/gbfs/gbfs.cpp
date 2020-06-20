#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct WeightedEdge
{
	// Which nodes are connected by this edge?
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	// Weight of this edge
	float mWeight;
};

struct WeightedGraphNode
{
	std::vector<WeightedEdge*> mEdges;
	int name;
};

struct WeightedGraph
{
	std::vector<WeightedGraphNode*> mNodes;
};

struct GBFSScratch
{
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

using GBFSMap =
std::unordered_map<const WeightedGraphNode*, GBFSScratch>;

float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	float x = (b->name % 10) - (a->name % 10);
	float y = (b->name / 10) - (a->name / 10);
	return x + y;
}

float ComputeWeight(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	float x = (a->name % 10) - (b->name % 10);
	float y = (a->name / 10) - (b->name / 10);
	return x + y;
}

bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// Set current node to start, and mark in closed set
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// Add adjacent nodes to open set
		for (const WeightedEdge* edge : current->mEdges)
		{
			// Get scratch data for this node
			GBFSScratch& data = outMap[edge->mTo];
			// Add it only if it's not in the closed set
			if (!data.mInClosedSet)
			{
				// Set the adjacent node's parent edge
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					// Compute the heuristic for this node, and add to open set
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		// If open set is empty, all possible paths are exhausted
		if (openSet.empty())
		{
			break;
		}

		// Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		// Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// Did we find a path?
	return (current == goal) ? true : false;
}

std::string join(const std::vector<WeightedEdge*>& v, const char * delim = 0)
{
	std::string s;
	if (!v.empty())
	{
		s += "(" + std::to_string(v[0]->mFrom->name) + ", " + std::to_string(v[0]->mTo->name) +
		   ", " + std::to_string(v[0]->mWeight) + ")";
		for (decltype(v.size()) i = 1, c = v.size(); i < c; i++)
		{
			if (delim) s += delim;
			s += "(" + std::to_string(v[i]->mFrom->name) + ", " + std::to_string(v[i]->mTo->name)
				 + ", " + std::to_string(v[i]->mWeight) + ")";
		}
	}
	return s;
}

void printGraph(const WeightedGraph& g)
{
	std::cout << "Graph\n\n";
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = g.mNodes[i * 5 + j];
			std::cout << node->name << ": [" << join(node->mEdges, ", ") << "]\n";
		}
	}
}

void printRoute(const WeightedGraph& g, GBFSMap& map)
{
	const WeightedGraphNode* child = g.mNodes[9];
	std::cout << "GOAL : " << child->name;
	while (true)
	{
		const WeightedGraphNode* parent = map[child].mParentEdge->mFrom;
		if (parent == nullptr)
		{
			break;
		}
		std::cout << " <- " << parent->name;
		if (parent == g.mNodes[0])
		{
			std::cout << " : START\n";
			break;
		}
		child = parent;
	}
}

void testHeuristic()
{
	WeightedGraph g;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = new WeightedGraphNode;
			node->name = i * 10 + j;
			g.mNodes.emplace_back(node);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			WeightedGraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i - 1) * 5 + j];
				e->mWeight = ComputeWeight(node, g.mNodes[0]);
				node->mEdges.emplace_back(e);
			}
			if (i < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i + 1) * 5 + j];
				e->mWeight = ComputeWeight(node, g.mNodes[0]);
				node->mEdges.emplace_back(e);
			}
			if (j > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j - 1];
				e->mWeight = ComputeWeight(node, g.mNodes[0]);
				node->mEdges.emplace_back(e);
			}
			if (j < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j + 1];
				e->mWeight = ComputeWeight(node, g.mNodes[0]);
				node->mEdges.emplace_back(e);
			}
		}
	}
	bool found = false;

	GBFSMap map;
	found = GBFS(g, g.mNodes[0], g.mNodes[9], map);

	printGraph(g);

	if (found)
	{
		std::cout << "\nfound\n";
		printRoute(g, map);
	}
	else
	{
		std::cout << "\nnot found\n";
	}
}

int main(int argc, char** argv)
{

	testHeuristic();
	return 0;
}
