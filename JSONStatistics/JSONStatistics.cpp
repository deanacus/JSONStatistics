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

string JSONStatistics::BuildMatchData() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	ServerWrapper server = gameWrapper->GetOnlineGame();
	string timestamp = GetTimeStamp();

	string output;
	output += "\"id\": \"" + server.GetMatchGUID() + "\",\n";
	output += "\"date\": \"" + timestamp + "\",\n";
	output += "\"playlist\":\"" + server.GetPlaylist().GetTitle().ToString() + "\",\n";
	output += "\"win\"" + string(localPrimaryPlayer.GetTeamNum() == server.GetMatchWinner().GetTeamNum() ? "true" : "false") + "\n";
	return output;
}

string JSONStatistics::BuildMetadata() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	GameSettingPlaylistWrapper playlist = gameWrapper->GetOnlineGame().GetPlaylist();

	string output;

	output += "\"metadata\": {\n";
	output += "  \"team\": \"" + string(localPrimaryPlayer.GetTeamNum() == 0 ? "Blue" : "Orange") + "\",\n";
	output += "  \"arena\": \"" + gameWrapper->GetCurrentMap() + "\",\n";
	output += "  \"ranked\": " + string(playlist.GetbRanked() == 0 ? "false" : "true") + "\n}";

	return output;
}

string JSONStatistics::BuildRating() {
	string delta = "0";
	string division = "0";
	string prevDivision = "0";
	string prevRank = "0";
	string rank = "0";
	string rating = "0";

	string output;

	output += "\"rating\": {\n";
	output += "  \"delta\": " + delta + ",\n";
	output += "  \"division\": " + division + ",\n";
	output += "  \"prevDivision\": " + prevDivision + ",\n";
	output += "  \"prevRank\": " + prevRank + ",\n";
	output += "  \"rank\": " + rank + ",\n";
	output += "  \"rating\": " + rating + "\n}";

	return output;
}

string JSONStatistics::BuildScoreStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();

	string damage = "0"; // Temporary placeholder -> to_string(inGameStats[damage]);
	string ultraDamage = "0"; // Temporary placeholder -> to_string(inGameStats[utraDamage]);

	string output;
	output += "  \"score\": {\n";
	output += "    \"mvp\": " + string(localPrimaryPlayer.GetbMatchMVP() == 0 ? "false" : "true") + ",\n";
	output += "    \"points\": " + to_string(localPrimaryPlayer.GetMatchScore()) + ",\n";
	output += "    \"damage\": " + damage + ",\n";
	output += "    \"ultraDamage\": " + ultraDamage + "\n  }";

	return output;
}

string JSONStatistics::BuildOffenseStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();

	string hatTricks = "0"; // Temporary placeholder -> to_string(inGameStats[hatTrick]);
	string aerialGoals = "0"; // Temporary placeholder -> to_string(inGameStats[aerialGoal]);
	string backwardsGoals = "0"; // Temporary placeholder -> to_string(inGameStats[backwardsGoal]);
	string bicycleGoals = "0"; // Temporary placeholder -> to_string(inGameStats[bicycleGoal]);
	string longGoals = "0"; // Temporary placeholder -> to_string(inGameStats[longGoal]);
	string overtimeGoals = "0"; // Temporary placeholder -> to_string(inGameStats[overtimeGoal]);
	string turtleGoals = "0"; // Temporary placeholder -> to_string(inGameStats[turtleGoal]);
	string poolShots = "0"; // Temporary placeholder -> to_string(inGameStats[poolShot]);
	string swishes = "0"; // Temporary placeholder -> to_string(inGameStats[swish]);

	string output;
	output += "  \"offense\": {\n";
	output += "    \"goals\": " + to_string(localPrimaryPlayer.GetMatchGoals()) + ",\n";
	output += "    \"shots\": " + to_string(localPrimaryPlayer.GetMatchShots()) + ",\n";
	output += "    \"assists\": " + to_string(localPrimaryPlayer.GetMatchAssists()) + ",\n";
	output += "    \"hatTricks\": " + hatTricks + ",\n";
	output += "    \"aerialGoals\": " + aerialGoals + ",\n";
	output += "    \"backwardsGoals\": " + backwardsGoals + ",\n";
	output += "    \"bicycleGoals\": " + bicycleGoals + ",\n";
	output += "    \"longGoals\": " + longGoals + ",\n";
	output += "    \"overtimeGoals\": " + overtimeGoals + ",\n";
	output += "    \"turtleGoals\": " + turtleGoals + ",\n";
	output += "    \"poolShots\": " + poolShots + ",\n";
	output += "    \"swishes\": " + swishes + "\n  }";

	return output;
}

string JSONStatistics::BuildDefenseStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	string saves = to_string(localPrimaryPlayer.GetMatchSaves());
	string epicSaves = "0"; // Temporary placeholder -> to_string(inGameStats[epicSave]);
	string clears = "0"; // Temporary placeholder -> to_string(inGameStats[clear]);
	string ownGoals = to_string(localPrimaryPlayer.GetMatchOwnGoals());

	string output;

	output += "  \"defense\": {\n";
	output += "    \"saves\": " + saves + ",\n";
	output += "    \"epicSaves\": " + epicSaves + ",\n";
	output += "    \"clears\": " + clears + ",\n";
	output += "    \"ownGoals\": " + ownGoals + "\n  }";

	return output;
}

string JSONStatistics::BuildTouchStats() {
	PriWrapper localPrimaryPlayer = this->GetPrimaryPlayer();
	string centers = "0";
	string firstTouches = "0";

	string output;

	output += "  \"touches\": {\n";
	output += "    \"boostPickups\": " + to_string(localPrimaryPlayer.GetBoostPickups()) + ",\n";
	output += "    \"bumps\": " + to_string(localPrimaryPlayer.GetCarTouches()) + ",\n";
	output += "    \"touches\": " + to_string(localPrimaryPlayer.GetBallTouches()) + ",\n";
	output += "    \"demolitions\": " + to_string(localPrimaryPlayer.GetMatchDemolishes()) + ",\n";
	output += "    \"deaths\": " + to_string(inGameStats[death]) + ",\n";
	output += "    \"centers\": " + centers + ",\n"; // Temporary placeholder -> to_string(inGameStats[center]);;
	output += "    \"firstTouches\": " + firstTouches + "\n  }"; // Temporary placeholder -> to_string(inGameStats[firstTouch]);;

	return output;
}

string JSONStatistics::BuildFileContents() {
	string stats;
	stats += "\n\"stats\": {\n";
	stats += this->BuildScoreStats() + ",";
	stats += this->BuildOffenseStats() + ",";
	stats += this->BuildDefenseStats() + ",";
	stats += this->BuildTouchStats() + "\n}";

	string output;
	output = "{\n";
	output += this->BuildMatchData() + ",";
	output += this->BuildMetadata() + ",";
	output += this->BuildRating() + ",";
	output += stats + "\n}";

	return output;
}

//void JSONStatistics::HandleStatEvent() {
//
//}

void JSONStatistics::HandleMatchEnded(string name) {
	if (!this->IsInRealGame()) {
		return;
	}
	string fileContents = this->BuildFileContents();
	this->saveFile(fileContents);
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
	unsigned int idx = 0;
	for (auto const& [key, val] : stats) {
		string value = isNumber(val) ? val : WrapString(val, "\"");
		this->Log(key + ": " + value);
		string statLine = newLine + WrapString(key, "\"") + separator + WrapString(val, "\"");

		if (idx < stats.size()) {
			statLine += ",";
		}

		idx++;

		json.append(statLine);
	}

	json = PadLeft(json, "{", 1);
	json = PadRight(json, "}", 1);
	json.append("\n");

	this->Log(json);

	return json;
}

void JSONStatistics::saveFile(string fileContents) {
	string fileName = GetTimeStamp() + string(".json");
	filesystem::path filePath = dataDir / fileName;
	ofstream totalFile(filePath);
	totalFile << fileContents;
	totalFile.close();
	this->Log("wrote stats to " + filePath.generic_string());
	filePath.clear();
}

void JSONStatistics::Log(string msg) {
	cvarManager->log(msg);
}