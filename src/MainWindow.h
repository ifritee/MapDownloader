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
class WGetCoordinates;

/**
 * @class MainWindow
 * @brief Основное окно скачивалки тайлов */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /** @brief Конструктор */
  explicit MainWindow(QWidget *parent = nullptr);
  /** @brief Деструктор */
  virtual ~MainWindow();

private:
  Ui::MainWindow * m_view; ///< @brief Вид интерфейса пользователя

  QButtonGroup m_zoomButtons; ///< @brief Zoom buttons group
  QQueue<QPair<QString, QString> > m_tilesQue; ///< @brief Queue from tiles
  QLabel * m_label;
  FileDownloader * m_fileDownloader; ///< @brief Загрузчик файлов

  QTimer * m_timer; ///< @brief Таймер вывода оставшихся файлов
  WGetCoordinates * m_coordinatesView_po; ///< @brief Выбор точек на карте

private slots:
  /**
   * @brief Слот закачки тайлов */
  void download();
  /**
   * @brief Слот отображения диалога выбора координат */
  void showMapWidget_slt();
};
