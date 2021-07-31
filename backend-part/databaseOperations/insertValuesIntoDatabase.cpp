#include <stdlib.h>
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "nlohmann/json.hpp"
  
using json = nlohmann::json;
using namespace std;
using namespace sql;

void takeInformationFromFile(vector<string> &connectionInfo, string fileName){
  ifstream file(fileName);
  string data;
  while (getline(file, data)){
    connectionInfo.push_back(data);
  }
}

void myExecuteStatement(sql::PreparedStatement *pstmt, sql::Connection *con, const sql::SQLString &sql) {
    pstmt = con->prepareStatement(sql);
    pstmt->executeUpdate();
}

sql::Connection * connectToDatabase(vector<string>& connectionInfo, sql::Driver *driver){
    //hostname , username , password
    string hostname = "tcp://" + connectionInfo[0] + ":" + connectionInfo[1];
    string userName = connectionInfo[2];
    string password = connectionInfo[3];
    return driver->connect(hostname, userName, password);
}

void prepareAndCreateTable(sql::Connection *con,sql::Statement *stmt){
    con->setSchema("aggregator");
    stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS leafNodes");
    stmt->execute("CREATE TABLE leafNodes(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, temperature JSON)");
}


int main(){
  vector<string> connectionInfo;
  string fileName = "config.txt";
  Driver *driver ;
  Connection *con;
  Statement *stmt;
  ResultSet *res;
  PreparedStatement *pstmt;

  try{
    driver = get_driver_instance();
    // read the config.txt and take the information from there
    takeInformationFromFile(connectionInfo, fileName);
    
    // connect to the database
    con = connectToDatabase(connectionInfo, driver);

    // create the leafNodes table
    prepareAndCreateTable(con,stmt);

    //Create an empty array into which you will put your data
    auto datas = json::array({});
    // push all data required for our database
    datas.push_back(1.0);
    datas.push_back(1.1);
    datas.push_back(1.2);
    datas.push_back(1.3);
    datas.push_back(1.4);
    datas.push_back(1.5);
    datas.push_back(1.6);
    datas.push_back(1.7);
    datas.push_back(1.8);
    datas.push_back(1.9);
    //execute insert statement into the leafNode table
    myExecuteStatement(pstmt,con,"INSERT INTO leafNodes(temperature) VALUES (@datas)");

    cout << "The values are inserted into table!" << endl;
  }
  catch (sql::SQLException e){
    cout << "Could not connect to server. Error message: " << e.what() << endl;
    system("pause");
    exit(1);
  }

  return 0;
}
