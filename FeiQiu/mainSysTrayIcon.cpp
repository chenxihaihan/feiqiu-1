#include "mainwindow.h"
#include "ui_mainwindow.h"

//--------------------ϵͳ������ʾ---------------------------------------------------
void MainWindow::SystemTrayShow()
{

    trayicon = new QSystemTrayIcon(this);
     //����QIcon���󣬲�����ͼ����Դ��ֵΪ��Ŀ����Դ�ļ���ͼ��ĵ�ַ
     QIcon icon(":/img/tuopan.jpg");
      createActions();
      trayiconMenu = new QMenu(this);
       //Ϊ���̲˵���Ӳ˵���
      trayiconMenu->addAction(minimizeAction);
      //trayiconMenu->addAction(maximizeAction);
      trayiconMenu->addAction(restoreAction);

      //Ϊ���̲˵���ӷָ���
      trayiconMenu->addSeparator();

      trayiconMenu->addAction(quitAction);

      //��������QIcon������Ϊϵͳ����ͼ��
       trayicon->setIcon(icon);

       //��ʾ����ͼ��
       trayicon->show();

       //����ϵͳ������ʾ
      // trayicon->setToolTip(tr("���̲���"));

       //�������˵���Ϊϵͳ���̲˵�
       trayicon->setContextMenu(trayiconMenu);

       //��ϵͳ������ʾ������Ϣ��ʾ  ���̲���
       trayicon->showMessage(tr("hello"), tr("������"), QSystemTrayIcon::Information, 3000);

       //Ϊϵͳ���̰�˫���źŵĲ� ��ͼ�꼤��ʱ
       connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
       connect(ui->Window_MinSize,SIGNAL(clicked()),this,SLOT(on_action_quit_triggered()));
}

void MainWindow::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
  switch(reason)
  {
  //����
  //case QSystemTrayIcon::Trigger:

  //˫��
  case QSystemTrayIcon::DoubleClick:

      //�ָ�������ʾ
      this->setWindowState(Qt::WindowActive);
      this->show();
      break;

  default:
      break;
  }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if(trayicon->isVisible())
  {
      hide();
      event->ignore();
  }
}


void MainWindow::on_action_quit_triggered()
{
  this->hide();
  this->close();
  trayicon->showMessage(tr("��������"), tr("����һ�ԭ~~"), QSystemTrayIcon::Information, 3000);
}

void MainWindow::createActions()
{
     minimizeAction = new QAction(tr("��С�� (&I)"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    // maximizeAction = new QAction(tr("��� (&X)"), this);
     //connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
     restoreAction = new QAction(tr("��ԭ (&R)"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
     quitAction = new QAction(tr("�˳� (&Q)"), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}


//----------------��������Ϣʱ����----------------
void MainWindow::SysTrayIconTwinkle()
{
    TrayIconTimers = new QTimer(this);
    TrayIconTimers->setSingleShot(false);//����ѭ��
    TrayIconTimers->setInterval(500);//����ʱ����
    TrayIconTimers->start();
    connect(TrayIconTimers,SIGNAL(timeout()),this,SLOT(changeIco()));
    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                         this, SLOT(Maction(QSystemTrayIcon::ActivationReason)));
}





void MainWindow::changeIco()
{
    static int m = 0;
    m++;
    /*if(m==20)
    {
        times->stop();
    }*/
    if(m%2==0)
    {
        trayicon->setIcon( QIcon(":/img/xinfeng.jpg") );
    }
    else
    {
        trayicon->setIcon( QIcon(":/img/tuopan.jpg" ));
     }
}

void MainWindow::Maction(QSystemTrayIcon::ActivationReason wch)
{
    ChatDialog *chat;
    UdpConnection *noticeMsg;
    switch(wch)
    {
        case QSystemTrayIcon::Trigger:
        TrayIconTimers->stop();
        trayicon->setIcon( QIcon(":/img/tuopan.jpg") );

        chat = new ChatDialog(&rfriendInfo);
        chat->show();
        chat->DisplayMessage(1,rfriendInfo.firstMsg);
        noticeMsg = new UdpConnection();
        noticeMsg->sendMessage(3,QHostAddress(rfriendInfo.logHostIp),"OK",7777);
        qDebug()<<"Maction1111111111111111111111"<<rfriendInfo.logHostIp;
        break;//���û�����ʱ ������ͼ��ֹͣ��˸

    default:;
    }

}
