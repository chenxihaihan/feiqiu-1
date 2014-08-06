#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QUdpSocket>
#include "SystemInfo.h"
#include "UDP_connection.h"
#include <vector>
#include <QInputDialog>
#include <QColorDialog>
#include <QFontDialog>

#include "PersonBuddyList.h"
#include "PersonBuddyInfo.h"
#include "PersonLoginInfo.h"
#include "userinfopage.h"
#include "chatdialog.h"
#include "work.h"
#include "searchdialog.h"
#include "sharefile.h"
#include "mysharefile.h"
#include "FileInformation.h"
#include "app.h"
#include "applicationcenter.h"
#include "music.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
/*
public:
    ~MainWindow();
    static MainWindow* Instance()
    {
        if(_instance == 0)
        {
            _instance = new MainWindow;
        }
        return _instance;
    }

protected:
    explicit MainWindow(QWidget *parent = 0);

private:
    static MainWindow* _instance;

*/

public:
    ~MainWindow();
    explicit MainWindow(QWidget *parent = 0);

private:
      //QInputDialog input;
     // QColorDialog color;
     //QFontDialog font;
     // QMessageBox box;


private slots:
    void on_Window_close_clicked();
    void slotUserInfoEdit();

private:
    Ui::MainWindow *ui;
    Chat_Connection_Ser *ser;
    PersonBuddyList *list;
//------------------����������Ϣ--------------------------
private:
    vector<QString> isChat;
private:
    PersonLoginInfo rfriendInfo;
    FileInformation rfileInfo;
//-------------������Ϣ--------------------------------
private:
    SystemInformation *HostInfo;
    QString HostName;
    QString HostIP;

//-------------����϶�����-----------------------------
private:
    bool	m_MousePressed;
    QPoint	m_MousePos;
    QPoint	m_WindowPos;

private:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

//--------------ϵͳ����-------------------------------
public:
    void SysTrayIconTwinkle();   //������˸����


private:
    QSystemTrayIcon *trayicon;
    QMenu *trayiconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QTimer  *TrayIconTimers;

private slots:
    //���һ���ۺ���������Ӧ������ͼ�꼰���̲˵��ϵ����������
    void onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
    void on_action_quit_triggered();
    //������˸
    void Maction(QSystemTrayIcon::ActivationReason wch);
    void changeIco();
private:
    void SystemTrayShow();
    void showMessage(char *msg);
    void createActions();
    void createTrayIcon();
    void closeEvent(QCloseEvent *event);
//------------���߹㲥----------------------------------
public slots:
      void RecvBroadcastInfo();
public:
      void SendBroadcast(QString,qint32);
private:
    QUdpSocket *udpsocket;
    int port;
signals:
    void initMsg(QString title,QString content);


//------------�����û���Ϣ�洢����------------------------------
private:
    vector<PersonLoginInfo> LoginInfo;
//--------------�ļ�����----------------------------
private:
    vector<FileInformation> filelist;
public:
    void RecvFileInfo(FileInformation,int);

//------------�����б�ʱˢ��---------------------------------
private:
    QTimer *testTimer;
private slots:
    void ReloadBuddyList();

//------------��������------------------------------------
public:
    class SearchDialog *searchdialog;
    bool Have;
    QString lineedit;
private slots:
    void ready_to_search_2();
    void ready_to_search();
//---------------С����----------------------------------
    void on_calendar_clicked();
    void on_about_clicked();
    void on_find_clicked();
    void on_setting_clicked();
    void on_refresh_clicked();
    void on_fileShare_clicked();
    void on_Application_clicked();


//-------------Ӧ������----------
    void on_music_clicked();

private:
        ApplicationCenter *app;



};

#endif // MAINWINDOW_H
