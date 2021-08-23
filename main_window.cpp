/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QFileDialog>
#include <QFile>
#include <QProcess>
#include "the_player.h"
#include "the_button.h"
#include "upload.h"


using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

            if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
          QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
          break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally

    QGridLayout *layout = new QGridLayout();
    buttonWidget->setLayout(layout);


    // create the 5*5 grid buttons
    for( int j = 0; j < 5; j++ ){
        for ( int i = 0; i < 5; i++ ) {
            TheButton *button = new TheButton(buttonWidget);
            button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
            buttons.push_back(button);
            layout->addWidget(button,i,j);
            button->init(&videos.at((i+j*5)%videos.size()));
        }
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    QWidget *pushButton = new QWidget();
    upload *upbutton = new upload(pushButton);
    upbutton->setText("Upload Video");
    upload *upbutton1 = new upload(pushButton);
    upbutton1->setText("Delete Video");
    upload *upbutton2 = new upload(pushButton);
    upbutton2->setText("Favorite Video");
    upload *upbutton3 = new upload(pushButton);
    upbutton3->setText("Export Video");

    QVBoxLayout *nextlayout = new QVBoxLayout();
    nextlayout->addWidget(upbutton);
    nextlayout->addWidget(upbutton1);
    nextlayout->addWidget(upbutton2);
    nextlayout->addWidget(upbutton3);
    pushButton->setLayout(nextlayout);


    // create the main window and layout
    QWidget *l = new QWidget();
    QVBoxLayout *top = new QVBoxLayout();



    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);

    l->setLayout(top);

    QHBoxLayout *t = new QHBoxLayout();
    t->addWidget(l);
    t->addWidget(pushButton);


    QWidget window;
    window.setLayout(t);
    window.setWindowTitle("video browsing tool");
    window.setMinimumSize(850, 600);
    window.setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/111.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    window.setPalette(palette);
    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
