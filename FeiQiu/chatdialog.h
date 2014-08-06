#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

#include "PersonBuddyInfo.h"
#include "PersonLoginInfo.h"
#include "UdpConnection.h"
#include "chatlogging.h"
#include "mywidget.h"

#include "SystemInfo.h"

#include <cstdlib>
#include <ctime>
#include <QUdpSocket>


namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

/*
public:
   static ChatDialog* Instance(PersonLoginInfo *pb)
    {
        if(_instance == 0)
        {
            _instance = new ChatDialog(pb);
        }
        return _instance;
    }
private:
    static ChatDialog* _instance;

protected:
    explicit ChatDialog(PersonLoginInfo *pb,QWidget *parent=0);
*/



public:
    ~ChatDialog();
    explicit ChatDialog(PersonLoginInfo *pb,FileInformation info =FileInformation(),QWidget *parent=0);

private:
    Ui::ChatDialog *ui;
    PersonLoginInfo *pb;
    bool	m_MousePressed;
    QPoint	m_MousePos;
    QPoint	m_WindowPos;


private slots:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void minWindows();
    void closeWindows();
    void on_SendMessage_clicked();

//----------------���ڶ���-------------------------
public:
    void shakeStart();
private:
    QDialog *dialog;
    QTimer* M_Timer;
    int Counter;
    QPoint curPos;

    enum{MaxLimitTimes=500};//��������
    enum{MaxLimitSpace=20};//��������
    enum{ShakeSpeed=1000}; //�����ٶ�

private slots:
    void S_TimeOut();
    void slotShakeWindow();

//-----------��������------------------------
private slots:
    void receiveMessage();
    void on_sendfile_clicked();

private:
    UdpConnection *udpService;
    UdpConnection *noticeMsg;
    QUdpSocket *receive;
    bool chatMark;
public:
    void DisplayMessage(int,QString);

//----------�ļ�����--------------------------
private:
    qint64 TotalBytes; //�ܹ��跢�͵��ֽ���
    QFile *localFile; //�������ļ�
public:
    QString fileName; //�������ļ����ļ���

//��������
private:
    QTcpServer tcpServer; //�������׽���
    QTcpSocket *tcpServerConnection; //���Ӻ���������ص��׽���
    qint64 bytesReceived; //�ѽ����ֽ���
    qint64 fileNameSize; //�������ļ����ֽ���
    QByteArray inBlock;

public slots:
    void startServer();
    void refuse();
    void acceptConnection();
    void updateServerProgress();


//�ͻ���
private:
    QTcpSocket tcpClient; //�ͻ����׽���
    qint64 bytesWritten; //�ѷ����ֽ���
    qint64 bytesToWrite; //�������ֽ���
    qint64 loadSize; //����ʼ��Ϊһ��4Kb�ĳ���
    QByteArray outBlock; //����һ�η��͵�����

public slots:
    void startClient();
    void startTransfer();
    void updateClientProgress(qint64 numBytes);
    void openFile();

    //----------------���������¼-----------
    public:
        bool saveFile(QString &fileName,PersonLoginInfo *pb);
        void openChatSave();
    private:
        chatLogging *Chatlog;

    private slots:
        void openChat();
        void on_openChatButton_clicked();
        void on_CutPic_clicked();
};

#endif // CHATDIALOG_H
