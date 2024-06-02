/********************************************************************************
** Form generated from reading UI file 'liked.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIKED_H
#define UI_LIKED_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Liked
{
public:
    QTableWidget *likedTracks;
    QPushButton *backLiked;

    void setupUi(QWidget *Liked)
    {
        if (Liked->objectName().isEmpty())
            Liked->setObjectName("Liked");
        Liked->resize(783, 480);
        likedTracks = new QTableWidget(Liked);
        likedTracks->setObjectName("likedTracks");
        likedTracks->setGeometry(QRect(320, 140, 351, 301));
        backLiked = new QPushButton(Liked);
        backLiked->setObjectName("backLiked");
        backLiked->setGeometry(QRect(190, 330, 100, 32));

        retranslateUi(Liked);

        QMetaObject::connectSlotsByName(Liked);
    } // setupUi

    void retranslateUi(QWidget *Liked)
    {
        Liked->setWindowTitle(QCoreApplication::translate("Liked", "Form", nullptr));
        backLiked->setText(QCoreApplication::translate("Liked", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Liked: public Ui_Liked {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIKED_H
