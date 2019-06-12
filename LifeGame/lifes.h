#pragma once
#include<vector>

class Lifes
{
public:
	Lifes(int width, int height);
	~Lifes();
	void clearLifes();
	void setLife(int row, int col, bool live);
	bool setLifes(std::vector<std::vector<bool>>& lifes);
	const std::vector<std::vector<bool>>& getLifes();
	void nextLifes();
	void setUpperLifeLimit(int upper);
	void setLowerLifeLimit(int lower);
private:
	std::vector<bool> getNeighbors(const int row, const int col);
	bool nextState(const int row, const int col);
	std::vector<std::vector<bool>> lifes;
	int lifeUpperLimitNum;
	int lifeLowerLimitNum;
};