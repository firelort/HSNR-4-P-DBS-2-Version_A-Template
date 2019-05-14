//
// DBS-Praktikum Aufgabe 2
//    Source DB import
//
// Robert Hartings & Alexander Niersmann
// Hochschule Niederrhein
// 2019/05/10
//


#include <iostream>
#include <fstream>
#include "db.h"
#include "tokenizer.h"

/**
 * Exits the program with information about the command line parameters
 */

void exitWithParameterError() {
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
 * Exits the program with a given error msg
 */
void exitWithMsg(std::string error) {
    //Print the error to the console
    std::cout << error << std::endl;

    //Close/Finish db connection
    db_logout();
    exit(EXIT_FAILURE);
}

/**
 * Program can import data from a file into a psql database.
 * @param argc counter of the number of parameters
 * @param argv Array of char arrays, each array is an input string
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
                    exitWithParameterError();
                }

                //Increase the value of i by two
                i += 2;
            } else { // Return with an error, because the key can't have a value
                exitWithParameterError();
            }
        }

    }

    //Test if username, password, hostname & database are given
    if (username.empty() || password.empty() || hostname.empty() || databaseName.empty()) {
        exitWithParameterError();
    }

    //Open a file stream with the given filename
    std::ifstream file(argv[argc - 1]);

    //Test if the file is good/exists
    if (!file.good()) {
        exitWithParameterError();
    }

    //Login to database and open database connection
    if (db_login(username, password, hostname, databaseName)) {
        std::string msg = "SQL Error | Couldn't login to database '";
        msg.append(databaseName).append("' at '").append(hostname).append("' with the user '").append(username).append(
                "' and the given password. (password isn't empty)");
        exitWithMsg(msg);
    }

    //Begin commit
    if (db_begin()) {
        exitWithMsg("SQL Error | Couldn't begin the sql transaction.");
    }

    //If del is set -> delete table
    if (deleteTableContent) {
        if (db_delete()) {
            std::string msg = "SQL Error | Couldn't delete the table content.";
            if (db_rollback()) {
                msg += " Warning: An error occurred while performing rollback.";
            }
            exitWithMsg(msg);
        }

    }

    //Counter for rows read
    unsigned int rowsRead = 0;
    //Counter for inserted rows
    unsigned int rowsInserted = 0;
    //Buffer for strings read, hnr, name, plz and ort
    std::string line, hnr, name, plz, ort;
    //Buffer for return value of db_findhnr;
    int bufferFindhnr;

    //Get lines of file
    while (std::getline(file, line)) {
        rowsRead += 1;

        //Tokenize the line
        Tokenizer tok(line, ";");

        //Test if all variables are present
        if (tok.countTokens() != 4) {
            std::string msg = "FILE Error | The given file isn't in the right format.";
            if (db_rollback()) {
                msg += " Warning: An error occurred while performing rollback.";
            }
            exitWithMsg(msg);
        }

        hnr = tok.nextToken();
        name = tok.nextToken();
        plz = tok.nextToken();
        ort = tok.nextToken();

        //Test if entry is already in database --> skip the entry from file
        bufferFindhnr = db_findhnr(hnr);

        if (bufferFindhnr == -1) {
            //db_findhnr returned an error
            std::string msg = "SQL Error | Couldn't perform search for hnr successfully.";
            if (db_rollback()) {
                msg += " Warning: An error occurred while performing rollback.";
            }
            exitWithMsg(msg);
        } else if (bufferFindhnr == 1) {
            // Skip insert, because row is present.
        } else if (bufferFindhnr == 0) {
            //insert values into database
            if (db_insert(hnr, name, plz, ort)) {
                std::string msg = "SQL Error | Couldn't insert row into database.";
                if (db_rollback()) {
                    msg += " Warning: An error occurred while performing rollback.";
                }
                exitWithMsg(msg);
            }
            rowsInserted += 1;
        } else {
            std::string msg = "PROGRAMMING Error | db_findhnr returned an unexpected value.";
            if (db_rollback()) {
                msg += " Warning: An error occurred while performing rollback.";
            }
            exitWithMsg(msg);
        }
    }

    //if all success commit
    if (db_commit()) {
        std::string msg = "SQL Error | Couldn't commit changes.";
        if (db_rollback()) {
            msg += " Warning: An error occurred while performing rollback.";
        }
        exitWithMsg(msg);
    }
    //Close database connection
    db_logout();

    //Create import statistics
    std::cout << argv[0] << " " << argv[argc - 1] << "  | " << rowsRead << "/" << rowsInserted << std::endl;
    return 0;
}

