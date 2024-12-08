#include "FileWatcher.h"

#include <QFileInfo>
#include <QDebug>

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent)
{
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &FileWatcher::checkFilesStatus);
}

void FileWatcher::addFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    _files[filePath] = {fileInfo.exists(), fileInfo.exists() ? fileInfo.size() : 0};
    emit message(QString("Добавлен файл для слежения: %1").arg(filePath));
}

void FileWatcher::removeFile(const QString &filePath)
{
    if (_files.remove(filePath)) {
        emit message(QString("Файл удалён из слежения: %1").arg(filePath));
    } else {
        emit message(QString("Файл не найден в списке для слежения: %1").arg(filePath));
    }
}

void FileWatcher::startWatching(const int interval)
{
    _timer->start(interval);
    if (!_files.isEmpty()) {
        emit message(QString("Начато слежение за файлами с интервалом %1 мс").arg(interval));
    } else {
        emit message(QString("Нет файлов для слежения"));
    }
}

void FileWatcher::stopWatching() {
    if (_timer->isActive()) {
        _timer->stop();
        emit message(QString("Слежение остановлено"));
    } else {
        emit message(QString("Слежение уже остановлено"));
    }
}

void FileWatcher::checkFilesStatus()
{
    for (auto it = _files.begin(); it != _files.end(); ++it) {
        const QString &filePath = it.key();
        QFileInfo fileInfo(filePath);
        bool currentExists = fileInfo.exists();
        qint64 currentSize = currentExists ? fileInfo.size() : 0;

        if (currentExists != it.value().exists) {
            it.value().exists = currentExists;
            if (currentExists) {
                emit message(QString("Файл '%1' появился, размер: %2 байт").arg(filePath).arg(currentSize));
            } else {
                emit message(QString("Файл '%1' был удалён").arg(filePath));
            }
        } else if (currentExists && currentSize != it.value().size) {
            it.value().size = currentSize;
            emit message(QString("Файл '%1' изменён, новый размер: %2 байт").arg(filePath).arg(currentSize));
        }
    }
}
