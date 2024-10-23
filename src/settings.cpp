#include "../includes/settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
{
    this->setFixedSize(300,300);

    mainLayout = new QVBoxLayout;
    tempLayout = new QVBoxLayout;
    presLayout = new QVBoxLayout;
    themeLayout = new QHBoxLayout;
    confirmLayout = new QHBoxLayout;

    tempLabel = new QLabel("Шкала температуры:", this);
    presLabel = new QLabel("Шкала давления:", this);

    cels = new QRadioButton("Цельсия", this);
    far = new QRadioButton("Фаренгейта", this);
    kelv = new QRadioButton("Кельвина", this);

    tempGroup = new QButtonGroup(this);
    tempGroup->addButton(cels, 1);
    tempGroup->addButton(far, 2);
    tempGroup->addButton(kelv, 3);

    pa = new QRadioButton("Па", this);
    mmrtst = new QRadioButton("мм.рт.ст.", this);

    presGroup = new QButtonGroup(this);
    presGroup->addButton(pa, 1);
    presGroup->addButton(mmrtst, 2);

    white = new QPushButton("Светлая тема", this);
    black = new QPushButton("Тёмная тема", this);

    confirmButton = new QPushButton("Применить", this);

    tempLayout->addWidget(tempLabel);
    tempLayout->addWidget(cels);
    tempLayout->addWidget(far);
    tempLayout->addWidget(kelv);

    presLayout->addWidget(presLabel);
    presLayout->addWidget(pa);
    presLayout->addWidget(mmrtst);

    themeLayout->addWidget(white);
    themeLayout->addWidget(black);

    confirmLayout->addWidget(confirmButton);

    mainLayout->addLayout(tempLayout);
    mainLayout->addLayout(presLayout);
    mainLayout->addLayout(themeLayout);
    mainLayout->addLayout(confirmLayout);

    this->setLayout(mainLayout);

    connect(white, &QPushButton::clicked, this, [=](){
        emit lightThemeSelected();
    });
    connect(black, &QPushButton::clicked, this, [=](){
        emit darkThemeSelected();
    });
    connect(confirmButton, &QPushButton::clicked, this, [=](){
        int tempId = tempGroup->checkedId();
        int presId = presGroup->checkedId();
        emit confirmSettings(tempId, presId);
    });
}

void Settings::setActiveTempUnit(int id) {
    switch (id) {
        case 1:
            cels->setChecked(true);
            break;
        case 2:
            far->setChecked(true);
            break;
        case 3:
            kelv->setChecked(true);
            break;
        default:
            cels->setChecked(true);
            break;
    }
}

void Settings::setActivePresUnit(int id) {
    switch (id) {
        case 1:
            pa->setChecked(true);
            break;
        case 2:
            mmrtst->setChecked(true);
            break;
        default:
            pa->setChecked(true);
            break;
    }
}

Settings::~Settings()
{
}