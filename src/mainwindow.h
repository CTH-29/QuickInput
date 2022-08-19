#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include <QMessageBox>
#include <QSignalMapper>
#include <QList>
#include <QSettings>
#include <QClipboard>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton *CurBtn;
    QSettings *settings;
    QClipboard *clipboard;

    QWidget *CreateTab(int index);


    QPushButton *CreateBtn(int tabIndex, int btnIndex);
    void UpdateBtn(QPushButton *btn);

    void LoadAll(void);
    void ResizeButton(void);



public slots:

    void ShowHideEdit(void);
    void AlwaysOntop(void);

    void AddTab(void);
    void AddButton(void);
    void SaveButton(void);

    void HowToUse(void);
    void About(void);

    void ButtonClick(int CurBtnIndex);


private:
    Ui::MainWindow *ui;
    QSignalMapper *signalMapper;

};
#endif // MAINWINDOW_H
