#pragma once
#include "ui_EdgeDetection.h"
#include <opencv2/core.hpp>

class ImageShop;

class EdgeDetection : public QDialog
{
    Q_OBJECT

public:
    EdgeDetection(QImage src, ImageShop *parent = Q_NULLPTR);
signals:
    void sendImage(QImage);
public slots:
    void onTuneUi();
    void onSetR();
    void onSetThreshold1();
    void onSetThreshold2();
    void onDetectEdge();
private:
    Ui::EdgeDetection ui;
    cv::Mat source;
};

