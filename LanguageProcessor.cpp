//
// Created by denshlk on 01.02.2021.
//
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <cstdio>
#include <iostream>
#include "LanguageProcessor.h"
#include <codecvt>
#include <fstream>

#include <chrono>
#include <ctime>
#include <ratio>

using namespace std;

chrono::high_resolution_clock::time_point t_start;
chrono::high_resolution_clock::time_point t_end;

/**
 * Gets list of scripts from data file. (Script is a family of  languages).
 * Each script has regex for it's characters. Script usually contains languages, each of them has trigram list.
 * Some scripts are languages themselves.
 */
void LanguageProcessor::readData() {
    const std::locale empty_locale = std::locale::empty();
    typedef std::codecvt_utf8<wchar_t> converter_type;
    const converter_type* converter = new converter_type;
    const std::locale utf8_locale = std::locale(empty_locale, converter);
    std::wifstream data_stream(L"data.txt");
    data_stream.imbue(utf8_locale);

    int scripts_count;
    data_stream >> scripts_count;

    for (int i = 0; i < scripts_count; ++i) {

        wstring script;
        wstring script_regex;
        data_stream >> ws;
        data_stream >> script;

        data_stream >> ws;
        getline(data_stream, script_regex);

        scripts.emplace_back(Script(script, script_regex));

        int langs_count;
        data_stream >> langs_count;

        for (int j = 0; j < langs_count; ++j) {

            wstring lang;
            wstring lang_trigrams;
            data_stream >> lang;
            data_stream >> ws;
            getline(data_stream, lang_trigrams);

            scripts.back().lang_names.push_back(lang);
            scripts.back().trigrams.emplace_back();

            for (int weight = 0; weight*3 < lang_trigrams.size(); ++weight) {
                wstring tri = lang_trigrams.substr(weight * 3, 3);
                if(!weight && tri[0] == '_'){
                    tri = L" " + tri.substr(1,2);
                }
                scripts.back()
                        .trigrams[j][tri] = weight;
            }
        }
    }
}

LanguageProcessor::LanguageProcessor() {
    readData();
}


/**
 * Get the most probable language for the given value.
 *
 * @param {string} value - The value to test.
 * @param {Object} options - Configuration.
 * @return {string} The most probable language.
 */
wstring LanguageProcessor::Detect(wstring &value) {
    if (value.size() > MAX_LENGTH) {
        value.resize(MAX_LENGTH);
    }
    if (value.size() < MIN_LENGTH) {
        //report error?
        return L"";
    }

    Script script(L"??", L"");

    t_start = chrono::high_resolution_clock::now();
    getTopScript(value, script);
    t_end = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> elapsed = t_end - t_start;
    getTopScriptTime += elapsed.count();

    if(script.lang_names.empty()){
        //mono-script (script===language)
        return script.name;
    }

    t_start = chrono::high_resolution_clock::now();
    auto result = getTopLanguage(value, script);
    t_end = chrono::high_resolution_clock::now();

    elapsed = t_end - t_start;
    getTopLanguageTime += elapsed.count();

    return result;
}



/**
* From `scripts`, get the most occurring expression for
* `value`.
*
* @param {string} value - Value to check.
* @param {Object.<RegExp>} scripts - Top-Scripts.
* @return {Array} Top script and its
*   occurrence percentage.
*/
void LanguageProcessor::getTopScript(wstring &value, Script &result){
    float topRatio = -1;

    for(Script script : scripts){
        float ratio = getOccurrence(value, script.regex_exp);

        //wcout << script.name << L" " << ratio << endl;

        if(ratio > topRatio){
            result = script;
            topRatio = ratio;

            if(ratio > 0.4f) return;
        }
    }
}

/**
 * Get the occurrence ratio of `expression` for `value`.
 *
 * @param {string} value - Value to check.
 * @param {RegExp} expression - Code-point expression.
 * @return {number} Float between 0 and 1.
 */
float LanguageProcessor::getOccurrence(wstring &value, wregex &pattern){

    std::ptrdiff_t const match_count(std::distance(
            wsregex_iterator(value.begin(), value.end(), pattern),
            wsregex_iterator()));

    int count = match_count;

    return 1.f * count / value.size();
}

/**
 * Iterates throw the all languages and gets distance from given string for each. Returns the name of the closest.
 * @param value
 * @param script
 * @return
 */
wstring LanguageProcessor::getTopLanguage(wstring &value, Script &script){
    wstring topLanguage = L"??";
    int topDistance = INT32_MAX;

    vector<pair<wstring, int>> trigramsCounts;
    getTrigramsCounts(value, trigramsCounts);

    for (int i = 0; i < script.lang_names.size(); ++i) {
        int distance = getDistance(trigramsCounts, script.trigrams[i]);

        //wcout << script.lang_names[i] << L" " << distance << endl;

        if(distance < topDistance){
            topDistance = distance;
            topLanguage = script.lang_names[i];
        }
    }

    return topLanguage;
}

/**
 * Calculates array <trigram : count in string> for given string.
 * @param value
 * @return
 */
void LanguageProcessor::getTrigramsCounts(wstring &value, vector<pair<wstring, int>> &result){
    map<wstring, int> count;
    for (int i = 0; i < value.length() - 2; ++i) {
        count[value.substr(i, 3)]++;
    }

    for(auto p : count){
        //wcout << p.first << " " << p.second << endl;
        result.push_back({p.first, p.second});
    }
}

/**
 * Get the distance between an array of trigram--count
 * tuples, and a language dictionary.
 *
 * @param {Array.<Array.<string, number>>} trigrams - An
 *   array containing trigram--count tuples.
 * @param {Object.<number>} model - Object
 *   containing weighted trigrams.
 * @return {number} - The distance between the two.
 */
int LanguageProcessor::getDistance(vector<pair<wstring, int>> &trigramsCounts, map<wstring, int> &model){
    int distance = 0;

    for (int i = 0; i < trigramsCounts.size(); ++i) {
        auto trigram = trigramsCounts[i];

        //wcout << trigrams.first << " " << model[trigram.first] << endl;

        int difference;
        if(model.count(trigram.first)){
             difference = abs(trigram.second - model[trigram.first] - 1);
        }else{
            difference = MAX_DIFFERENCE;
        }

        distance += difference;
    }

    return distance;
}