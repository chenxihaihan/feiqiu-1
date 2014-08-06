#include "chatdialog.h"
#include "ui_chatdialog.h"



ChatDialog::ChatDialog(PersonLoginInfo *pb,FileInformation info,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{

    qDebug(__func__);

//------------------------������ʽ--------------------------------------

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);  //ȥ��������
    this->setFixedSize( this->width(),this->height());//���ô��ڹ̶���С
    //�������������ť�ϵ���ʾ��Ϣ
    ui->minButton->setToolTip(tr("��С��"));
    ui->closeButton->setToolTip(tr("�ر�"));
    connect(ui->minButton,SIGNAL(clicked()),this,SLOT(minWindows()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(closeWindows()));

    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);

//----------------------������Ϣ��ʼ��-----------------------------------

    chatMark = false;
    this->pb = new PersonLoginInfo(*pb);



       // QString time = QDateTime::currentDateTime().toString("hh:mm:ss ap");
      //  ui->ShowMessage->setTextColor(Qt::blue);
        //QString title = pb->logHostIp + "   "+time;
       // ui->ShowMessage->append(title);
       // ui->ShowMessage->setTextColor(QColor(0,0,255));
        //ui->ShowMessage->append(this->pb->firstMsg);




    QString name = pb->logInfo.userName == "" ? pb->logHostName : pb->logInfo.userName;
    ui->nameLable->setText(name);
    ui->userHead->setPixmap(QPixmap(pb->logInfo.userHeadPath));


//----------------------�Ҳ������Ϣ��ʾ---------------------------------

    ui->hostname->setText(pb->logHostName);
    ui->hostip->setText(pb->logHostIp);


//----------------------���ڶ�����ʼ��-----------------------------------


    M_Timer=new QTimer(this);
    connect(M_Timer,SIGNAL(timeout()),this,SLOT(S_TimeOut()));
    connect(ui->ShakeWindow,SIGNAL(clicked()),this,SLOT(slotShakeWindow()));
    Counter=MaxLimitTimes;



//----------------------socket��ʼ��----------------------------------
    udpService = new UdpConnection;
    receive = new QUdpSocket(this);
    receive->bind(7777,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(receive,SIGNAL(readyRead()),this,SLOT(receiveMessage()));

//-------------------------�ļ�����-------------------------------

    TotalBytes = 0;
    fileName = "";
    /*----�ͻ���-------*/
    loadSize = 4*1024; // 4Kb
    bytesWritten = 0;
    bytesToWrite = 0;
    connect(&tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
    connect(&tcpClient, SIGNAL(bytesWritten(qint64)),this, SLOT(updateClientProgress(qint64)));
    connect(ui->CliSendfile, SIGNAL(clicked()), this, SLOT(startClient()));
    connect(ui->CliOpenfile, SIGNAL(clicked()), this, SLOT(openFile()));

    /*----��������-------*/

    bytesReceived = 0;
    fileNameSize = 0;
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(ui->SerConfirm,SIGNAL(clicked()), this, SLOT(startServer()));
    connect(ui->SerRefuse, SIGNAL(clicked()), this, SLOT(refuse()));

    //startServer();   //��������



//----------------------�������¼---------------------------------
    openChatSave();

}






ChatDialog::~ChatDialog()
{
    delete ui;
    delete udpService;
    delete receive;
    tcpServerConnection->close();
}



void ChatDialog::mousePressEvent(QMouseEvent *lpEvent)
{
    if (lpEvent->button() == Qt::LeftButton)
    {
        m_WindowPos = this->pos();
        m_MousePos = lpEvent->globalPos();
        this->m_MousePressed = true;
    }
}


void ChatDialog::mouseReleaseEvent(QMouseEvent *lpEvent)
{

    if (lpEvent->button() == Qt::LeftButton)
    {
        this->m_MousePressed = false;
    }
}


void ChatDialog::mouseMoveEvent(QMouseEvent *lpEvent)
{
    if (m_MousePressed)
    {
        this->move(m_WindowPos + (lpEvent->globalPos() - m_MousePos));
    }
}


void ChatDialog::minWindows()
{
    this->showMinimized();
}

void ChatDialog::closeWindows()
{
    this->close();
    saveFile(fileName,pb);
}



//---------------------���ڶ���--------------------------------------
void ChatDialog::shakeStart()
{
    if(Counter<MaxLimitTimes)//���ڶ���
        return ;
    Counter=1;
    curPos=this->pos();
    M_Timer->start(ShakeSpeed);
}
void ChatDialog::S_TimeOut()
{
    if(Counter<MaxLimitTimes)//û�ж�������
    {
        ++Counter;
        switch(Counter%4)
        {
        case 1:
        {
            QPoint TmpPos(curPos.x(),curPos.y()-MaxLimitSpace);
            this->move(TmpPos);
        }
            break;
        case 2:
        {
            QPoint TmpPos(curPos.x()-MaxLimitSpace,curPos.y()-MaxLimitSpace);
            this->move(TmpPos);
        }
            break;
        case 3:
        {
            QPoint TmpPos(curPos.x()-MaxLimitSpace,curPos.y());
            this->move(TmpPos);
        }
            break;
        case 0:
        default:
            this->move(curPos);
            break;
        }

    }
    else
    {
        M_Timer->stop();
    }

}

void ChatDialog::slotShakeWindow()
{

     UdpConnection *noticeMsg = new UdpConnection();
     noticeMsg->sendMessage(4,QHostAddress(pb->logHostIp),"shake",7777);
     QString msg = tr("������%1����һ�����ڶ���").arg(pb->logHostIp);
     ui->ShowMessage->append(msg);
}





void ChatDialog::on_CutPic_clicked()
{
    MyWidget *widget = new MyWidget;
    widget->setMouseTracking(true);
    widget->show();
}
