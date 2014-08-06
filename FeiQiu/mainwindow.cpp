#include "mainwindow.h"
#include "ui_mainwindow.h"



#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug(__func__);


//------------��������ʽ�趨------------------------------------
    this->setWindowFlags(Qt::FramelessWindowHint);  //ȥ��������
    this->setFixedSize( this->width(),this->height());//���ô��ڹ̶���С

    ui->Window_MinSize->setToolTip(tr("��С��"));
    ui->Window_close->setToolTip(tr("�ر�"));

    this->setWindowOpacity(0.9);//���ô���͸��


        this->setAutoFillBackground(true);
        QPalette p =this->palette();
        p.setColor(QPalette::Window,QColor(52,203,147));
        //p.setBrush(QPalette::Window,brush);
        this->setPalette(p);



//--------�û����ϱ༭ҳ------------------------

    connect(ui->head, SIGNAL(clicked()),this,SLOT(slotUserInfoEdit()));
//------------��������-------------------------
    connect(ui->search,SIGNAL(returnPressed()),this,SLOT(ready_to_search_2()));

    connect(ui->search,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(ready_to_search()));
    searchdialog = new SearchDialog();
    Have = false;

//--------��ȡ������������IP---------------------
    HostInfo = new SystemInformation;
    HostName = HostInfo->getHostName();
    HostIP = HostInfo->getHostIP();
    ui->Username->setText(HostName);


//--------�û�ͷ���ʼ����-----------------------

   // QIcon pixmap("img/head.bmp");
   // QIcon icon(pixmap);
    //ui->head->resize(pixmap.,pixmap.height());
   // ui->head->setIcon(icon);
   // ui->head->setIcon(pixmap);

//--------��ʾϵͳ����-----------------------------
    SystemTrayShow();

//--------�����б�--------------------------------
    list = new PersonBuddyList();
    list->setParent(ui->tab);
    ui->tab->graphicsEffect();
    list->setFixedWidth(ui->tab->width());
    list->setFixedHeight(ui->tab->height());
    list->show();

//-------------���߹㲥---------------------------
    udpsocket = new QUdpSocket(this);
    port = 6666;
    connect(udpsocket, SIGNAL(readyRead()), this,SLOT(RecvBroadcastInfo()));
    udpsocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    this->SendBroadcast(QString("255.255.255.255"),0);
//-----------�ļ�������Ϣ-------------------------------
   // connect(udpsocket, SIGNAL(readyRead()), this,SLOT(RecvFileInfo()));

//-----------��ʼ���Լ�����Ϣ----------------------
    PersonLoginInfo plogInfo(HostName,HostIP);
    LoginInfo.push_back(plogInfo);
    list->AddBuddy(plogInfo);

    ui->head->setIcon(QIcon(plogInfo.logInfo.userHeadPath));
    QString name = plogInfo.logInfo.userName == "" ? HostName : plogInfo.logInfo.userName;
    ui->Username->setText(name);
    ui->userSign->setText(plogInfo.logInfo.userSign);

//-----------�����б�ʱˢ��----------------------
    testTimer = new QTimer(this);
    //����ʱ����ʱ�ź����(���ܺ���)��ϵ����
    connect(testTimer, SIGNAL(timeout()), this,SLOT(ReloadBuddyList()));
    //��ʼ���ж�ʱ������ʱʱ����Ϊ1000ms
    testTimer->start(5000);






}

MainWindow::~MainWindow()
{
    //-------------���߹㲥--------------------------------
    this->SendBroadcast(QString("255.255.255.255"),1);
    testTimer->stop();
    delete ui;
}
void MainWindow::on_Window_close_clicked()
{
    /*QMessageBox ms;
    ms.setIcon(QMessageBox::Warning);
    ms.setWindowTitle(tr("���棡"));
    ms.setText(tr("ȷ��Ҫ�˳���"));
    QIcon ico(":/img/tuopan.jpg");
    ms.setWindowIcon(ico);
    QPushButton *bt1 = ms.addButton(tr("ȷ��"),QMessageBox::AcceptRole);
    QPushButton *bt2 =  ms.addButton(tr("ȡ��"),QMessageBox::RejectRole);
    ms.exec();
    if(ms.clickedButton() == bt1)
    {
        this->close();
    }
    else if(ms.clickedButton() == bt2)
    {
        return;
    }
     */
    this->close();
}


//-----------�����б�ʱˢ��----------------------
void MainWindow::ReloadBuddyList()
{
   list->clear();
   list->AddGroup(false);

   for(vector<PersonLoginInfo>::iterator it = LoginInfo.begin();
              it!=LoginInfo.end(); ++it)
   {
       QString name = it->logInfo.userName == "" ? it->logHostName : it->logInfo.userName;
       list->AddBuddy(*it);
         if(it->logHostIp == HostIP)
         {
             ui->head->setIcon(QIcon(it->logInfo.userHeadPath));
             ui->Username->setText(name);
             ui->userSign->setText(it->logInfo.userSign);
         }
   }

}
//------------------------����϶�---------------------------------
void MainWindow::mousePressEvent(QMouseEvent *lpEvent)
{
    QMainWindow::mousePressEvent(lpEvent);
    if (lpEvent->button() == Qt::LeftButton)
    {
        m_WindowPos = this->pos();
        m_MousePos = lpEvent->globalPos();
        this->m_MousePressed = true;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *lpEvent)
{
    QMainWindow::mouseReleaseEvent(lpEvent);
    if (lpEvent->button() == Qt::LeftButton)
    {
        this->m_MousePressed = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *lpEvent)
{
    QMainWindow::mouseMoveEvent(lpEvent);
    if (m_MousePressed)
    {
        this->move(m_WindowPos + (lpEvent->globalPos() - m_MousePos));
    }

 //  this->setCursor(Qt::PointingHandCursor);

}

//-----------------------���Ѳ���--------------------------------------------------------
void MainWindow :: ready_to_search()
{
    QString ip,name;
    QString ss;
    lineedit = ui->search->text();
    if(lineedit.size() > 3)
    {
        qDebug()<<LoginInfo.size();
        for(int i = 0;i < LoginInfo.size();i++)
        {
            ip = LoginInfo[i].logHostIp;
            name = LoginInfo[i].logHostName;
            if(ip.contains(lineedit,Qt::CaseInsensitive))
                {
                ss = name+"       "+lineedit;
                    if(!Have)
                    {
                        searchdialog->additem(name,lineedit);
                        if(i == LoginInfo.size() - 1)
                        Have = true;
                    }
                    else
                    {
                        searchdialog->updateitem(name,lineedit);
                        qDebug("update now.........");
                            if(lineedit.size() > 8)
                            {
                                for(int i = 0;i < LoginInfo.size();i++)
                                {
                                    if(LoginInfo[i].logHostIp.contains(lineedit,Qt::CaseInsensitive))
                                    {
                                    searchdialog->deleteitem();
                                    searchdialog->additem(LoginInfo[i].logHostName,LoginInfo[i].logHostIp);
                                    }
                                }
                            }
                    }
                searchdialog->show();
                }
            else if(name.contains(lineedit,Qt::CaseInsensitive))
                {
                ss = name+"       "+lineedit;
                    if(!Have)
                    {
                        searchdialog->additem(lineedit,ip);
                        if(i == LoginInfo.size() - 1)
                        Have = true;
                    }
                    else
                    {
                        searchdialog->updateitem(lineedit,ip);
                        qDebug("update now.........");
                            if(lineedit.size() > 8)
                            {
                                for(int i = 0;i < LoginInfo.size();i++)
                                {
                                    if(LoginInfo[i].logHostIp.contains(lineedit,Qt::CaseInsensitive))
                                    {
                                    searchdialog->deleteitem();
                                    searchdialog->additem(LoginInfo[i].logHostName,LoginInfo[i].logHostIp);
                                    }
                                }
                            }
                    }
                searchdialog->show();
                }
                else
                {
                    searchdialog->deleteitem();
                    Have = false;
                }
        }
    }
    if(lineedit.size() < 3)
        searchdialog->deleteitem();
}

void MainWindow :: ready_to_search_2()
{
    QString str = ui->search->text();
    for(int i = 0;i < LoginInfo.size();i++)
    {
        if(str == LoginInfo[i].logHostIp || str == LoginInfo[i].logHostName || str == LoginInfo[i].logInfo.userName)
            {
            PersonLoginInfo *pLog = new PersonLoginInfo(LoginInfo[i].logHostName,LoginInfo[i].logHostIp);
            ChatDialog *chat = new ChatDialog(pLog);
            chat->show();
            }
    }
    ui->search->clear();
}

//----------------------�û���������ҳ----------------------------------------------------

void MainWindow::slotUserInfoEdit()
{

    UserInfoPage *page = new UserInfoPage;
    page->show();
}

void MainWindow::on_calendar_clicked()
{
    work *pw=new work;
    pw->show();
}


void MainWindow::on_about_clicked()
{

    QMessageBox::information(this, "About",
                             tr("����--������\n\t�鳤������\n\t��Ա���ſ� ������ ���� ���� ��ΰ ����� "),
                                    QMessageBox::Yes,
                                    QMessageBox::Yes);


}

void MainWindow::on_find_clicked()
{
         bool isOK;
         bool isFind = false;
 find:   QString str = QInputDialog::getText(this, "Find",
                                               tr("�������û�����IP��"),
                                               QLineEdit::Normal,
                                                "",
                                                &isOK);
    if(isOK)
    {
        for(int i = 0;i < LoginInfo.size();i++)
        {
            if(str == LoginInfo[i].logInfo.userName || str == LoginInfo[i].logHostIp ||
                    str == LoginInfo[i].logHostName)
            {
                isFind = true;
                PersonLoginInfo *pLog = new PersonLoginInfo(LoginInfo[i].logHostName,LoginInfo[i].logHostIp);
                ChatDialog *chat = new ChatDialog(pLog);
                chat->show();
                return;
            }
        }
        if(!isFind)
        {
            QMessageBox::information(this, tr("����"),
                                     tr("û���ҵ��ú��ѣ�"),
                                            QMessageBox::Yes,
                                            QMessageBox::Yes);
            goto find;
        }


    }
}

void MainWindow::on_setting_clicked()
{
    UserInfoPage *page = new UserInfoPage;
    page->show();
}

void MainWindow::on_refresh_clicked()
{
    list->clear();
    Sleep(2000);
    ReloadBuddyList();
    QMessageBox::information(this, tr("��ϲ"),
                             tr("ˢ�����"),
                             QMessageBox::Yes,
                             QMessageBox::Yes);
}

void MainWindow::on_fileShare_clicked()
{
    sharefile *shareWindow = new sharefile(&filelist);
    shareWindow->show();
}


void MainWindow::on_Application_clicked()
{
    app = new ApplicationCenter;
    app->show();
}

void MainWindow::on_music_clicked()
{
    music *mu= new music();
    mu->show();
}
