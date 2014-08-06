#include "sharefile.h"
#include "ui_sharefile.h"
#include "chatdialog.h"


sharefile::sharefile(vector<FileInformation> *list,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sharefile)
{
    ui->setupUi(this);
    //�ļ�������Ϣ��ʼ��
    filelist = list;

    QStringList header;
    header<<tr("�����û�")<<tr("�ļ���")<<tr("�ļ�����")<<tr("�ļ���С")<<tr("�޸�ʱ��")<<tr("�ļ�·��");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(13);
    ui->tableWidget->setWindowTitle(tr("�ļ�����ҳ"));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);      //���ܸ��ı���������
    ui->tableWidget->setSelectionBehavior((QAbstractItemView::SelectRows));   //ѡ��һ��
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //����Ϊ����ѡ�ж��Ŀ��
    //QTableWidgetItem *item = new QTableWidgetItem();
    //item->setBackground(QColor(255,0,0));
    //item->setTextColor(QColor(255,100,100));
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));
}

void sharefile::getItem(QTableWidgetItem* item)
{
    QString fileName;
    qint64  fileSize;  //��λKB
    QString fileType;
    QString time;
    QList<QTableWidgetItem*> temp = ui->tableWidget->selectedItems();

    vector<FileInformation>::iterator it;
    for(it = filelist->begin(); it != filelist->end(); ++it)
    {
        if((*it).fileName == temp[1]->text())
        {
            checkedFile = (*it);
            return;
        }
    }
    checkedFile = FileInformation();
}

sharefile::~sharefile()
{
    delete ui;
}

void sharefile::on_mysharefile_clicked()
{
    mysharefile *temp = new mysharefile(filelist);
    temp->show();
}


void sharefile::on_update_clicked()
{
    qDebug(__func__);
    if(filelist->empty())
    {
        qDebug()<<"ninini";
        return;
    }
    int i=0;

    ui->tableWidget->clear();
    QStringList header;
    header<<tr("�����û�")<<tr("�ļ���")<<tr("�ļ�����")<<tr("�ļ���С")<<tr("�޸�ʱ��")<<tr("�ļ�·��");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    vector<FileInformation>::iterator it;
    for(it = filelist->begin(); it != filelist->end(); ++it)
    {   qDebug() << (*it).fileName;
        ui->tableWidget->setItem(i,0, new QTableWidgetItem((*it).HostName));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem((*it).fileName));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem((*it).fileType));
        QString size = tr("%1�ֽ�").arg(QString::number((*it).fileSize),10);
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(size));
        ui->tableWidget->setItem(i,4, new QTableWidgetItem((*it).getfiletime()));
        ui->tableWidget->setItem(i++,5, new QTableWidgetItem((*it).filePath));

    }
   // ui->tableWidget->resizeColumnsToContents();
}


void sharefile::on_download_clicked()
{
    PersonLoginInfo *info = new PersonLoginInfo(checkedFile.HostName,checkedFile.HostIP);
    ChatDialog *ServerDialog = new ChatDialog(info);
    ServerDialog->fileName = checkedFile.filePath;
    ServerDialog->show();
    //ServerDialog->startServer();

    downSend = new UdpConnection();
    checkedFile.ServerIP = SystemInformation().getHostIP();
    checkedFile.ServerName = SystemInformation().getHostName();
    downSend->sendMessage(5,checkedFile,QHostAddress(checkedFile.HostIP),6666);


}


void sharefile::on_quit_clicked()
{
    this->close();
}
