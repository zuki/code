#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct GameState
{
	// (For tic-tac-toe, array of board)
	enum SquareState { Empty, X, O };
	SquareState mBoard[3][3];
};

struct GTNode
{
	// Children nodes
	std::vector<GTNode*> mChildren;
	// State of game
	GameState mState;
};

void GenStates(GTNode* root, bool xPlayer)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (root->mState.mBoard[i][j] == GameState::Empty)
			{
				GTNode* node = new GTNode;
				root->mChildren.emplace_back(node);
				node->mState = root->mState;
				node->mState.mBoard[i][j] = xPlayer ? GameState::X : GameState::O;
				GenStates(node, !xPlayer);
			}
		}
	}
}

float GetScore(const GameState& state)
{
	// 横が揃った?
	for (int i = 0; i < 3; i++)
	{
		bool same = true;
		GameState::SquareState v = state.mBoard[i][0];
		for (int j = 1; j < 3; j++)
		{
			if (state.mBoard[i][j] != v)
			{
				same = false;
			}
		}

		if (same)
		{
			if (v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}

	// 縦が揃った?
	for (int j = 0; j < 3; j++)
	{
		bool same = true;
		GameState::SquareState v = state.mBoard[0][j];
		for (int i = 1; i < 3; i++)
		{
			if (state.mBoard[i][j] != v)
			{
				same = false;
			}
		}

		if (same)
		{
			if (v == GameState::X)
			{
				return 1.0f;
			}
			else
			{
				return -1.0f;
			}
		}
	}

	// 対角線が揃った?
	if (((state.mBoard[0][0] == state.mBoard[1][1]) &&
		(state.mBoard[1][1] == state.mBoard[2][2])) ||
		((state.mBoard[2][0] == state.mBoard[1][1]) &&
		(state.mBoard[1][1] == state.mBoard[0][2])))
	{
		if (state.mBoard[1][1] == GameState::X)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}
	}
	// 引き分け
	return 0.0f;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta);

float AlphaBetaMax(const GTNode* node, float alpha, float beta)
{
	// If this is a leaf, return score
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	// Find the subtree with the maximum value
	for (const GTNode* child : node->mChildren)
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, alpha, beta));
		if (maxValue >= beta)
		{
			return maxValue; // Beta prune
		}
		alpha = std::max(maxValue, alpha);
	}
	return maxValue;
}

float AlphaBetaMin(const GTNode* node, float alpha, float beta)
{
	// If this is a leaf, return score
	if (node->mChildren.empty())
	{
		return GetScore(node->mState);
	}

	float minValue = std::numeric_limits<float>::infinity();
	// Find the subtree with the minimum value
	for (const GTNode* child : node->mChildren)
	{
		minValue = std::min(minValue, AlphaBetaMax(child, alpha, beta));
		if (minValue <= alpha)
		{
			return minValue; // Alpha prune
		}
		beta = std::min(minValue, beta);
	}
	return minValue;
}

const GTNode* AlphaBetaDecide(const GTNode* root)
{
	// Find the subtree with the maximum value, and save the choice
	const GTNode* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (const GTNode* child : root->mChildren)
	{
		float v = AlphaBetaMin(child, maxValue, beta);
		if (v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
	return choice;
}

void printBoard(const GameState& g)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			GameState::SquareState v = g.mBoard[i][j];
			if (v == GameState::X)      std::cout << "X ";
			else if (v == GameState::O) std::cout << "O ";
			else                        std::cout << "  ";
		}
		std::cout << "\n";
	}
}

void testTicTac()
{
	GTNode* root = new GTNode;
	root->mState.mBoard[0][0] = GameState::O;
	root->mState.mBoard[0][1] = GameState::Empty;
	root->mState.mBoard[0][2] = GameState::X;
	root->mState.mBoard[1][0] = GameState::X;
	root->mState.mBoard[1][1] = GameState::O;
	root->mState.mBoard[1][2] = GameState::O;
	root->mState.mBoard[2][0] = GameState::X;
	root->mState.mBoard[2][1] = GameState::Empty;
	root->mState.mBoard[2][2] = GameState::Empty;

	GenStates(root, true);
	const GTNode* choice = AlphaBetaDecide(root);
	std::cout << "7手目: 先行\n";
	printBoard(choice->mState);
	std::cout << "\n";
	int n = 1;
	for (auto c1Node : choice->mChildren)
	{
		std::cout << "8手目: 後攻 (" << n << ")\n";
		printBoard(c1Node->mState);
		std::cout << "\n";
		for (auto c2Node : c1Node->mChildren)
		{
			std::cout << "9手目: 先行\n";
			printBoard(c2Node->mState);
			std::cout << "\n";

			if (c2Node->mChildren.empty())
			{
				float score = GetScore(c2Node->mState);
				std::cout << "勝負: " << ((score > 0) ? "先行勝ち" : (score < 0) ? "先行負け" : "引き分け") << '\n';
				std::cout << "----------------\n";
			}
		}
		n++;
	}
}

int main(int argc, char** argv)
{
	testTicTac();
	return 0;
}
