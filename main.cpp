#include <iostream>
#include <fstream>

/**
 * TBD
 */

void exitWithError() {
    std::cout << "Usage:" << std::endl
              << "         dbimp [options] <inflie>" << std::endl
              << "Options:" << std::endl
              << "         -del  delete table contents before import" << std::endl
              << "         -u database user" << std::endl
              << "         -p password" << std::endl
              << "         -h database host" << std::endl
              << "         -d database name" << std::endl;
    exit(EXIT_FAILURE);
}

/**
 * TBD
 * @param argc
 * @param argv
 * @return
 */


int main(int argc, char **argv) {

    //String for commandline parameter key
    std::string key;
    //String for commandline parameter value
    std::string value;
    //String for the user value
    std::string username;
    //String for the password for the user
    std::string password;
    //String for the database host
    std::string hostname;
    //String for the database name
    std::string databaseName;
    //Bool for delete table content
    bool deleteTableContent = false;

    //Skip program name -> ignore argc = 0
    //Ignore the infile name argv[argc-1]
    for (int i = 1; i < argc - 1;) {
        //Get the parameter key
        key = argv[i];

        //Test if the parameter is -del
        if (key == "-del") {
            deleteTableContent = true;
            //Increase the value of i by one
            i += 1;
        } else {
            //Test if a key can have a value by the given length
            // i + 1 must be smaller than argc - 1 --> +1 on both side -> result i+2 < argc
            if (i + 2 < argc) {
                //Get the parameter value
                value = argv[i + 1];

                //Test if the key is on of the following
                if (key == "-u") {
                    username = value;
                } else if (key == "-p") {
                    password = value;
                } else if (key == "-h") {
                    hostname = value;
                } else if (key == "-d") {
                    databaseName = value;
                } else { //Return with an error, because the key wasn't found in the parameter list
                    exitWithError();
                }

                //Increase the value of i by two
                i += 2;
            } else { // Return with an error, because the key can't have a value
                exitWithError();
            }
        }

    }

    //Test if username, password, hostname & database are given
    if(username.empty() || password.empty() || hostname.empty() || databaseName.empty()) {
        exitWithError();
    }

    //Open a file stream with the given filename
    std::ifstream file(argv[argc - 1]);

    //Test if the file is good/exists
    if (!file.good()) {
        exitWithError();
    }


    return 0;
}

