#pragma once

#include <QMainWindow>
#include <QButtonGroup>
#include <QQueue>
#include <QPair>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTimer;
class FileDownloader;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  QButtonGroup m_zoomButtons; ///< @brief Zoom buttons group
  QQueue<QPair<QString, QString> > m_tilesQue; ///< @brief Queue from tiles
  QLabel * m_label;
  FileDownloader * m_FileDownloader; ///< @brief Загрузчик файлов

  QTimer * m_timer; ///< @brief Таймер вывода оставшихся файлов

public slots:
  void download();
};
