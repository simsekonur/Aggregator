#pragma once

#include <Forest/AggForest.hpp>
#include <QDebug>
#include <QGraphicsItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <ui_mainwindow.h>
#include <QCheckBox>
#include <QMetaObject>
#include <ConnDialog.h>
#include <QMutex>
#include <QWaitCondition>



class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

 public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(){};

 private slots:
    void on_btnAggregate_clicked();
    void on_btnAddFormulaNode_clicked();
    void on_btnAddDataNode_clicked();
    void on_btnRemoveNode_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *node);
    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_table_cellChanged(int row, int column);
    void critChanged();
    void normChanged();
    void weightChanged();

    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

    void on_weightTable_cellChanged(int row, int column);

    void on_comboBox_currentIndexChanged(int index);

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    AggForest *forest;
    UINode *root;
    UINode *mainRoot;
    DBHandler handler;

    void fillWeightTable(UINode* node);
    void clearWeightTable();
    void clearWindow();
    void errorMessage(const std::string &str);
    void successMessage(const std::string& msg);
};
