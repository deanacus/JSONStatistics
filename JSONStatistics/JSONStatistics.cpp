#include "JSONStatistics.h"
#include "Utils.h"

#include <array>

using namespace std;

BAKKESMOD_PLUGIN(JSONStatistics, "Save your stats to JSON", "0.1.0", 0);

array<int, 17> inGameStats;

void JSONStatistics::onLoad()
{
	this->Log("JSONStatistics Loaded");
	this->Bootstrap();
}

void JSONStatistics::onUnload() {
}

void JSONStatistics::Bootstrap() {
	dataDir = gameWrapper->GetDataFolder().append("JSONStatistics");
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", bind(&JSONStatistics::HandleMatchEnded, this, std::placeholders::_1));

	namespace fs = filesystem;
	fs::create_directories(dataDir);
}

string JSONStatistics::BuildScoreStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	map<string, string> scoreStats;
	scoreStats["mvp"] = localPrimaryPlayer.GetbMatchMVP() == true ? "true" : "false";
	scoreStats["points"] = to_string(localPrimaryPlayer.GetMatchScore());
	// scoreStats["damage"] = to_string(inGameStats[damage]);
	// scoreStats["ultraDamage"] = to_string(inGameStats[ultraDamage]);

	return this->MarshallToJSON(scoreStats);
}

string JSONStatistics::BuildOffenseStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	map<string, string> offenseStats;
	offenseStats["goals"] = to_string(localPrimaryPlayer.GetMatchGoals());
	offenseStats["shots"] = to_string(localPrimaryPlayer.GetMatchShots());
	offenseStats["assists"] = to_string(localPrimaryPlayer.GetMatchAssists());

	// offenseStats["hatTricks"] = to_string(inGameStats[hatTrick]);
	// offenseStats["aerialGoals"] = to_string(inGameStats[aerialGoal]);
	// offenseStats["backwardsGoals"] = to_string(inGameStats[backwardsGoal]);
	// offenseStats["bicycleGoals"] = to_string(inGameStats[bicycleGoal]);
	// offenseStats["longGoals"] = to_string(inGameStats[longGoal]);
	// offenseStats["overtimeGoals"] = to_string(inGameStats[overtimeGoal]);
	// offenseStats["turtleGoals"] = to_string(inGameStats[turtleGoal]);
	// offenseStats["poolShots"] = to_string(inGameStats[poolShot]);
	// offenseStats["swishes"] = to_string(inGameStats[swish]);

	return this->MarshallToJSON(offenseStats);
}

string JSONStatistics::BuildDefenseStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	map<string, string> defenseStats;
	defenseStats["saves"] = to_string(localPrimaryPlayer.GetMatchSaves());
	// defenseStats["epicSaves"] = to_string(inGameStats[epicSave]);
	// defenseStats["clears"] = to_string(inGameStats[clear]);
	defenseStats["ownGoals"] = to_string(localPrimaryPlayer.GetMatchOwnGoals());

	return this->MarshallToJSON(defenseStats);
}

string JSONStatistics::BuildTouchStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	map<string, string> touchStats;
	touchStats["boostPickups"] = to_string(localPrimaryPlayer.GetBoostPickups());
	touchStats["bumps"] = to_string(localPrimaryPlayer.GetCarTouches());
	touchStats["touches"] = to_string(localPrimaryPlayer.GetBallTouches());
	touchStats["demolitions"] = to_string(localPrimaryPlayer.GetMatchDemolishes());
	touchStats["deaths"] = to_string(inGameStats[death]);
	// touchStats["centers"] = to_string(inGameStats[center]);
	// touchStats["firstTouches"] = to_string(inGameStats[firstTouch]);

	return this->MarshallToJSON(touchStats);
}

string JSONStatistics::BuildMetadata() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();

	ServerWrapper server = gameWrapper->GetOnlineGame();

	string timestamp = GetTimeStamp();

	map<string, string> metaData;
	metaData["id"] = server.GetMatchGUID();
	metaData["date"] = timestamp;
	metaData["playlist"] = server.GetPlaylist().GetTitle().ToString();
	metaData["win"] = to_string((localPrimaryPlayer.GetTeamNum() == server.GetMatchWinner().GetTeamNum()));
	metaData["team"] = to_string(localPrimaryPlayer.GetTeamNum());
	metaData["arena"] = gameWrapper->GetCurrentMap();
	metaData["mmrAdjustment"] = "0";
}

//void JSONStatistics::HandleStatEvent() {
//
//}

void JSONStatistics::HandleMatchEnded(string name) {
	if (!this->IsInRealGame()) {
		return;
	}

	this->saveFile();
}

PriWrapper JSONStatistics::GetPrimaryPlayer() {
	ServerWrapper server = gameWrapper->GetOnlineGame();
	PlayerControllerWrapper localPrimaryPlayerController = server.GetLocalPrimaryPlayer();
	return localPrimaryPlayerController.GetPRI();
}

bool JSONStatistics::IsInRealGame() {
	if(!gameWrapper->IsInOnlineGame()) {
		return false;
	}
	ServerWrapper server = gameWrapper->GetOnlineGame();
	GameSettingPlaylistWrapper playlist = server.GetPlaylist();
	PlayerControllerWrapper localPrimaryPlayerController = server.GetLocalPrimaryPlayer();
	TeamWrapper matchWinner = server.GetMatchWinner();
	return !gameWrapper->IsInReplay() || !gameWrapper->IsInFreeplay() || !playlist.IsLanMatch() || !playlist.IsPrivateMatch() || !localPrimaryPlayerController.IsNull() || !matchWinner.IsNull();
}

string JSONStatistics::MarshallToJSON(map<string, string> stats) {
	string newLine = "\n";
	string lineEnding = ",";
	string separator = ": ";

	string json = "";
	for (auto const& [key, val] : stats) {
		string value = isNumber(val) ? val : WrapString(val, "\"");
		this->Log(key + ": " + value);
		string statLine = newLine + WrapString(key, "\"") + separator + WrapString(val, "\"") + lineEnding;
		json.append(statLine);
	}

	json = TrimRight(json, ",");
	json = PadLeft(json, "{", 1);
	json = PadRight(json, "}", 1);
	json.append("\n");

	this->Log(json);

	return json;
}

void JSONStatistics::saveFile() {
	string fileContents = "";

	fileContents += this->BuildMetadata();
	fileContents += this->BuildScoreStats();
	fileContents += this->BuildOffenseStats();
	fileContents += this->BuildDefenseStats();
	fileContents += this->BuildTouchStats();

	string fileDate = GetTimeStamp();

	ofstream totalFile(dataDir / (fileDate + string(".json")));
	this->Log("wrote");
	totalFile << fileContents;
	totalFile.close();
}

void JSONStatistics::Log(string msg) {
	cvarManager->log(msg);
}