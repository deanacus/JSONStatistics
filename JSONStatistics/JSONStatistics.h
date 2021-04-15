#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/wrappers/GameObject/Stats/StatEventWrapper.h"

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
	string BuildMatchData();
	string BuildMetadata();
	string BuildRating();
	string BuildScoreStats();
	string BuildOffenseStats();
	string BuildDefenseStats();
	string BuildTouchStats();
	string BuildFileContents();
	//void HandleStatEvent();
	void HandleMatchEnded(string name);
	PriWrapper GetPrimaryPlayer();
	bool IsInRealGame();
	string MarshallToJSON(map<string, string> stats);
	void saveFile(string fileContents);
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

