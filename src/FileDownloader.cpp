#include "FileDownloader.h"

#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QUrl>
#include <QFile>
#include <QPixmap>
#include <QAtomicInt>
#include <QApplication>

#include "UDownloadProcess.h"

int g_threadCounter = 0;
QAtomicInt g_currentNumber;
QMutex sm_mutex;

FileDownloader::FileDownloader(QObject *parent)
  : QObject(parent)
{

}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::curlDownload(QUrl imageUrl, QString fileName, int threadsQty, int number)
{
  while (g_threadCounter > threadsQty) {
    QThread::usleep(10000);
  }
  if (m_isStop) {
    return;
  }
  g_currentNumber = number;
  sm_mutex.lock();
  ++g_threadCounter;
//  emit downloaded(number);
  sm_mutex.unlock();
  UDownloadProcess* worker = new UDownloadProcess("curl", QStringList() << imageUrl.toString() << "--output" << fileName);
  QThread* thread = new QThread;
  worker->moveToThread(thread);

  connect(thread, &QThread::started, worker, &UDownloadProcess::process);
  connect(worker, &UDownloadProcess::finished, [thread, worker](){
    thread->quit();
    worker->deleteLater();
  });
  connect(thread, &QThread::finished, [this, thread, imageUrl, fileName, threadsQty, number](){
    thread->deleteLater();
    sm_mutex.lock();
    g_threadCounter--;
    sm_mutex.unlock();
    QPixmap mypix(fileName);
    if (mypix.isNull()) {
      curlDownload(imageUrl, fileName, threadsQty, number);
    }
    thread->deleteLater();
  });

  thread->start();
}

void FileDownloader::startDownload(const QQueue<QPair<QString, QString> > &tilesQue, int threadsCount)
{
  m_tilesQue = tilesQue;
  m_isStop = false;
  g_currentNumber = 0;
  QThread * thread = new QThread;
  moveToThread(thread);
  connect(thread, &QThread::started, this, [this, thread, threadsCount](){
    while (m_tilesQue.size() && !m_isStop) {
      QPair<QString, QString> data = m_tilesQue.front();
      m_tilesQue.pop_front();
      curlDownload(QUrl(data.first), data.second, threadsCount, m_tilesQue.size());
    }
    emit this->finished();
    thread->quit();
    thread->deleteLater();
    this->moveToThread(qApp->thread());
  }, Qt::DirectConnection);
  thread->start();
}

void FileDownloader::stopDownload()
{
  m_isStop = true;
}

int FileDownloader::currentNumber()
{
  return g_currentNumber;
}
