//
// DBS-Praktikum Aufgabe 2
//    Source Tokenizer-Interface
//
// Developed by Prof. Dr. Peter Davids
// Used by Robert Hartings & Alexander Niersmann
// Hochschule Niederrhein
// 2019/05/10
//

#include "Tokenizer.h"

Tokenizer::Tokenizer(string data, string separator) {
    _pos = 0;
    char *dat = (char *) data.c_str();
    char *sep = (char *) separator.c_str();

    char *tok = strtok(dat, sep);
    while (tok != 0) {
        _tokens.insertNewEntry(string(tok));
        tok = strtok(0, sep);
    }
}

int Tokenizer::countTokens() {
    return _tokens.getListSize();
}

bool Tokenizer::hasMoreTokens() {
    return _pos < _tokens.getListSize();
}

string Tokenizer::nextToken() {
    return _tokens.getValueAtPos(_pos++);
}