#include <QCoreApplication>
#include <QDebug>

#include "CommandThread.h"
#include "FileWatcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QPointer<FileWatcher> watcher = new FileWatcher();

    QTextStream consoleOutput(stdout);
    consoleOutput.setCodec("CP866");

    consoleOutput << QString("Команды для управления:") << Qt::endl
                  << QString("'add <path>' - добавить файл для слежения") << Qt::endl
                  << QString("'remove <path>' - удалить файл из слежения") << Qt::endl
                  << QString("'start' - начать слежение") << Qt::endl
                  << QString("'stop' - остановить слежение") << Qt::endl
                  << QString("'exit' - выйти") << Qt::endl;

    CommandThread commandThread(watcher);
    commandThread.start();

    QObject::connect(&commandThread, &CommandThread::startWatchingRequested, watcher,
                     &FileWatcher::startWatching);

    QObject::connect(&commandThread, &CommandThread::stopWatchingRequested,
                     watcher, &FileWatcher::stopWatching);

    QObject::connect(&commandThread, &CommandThread::exitRequested, &a, &QCoreApplication::quit);

    QObject::connect(watcher, &FileWatcher::message, [&consoleOutput](const QString &message) {
        consoleOutput << message << Qt::endl;
    });

    return a.exec();
}
