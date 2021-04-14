#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"

#include <map>
#include <fstream>
#include <array>

using namespace std;

class JSONStatistics: public BakkesMod::Plugin::BakkesModPlugin
{
	filesystem::path dataDir;

	virtual void onLoad();
	virtual void onUnload();

	void Bootstrap();
	bool IsInRealGame();
	void HandleMatchEnded(string name);
	void saveFile();
	string MarshallToJSON(map<string, string> stats);
	string BuildScoreStats();
	string BuildOffenseStats();
	string BuildDefenseStats();
	string BuildTouchStats();
	string BuildMetadata();
	PriWrapper GetPrimaryPlayer();
	void Log(string msg);

	enum Events {
		demolition,
		aerialGoal,
		backwardsGoal,
		bicycleGoal,
		longGoal,
		turtleGoal,
		poolShot,
		overtimeGoal,
		hatTrick,
		epicSave,
		center,
		clear,
		firstTouch,
		damage,
		ultraDamage,
		swish,
		death,
	};
};

