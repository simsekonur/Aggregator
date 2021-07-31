#pragma once

#include <vector>
#include <string>
#include <QTreeWidgetItem>
#include <string>
#include "tinyxml/tinyxml2.h"
#include "AggExceptions.hpp"

struct DataConfig {
    double min;
    double max;
    bool higherIsBetter;
    bool isAuto;
};

struct FormulaConfig {
    double alpha;
    bool isAutoWeights;
    std::map<std::string, double> manuelWeights;
    std::map<std::string, double> autoWeights;
};

class UINode : public QTreeWidgetItem {
private:
    std::vector<UINode*> children;
    bool _isLeaf;
    std::string name;
    std::string variableName;
    std::string formulaString;
    std::string tableName;
    std::string fieldName;
    std::string result;

public:
    bool missingData;
    FormulaConfig fConfig;
    DataConfig dConfig;


    ~UINode();
    UINode(bool _isLeaf, const std::string& name, const std::string& variableName, QTreeWidget* widget);
    UINode(bool _isLeaf, const std::string& name, const std::string& variableName);

    UINode(bool _isLeaf, const std::string& name, const std::string& variableName, const std::string& tableName, const std::string& fieldName);
    UINode(QTreeWidget* widget, bool _isLeaf, const std::string& name, const std::string& variableName, const std::string& tableName, const std::string& fieldName);

    UINode(bool _isLeaf, const std::string& name, const std::string& variableName, const std::string formulaStr);
    UINode(QTreeWidget* widget, bool _isLeaf, const std::string& name, const std::string& variableName, const std::string formulaStr);

    bool isLeaf() const;
    std::string getName() const;
    std::string getDatabaseTable() const;
    std::string getDatabaseField() const;
    std::string getVariableName() const;
    std::string getFormulaString() const;
    std::string getResult() const;

    void setName(const std::string& s);
    void setDatabaseTable(const std::string& s);
    void setDatabaseField(const std::string& s);
    void setVariableName(const std::string& s);
    void setFormulaString(const std::string& s);
    void setResult(const std::string& s);

    const std::vector<UINode*>* getChildren() const;
    void addChild(UINode* child);
    void removeChild(UINode* child);
    void printConfigs();

    tinyxml2::XMLElement* createXMLElement(tinyxml2::XMLDocument& doc, UINode* node);
    void saveAsXML(const std::string& filename);
    static UINode* createFromElement(tinyxml2::XMLElement* element, QTreeWidget* widget);
    static UINode* loadFromXML(const std::string& filename, QTreeWidget* widget);

    void updateUITexts();
};
