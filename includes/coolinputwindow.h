#ifndef COOLINPUTWINDOW_H
#define COOLINPUTWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>

class CoolInput : public QDialog
{
    Q_OBJECT

public:
    explicit CoolInput(QWidget *parent = nullptr);
    ~CoolInput();
    void setMinMaxTempUnit(double minT, double maxT);
    void setMinMaxHumUnit(double minH, double maxH);
    void setMinMaxPresUnit(double minP, double maxP);
signals:
    void sendInputData(double tData, double hData, double pData);
private:
    QVBoxLayout *mainLayout;

    QHBoxLayout *tempLayout;
    QLabel *tempLabel;
    QDoubleSpinBox *tempDSpinBox;

    QHBoxLayout *humLayout;
    QLabel *humLabel;
    QDoubleSpinBox *humDSpinBox;

    QHBoxLayout *presLayout;
    QLabel *presLabel;
    QDoubleSpinBox *presDSpinBox;

    QHBoxLayout *confirmLayout;
    QPushButton *confirmButton;
};

#endif