#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"

#include <map>
#include <fstream>


class JSONStatistics: public BakkesMod::Plugin::BakkesModPlugin
{
	std::filesystem::path dataDir;

	virtual void onLoad();
	virtual void onUnload();

	void Bootstrap();
	bool IsInRealGame();
	void HandleMatchEnded(std::string name);
	void saveFile(std::map<std::string, std::string> stats);
	std::string MarshallToJSON(std::map<std::string, std::string> stats);

	// Utilities
	std::string WrapString(std::string input);
	bool isNumber(const std::string& str);
	void Log(std::string msg);
};

