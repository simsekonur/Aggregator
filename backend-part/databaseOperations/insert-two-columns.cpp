#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <fstream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#define NUMOFRECORDS 10
#define SIZEOFVECTORS 5
using namespace std;
using namespace sql;

void takeInformationFromFile(vector<string> &connectionInfo, string fileName){
  ifstream file(fileName);
  string data;
  while (getline(file, data)){
    connectionInfo.push_back(data);
  }
}

void myExecuteStatement(PreparedStatement *pstmt, Connection *con, const SQLString &query) {
    pstmt = con->prepareStatement(query);
    pstmt->executeUpdate();
}

Connection * connectToDatabase(vector<string>& connectionInfo, Driver *driver){
    //hostname , username , password
    string hostname = "tcp://" + connectionInfo[0] + ":" + connectionInfo[1];
    string userName = connectionInfo[2];
    string password = connectionInfo[3];
    return driver->connect(hostname, userName, password);
}

void prepareAndCreateTable(Connection *con,Statement *stmt){
    con->setSchema("aggregator");
    stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS leafNodes");
    stmt->execute("CREATE TABLE leafNodes(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, temperature JSON, pressure JSON)");
}

void generateRandomNumbers(int howMany, int maxRange,vector<double> &v){
  for (int i =0; i< howMany;i++){
    double number = rand()%maxRange+1;
    v.push_back(number);
  }
}

void normalizeTheVector(vector<double> &v){
  double sum=0.0;
  int s = v.size();
  for(int i=0; i< s;i++){
    sum += v[i];
  }
  for(int i=0; i< s;i++){
    v[i] /= sum;
  }
}

int main(){
  vector<double> temperatureSequence;
  vector<double> pressureSequence;

  vector<string> connectionInfo;
  string fileName = "config.txt";
  Driver *driver;
  Connection *con;
  Statement *stmt;
  ResultSet *res;
  PreparedStatement *pstmt;

  srand(time(0));

  try{
    driver = get_driver_instance();
    // read the config.txt and take the information from there
    takeInformationFromFile(connectionInfo, fileName);
    
    // connect to the database
    con = connectToDatabase(connectionInfo, driver);

    // create the leafNodes table
    prepareAndCreateTable(con,stmt);
 
    for (int i=0 ; i < NUMOFRECORDS;i++){
      temperatureSequence.clear();
      pressureSequence.clear();
      generateRandomNumbers(SIZEOFVECTORS,NUMOFRECORDS,temperatureSequence);
      generateRandomNumbers(SIZEOFVECTORS,NUMOFRECORDS,pressureSequence);
      //normalizeTheVector(temperatureSequence);
      //normalizeTheVector(pressureSequence);

      SQLString tmp = "INSERT INTO leafNodes(temperature, pressure) VALUES ('[" ;
      for (int j=0; j < SIZEOFVECTORS;j++){      
        if (j== SIZEOFVECTORS -1){
          tmp += to_string(temperatureSequence[j]);
          //tmp += "]')";
          tmp += "]', ";
          for(int k=0; k < SIZEOFVECTORS;k++){
            if (k== 0){
              tmp += "'[";
              tmp += to_string(pressureSequence[k]);
              tmp += ", ";
    
            }
            else if (k== SIZEOFVECTORS -1){
              tmp += to_string(pressureSequence[k]);
              tmp += "]')";
              //cout << tmp << endl;
            }
            else {
              tmp += to_string(pressureSequence[k]);
              tmp += ",";
            }
          }
        }
        else {
          tmp += to_string(temperatureSequence[j]);
          tmp += ",";
        }
      }
      
      myExecuteStatement(pstmt,con,tmp);
      
    }

    cout << "The values are inserted into table!" << endl;
  }
  catch (SQLException e)
  {
    cout << "Could not connect to server. Error message: " << e.what() << endl;
    system("pause");
    exit(1);
  }

  return 0;
}
