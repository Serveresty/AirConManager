#include "../includes/coolinputwindow.h"

/**
 * @file coolinputwindow.cpp
 * @brief Реализация класса CoolInput.
 * 
 * Этот файл содержит реализацию методов класса CoolInput,
 * который отвечает за создание и управление интерфейсом ввода данных.
 */

/**
 * @brief Конструктор класса CoolInput.
 * 
 * Создаёт экземпляр окна ввода данных с элементами управления
 * для ввода температуры, влажности и давления.
 * 
 * @param parent Указатель на родительский объект.
 */
CoolInput::CoolInput(QWidget *parent)
    : QDialog(parent)
{
    this->setFixedSize(300,300);

    mainLayout = new QVBoxLayout; ///< Главная компоновка для размещения элементов управления.
    tempLayout = new QHBoxLayout; ///< Компоновка для размещения элементов управления температурой.
    humLayout = new QHBoxLayout; ///< Компоновка для размещения элементов управления влажностью.
    presLayout = new QHBoxLayout; ///< Компоновка для размещения элементов управления давлением.
    confirmLayout = new QHBoxLayout; ///< Компоновка для размещения кнопки подтверждения.

    tempLabel = new QLabel("Температура: ", this); ///< Метка для отображения текста "Температура".
    humLabel = new QLabel("Влажность: ", this); ///< Метка для отображения текста "Влажность".
    presLabel = new QLabel("Давление: ", this); ///< Метка для отображения текста "Давление".

    tempDSpinBox = new QDoubleSpinBox; ///< Поле для ввода температуры.
    humDSpinBox = new QDoubleSpinBox; ///< Поле для ввода влажности.
    presDSpinBox = new QDoubleSpinBox; ///< Поле для ввода давления.

    confirmButton = new QPushButton("Применить", this); ///< Кнопка для подтверждения ввода данных.

    tempLayout->addWidget(tempLabel); ///< Добавление метки температуры в компоновку.
    tempLayout->addWidget(tempDSpinBox); ///< Добавление поля ввода температуры в компоновку.

    humLayout->addWidget(humLabel); ///< Добавление метки влажности в компоновку.
    humLayout->addWidget(humDSpinBox); ///< Добавление поля ввода влажности в компоновку.

    presLayout->addWidget(presLabel); ///< Добавление метки давления в компоновку.
    presLayout->addWidget(presDSpinBox); ///< Добавление поля ввода давления в компоновку.

    confirmLayout->addWidget(confirmButton); ///< Добавление кнопки подтверждения в компоновку.

    mainLayout->addLayout(tempLayout); ///< Добавление компоновки температуры в главную компоновку.
    mainLayout->addLayout(humLayout); ///< Добавление компоновки влажности в главную компоновку.
    mainLayout->addLayout(presLayout); ///< Добавление компоновки давления в главную компоновку.
    mainLayout->addLayout(confirmLayout); ///< Добавление компоновки подтверждения в главную компоновку.

    this->setLayout(mainLayout); ///< Установка главной компоновки как основной компоновки окна.

    connect(confirmButton, &QPushButton::clicked, this, [=](){
        double tData = tempDSpinBox->value(); ///< Получение значения температуры.
        double hData = humDSpinBox->value(); ///< Получение значения влажности.
        double pData = presDSpinBox->value(); ///< Получение значения давления.
        emit sendInputData(tData, hData, pData); ///< Отправка введённых данных через сигнал.
    });
}

/**
 * @brief Устанавливает минимальное и максимальное значения для поля ввода температуры.
 * 
 * @param minT Минимальное значение температуры.
 * @param maxT Максимальное значение температуры.
 */
void CoolInput::setMinMaxTempUnit(double minT, double maxT) {
    tempDSpinBox->setMinimum(minT);
    tempDSpinBox->setMaximum(maxT);
}

/**
 * @brief Устанавливает минимальное и максимальное значения для поля ввода влажности.
 * 
 * @param minH Минимальное значение влажности.
 * @param maxH Максимальное значение влажности.
 */
void CoolInput::setMinMaxHumUnit(double minH, double maxH) {
    humDSpinBox->setMinimum(minH);
    humDSpinBox->setMaximum(maxH);
    humDSpinBox->setDecimals(0);
}

/**
 * @brief Устанавливает минимальное и максимальное значения для поля ввода давления.
 * 
 * @param minP Минимальное значение давления.
 * @param maxP Максимальное значение давления.
 */
void CoolInput::setMinMaxPresUnit(double minP, double maxP) {
    presDSpinBox->setMinimum(minP);
    presDSpinBox->setMaximum(maxP);
}

/**
 * @brief Устанавливает текущие значения для полей ввода.
 * 
 * @param tmp Значение температуры.
 * @param hum Значение влажности.
 * @param pres Значение давления.
 */
void CoolInput::setCurrentValues(double tmp, double hum, double pres) {
    tempDSpinBox->setValue(tmp);
    humDSpinBox->setValue(hum);
    presDSpinBox->setValue(pres);
}

/**
 * @brief Деструктор класса CoolInput.
 * 
 * Освобождает ресурсы, используемые экземпляром класса.
 */
CoolInput::~CoolInput()
{
}