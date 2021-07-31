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
#include "databaseHandler.hpp"
#define NUMOFRECORDS 15
#define SIZEOFVECTORS 10
using namespace std;

void DatabaseHandler::takeInformationFromFile(vector<string> &connectionInfo, string fileName){
  ifstream file(fileName);
  string data;
  while (getline(file, data)){
    connectionInfo.push_back(data);
  }
}

void DatabaseHandler::myExecuteStatement(sql::PreparedStatement *pstmt, sql::Connection *con, const sql::SQLString &sql) {
    pstmt = con->prepareStatement(sql);
    pstmt->executeUpdate();
}

sql::Connection * DatabaseHandler::connectToDatabase(vector<string>& connectionInfo, sql::Driver *driver){
    //hostname , username , password
    string hostname = "tcp://" + connectionInfo[0] + ":" + connectionInfo[1];
    string userName = connectionInfo[2];
    string password = connectionInfo[3];
    return driver->connect(hostname, userName, password);
}
void DatabaseHandler::receiveDataFromColumn(sql::SQLString &column){
    sql::SQLString tmp = "SELECT " + column + " FROM leafNodes"; 
    res= stmt->executeQuery(tmp);
    while (res->next()) {
        receivedData.push_back(res->getString(column));
        //cout << res->getString("temperature") << endl;
        
    }
}
void DatabaseHandler::receiveDataAsVector(){
    vector<double> tmp;
    string delimiter = ", ";
    size_t pos = 0;
    string s= "";
    string token = ""; 
    int len =0;
    for(int i=0; i< receivedData.size();i++){
        tmp.clear();       
        len = receivedData[i].length();
        s = receivedData[i].substr(1,len-2);
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            tmp.push_back(stod(token));
            //std::cout << token << std::endl;
            s.erase(0, pos + delimiter.length());
        }
        //std::cout << s << endl;
        tmp.push_back(stod(s));
        dataAsVectors.push_back(tmp);
    }
}
vector<double> DatabaseHandler::getData(int id, sql::SQLString &column){
    this->receiveDataFromColumn(column);
    this->receiveDataAsVector();
    return this->dataAsVectors[id];
}
int main(){

  srand(time(0));
  DatabaseHandler * db = new DatabaseHandler();
  try{
    db->driver = get_driver_instance();
    // read the config.txt and take the information from there
    string fileName = db->fileName;
    vector<string> connectionInfo;
    db->takeInformationFromFile(connectionInfo,fileName);
    
    // connect to the database
    db->con = db->connectToDatabase(connectionInfo,db->driver);

    db->con->setSchema("aggregator");
    db->stmt = db->con->createStatement();

    sql::SQLString column = "temperature";

    db->specificRow = db->getData(1,column);

    // just for testing purposes
    for (int i=0; i< db->specificRow.size();i++){
        cout << db->specificRow[i] << " ";
    }
    cout << endl;

    cout << "The values are read from table!" << endl;
  }
  catch (sql::SQLException e)
  {
    cout << "Could not connect to server. Error message: " << e.what() << endl;
    system("pause");
    exit(1);
  }

  return 0;
}
