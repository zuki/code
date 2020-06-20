#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <string>

struct GraphNode
{
	// Adjacency list
	std::vector<GraphNode*> mAdjacent;
	int name;
};

struct Graph
{
	// A graph contains nodes
	std::vector<GraphNode*> mNodes;
};


using NodeToParentMap =
std::unordered_map<const GraphNode*, const GraphNode*>;

bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap)
{
	// Whether we found a path
	bool pathFound = false;
	// Nodes to consider
	std::queue<const GraphNode*> q;
	// Enqueue the first node
	q.emplace(start);

	while (!q.empty())
	{
		// Dequeue a node
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// Enqueue adjacent nodes that aren't already in the queue
		for (const GraphNode* node : current->mAdjacent)
		{
			// If the parent is null, it hasn't been enqueued
			// (except for the start node)
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start)
			{
				// Enqueue this node, setting its parent
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}

std::string join(const std::vector<GraphNode*>& v, const char * delim = 0)
{
	std::string s;
	if (!v.empty())
	{
		s += std::to_string(v[0]->name);
		for (decltype(v.size()) i = 1, c = v.size(); i < c; i++)
		{
			if (delim) s += delim;
			s += std::to_string(v[i]->name);
		}
	}
	return s;
}

void testBFS()
{
	Graph g;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			GraphNode* node = new GraphNode;
			node->name = i * 10 + j;
			g.mNodes.emplace_back(node);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			GraphNode* node = g.mNodes[i * 5 + j];
			if (i > 0)
			{
				node->mAdjacent.emplace_back(g.mNodes[(i - 1) * 5 + j]);
			}
			if (i < 4)
			{
				node->mAdjacent.emplace_back(g.mNodes[(i + 1) * 5 + j]);
			}
			if (j > 0)
			{
				node->mAdjacent.emplace_back(g.mNodes[i * 5 + j - 1]);
			}
			if (j < 4)
			{
				node->mAdjacent.emplace_back(g.mNodes[i * 5 + j + 1]);
			}
		}
	}

	NodeToParentMap map;
	bool found = BFS(g, g.mNodes[0], g.mNodes[9], map);

	std::cout << "Graph\n\n";
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			GraphNode* node = g.mNodes[i * 5 + j];
			std::cout << node->name << ": [" << join(node->mAdjacent, ", ") << "]\n";
		}
	}

	std::cout << '\n' << (found ? "found" : "not found") << "\n";

	if (found)
	{
		const GraphNode* child = g.mNodes[9];
		std::cout << "GOAL : " << child->name;
		while (true)
		{
			const GraphNode* parent = map[child];
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
}

int main(int argc, char** argv)
{

	testBFS();
	return 0;
}
