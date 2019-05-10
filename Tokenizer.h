//
// DBS-Praktikum Aufgabe 2
//    Header Tokenizer-Interface
//
// Developed by Prof. Dr. Peter Davids
// Used by Robert Hartings & Alexander Niersmann
// Hochschule Niederrhein
// 2019/05/10
//

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <list.h>


class Tokenizer {
protected:
    unsigned int _pos;
    List <string> _tokens;
public:
    Tokenizer(string data, string separator);

    int countTokens();

    std::string nextToken();

    bool hasMoreTokens();
};


#endif //TOKENIZER_H
