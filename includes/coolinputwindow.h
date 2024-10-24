#ifndef COOLINPUTWINDOW_H
#define COOLINPUTWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>

/**
 * @file coolinputwindow.h
 * @brief Заголовочный файл для класса CoolInput.
 * 
 * Этот файл содержит объявление класса CoolInput,
 * который отвечает за создание и управление интерфейсом ввода данных.
 */

/**
 * @class CoolInput
 * @brief Класс для управления вводом данных через графический интерфейс.
 * 
 * Обеспечивает функциональность управления параметрами системы кондиционирования, такими как
 * температура, влажность, давление. Поддерживает изменение настроек.
 */
class CoolInput : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса CoolInput.
     * 
     * Создаёт экземпляр окна ввода данных с элементами управления
     * для ввода температуры, влажности и давления.
     * 
     * @param parent Указатель на родительский объект.
     */
    explicit CoolInput(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса CoolInput.
     * 
     * Освобождает ресурсы, используемые экземпляром класса.
     */
    ~CoolInput();

    /**
     * @brief Устанавливает минимальное и максимальное значения для поля ввода температуры.
     * 
     * @param minT Минимальное значение температуры.
     * @param maxT Максимальное значение температуры.
     */
    void setMinMaxTempUnit(double minT, double maxT);

    /**
     * @brief Устанавливает минимальное и максимальное значения для поля ввода влажности.
     * 
     * @param minH Минимальное значение влажности.
     * @param maxH Максимальное значение влажности.
     */
    void setMinMaxHumUnit(double minH, double maxH);

    /**
     * @brief Устанавливает минимальное и максимальное значения для поля ввода давления.
     * 
     * @param minP Минимальное значение давления.
     * @param maxP Максимальное значение давления.
     */
    void setMinMaxPresUnit(double minP, double maxP);

    /**
     * @brief Устанавливает текущие значения для полей ввода.
     * 
     * @param tmp Значение температуры.
     * @param hum Значение влажности.
     * @param pres Значение давления.
     */
    void setCurrentValues(double tmp, QString tS, double hum, double pres, QString pS);

signals:
    /**
     * @brief Сигнал, отправляемый при подтверждении ввода данных.
     * 
     * @param tData Значение температуры.
     * @param hData Значение влажности.
     * @param pData Значение давления.
     */
    void sendInputData(double tData, double hData, double pData);

private:
    QVBoxLayout *mainLayout; ///< Главная компоновка для размещения элементов управления.

    QHBoxLayout *tempLayout; ///< Компоновка для размещения элементов управления температурой.
    QLabel *tempLabel; ///< Метка для отображения текста "Температура".
    QDoubleSpinBox *tempDSpinBox; ///< Поле для ввода температуры.

    QHBoxLayout *humLayout; ///< Компоновка для размещения элементов управления влажностью.
    QLabel *humLabel; ///< Метка для отображения текста "Влажность".
    QDoubleSpinBox *humDSpinBox; ///< Поле для ввода влажности.

    QHBoxLayout *presLayout; ///< Компоновка для размещения элементов управления давлением.
    QLabel *presLabel; ///< Метка для отображения текста "Давление".
    QDoubleSpinBox *presDSpinBox; ///< Поле для ввода давления.

    QHBoxLayout *confirmLayout; ///< Компоновка для размещения кнопки подтверждения.
    QPushButton *confirmButton; ///< Кнопка для подтверждения ввода данных.
};

#endif