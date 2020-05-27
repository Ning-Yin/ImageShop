#include "ImageShop.h"
#include <QtWidgets/qfiledialog.h>
#include "Help.h"
#include "Formatting.h"
#include "Smoothing.h"
#include "Sharpening.h"
#include "Segmentation.h"
#include "EdgeDetection.h"
#include "FeatureExtraction.h"

ImageShop::ImageShop(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QPalette default_background;
    default_background.setBrush(QPalette::Background, QBrush(QPixmap(":/ImageShop/Resources/transparent.bmp")));
    ui.label->setPalette(default_background);
    ui.label_3->setPalette(default_background);
    source_status = new QLabel(ui.statusBar);
    source_status->setAlignment(Qt::AlignLeft);
    source_status->setFont(QFont("Consolas"));
    ui.statusBar->addPermanentWidget(source_status);
    updateSourceStatus();
    target_status = new QLabel(ui.statusBar);
    target_status->setAlignment(Qt::AlignLeft);
    target_status->setFont(QFont("Consolas"));
    ui.statusBar->addPermanentWidget(target_status);
    updateTargetStatus();
    setWindowIcon(QIcon(":/ImageShop/Resources/icon.ico"));
    connect(ui.actionOpen, &QAction::triggered, this, &ImageShop::onOpenFileAction);
    connect(ui.actionSave, &QAction::triggered, this, &ImageShop::onSaveFileAction);
    connect(ui.actionClose, &QAction::triggered, this, &ImageShop::onCloseFileAction);
    connect(ui.actionExit, &QAction::triggered, this, &ImageShop::close);
    connect(ui.actionFormatting, &QAction::triggered, this, &ImageShop::onFormattingAction);
    connect(ui.actionSmoothing, &QAction::triggered, this, &ImageShop::onSmoothingAction);
    connect(ui.actionSharpening, &QAction::triggered, this, &ImageShop::onSharpeningAction);
    connect(ui.actionSegmentation, &QAction::triggered, this, &ImageShop::onSegmentationAction);
    connect(ui.actionEdgeDetection, &QAction::triggered, this, &ImageShop::onEdgeDetectionAction);
    connect(ui.actionFeatureExtraction, &QAction::triggered, this, &ImageShop::onFeatureExtractionAction);
    connect(ui.actionAbout, &QAction::triggered, this, &ImageShop::onAboutAction);
    connect(ui.pbCopyFromSource, &QPushButton::clicked, this, &ImageShop::onCopyFromSource);
    connect(ui.pbCopyToSource, &QPushButton::clicked, this, &ImageShop::onCopyToSource);

    showMaximized();
}

void ImageShop::onOpenFileAction() {
    QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开文件"),
        ".", "Image(*.bmp *.png *.tif *.tiff *.jpg)");
    if (path.isEmpty()) {
        ui.statusBar->showMessage(QString::fromLocal8Bit("警告：路径为空！"), 3000);
        return;
    }
    source_image = QImage(path);
    ui.label->setPixmap(QPixmap::fromImage(source_image));
    ui.statusBar->showMessage(QString::fromLocal8Bit("成功打开原图"), 3000);
    updateSourceStatus();
}

void ImageShop::onSaveFileAction() {
    if (source_image.isNull()) {
        ui.statusBar->showMessage(QString::fromLocal8Bit("警告：原图为空！"), 3000);
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存图像"),
        ".", "Image(*.bmp *.png *.tif *.tiff *.jpg)");
    if (path.isEmpty()) {
        ui.statusBar->showMessage(QString::fromLocal8Bit("警告：路径为空！"), 3000);
        return;
    }
    source_image.save(path);
    ui.statusBar->showMessage(QString::fromLocal8Bit("成功保存原图"), 3000);
}

void ImageShop::onCloseFileAction() {
    source_image = QImage();
    ui.label->setPixmap(QPixmap::fromImage(source_image));
    ui.statusBar->showMessage(QString::fromLocal8Bit("成功关闭原图"), 3000);
    updateSourceStatus();
}

void ImageShop::onFormattingAction() {
    Formatting *formatting = new Formatting(source_image, this);
    formatting->exec();
}

void ImageShop::onSmoothingAction() {
    Smoothing *smoothing = new Smoothing(source_image, this);
    smoothing->exec();
}

void ImageShop::onSharpeningAction() {
    Sharpening *sharpening = new Sharpening(source_image, this);
    sharpening->exec();
}

void ImageShop::onSegmentationAction() {
    Segmentation *segmentation = new Segmentation(source_image, this);
    segmentation->exec();
}

void ImageShop::onEdgeDetectionAction() {
    EdgeDetection *edge_detection = new EdgeDetection(source_image, this);
    edge_detection->exec();
}

void ImageShop::onFeatureExtractionAction() {
    FeatureExtraction *feature_extraction = new FeatureExtraction(source_image, this);
    feature_extraction->exec();
}

void ImageShop::onAboutAction() {
    About *about = new About(this);
    about->exec();
}

void ImageShop::onCopyFromSource() {
    target_image = source_image.copy();
    ui.label_3->setPixmap(QPixmap::fromImage(target_image));
    updateTargetStatus();
}

void ImageShop::onCopyToSource() {
    source_image = target_image.copy();
    ui.label->setPixmap(QPixmap::fromImage(source_image));
    updateSourceStatus();
}

void ImageShop::onReceiveTarget(QImage image) {
    target_image = image.copy();
    ui.label_3->setPixmap(QPixmap::fromImage(target_image));
    updateTargetStatus();
}

void ImageShop::updateSourceStatus() {
    if (source_image.isNull()) {
        source_status->setText(QString("S:Null").leftJustified(25));
        return;
    }
    QString format;
    switch (source_image.format())
    {
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
        format = "Binary";
        break;
    case QImage::Format_Grayscale8:
        format = "Gray";
        break;
    case QImage::Format_RGB32:
        format = "RGB";
        break;
    case QImage::Format_ARGB32:
        format = "ARGB";
        break;
    default:
        format = "Unknown";
        break;
    }
    source_status->setText(QString("S:{f:%1,w:%2,h:%3}")
        .arg(format).arg(source_image.width()).arg(source_image.height())
        .leftJustified(25));
}

void ImageShop::updateTargetStatus() {
    if (target_image.isNull()) {
        target_status->setText(QString("T:Null").leftJustified(25));
        return;
    }
    QString format;
    switch (target_image.format())
    {
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
        format = "Binary";
        break;
    case QImage::Format_Grayscale8:
        format = "Gray";
        break;
    case QImage::Format_RGB32:
        format = "RGB";
        break;
    case QImage::Format_ARGB32:
        format = "ARGB";
        break;
    default:
        format = "Unknown";
        break;
    }
    target_status->setText(QString("T:{f:%1,w:%2,h:%3}")
        .arg(format).arg(target_image.width()).arg(target_image.height())
        .leftJustified(25));
}
