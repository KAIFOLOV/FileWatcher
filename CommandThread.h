#ifndef COMMANDTHREAD_H
#define COMMANDTHREAD_H

#include "FileWatcher.h"
#include <QObject>
#include <QThread>
#include <QTextStream>

class CommandThread : public QThread
{
    Q_OBJECT
public:
    CommandThread(QPointer<FileWatcher> watcher, QObject *parent = nullptr);

    Q_SIGNAL void startWatchingRequested(const int interval);
    Q_SIGNAL void stopWatchingRequested();
    Q_SIGNAL void exitRequested();

protected:
    void run() override;

private:
    QPointer<FileWatcher> _watcher;
};

#endif // COMMANDTHREAD_H
