#include "addwork.h"
#include "ui_addwork.h"
#include "work.h"
addwork::addwork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addwork)
{
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    //load();
    ui->setupUi(this);
}

addwork::~addwork()
{
    delete ui;
}
//���汸��¼
void addwork::on_pushButton_2_clicked()
{
    qDebug(__func__);
    emit save_signal();
    bool flag = false;
    if(!ui->textEdit->document()->isEmpty())
    {
        for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++)
        {
            if(it->Date == Sdate)
            {
                    it->record = ui->textEdit->toPlainText();
                    qDebug()<<it->record;
                    flag = true;
            }
        }
        if(!flag)
        {
            rd.Date = Sdate;
            rd.record = ui->textEdit->toPlainText();
            qDebug()<<rd.Date<<"  "<<rd.record;
            RList.push_back(rd);
            qDebug() << "aaaaaaaaaaaaaaaaa";
            qDebug() <<RList.size();
        }
        else
        {
            //RList.push_back(rd);
        }
        //RList.push_back(rd);
        saveFile(QString("record.txt"));
        ui->textEdit->clear();
        emit RecordSaved();

    }

    this->close();
}
//ȡ������
void addwork::on_pushButton_clicked()
{
    ui->textEdit->clear();
    this->close();
    emit cancel_signal();
}
//�Ե�ǰ���ڵĸ�ֵ
void addwork::setDate(QString date)
{
    qDebug(__func__);
    this->Sdate = date;
    qDebug()<<Sdate;
}
//��ʾ����¼
void addwork::apperRecord()
{
    qDebug(__func__);
    for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++){
        if(it->Date == Sdate){
            ui->textEdit->append(it->record);
        }
    }
}


//�����ļ���Ϣ
void addwork::load(/*QCalendarWidget *ca*/)
{
    qDebug(__func__);
    QFile file("record.txt");
    if(file.exists()){
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug()<<"open faile";
            return;
        }
        QDataStream in(&file);

        while(!in.atEnd())
        {

            in>>rd.Date>>rd.record;
            RList.push_back(rd);
            //rd.record = "";
        }
        file.close();
    }
}
//���汸��¼���ļ�
void addwork::saveFile(QString fileName)
{
    qDebug(__func__);
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        qDebug()<<"open failed";
        return;
    }

    QDataStream out(&file);
    for(vector<Record>::iterator it=RList.begin();it!=RList.end();it++){
        out << it->Date ;
        out << it->record ;
      //  out << '\n';
    }
}

