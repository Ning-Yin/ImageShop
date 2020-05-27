#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ImageShop.h"

class ImageShop : public QMainWindow
{
    Q_OBJECT

public:
    ImageShop(QWidget *parent = Q_NULLPTR);
public slots:
    void onOpenFileAction();
    void onSaveFileAction();
    void onCloseFileAction();
    void onFormattingAction();
    void onSmoothingAction();
    void onSharpeningAction();
    void onSegmentationAction();
    void onEdgeDetectionAction();
    void onFeatureExtractionAction();
    void onAboutAction();
    void onCopyFromSource();
    void onCopyToSource();
    void onReceiveTarget(QImage image);
private:
    void updateSourceStatus();
    void updateTargetStatus();
private:
    Ui::ImageShop ui;
    QLabel *source_status, *target_status;
    QImage source_image, target_image;

};
