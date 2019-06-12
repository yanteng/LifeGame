#pragma once
#include<map>

class Lifes;
class Game
{
public:
	Game();
	~Game();
	bool init();
	void home();
	void print();
	void update();
	void clearScreen();
private:
	bool saveConfigs(const std::string& path);
	bool readConfigFile(const std::string& path);
	Lifes* lifes;
	std::map<std::string, std::string> configs;
};
