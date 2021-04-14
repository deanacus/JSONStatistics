#include "Utils.h"

string TrimRight(string str, string chars) {
    return str.erase(str.find_last_of(chars) + 1);
}

string TrimLeft(string str, string chars) {
    return str.erase(0, str.find_first_of(chars));
}

string PadLeft(string str, string chr, int count) {
    string padding = "";
    for (int i = 0; i < count; i++) {
        padding += chr;
    }
    return chr + str;
}

string PadRight(string str, string chr, int count) {
    string padding = "";
    for (int i = 0; i < count; i++) {
        padding += chr;
    }
    return str + chr;
}

string WrapString(string str, string chr) {
    return chr + str + chr;
}

bool isNumber(string str) {
    return !str.empty() && str.find_first_not_of("-0123456789") == string::npos;
}

string GetTimeStamp() {
	time_t rawtime;
	time(&rawtime);
	char timestamp[sizeof "2021-04-14-13-13-13"];
  strftime(timestamp, sizeof timestamp, "%F-%H-%M-%S", gmtime(&rawtime));
	return timestamp;
}