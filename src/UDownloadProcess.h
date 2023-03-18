#pragma once

#include <QObject>

class QProcess;

/**
 * @class UDownloadProcess
 * @brief Класс управления процессом скачивания тайла */
class UDownloadProcess : public QObject
{
  Q_OBJECT
public:
  /** @brief Конструктор */
  explicit UDownloadProcess(const QString & procName, const QStringList & args, QObject *parent = nullptr);
  /** @brief Деструктор */
  virtual ~UDownloadProcess();

private:
  QString m_processName;  ///< @brief Имя запускаемой программы
  QStringList m_args; ///< @brief аргументы запускаемой программы
  QProcess * m_process; ///< @brief Процесс

public slots:
  /** @brief создает и запускает построитель отчетов */
  void process();
  /** @brief останавливает построитель отчетов */
  void stop();

signals:
  void finished(); 	///< @brief сигнал о завершении  работы построителя отчетов

};

