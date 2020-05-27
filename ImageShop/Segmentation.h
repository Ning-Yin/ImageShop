#pragma once
#include "ui_Segmentation.h"
#include <opencv2/core.hpp>

class ImageShop;

class Segmentation : public QDialog
{
    Q_OBJECT

public:
    Segmentation(QImage src, ImageShop *parent = Q_NULLPTR);
public slots:
    void onSetThreshold();
    void onSegment();
    void onOtsu();
    void onTriangle();
signals:
    void sendImage(QImage);
private:
    Ui::Segmentation ui;
    cv::Mat source;
};

