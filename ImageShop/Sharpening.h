#pragma once
#include "ui_Sharpening.h"
#include <opencv2/core.hpp>

class ImageShop;

class Sharpening : public QDialog
{
    Q_OBJECT

public:
    Sharpening(QImage src, ImageShop *parent = Q_NULLPTR);
public slots:
    void onTuneUi();
    void onSetR();
    void onSharpen();
signals:
    void sendImage(QImage);
private:
    Ui::Sharpening ui;
    cv::Mat source;
};
