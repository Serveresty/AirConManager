#ifndef COOLWINDOW_H
#define COOLWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QMovie>
#include "settings.h"
#include "coolinputwindow.h"

/**
 * @file coolwindow.h
 * @brief Заголовочный файл для класса CoolWindow.
 * 
 * Этот файл содержит объявление класса CoolWindow,
 * который отвечает за создание и управление основным интерфейсом приложения.
 */

/**
 * @class CoolWindow
 * @brief Класс для управления системой кондиционирования через графический интерфейс.
 * 
 * Обеспечивает функциональность отображения и управления параметрами системы кондиционирования, такими как
 * температура, влажность, давление и направления воздушного потока. Поддерживает изменение настроек и выбор тем.
 */
class CoolWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса CoolWindow.
     * @param parent Родительский элемент (по умолчанию nullptr).
     */
    CoolWindow(QWidget *parent = nullptr);
    /**
     * @brief Деструктор класса CoolWindow.
     */
    ~CoolWindow();

    /**
     * @enum TemperatureUnit
     * @brief Перечисление возможных единиц измерения температуры.
     */
    enum class TemperatureUnit {
        Celsius = 1,
        Fahrenheit,
        Kelvin
    };

    /**
     * @enum PressureUnit
     * @brief Перечисление возможных единиц измерения давления.
     */
    enum class PressureUnit {
        Pascal = 1,
        Mmhg
    };

    /**
     * @enum Theme
     * @brief Перечисление возможных тем интерфейса.
     */
    enum class Theme {
        Light = 1,
        Dark
    };

private slots:
    /**
     * @brief Переключение индикатора включения/выключения системы.
     */
    void toggleIndicator();
    /**
     * @brief Открытие окна настроек.
     */
    void openSettingsWindow();
    /**
     * @brief Открытие окна ввода параметров.
     */
    void openInputWindow();

public slots:
    /**
     * @brief Применяет темную тему интерфейса.
     */
    void applyDarkTheme();
    /**
     * @brief Применяет светлую тему интерфейса.
     */
    void applyLightTheme();
    /**
     * @brief Принимает новые данные о температуре, влажности и давлении.
     * @param tData Новое значение температуры.
     * @param hData Новое значение влажности.
     * @param pData Новое значение давления.
     */
    void acceptNewData(double tData, double hData, double pData);
    /**
     * @brief Принимает настройки для температуры и давления.
     * @param tempId Идентификатор единицы измерения температуры.
     * @param presId Идентификатор единицы измерения давления.
     */
    void acceptSettings(int tempId, int presId);

private:
    QWidget *centralWidget; ///< Основной виджет окна

    QVBoxLayout *mainLayout; ///< Главная компоновка элементов
    QHBoxLayout *dataLayout; ///< Компоновка данных температуры, влажности и давления
    double *temperature; ///< Указатель на значение температуры
    double *humidity; ///< Указатель на значение влажности
    double *pressure; ///< Указатель на значение давления
    int *hGateDir; ///< Горизонтальное направление воздушного потока
    int *vGateDir; ///< Вертикальное направление воздушного потока
    TemperatureUnit currentTempUnit; ///< Текущая единица измерения температуры
    PressureUnit currentPresUnit; ///< Текущая единица измерения давления
    Theme currentTheme; ///< Текущая тема интерфейса

    // Графические элементы
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsRectItem *thermometer;
    QGraphicsRectItem *humidityScale;
    QGraphicsRectItem *pressureScale;
    QGraphicsRectItem *mercuryLevel;
    QGraphicsRectItem *humidityLevel;
    QGraphicsRectItem *pressureLevel;
    QGraphicsTextItem *temperatureText;
    QGraphicsTextItem *humidityText;
    QGraphicsTextItem *pressureText;

    QGraphicsLineItem *hArrow; ///< Горизонтальное направление воздушного потока визуализация
    QGraphicsLineItem *hStaticArrow; ///< Горизонтальное направление воздушного потока визуализация
    QGraphicsLineItem *hStaticArrow2; ///< Горизонтальное направление воздушного потока визуализация
    QGraphicsPathItem *hAngleArc; ///< Горизонтальное направление воздушного потока угол
    QGraphicsLineItem *vArrow; ///< Вертикальное направление воздушного потока визуализация
    QGraphicsLineItem *vStaticArrow; ///< Вертикальное направление воздушного потока визуализация
    QGraphicsLineItem *vStaticArrow2; ///< Вертикальное направление воздушного потока визуализация
    QGraphicsTextItem *hAirText;
    QGraphicsTextItem *vAirText;

    void setTemp();
    void setHum();
    void setPres();

    QHBoxLayout *buttonsLayout;
    QPushButton *onOffButton;

    QVBoxLayout *changeTempLayout;
    QPushButton *tempUp;
    QPushButton *tempDown;
    
    QLabel *airBttnsLabel;
    QGridLayout *airLayout;
    QPushButton *airUp;
    QPushButton *airDown;
    QPushButton *airRight;
    QPushButton *airLeft;

    QPushButton *openSettings;
    QPushButton *openInput;

    Settings *settingsWindow = nullptr;
    CoolInput *inputWindow = nullptr;

    QLabel *onOffLabel;
    QMovie *airBlades;
    bool isOn = false;

    QString getTemperatureScaleByUnitId(TemperatureUnit id);
    TemperatureUnit getTemperatureUnitByScale(QString scale);
    void temperatureUp();
    void temperatureDown();

    void offSystem();

    PressureUnit getPressureUnitByScale(QString scale);
    QString getPressureScaleByUnitId(PressureUnit id);

    QString getThemeById(Theme id);
    Theme getThemeUnitByName(QString theme);
    void setCurrentTheme();

    void recalculateTemp(TemperatureUnit from, TemperatureUnit to);
    void recalculatePres(PressureUnit from, PressureUnit to);

    void addAirUp();
    void addAirDown();
    void addAirLeft();
    void addAirRight();
    int getMinHDir();
    int getMaxHDir();
    int getMinVDir();
    int getMaxVDir();

    void saveSettings(const QString &filePath);
    void loadSettings(const QString &filePath);
    void setBaseSettings();

    double getMinTempForCurrentUnit();
    double getMaxTempForCurrentUnit();
    double getMinPresForCurrentUnit();
    double getMaxPresForCurrentUnit();

    void setHumRange();

    void updateHArrow();
    void updateVArrow();

    QString getLockStyle();
};

#endif