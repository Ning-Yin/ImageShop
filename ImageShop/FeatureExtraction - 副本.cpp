#include "FeatureExtraction.h"
#include "ImageShop.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

FeatureExtraction::FeatureExtraction(QImage src, ImageShop *parent)
    : QDialog(parent) {
    ui.setupUi(this);

    switch (src.format())
    {
    case QImage::Format_Grayscale8:
        source = Mat(src.height(), src.width(), CV_8UC1,
            (void *)src.constBits(), src.bytesPerLine());
        source_gray = source.clone();
        break;
    case QImage::Format_RGB32:
        source = Mat(src.height(), src.width(), CV_8UC4,
            (void *)src.constBits(), src.bytesPerLine());
        cvtColor(source, source, COLOR_BGRA2RGB); // big endian
        // cvtColor(source, source, COLOR_RGBA2RGB); // little endian
        cvtColor(source, source_gray, COLOR_BGR2GRAY);
        break;
    case QImage::Format_ARGB32:
        source = Mat(src.height(), src.width(), CV_8UC4,
            (void *)src.constBits(), src.bytesPerLine());
        cvtColor(source, source_gray, COLOR_BGRA2GRAY);
        break;
    default:
        break;
    }

    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FeatureExtraction::onTuneUi);
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FeatureExtraction::onExtractFeature);
    connect(ui.sliderBr, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onSetBr);
    connect(ui.sliderBr, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onExtractFeature);
    connect(ui.sliderKr, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onSetKr);
    connect(ui.sliderKr, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onExtractFeature);
    connect(ui.sliderK, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onSetK);
    connect(ui.sliderK , QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onExtractFeature);
    connect(ui.sliderRt, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onSetRt);
    connect(ui.sliderRt, QOverload<int>::of(&QSlider::valueChanged), this, &FeatureExtraction::onExtractFeature);
    connect(ui.pushButton, &QPushButton::clicked, this, &FeatureExtraction::accept);
    connect(this, &FeatureExtraction::sendImage, parent, &ImageShop::onReceiveTarget);

    adjustSize();
    setWindowFlags((windowFlags() | Qt::MSWindowsFixedSizeDialogHint) & ~Qt::WindowContextHelpButtonHint);
    ui.labelBr->hide();
    ui.sliderBr->hide();
    ui.valueBr->hide();
    ui.labelKr->hide();
    ui.sliderKr->hide();
    ui.valueKr->hide();
    ui.labelK->hide();
    ui.sliderK->hide();
    ui.valueK->hide();
    ui.labelRt->hide();
    ui.sliderRt->hide();
    ui.valueRt->hide();
}

double FeatureExtraction::k() {
    int slider = ui.sliderK->value();
    double ret;
    // 1~100     =>   0.001~0.100
    ret = slider / 1000.0;
    return ret;
}

void FeatureExtraction::onTuneUi() {
    switch (ui.comboBox->currentIndex())
    {
    case 0:
        // Harris
        ui.labelBr->show();
        ui.sliderBr->show();
        ui.valueBr->show();
        ui.labelKr->show();
        ui.sliderKr->show();
        ui.valueKr->show();
        ui.labelK->show();
        ui.sliderK->show();
        ui.valueK->show();
        ui.labelRt->show();
        ui.sliderRt->show();
        ui.valueRt->show();
        break;
    case 1:
        // Min Eigen Value
        ui.labelBr->show();
        ui.sliderBr->show();
        ui.valueBr->show();
        ui.labelKr->show();
        ui.sliderKr->show();
        ui.valueKr->show();
        ui.labelK->hide();
        ui.sliderK->hide();
        ui.valueK->hide();
        ui.labelRt->show();
        ui.sliderRt->show();
        ui.valueRt->show();
        break;
    default:
        break;
    }
}

void FeatureExtraction::onSetBr() {
    ui.valueBr->setText(QString::number(ui.sliderBr->value()).leftJustified(5));
}

void FeatureExtraction::onSetKr() {
    ui.valueKr->setText(QString::number(ui.sliderKr->value()).leftJustified(5));
}

void FeatureExtraction::onSetK() {
    ui.valueK->setText(QString::number(k()).leftJustified(5));
}

void FeatureExtraction::onSetRt() {
    ui.valueRt->setText(QString::number(ui.sliderRt->value()).leftJustified(5));
}

void FeatureExtraction::onExtractFeature() {
    Mat target;
    Mat response;
    int bsize, ksize;
    Ptr<FeatureDetector> detector;
    vector<KeyPoint> keypoints;
    if (source.type() == CV_8UC1) {
        cvtColor(source, target, COLOR_GRAY2BGR);
    }
    else if (source.type() == CV_8UC3) {
        target = source.clone();
    }
    else if (target.type() == CV_8UC4) {
        cvtColor(source, target, COLOR_BGRA2BGR);
    }
    switch (ui.comboBox->currentIndex())
    {
    case 0:
        // Harris
        bsize = ui.sliderBr->value() * 2 - 1;
        ksize = ui.sliderKr->value() * 2 - 1;
        cornerHarris(source_gray, response, bsize, ksize, k());
        normalize(response, response, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
        convertScaleAbs(response, response);
        for (int i = 0; i < response.rows; i++) {
            for (int j = 0; j < response.cols; j++) {
                uchar value = response.at<uchar>(i, j);
                if (value > ui.sliderRt->value()) {
                    circle(target, Point(j, i), 4, Scalar(0, 0, 255));
                }
            }
        }
        break;
    case 1:
        // Min Eigen Value
        bsize = ui.sliderBr->value() * 2 - 1;
        ksize = ui.sliderKr->value() * 2 - 1;
        cornerMinEigenVal(source_gray, response, bsize, ksize);
        normalize(response, response, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
        convertScaleAbs(response, response);
        for (int i = 0; i < response.rows; i++) {
            for (int j = 0; j < response.cols; j++) {
                uchar value = response.at<uchar>(i, j);
                if (value > ui.sliderRt->value()) {
                    circle(target, Point(j, i), 4, Scalar(0, 0, 255));
                }
            }
        }
        break;
    case 2:
        // ORB
        detector = ORB::create();
        detector->detect(source, keypoints);
        drawKeypoints(target, keypoints, target, Scalar(0, 0, 255));
        break;
    case 3:
        // FAST
        detector = FastFeatureDetector::create();
        detector->detect(source, keypoints);
        drawKeypoints(target, keypoints, target, Scalar(0, 0, 255));
        break;
    case 4:
        // BRISK
        detector = BRISK::create();
        detector->detect(source, keypoints);
        drawKeypoints(target, keypoints, target, Scalar(0, 0, 255));
        break;
    case 5:
        // MSER
        detector = MSER::create();
        detector->detect(source, keypoints);
        drawKeypoints(target, keypoints, target, Scalar(0, 0, 255));
        break;
    default:
        break;
    }
    QImage qTarget;
    const uchar *pSrc = (const uchar *)target.data;
    qTarget = QImage(pSrc, target.cols, target.rows, target.step, QImage::Format_RGB888)
        .copy();
    qTarget = qTarget.convertToFormat(QImage::Format_RGB32);
    emit sendImage(qTarget);
}