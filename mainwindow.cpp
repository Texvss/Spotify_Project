#include "mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDir>
#include <QTemporaryFile>
#include "./ui_mainwindow.h"
#include "lyrics.h"
#include "spotify.h"
#include "trackview.h"


const QString path = ":/data/clusters_50.csv";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listModel(new QStringListModel)
    , spotifyData(new Spotify(path))
    , trackView(nullptr)
    , stackedWidget(new QStackedWidget(this))
    , searchModel(new QStandardItemModel(this))
    , login(nullptr)
    , liked(new Liked(this))
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4C4C4C, stop:1 black); }");
    this->hide();
    ui->searchList->setVisible(false);

    connect(ui->popButton, &QPushButton::clicked, this, &MainWindow::on_popButton_clicked);
    connect(ui->rapButton, &QPushButton::clicked, this, &MainWindow::on_rapButton_clicked);
    connect(ui->rockButton, &QPushButton::clicked, this, &MainWindow::on_rockButton_clicked);
    connect(ui->randbButton, &QPushButton::clicked, this, &MainWindow::on_randbButton_clicked);
    connect(ui->hiphopButton, &QPushButton::clicked, this, &MainWindow::on_hiphopButton_clicked);
    connect(ui->dancepopButton, &QPushButton::clicked, this, &MainWindow::on_dancepopButton_clicked);
    connect(ui->poprapbutton, &QPushButton::clicked, this, &MainWindow::on_poprapbutton_clicked);
    connect(ui->urbanconbutton, &QPushButton::clicked, this, &MainWindow::on_urbanconbutton_clicked);
    connect(ui->trapButton, &QPushButton::clicked, this, &MainWindow::on_trapButton_clicked);
    connect(ui->southernhiphopButton, &QPushButton::clicked, this, &MainWindow::on_southernhiphopButton_clicked);
    connect(ui->modernrockButton, &QPushButton::clicked, this, &MainWindow::on_modernrockButton_clicked);
    connect(ui->canadianpopButton, &QPushButton::clicked, this, &MainWindow::on_canadianpopButton_clicked);
    connect(ui->hippopButton, &QPushButton::clicked, this, &MainWindow::on_hippopButton_clicked);
    connect(ui->neoMellowButton, &QPushButton::clicked, this, &MainWindow::on_neoMellowButton_clicked);
    connect(ui->postgrungeButton, &QPushButton::clicked, this, &MainWindow::on_postgrungeButton_clicked);
    connect(ui->edmButton, &QPushButton::clicked, this, &MainWindow::on_edmButton_clicked);
    connect(ui->searchLine, &QLineEdit::textChanged, this, &MainWindow::on_searchLine_textChanged);
    connect(ui->searchList, &QListView::clicked, this, &MainWindow::on_searchList_clicked);
    connect(liked, &Liked::backLikedClicked, this, &MainWindow::on_backButton_clicked);

    listModel->setStringList(spotifyData->getTrackNames());
    ui->searchLine->setPlaceholderText("Search...");
    ui->searchLine->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid gray; border-radius: 5px; }");
    ui->searchList->setStyleSheet("QListView { padding: 5px; border: 1px solid gray; border-radius: 5px; }");

    ui->likedButton->setStyleSheet("QPushButton:!hover{border: 1px solid black;border-radius: 5px;background-color: #717072;color:white;}"
                                   "QPushButton:hover{border: 1px solid black;border-radius: 5px;background-color: #33322e;color:#c0c0c0;}");


    stackedWidget->addWidget(ui->centralwidget);
    stackedWidget->addWidget(liked);
    setCentralWidget(stackedWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete listModel;
    delete spotifyData;
    delete trackView;
    delete searchModel;
    delete stackedWidget;
}


void MainWindow::showUsername(const QString &username)
{
    this->username = username;
    ui->userLabel->setText("User: " + username);
    if (trackView) {
        trackView->setUsername(username);
    }
    if (liked)
    {
        liked->loadPlaylist(username);
    }
    qDebug() << username;
}
void MainWindow::showTracks(const QStringList &trackNames)
{
    if (!trackView) {
        trackView = new TrackView(this, spotifyData); // Pass Spotify instance here
        connect(trackView, &TrackView::backButtonClicked, this, &MainWindow::on_backButton_clicked);
        stackedWidget->addWidget(trackView);
    }
    trackView->setUsername(username);
    trackView->genreTracks(trackNames);
    stackedWidget->setCurrentWidget(trackView);
}


QStringList MainWindow::getTrackNames(const QList<QList<QString>> &filteredData) const
{
    QStringList trackNames;
    for (const auto &row : filteredData) {
        QString trackWithArtist = row[static_cast<int>(COLUMNS::artist_name)] + " - "
                                  + row[static_cast<int>(COLUMNS::track_name)];
        trackNames << trackWithArtist;
    }
    return trackNames;
}

void MainWindow::on_popButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("pop");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
        setWindowTitle("Genre");
    }
}

void MainWindow::on_rapButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("rap");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_rockButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("rock");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_randbButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("r&b");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_hiphopButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("hip hop");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_dancepopButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("dance pop");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_poprapbutton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("pop rap");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_urbanconbutton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("urban contemporary");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_trapButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("trap");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_southernhiphopButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("southern hip hop");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_modernrockButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("modern rock");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_canadianpopButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("canadian pop");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_hippopButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("hip pop");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_neoMellowButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("neo mellow");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_postgrungeButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("post-grunge");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

void MainWindow::on_edmButton_clicked()
{
    if (spotifyData) {
        QList<QList<QString>> filteredData = spotifyData->filterByGenre("edm");
        QStringList trackNames = getTrackNames(filteredData);
        showTracks(trackNames);
    }
}

// void MainWindow::fetchLyrics(const QString &artistName, const QString &songName)
// {
//     QStringList arguments;
//     arguments << artistName << songName;

//     qDebug() << "Fetching lyrics for:" << artistName << "-" << songName;
//     process->start("python3",
//                    QStringList() << "/Users/mansur/PycharmProjects/genius_parser/parser.py" << arguments);
//     process->waitForFinished();
//     qDebug() << "Results: "
//              << "/n" << process->readAllStandardOutput();
// }

void MainWindow::fetchLyrics(const QString &artistName, const QString &songName)
{

    Lyrics *lyricsView = new Lyrics(nullptr, spotifyData);

    lyricsView->loadLyrics(artistName, songName);
    lyricsView->showClusterForSong(artistName, songName);
    lyricsView->show();

}

void MainWindow::on_searchLine_textChanged(const QString &text)
{
    searchModel->clear();
    if (text.isEmpty()) {
        ui->searchList->setVisible(false);
    } else
    {
        for (const auto &row : spotifyData->data) {
            if (row[static_cast<int>(COLUMNS::track_name)].contains(text, Qt::CaseInsensitive)) {
                QString displayText = row[static_cast<int>(COLUMNS::track_name)];
                searchModel->appendRow(new QStandardItem(displayText));
                ui->searchList->setModel(searchModel);
            }
            ui->searchList->setVisible(true);
        }
    }
}



void MainWindow::on_searchList_clicked(const QModelIndex &index)
{
    QString trackName = searchModel->data(index, Qt::DisplayRole).toString();
    QString artistName;
    for (const auto &row : spotifyData->data) {
        if (row[static_cast<int>(COLUMNS::track_name)].compare(trackName, Qt::CaseInsensitive)
            == 0) {
            artistName = row[static_cast<int>(COLUMNS::artist_name)];
            break;
        }
    }

    fetchLyrics(artistName, trackName);
}



void MainWindow::on_backButton_clicked()
{
    stackedWidget->setCurrentWidget(ui->centralwidget);
}


void MainWindow::on_likedButton_clicked()
{
    if (!liked) {
        stackedWidget->addWidget(liked);
    }
    liked->loadPlaylist(username);
    liked->show();
    stackedWidget->setCurrentWidget(liked);
}

void MainWindow::on_backLiked_clicked()
{
    // stackedWidget->setCurrentWidget(ui->centralwidget);
}




