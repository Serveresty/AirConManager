#include "../includes/coolwindow.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
#include <QDebug>
#include <QtMath>

/**
 * @file coolwindow.cpp
 * @brief Реализация класса CoolWindow.
 * 
 * Этот файл содержит реализацию методов класса CoolWindow,
 * отвечающего за создание и управление основным интерфейсом приложения.
 */
/**
 * @brief Конструктор класса CoolWindow.
 * 
 * Загружает настройки пользователя из XML-файла, устанавливает начальные значения для параметров
 * температуры, влажности и давления. Определяет графический интерфейс пользователя (GUI) с элементами
 * управления, такими как кнопки включения/выключения, управление температурой, направлением воздуха,
 * а также интерфейс для отображения графических элементов.
 * @param parent Родительский виджет для главного окна.
 */
CoolWindow::CoolWindow(QWidget *parent)
    : QMainWindow(parent)
{
    loadSettings("user_settings.xml"); // Загрузка настроек пользователя
    hGateDir = new int(0); // Инициализация направления по горизонтали
    vGateDir = new int(0); // Инициализация направления по вертикали

    // Установка минимального и максимального размера окна
    this->setMinimumSize(800,600);
    this->setMaximumSize(1024, 768);

    centralWidget = new QWidget(this); // Центральный виджет окна
    this->setCentralWidget(centralWidget); // Установка центрального виджета

    dataLayout = new QHBoxLayout;

    // Метка для индикации включения/выключения
    onOffLabel = new QLabel;

    airBlades = new QMovie("../../resourses/fan.gif");
    onOffLabel->setMovie(airBlades);
    onOffLabel->setFixedSize(100, 100);  // Пример размера 100x100 пикселей
    onOffLabel->setScaledContents(true);
    airBlades->start();
    airBlades->stop();

    // Создание сцены и графического вида для отображения данных
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);
    view->setStyleSheet("border: 0px;");

    // Добавление графических элементов на сцену для отображения температуры, влажности и давления
    thermometer = scene->addRect(50, 10, 30, 300);
    mercuryLevel = scene->addRect(51, 310, 28, 0, QPen(), QBrush(Qt::red));

    humidityScale = scene->addRect(150, 10, 30, 300);
    QColor customBlueColor("#8AC8FF");
    QBrush humBrush(customBlueColor);
    humidityLevel = scene->addRect(151, 310, 28, 0, QPen(), humBrush);

    pressureScale = scene->addRect(250, 10, 30, 300);
    pressureLevel = scene->addRect(251, 310, 28, 0, QPen(), QBrush(Qt::gray));

    // Создаем стрелку, указывающую текущее вертикальное направление
    hArrow = scene->addLine(0, 0, 0, -90, QPen(Qt::red, 3));
    hStaticArrow = scene->addLine(0, 0, 0, -90, QPen(Qt::gray, 3));
    hStaticArrow->setLine(450, 10, 450, 100);
    hStaticArrow2 = scene->addLine(0, 0, -90, 0, QPen(Qt::gray, 3));
    hStaticArrow2->setLine(450, 10, 360, 10);

    hAngleArc = new QGraphicsPathItem();
    hAngleArc->setPen(QPen(Qt::blue, 2));
    scene->addItem(hAngleArc);

    hArrow->setZValue(1);

    updateHArrow();

    hAirText = scene->addText("Вертикальное направление воздуха");
    hAirText->setPos(310, 125);

    // Создаем стрелку, указывающую текущее горизонтальное направление
    vArrow = scene->addLine(0, 0, 0, -90, QPen(Qt::red, 3));
    vStaticArrow = scene->addLine(0, 0, 0, -90, QPen(Qt::gray, 3));
    vStaticArrow->setLine(405, 195, 360, 240);
    vStaticArrow2 = scene->addLine(0, 0, -90, 0, QPen(Qt::gray, 3));
    vStaticArrow2->setLine(405, 195, 450, 240);

    vArrow->setZValue(1);

    updateVArrow();

    vAirText = scene->addText("Горизонтальное направление воздуха");
    vAirText->setPos(310, 260);

    // Отображение текстовых меток для температуры, влажности и давления
    temperatureText = scene->addText("Т: ");
    temperatureText->setPos(40, 320);
    humidityText = scene->addText("В: ");
    humidityText->setPos(150, 320);
    pressureText = scene->addText("Д: ");
    pressureText->setPos(220, 320);

    // Добавление виджета индикации включения/выключения и графического вида в макет
    dataLayout->addWidget(onOffLabel);
    dataLayout->addWidget(view);

    buttonsLayout = new QHBoxLayout;
    onOffButton = new QPushButton("Вкл", this); // Кнопка включения/выключения
    onOffButton->setMinimumHeight(50);
    onOffButton->setMaximumWidth(300);

    // Вертикальный макет для управления температурой
    changeTempLayout = new QVBoxLayout;
    tempUp = new QPushButton("+", this); // Кнопка увеличения температуры
    tempUp->setMaximumSize(35,35);
    tempUp->setEnabled(isOn); // Кнопка активна, если система включена

    tempDown = new QPushButton("-", this); // Кнопка уменьшения температуры
    tempDown->setMaximumSize(35,35);
    tempDown->setEnabled(isOn); // Кнопка активна, если система включена

    changeTempLayout->addWidget(tempUp);
    changeTempLayout->addWidget(tempDown);

    // Макеты для управления направлением воздушного потока
    airLayout = new QGridLayout;
    airUp = new QPushButton(QString::fromUtf8("\u2191"), this);
    airDown = new QPushButton(QString::fromUtf8("\u2193"), this);
    airLeft = new QPushButton(QString::fromUtf8("\u2190"), this);
    airRight = new QPushButton(QString::fromUtf8("\u2192"), this);
    airUp->setMaximumSize(50,45);
    airDown->setMaximumSize(50,45);
    airLeft->setMaximumSize(50,45);
    airRight->setMaximumSize(50,45);
    airUp->setMinimumHeight(25);
    airDown->setMinimumHeight(25);
    airLeft->setMinimumHeight(25);
    airRight->setMinimumHeight(25);
    airUp->setEnabled(isOn);
    airDown->setEnabled(isOn);
    airLeft->setEnabled(isOn);
    airRight->setEnabled(isOn);

    // Добавление кнопок управления воздухом в макет
    airBttnsLabel = new QLabel;
    airBttnsLabel->setText("Воздух");
    airBttnsLabel->setAlignment(Qt::AlignCenter);

    airLayout->addWidget(airUp, 0, 1);
    airLayout->addWidget(airDown, 2, 1);
    airLayout->addWidget(airLeft, 1, 0);
    airLayout->addWidget(airRight, 1, 2);
    airLayout->addWidget(airBttnsLabel, 1, 1);
    
     // Кнопка для открытия настроек
    openSettings = new QPushButton("Настройки", this);
    openSettings->setMinimumHeight(50);
    openSettings->setMaximumWidth(300);
    openSettings->setEnabled(isOn);

    // Кнопка для изменения входных параметров
    openInput = new QPushButton("Изменить входные параметры", this);
    openInput->setMinimumHeight(50);
    openInput->setMaximumWidth(300);
    openInput->setEnabled(isOn);

    // Добавление всех кнопок в макет
    buttonsLayout->addWidget(onOffButton);
    buttonsLayout->addLayout(changeTempLayout);
    buttonsLayout->addLayout(airLayout);
    buttonsLayout->addWidget(openSettings);
    buttonsLayout->addWidget(openInput);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(buttonsLayout);

    setCurrentTheme(); // Установка текущей темы оформления
    openSettings->setStyleSheet(getLockStyle());
    openInput->setStyleSheet(getLockStyle());
    tempUp->setStyleSheet(getLockStyle());
    tempDown->setStyleSheet(getLockStyle());
    airUp->setStyleSheet(getLockStyle());
    airDown->setStyleSheet(getLockStyle());
    airLeft->setStyleSheet(getLockStyle());
    airRight->setStyleSheet(getLockStyle());

    centralWidget->setLayout(mainLayout);

    // Соединение сигналов и слотов для обработки нажатий кнопок
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

/**
 * @brief Устанавливает текущую тему оформления в зависимости от выбранной пользователем.
 */
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

/**
 * @brief Применяет темную тему оформления.
 * 
 * Устанавливает темный фон и белые границы для всех элементов интерфейса.
 */
void CoolWindow::applyDarkTheme() {
    centralWidget->setStyleSheet("background: black; color: white;");
    onOffButton->setStyleSheet("border: 1px solid white;");
    openSettings->setStyleSheet("border: 1px solid white;");
    openInput->setStyleSheet("border: 1px solid white;");
    tempUp->setStyleSheet("border: 1px solid white;");
    tempDown->setStyleSheet("border: 1px solid white;");
    airBttnsLabel->setStyleSheet("color: white;");
    airUp->setStyleSheet("border: 1px solid white;");
    airDown->setStyleSheet("border: 1px solid white;");
    airLeft->setStyleSheet("border: 1px solid white;");
    airRight->setStyleSheet("border: 1px solid white;");
    temperatureText->setDefaultTextColor(Qt::white);
    humidityText->setDefaultTextColor(Qt::white);
    pressureText->setDefaultTextColor(Qt::white);
    hAirText->setDefaultTextColor(Qt::white);
    vAirText->setDefaultTextColor(Qt::white);

    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(1);
    thermometer->setPen(pen);
    humidityScale->setPen(pen);
    pressureScale->setPen(pen);

    mercuryLevel->setPen(pen);
    humidityLevel->setPen(pen);
    pressureLevel->setPen(pen);

    currentTheme = Theme::Dark;
}

/**
 * @brief Применяет светлую тему оформления.
 * 
 * Устанавливает светлый фон и черные границы для всех элементов интерфейса.
 */
void CoolWindow::applyLightTheme() {
    centralWidget->setStyleSheet("background: white; color: black;");
    onOffButton->setStyleSheet("border: 1px solid black;");
    openSettings->setStyleSheet("border: 1px solid black;");
    openInput->setStyleSheet("border: 1px solid black;");
    tempUp->setStyleSheet("border: 1px solid black;");
    tempDown->setStyleSheet("border: 1px solid black;");
    airBttnsLabel->setStyleSheet("color: black;");
    airUp->setStyleSheet("border: 1px solid black;");
    airDown->setStyleSheet("border: 1px solid black;");
    airLeft->setStyleSheet("border: 1px solid black;");
    airRight->setStyleSheet("border: 1px solid black;");
    temperatureText->setDefaultTextColor(Qt::black);
    humidityText->setDefaultTextColor(Qt::black);
    pressureText->setDefaultTextColor(Qt::black);
    hAirText->setDefaultTextColor(Qt::black);
    vAirText->setDefaultTextColor(Qt::black);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    thermometer->setPen(pen);
    humidityScale->setPen(pen);
    pressureScale->setPen(pen);

    mercuryLevel->setPen(pen);
    humidityLevel->setPen(pen);
    pressureLevel->setPen(pen);

    currentTheme = Theme::Light;
}

/**
 * @brief Принимает новые данные о температуре, влажности и давлении.
 * 
 * @param tData Температура.
 * @param hData Влажность.
 * @param pData Давление.
 */
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

/**
 * @brief Обновляет визуальное представление уровня ртути в зависимости от температуры.
 */
void CoolWindow::setTemp() {
    double minT = getMinTempForCurrentUnit();
    double maxT = getMaxTempForCurrentUnit();
    double range = 300/(maxT-minT);
    mercuryLevel->setRect(51, 310 - (*temperature - minT) * range, 28, (*temperature - minT) * range);
}

/**
 * @brief Обновляет визуальное представление уровня влажности.
 */
void CoolWindow::setHum() {
    humidityLevel->setRect(151, 310 - *humidity * 3, 28, *humidity * 3);
}

/**
 * @brief Обновляет визуальное представление уровня давления.
 */
void CoolWindow::setPres() {
    double minP = getMinPresForCurrentUnit();
    double maxP = getMaxPresForCurrentUnit();
    double range = 300/(maxP-minP);
    pressureLevel->setRect(251, 310 - (*pressure - minP) * range, 28, (*pressure - minP) * range);
}

/**
 * @brief Принимает новые настройки для единиц измерения температуры и давления.
 * 
 * @param tempId ID новой единицы измерения температуры.
 * @param presId ID новой единицы измерения давления.
 */
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

    if (inputWindow) {
        inputWindow->setMinMaxTempUnit(getMinTempForCurrentUnit(), getMaxTempForCurrentUnit());
        setHumRange();
        inputWindow->setMinMaxPresUnit(getMinPresForCurrentUnit(), getMaxPresForCurrentUnit());
        inputWindow->setCurrentValues(*temperature, getTemperatureScaleByUnitId(currentTempUnit), *humidity, *pressure, getPressureScaleByUnitId(currentPresUnit));
    }
}

/**
 * @brief Возвращает строковое представление единицы измерения температуры по ID.
 * 
 * @param id ID единицы измерения температуры.
 * @return QString Строка с обозначением единицы (C, F, K).
 */
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

/**
 * @brief Возвращает единицу измерения температуры по строковому обозначению.
 * 
 * @param scale Строка с обозначением единицы измерения.
 * @return CoolWindow::TemperatureUnit Соответствующая единица измерения температуры.
 */
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

/**
 * @brief Увеличивает значение температуры.
 */
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

/**
 * @brief Уменьшает значение температуры.
 */
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

/**
 * @brief Увеличивает направление воздушного потока вверх.
 */
void CoolWindow::addAirUp() {
    if (*hGateDir + 5 <= getMaxHDir()) {
        *hGateDir = *hGateDir + 5;
        updateHArrow();
    }
}

/**
 * @brief Увеличивает направление воздушного потока вниз.
 */
void CoolWindow::addAirDown() {
    if (*hGateDir - 5 >= getMinHDir()) {
        *hGateDir = *hGateDir - 5;
        updateHArrow();
    }
}

/**
 * @brief Увеличивает направление воздушного потока влево.
 */
void CoolWindow::addAirLeft() {
    if (*vGateDir + 5 <= getMaxVDir()) {
        *vGateDir = *vGateDir + 5;
        updateVArrow();
    }
}

/**
 * @brief Уменьшает направление воздушного потока вправо.
 */
void CoolWindow::addAirRight() {
    if (*vGateDir - 5 >= getMinVDir()) {
        *vGateDir = *vGateDir - 5;
        updateVArrow();
    }
}

/**
 * @brief Возвращает минимальное значение направления вертикального потока воздуха.
 * 
 * @return int Минимальное значение направления вертикального потока воздуха.
 */
int CoolWindow::getMinHDir() {
    return 0;
}

/**
 * @brief Возвращает максимальное значение направления вертикального потока воздуха.
 * 
 * @return int Максимальное значение направления вертикального потока воздуха.
 */
int CoolWindow::getMaxHDir() {
    return 90;
}

/**
 * @brief Возвращает минимальное значение направления горизонтального потока воздуха.
 * 
 * @return int Минимальное значение направления горизонтального потока воздуха.
 */
int CoolWindow::getMinVDir() {
    return -45;
}

/**
 * @brief Возвращает максимальное значение направления горизонтального потока воздуха.
 * 
 * @return int Максимальное значение направления горизонтального потока воздуха.
 */
int CoolWindow::getMaxVDir() {
    return 45;
}

/**
 * @brief Возвращает строковое представление единицы измерения давления по ID.
 * 
 * @param id ID единицы измерения давления.
 * @return QString Строка с обозначением единицы (Pa, mm.h.g.).
 */
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

/**
 * @brief Возвращает единицу измерения давления по строковому обозначению.
 * 
 * @param scale Строка с обозначением единицы измерения.
 * @return CoolWindow::PressureUnit Соответствующая единица измерения давления.
 */
CoolWindow::PressureUnit CoolWindow::getPressureUnitByScale(QString scale) {
    if (scale == "Pa") {
        return PressureUnit::Pascal;
    } else if (scale == "mm.h.g.") {
        return PressureUnit::Mmhg;
    }

    return PressureUnit::Pascal;
}

/**
 * @brief Возвращает строковое представление темы по ID.
 * 
 * @param id ID темы.
 * @return QString Строка с обозначением темы (Light, Dark).
 */
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

/**
 * @brief Возвращает тему по ее строковому обозначению.
 * 
 * @param theme Строка с обозначением темы.
 * @return CoolWindow::Theme Соответствующая тема.
 */
CoolWindow::Theme CoolWindow::getThemeUnitByName(QString theme) {
    if (theme == "Light") {
        return Theme::Light;
    } else if (theme == "Dark") {
        return Theme::Dark;
    }
    return Theme::Light;
}

/**
 * @brief Пересчитывает температуру из одной единицы измерения в другую.
 * 
 * @param from Единица измерения, из которой необходимо пересчитать.
 * @param to Единица измерения, в которую необходимо пересчитать.
 */
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

/**
 * @brief Пересчитывает давление из одной единицы измерения в другую.
 * 
 * @param from Единица измерения, из которой необходимо пересчитать.
 * @param to Единица измерения, в которую необходимо пересчитать.
 */
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

/**
 * @brief Переключает индикатор состояния устройства (включено/выключено).
 */
void CoolWindow::toggleIndicator() {
    isOn = !isOn;
    if (isOn) {
        airBlades->start();
        onOffButton->setText("Выкл");

        setTemp(); // Установка температуры
        setHum(); // Установка влажности
        setPres(); // Установка давления
        temperatureText->setPlainText("Т: " + QString::number(*temperature) + " " + getTemperatureScaleByUnitId(currentTempUnit));
        humidityText->setPlainText("В: " + QString::number(*humidity) + " %");
        pressureText->setPlainText("Д: " + QString::number(*pressure, 'f', 1) + " " + getPressureScaleByUnitId(currentPresUnit));
        setCurrentTheme();
    } else {
        airBlades->stop();
        onOffButton->setText("Вкл");

        offSystem();

        openSettings->setStyleSheet(getLockStyle());
        openInput->setStyleSheet(getLockStyle());
        tempUp->setStyleSheet(getLockStyle());
        tempDown->setStyleSheet(getLockStyle());
        airUp->setStyleSheet(getLockStyle());
        airDown->setStyleSheet(getLockStyle());
        airLeft->setStyleSheet(getLockStyle());
        airRight->setStyleSheet(getLockStyle());
    }
    openInput->setEnabled(isOn);
    tempUp->setEnabled(isOn);
    tempDown->setEnabled(isOn);
    airUp->setEnabled(isOn);
    airDown->setEnabled(isOn);
    airLeft->setEnabled(isOn);
    airRight->setEnabled(isOn);
    openSettings->setEnabled(isOn);
}

/**
 * @brief Задаёт минимальные значения столбцов с показателями, убирает показатель температуры и устанавливает жалюзи в стартовую позицию.
 */
void CoolWindow::offSystem() {
    mercuryLevel->setRect(51, 310, 28, 0);
    pressureLevel->setRect(251, 310, 28, 0);
    humidityLevel->setRect(151, 310, 28, 0);

    temperatureText->setPlainText("Т: ");
    humidityText->setPlainText("В: ");
    pressureText->setPlainText("Д: ");

    *vGateDir = 0;
    *hGateDir = 0;
    updateVArrow();
    updateHArrow();
}

/**
 * @brief Открывает окно настроек.
 */
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

/**
 * @brief Открывает окно ввода данных.
 */
void CoolWindow::openInputWindow() {
    if (!inputWindow) {
        inputWindow = new CoolInput(this);

        onOffButton->setEnabled(false);
        tempUp->setEnabled(false);
        tempDown->setEnabled(false);
        airUp->setEnabled(false);
        airDown->setEnabled(false);
        airLeft->setEnabled(false);
        airRight->setEnabled(false);

        QString lockStyle = getLockStyle();
        onOffButton->setStyleSheet(lockStyle);
        tempUp->setStyleSheet(lockStyle);
        tempDown->setStyleSheet(lockStyle);
        airUp->setStyleSheet(lockStyle);
        airDown->setStyleSheet(lockStyle);
        airLeft->setStyleSheet(lockStyle);
        airRight->setStyleSheet(lockStyle);
        connect(inputWindow, &QDialog::finished, this, [=]() {
            inputWindow = nullptr;
            onOffButton->setEnabled(true);
            tempUp->setEnabled(true);
            tempDown->setEnabled(true);
            airUp->setEnabled(true);
            airDown->setEnabled(true);
            airLeft->setEnabled(true);
            airRight->setEnabled(true);

            setCurrentTheme();
        });

        connect(inputWindow, &CoolInput::sendInputData, this, &CoolWindow::acceptNewData);

        inputWindow->setMinMaxTempUnit(getMinTempForCurrentUnit(), getMaxTempForCurrentUnit());
        setHumRange();
        inputWindow->setMinMaxPresUnit(getMinPresForCurrentUnit(), getMaxPresForCurrentUnit());
        inputWindow->setCurrentValues(*temperature, getTemperatureScaleByUnitId(currentTempUnit), *humidity, *pressure, getPressureScaleByUnitId(currentPresUnit));
    }
    
    inputWindow->show();
    inputWindow->raise();
    inputWindow->activateWindow();
}

/**
 * @brief Возвращает стиль блокировки в зависимости от текущей темы.
 * 
 * @return QString Стиль блокировки в виде строки.
 */
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

/**
 * @brief Возвращает минимально допустимую температуру для текущей единицы измерения.
 * @return Минимальная температура в текущей единице измерения (Цельсий, Фаренгейт, Кельвин).
 */
double CoolWindow::getMinTempForCurrentUnit() {
    switch (currentTempUnit) {
        case TemperatureUnit::Celsius:
            return -10; // Минимальная температура в Цельсиях
        case TemperatureUnit::Fahrenheit:
            return 14; // Минимальная температура в Фаренгейтах
        case TemperatureUnit::Kelvin:
            return 263.15; // Минимальная температура в Кельвинах
        default:
            return 0.0; // Значение по умолчанию
    }
}

/**
 * @brief Возвращает максимально допустимую температуру для текущей единицы измерения.
 * @return Максимальная температура в текущей единице измерения (Цельсий, Фаренгейт, Кельвин).
 */
double CoolWindow::getMaxTempForCurrentUnit() {
    switch (currentTempUnit) {
        case TemperatureUnit::Celsius:
            return 30.0; // Максимальная температура в Цельсиях
        case TemperatureUnit::Fahrenheit:
            return 86; // Максимальная температура в Фаренгейтах
        case TemperatureUnit::Kelvin:
            return 303.15; // Максимальная температура в Кельвинах
        default:
            return 100.0; // Значение по умолчанию
    }
}

/**
 * @brief Устанавливает диапазон влажности в окне ввода данных.
 * 
 * Минимальное значение влажности установлено на 0%, максимальное — на 100%.
 */
void CoolWindow::setHumRange() {
    double minH, maxH;
    minH = 0.0; // Минимальная влажность
    maxH = 100.0; // Максимальная влажность
    inputWindow->setMinMaxHumUnit(minH, maxH); // Установка диапазона влажности
}

/**
 * @brief Возвращает минимальное давление для текущей единицы измерения.
 * @return Минимальное давление в текущей единице измерения (Паскали или мм рт. ст.).
 */
double CoolWindow::getMinPresForCurrentUnit() {
    switch (currentPresUnit) {
        case PressureUnit::Pascal:
            return 87000.0; // Минимальное давление в Паскалях
        case PressureUnit::Mmhg:
            return 652.0; // Минимальное давление в мм рт. ст.
        default:
            return 87000.0; // Значение по умолчанию
    }
}

/**
 * @brief Возвращает максимальное давление для текущей единицы измерения.
 * @return Максимальное давление в текущей единице измерения (Паскали или мм рт. ст.).
 */
double CoolWindow::getMaxPresForCurrentUnit() {
    switch (currentPresUnit) {
        case PressureUnit::Pascal:
            return 108500.0; // Максимальное давление в Паскалях
        case PressureUnit::Mmhg:
            return 814.0; // Максимальное давление в мм рт. ст.
        default:
            return 108500.0; // Значение по умолчанию
    }
}

/**
 * @brief Сохраняет текущие настройки пользователя в XML-файл.
 * @param filePath Путь к файлу для сохранения настроек.
 * 
 * Сохраняет значения температуры, влажности, давления и выбранную тему интерфейса
 * в указанный XML-файл.
 */
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

/**
 * @brief Загружает настройки пользователя из XML-файла.
 * @param filePath Путь к файлу для загрузки настроек.
 * 
 * Загружает значения температуры, влажности, давления и темы интерфейса из указанного XML-файла.
 * Если файл недоступен или имеет ошибку, загружаются базовые настройки.
 */
void CoolWindow::loadSettings(const QString &filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        setBaseSettings();
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
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

/**
 * @brief Устанавливает базовые настройки температуры, влажности, давления и темы.
 * 
 * Устанавливаются значения по умолчанию: температура — 16°C, давление — 87000 Паскалей,
 * влажность — 0%, тема интерфейса — светлая.
 */
void CoolWindow::setBaseSettings() {
    temperature = new double(16.0);
    humidity = new double(0.0);
    pressure = new double(87000.0);
    currentTempUnit = TemperatureUnit::Celsius;
    currentPresUnit = PressureUnit::Pascal;
    currentTheme = Theme::Light;
}

/**
 * @brief Изменяет положение горизонтальных жалюзи и отображает вертикальное направление воздуха.
 */
void CoolWindow::updateHArrow() {
    double radians = qDegreesToRadians(static_cast<double>(*hGateDir+90));
    int x = static_cast<int>(90 * qCos(radians));
    int y = static_cast<int>(90 * qSin(radians));

    hArrow->setLine(450, 10, x+450, y+10);

    QPainterPath path;
    path.moveTo(450, 100);

    double endAngle = *hGateDir;

    QRectF arcRect(360, -80, 180, 180);
    path.arcTo(arcRect, -90, -endAngle);

    hAngleArc->setPath(path);
}

/**
 * @brief Изменяет положение вертикальных жалюзи и отображает горизонтальное направление воздуха.
 */
void CoolWindow::updateVArrow() {
    double radians = qDegreesToRadians(static_cast<double>(*vGateDir+90));
    int x = static_cast<int>(55 * qCos(radians));
    int y = static_cast<int>(55 * qSin(radians));

    vArrow->setLine(405, 195, x+405, y+195);
}

/**
 * @brief Деструктор класса CoolWindow.
 * 
 * При завершении работы сохраняет настройки в файл "user_settings.xml" и освобождает память,
 * выделенную под значения температуры, влажности и давления.
 */
CoolWindow::~CoolWindow() {
    saveSettings("user_settings.xml");
    delete temperature;
    delete humidity;
    delete pressure;
    delete hGateDir;
    delete vGateDir;
}