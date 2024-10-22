#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>

class Settings : public QDialog 
{
        Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    void setActiveTempUnit(int id);
    void setActivePresUnit(int id);
signals:
    void darkThemeSelected();
    void lightThemeSelected();
    void confirmSettings(int tempId, int presId);

private:
    QVBoxLayout *mainLayout;

    QVBoxLayout *tempLayout;
    QLabel *tempLabel;
    QButtonGroup *tempGroup;
    QRadioButton *cels;
    QRadioButton *far;
    QRadioButton *kelv;

    QVBoxLayout *presLayout;
    QLabel *presLabel;
    QButtonGroup *presGroup;
    QRadioButton *pa;
    QRadioButton *mmrtst;

    QHBoxLayout *themeLayout;
    QLabel *themeLabel;
    QPushButton *white;
    QPushButton *black;

    QHBoxLayout *confirmLayout;
    QPushButton *confirmButton;
};

#endif