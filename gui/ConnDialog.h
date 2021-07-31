#pragma once

#include <QDialog>
#include <QMessageBox>

#include "ui_ConnDialog.h"

class ConnDialog : public QDialog {
    Q_OBJECT
 public:
    ConnDialog(QWidget *parent = nullptr);
    ~ConnDialog();

    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    bool useInMemoryDatabase() const;

 private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked() { reject(); }
    void on_dbCheckBox_clicked() { ui.connGroupBox->setEnabled(!ui.dbCheckBox->isChecked()); }

 private:
    Ui::ConnDialogUi ui;
};