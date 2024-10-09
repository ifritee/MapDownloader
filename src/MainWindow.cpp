#include "MainWindow.h"

#include <QDir>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <cmath>
#include <climits>

#include "ui_MainWindow.h"
#include "FileDownloader.h"
#include "UTileCalculate.h"
#include "WGetCoordinates.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_view(new Ui::MainWindow)
  , m_label(new QLabel(this))
  , m_fileDownloader(new FileDownloader())
  , m_timer(new QTimer(this))
  , m_coordinatesView_po(new WGetCoordinates(this))
{
  m_view->setupUi(this);

  m_view->statusbar->addPermanentWidget(m_label);

  m_zoomButtons.addButton(m_view->zoom_0, 0);
  m_zoomButtons.addButton(m_view->zoom_1, 1);
  m_zoomButtons.addButton(m_view->zoom_2, 2);
  m_zoomButtons.addButton(m_view->zoom_3, 3);
  m_zoomButtons.addButton(m_view->zoom_4, 4);
  m_zoomButtons.addButton(m_view->zoom_5, 5);
  m_zoomButtons.addButton(m_view->zoom_6, 6);
  m_zoomButtons.addButton(m_view->zoom_7, 7);
  m_zoomButtons.addButton(m_view->zoom_8, 8);
  m_zoomButtons.addButton(m_view->zoom_9, 9);
  m_zoomButtons.addButton(m_view->zoom_10, 10);
  m_zoomButtons.addButton(m_view->zoom_11, 11);
  m_zoomButtons.addButton(m_view->zoom_12, 12);
  m_zoomButtons.addButton(m_view->zoom_13, 13);
  m_zoomButtons.addButton(m_view->zoom_14, 14);
  m_zoomButtons.addButton(m_view->zoom_15, 15);
  m_zoomButtons.addButton(m_view->zoom_16, 16);
  m_zoomButtons.addButton(m_view->zoom_17, 17);
  m_zoomButtons.addButton(m_view->zoom_18, 18);
  m_zoomButtons.setExclusive(false);
  m_timer->setInterval(1000);

  m_coordinatesView_po->setMinimumSize(640, 480);
  m_coordinatesView_po->setVisible(false);

  connect(m_view->start_pb, &QPushButton::clicked, this, &MainWindow::download);
  connect(m_view->server_cb, QOverload<int>::of(&QComboBox::activated), [this](int){
    if (m_view->server_cb->currentIndex() == 0) {m_view->key_le->setEnabled(true);}
    else {m_view->key_le->setEnabled(false);}
  });
  connect(m_fileDownloader, &FileDownloader::finished, [this]() {
    m_view->start_pb->setText(tr("Скачать"));
    m_label->setText(tr("Остановлено"));
    m_view->key_le->setEnabled(true);
    m_view->server_cb->setEnabled(true);
    m_view->threadsQty_le->setEnabled(true);
    for(auto * button : m_zoomButtons.buttons()) {
      button->setEnabled(true);
      button->setChecked(false);
    }
  });
  connect(m_fileDownloader, &FileDownloader::finished, m_timer, &QTimer::stop);
  connect(m_timer, &QTimer::timeout, [this](){
    m_label->setText(tr("Осталось: ") + QString::number(FileDownloader::currentNumber()));
  });
  connect(m_view->showMap_pb, &QPushButton::clicked, this, &MainWindow::showMapWidget_slt);
}

MainWindow::~MainWindow()
{
  delete m_view;
  delete m_label;
  delete m_fileDownloader;
  delete m_coordinatesView_po;
}

void MainWindow::download()
{
  if (m_view->start_pb->text() == tr("Остановить") ) {
    m_view->start_pb->setText(tr("Скачать"));
    m_fileDownloader->stopDownload();
    m_tilesQue.clear();
    return;
  }
  // 0. begin and end tiles
  int xBegin = 0, yBegin = 0, xEnd = INT_MAX, yEnd = INT_MAX;
  UTileCalculate tileCalc;
  //1. dirs formation
  QString tileUrl = "";
  if (m_view->server_cb->currentIndex() == 0) {
    tileUrl = "https://tile.thunderforest.com/atlas/%1/%2/%3.png?apikey=" + m_view->key_le->text();
  } else if (m_view->server_cb->currentIndex() == 1) {
    tileUrl = "https://tile.openstreetmap.org/%1/%2/%3.png";
  } else if (m_view->server_cb->currentIndex() == 2) {
    tileUrl = "https://tile.opentopomap.org/%1/%2/%3.png";
  } else if (m_view->server_cb->currentIndex() == 3) {
    tileUrl = "http://mt0.google.com/vt/lyrs=m&hl=ru&x=%2&y=%3&z=%1&s=Ga";
  } else if (m_view->server_cb->currentIndex() == 4) {
     tileUrl = "https://core-renderer-tiles.maps.yandex.net/tiles?l=map&x=%2&y=%3&z=%1&scale=1&lang=ru_RU";
     tileCalc.setProjection(UTileCalculate::P_4326);
  } else if (m_view->server_cb->currentIndex() == 5) {
     tileUrl = "https://core-renderer-tiles.maps.yandex.net/tiles?l=map&theme=dark&x=%2&y=%3&z=%1&scale=1&lang=ru_RU";
     tileCalc.setProjection(UTileCalculate::P_4326);
  }
  // 3. Calculate tiles
  m_tilesQue.clear();
  QDir().mkpath("map_cache");
  for(auto * button : m_zoomButtons.buttons()) {
    if (button->isChecked()) { // is checked only
      int z = m_zoomButtons.id(button);
      if (!m_view->latBegin_le->text().isEmpty() && !m_view->lonBegin_le->text().isEmpty()) {
        auto point = tileCalc.calculate(z, m_view->latBegin_le->text().toDouble(), m_view->lonBegin_le->text().toDouble());
        xBegin = point.first; yBegin = point.second;
      }
      if (!m_view->latEnd_le->text().isEmpty() && !m_view->lonEnd_le->text().isEmpty()) {
        auto point = tileCalc.calculate(z, m_view->latEnd_le->text().toDouble(), m_view->lonEnd_le->text().toDouble());
        xEnd = point.first; yEnd = point.second;
      }
      QDir().mkpath("map_cache/" + QString::number(z));
      int qty = std::pow(2, z);
      for (int x = 0; x < qty; ++x) {
        if (x >= xBegin && x <= xEnd) {
          QDir().mkpath("map_cache/" + QString::number(z) + "/" + QString::number(x));
          for (int y = 0; y < qty; ++y) {
            QString url = tileUrl.arg(z).arg(x).arg(y);
            QString fileName = QString("map_cache/%1/%2/%3.png").arg(z).arg(x).arg(y);
            if (QFile::exists(fileName)) {
              continue;
            }
            if (y >= yBegin && y <= yEnd) {
              m_tilesQue.push_back(QPair<QString, QString>(url, fileName));
            }
          }
        }
      }
    }
  }
  m_view->start_pb->setText(tr("Остановить"));
  m_label->setText(tr("Осталось: ") + QString::number(m_tilesQue.size()));
  m_fileDownloader->startDownload(m_tilesQue, m_view->threadsQty_le->text().toUInt() + 1);
  m_view->key_le->setEnabled(false);
  m_view->server_cb->setEnabled(false);
  m_view->threadsQty_le->setEnabled(false);
  for(auto * button : m_zoomButtons.buttons()) {
    button->setEnabled(false);
  }
  m_timer->start();
}

void MainWindow::showMapWidget_slt()
{
  if (m_coordinatesView_po->exec() == QDialog::Accepted) {
    m_view->latBegin_le->setText(QString::number(m_coordinatesView_po->beginPoint().x()));
    m_view->lonBegin_le->setText(QString::number(m_coordinatesView_po->beginPoint().y()));
    m_view->latEnd_le->setText(QString::number(m_coordinatesView_po->endPoint().x()));
    m_view->lonEnd_le->setText(QString::number(m_coordinatesView_po->endPoint().y()));
  }
}
