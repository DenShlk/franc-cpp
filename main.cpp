#include <iostream>
#include <codecvt>
#include <string>
#include <fstream>

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

    LanguageProcessor processor;

    wcout << processor.Detect(input) << endl;
    return 0;
}
