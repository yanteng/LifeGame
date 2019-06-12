#include "lifes.h"
#include<iostream>

using namespace std;

static vector<vector<int>> offsets
{
	vector<int>{-1, -1},
	vector<int>{-1, 0},
	vector<int>{-1, 1},
	vector<int>{0, -1},
	vector<int>{0, 1},
	vector<int>{1, -1},
	vector<int>{1, 0},
	vector<int>{1, 1},
};

static int getMaxRowWidth(const std::vector<std::vector<bool>>& lifes)
{
	int max = 0;
	for (auto const& row : lifes)
	{
		if (row.size() > max)
			max = row.size();
	}
	return max;
}

Lifes::Lifes(int width, int height) :
	lifes(height, vector<bool>(width, false)),
	lifeLowerLimitNum(2),
	lifeUpperLimitNum(3)
{
}

Lifes::~Lifes()
{
}

void Lifes::clearLifes()
{
	int height = this->lifes.size();
	int width = this->lifes[0].size();
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			this->lifes[row][col] = false;
			//this->nextLifes[row][col] = false;
		}
	}
}

void Lifes::setLife(int row, int col, bool life)
{
	this->lifes[row][col] = life;
}

bool Lifes::setLifes(std::vector<std::vector<bool>>& lifes)
{
	if (lifes.empty())
	{
		return false;
	}
	int inputHeight = lifes.size(), inputWidth = getMaxRowWidth(lifes);
	int height = this->lifes.size(), width = this->lifes[0].size();

	if (inputHeight > height || inputWidth > width)
	{
		return false;
	}
	// æ”÷–
	int startRow = (height - inputHeight) / 2;
	int startCol = (width - inputWidth) / 2;
	for (size_t row = 0; row < inputHeight; ++row)
	{
		if (lifes[row].size() < inputWidth)
		{
			lifes[row].resize(inputWidth, false);
		}
		for (size_t col = 0; col < inputWidth; ++col)
		{
			this->lifes[row + startRow][col + startCol] = lifes[row][col];
		}
	}
	return true;
}

const vector<vector<bool>>& Lifes::getLifes()
{
	return this->lifes;
}

void Lifes::nextLifes()
{
	int height = this->lifes.size();
	int width = this->lifes[0].size();
	vector<vector<bool>> nextLifes(height, vector<bool>(width));
	
	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			nextLifes[row][col] = nextState(row, col);
		}
	}
	this->lifes = nextLifes;
}

void Lifes::setUpperLifeLimit(int upper)
{
	this->lifeUpperLimitNum = upper;
}

void Lifes::setLowerLifeLimit(int lower)
{
	this->lifeLowerLimitNum = lower;
}

vector<bool> Lifes::getNeighbors(const int row, const int col)
{
	vector<bool> neighbors;
	for (auto const& offset : offsets)
	{
		int offsetRow = row + offset[0];
		int offsetCol = col + offset[1];
		if (offsetRow < 0 || offsetCol < 0 || offsetRow >= lifes.size() || offsetCol >= lifes[0].size())
		{
			neighbors.push_back(false);
		}
		else
		{
			neighbors.push_back(this->lifes[offsetRow][offsetCol]);
		}
	}
	return neighbors;
}

bool Lifes::nextState(const int row, const int col)
{
	bool originState = this->lifes[row][col];
	vector<bool> neighbors = this->getNeighbors(row, col);
	int liveNum = 0;
	for (const auto& life : neighbors)
	{
		life ? liveNum++ : liveNum;
	}
	if (originState && (liveNum > this->lifeUpperLimitNum || liveNum < this->lifeLowerLimitNum))
	{
		return false;
	}
	if (originState && (liveNum >= liveNum < this->lifeLowerLimitNum && liveNum <= this->lifeUpperLimitNum))
	{
		return originState;
	}
	if (!originState && liveNum == this->lifeUpperLimitNum)
	{
		return true;
	}
	return originState;
}
