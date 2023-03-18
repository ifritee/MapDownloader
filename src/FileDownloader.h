#pragma once

#include <QObject>
#include <QQueue>

/**
 * @class FileDownloader
 * @brief Скачивалка тайлов */
class FileDownloader : public QObject
{
  Q_OBJECT

public:
  /** @brief Конструктор */
  explicit FileDownloader(QObject *parent = nullptr);
  /** @brief Деструктор */
  virtual ~FileDownloader();
  /**
   * @brief curlDownload Скачивание при помощи curl
   * @param imageUrl путь к картинке
   * @param fileName имя сохраняемого файла */
  void curlDownload(QUrl imageUrl, QString fileName, int threadsQty, int);

  /**
   * @brief startDownload Запуск закачки в отдельном потоке
   * @param tilesQue набор закачек */
  void startDownload(const QQueue<QPair<QString, QString> > & tilesQue, int threadsCount);

  void stopDownload();

  static int currentNumber();

private:
  QQueue<QPair<QString, QString> > m_tilesQue;
  bool m_isStop = false;

signals:
  /** @brief Конец работы */
  void finished();

};

