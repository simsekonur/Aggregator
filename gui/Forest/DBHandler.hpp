#pragma once

#include <QMutex>
#include <map>
#include <vector>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/mysql_connection.h>
#include "UINode.hpp"

class DBHandler {
private:
    sql::Driver *driver;
    sql::ResultSet* query(const std::string& query) const;
    void parseJsonForVector(std::vector<double>& targetData, const sql::SQLString& jsonStr) const;
    sql::SQLString host;
    sql::SQLString database;
    sql::SQLString user;
    sql::SQLString passw;

public:
    DBHandler();
    bool connectDatabase(const std::string& host, const std::string& database, const std::string& user, const std::string& passw);
    std::vector<double> getData(const std::string& table, const std::string& field, unsigned int id) const;
    int getCount(const std::string& table) const;
    void findFieldMinMax(DataConfig& d, const std::string& table, const std::string& field) const;
};
