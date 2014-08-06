#include "music.h"
#include "ui_music.h"
#include <QFileDialog>
#include <QtGlobal>
#include <QDesktopServices>
#include <QDebug>
#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QtGui>
music::music(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::music)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("���ֲ�����"));//���ñ���
    this->setMouseTracking(true);//����widget�������١�

    /**��ʼ��ý��**/
    audio =new Phonon::MediaObject();//ý�����
    audio->setTickInterval(1);
    audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory);//��Ƶ���
    Phonon::createPath(audio,audioOutput);//����ý���������Ƶ���

    musicInformationMediaObject = new Phonon::MediaObject(this);  //������Ϣ����

    volumeSlider = new Phonon::VolumeSlider(audioOutput,this);  //����������
    volumeSlider->move(50,70);
    volumeSlider->resize(100,20);
    volumeSlider->setStyleSheet("background-color:rgb(255,255,255,100)");
    volumeSlider->setFixedWidth(250);//�̶���������С
    volumeSlider->setFocus();

    seekSlider = new Phonon::SeekSlider(audio,this); //���Ȼ�����
    seekSlider->move(10,35);
    seekSlider->resize(170,20);
    seekSlider->setStyleSheet("background-color:rgb(255,255,255,100)");

    //creatActions();

    connect(audio,SIGNAL(tick(qint64)),this,SLOT(showTime(qint64)));
    //ʱ����ʾ���ʵĸ���,tick����ý���������ʱ���������λ����
    connect(audio,SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this,SLOT(mediaObjectStateChanged(Phonon::State,Phonon::State)));

    text = new QTextEdit(this);
    text->hide();//���ڲ����¾����Լ���ʱ����
    //ui->textEdit->setVisible(false);//�ı��༭�򲻿ɼ�����������ʣ�
    connect(ui->listWidget,SIGNAL(doubleClicked(QModelIndex)),SLOT(listWidget_cellDoubleClicked(QModelIndex)));
    //˫�����Ÿ���
    connect(musicInformationMediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(musicInformationStateChanged(Phonon::State,Phonon::State)));
    //������������ʱִ��
    //�Զ�������һ��
    connect(audio,SIGNAL(aboutToFinish()),this,SLOT(musicAboutToFinish()));//�������ֿ����ʱ
    connect(audio,SIGNAL(currentSourceChanged(Phonon::MediaSource)),this,
            SLOT(currentMusicSourceChanged(Phonon::MediaSource)));

     QPalette palette = ui->label_lrc->palette();//���ô����ڸ����ɫ
     palette.setColor(QPalette::WindowText,Qt::darkBlue);
     ui->label_lrc->setPalette(palette);


      setAutoFillBackground(true);


      ui->listWidget->setStyleSheet("background:rgb(255,255,255,0,)");
      seekSlider->setToolTip(tr("���Ž�����"));//���ͣ����ʾ setAcceptDrops(true);
      setAcceptDrops(true);



      QSplashScreen splash(QPixmap(":/imag/tuopan.jpg"));
      QLabel lbl(&splash);
      QMovie mv(":/imag/tuopan.jpg");
      lbl.setMovie(&mv);
      mv.start ();
      splash.show ();
      splash.setCursor(Qt::BlankCursor);
}

music::~music()
{
    delete ui;
}

void music::on_back_clicked()
{
    int index = musicSourceList.indexOf(audio->currentSource()) - 1;
    if(index >= 0)
    {
        audio->stop();
        audio->setCurrentSource(musicSourceList.at(index));
        audio->play();
    }
    else
       return;
}

void music::on_next_clicked()
{
    int index = musicSourceList.indexOf(audio->currentSource()) + 1;
    if(index < musicSourceList.size())
    {
        audio->stop();
        audio->setCurrentSource(musicSourceList.at(index));
        audio->play();
    }
    else
       return;
}

void music::on_open_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(
                this,tr("�������ļ�"),
                QDesktopServices::storageLocation(QDesktopServices::MusicLocation),
                tr("Music (*.mp3 *.wma *.wav *.mid *.rm)"));//�ļ����͹���
    if(list.isEmpty()) return;
    int index = musicSourceList.size();  //��ȡ��ǰ�б�ĳ���
    foreach(QString string,list)
    {
        Phonon::MediaSource source(string);
        musicSourceList.append(source);//�ڵ�ǰ�б�����һ����¼
    }
    if(!musicSourceList.isEmpty())
        musicInformationMediaObject->setCurrentSource(musicSourceList.at(index));


    index = musicSourceList.size();  //��ȡ��ǰ�б�ĳ���
      for(int i=0;i<index;i++)
      {
           musicInformationMediaObject->setCurrentSource(musicSourceList.at(i));
           QString fileName = musicInformationMediaObject->currentSource().fileName();
           QString mp3Name = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);

           ui->listWidget->addItem(mp3Name);
     }
}
void music::on_stop_clicked()
{
   audio->stop();
}

void music::on_begin_clicked()
{
    if(musicSourceList.isEmpty())
        {
            //QMessageBox::information(this, tr("no music files"), tr("no files to play"));
            return ;
        }
        audio->setQueue(musicSourceList);//�б�ѭ��

    if(audio->state() == Phonon::PlayingState)
        audio->pause();
    else
    {
        audio->play();
    }

}

void music::setCurrentLrc()//���ø����ʾ
{
    QString fileName = audio->currentSource().fileName();
    qDebug() << fileName;
    QString lrcName = fileName.remove(fileName.right(3)) + "lrc";
    qDebug() << lrcName;
    QFile file(lrcName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        ui->label_lrc->setText(tr("��ǰ����Ŀ¼��δ�ҵ�����ļ�"));
    }
    QTextStream in(&file);
    //ui->textEdit->setText(in.readAll());
}
void music::listWidget_cellDoubleClicked(QModelIndex index)
{
    audio->stop();
    audio->clearQueue();
    int row = index.row();

    if (row >= musicSourceList.size())
        return;
    audio->setCurrentSource(musicSourceList[row]);
    audio->play();
}



void music::showTime(qint64 time)
{
    qint64 temp = audio->totalTime();
    QTime totalTime(0,(temp / 60000) % 60,(temp / 1000) % 60,time %1000);//(h,min,s,ms)
    QTime curTime(0,(time / 60000) % 60,(time / 1000) % 60,time %1000);
    ui->label_time->setText(tr("%1 / %2")
                       .arg(curTime.toString("mm:ss"))
                       .arg(totalTime.toString("mm:ss")));
    ui->label_time->update();
}



void music::musicAboutToFinish() //�������ֿ����ʱ
{
    int index = musicSourceList.indexOf(audio->currentSource()) + 1; //��һ��
    if(musicSourceList.size() > index)
        audio->enqueue(musicSourceList.at(index));//ע�⣺�������״̬�ĸı�state
    else
        audio->stop();
}

void music::currentMusicSourceChanged(Phonon::MediaSource source)
        //���Զ�������һ������ʱ����listWidget������Ϊѡ��״̬
{
    ui->listWidget->setCurrentRow(musicSourceList.indexOf(source),
                                  QItemSelectionModel::ToggleCurrent);

    setCurrentLrc();
    QString fileName = audio->currentSource().fileName();
    QString title = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
    this->setWindowTitle(title);//��ʾ��*.mp3��

    audio->pause();
    audio->play();
}

