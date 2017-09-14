/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelCom;
    QComboBox *comboBoxCom;
    QLabel *labelRate;
    QComboBox *comboBoxRate;
    QPushButton *pushButtonConnect;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButtonOpenFile;
    QLabel *label;
    QPushButton *pushButtonStart;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QProgressBar *progressBar;
    QProgressBar *progressBar_2;
    QLabel *label_2;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->setEnabled(true);
        Widget->resize(790, 442);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Widget->sizePolicy().hasHeightForWidth());
        Widget->setSizePolicy(sizePolicy);
        Widget->setMinimumSize(QSize(790, 442));
        Widget->setMaximumSize(QSize(790, 442));
        horizontalLayoutWidget_2 = new QWidget(Widget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 0, 310, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        labelCom = new QLabel(horizontalLayoutWidget_2);
        labelCom->setObjectName(QStringLiteral("labelCom"));

        horizontalLayout_2->addWidget(labelCom);

        comboBoxCom = new QComboBox(horizontalLayoutWidget_2);
        comboBoxCom->setObjectName(QStringLiteral("comboBoxCom"));

        horizontalLayout_2->addWidget(comboBoxCom);

        labelRate = new QLabel(horizontalLayoutWidget_2);
        labelRate->setObjectName(QStringLiteral("labelRate"));

        horizontalLayout_2->addWidget(labelRate);

        comboBoxRate = new QComboBox(horizontalLayoutWidget_2);
        comboBoxRate->setObjectName(QStringLiteral("comboBoxRate"));

        horizontalLayout_2->addWidget(comboBoxRate);

        pushButtonConnect = new QPushButton(horizontalLayoutWidget_2);
        pushButtonConnect->setObjectName(QStringLiteral("pushButtonConnect"));

        horizontalLayout_2->addWidget(pushButtonConnect);

        horizontalLayoutWidget = new QWidget(Widget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 60, 501, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(horizontalLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButtonOpenFile = new QPushButton(horizontalLayoutWidget);
        pushButtonOpenFile->setObjectName(QStringLiteral("pushButtonOpenFile"));

        horizontalLayout->addWidget(pushButtonOpenFile);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        pushButtonStart = new QPushButton(horizontalLayoutWidget);
        pushButtonStart->setObjectName(QStringLiteral("pushButtonStart"));

        horizontalLayout->addWidget(pushButtonStart);

        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 110, 771, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(verticalLayoutWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(textBrowser);

        progressBar = new QProgressBar(verticalLayoutWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        progressBar_2 = new QProgressBar(verticalLayoutWidget);
        progressBar_2->setObjectName(QStringLiteral("progressBar_2"));
        progressBar_2->setValue(0);

        verticalLayout->addWidget(progressBar_2);

        label_2 = new QLabel(Widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(540, 70, 231, 21));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        labelCom->setText(QApplication::translate("Widget", "\350\256\276\345\244\207\345\217\267:", Q_NULLPTR));
        labelRate->setText(QApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207", Q_NULLPTR));
        comboBoxRate->clear();
        comboBoxRate->insertItems(0, QStringList()
         << QApplication::translate("Widget", "115200", Q_NULLPTR)
         << QApplication::translate("Widget", "38400", Q_NULLPTR)
         << QApplication::translate("Widget", "9600", Q_NULLPTR)
        );
        pushButtonConnect->setText(QApplication::translate("Widget", "\350\277\236\346\216\245", Q_NULLPTR));
        pushButtonOpenFile->setText(QApplication::translate("Widget", "\346\265\217\350\247\210", Q_NULLPTR));
        label->setText(QString());
        pushButtonStart->setText(QApplication::translate("Widget", "\345\274\200\345\247\213", Q_NULLPTR));
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
