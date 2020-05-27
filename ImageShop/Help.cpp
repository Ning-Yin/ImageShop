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

SmoothingHelp::SmoothingHelp(QWidget *parent)
    : QDialog(parent) {
    setWindowFilePath(QString::fromLocal8Bit("帮助"));
    QGridLayout *gridLayout = new QGridLayout(this);
    QLabel *label = new QLabel(this);
    label->setText(QString::fromLocal8Bit(
        "当平滑方法为均值滤波、中值滤波、最大值滤波、\n最小值滤波时，参数σ1与σ2无效。\n"
        "当平滑方法为高斯滤波时，参数σ1表示σ参数，\n参数σ2无效。\n"
        "当平滑方法为双边滤波时，参数σ1表示颜色域σ\n参数，参数σ2表示空间域σ参数。"));
    gridLayout->addWidget(label, 0, 0, Qt::AlignCenter);
    QPushButton *pushButton = new QPushButton(QString::fromLocal8Bit("关闭"), this);
    pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridLayout->addWidget(pushButton, 1, 0, Qt::AlignCenter);
    connect(pushButton, &QPushButton::clicked, this, &SmoothingHelp::close);
}
