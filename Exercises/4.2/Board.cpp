// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Board.h"
#include "Random.h"
#include "SDL/SDL.h"

BoardState::BoardState()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mBoard[i][j] = Empty;
		}
	}
}

std::vector<BoardState*> BoardState::GetPossibleMoves(SquareState player) const
{
	std::vector<BoardState*> retVal;

	// 各カラムについて、着手可能か調べる
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));	// 現在のボードをコピーして新たなボードを作成
				retVal.back()->mBoard[row][col] = player;   // 新しいボードのrow/colをplayerとする
				break;
			}
		}
	}

	return retVal;
}

bool BoardState::IsTerminal() const
{
	// Is the board full?
	if (IsFull())
	{
		return true;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return true;
	}

	return false;
}

float BoardState::GetScore() const
{
	// If the board is full, the score is 0
	if (IsFull())
	{
		return 0.0f;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return static_cast<float>(fourInRow);
	}

	return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
	bool isFull = true;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (mBoard[i][j] == Empty)
			{
				isFull = false;
			}
		}
	}

	return isFull;
}

int BoardState::GetFourInARow() const
{
	// Returns -1 if yellow wins, 1 if red wins, 0 otherwise

	// Check if there's a row with four in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (mBoard[row][col] == mBoard[row][col + 1] &&
				mBoard[row][col] == mBoard[row][col + 2] &&
				mBoard[row][col] == mBoard[row][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a column with four in a row
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col] &&
				mBoard[row][col] == mBoard[row + 2][col] &&
				mBoard[row][col] == mBoard[row + 3][col])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a right diagonal four in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a left diagonal for in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

float BoardState::CalculateHeuristic() const
{
	// TODO: You could change this to calculate an actual heuristic
	return 0.0f;
}

bool TryPlayerMove(BoardState* state, int column)
{
	// Find the first row in that column that's available
	// (if any)
	for (int row = 5; row >= 0; row--)
	{
		if (state->mBoard[row][column] == BoardState::Empty)
		{
			state->mBoard[row][column] = BoardState::Yellow;
			return true;
		}
	}

	return false;
}

void CPUMove(BoardState* state)
{
	int maxDepth = 4;
	// 着手可能なボード（着手済み）をベクタとして得る
	std::vector<BoardState*> moves = state->GetPossibleMoves(BoardState::Red);
	// 最適なものを選択
	int index = AlphaBetaDecide(moves, maxDepth);
	// 選択したボードを返す
	*state = *moves[index];

	// 着手可能なボード用のメモリを開放
	for (auto state : moves)
	{
		delete state;
	}
}

int AlphaBetaDecide(const std::vector<BoardState*>& states, int maxDepth)
{
	// Find the subtree with the maximum value, and save the choice
	int choice;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (int i = 0; i < states.size(); i++)
	{
		float v = AlphaBetaMin(states[i], maxDepth - 1, maxValue, beta);
		if (v > maxValue)
		{
			maxValue = v;
			choice = i;
		}
	}
	return choice;
}

float AlphaBetaMax(const BoardState* state, int depth, float alpha, float beta)
{
	// If this is a leaf, return score
	if (depth == 0 || state->IsTerminal())
	{
		return state->GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	std::vector<BoardState*> moves = state->GetPossibleMoves(BoardState::Red);
	// Find the subtree with the maximum value
	for (auto move : moves)
	{
		maxValue = std::max(maxValue, AlphaBetaMin(move, depth -1, alpha, beta));
		//SDL_Log("Max: max=%2.0f, alpha=%2.0f, beta=%2.0f", maxValue, alpha, beta);
		if (maxValue >= beta)
		{
			return maxValue; // Beta prune
		}
		alpha = std::max(maxValue, alpha);
	}
	for (auto move : moves)
	{
		delete move;
	}
	return maxValue;
}

float AlphaBetaMin(const BoardState* state, int depth, float alpha, float beta)
{
	// If this is a leaf, return score
	if (depth == 0 || state->IsTerminal())
	{
		return state->GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();
	std::vector<BoardState*> moves = state->GetPossibleMoves(BoardState::Yellow);
	// Find the subtree with the minimum value
	for (auto move : moves)
	{
		minValue = std::min(minValue, AlphaBetaMax(move, depth - 1, alpha, beta));
		//SDL_Log("Min: min=%2.0f, alpha=%2.0f, beta=%2.0f", minValue, alpha, beta);
		if (minValue <= alpha)
		{
			return minValue; // Alpha prune
		}
		beta = std::min(minValue, beta);
	}
	for (auto move : moves)
	{
		delete move;
	}
	return minValue;
}
