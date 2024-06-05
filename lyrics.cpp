#include "lyrics.h"
#include "ui_lyrics.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDir>
#include <QTemporaryFile>
#include <QProcess>


Lyrics::Lyrics(QWidget *parent, Spotify *spotifyInstance)
    : QWidget(parent)
    , ui(new Ui::Lyrics)
    , model(new QStringListModel(this))
    , contextMenu(new QMenu(this))
    , spotify(spotifyInstance)
{
    ui->setupUi(this);

    ui->clusterList->setModel(model);
    connect(ui->lyricsBack, &QPushButton::clicked, this, &Lyrics::on_lyricsBack_clicked);

    QAction *viewAction = new QAction("View", this);
    connect(viewAction, &QAction::triggered, this, &Lyrics::onViewLyrics);
    contextMenu->addAction(viewAction);

    ui->clusterList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->clusterList, &QListView::customContextMenuRequested, this, &Lyrics::showContextMenu);

    ui->lyricsBack->setStyleSheet("QPushButton:!hover{border: 1px solid black;border-radius: "
                                  "5px;background-color: #b83030;color:white;}"
                                  "QPushButton:hover{border: 1px solid black;border-radius: "
                                  "5px;background-color: #3d010e;color:#c0c0c0;}");
}

Lyrics::~Lyrics()
{
    delete ui;
}

void Lyrics::loadLyrics(const QString &artistName, const QString &songName)
{
    QTemporaryFile tempScript;
    if (tempScript.open()) {
        QFile::copy(":/parser/parser.py", tempScript.fileName());
        tempScript.setPermissions(QFile::ExeOwner | QFile::ReadOwner | QFile::WriteOwner);
    }

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
    QProcess process;

    process.start("python3", QStringList() << scriptPath << arguments);

    if (!process.waitForFinished()) {
        qDebug() << "Process did not finish.";
        return;
    }
    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        QString errorOutput = process.readAllStandardError();
        qDebug() << "Failed to fetch lyrics: " << errorOutput;
        QMessageBox::critical(this, "Error", "Failed to fetch lyrics: " + errorOutput);
        return;
    }
    QString lyrics = process.readAllStandardOutput().trimmed();
    if (lyrics.isEmpty()) {
        qDebug() << "No lyrics found.";
        // QMessageBox::warning(this, "Warning", "No lyrics found for " + artistName + " - " + songName);
        return;
    }

    ui->lyricsList->setPlainText(lyrics);
}

void Lyrics::updateSongList(int cluster)
{
    QList<QList<QString>> songs = spotify->filterByCluster(cluster);
    QStringList songNames;
    for (const auto &song : songs) {
        if (song.size() > static_cast<int>(COLUMNS::track_name) && song.size() > static_cast<int>(COLUMNS::artist_name)) {
            QString formattedName = song[static_cast<int>(COLUMNS::artist_name)] + " - " + song[static_cast<int>(COLUMNS::track_name)];
            songNames << formattedName;
        }
    }
    model->setStringList(songNames);
    ui->clusterList->setModel(model);
}

void Lyrics::showClusterForSong(const QString &artistName, const QString &songName)
{
    for (const auto &row : spotify->data) {
        if (row[static_cast<int>(COLUMNS::artist_name)] == artistName && row[static_cast<int>(COLUMNS::track_name)] == songName) {
            int clusterNumber = row[static_cast<int>(COLUMNS::s_clusters)].toInt();
            updateSongList(clusterNumber);
            ui->artistSongLabel->setText(artistName + " - " + songName);
            break;
        }
    }
}

void Lyrics::contextMenuEvent(QContextMenuEvent *event)
{
}

void Lyrics::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->clusterList->indexAt(pos);
    if (index.isValid()) {
        contextMenu->exec(ui->clusterList->viewport()->mapToGlobal(pos));
    }
}

void Lyrics::onViewLyrics()
{
    QModelIndex index = ui->clusterList->currentIndex();
    if (!index.isValid()) {
        return;
    }

    QString trackName = model->data(index, Qt::DisplayRole).toString();
    QStringList nameParts = trackName.split(" - ");
    if (nameParts.size() == 2) {
        QString artistName = nameParts[0].trimmed();
        QString songName = nameParts[1].trimmed();

        Lyrics *lyricsWindow = new Lyrics(nullptr, spotify);
        lyricsWindow->loadLyrics(artistName, songName);
        lyricsWindow->showClusterForSong(artistName, songName);
        lyricsWindow->show();
    }
}

void Lyrics::on_lyricsBack_clicked()
{
    close();
}
