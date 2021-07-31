#include "ConnDialog.h"

#include <QSqlDatabase>

#include "ui_ConnDialog.h"

ConnDialog::ConnDialog(QWidget *parent): QDialog(parent) {
ui.setupUi(this);

    QStringList drivers = QSqlDatabase::drivers();

    ui.comboDriver->insertItem(0, "MYSQL");
    ui.comboDriver->setCurrentText("MYSQL");
}

ConnDialog::~ConnDialog() {
}

QString ConnDialog::driverName() const {
    return ui.comboDriver->currentText();
}

QString ConnDialog::databaseName() const {
    return ui.editDatabase->text();
}

QString ConnDialog::userName() const {
    return ui.editUsername->text();
}

QString ConnDialog::password() const {
    return ui.editPassword->text();
}

QString ConnDialog::hostName() const {
    return ui.editHostname->text();
}

int ConnDialog::port() const {
    return ui.portSpinBox->value();
}

bool ConnDialog::useInMemoryDatabase() const {
    return ui.dbCheckBox->isChecked();
}

void ConnDialog::on_okButton_clicked() {
    if (ui.comboDriver->currentText().isEmpty()) {
        QMessageBox::information(this, tr("No database driver selected"),
                                 tr("Please select a database driver"));
        ui.comboDriver->setFocus();
    }
    else {
        accept();
    }
}
