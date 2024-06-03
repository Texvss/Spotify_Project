#include "mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDir>
#include "./ui_mainwindow.h"
#include "lyrics.h"
#include "spotify.h"
#include "trackview.h"


const QString path = ":/data/playlist_2010to20222Ars.csv";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , listModel(new QStringListModel)
    , spotifyData(new Spotify(path))
    , trackView(nullptr)
    , stackedWidget(new QStackedWidget(this))
    , searchModel(new QStandardItemModel(this))
    , lyricsView(new Lyrics(this))
    , process(new QProcess(this))
    , login(nullptr)
    , liked(new Liked(this))
{
    ui->setupUi(this);
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
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onLyricsFetched);
    connect(lyricsView, &Lyrics::backLyricsClicked, this, &MainWindow::on_backButton_clicked);
    connect(liked, &Liked::backLikedClicked, this, &MainWindow::on_backButton_clicked);

    listModel->setStringList(spotifyData->getTrackNames());
    ui->searchLine->setPlaceholderText("Search...");
    ui->searchLine->setStyleSheet("border: 1px solid black;border-radius: 5px;background-color: #717072;color:white;");

    ui->searchList->setStyleSheet("QListView { padding: 5px; border: 1px solid gray; border-radius: 5px; }");
    ui->likedButton->setStyleSheet("QPushButton:!hover{border: 1px solid black;border-radius: 5px;background-color: #717072;color:white;}"
                                   "QPushButton:hover{border: 1px solid black;border-radius: 5px;background-color: #33322e;color:#c0c0c0;}");

    stackedWidget->addWidget(ui->centralwidget);
    stackedWidget->addWidget(lyricsView);
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
    delete process;
}



void MainWindow::showUsername(const QString &username)
{
    this->username = username;
    ui->userLabel->setText("User: " + username);
    if (trackView) {
        trackView->setUsername(username);
    }

    if (liked) {
        liked->loadPlaylist(username);
    }
    qDebug() << username;
}
void MainWindow::showTracks(const QStringList &trackNames)
{
    if (!trackView) {
        trackView = new TrackView(this);
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
    QStringList arguments;
    arguments << artistName << songName.split("-");

    qDebug() << "Fetching lyrics for:" << artistName << "-" << songName;

    QProcess installProcessRequests;
    installProcessRequests.start("python3", QStringList() << "-m" << "pip" << "install" << "requests");
    installProcessRequests.waitForFinished();

    QProcess installProcessBs4;
    installProcessBs4.start("python3", QStringList() << "-m" << "pip" << "install" << "beautifulsoup4");
    installProcessBs4.waitForFinished();

    QProcess installProcessLxml;
    installProcessLxml.start("python3", QStringList() << "-m" << "pip" << "install" << "requests" << "beautifulsoup4" << "lxml");
    installProcessLxml.waitForFinished();

    QString appDirPath = QCoreApplication::applicationDirPath();
    QString scriptPath = QDir(appDirPath).filePath("parser.py");
    QFile scriptFile(":/data/parser.py");

    if (scriptFile.exists()) {
        if (scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QFile outFile(scriptPath);
            if (outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream in(&scriptFile);
                QTextStream out(&outFile);
                out << in.readAll();
                scriptFile.close();
                outFile.close();
                qDebug() << "Script extracted to: " << scriptPath;
            } else {
                qDebug() << "Failed to open output file for writing.";
                QMessageBox::critical(this, "Error", "Failed to open output file for writing.");
                return;
            }
        } else {
            qDebug() << "Failed to open resource file for reading.";
            QMessageBox::critical(this, "Error", "Failed to open resource file for reading.");
            return;
        }
    } else {
        qDebug() << "Script file not found in resources.";
        QMessageBox::critical(this, "Error", "Script file not found in resources.");
        return;
    }

    qDebug() << "Executing script at: " << scriptPath;
    process->start("python3", QStringList() << scriptPath << arguments);

    if (!process->waitForFinished()) {
        qDebug() << "Process did not finish.";
        return;
    }
    if (process->exitStatus() != QProcess::NormalExit || process->exitCode() != 0) {
        QString errorOutput = process->readAllStandardError();
        qDebug() << "Failed to fetch lyrics: " << errorOutput;
        QMessageBox::critical(this, "Error", "Failed to fetch lyrics: " + errorOutput);
        return;
    }
    QString lyrics = process->readAllStandardOutput().trimmed();
    if (lyrics.isEmpty()) {
        qDebug() << "No lyrics found.";
        // QMessageBox::warning(this, "Warning", "No lyrics found for " + artistName + " - " + songName);
        return;
    }
    lyricsView->setLyrics(lyrics);
    stackedWidget->setCurrentWidget(lyricsView);
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

void MainWindow::onLyricsFetched(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with exit code:" << exitCode << "and exit status:" << exitStatus;

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        QString lyrics = process->readAllStandardOutput();

        lyricsView->setLyrics(lyrics);
        stackedWidget->setCurrentWidget(lyricsView);
        QString errorOutput = process->readAllStandardError();

        // QMessageBox::critical(this, "Error", "Failed to fetch lyrics");
    }
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
    stackedWidget->setCurrentWidget(ui->centralwidget);
}
