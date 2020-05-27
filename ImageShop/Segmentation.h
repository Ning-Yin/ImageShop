#pragma once
#include "ui_Segmentation.h"
#include <opencv2/core.hpp>

class ImageShop;

class Segmentation : public QDialog
{
    Q_OBJECT

public:
    Segmentation(QImage src, ImageShop *parent = Q_NULLPTR);
signals:
    void sendImage(QImage);
public slots:
    void onSetThreshold();
    void onSegment();
    void onOtsu();
    void onTriangle();
private:
    Ui::Segmentation ui;
    cv::Mat source;
};

