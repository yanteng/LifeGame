#include<iostream>
#include <filesystem>
#include <fstream>

#include "lifes.h"
#include "game.h"

using namespace std;
namespace fs = std::experimental::filesystem;

const string templatePath = "./template/";
const string configPath = "./config.ini";

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
	system("cls");
	string buffer;
	for (auto const& row : this->lifes->getLifes())
	{
		for (auto const& life : row)
		{
			buffer += life ? 'O' : '.';
		}
		buffer += '\n';
	}
	cout << buffer;
}

void Game::update()
{
	this->lifes->nextLifes();
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
