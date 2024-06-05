#ifndef LYRICS_H
#define LYRICS_H

#include <QWidget>
#include <QStringListModel>
#include <QMenu>
#include <QLabel>
#include <QContextMenuEvent>
#include "spotify.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Lyrics; }
QT_END_NAMESPACE

class Lyrics : public QWidget
{
    Q_OBJECT

public:
    explicit Lyrics(QWidget *parent = nullptr, Spotify *spotify = nullptr);
    ~Lyrics();
    void loadLyrics(const QString &artistName, const QString &songName);
    void updateSongList(int cluster);
    void showClusterForSong(const QString &artistName, const QString &songName);
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void on_lyricsBack_clicked();
    void onViewLyrics();
    void showContextMenu(const QPoint &pos);

private:
    Ui::Lyrics *ui;
    QStringListModel *model;
    QMenu *contextMenu;
    Spotify *spotify;
    QLabel *artistSongLabel;
};

#endif // LYRICS_H
