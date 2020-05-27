#pragma once

#include <QtWidgets/QDialog>

class About : public QDialog
{
    Q_OBJECT

public:
    About(QWidget *parent = Q_NULLPTR);
};

class SmoothingHelp : public QDialog
{
    Q_OBJECT

public:
    SmoothingHelp(QWidget *parent = Q_NULLPTR);
};

class SegmentationHelp : public QDialog
{
    Q_OBJECT

public:
    SegmentationHelp(QWidget *parent = Q_NULLPTR);
};

