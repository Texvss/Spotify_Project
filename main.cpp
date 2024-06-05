#include <QApplication>
#include "login.h"
#include "mainwindow.h"
#include <QFile>
#include <QStandardPaths>

void copyFileToAppData(const QString &sourcePath, const QString &destPath)
{
    if (!QFile::exists(destPath)) {
        QFile::copy(sourcePath, destPath);
        QFile::setPermissions(destPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataPath);

    Login login;
    MainWindow w;

    QObject::connect(&login, &Login::loginSuccess, [&w, &login]() {
        QString username = login.showUsername();
        login.hide();
        w.show();
        w.showUsername(username);
    });

    QObject::connect(&login, &Login::signUpSuccess, [&w, &login]() {
        QString username = login.showUsername();
        login.hide();
        w.show();
        w.showUsername(username);
    });

    login.show();

    return a.exec();

}
