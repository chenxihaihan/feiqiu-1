#ifndef MUSIC_H
#define MUSIC_H

#include <QDialog>
#include <phonon>
#include <QTimer>

#include <QTextEdit>

namespace Ui {
class music;
}

class music : public QDialog
{
    Q_OBJECT
    
public:
    explicit music(QWidget *parent = 0);
    ~music();
    void creatActions();//��������
    void setCurrentLrc();//���ø����ʾ
    
private slots:
    void on_back_clicked();

    void on_next_clicked();

    void on_open_clicked();

    void on_stop_clicked();

    void on_begin_clicked();

    void on_list_clicked();

private:
    Ui::music *ui;
    Phonon::MediaObject *audio;//����ý��Դ
    Phonon::MediaObject *musicInformationMediaObject;//
    Phonon::VideoWidget *videoWidget;//
    Phonon::AudioOutput *audioOutput;//���������豸
    Phonon::SeekSlider *seekSlider;//ʵ�ֽ�����
    Phonon::VolumeSlider *volumeSlider;//��������
    QList<Phonon::MediaSource> musicSourceList;//�����б�

    QTimer *timer;
    QTextEdit *text;//�������
    QIcon *iconplay;
    QIcon *iconpause;

    QAction *play;
    QAction *stop;
    QAction *open;
    QAction *sound;
    QAction *exit;
    QAction *remove;
    QMap<QString, QString> lrcMap;

private slots:
    void listWidget_cellDoubleClicked(QModelIndex index);//˫��ѡ��
    void showTime(qint64);//ʱ�������ʾ

    void musicAboutToFinish();//���������ʱ
    void currentMusicSourceChanged(Phonon::MediaSource);
};

#endif // MUSIC_H
