#include "../includes/coolwindow.h"
#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

CoolWindow::CoolWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadSettings("user_settings.xml");
    hGateDir = new int(0);
    vGateDir = new int(0);

    this->setMinimumSize(800,600);
    this->setMaximumSize(1024, 768);

    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    dataLayout = new QHBoxLayout;
    onOffLabel = new QLabel;
    onOffLabel->setFixedSize(10,10);
    onOffLabel->setStyleSheet("border-radius: 5px; background-color:red;");

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setStyleSheet("border: 0px;");

    thermometer = scene->addRect(50, 10, 30, 300);
    mercuryLevel = scene->addRect(51, 310, 28, 0, QPen(), QBrush(Qt::red));

    humidityScale = scene->addRect(150, 10, 30, 300);
    humidityLevel = scene->addRect(151, 310, 28, 0, QPen(), QBrush(Qt::blue));

    pressureScale = scene->addRect(250, 10, 30, 300);
    pressureLevel = scene->addRect(251, 310, 28, 0, QPen(), QBrush(Qt::magenta));

    temperatureText = scene->addText("Т: " + QString::number(*temperature) + " " + getTemperatureScaleByUnitId(currentTempUnit));
    temperatureText->setPos(40, 320);
    humidityText = scene->addText("В: " + QString::number(*humidity) + " %");
    humidityText->setPos(150, 320);
    pressureText = scene->addText("Д: " + QString::number(*pressure, 'f', 1) + " " + getPressureScaleByUnitId(currentPresUnit));
    pressureText->setPos(220, 320);

    dataLayout->addWidget(onOffLabel);
    dataLayout->addWidget(view);

    buttonsLayout = new QHBoxLayout;
    onOffButton = new QPushButton("Вкл", this);
    onOffButton->setMinimumHeight(50);
    onOffButton->setMaximumWidth(300);

    changeTempLayout = new QVBoxLayout;
    tempUp = new QPushButton("+", this);
    tempUp->setMaximumSize(50,50);
    tempUp->setEnabled(isOn);

    tempDown = new QPushButton("-", this);
    tempDown->setMaximumSize(50,50);
    tempDown->setEnabled(isOn);

    changeTempLayout->addWidget(tempUp);
    changeTempLayout->addWidget(tempDown);

    verticalAirLayout = new QVBoxLayout;
    horizontalAirLayout = new QHBoxLayout;
    airUp = new QPushButton(QString::fromUtf8("\u2191"), this);
    airDown = new QPushButton(QString::fromUtf8("\u2193"), this);
    airLeft = new QPushButton(QString::fromUtf8("\u2190"), this);
    airRight = new QPushButton(QString::fromUtf8("\u2192"), this);
    airUp->setMaximumSize(50,25);
    airDown->setMaximumSize(50,25);
    airLeft->setMaximumSize(50,25);
    airRight->setMaximumSize(50,25);
    airUp->setEnabled(isOn);
    airDown->setEnabled(isOn);
    airLeft->setEnabled(isOn);
    airRight->setEnabled(isOn);

    verticalAirLayout->addWidget(airUp);
    verticalAirLayout->addWidget(airDown);
    horizontalAirLayout->addWidget(airLeft);
    horizontalAirLayout->addWidget(airRight);
    
    openSettings = new QPushButton("Настройки", this);
    openSettings->setMinimumHeight(50);
    openSettings->setMaximumWidth(300);

    openInput = new QPushButton("Изменить входные параметры", this);
    openInput->setMinimumHeight(50);
    openInput->setMaximumWidth(300);
    openInput->setEnabled(isOn);

    buttonsLayout->addWidget(onOffButton);
    buttonsLayout->addLayout(changeTempLayout);
    buttonsLayout->addLayout(verticalAirLayout);
    buttonsLayout->addLayout(horizontalAirLayout);
    buttonsLayout->addWidget(openSettings);
    buttonsLayout->addWidget(openInput);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(buttonsLayout);

    setTemp();
    setHum();
    setPres();
    setCurrentTheme();

    centralWidget->setLayout(mainLayout);

    connect(onOffButton, &QPushButton::clicked, this, &CoolWindow::toggleIndicator);
    connect(openSettings, &QPushButton::clicked, this, &CoolWindow::openSettingsWindow);
    connect(openInput, &QPushButton::clicked, this, &CoolWindow::openInputWindow);
    connect(tempUp, &QPushButton::clicked, this, &CoolWindow::temperatureUp);
    connect(tempDown, &QPushButton::clicked, this, &CoolWindow::temperatureDown);
    connect(airUp, &QPushButton::clicked, this, &CoolWindow::addAirUp);
    connect(airDown, &QPushButton::clicked, this, &CoolWindow::addAirDown);
    connect(airLeft, &QPushButton::clicked, this, &CoolWindow::addAirLeft);
    connect(airRight, &QPushButton::clicked, this, &CoolWindow::addAirRight);
}

void CoolWindow::setCurrentTheme() {
    switch (currentTheme) {
        case Theme::Light:
            applyLightTheme();
            return;
        case Theme::Dark:
            applyDarkTheme();
            return;
        default:
            applyLightTheme();
            return;
    }
}

void CoolWindow::applyDarkTheme() {
    centralWidget->setStyleSheet("background: black; color: white;");
    onOffButton->setStyleSheet("border: 1px solid white;");
    openSettings->setStyleSheet("border: 1px solid white;");
    openInput->setStyleSheet("border: 1px solid white;");
    tempUp->setStyleSheet("border: 1px solid white;");
    tempDown->setStyleSheet("border: 1px solid white;");
    airUp->setStyleSheet("border: 1px solid white;");
    airDown->setStyleSheet("border: 1px solid white;");
    airLeft->setStyleSheet("border: 1px solid white;");
    airRight->setStyleSheet("border: 1px solid white;");
    temperatureText->setDefaultTextColor(Qt::white);
    humidityText->setDefaultTextColor(Qt::white);
    pressureText->setDefaultTextColor(Qt::white);

    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(1);
    thermometer->setPen(pen);
    humidityScale->setPen(pen);
    pressureScale->setPen(pen);

    currentTheme = Theme::Dark;
}

void CoolWindow::applyLightTheme() {
    centralWidget->setStyleSheet("background: white; color: black;");
    onOffButton->setStyleSheet("border: 1px solid black;");
    openSettings->setStyleSheet("border: 1px solid black;");
    openInput->setStyleSheet("border: 1px solid black;");
    tempUp->setStyleSheet("border: 1px solid black;");
    tempDown->setStyleSheet("border: 1px solid black;");
    airUp->setStyleSheet("border: 1px solid black;");
    airDown->setStyleSheet("border: 1px solid black;");
    airLeft->setStyleSheet("border: 1px solid black;");
    airRight->setStyleSheet("border: 1px solid black;");
    temperatureText->setDefaultTextColor(Qt::black);
    humidityText->setDefaultTextColor(Qt::black);
    pressureText->setDefaultTextColor(Qt::black);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    thermometer->setPen(pen);
    humidityScale->setPen(pen);
    pressureScale->setPen(pen);

    currentTheme = Theme::Light;
}

void CoolWindow::acceptNewData(double tData, double hData, double pData) {
    delete temperature;
    temperature = new double(tData);
    delete humidity;
    humidity = new double(hData);
    delete pressure;
    pressure = new double(pData);

    setTemp();
    setHum();
    setPres();
    temperatureText->setPlainText("Т: " + QString::number(*temperature) + " " + getTemperatureScaleByUnitId(currentTempUnit));
    humidityText->setPlainText("В: " + QString::number(*humidity) + " %");
    pressureText->setPlainText("Д: " + QString::number(*pressure, 'f', 1) + " " + getPressureScaleByUnitId(currentPresUnit));
}

void CoolWindow::setTemp() {
    double minT = getMinTempForCurrentUnit();
    double maxT = getMaxTempForCurrentUnit();
    double range = 300/(maxT-minT);
    mercuryLevel->setRect(51, 310 - (*temperature - minT) * range, 28, (*temperature - minT) * range);
}

void CoolWindow::setHum() {
    humidityLevel->setRect(151, 310 - *humidity * 3, 28, *humidity * 3);
}

void CoolWindow::setPres() {
    double minP = getMinPresForCurrentUnit();
    double maxP = getMaxPresForCurrentUnit();
    double range = 300/(maxP-minP);
    pressureLevel->setRect(251, 310 - (*pressure - minP) * range, 28, (*pressure - minP) * range);
}

void CoolWindow::acceptSettings(int tempId, int presId) {
    TemperatureUnit tid = static_cast<TemperatureUnit>(tempId);
    PressureUnit pid = static_cast<PressureUnit>(presId);

    recalculateTemp(currentTempUnit, tid);
    recalculatePres(currentPresUnit, pid);

    currentTempUnit = tid;
    currentPresUnit = pid;

    setTemp();
    setHum();
    setPres();
    temperatureText->setPlainText("Т: " + QString::number(*temperature) + " " + getTemperatureScaleByUnitId(currentTempUnit));
    humidityText->setPlainText("В: " + QString::number(*humidity) + " %");
    pressureText->setPlainText("Д: " + QString::number(*pressure, 'f', 1) + " " + getPressureScaleByUnitId(currentPresUnit));
}

QString CoolWindow::getTemperatureScaleByUnitId(TemperatureUnit id) {
    switch (id) {
        case TemperatureUnit::Celsius:
            currentTempUnit = TemperatureUnit::Celsius;
            return "C";
        case TemperatureUnit::Fahrenheit:
            currentTempUnit = TemperatureUnit::Fahrenheit;
            return "F";
        case TemperatureUnit::Kelvin:
            currentTempUnit = TemperatureUnit::Kelvin;
            return "K";
    }

    return "Unknown";
}

CoolWindow::TemperatureUnit CoolWindow::getTemperatureUnitByScale(QString scale) {
    if (scale == "C") {
        return TemperatureUnit::Celsius;
    } else if (scale == "F") {
        return TemperatureUnit::Fahrenheit;
    } else if (scale == "K") {
        return TemperatureUnit::Kelvin;
    }

    return TemperatureUnit::Celsius;
}

void CoolWindow::temperatureUp() {
    switch (currentTempUnit) {
        case TemperatureUnit::Fahrenheit:
            if (*temperature + 1.8 <= getMaxTempForCurrentUnit()) {
                *temperature = *temperature + 1.8;
            }
            break;
        default:
            if (*temperature + 1 <= getMaxTempForCurrentUnit()) {
                *temperature = *temperature + 1;
            }
            break;
    }

    setTemp();
    temperatureText->setPlainText("Т: " + QString::number(*temperature) + " " + getTemperatureScaleByUnitId(currentTempUnit));
}

void CoolWindow::temperatureDown() {
    switch (currentTempUnit) {
        case TemperatureUnit::Fahrenheit:
            if (*temperature - 1.8 >= getMinTempForCurrentUnit()) {
                *temperature = *temperature - 1.8;
            }
            break;
        default:
            if (*temperature - 1 >= getMinTempForCurrentUnit()) {
                *temperature = *temperature - 1;
            }
            break;
    }

    setTemp();
    temperatureText->setPlainText("Т: " + QString::number(*temperature) + " " + getTemperatureScaleByUnitId(currentTempUnit));
}

void CoolWindow::addAirUp() {
    if (*hGateDir + 5 <= getMaxHDir()) {
        *hGateDir = *hGateDir + 5;
    }
}

void CoolWindow::addAirDown() {
    if (*hGateDir - 5 >= getMinHDir()) {
        *hGateDir = *hGateDir - 5;
    }
}

void CoolWindow::addAirLeft() {
    if (*vGateDir + 5 <= getMaxVDir()) {
        *vGateDir = *vGateDir + 5;
    }
}

void CoolWindow::addAirRight() {
    if (*vGateDir - 5 >= getMinVDir()) {
        *vGateDir = *vGateDir - 5;
    }
}

int CoolWindow::getMinHDir() {
    return 0;
}

int CoolWindow::getMaxHDir() {
    return 90;
}

int CoolWindow::getMinVDir() {
    return -45;
}

int CoolWindow::getMaxVDir() {
    return 45;
}

QString CoolWindow::getPressureScaleByUnitId(PressureUnit id) {
    switch (id) {
        case PressureUnit::Pascal:
            currentPresUnit = PressureUnit::Pascal;
            return "Pa";
        case PressureUnit::Mmhg:
            currentPresUnit = PressureUnit::Mmhg;
            return "mm.h.g.";
    }

    return "Unknown";
}

CoolWindow::PressureUnit CoolWindow::getPressureUnitByScale(QString scale) {
    if (scale == "Pa") {
        return PressureUnit::Pascal;
    } else if (scale == "mm.h.g.") {
        return PressureUnit::Mmhg;
    }

    return PressureUnit::Pascal;
}

QString CoolWindow::getThemeById(Theme id) {
    switch (id) {
        case Theme::Light:
            currentPresUnit = PressureUnit::Pascal;
            return "Light";
        case Theme::Dark:
            currentPresUnit = PressureUnit::Mmhg;
            return "Dark";
    }

    return "Unknown";
}

CoolWindow::Theme CoolWindow::getThemeUnitByName(QString theme) {
    if (theme == "Light") {
        return Theme::Light;
    } else if (theme == "Dark") {
        return Theme::Dark;
    }
    return Theme::Light;
}

void CoolWindow::recalculateTemp(TemperatureUnit from, TemperatureUnit to) {
    if (from == to) {
        return;
    }

    if (from == TemperatureUnit::Celsius) {
        if (to == TemperatureUnit::Fahrenheit) {
            *temperature = *temperature * (9.0/5.0) + 32;
            return;
        }
        if (to == TemperatureUnit::Kelvin) {
            *temperature = *temperature + 273.15;
            return;
        }
    }
    if (from == TemperatureUnit::Fahrenheit) {
        if (to == TemperatureUnit::Celsius) {
            *temperature = (*temperature - 32) * (5.0/9.0);
            return;
        }
        if (to == TemperatureUnit::Kelvin) {
            *temperature = (*temperature - 32) * (5.0/9.0) + 273.15;
            return;
        }
    }
    if (from == TemperatureUnit::Kelvin) {
        if (to == TemperatureUnit::Celsius) {
            *temperature = *temperature - 273.15;
            return;
        }
        if (to == TemperatureUnit::Fahrenheit) {
            *temperature = (*temperature - 273.15) * (9.0/5.0) + 32;
            return;
        }
    }
}
void CoolWindow::recalculatePres(PressureUnit from, PressureUnit to) {
    if (from == to) {
        return;
    }
    if (from == PressureUnit::Pascal) {
        if (to == PressureUnit::Mmhg) {
            *pressure = *pressure / (133.3224);
            return;
        }
    }
    if (from == PressureUnit::Mmhg) {
        if (to == PressureUnit::Pascal) {
            *pressure = *pressure * 133.3224;
            return;
        }
    }
}

void CoolWindow::toggleIndicator() {
    isOn = !isOn;
    if (isOn) {
        onOffLabel->setStyleSheet("border-radius: 5px; background-color:green;");
        onOffButton->setText("Выкл");
    } else {
        onOffLabel->setStyleSheet("border-radius: 5px; background-color:red;");
        onOffButton->setText("Вкл");
    }
    openInput->setEnabled(isOn);
    tempUp->setEnabled(isOn);
    tempDown->setEnabled(isOn);
    airUp->setEnabled(isOn);
    airDown->setEnabled(isOn);
    airLeft->setEnabled(isOn);
    airRight->setEnabled(isOn);
}

void CoolWindow::openSettingsWindow() {
    if (!settingsWindow) {
        settingsWindow = new Settings(this);
        connect(settingsWindow, &QDialog::finished, this, [=]() {
            settingsWindow = nullptr;
        });

        connect(settingsWindow, &Settings::darkThemeSelected, this, &CoolWindow::applyDarkTheme);
        connect(settingsWindow, &Settings::lightThemeSelected, this, &CoolWindow::applyLightTheme);
        connect(settingsWindow, &Settings::confirmSettings, this, &CoolWindow::acceptSettings);

        int tid = static_cast<int>(currentTempUnit);
        int pid = static_cast<int>(currentPresUnit);

        settingsWindow->setActiveTempUnit(tid);
        settingsWindow->setActivePresUnit(pid);
    }

    settingsWindow->show();
    settingsWindow->raise();
    settingsWindow->activateWindow();
}

void CoolWindow::openInputWindow() {
    if (!inputWindow) {
        inputWindow = new CoolInput(this);

        QString origStOnOff = onOffButton->styleSheet();
        QString origStOpSet = openSettings->styleSheet();
        QString origStTempUp = openSettings->styleSheet();
        QString origStTempDown = openSettings->styleSheet();
        QString u = airUp->styleSheet();
        QString d = airDown->styleSheet();
        QString l = airLeft->styleSheet();
        QString r = airRight->styleSheet();
        onOffButton->setEnabled(false);
        openSettings->setEnabled(false);
        tempUp->setEnabled(false);
        tempDown->setEnabled(false);
        airUp->setEnabled(false);
        airDown->setEnabled(false);
        airLeft->setEnabled(false);
        airRight->setEnabled(false);

        QString lockStyle = getLockStyle();
        onOffButton->setStyleSheet(lockStyle);
        openSettings->setStyleSheet(lockStyle);
        tempUp->setStyleSheet(lockStyle);
        tempDown->setStyleSheet(lockStyle);
        airUp->setStyleSheet(lockStyle);
        airDown->setStyleSheet(lockStyle);
        airLeft->setStyleSheet(lockStyle);
        airRight->setStyleSheet(lockStyle);
        connect(inputWindow, &QDialog::finished, this, [=]() {
            inputWindow = nullptr;
            onOffButton->setEnabled(true);
            openSettings->setEnabled(true);
            tempUp->setEnabled(true);
            tempDown->setEnabled(true);
            airUp->setEnabled(true);
            airDown->setEnabled(true);
            airLeft->setEnabled(true);
            airRight->setEnabled(true);

            onOffButton->setStyleSheet(origStOnOff);
            openSettings->setStyleSheet(origStOpSet);
            tempUp->setStyleSheet(origStTempUp);
            tempDown->setStyleSheet(origStTempDown);
            airUp->setStyleSheet(u);
            airDown->setStyleSheet(d);
            airLeft->setStyleSheet(l);
            airRight->setStyleSheet(r);
        });

        connect(inputWindow, &CoolInput::sendInputData, this, &CoolWindow::acceptNewData);

        inputWindow->setMinMaxTempUnit(getMinTempForCurrentUnit(), getMaxTempForCurrentUnit());
        setHumRange();
        inputWindow->setMinMaxPresUnit(getMinPresForCurrentUnit(), getMaxPresForCurrentUnit());
        inputWindow->setCurrentValues(*temperature, *humidity, *pressure);
    }
    
    inputWindow->show();
    inputWindow->raise();
    inputWindow->activateWindow();
}

QString CoolWindow::getLockStyle() {
    switch (currentTheme) {
        case Theme::Dark:
            return "background-color: rgba(169, 169, 169, 150); border: 1px solid white;";
        case Theme::Light:
            return "background-color: rgba(169, 169, 169, 150); border: 1px solid black;";
        default:
            return "background-color: rgba(169, 169, 169, 150); border: 1px solid black;";
    }
}

double CoolWindow::getMinTempForCurrentUnit() {
    switch (currentTempUnit) {
        case TemperatureUnit::Celsius:
            return 16.0;
        case TemperatureUnit::Fahrenheit:
            return 60.8;
        case TemperatureUnit::Kelvin:
            return 289.15;
        default:
            return 0.0;
    }
}

double CoolWindow::getMaxTempForCurrentUnit() {
    switch (currentTempUnit) {
        case TemperatureUnit::Celsius:
            return 30.0;
        case TemperatureUnit::Fahrenheit:
            return 86;
        case TemperatureUnit::Kelvin:
            return 303.15;
        default:
            return 100.0;
    }
}


void CoolWindow::setHumRange() {
    double minH, maxH;
    minH = 0.0;
    maxH = 100.0;
    inputWindow->setMinMaxHumUnit(minH, maxH);
}

double CoolWindow::getMinPresForCurrentUnit() {
    switch (currentPresUnit) {
        case PressureUnit::Pascal:
            return 87000.0;
        case PressureUnit::Mmhg:
            return 652.0;
        default:
            return 87000.0;
    }
}

double CoolWindow::getMaxPresForCurrentUnit() {
    switch (currentPresUnit) {
        case PressureUnit::Pascal:
            return 108500.0;
        case PressureUnit::Mmhg:
            return 814.0;
        default:
            return 108500.0;
    }
}

void CoolWindow::saveSettings(const QString &filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QDomDocument doc;
    QDomElement root = doc.createElement("Settings");
    doc.appendChild(root);

    QDomElement tempElem = doc.createElement("Temperature");
    tempElem.setAttribute("value", QString::number(*temperature));
    tempElem.setAttribute("scale", getTemperatureScaleByUnitId(currentTempUnit));
    root.appendChild(tempElem);

    QDomElement humElem = doc.createElement("Humidity");
    humElem.setAttribute("value", QString::number(*humidity));
    root.appendChild(humElem);

    QDomElement presElem = doc.createElement("Pressure");
    presElem.setAttribute("value", QString::number(*pressure));
    presElem.setAttribute("scale", getPressureScaleByUnitId(currentPresUnit));
    root.appendChild(presElem);

    QDomElement themeElem = doc.createElement("Theme");
    themeElem.setAttribute("value", getThemeById(currentTheme));
    root.appendChild(themeElem);

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
}

void CoolWindow::loadSettings(const QString &filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open XML.";
        setBaseSettings();
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML.";
        file.close();
        setBaseSettings();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();

    QDomElement tempElem = root.firstChildElement("Temperature");
    temperature = new double(tempElem.attribute("value").toDouble());
    currentTempUnit = getTemperatureUnitByScale(tempElem.attribute("scale"));

    QDomElement humElem = root.firstChildElement("Humidity");
    humidity = new double(humElem.attribute("value").toDouble());

    QDomElement presElem = root.firstChildElement("Pressure");
    pressure = new double(presElem.attribute("value").toDouble());
    currentPresUnit = getPressureUnitByScale(presElem.attribute("scale"));

    QDomElement themeElem = root.firstChildElement("Theme");
    currentTheme = getThemeUnitByName(themeElem.attribute("value"));
}

void CoolWindow::setBaseSettings() {
    temperature = new double(16.0);
    humidity = new double(0.0);
    pressure = new double(87000.0);
    currentTempUnit = TemperatureUnit::Celsius;
    currentPresUnit = PressureUnit::Pascal;
    currentTheme = Theme::Light;
}

CoolWindow::~CoolWindow() {
    saveSettings("user_settings.xml");
    delete temperature;
    delete humidity;
    delete pressure;
}