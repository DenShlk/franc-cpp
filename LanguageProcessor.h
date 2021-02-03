//
// Created by denshlk on 01.02.2021.
//
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <cstdio>
#include <iostream>

#include <chrono>
#include <ctime>
#include <ratio>

using namespace std;

class LanguageProcessor{
private:
    struct Script {
        wstring name;
        wregex regex_exp;

        Script(wstring name, wstring regex_exp) : name(name), regex_exp(wregex(regex_exp)) {}

        vector<wstring> lang_names;
        vector<map<wstring, int>> trigrams;
    };

    /* The maximum distance to add when a given trigram does
    * not exist in a trigram dictionary. */
    const int MAX_DIFFERENCE = 300;

    vector<LanguageProcessor::Script> scripts;

    void readData();

    void getTopScript(wstring &value, Script &result);

    float getOccurrence(wstring &value, wregex &pattern);

    wstring getTopLanguage(wstring &value, Script &script);

    void getTrigramsCounts(wstring &value, vector<pair<wstring, int>> &result);

    int getDistance(vector<pair<wstring, int>> &trigrams, map<wstring, int> &model);

public:

    double getTopScriptTime = 0;
    double getTopLanguageTime = 0;

    /* Maximum sample length. */
    const int MAX_LENGTH = 2048;

    /* Minimum sample length. */
    const int MIN_LENGTH = 10;

    LanguageProcessor();

    wstring Detect(wstring &value);
};