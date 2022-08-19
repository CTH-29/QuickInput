#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle( "QuickInput" );
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMinMaxButtonsHint);
    this->setWindowIcon(QIcon(":/img/icon.png"));

    clipboard = QApplication::clipboard();

    settings = new QSettings("QuickInput.ini", QSettings::IniFormat);

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL( mapped(int) ), this, SLOT( ButtonClick(int) ));

    connect(ui->actionshow_hide_edit, SIGNAL(triggered()), this, SLOT(ShowHideEdit()));
    connect(ui->actionAlways_on_Top, SIGNAL(triggered()), this, SLOT(AlwaysOntop()));
    connect(ui->actionAdd_Tab, SIGNAL(triggered()), this, SLOT(AddTab()));
    connect(ui->actionAdd_Button, SIGNAL(triggered()), this, SLOT(AddButton()));
    connect(ui->actionSave_Button, SIGNAL(triggered()), this, SLOT(SaveButton()));
    connect(ui->actionHow_to_Use, SIGNAL(triggered()), this, SLOT(HowToUse()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(About()));

    this->ui->groupBox->setEnabled(false);

    LoadAll();

    this->ui->tabWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadAll(void)
{
    int tabIndex = 0;
    int btnIndex = 0;
    int btnCount = 0;
    while(1)
    {
        if(this->settings->contains("tab"+QString::number(tabIndex)+"/key_count"))
        {
            btnCount = this->settings->value("tab"+QString::number(tabIndex)+"/key_count").toUInt();
            qDebug()<<"Load Tab: tabIndex = "<< tabIndex<<" btnCount = "<< btnCount;

            CreateTab(tabIndex);

            for (btnIndex = 0; btnIndex <  btnCount; btnIndex++) {
                UpdateBtn(CreateBtn(tabIndex,btnIndex));
            }
            tabIndex++;
        }
        else
        {
            break;
        }
    }
    if(tabIndex == 0 )
        CreateTab(tabIndex);
}


QWidget *MainWindow::CreateTab(int index)
{
    QWidget *tab;

    qDebug()<<"CreateTab: tabIndex = "<< index;

    int tabCnt = this->ui->tabWidget->count();

    if( tabCnt != 0 && index  < tabCnt)
    {
        tab = this->ui->tabWidget->findChild<QWidget*>("tab"+QString::number(index));
        if(tab)
        {
            qDebug()<<"CreateTab: already exist = "<< index;
            return tab;
        }
    }
    QString strn = QString::number(index);

    tab = new QWidget(this->ui->tabWidget);
    tab->setObjectName(QString("tab"+strn));

    QVBoxLayout *verticalLayout = new QVBoxLayout(tab);
    verticalLayout->setObjectName(QString("verticalLayout"+strn));

    QScrollArea *scrollArea = new QScrollArea(tab);
    scrollArea->setObjectName(QString("scrollArea"+strn));
    scrollArea->setWidgetResizable(true);

    QWidget * scrollAreaWidgetContents = new QWidget(scrollArea);
    scrollAreaWidgetContents->setObjectName(QString("scrollAreaWidgetContents"+strn));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 639, 320));

    QGridLayout *gridLayout = new QGridLayout(scrollAreaWidgetContents);
    gridLayout->setObjectName(QString("BtngridLayout"+strn));

    scrollArea->setWidget(scrollAreaWidgetContents);
    verticalLayout->addWidget(scrollArea);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 65535, 0, 1, 1);

    this->ui->tabWidget->addTab(tab, tab->objectName());

    return tab;
}


void MainWindow::AddTab(void)
{
    QWidget *tab = CreateTab(this->ui->tabWidget->count());

    this->ui->tabWidget->setCurrentWidget(tab);
}


QPushButton *MainWindow::CreateBtn(int tabIndex, int btnIndex)
{
    qDebug()<<"CreateBtn: tabIndex ="<<tabIndex << " btnIndex ="<<btnIndex;

    QPushButton *bt = nullptr;
    if( tabIndex  > this->ui->tabWidget->count() )
    {
        qDebug()<<"return null: tabIndex ="<<tabIndex << " tabWidget->count"<< this->ui->tabWidget->count();
        return bt;
    }

    bt = this->ui->tabWidget->findChild<QPushButton*>( "tab"+QString::number(tabIndex) +"/key"+QString::number(btnIndex) );

    if(bt != nullptr)
    {
        return bt;
    }

    QWidget *scrollAreaWidgetContents = this->ui->tabWidget->findChild<QWidget*>(QString("scrollAreaWidgetContents"+QString::number(tabIndex)));
    QGridLayout *gridLayout = this->ui->tabWidget->findChild<QGridLayout*>(QString("BtngridLayout"+QString::number(tabIndex)));

    if(scrollAreaWidgetContents == NULL)
    {
        qDebug()<<"scrollAreaWidgetContents == NULL";
        return NULL;
    }
    if(gridLayout == NULL)
    {
        qDebug()<<"gridLayout == NULL";
        return NULL;
    }

    bt = new QPushButton(scrollAreaWidgetContents);

    bt->setObjectName("tab"+QString::number(tabIndex) +"key"+QString::number(btnIndex));

    gridLayout->addWidget(bt, btnIndex, 0, 1, 1);

    connect(bt, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping (bt, btnIndex);

    return bt;
}

void MainWindow::UpdateBtn(QPushButton *btn)
{
    if(btn==nullptr)
        return;

    QString tabname = QString(btn->objectName().split("key")[0]);

    QString KeyName = tabname+"/"+btn->objectName()+"_name";
    QString ValueName = tabname+"/"+btn->objectName()+"_value";

    qDebug()<<"button update: "<<"KeyName = "<<KeyName<<"ValueName = "<<ValueName;

    if( this->settings->contains(KeyName) )
    {
        QString keyStr = this->settings->value(KeyName).toString();
        btn->setText(keyStr);
    }

    if( this->settings->contains(ValueName) )
    {
        QString valueStr = this->settings->value(ValueName).toString();
        btn->setStatusTip(valueStr);
    }
}

void MainWindow::AddButton(void)
{
    QString keycount_str = "tab"+ QString::number(this->ui->tabWidget->currentIndex())+"/key_count";

    int keycount = this->settings->value(keycount_str).toUInt();

    CreateBtn(this->ui->tabWidget->currentIndex(),keycount);

    ButtonClick(keycount);

    this->settings->setValue(keycount_str,keycount+1);
}

void MainWindow::ShowHideEdit(void)
{
    QGroupBox *edit = this->ui->centralwidget->findChild<QGroupBox*>("groupBox");
    edit->setVisible(!edit->isVisible());
}

void MainWindow::AlwaysOntop()
{
    this->setWindowFlags(this->windowFlags() ^ Qt::WindowStaysOnTopHint);
    this->show();
}

void MainWindow::ButtonClick(int CurBtnIndex)
{
    this->ui->groupBox->setEnabled(true);

    QPushButton *btn = this->ui->tabWidget->findChild<QPushButton*>("tab"+QString::number(this->ui->tabWidget->currentIndex()) +"key"+QString::number(CurBtnIndex));
    if(btn == NULL)
    {
        qDebug()<<"ButtonClick:btn is NULL"<<endl;
        return;
    }

    CurBtn = btn;
    UpdateBtn(CurBtn);

    this->ui->lineEdit->setText(CurBtn->text());
    this->ui->textEdit->setText(CurBtn->statusTip());
    if(! CurBtn->statusTip().isEmpty())
    {
        clipboard->setText(CurBtn->statusTip());
        this->ui->statusbar->showMessage("copied",1000);
    }
    else
    {
        QGroupBox *edit = this->ui->centralwidget->findChild<QGroupBox*>("groupBox");
        edit->setVisible(true);
    }

}

void MainWindow::SaveButton(void)
{
    if(! this->ui->groupBox->isEnabled())
        return;

    QString tabname = "tab"+QString::number(this->ui->tabWidget->currentIndex());
    QString KeyName = tabname+"/"+CurBtn->objectName()+"_name";
    QString ValueName = tabname+"/"+CurBtn->objectName()+"_value";

    this->settings->setValue(KeyName,this->ui->lineEdit->text());
    this->settings->setValue(ValueName,this->ui->textEdit->toPlainText());

    UpdateBtn(CurBtn);

    this->ui->statusbar->showMessage("saved",1000);
}

void MainWindow::HowToUse(void)
{
    QMessageBox::information(this,"How to use",
                            "将常用的文本存入到按钮里，然后点击按钮，复制到剪切板\n"
                            "\n"
                            "ctrl+T: 新增一个tab\n"
                            "ctrl+B: 新增一个按钮\n"
                            "ctrl+S: 将文本保存在按钮里\n"
                             "\n"
                            "删除的话，在 QuickInput.ini 里自行删除吧 :)\n"
                         ,QMessageBox::Ok);
}
void MainWindow::About(void)
{
    QString link = QString("https://github.com/CTH-29/QuickInput");
    QMessageBox::information(this,"about",
                            "项目GitHub:(ctrl+v试试)\n"
                            "致敬https://github.com/Savior2016/commander\n"
                         ,QMessageBox::Ok);
    this->clipboard->setText(link);
}

void MainWindow::ResizeButton(void)
{
    ;
}


