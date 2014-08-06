#include"chatdialog.h"
#include "ui_chatdialog.h"
#include"FileInformation.h"



void ChatDialog::on_sendfile_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}

//---------------------------��������-------------------------------
void ChatDialog::startServer()
{


    bytesReceived = 0;
    QString addr = pb->logHostIp;
    qDebug()<<"StartServer::fileName"<<fileName;
    udpService->sendMessage(0,QHostAddress(addr),fileName,7777);  //��ͻ��˷���ȷ����Ϣ

    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::Any,4444))
    {
        break;
    }


}

void ChatDialog::refuse()
{
    tcpServer.close();
    ui->stackedWidget->setCurrentIndex(0);
    QString msg = tr("����ܾ�������%1���ļ�%2��������!").arg(pb->logHostIp).arg(fileName);
    ui->ShowMessage->append(msg);

    udpService->sendMessage(2,QHostAddress(pb->logHostIp),fileName,7777);  //��ͻ��˷�����Ϣ


}

void ChatDialog::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();//���һ��TCP����
    connect(tcpServerConnection, SIGNAL(readyRead()),this, SLOT(updateServerProgress()));
    //connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
    //this, SLOT(displayError(QAbstractSocket::SocketError)));
    tcpServer.close();
}


void ChatDialog::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_3);

    if(bytesReceived <= sizeof(qint64)*2)
    {//������յ�������С��16���ֽڣ���ô�Ǹտ�ʼ��������
        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)&&(fileNameSize ==0))
        {//���������ܴ�С��Ϣ���ļ�����С��Ϣ
            in >> TotalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tcpServerConnection->bytesAvailable() >= fileNameSize)&&(fileNameSize !=0))
        {//�����ļ������������ļ�
            in >> fileName;
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly ))
            {
                QMessageBox::warning(this, tr("Ӧ�ó���"),
                tr("�޷���ȡ�ļ� %1:\n%2.").arg(fileName).arg(localFile->errorString()));
                return;
            }
       }
       else
       {
          return;
       }
    }

    qDebug()<<TotalBytes;

    if (bytesReceived < TotalBytes)
    {
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

        ui->ServerProgressBar->setMaximum(TotalBytes);
        ui->ServerProgressBar->setValue(bytesReceived);
        qDebug()<<bytesReceived;
        ui->ServerMsg->setText(tr("�ѽ��� %1KB").arg(bytesReceived / (1024)));

    if (bytesReceived == TotalBytes)
    {
        tcpServerConnection->close();
        localFile->close();
        QString msg = tr("������%1���ļ�%2�������!").arg(pb->logHostIp).arg(fileName);
        ui->ShowMessage->append(msg);
        ui->stackedWidget->setCurrentIndex(0);

        //��ͻ��˷��ͽ������ȷ����Ϣ
        msg = tr("������������ļ�%1�Ľ���!").arg(fileName);
        udpService->sendMessage(3,QHostAddress(pb->logHostIp),msg,7777);  //��ͻ��˷���ȷ����Ϣ
    }
}

//------------------�ͻ���------------------------------------

void ChatDialog::startClient()  //���Ͱ�ť
{
    bytesWritten = 0;
    QString addr = pb->logHostIp;
    qDebug()<<"file0000000000000000000000"<<addr;
    qDebug()<<"file0000000000000000000000"<<fileName;
	if(fileName == "")
    {
        QMessageBox::warning(this,tr("����"),tr("��ѡ��һ��Ҫ���͵��ļ�"));
        return;
    }

    udpService->sendMessage(1,QHostAddress(addr),fileName,7777);


    QString msg = tr("������%1������һ���ļ���%2!").arg(pb->logHostIp).arg(fileName);
    ui->ShowMessage->append(msg);

}

void ChatDialog::startTransfer()
{
    qDebug("file1111111111111111111111111111111111");
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly ))
    {
        QMessageBox::warning(this, tr("Ӧ�ó���"),
        tr("�޷���ȡ�ļ� %1:\n%2.")
        .arg(fileName)
        .arg(localFile->errorString()));
        return;
    }

    QString name  = localFile->fileName();
    TotalBytes = localFile->size();//�ļ��ܴ�С
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_3);

    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
    //����д���ܴ�С��Ϣ�ռ䣬�ļ�����С��Ϣ�ռ䣬�ļ���---�����ļ�������·����û��Ҫ��·��
    sendOut << qint64(0) << qint64(0) << currentFile;//������ܴ�С���ļ�����С����Ϣ��ʵ���ļ���С���ܺ�
    TotalBytes += outBlock.size();
    sendOut.device()->seek(0);
    //totalBytes���ļ��ܴ�С��������quint64�Ĵ�С+�ļ���+�ļ�ʵ�����ݵĴ�С
        //qint64((outBlock.size() - sizeof(qint64)*2))�õ������ļ�����С
    sendOut << TotalBytes << qint64((outBlock.size() - sizeof(qint64) * 2));
    //������ͷ���ݺ�ʣ�����ݵĴ�С�����ļ�ʵ�����ݵĴ�С
    bytesToWrite = TotalBytes - tcpClient.write(outBlock);
    qDebug() << currentFile << TotalBytes;
    outBlock.resize(0);
}


void ChatDialog::updateClientProgress(qint64 numBytes)
{
    bytesWritten += (int)numBytes;//�Ѿ��������ݵĴ�С
    if (bytesToWrite > 0)//����Ѿ�����������
    {
        //ÿ�η���loadSize��С�����ݣ���������Ϊ4KB�����ʣ������ݲ���4KB��
              //�ͷ���ʣ�����ݵĴ�С
     outBlock = localFile->read(qMin(bytesToWrite, loadSize));
    //�ͷ���ʣ�����ݵĴ�С
     bytesToWrite -= (int)tcpClient.write(outBlock);
     outBlock.resize(0); //��շ��ͻ�����
    }
    else
    {
      localFile->close();//���û�з����κ����ݣ���ر��ļ�
      ui->stackedWidget->setCurrentIndex(0);
    }
       //���½�����
    ui->ClientProgressBar->setMaximum(TotalBytes);
    ui->ClientProgressBar->setValue(bytesWritten);
    //clientStatusLabel->setText(tr("�ѷ��� %1MB").arg(bytesWritten / (1024 * 1024)));
}


void ChatDialog::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(fileName == "")
        return;
    FileInformation info(fileName);
    info.getFileInformation();


    ui->tableWidget->setItem(0,0,new QTableWidgetItem(info.fileName));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(info.fileType));
    QString size = tr("%1�ֽ�").arg(QString::number(info.fileSize,10));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(size));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem(fileName));
    ui->tableWidget->resizeColumnsToContents();



}




