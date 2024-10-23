#include "../includes/coolinputwindow.h"

CoolInput::CoolInput(QWidget *parent)
    : QDialog(parent)
{
    this->setFixedSize(300,300);

    mainLayout = new QVBoxLayout;
    tempLayout = new QHBoxLayout;
    humLayout = new QHBoxLayout;
    presLayout = new QHBoxLayout;
    confirmLayout = new QHBoxLayout;

    tempLabel = new QLabel("Температура: ", this);
    humLabel = new QLabel("Влажность: ", this);
    presLabel = new QLabel("Давление: ", this);

    tempDSpinBox = new QDoubleSpinBox;
    humDSpinBox = new QDoubleSpinBox;
    presDSpinBox = new QDoubleSpinBox;

    confirmButton = new QPushButton("Применить", this);

    tempLayout->addWidget(tempLabel);
    tempLayout->addWidget(tempDSpinBox);

    humLayout->addWidget(humLabel);
    humLayout->addWidget(humDSpinBox);

    presLayout->addWidget(presLabel);
    presLayout->addWidget(presDSpinBox);

    confirmLayout->addWidget(confirmButton);

    mainLayout->addLayout(tempLayout);
    mainLayout->addLayout(humLayout);
    mainLayout->addLayout(presLayout);
    mainLayout->addLayout(confirmLayout);

    this->setLayout(mainLayout);

    connect(confirmButton, &QPushButton::clicked, this, [=](){
        double tData = tempDSpinBox->value();
        double hData = humDSpinBox->value();
        double pData = presDSpinBox->value();
        emit sendInputData(tData, hData, pData);
    });
}

void CoolInput::setMinMaxTempUnit(double minT, double maxT) {
    tempDSpinBox->setMinimum(minT);
    tempDSpinBox->setMaximum(maxT);
}

void CoolInput::setMinMaxHumUnit(double minH, double maxH) {
    humDSpinBox->setMinimum(minH);
    humDSpinBox->setMaximum(maxH);
    humDSpinBox->setDecimals(0);
}

void CoolInput::setMinMaxPresUnit(double minP, double maxP) {
    presDSpinBox->setMinimum(minP);
    presDSpinBox->setMaximum(maxP);
}

void CoolInput::setCurrentValues(double tmp, double hum, double pres) {
    tempDSpinBox->setValue(tmp);
    humDSpinBox->setValue(hum);
    presDSpinBox->setValue(pres);
}

CoolInput::~CoolInput()
{
}