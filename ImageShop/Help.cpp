#include "Help.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

About::About(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle(QString::fromLocal8Bit("关于"));
    QGridLayout *gridLayout = new QGridLayout(this);
    QLabel *label = new QLabel(this);
    label->setText(QString::fromLocal8Bit(
        "作者：殷宁\n"
        "邮箱：isyinning@163.com\n"
        "学号：119106010644\n"
        "学校：南京理工大学"));
    gridLayout->addWidget(label, 0, 0, Qt::AlignCenter);
    QPushButton *pushButton = new QPushButton(QString::fromLocal8Bit("关闭"), this);
    pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridLayout->addWidget(pushButton, 1, 0, Qt::AlignCenter);
    connect(pushButton, &QPushButton::clicked, this, &About::close);
}
