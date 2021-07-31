#include "UINode.hpp"
#include <iostream>
#include <QBrush>

std::string UINode::getName() const {
    return name;
}
std::string UINode::getDatabaseTable() const {
    return tableName;
}
std::string UINode::getDatabaseField() const {
    return fieldName;
}
std::string UINode::getVariableName() const {
    return variableName;
}
std::string UINode::getFormulaString() const {
    return formulaString;
}
std::string UINode::getResult() const {
    return result;
}

void UINode::setName(const std::string& s) {
    name = s;
    setText(0, QString(s.c_str()));
}

void UINode::setDatabaseTable(const std::string& s) {
    tableName = s;
    setText(3, QString((s + "." + fieldName).c_str()));
}

void UINode::setDatabaseField(const std::string& s) {
    fieldName = s;
    setText(3, QString((tableName + "." + s).c_str()));
}

void UINode::setVariableName(const std::string& s) {
    variableName = s;
    setText(1, QString(s.c_str()));
}

void UINode::setFormulaString(const std::string& s) {
    formulaString = s;
    setText(2, QString(s.c_str()));
}

void UINode::setResult(const std::string& s) {
    result = s;
    setText(4, QString(s.c_str()));
}

void UINode::updateUITexts() {
    auto toQT = [] (std::string s) {return QString(s.c_str()); };

    if (missingData) {
        QBrush b(Qt::red);
        result = "MISSING DATA";
        setText(4, toQT(result));
        for (int i = 0; i < 5; i++)
            setForeground(i, b);
    }

    setText(0, toQT(name));
    setText(1, toQT(variableName));
    if (_isLeaf) {
        setIcon(0, QIcon(":/icons/leaf.png"));
        setText(3, toQT(tableName + "." + fieldName));
    }
    else {
        setIcon(0, QIcon(":/icons/branch.png"));
        setText(2, toQT(formulaString));
        setText(4, toQT(result));
    }
    setExpanded(true);
}

UINode::UINode(bool _isLeaf, const std::string& name, const std::string& variableName) {
    this->_isLeaf = _isLeaf;
    this->name = name;
    this->variableName = variableName;

    dConfig.max = 0;
    dConfig.min = 0;
    dConfig.higherIsBetter = true;
    dConfig.isAuto = true;
    fConfig.alpha = 0;
    fConfig.isAutoWeights = true;
    missingData = false;
}

UINode::UINode(bool _isLeaf, const std::string& name, const std::string& variableName, const std::string& tableName, const std::string& fieldName)
: UINode(_isLeaf, name, variableName) {
    this->tableName = tableName;
    this->fieldName = fieldName;
}

UINode::UINode(bool _isLeaf, const std::string& name, const std::string& variableName, const std::string formulaStr)
: UINode(_isLeaf, name, variableName) {
    this->formulaString = formulaStr;
}

UINode::UINode(bool _isLeaf, const std::string& name, const std::string& variableName, QTreeWidget* widget): QTreeWidgetItem(widget) {
    this->_isLeaf = _isLeaf;
    this->name = name;
    this->variableName = variableName;

    dConfig.max = 0;
    dConfig.min = 0;
    dConfig.higherIsBetter = true;
    dConfig.isAuto = true;
    fConfig.alpha = 0;
    fConfig.isAutoWeights = true;
    missingData = false;
}

UINode::UINode(QTreeWidget* widget, bool _isLeaf, const std::string& name, const std::string& variableName, const std::string& tableName, const std::string& fieldName)
: UINode(_isLeaf, name, variableName, widget) {
    this->tableName = tableName;
    this->fieldName = fieldName;
}

UINode::UINode(QTreeWidget* widget, bool _isLeaf, const std::string& name, const std::string& variableName, const std::string formulaStr)
: UINode(_isLeaf, name, variableName, widget) {
    this->formulaString = formulaStr;
}

const std::vector<UINode*>* UINode::getChildren() const {
    return &children;
}

void UINode::addChild(UINode* child) {
    QTreeWidgetItem::addChild(child);
    children.push_back(child);
}

void UINode::removeChild(UINode* child) {
    QTreeWidgetItem::removeChild(child);
    int i;
    for (i = 0; i < children.size(); i++) {
        if (children[i] == child)
            break;
    }
    children.erase(children.begin() + i);
    std::string varName = child->getVariableName();

    if (fConfig.autoWeights.find(varName) != fConfig.autoWeights.end()) {
        fConfig.autoWeights.erase(fConfig.autoWeights.find(varName));
    }
    if (fConfig.manuelWeights.find(varName) != fConfig.manuelWeights.end()) {
        fConfig.manuelWeights.erase(fConfig.manuelWeights.find(varName));
    }
}

bool UINode::isLeaf() const {
    return _isLeaf;
}

UINode::~UINode() {
    for (auto child: children) {
        delete child;
    }
}

void UINode::printConfigs() {
    std::cout << "dconfig.max = " << dConfig.max << std::endl;
    std::cout << "dconfig.min = " << dConfig.min << std::endl;
    std::cout << "dconfig.isAuto = " << dConfig.isAuto << std::endl;
    std::cout << "dconfig.higherIsBetter = " << dConfig.higherIsBetter << std::endl;

    std::cout << "fconfig.alpha" << fConfig.alpha << std::endl;

}

tinyxml2::XMLElement* UINode::createXMLElement(tinyxml2::XMLDocument& doc, UINode* node) {
    using namespace tinyxml2;
    XMLElement* element = doc.NewElement("Node");
    XMLElement* config = element->InsertNewChildElement("Config");
    element->SetAttribute("name", node->getName().c_str());
    element->SetAttribute("variableName", node->getVariableName().c_str());

    if (node->isLeaf()) {
        element->SetAttribute("type", "data");
        element->SetAttribute("tableName", node->getDatabaseTable().c_str());
        element->SetAttribute("fieldName", node->getDatabaseField().c_str());
        config->SetAttribute("higherIsBetter", node->dConfig.higherIsBetter ? "true" : "false");
        config->SetAttribute("isAuto", node->dConfig.isAuto ? "true" : "false");
        if (!node->dConfig.isAuto) {
            config->SetAttribute("max", std::to_string(node->dConfig.max).c_str());
            config->SetAttribute("min", std::to_string(node->dConfig.min).c_str());
        }
    }
    else {
        element->SetAttribute("type", "formula");
        element->SetAttribute("formula", node->getFormulaString().c_str());
        element->SetAttribute("result", node->getResult().c_str());

        config->SetAttribute("isAutoWeights", node->fConfig.isAutoWeights ? "true" : "false");
        if (!node->fConfig.autoWeights.empty()) {
            XMLElement* autoWeights = config->InsertNewChildElement("AutoWeights");
            for (auto& pair: node->fConfig.autoWeights) {
                XMLElement* weight = autoWeights->InsertNewChildElement("Weight");
                weight->SetAttribute("name", pair.first.c_str());
                weight->SetAttribute("value", std::to_string(pair.second).c_str());
            }
        }
        if ( !node->fConfig.isAutoWeights && !node->fConfig.manuelWeights.empty()) {
            XMLElement* manuelWeights = config->InsertNewChildElement("ManuelWeights");
            manuelWeights->SetAttribute("alpha", std::to_string(node->fConfig.alpha).c_str());
            for (auto& pair: node->fConfig.manuelWeights) {
                XMLElement* weight = manuelWeights->InsertNewChildElement("Weight");
                weight->SetAttribute("name", pair.first.c_str());
                weight->SetAttribute("value", std::to_string(pair.second).c_str());
            }
        }
        XMLElement* childrenElement = element->InsertNewChildElement("Children");
        const std::vector<UINode*>& children = *node->getChildren();
        for (const auto child: children) {
            XMLElement* childElem = createXMLElement(doc, child);
            childrenElement->InsertEndChild(childElem);
        }
    }

    return element;
}

void UINode::saveAsXML(const std::string& filename) {
    using namespace tinyxml2;
    XMLDocument doc;
    XMLElement* root = createXMLElement(doc, this);
    doc.InsertFirstChild(root);
    FILE* f = fopen(filename.c_str(), "w");
    if (f)
        doc.SaveFile(f);
    else
        throw node_exception("Error while saving tree as an XML.");
    fclose(f);
}

UINode* UINode::createFromElement(tinyxml2::XMLElement* element, QTreeWidget* widget) {
    using namespace tinyxml2;
    std::string name = element->Attribute("name");
    std::string variableName = element->Attribute("variableName");
    std::cout << name << std::endl;

    if (element->Attribute("type", "data")) {
        XMLElement* config = element->FirstChildElement("Config");
        std::string table = element->Attribute("tableName");
        std::string field = element->Attribute("fieldName");
        std::string isAuto = config->Attribute("isAuto");
        std::string higherIsBetter = config->Attribute("higherIsBetter");
        UINode* dataNode = new UINode (
            true,
            name,
            variableName,
            table,
            field
        );
        dataNode->dConfig.higherIsBetter = higherIsBetter == "true" ? true : false;
        dataNode->dConfig.isAuto = isAuto == "true" ? true : false;
        if (!dataNode->dConfig.isAuto) {
            std::string max = config->Attribute("max");
            std::string min = config->Attribute("min");
            dataNode->dConfig.max = std::stod(max);
            dataNode->dConfig.min = std::stod(min);
        }
        dataNode->updateUITexts();
        return dataNode;
    }
    else {
        UINode* formulaNode;
        std::string formula = element->Attribute("formula");
        std::string result = element->Attribute("result");
        if (widget)
            formulaNode = new UINode(widget, false, name, variableName, formula);
        else
            formulaNode = new UINode(false, name, variableName, formula);
        formulaNode->setResult(result);
        formulaNode->updateUITexts();

        XMLElement* config = element->FirstChildElement("Config");
        std::string isAutoWeights = config->Attribute("isAutoWeights");
        formulaNode->fConfig.isAutoWeights = isAutoWeights == "true" ? true : false;
        XMLElement* autoWeights = config->FirstChildElement("AutoWeights");
        
        if (autoWeights) {
            for (XMLElement* weight = autoWeights->FirstChildElement(); weight; weight = weight->NextSiblingElement()) {
                std::string weightName = weight->Attribute("name");
                std::string value = weight->Attribute("value");
                formulaNode->fConfig.autoWeights.emplace(weightName, std::stod(value));                  
            }
        }
        
        if (!formulaNode->fConfig.isAutoWeights) {
            XMLElement* manuelWeights = config->FirstChildElement("ManuelWeights");
            formulaNode->fConfig.alpha = std::stod(manuelWeights->Attribute("alpha"));
            for (XMLElement* weight = manuelWeights->FirstChildElement(); weight; weight = weight->NextSiblingElement()) {
                std::string weightName = weight->Attribute("name");
                std::string value = weight->Attribute("value");
                formulaNode->fConfig.manuelWeights.emplace(weightName, std::stod(value));                  
            }
        }
        
        XMLElement* childrenElem = element->FirstChildElement("Children");
        for (XMLElement* childElem = childrenElem->FirstChildElement("Node");
            childElem != NULL;
            childElem = childElem->NextSiblingElement()
        ) {
            UINode* childNode = createFromElement(childElem, nullptr);
            formulaNode->addChild(childNode);
        }

        return formulaNode;
    }
}

UINode* UINode::loadFromXML(const std::string& filename, QTreeWidget* widget) {
    using namespace tinyxml2;
    XMLDocument doc;
    XMLError status = doc.LoadFile(filename.c_str());
    if (status == XML_SUCCESS) {
        XMLElement* root = doc.FirstChildElement();
        UINode* rootNode = createFromElement(root, widget);
        return rootNode;
    }
    else {
        throw node_exception("Error while creating tree from loaded XML file:" + filename);
    }
}
