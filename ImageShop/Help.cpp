#include "Help.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

About::About(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle(QString::fromLocal8Bit("����"));
    QGridLayout *gridLayout = new QGridLayout(this);
    QLabel *label = new QLabel(this);
    label->setText(QString::fromLocal8Bit(
        "���ߣ�����\n"
        "���䣺isyinning@163.com\n"
        "ѧ�ţ�119106010644\n"
        "ѧУ���Ͼ�����ѧ"));
    gridLayout->addWidget(label, 0, 0, Qt::AlignCenter);
    QPushButton *pushButton = new QPushButton(QString::fromLocal8Bit("�ر�"), this);
    pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridLayout->addWidget(pushButton, 1, 0, Qt::AlignCenter);
    connect(pushButton, &QPushButton::clicked, this, &About::close);
}

SmoothingHelp::SmoothingHelp(QWidget *parent)
    : QDialog(parent) {
    setWindowFilePath(QString::fromLocal8Bit("����"));
    QGridLayout *gridLayout = new QGridLayout(this);
    QLabel *label = new QLabel(this);
    label->setText(QString::fromLocal8Bit(
        "��ƽ������Ϊ��ֵ�˲�����ֵ�˲������ֵ�˲���\n��Сֵ�˲�ʱ��������1���2��Ч��\n"
        "��ƽ������Ϊ��˹�˲�ʱ��������1��ʾ�Ҳ�����\n������2��Ч��\n"
        "��ƽ������Ϊ˫���˲�ʱ��������1��ʾ��ɫ���\n������������2��ʾ�ռ���Ҳ�����"));
    gridLayout->addWidget(label, 0, 0, Qt::AlignCenter);
    QPushButton *pushButton = new QPushButton(QString::fromLocal8Bit("�ر�"), this);
    pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridLayout->addWidget(pushButton, 1, 0, Qt::AlignCenter);
    connect(pushButton, &QPushButton::clicked, this, &SmoothingHelp::close);
}
