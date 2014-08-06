#include "mainwindow.h"
#include "ui_mainwindow.h"


//-------------------------------���߹㲥--------------------------------------------
void MainWindow::SendBroadcast(QString Address,qint32 type)
{
    qDebug(__func__);
    QByteArray data;
    QDataStream out(&data, QIODevice::ReadWrite);

    PersonLoginInfo info(HostName,HostIP);
    FileInformation fileInfo;
     out<<type;
     out<<fileInfo;
     out<<info;


    qint64 v = udpsocket->writeDatagram(data,data.length() ,QHostAddress(Address),port);
    qDebug()<< "send::" << v;


}

void MainWindow::RecvBroadcastInfo()
{
     qDebug(__func__);
    while(udpsocket->hasPendingDatagrams())
    {
        QByteArray data;
        int type;

        QDataStream in(&data, QIODevice::ReadOnly);


        data.resize(udpsocket->pendingDatagramSize());
        udpsocket->readDatagram(data.data(), data.size());



        in>>type;
        in>>rfileInfo;
        in>>rfriendInfo;

        if(type == 4 || type == 5)
        {
            qDebug("ddddddddddddddddddddddddddddd");
            RecvFileInfo(rfileInfo,type);
            return;
        }
        if(type == 6)
        {

            SysTrayIconTwinkle();

        }

        qDebug()<<"Recv::"<<rfriendInfo.logHostIp;
        qDebug()<<"Recv::"<<type;

        for(vector<PersonLoginInfo>::iterator it = LoginInfo.begin();
                   it!=LoginInfo.end(); ++it)
        {
             if(it->logHostIp == rfriendInfo.logHostIp)
             {
                 if(type == 1)
                 {
                     LoginInfo.erase(it);
                     return;
                 }
                 else if(type == 3)
                 {
                     LoginInfo.erase(it);
                     LoginInfo.push_back(rfriendInfo);
                     return;
                 }
                 else
                 {
                     return;
                 }
             }
        }

        if(type != 1)   //�ж��Ƿ�Ϊ���߹㲥
        {
            list->AddBuddy(rfriendInfo);
            QString name = rfriendInfo.logInfo.userName == "" ? rfriendInfo.logHostName : rfriendInfo.logInfo.userName;
            emit initMsg(name,rfriendInfo.logHostIp);
            LoginInfo.push_back(rfriendInfo);
            SendBroadcast(rfriendInfo.logHostIp,2);
        }
    }
}


void MainWindow::RecvFileInfo(FileInformation info,int type)
{
      qDebug(__func__);


      if(type == 4)
         filelist.push_back(info);
      else if(type == 5)
      {
           qDebug("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
           PersonLoginInfo *pinfo = new PersonLoginInfo(info.ServerName,info.ServerIP);
           ChatDialog *share = new ChatDialog(pinfo,info);
           share->fileName = info.filePath;
           share->startClient();

      }



      /*  while(udpsocket->hasPendingDatagrams())
        {
            qDebug() << "cmcmcmcm";
            qint16 type;
            QString IP;
            FileInformation value;
            qDebug() << filelist.size();
            QList<FileInformation>::iterator File  = filelist.begin();
            udpcli->receiveMessage(uudpsocket,type,IP,value);

            if(type == 4)//��Ϊ�ͻ��˽��յ����˵���Ϣ�����ļ���Ϣ���µ��Լ���tablewidget
            {
                qDebug() << type;
                if(IP != "222.24.74.30")
                 {
                  //  filelist.push_back(value);
                    qDebug()<<value.fileName;
                    qDebug()<<value.fileSize;
                    qDebug()<<value.fileType;
                 }

            }
            else if(type == 5)//��Ϊ�������յ����˵�����������Ϣ��Ȼ������Լ��������Ƿ��������ļ���Ȼ����ļ�������
               {
                qDebug() << "ok";
                if(!filelist.isEmpty())
                {
                    while(value.fileName == (*File).fileName && File != filelist.end())
                    {
                        ++File;
                        qDebug() << "okk";
                         //�ҵ��ˣ�����һ���źŸ������ļ�����ʼ�����ļ�
                        qDebug() << value.fileload;

                        file = new ChatDialog();
                        file.setfileload(value.fileload);
                        file.setip(value.ip);
                    }
                    qDebug() << "NO";
                }
               }

        }*/


}

