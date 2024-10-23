#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>

/**
 * @file settings.h
 * @brief Заголовочный файл для класса Settings.
 * 
 * Этот файл содержит объявление класса Settings,
 * который отвечает за создание и управление интерфейсом настроек.
 */
/**
 * @class Settings
 * @brief Класс для управления настройками приложения через графический интерфейс.
 * 
 * Обеспечивает функциональность управления шкалами данных, таких как
 * температура, влажность, давление, а так же изменение тем.
 */
class Settings : public QDialog 
{
        Q_OBJECT

public:
    /**
     * @brief Конструктор класса Settings.
     * 
     * Создаёт экземпляр окна настроек с элементами управления для выбора 
     * шкалы температуры, шкалы давления и темы оформления.
     * 
     * @param parent Указатель на родительский объект.
     */
    explicit Settings(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса Settings.
     * 
     * Освобождает ресурсы, используемые экземпляром класса.
     */
    ~Settings();

    /**
     * @brief Устанавливает активную шкалу температуры.
     * 
     * Выбирает радио-кнопку, соответствующую заданному идентификатору
     * шкалы температуры.
     * 
     * @param id Идентификатор шкалы температуры (1 - Цельсий, 2 - Фаренгейт, 3 - Кельвин).
     */
    void setActiveTempUnit(int id);

    /**
     * @brief Устанавливает активную шкалу давления.
     * 
     * Выбирает радио-кнопку, соответствующую заданному идентификатору
     * шкалы давления.
     * 
     * @param id Идентификатор шкалы давления (1 - Па, 2 - мм.рт.ст.).
     */
    void setActivePresUnit(int id);
signals:

    /**
     * @brief Сигнал, отправляемый при выборе тёмной темы.
     */
    void darkThemeSelected();

    /**
     * @brief Сигнал, отправляемый при выборе светлой темы.
     */
    void lightThemeSelected();

    /**
     * @brief Сигнал, отправляемый при подтверждении настроек.
     * 
     * @param tempId Идентификатор выбранной шкалы температуры.
     * @param presId Идентификатор выбранной шкалы давления.
     */
    void confirmSettings(int tempId, int presId);

private:
    QVBoxLayout *mainLayout; ///< Главная компоновка для размещения элементов управления.

    QVBoxLayout *tempLayout; ///< Компоновка для размещения элементов управления температурой.
    QLabel *tempLabel; ///< Метка для отображения текста "Шкала температуры".
    QButtonGroup *tempGroup; ///< Группа радиокнопок для выбора шкалы температуры.
    QRadioButton *cels; ///< Радиокнопка для выбора Цельсия.
    QRadioButton *far; ///< Радиокнопка для выбора Фаренгейта.
    QRadioButton *kelv; ///< Радиокнопка для выбора Кельвина.

    QVBoxLayout *presLayout; ///< Компоновка для размещения элементов управления давлением.
    QLabel *presLabel; ///< Метка для отображения текста "Шкала давления".
    QButtonGroup *presGroup; ///< Группа радиокнопок для выбора шкалы давления.
    QRadioButton *pa; ///< Радиокнопка для выбора Паскаля.
    QRadioButton *mmrtst; ///< Радиокнопка для выбора миллиметров ртутного столба.

    QHBoxLayout *themeLayout; ///< Компоновка для размещения элементов управления темой.
    QLabel *themeLabel; ///< Метка для отображения текста "Тема оформления".
    QPushButton *white; ///< Кнопка для выбора светлой темы.
    QPushButton *black; ///< Кнопка для выбора тёмной темы.

    QHBoxLayout *confirmLayout; ///< Компоновка для размещения кнопки подтверждения.
    QPushButton *confirmButton; ///< Кнопка для подтверждения выбранных настроек.
};

#endif