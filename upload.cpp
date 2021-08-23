#include "upload.h"
#include <QDebug>
#include <QFile>
#include <QTime>
#include <QProcess>

void upload::getpath() {
    QString path = QFileDialog::getOpenFileName(
                this,
                "open",
                "./",
                "wmv(*.wmv);;mp4(*.mp4);;avi(*.avi);;mov(*.mov)"
                );
    qDebug() << path;
    QFile video(path);
    QFileInfo videoinfo(path);
    QString filename = videoinfo.fileName();
    QString basename = videoinfo.baseName();
    qDebug() << filename;
    qDebug() << basename;
    QString newpath = "D:/videoplayer/videos/" ;
    video.copy(newpath+filename);

    //QTime time;
    //time.start();
    QProcess p(0);
    QString program = "D:/ffmpeg-N-102848-gb7ba472f43-win64-gpl/bin/ffmpeg.exe";
    QStringList argu;
    argu.append("-ss");
    argu.append("00:01");
    argu.append("-i");
    argu.append(newpath+filename);
    argu.append("-vframes");
    argu.append("1");
    argu.append("-q:v");
    argu.append("2");
    argu.append(newpath+basename+".png");
    p.start(program,argu);
    if (!p.waitForFinished()) {
        qDebug() << "fail";
    }
    else {
        qDebug() << "success";
    }

}

void upload::init(){

}
