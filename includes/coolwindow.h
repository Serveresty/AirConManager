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
#include "settings.h"
#include "coolinputwindow.h"

class CoolWindow : public QMainWindow
{
    Q_OBJECT

public:
    CoolWindow(QWidget *parent = nullptr);
    ~CoolWindow();

    double GetTemperature();
    void SetTemperature(double temp);

    int GetHumidity();
    void SetHumidity(int hum);

    double GetPressure();
    void SetPressure(double pres);

    enum class TemperatureUnit {
        Celsius = 1,
        Fahrenheit,
        Kelvin
    };

    enum class PressureUnit {
        Pascal = 1,
        Mmhg
    };

    enum class Theme {
        Light = 1,
        Dark
    };

private slots:
    void toggleIndicator();
    void openSettingsWindow();
    void openInputWindow();

public slots:
    void applyDarkTheme();
    void applyLightTheme();
    void acceptNewData(double tData, double hData, double pData);
    void acceptSettings(int tempId, int presId);

private:
    QWidget *centralWidget;

    QVBoxLayout *mainLayout;
    QHBoxLayout *dataLayout;
    double *temperature;
    double *humidity;
    double *pressure;
    int *hGateDir;
    int *vGateDir;
    TemperatureUnit currentTempUnit;
    PressureUnit currentPresUnit;
    Theme currentTheme;

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
    void setTemp();
    void setHum();
    void setPres();

    QHBoxLayout *buttonsLayout;
    QPushButton *onOffButton;

    QVBoxLayout *changeTempLayout;
    QPushButton *tempUp;
    QPushButton *tempDown;
    
    QVBoxLayout *verticalAirLayout;
    QHBoxLayout *horizontalAirLayout;
    QPushButton *airUp;
    QPushButton *airDown;
    QPushButton *airRight;
    QPushButton *airLeft;

    QPushButton *openSettings;
    QPushButton *openInput;

    Settings *settingsWindow = nullptr;
    CoolInput *inputWindow = nullptr;

    QLabel *onOffLabel;
    bool isOn = false;

    QString getTemperatureScaleByUnitId(TemperatureUnit id);
    TemperatureUnit getTemperatureUnitByScale(QString scale);
    void temperatureUp();
    void temperatureDown();

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

    QString getLockStyle();
};

#endif