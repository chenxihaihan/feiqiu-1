#include "chatdialog.h"
#include "ui_chatdialog.h"

//-------------------------------------��Ϣ����------------------------------------------
void ChatDialog::on_SendMessage_clicked()
{
    qDebug(__func__);
    if(!chatMark)
    {
        SystemInformation sysinfo;
        PersonLoginInfo *logInfo = new PersonLoginInfo(sysinfo.getHostName(),sysinfo.getHostIP());

        logInfo->firstMsg = ui->MessageSend->toPlainText();
        noticeMsg = new UdpConnection();
        noticeMsg->sendMessage(6,*logInfo,QHostAddress(pb->logHostIp),6666);
    }

        if(ui->MessageSend->toPlainText() == "")
        {
           ui->MessageSend->setText(tr("��ϢΪ��!"));
        }
        else
        {
            QString msg = ui->MessageSend->toPlainText();
            QString addr = pb->logHostIp;
            udpService->sendMessage(3,QHostAddress(addr),msg,7777);
            qDebug()<<msg;
            DisplayMessage(0,msg);
            ui->MessageSend->clear();

        }
}

void ChatDialog::receiveMessage()
{
    while(receive->hasPendingDatagrams())
    {
        qint16 type;
        QString IP;
        QString value;
        udpService->receiveMessage(receive,type,IP,value);
        if(type == 1)  //�ͻ��˷��������ļ�������Ϣ
        {
            qDebug("file22222222222222222222222222222222222");
            QString msg = QString(tr("��%1 ������ %2�������...")).arg(IP).arg(value);
            ui->ShowMessage->append(msg);
            ui->stackedWidget->setCurrentIndex(2);

        }
        else if(type == 0)  //�������˷�������ȷ����Ϣ
        {
            qDebug()<<"connectToHost::"<<pb->logHostIp;
            tcpClient.connectToHost(pb->logHostIp,4444);
        }
        else if(type == 2)  //�������ܾ���Ϣ
        {
             QString msg = QString(tr("��%1�ܾ��������ļ���������...")).arg(IP);
             ui->ShowMessage->append(msg);
             ui->stackedWidget->setCurrentIndex(0);
        }
        else if(type == 3)
        {
            if(value == "OK" )
            {
              if(chatMark == false)
              {
                  chatMark = true;
                  UdpConnection *noticeMsg = new UdpConnection();
                  noticeMsg->sendMessage(3,QHostAddress(IP),"OK",7777);
              }

            }
            else
            {
                DisplayMessage(1,value);
            }

        }
        else if(type == 4)
        {
            qDebug()<<"shakexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
            Counter=0;
            curPos=this->pos();
            M_Timer->start();
            QString msg = tr("��%1����������һ�����ڶ�����").arg(IP);
            ui->ShowMessage->append(msg);

        }

    }
}


#include <QDateTime>
void ChatDialog::DisplayMessage(int flag,QString value)
{
   // ui->ShowMessage->setText(value);

    QString ip;
    QColor crl;
    if(flag == 0){
        ip = SystemInformation().getHostIP();
        crl = QColor(255,0,0);
    }else if(flag == 1){
        ip =pb->logHostIp;
        crl = QColor(0,0,255);
    }

    QString time = QDateTime::currentDateTime().toString("hh:mm:ss ap");
    ui->ShowMessage->setTextColor(Qt::blue);
    QString title = ip + "   "+time;
    ui->ShowMessage->append(title);
    ui->ShowMessage->setTextColor(crl);
    ui->ShowMessage->append(value);
}

