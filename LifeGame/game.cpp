#include<iostream>
#include <filesystem>
#include <fstream>
#include <Windows.h>

#include "lifes.h"
#include "game.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

using namespace std;
namespace fs = std::experimental::filesystem;

const string templatePath = "./template/";
const string configPath = "./config.ini";

static void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

static vector<string> getTplFileName()
{
	vector<string> fileNames;
	for (const auto & entry : fs::directory_iterator(templatePath))
	{
		if (fs::path(entry.path()).extension() == ".tpl")
		{
			fileNames.push_back(fs::path(entry.path()).filename().string());
		}
	}
	return fileNames;
}

static bool readLifesFromTplFile(const string& path, vector<vector<bool>>& lifes)
{
	ifstream istrm(path);
	if (!istrm.is_open())
	{
		return false;
	}
	string line;
	int row=0, col=0;
	while (getline(istrm, line))
	{
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		if (!line.empty())
		{
			lifes.push_back(vector<bool>(line.size()));
			for (auto const& c : line)
			{
				c == 'O' ? lifes[lifes.size() - 1].push_back(true) : lifes[lifes.size() - 1].push_back(false);
				
			}
		}
	}
	return true;
}

Game::Game()
{
	configs.insert(pair<string, string>("width", "100"));
	configs.insert(pair<string, string>("height", "100"));
}
 
Game::~Game()
{
	delete lifes;
}

bool Game::init()
{
	if (!fs::exists(configPath))
	{
		saveConfigs(configPath);
	}
	else if (!readConfigFile(configPath))
	{
		return false;
	}
	this->lifes = new Lifes(stof(configs["width"]), stof(configs["height"]));
	return true;
}

void Game::home()
{
	system("cls");
	int selectedHomeItem;
	vector<string> tplNames = getTplFileName();
	cout << "select a template:\n";
	for (int i = 0; i < tplNames.size(); ++i)
	{
		cout << i + 1 << ". " << tplNames[i] << endl;
	}
	cout << "input 0 use a empty template.\n";
	cin >> selectedHomeItem;
	system("cls");
	if (selectedHomeItem)
	{
		const string path = templatePath + tplNames[selectedHomeItem - 1];
		vector<vector<bool>> lifes;
		if (!readLifesFromTplFile(path, lifes))
		{
			cerr << path << " open failed\n";
		}
		this->lifes->setLifes(lifes);
	}
	cout << "asd";
}

void Game::print()
{
	vector<vector<bool>> lifes = this->lifes->getLifes();
	static vector<vector<bool>> prevLifes;
	string buffer;
	int height = this->lifes->getLifes().size();
	int width = this->lifes->getLifes()[0].size();

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			if (!prevLifes.empty() && prevLifes[row][col] == lifes[row][col]) continue;
			setCursorPosition(col, row);
			lifes[row][col] ? cout << 'O' : cout << '.';
		}
	}
	prevLifes = lifes;
}

void Game::update()
{
	this->lifes->nextLifes();
}

void Game::clearScreen()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

bool Game::saveConfigs(const std::string & path)
{
	ofstream ostrm(path);
	if (ostrm.is_open())
	{
		for (auto& it = configs.begin(); it != configs.end(); ++it)
			ostrm << it->first << "=" << it->second << '\n';
		return true;
	}
	return false;
	
}

bool Game::readConfigFile(const string& path)
{
	ifstream istrm(path);
	if (istrm.is_open())
	{
		string line;
		while (getline(istrm, line)) 
		{
			line.erase(remove_if(line.begin(), line.end(), isspace),line.end());
			if (line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			if (configs.find(name) != configs.end())
			{
				configs.erase(name);
			}
			configs.insert(pair<string, string>(name, value));
		}
		return true;
	}
	cerr << "Couldn't open config file for reading.\n";
	return false;
}
