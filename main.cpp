#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>
#include <QStyle>
#include <QHBoxLayout>
#include <QAudioOutput>

void openFile(QMediaPlayer *mediaPlayer)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv)");
    if (!fileName.isEmpty())
    {
        mediaPlayer->setSource(QUrl::fromLocalFile(fileName));
        mediaPlayer->play();
    }
}

void playVideo(QMediaPlayer *mediaPlayer, QPushButton *playButton)
{
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState)
    {
        mediaPlayer->pause();
        playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPlay));
    }
    else
    {
        mediaPlayer->play();
        playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void setPosition(QMediaPlayer *mediaPlayer, int position)
{
    mediaPlayer->setPosition(position);
}

void switchAudio(QMediaPlayer *mediaPlayer, QPushButton *audSwitch)
{
    int track = mediaPlayer->activeAudioTrack();
    if (track == 1)
    {
        mediaPlayer->setActiveAudioTrack(0);
    }
    else
    {
        mediaPlayer->setActiveAudioTrack(1);
    }
}

void updatePosition(QSlider *slider, qint64 position)
{
    slider->setValue(position);
}

void updateDuration(QSlider *slider, qint64 duration)
{
    slider->setRange(0, duration);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QWidget window;
    window.setWindowTitle("Qt Video Player");

    // Create media player
    QMediaPlayer *mediaPlayer = new QMediaPlayer(&window);

    // Create video widget
    QVideoWidget *videoWidget = new QVideoWidget(&window);

    auto audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setVideoOutput(videoWidget);

    // Create open button
    QPushButton *openButton = new QPushButton("Open Video", &window);
    QObject::connect(openButton, &QPushButton::clicked, [=]()
                     { openFile(mediaPlayer); });

    // Create play button
    QPushButton *playButton = new QPushButton();
    playButton->setIcon(playButton->style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(playButton, &QPushButton::clicked, [=]()
                     { playVideo(mediaPlayer, playButton); });

    // Create play button
    QPushButton *audSwitch = new QPushButton();
    audSwitch->setIcon(audSwitch->style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(audSwitch, &QPushButton::clicked, [=]()
                     { switchAudio(mediaPlayer, audSwitch); });

    // Create slider for video progress
    QSlider *slider = new QSlider(Qt::Horizontal, &window);
    QObject::connect(slider, &QSlider::sliderMoved, [=](int position)
                     { setPosition(mediaPlayer, position); });
    QObject::connect(mediaPlayer, &QMediaPlayer::positionChanged, [=](qint64 position)
                     { updatePosition(slider, position); });
    QObject::connect(mediaPlayer, &QMediaPlayer::durationChanged, [=](qint64 duration)
                     { updateDuration(slider, duration); });

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(audSwitch);
    controlLayout->addWidget(slider);

    layout->addLayout(controlLayout);

    window.setLayout(layout);
    window.resize(800, 600);
    window.show();

    return app.exec();
}