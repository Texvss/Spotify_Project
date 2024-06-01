#include "login.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include "ui_login.h"


Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->loginButton->setStyleSheet("QPushButton:!hover{border: 2px solid black;border-radius: 5px;background-color: #717072;color:white;}"
                                   "QPushButton:hover{border: 2px solid black;border-radius: 5px;background-color: #33322e;color:#c0c0c0;}");
    ui->signupButton->setStyleSheet("QPushButton:!hover{border: 2px solid black;border-radius: 5px;background-color: #717072;color:white;}"
                                    "QPushButton:hover{border: 2px solid black;border-radius: 5px;background-color: #33322e;color:#c0c0c0;}");



    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Arsentii/Desktop/draft.db");

    if (!database.open()) {
        ui->label->setText("Failed!!!");
    } else {
        ui->label->setText("Connected!");
    }
}

Login::~Login()
{
    delete ui;
}

void Login::on_loginButton_clicked()
{
    QString username, password;
    username = ui->usernameLine->text();
    password = ui->passwordLine->text();

    if (!database.isOpen()) {
        qDebug() << "Failed to open the database";
        return;
    }
    QSqlQuery qry;
    if (qry.exec("select * from users where username='" + username + "' and password='" + password + "'")) {
        int count = 0;
        while (qry.next()) {
            count++;
        }
        if (username.isEmpty() || password.isEmpty()) {
            ui->label->setText("Please! Log in or Sign up");
        }
        if (count == 1) {
            ui->label->setText("username and password is correct");
            emit loginSuccess();
        }
        if (count > 1) {
            ui->label->setText("Username already exists");
        }
        if (count < 1) {
            ui->label->setText("username and password is incorrect");
        }
    }
}

void Login::on_signupButton_clicked()
{
    QString username, password;
    username = ui->usernameLine->text();
    password = ui->passwordLine->text();

    if (!database.isOpen()) {
        qDebug() << "Failed to open the database";
        return;
    }

    QSqlQuery qry;
    qry.prepare("insert into users (username, password) values ('" + username + "', '" + password + "')");
    qry.bindValue(":username", username);
    qry.bindValue(":password", password);
    if (qry.exec()) {
        ui->label->setText("OK!");
        emit signUpSuccess();
    } else {
        ui->label->setText("Sign up failed.");
        qDebug() << "Sign up failed: " << qry.lastError().text();
    }
}

QString Login::showUsername()
{
    QString username;
    username = ui->usernameLine->text();
    return username;
}
