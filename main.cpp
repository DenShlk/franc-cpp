#include <iostream>
#include <codecvt>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <ratio>

#include "LanguageProcessor.h"

using namespace std;

int main() {
    const std::locale empty_locale = std::locale::empty();
    typedef std::codecvt_utf8<wchar_t> converter_type;
    const converter_type* converter = new converter_type;
    const std::locale utf8_locale = std::locale(empty_locale, converter);
    std::wifstream input_stream(L"input.txt");
    input_stream.imbue(utf8_locale);

    wcin.imbue(utf8_locale);
    wcout.imbue(utf8_locale);

    wstring input;
    getline(input_stream, input);

    setlocale(LC_CTYPE, "en_US.UTF-8");
    for (std::wstring::iterator it = input.begin(); it != input.end(); ++it)
        *it = towlower(*it);

    LanguageProcessor processor;


    chrono::high_resolution_clock::time_point t_start = chrono::high_resolution_clock::now();

    for (int i = 0; i < 100; ++i) {
        processor.Detect(input);
    }

    chrono::high_resolution_clock::time_point t_end = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> elapsed = t_end - t_start;

    wcout << L"100 cycles, time elapsed: " << elapsed.count() << L"ms" << endl;
    wcout << L"getTopScript() time: " << processor.getTopScriptTime  << L"ms" << endl;
    wcout << L"getTopLanguage() time: " << processor.getTopLanguageTime  << L"ms" << endl;

    return 0;
}
