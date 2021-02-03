//
// Created by denshlk on 01.02.2021.
//
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <cstdio>
#include <iostream>

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

    Script getTopScript(wstring value);

    float getOccurrence(wstring value, wregex pattern);

    wstring LanguageProcessor::getTopLanguage(wstring value, Script script);

    vector<pair<wstring, int>> LanguageProcessor::getTrigramsCounts(wstring value);

    int LanguageProcessor::getDistance(vector<pair<wstring, int>> trigrams, map<wstring, int> model);

public:

    /* Maximum sample length. */
    const int MAX_LENGTH = 2048;

    /* Minimum sample length. */
    const int MIN_LENGTH = 10;

    LanguageProcessor();

    wstring Detect(wstring value);
};