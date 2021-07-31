#include <string>
#include <sstream>
#pragma once

class parser_exception: public std::exception {private:
    std::string what_msg;
public:
    explicit parser_exception(const std::string& what) {
        what_msg = "Parser error: " + what;
    }
    virtual const char* what() const throw() {
        return what_msg.c_str();
    }
};

class database_exception: public std::exception {private:
    std::string what_msg;
public:
    explicit database_exception(const std::string& what) {
        what_msg = "Database error: " + what;
    }
    virtual const char* what() const throw() {
        return what_msg.c_str();
    }
};

class node_exception: public std::exception {private:
    std::string what_msg;
public:
    explicit node_exception(const std::string& what) {
        what_msg = "Node error: " + what;
    }
    virtual const char* what() const throw() {
        return what_msg.c_str();
    }
};

class tree_exception: public std::exception {private:
    std::string what_msg;
public:
    explicit tree_exception(const std::string& what) {
        what_msg = "Tree error: " + what;
    }
    virtual const char* what() const throw() {
        return what_msg.c_str();
    }
};

class missing_value_exception: public std::exception {private:
    std::string what_msg;
public:
    explicit missing_value_exception(const std::string& what) {
        what_msg = "Tree error: " + what;
    }
    virtual const char* what() const throw() {
        return what_msg.c_str();
    }
};
