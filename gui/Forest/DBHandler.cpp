#include "DBHandler.hpp"
#include "AggExceptions.hpp"
#include <sstream>
#include <limits>
#include <algorithm>

DBHandler::DBHandler() {
    driver = get_driver_instance();
    if (!driver)
        throw database_exception("Error while initializing database driver.");
}

bool DBHandler::connectDatabase(
        const std::string& host,
        const std::string& database,
        const std::string& user,
        const std::string& passw) {
    using namespace sql;
    this->host = SQLString(host);
    this->database = SQLString(database);
    this->user = SQLString(user);
    this->passw = SQLString(passw);


}

void DBHandler::parseJsonForVector(std::vector<double>& fillingData, const sql::SQLString& resStr) const {
    try {
        std::stringstream tmp;
        std::stringstream json;
        tmp << resStr.asStdString();
        char c;
        while (tmp >> c) { // skip the space characters
            if (c != ' '  && c != '\t')
                json << c;
        }
        json >> c; // '['
        while (c != ']') { // until list ends
            std::stringstream number;
            json >> c;
            while (c != ',' && c != ']') {
                number << c;
                json >> c;
            }
            char *check;
            double d = std::strtod(number.str().c_str(), &check);
            if (*check) {
                throw missing_value_exception("null value in dataset");
            }
            else {
                fillingData.push_back(d);
                number.clear();
            }

            if (c == ']')
                break;
        }
    }
    catch (missing_value_exception& me) {
        throw me; 
    }
    catch (...) {
        throw database_exception("Error while parsing JSON array.");
    }
}

sql::ResultSet* DBHandler::query(const std::string& query) const {
    using namespace sql;

    Connection* con;
    con = driver->connect(host, user, passw);
    con->setSchema(database);

    try {
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery(SQLString(query));
        delete stmt;
        delete con;
        return res;
    }
    catch (sql::SQLException& e) {
        if (con) delete con;
        throw database_exception(e.what());
    }
    catch (...) {
        if (con) delete con;
        throw database_exception(std::string("Error while executing database query."));
    }
}

std::vector<double> DBHandler::getData(
        const std::string& table,
        const std::string& field,
        unsigned int id) const {
    
    using namespace sql;

    std::stringstream queryStr;
    queryStr << "SELECT " << field << " FROM " << table << " WHERE id" << "=" << id << ";"; 
    ResultSet* res = query(queryStr.str());
    
    if (res->next()) {
        std::vector<double> result;
        SQLString resStr = res->getString(1);
        parseJsonForVector(result, resStr);
        delete res;
        return result;
    }
    else {
        delete res;
        std::stringstream err;
        err << "No record found in database for id: " << id << ", field: " << field;
        throw database_exception(err.str());
    }
}

int DBHandler::getCount(const std::string& table) const {
    std::stringstream queryStr;
    queryStr << "SELECT COUNT(*) FROM " << table << ";";
    sql::ResultSet* res = query(queryStr.str());
    int count;
    if (res->next())
        count = res->getInt(1);
    else {
        delete res;
        throw database_exception("No result while getting count of table from database.");
    }
    delete res;
    return count;
}

void DBHandler::findFieldMinMax(DataConfig& d, const std::string& table, const std::string& field) const {
    using namespace sql;

    double max = std::numeric_limits<double>::min();
    double min = std::numeric_limits<double>::max();

    ResultSet* res = query("SELECT " + field + " FROM " + table + ";");

    while (res->next()) {
        std::vector<double> result;
        SQLString resStr = res->getString(1);
        try {
            parseJsonForVector(result, resStr);
            double localMax = *std::max_element(result.begin(), result.end(), [] (const double& a, const double& b) {return a < b;} );
            double localMin = *std::min_element(result.begin(), result.end(), [] (const double& a, const double& b) {return a < b;} );
            if (localMax > max)
                max = localMax;
            if (localMin < min)
                min = localMin;
        }
        catch (missing_value_exception& e) {
            continue;
        }
    }
    d.max = max;
    d.min = min;
}
