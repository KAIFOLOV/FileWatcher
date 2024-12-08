#include "CommandThread.h"

#include <QDebug>

CommandThread::CommandThread(QPointer<FileWatcher> watcher, QObject *parent)
    : QThread(parent), _watcher(watcher) {}

void CommandThread::run() {
    QTextStream consoleInput(stdin);

    while (true) {
        QString input = consoleInput.readLine().trimmed();
        QStringList parts = input.split(' ', Qt::SkipEmptyParts);

        if (parts.isEmpty()) {
            continue;
        }

        const QString command = parts[0].toLower();

        if (command == "add" && parts.size() > 1) {
            _watcher->addFile(parts[1]);
        } else if (command == "remove" && parts.size() > 1) {
            _watcher->removeFile(parts[1]);
        } else if (command == "start") {
            emit startWatchingRequested(1000);
        } else if (command == "stop") {
            emit stopWatchingRequested();
        } else if (command == "exit") {
            emit exitRequested();
            break;
        } else {
            qInfo() << QString("Unknown command");
        }
    }
}
