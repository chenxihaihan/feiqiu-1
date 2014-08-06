#include "mysharefile.h"
#include "ui_mysharefile.h"


mysharefile::mysharefile(vector<FileInformation> *list,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mysharefile)
{
    ui->setupUi(this);
    //��ʼ�������ļ���Ϣ
    filelist = list;

    QStringList header;
    header<<tr("�����û�")<<tr("�ļ���")<<tr("�ļ�����")<<tr("�ļ���С")<<tr("�޸�ʱ��")<<tr("�ļ�·��");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(13);
    ui->tableWidget->setWindowTitle(tr("�ļ�����ҳ"));

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//���ܸ��ı���������
    ui->tableWidget->setSelectionBehavior((QAbstractItemView::SelectRows));//ѡ��һ��
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //����Ϊ����ѡ�ж��Ŀ��
   // QTableWidgetItem *item = new QTableWidgetItem();
    //item->setBackground(QColor(255,255,0));
    //item->setTextColor(QColor(255,10,150));
}

mysharefile::~mysharefile()
{
    delete ui;
}

void mysharefile::on_addfile_clicked()
{


    qDebug(__func__);
    static int i = 0;

    filename = QFileDialog::getOpenFileName(this);
    if(filename == NULL)
        return;
    FileInformation info(filename);
    //filelist.push_back(info);
   // qDebug()<<filelist.size();
    ui->tableWidget->setItem(i,0, new QTableWidgetItem(info.HostName));
    ui->tableWidget->setItem(i,1, new QTableWidgetItem(info.fileName));
    ui->tableWidget->setItem(i,2, new QTableWidgetItem(info.fileType));

    QString size = tr("%1�ֽ�").arg(QString::number(info.fileSize,10));
    ui->tableWidget->setItem(i,3, new QTableWidgetItem(size));
    ui->tableWidget->setItem(i,4, new QTableWidgetItem(info.getfiletime()));
    ui->tableWidget->setItem(i++,5, new QTableWidgetItem(info.filePath));
    //ui->tableWidget->resizeColumnsToContents();
    //�㲥һ����Ϣ��ȥ

    send = new UdpConnection();
    send->sendMessage(4,info,QHostAddress::Broadcast,6666);
}

void mysharefile::on_OK_clicked()
{
    this->close();
}
