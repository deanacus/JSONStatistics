#include "pch.h"
#include "JSONStatistics.h"

BAKKESMOD_PLUGIN(JSONStatistics, "Save your stats to JSON", "0.1.0", 0)

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

	namespace fs = std::filesystem;
	fs::create_directories(dataDir);
}

void JSONStatistics::HandleMatchEnded(std::string name) {
	if (!this->IsInRealGame()) {
		return;
	}

	ServerWrapper server = gameWrapper->GetOnlineGame();
	GameSettingPlaylistWrapper playlist = server.GetPlaylist();
	PlayerControllerWrapper localPrimaryPlayerController = server.GetLocalPrimaryPlayer();
	TeamWrapper matchWinner = server.GetMatchWinner();

	if (playlist.IsLanMatch() || playlist.IsPrivateMatch() || localPrimaryPlayerController.IsNull() || matchWinner.IsNull()) {
		return;
	}

	PriWrapper localPrimaryPlayer = localPrimaryPlayerController.GetPRI();

	std::time_t rawtime;
	time(&rawtime);
	char timestamp[20];
	strftime(timestamp, 20, "%FT%TZ", gmtime(&rawtime));

	std::map<std::string, std::string> gameStats;

	gameStats["date"] = timestamp;
	gameStats["playlist"] = playlist.GetTitle().ToString();
	gameStats["win"] = (localPrimaryPlayer.GetTeamNum() == matchWinner.GetTeamNum());
	gameStats["mvp"] = localPrimaryPlayer.GetbMatchMVP();
	gameStats["team"] = localPrimaryPlayer.GetTeamNum();
	gameStats["stadium"] = "tmp";
	gameStats["weather"] = "tmp";
	gameStats["mmrAdjustment"] = "0";
	gameStats["points"] = localPrimaryPlayer.GetMatchScore();
	gameStats["goals"] = localPrimaryPlayer.GetMatchGoals();
	gameStats["shots"] = localPrimaryPlayer.GetMatchShots();
	gameStats["assists"] = localPrimaryPlayer.GetMatchAssists();
	gameStats["centers"] = "0";
	gameStats["deaths"] = "0";
	gameStats["demolitions"] = localPrimaryPlayer.GetMatchDemolishes();
	gameStats["firstTouchs"] = "0";
	gameStats["epicSaves"] = "0";
	gameStats["saves"] = localPrimaryPlayer.GetMatchSaves();
	gameStats["clears"] = "0";
	gameStats["aerialGoals"] = "0";
	gameStats["backwardsGoals"] = "0";
	gameStats["bicycleGoals"] = "0";
	gameStats["longGoals"] = "0";
	gameStats["overtimeGoals"] = "0";
	gameStats["turtleGoals"] = "0";
	gameStats["poolShots"] = "0";
	gameStats["swishs"] = "0";
	gameStats["damages"] = "0";
	gameStats["ultraDamages"] = "0";
	
	this->Log(this->MarshallToJSON(gameStats));
}

bool JSONStatistics::IsInRealGame() {
	return gameWrapper->IsInOnlineGame() && !gameWrapper->IsInReplay() && !gameWrapper->IsInFreeplay();
}

std::string JSONStatistics::MarshallToJSON(std::map<std::string, std::string> stats) {
	std::string lineEnding = ",\n";
	std::string separator = ": ";

	std::string json = "{\n";
	for (auto const& [key, val] : stats) {
		std::string statLine = this->WrapString(key) + separator + this->WrapString(val) + lineEnding;
		json.append(statLine);
	}

	json.pop_back();
	json.append("\n}");

	return json;
}

bool JSONStatistics::isNumber(const std::string& str) {
	return !str.empty() && str.find_first_not_of("-0123456789") == std::string::npos;
}

std::string JSONStatistics::WrapString(std::string input) {
	if (this->isNumber(input)) {
		return input;
	}
	std::string quote = "\"";
	return quote + input + quote;
}

void JSONStatistics::saveFile(std::map<std::string, std::string> stats) {
	std::ofstream totalFile(dataDir / (stats["date"] + ".json"));
	this->Log("wrote");
	totalFile << this->MarshallToJSON(stats);
	totalFile.close();
}

void JSONStatistics::Log(std::string msg) {
	cvarManager->log(msg);
}