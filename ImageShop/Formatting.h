#pragma once
#include "ui_Formatting.h"

class ImageShop;

class Formatting : public QDialog
{
    Q_OBJECT

public:
    Formatting(QImage src, ImageShop *parent = Q_NULLPTR);
public slots:
    void onFormat();
signals:
    void sendImage(QImage);
private:
    Ui::Formatting ui;
    QImage source;
};

