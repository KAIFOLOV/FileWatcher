#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QPointer>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileWatcher(QObject *parent = nullptr);

    Q_SIGNAL void message(const QString &message);

    Q_SLOT void startWatching(const int interval = 10);
    Q_SLOT void checkFilesStatus();
    Q_SLOT void stopWatching();

    void addFile(const QString &filePath);
    void removeFile(const QString &filePath);

private:
    struct FileStatus {
        bool exists;
        qint64 size;
    };

    QMap<QString, FileStatus> _files;
    QTimer* _timer;
};

#endif // FILEWATCHER_H
