#pragma once
#include "ui_Smoothing.h"
#include <opencv2/core.hpp>

class ImageShop;

class Smoothing : public QDialog
{
    Q_OBJECT

public:
    Smoothing(QImage src, ImageShop *parent = Q_NULLPTR);
private:
    double sigma();
    double sigmaC();
    double sigmaS();
public slots:
    void onTuneUi();
    void onSetR();
    void onSetSigma();
    void onSetSigmaC();
    void onSetSigmaS();
    void onSmooth();
    void onHelp();
signals:
    void sendImage(QImage);
private:
    Ui::Smoothing ui;
    cv::Mat source;
};

