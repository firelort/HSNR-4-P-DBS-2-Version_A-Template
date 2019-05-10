//
// DBS-Praktikum Aufgabe 2
//    Source DB-Interface
//
// Robert Hartings & Alexander Niersmann
// Hochschule Niederrhein
// 2019/05/10
//

#include "db.h"
#include <iostream>
#include <libpq-fe.h>

static PGconn *connection;

// Datenbank-Login
// rc: 0 = ok, 1 = error
int db_login(const string &user, const string &password, const string &host, const string &dbname) {
    std::string conninfo = "";
    //Build conninfo string with all given values plus timeout
    conninfo.append("port=5432 host=").append(host).append(" dbname=").append(dbname).append(" user=").append(user).append(" password=").append(password);

    //Make a connection to the database
    connection = PQconnectdb(conninfo.c_str());
    //Check if backend was successfully made
    if (PQstatus(connection) != CONNECTION_OK) {
        //print error msg
        std::cout << "SQL Error | Connection to database failed: " << PQerrorMessage(connection) << std::endl;

        //finish connection
        PQfinish(connection);

        //return with error code
        return 1;
    }

    //return with success code
    return 0;
}

// Datenbank-Logout
void db_logout() {
    // Close the connection and cleanup
    PQfinish(connection);
}

// Transaktionsbefehle
// rc: 0 = ok, 1 = error
int db_begin() {
    PGresult *res;
    //Execute sql command BEGIN to start a new transaction
    res = PQexec(connection, "BEGIN");

    //Test if the BEGIN command was successfully
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        //print error msg
        std::cout << "SQL Error | BEGIN command failed: " << PQerrorMessage(connection) << std::endl;

        //finish connection
        PQfinish(connection);

        //Clear the result variable
        PQclear(res);

        //return with error code
        return 1;
    }
    //Clear the result variable
    PQclear(res);

    //return with success code
    return 0;
}

int db_commit() {
    PGresult *res;
    //Execute sql command COMMIT / postgres coammand END to end a transaction and save changes
    res = PQexec(connection, "END");

    //Test if the END command was successfully
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        //print error msg
        std::cout << "SQL Error | END command failed: " << PQerrorMessage(connection) << std::endl;

        //finish connection
        PQfinish(connection);

        //Clear the result variable
        PQclear(res);

        //return with error code
        return 1;
    }
    //Clear the result variable
    PQclear(res);

    //return with success code
    return 0;
}

int db_rollback() {
    PGresult *res;
    //Execute sql command ROLLBACK to end a transaction and discard changes
    res = PQexec(connection, "ROLLBACK");

    //Test if the ROLLBACK command was successfully
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        //print error msg
        std::cout << "SQL Error | ROLLBACK command failed: " << PQerrorMessage(connection) << std::endl;

        //finish connection
        PQfinish(connection);

        //Clear the result variable
        PQclear(res);

        //return with error code
        return 1;
    }
    //Clear the result variable
    PQclear(res);

    //return with success code
    return 0;
}

// Herstellernummer schon vorhanden?
// rc: 0 = noch nicht da, 1 = schon da, -1 = error
int db_findhnr(const string &hnr) {
    PGresult *res;

    std::string command = "SELECT hnr FROM hersteller WHERE hnr='";
    command.append(hnr).append("';");

    //Execute sql command DELETE from datbase; to delete all entries
    res = PQexec(connection, command.c_str());

    //Test if the command was successfully
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        //print error msg
        std::cout << "SQL Error | SELECT command " << command << " failed: " << PQerrorMessage(connection) << std::endl;

        //Clear the result variable
        PQclear(res);

        //return with error code
        return -1;
    }

    //Check if hnr(key) is present in the database
    if (PQntuples(res) > 0) {
        //Clear the result variable
        PQclear(res);

        //return with info that hnr is in the database
        return 1;
    }

    //Clear the result variable
    PQclear(res);

    //return with info that hnr isn't in the database
    return 0;
}

// Einfuegen Datensatz
// rc: 0 = ok, 1 = error
int db_insert(const string &hnr, const string &name, const string &plz, const string &ort) {
    PGresult *res;

    std::string command = "INSERT INTO hersteller (hnr, name, plz, ort) VALUES ('";
    command.append(hnr).append("', '").append(name).append("', '").append(plz).append("', '").append(ort).append("');");

    //Execute sql command INSERT to add a entry
    res = PQexec(connection, command.c_str());

    //Test if the command was successfully
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        //print error msg
        std::cout << "SQL Error | INSERT command -" << command << "- failed: " << PQerrorMessage(connection)
                  << std::endl;

        //Clear the result variable
        PQclear(res);

        //return with error code
        return 1;
    }
    //Clear the result variable
    PQclear(res);

    //return with success code
    return 0;
}

// Loeschen des kompletten Tabelleninhalts
// rc: 0 = ok, 1 = error
int db_delete() {
    PGresult *res;

    //Execute sql command DELETE from datbase; to delete all entries
    res = PQexec(connection, "DELETE FROM hersteller;");

    //Test if the command was successfully
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        //print error msg
        std::cout << "SQL Error | DELTE command failed: " << PQerrorMessage(connection) << std::endl;


        //Clear the result variable
        PQclear(res);

        //return with error code
        return 1;
    }
    //Clear the result variable
    PQclear(res);

    //return with success code
    return 0;
}