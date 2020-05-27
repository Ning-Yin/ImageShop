#pragma once
#include "ui_Formatting.h"

class ImageShop;

class Formatting : public QDialog
{
    Q_OBJECT

public:
    Formatting(QImage src, ImageShop *parent = Q_NULLPTR);
signals:
    void sendImage(QImage);
public slots:
    void onFormat();
private:
    Ui::Formatting ui;
    QImage source;
};

