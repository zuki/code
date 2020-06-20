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

struct AStarScratch
{
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	float mActualFromStart = 0.0f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};
using AStarMap =
std::unordered_map<const WeightedGraphNode*, AStarScratch>;

float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	float x = (b->name % 10) - (a->name % 10);
	float y = (b->name / 10) - (a->name / 10);
	return x + y;
}

bool AStar(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AStarMap& outMap)
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
			const WeightedGraphNode* neighbor = edge->mTo;
			// Get scratch data for this node
			AStarScratch& data = outMap[neighbor];
			// Only check nodes that aren't in the closed set
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					// Not in the open set, so parent must be current
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// Actual cost is the parent's plus cost of traversing edge
					data.mActualFromStart = outMap[current].mActualFromStart +
						edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					// Compute what new actual cost is if current becomes parent
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						// Current should adopt this node
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
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
			// Calculate f(x) for nodes a/b
			float fOfA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float fOfB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return fOfA < fOfB;
		});
		// Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = true;
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
		s += "(" + std::to_string(v[0]->mFrom->name) + ", " + std::to_string(v[0]->mTo->name) + ")";
		for (decltype(v.size()) i = 1, c = v.size(); i < c; i++)
		{
			if (delim) s += delim;
			s += "(" + std::to_string(v[i]->mFrom->name) + ", " + std::to_string(v[i]->mTo->name) + ")";
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

void printRoute(const WeightedGraph& g, AStarMap& map)
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

void testAStar()
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
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (i < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[(i + 1) * 5 + j];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (j > 0)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j - 1];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
			if (j < 4)
			{
				WeightedEdge* e = new WeightedEdge;
				e->mFrom = node;
				e->mTo = g.mNodes[i * 5 + j + 1];
				e->mWeight = 1.0f;
				node->mEdges.emplace_back(e);
			}
		}
	}
	bool found = false;

	AStarMap map;
	found = AStar(g, g.mNodes[0], g.mNodes[9], map);

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

	testAStar();
	return 0;
}
