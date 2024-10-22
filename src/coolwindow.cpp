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

    this->setMinimumSize(800,600);
    this->setMaximumSize(1024, 768);

    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    dataLayout = new QHBoxLayout;
    onOffLabel = new QLabel;
    onOffLabel->setFixedSize(10,10);
    onOffLabel->setStyleSheet("border-radius: 5px; background-color:red;");

    temperatureFrame = createDataFrames("Температура", temperature, getTemperatureScaleByUnitId(currentTempUnit));
    humidityFrame = createDataFrames("Влажность", humidity, "%");
    pressureFrame = createDataFrames("Давление", pressure, getPressureScaleByUnitId(currentPresUnit));

    dataLayout->addWidget(onOffLabel);
    dataLayout->addWidget(temperatureFrame);
    dataLayout->addWidget(humidityFrame);
    dataLayout->addWidget(pressureFrame);

    buttonsLayout = new QHBoxLayout;
    onOffButton = new QPushButton("Вкл", this);
    onOffButton->setMinimumHeight(50);
    onOffButton->setMaximumWidth(300);

    openSettings = new QPushButton("Настройки", this);
    openSettings->setMinimumHeight(50);
    openSettings->setMaximumWidth(300);

    openInput = new QPushButton("Изменить входные параметры", this);
    openInput->setMinimumHeight(50);
    openInput->setMaximumWidth(300);
    openInput->setEnabled(isOn);

    buttonsLayout->addWidget(onOffButton);
    buttonsLayout->addWidget(openSettings);
    buttonsLayout->addWidget(openInput);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(buttonsLayout);

    setCurrentTheme();

    centralWidget->setLayout(mainLayout);

    connect(onOffButton, &QPushButton::clicked, this, &CoolWindow::toggleIndicator);
    connect(openSettings, &QPushButton::clicked, this, &CoolWindow::openSettingsWindow);
    connect(openInput, &QPushButton::clicked, this, &CoolWindow::openInputWindow);
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

    currentTheme = Theme::Dark;
}

void CoolWindow::applyLightTheme() {
    centralWidget->setStyleSheet("background: white; color: black;");
    onOffButton->setStyleSheet("border: 1px solid black;");
    openSettings->setStyleSheet("border: 1px solid black;");
    openInput->setStyleSheet("border: 1px solid black;");

    currentTheme = Theme::Light;
}

void CoolWindow::acceptNewData(double tData, double hData, double pData) {
    delete temperature;
    temperature = new double(tData);
    delete humidity;
    humidity = new double(hData);
    delete pressure;
    pressure = new double(pData);
    QString newT = QString::number(tData);
    QString newH = QString::number(hData);
    QString newP = QString::number(pData);

    temperatureFrame->findChild<QLabel*>("valueWidget")->setText(newT);
    humidityFrame->findChild<QLabel*>("valueWidget")->setText(newH);
    pressureFrame->findChild<QLabel*>("valueWidget")->setText(newP);
}

void CoolWindow::acceptSettings(int tempId, int presId) {
    TemperatureUnit tid = static_cast<TemperatureUnit>(tempId);
    PressureUnit pid = static_cast<PressureUnit>(presId);

    recalculateTemp(currentTempUnit, tid);
    recalculatePres(currentPresUnit, pid);

    QString newT = QString::number(*temperature);
    QString newP = QString::number(*pressure);

    temperatureFrame->findChild<QLabel*>("scaleWidget")->setText(getTemperatureScaleByUnitId(tid));
    pressureFrame->findChild<QLabel*>("scaleWidget")->setText(getPressureScaleByUnitId(pid));

    temperatureFrame->findChild<QLabel*>("valueWidget")->setText(newT);
    pressureFrame->findChild<QLabel*>("valueWidget")->setText(newP);

    currentTempUnit = tid;
    currentPresUnit = pid;
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
            *temperature = *temperature * (9/5) + 32;
            return;
        }
        if (to == TemperatureUnit::Kelvin) {
            *temperature = *temperature + 273.15;
            return;
        }
    }
    if (from == TemperatureUnit::Fahrenheit) {
        if (to == TemperatureUnit::Celsius) {
            *temperature = (*temperature - 32) * (5/9);
            return;
        }
        if (to == TemperatureUnit::Kelvin) {
            *temperature = (*temperature - 32) * (5/9) + 273.15;
            return;
        }
    }
    if (from == TemperatureUnit::Kelvin) {
        if (to == TemperatureUnit::Celsius) {
            *temperature = *temperature - 273.15;
            return;
        }
        if (to == TemperatureUnit::Fahrenheit) {
            *temperature = (*temperature - 273.15) * (9/5) + 32;
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
            *pressure = *pressure / (9.80665 * 1000);
            return;
        }
    }
    if (from == PressureUnit::Mmhg) {
        if (to == PressureUnit::Pascal) {
            *pressure = *pressure * 9.80665 * 1000;
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
        onOffButton->setEnabled(false);
        openSettings->setEnabled(false);

        QString lockStyle = getLockStyle();
        onOffButton->setStyleSheet(lockStyle);
        openSettings->setStyleSheet(lockStyle);
        connect(inputWindow, &QDialog::finished, this, [=]() {
            inputWindow = nullptr;
            onOffButton->setEnabled(true);
            openSettings->setEnabled(true);

            onOffButton->setStyleSheet(origStOnOff);
            openSettings->setStyleSheet(origStOpSet);
        });

        connect(inputWindow, &CoolInput::sendInputData, this, &CoolWindow::acceptNewData);

        setTempRange();
        setHumRange();
        setPresRange();
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

void CoolWindow::setTempRange() {
    double minT, maxT;

    switch (currentTempUnit) {
        case TemperatureUnit::Celsius:
            minT = 16.0;
            maxT = 30.0;
            break;
        case TemperatureUnit::Fahrenheit:
            minT = 60.8;
            maxT = 86;
            break;
        case TemperatureUnit::Kelvin:
            minT = 289.15;
            maxT = 303.15;
            break;
        default:
            minT = 0.0;
            maxT = 100.0;
            break;
    }

    inputWindow->setMinMaxTempUnit(minT, maxT);
}

void CoolWindow::setHumRange() {
    double minH, maxH;
    minH = 0.0;
    maxH = 100.0;
    inputWindow->setMinMaxHumUnit(minH, maxH);
}

void CoolWindow::setPresRange() {
    double minP, maxP;

    switch (currentPresUnit) {
        case PressureUnit::Pascal:
            minP = 87000.0;
            maxP = 108500.0;
            break;
        case PressureUnit::Mmhg:
            minP = 652.0;
            maxP = 814.0;
            break;
        default:
            minP = 760.0;
            maxP = 760.0;
            break;
    }

    inputWindow->setMinMaxPresUnit(minP, maxP);
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

QFrame* CoolWindow::createDataFrames(const QString& label, double* value, const QString& scale) {
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::Box);
    frame->setFixedSize(150,150);

    QVBoxLayout *mLayout = new QVBoxLayout(frame);

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *dataLt = new QHBoxLayout;
    QLabel *labelWidget = new QLabel(label);
    labelWidget->setAlignment(Qt::AlignCenter);

    QString valueString = QString::number(*value);

    QLabel *valueWidget = new QLabel(valueString);
    valueWidget->setAlignment(Qt::AlignRight);
    valueWidget->setObjectName("valueWidget");

    QLabel *scaleWidget = new QLabel(scale);
    scaleWidget->setAlignment(Qt::AlignLeft);
    scaleWidget->setObjectName("scaleWidget");

    mLayout->addLayout(layout);
    mLayout->addLayout(dataLt);

    layout->addWidget(labelWidget);

    dataLt->addWidget(valueWidget);
    dataLt->addWidget(scaleWidget);

    return frame;
}

CoolWindow::~CoolWindow() {
    saveSettings("user_settings.xml");
    delete temperature;
    delete humidity;
    delete pressure;
}