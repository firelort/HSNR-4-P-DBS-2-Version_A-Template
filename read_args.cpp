//
// Created by anier on 23.04.2019.
//

#include <iostream>
using namespace std;


/**
 * Wie kann man in einem C++-Programm die Kommandozeilenargumente auslesen? Legen Sie
 * Beispielcode fur das Auslesen der Argumente vor.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv)
{
    for (int i = 0; i < argc; i++){
        cout << argv[i] << endl;
    }
    return 0;
}

