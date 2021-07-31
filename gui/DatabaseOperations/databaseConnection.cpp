#include <stdlib.h>
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;

void takeInformationFromFile(vector<string> &connectionInfo, string fileName)
{
  ifstream file(fileName);
  string data;
  while (getline(file, data))
  {
    connectionInfo.push_back(data);
  }
}

int main()
{
  vector<string> connectionInfo;
  string fileName = "config.txt";
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;
  sql::PreparedStatement *pstmt;

  try
  {
    driver = get_driver_instance();
    //read the config.txt and take the information from there
    takeInformationFromFile(connectionInfo, fileName);
    //test the value

    //hostname , username , password
    string hostname = "tcp://" + connectionInfo[0] + ":" + connectionInfo[1];
    string userName = connectionInfo[2];
    string password = connectionInfo[3];
    con = driver->connect(hostname, userName, password);
    con->setSchema("aggregator");

    stmt = con->createStatement();
    stmt->execute("DROP TABLE IF EXISTS leafNodes");

    stmt->execute("CREATE TABLE leafNodes(id INT, label CHAR(1))");

    //free the space allocated to stmt pointer as we won't use it later

    delete stmt;
    pstmt = con->prepareStatement("INSERT INTO leafNodes(id) VALUES (?)");
    for (int i = 1; i <= 10; i++)
    {
      pstmt->setInt(1, i);
      pstmt->executeUpdate();
    }
    delete pstmt;
    pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
    res = pstmt->executeQuery();

    cout << "Connection is established and values are inserted!" << endl;
  }
  catch (sql::SQLException e)
  {
    cout << "Could not connect to server. Error message: " << e.what() << endl;
    system("pause");
    exit(1);
  }

  return 0;
}
