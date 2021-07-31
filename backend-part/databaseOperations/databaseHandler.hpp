#include <vector>
#include <stdlib.h>
#include <iostream>
#include <typeinfo>
#include <string.h>
#include <bits/stdc++.h>
#include <fstream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;
class DatabaseHandler {
    public:
        vector<double> specificRow;
        vector<vector<double>> dataAsVectors;
        vector<string> connectionInfo;
        string fileName = "config.txt";
        sql::Driver *driver;
        vector<sql::SQLString> receivedData; 
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        void takeInformationFromFile(vector<string> &connectionInfo, string fileName);
        void myExecuteStatement(sql::PreparedStatement *pstmt, sql::Connection *con, const sql::SQLString &sql);
        sql::Connection * connectToDatabase(vector<string>& connectionInfo, sql::Driver *driver);
        void receiveDataFromColumn(sql::SQLString &column);
        void receiveDataAsVector();
        vector<double> getData(int id,sql::SQLString &column);
};
