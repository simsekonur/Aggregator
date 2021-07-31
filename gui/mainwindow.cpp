#include "mainwindow.h"

#include <Forest/AggExceptions.hpp>
#include <Forest/AggForest.hpp>
#include <QDebug>
#include <QException>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>
#include <iostream>
#include <sstream>
#include <string>

QMutex mutex;
QWaitCondition levelDone;
QWaitCondition newLevel;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setupUi(this);
    root = new UINode(false, "Root", "", treeWidget);
    root->setText(0, "Root");
    root->setIcon(0, QIcon(":/icons/root.png"));
    root->setText(11, "r");
    root->setExpanded(true);

    btnAddFormulaNode->setEnabled(0);
    btnAddDataNode->setEnabled(0);
    btnRemoveNode->setEnabled(0);
    connect(actionExit, &QAction::triggered, qApp, &QCoreApplication::quit);

    // Statusbar
    QLabel* conlabel = new QLabel("", status);
    conlabel->setMargin(5);
    conlabel->setPixmap(QPixmap(":/icons/dfail.svg").scaled(QSize(30, status->height()), Qt::KeepAspectRatio));
    conlabel->setToolTip(tr("No database found"));
    conlabel->setObjectName("condq");
    status->addWidget(conlabel);

    // Table
    table->blockSignals(true);
    QComboBox* crit = new QComboBox(this);
    crit->insertItem(0, "Low is better");
    crit->insertItem(1, "High is better");
    crit->setObjectName("crit");
    connect(crit, SIGNAL(currentTextChanged(QString)), this, SLOT(critChanged()));
    table->setCellWidget(5, 1, crit);

    QCheckBox* norm = new QCheckBox(this);
    norm->setText("Auto");
    norm->setObjectName("norm");
    norm->setChecked(1);
    connect(norm, SIGNAL(stateChanged(int)), this, SLOT(normChanged()));
    table->setCellWidget(6, 1, norm);
    table->blockSignals(false);

    QCheckBox* weight = new QCheckBox(this);
    weight->setText("Auto");
    weight->setObjectName("weight");
    weight->setChecked(1);
    connect(weight, SIGNAL(stateChanged(int)), this, SLOT(weightChanged()));
    table->setCellWidget(9, 1, weight);
    table->blockSignals(false);

    weightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    weightTable->hide();

    handler.connectDatabase("localhost", "aggregator", "admin", "myadminpassword");
}

/* -------------------------------------------------------------------------- */
/*                                Push Buttons                                */
/* -------------------------------------------------------------------------- */

void MainWindow::on_btnAggregate_clicked() {
    try {
        forest = new AggForest(root, &handler);
        forest->evaluate();
        QStringList list;
        for (int i = 0; i < forest->getTreeNumber(); i++) {
            std::string result = std::to_string(forest->getResult(i));
            list.append(QString( (std::to_string(i + 1) + "                  (" + result + ")").c_str()));
        }

        comboBox->addItems(list);
        forest->printResult();
        status->showMessage("Aggregation complete");
        mainRoot = root;
        successMessage("Evaluation is complete.");
        //QModelIndex index = treeWidget->currentIndex();
        //UINode* curSelect = (UINode*)index.internalPointer();
        //curSelect->printConfigs();
    }
    catch (database_exception& e) {
        errorMessage(e.what());
    }
    catch (parser_exception& e) {
        errorMessage(e.what());
    }
    catch (node_exception& e) {
        errorMessage(e.what());
    }
    catch (tree_exception& e) {
        errorMessage(e.what());
    }
    catch (std::exception& e) {
        errorMessage(e.what());
    }
    catch (...) {
        errorMessage("Unknown error while evaluating forest.");
    }
}

void MainWindow::on_btnAddFormulaNode_clicked() {
    QModelIndex index = treeWidget->currentIndex();
    UINode* cur = (UINode*)index.internalPointer();
    UINode* newNode = new UINode(false, "Formula Node", "", "");
    cur->addChild(newNode);
    newNode->setText(0, "Formula Node");
    newNode->setIcon(0, QIcon(":/icons/branch.png"));
    newNode->setExpanded(true);
    treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void MainWindow::on_btnAddDataNode_clicked() {
    QModelIndex index = treeWidget->currentIndex();
    UINode* cur = (UINode*)index.internalPointer();
    UINode* newNode = new UINode(true, "Data Node", "", "", "");
    cur->addChild(newNode);
    newNode->setText(0, "Data Node");
    newNode->setIcon(0, QIcon(":/icons/leaf.png"));
}

void MainWindow::on_btnRemoveNode_clicked() {
    QModelIndex index = treeWidget->currentIndex();
    UINode* cur = (UINode*)index.internalPointer();
    UINode* parentNode = (UINode*) cur->parent();
    parentNode->removeChild(cur);
}

/* -------------------------------------------------------------------------- */
/*                           Interface Interactions                           */
/* -------------------------------------------------------------------------- */
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* node) {

    table->blockSignals(true);

    if (node->text(11) == "r")
        btnRemoveNode->setEnabled(0);
    else
        btnRemoveNode->setEnabled(1);
    btnAddFormulaNode->setEnabled(1);

    QModelIndex index = treeWidget->currentIndex();
    UINode* cur = (UINode*)index.internalPointer();
    cur->printConfigs();
    std::cout << std::endl << cur->getName() << " : " << cur->getResult() << std::endl;

    auto toQt = [] (const std::string& str) {return QString(str.c_str());};

    for (int i = 0; i < 11; i++) table->showRow(i);

    // defaults
    // name, variable
    table->item(1, 1)->setText(toQt(cur->getName()));
    table->item(2, 1)->setText(toQt(cur->getVariableName()));

    // if data node
    // max, min, isAuto, highIsBetter
    if (cur->isLeaf()) {
        icon->setMargin(5);
        icon->setPixmap(QPixmap(":/icons/leaf.png").scaled(QSize(40, 40), Qt::KeepAspectRatio));

        table->item(4, 1)->setText(toQt(cur->getDatabaseTable() + "." + cur->getDatabaseField())); // set source

        QComboBox* critBox = (QComboBox*) table->cellWidget(5, 1);
        bool highBetter = cur->dConfig.higherIsBetter;
        critBox->setCurrentIndex(highBetter ? 1 : 0);

        QCheckBox* checkBox = (QCheckBox*) table->cellWidget(6, 1);
        bool isAuto = cur->dConfig.isAuto;
        checkBox->setChecked(isAuto);

        QString max = QString(std::to_string(cur->dConfig.max).c_str());
        QString min = QString(std::to_string(cur->dConfig.min).c_str());
        table->item(7, 1)->setText(min);
        table->item(8, 1)->setText(max);

        if (isAuto) {
            table->hideRow(7); // hide max
            table->hideRow(8); // hide min
        }
        else {
            table->showRow(7);
            table->showRow(8);
        }

        table->hideRow(3); // hide formula
        table->hideRow(9); // hide weight check
        table->hideRow(10); // hide alpha
        weightTable->hide();

        btnAddFormulaNode->setEnabled(0);
        btnAddDataNode->setEnabled(0);
    }
    else { // if formula node
        // formula, alpha, formulanode.weightMap -> variable sayisi kadar row degerleriyle birlikte
        icon->setMargin(5);
        icon->setPixmap(QPixmap(":/icons/branch.png").scaled(QSize(40, 40), Qt::KeepAspectRatio));

        table->item(3, 1)->setText(toQt(cur->getFormulaString().c_str())); // set formula

        std::string alphaStr = std::to_string(cur->fConfig.alpha);

        QTableWidgetItem *item = table->item(10, 1);
        if(!item) {
            item = new QTableWidgetItem();
            item->setText(toQt(alphaStr));
            table->setItem(10, 1, item);
        }
        else {
            item->setText(toQt(alphaStr));
        }

        QCheckBox* weightCheckBox = (QCheckBox*) table->cellWidget(9, 1);
        bool isAutoWeights = cur->fConfig.isAutoWeights;
        weightCheckBox->setChecked(isAutoWeights);

        if (isAutoWeights) {
            weightTable->hide();
            table->hideRow(10); // hide alpha
            clearWeightTable();
        }
        else {
            // alpha
            table->showRow(10);
            clearWeightTable();
            fillWeightTable(cur);
            // add map rows
        }

        table->hideRow(4);
        table->hideRow(5);
        table->hideRow(6);
        table->hideRow(7);
        table->hideRow(8);

        btnAddFormulaNode->setEnabled(1);
        btnAddDataNode->setEnabled(1);
    }
    table->blockSignals(false);
}

void MainWindow::fillWeightTable(UINode* node) {
    weightTable->show();
    weightTable->blockSignals(true);

    auto toQt = [] (const std::string& str) {return QString(str.c_str());};

    clearWeightTable();

    const std::vector<UINode*>& children = *node->getChildren();
    for (const auto& child: children) {
        if (!child->isLeaf()) {
            std::string varName = child->getVariableName();
            std::string cellName;
            if (varName != "")
                cellName = "w_" + child->getVariableName();
            else
                continue;

            auto iterator = node->fConfig.manuelWeights.find(varName);
            double weight;
            if (iterator == node->fConfig.manuelWeights.end()) {
                weight = 0;
                node->fConfig.manuelWeights.emplace(varName, 0);
            }
            else {
                weight = node->fConfig.manuelWeights[varName];
            }

            QTableWidgetItem* nameItem = new QTableWidgetItem();
            nameItem->setText(toQt(cellName));
            QTableWidgetItem* manuelItem = new QTableWidgetItem();
            manuelItem->setText(toQt(std::to_string(weight)));

            QTableWidgetItem* autoItem = nullptr;
            if ((iterator = node->fConfig.autoWeights.find(varName)) != node->fConfig.autoWeights.end()) {
                autoItem = new QTableWidgetItem();
                autoItem->setText(toQt(std::to_string(node->fConfig.autoWeights[varName])));
            }

            weightTable->insertRow(weightTable->rowCount());
            int index = weightTable->rowCount() - 1;
            weightTable->setItem(index, 0, nameItem);
            weightTable->setItem(index, 1, manuelItem);
            std::cout << "added weight table items for: " << node->getName() << std::endl;
            if (autoItem)
                weightTable->setItem(index, 2, autoItem);
        }
    }

    weightTable->blockSignals(false);
}

void MainWindow::on_weightTable_cellChanged(int row, int column) {
    if (column == 1) {
        QModelIndex index = treeWidget->currentIndex();
        UINode* curSelect = (UINode*)index.internalPointer();
        std::string cellName = weightTable->item(row, 0)->text().toStdString();
        std::string value = weightTable->item(row, 1)->text().toStdString();

        curSelect->fConfig.manuelWeights[cellName.substr(2)] = std::stod(value);
    }
}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex& index) {
    QTreeWidgetItem* cur = (QTreeWidgetItem*)index.internalPointer();
    //cur->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
}

void MainWindow::on_table_cellChanged(int row, int column) {
    std::string property = table->item(row, 0)->text().toStdString();
    std::string value = table->item(row, 1)->text().toStdString();

    QModelIndex index = treeWidget->currentIndex();
    UINode* curSelect = (UINode*)index.internalPointer();

    double numValue;
    try {
        numValue = std::stod(value);
    }
    catch(...) {
        numValue = 0;
    }

    if (property == "Name") {
        curSelect->setName(value);
        return;
    }
    else if (property == "Variable") {
        curSelect->setVariableName(value);
        return;
    }
    else if (property == "Formula") {
        curSelect->setFormulaString(value);
        return;
    }
    else if (property == "Source") {
        int dotIndex = value.find_first_of(".");
        curSelect->setDatabaseTable(value.substr(0, dotIndex));
        curSelect->setDatabaseField(value.substr(dotIndex+1));
        return;
    }

    if (curSelect->isLeaf()) {
        DataConfig& config = curSelect->dConfig;
        if (property == "Min") {
            curSelect->dConfig.min = numValue;
        }
        else if (property == "Max") {
            curSelect->dConfig.max = numValue;
        }
    }
    else {
        if (property == "Alpha") {
            curSelect->fConfig.alpha = numValue;
        }
        else {
            ; // get variable weight
        }
    }
}

void MainWindow::critChanged() {
    QComboBox* crit = (QComboBox*)table->cellWidget(5, 1);

    QModelIndex index = treeWidget->currentIndex();
    UINode* curSelect = (UINode*)index.internalPointer();
    curSelect->dConfig.higherIsBetter = crit->currentIndex() == 1 ? true : false;
}

void MainWindow::weightChanged() {
    QCheckBox* weight = (QCheckBox*)table->cellWidget(9, 1);

    QModelIndex index = treeWidget->currentIndex();
    UINode* curSelect = (UINode*)index.internalPointer();


    if (weight->isChecked()) { // auto
        curSelect->fConfig.isAutoWeights = true;
        weightTable->hide();
        table->hideRow(10);
        clearWeightTable();
    }

    else { // custom
        curSelect->fConfig.isAutoWeights = false;
        weightTable->show();
        table->showRow(10);
        std::cout << "calling fill 2" << std::endl;
        fillWeightTable(curSelect);
    }
}

void MainWindow::normChanged() {
    QTreeWidgetItem* cur = treeWidget->currentItem();
    QCheckBox* norm = (QCheckBox*)table->cellWidget(6, 1);


    QModelIndex index = treeWidget->currentIndex();
    UINode* curSelect = (UINode*)index.internalPointer();


    if (norm->checkState() == 2) { // auto
        curSelect->dConfig.isAuto = true;
        table->hideRow(7);
        table->hideRow(8);
    }

    else { // custom
        curSelect->dConfig.isAuto = false;
        table->showRow(7);
        table->showRow(8);

    }
}

/* -------------------------------------------------------------------------- */
/*                                   Actions                                  */
/* -------------------------------------------------------------------------- */

void MainWindow::on_actionConnect_triggered() {
    ConnDialog dialog(nullptr);
    dialog.exec();
    handler.connectDatabase(dialog.hostName().toStdString(), dialog.databaseName().toStdString(), dialog.userName().toStdString(), dialog.password().toStdString());

    QLabel* conlabel = (QLabel*)status->children()[1];
    conlabel->setPixmap(QPixmap(":/icons/dsuc.svg").scaled(QSize(30, status->height()), Qt::KeepAspectRatio));
    conlabel->setToolTip(tr("Connection secured."));
}

void MainWindow::on_actionDisconnect_triggered() {
    QLabel* conlabel = (QLabel*)status->children()[1];
    conlabel->setPixmap(QPixmap(":/icons/dfail.svg").scaled(QSize(30, status->height()), Qt::KeepAspectRatio));
    conlabel->setToolTip(tr("No database found"));
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(0, "Reach Us", "<a>Aggregator Team</a>");
}

void MainWindow::on_actionAbout_Qt_triggered() {
    qApp->aboutQt();
}

/* -------------------------------------------------------------------------- */
/*                              Private Functions                             */
/* -------------------------------------------------------------------------- */

void MainWindow::successMessage(const std::string& msg) {
    QMessageBox msgBox;
    msgBox.setText("Success!");
    msgBox.setInformativeText(msg.c_str());
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIconPixmap(QPixmap(":/icons/success2.png").scaled(QSize(35, 35), Qt::KeepAspectRatio));
    msgBox.setWindowIcon(QIcon(":/icons/a_icon.png"));
    msgBox.exec();
}

void MainWindow::errorMessage(const std::string& err) {
    QMessageBox msgBox;
    msgBox.setText("Error!");
    msgBox.setInformativeText(err.c_str());
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Help);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Help) {
        QMessageBox::about(0, "Reach Us", "<a>Aggregator Team</a>");
    }
}

void MainWindow::clearWeightTable() {
    weightTable->blockSignals(true);
    int rowCount = weightTable->rowCount();
    weightTable->clearContents();
    for (int i = 0; i < rowCount; i++) {
        weightTable->removeRow(i);
    }
    weightTable->blockSignals(false);
}

void MainWindow::clearWindow() {
    delete root;
    treeWidget->clear();
    clearWeightTable();
    comboBox->blockSignals(true);
    int comboSize = comboBox->count();
    std::cout << "combo count: " << comboSize << std::endl;
    for (int i = comboSize - 1; i > 0; i--)
        comboBox->removeItem(i);
    comboBox->blockSignals(false);
}

void MainWindow::on_comboBox_currentIndexChanged(int index) {
    delete root;
    root = forest->getUITree(treeWidget, index);
    treeWidget->expandAll();
}

void MainWindow::on_actionSave_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Aggregation Tree"), "",
            tr("Aggregation Tree (*.xml);;All Files (*)"));
    root->saveAsXML(fileName.toStdString());
}

void MainWindow::on_actionLoad_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load Aggregation Tree"), "",
            tr("Aggregation Tree (*.xml);;All Files (*)"));
    clearWindow();
    root = UINode::loadFromXML(fileName.toStdString(), treeWidget);
    treeWidget->expandAll();
    treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
