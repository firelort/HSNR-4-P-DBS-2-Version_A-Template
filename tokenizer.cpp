//
// DBS-Praktikum Aufgabe 2
//    Source Tokenizer-Interface
//
// Developed by Prof. Dr. Peter Davids
// Used by Robert Hartings & Alexander Niersmann
// Hochschule Niederrhein
// 2019/05/10
//

#include "tokenizer.h"
#include <cstring>

using namespace std;

Tokenizer::Tokenizer(string data, string sep) {
    _pos = 0;

    string::size_type beg, end;
    beg = data.find_first_not_of(sep, 0);
    end = data.find_first_of(sep, beg);

    while (string::npos != beg || string::npos != end) {
        string s = data.substr(beg, end - beg);
        _tokens.append(s);
        beg = data.find_first_not_of(sep, end);
        end = data.find_first_of(sep, beg);
    }
}

int Tokenizer::countTokens() {
    return _tokens.size();
}

string Tokenizer::nextToken() {
    return _tokens.getValueAt(_pos++);
}

bool Tokenizer::hasMoreTokens() {
    return _pos < _tokens.size();
}
