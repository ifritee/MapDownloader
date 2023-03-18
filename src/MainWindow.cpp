#include "MainWindow.h"

#include <QDir>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include <cmath>

#include "ui_MainWindow.h"
#include "FileDownloader.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_label(new QLabel(this))
  , m_FileDownloader(new FileDownloader())
  , m_timer(new QTimer(this))
{
  ui->setupUi(this);

  ui->statusbar->addPermanentWidget(m_label);

  m_zoomButtons.addButton(ui->zoom_0, 0);
  m_zoomButtons.addButton(ui->zoom_1, 1);
  m_zoomButtons.addButton(ui->zoom_2, 2);
  m_zoomButtons.addButton(ui->zoom_3, 3);
  m_zoomButtons.addButton(ui->zoom_4, 4);
  m_zoomButtons.addButton(ui->zoom_5, 5);
  m_zoomButtons.addButton(ui->zoom_6, 6);
  m_zoomButtons.addButton(ui->zoom_7, 7);
  m_zoomButtons.addButton(ui->zoom_8, 8);
  m_zoomButtons.addButton(ui->zoom_9, 9);
  m_zoomButtons.addButton(ui->zoom_10, 10);
  m_zoomButtons.addButton(ui->zoom_11, 11);
  m_zoomButtons.addButton(ui->zoom_12, 12);
  m_zoomButtons.addButton(ui->zoom_13, 13);
  m_zoomButtons.addButton(ui->zoom_14, 14);
  m_zoomButtons.addButton(ui->zoom_15, 15);
  m_zoomButtons.addButton(ui->zoom_16, 16);
  m_zoomButtons.addButton(ui->zoom_17, 17);
  m_zoomButtons.addButton(ui->zoom_18, 18);
  m_zoomButtons.setExclusive(false);
  m_timer->setInterval(1000);

  connect(ui->start_pb, &QPushButton::clicked, this, &MainWindow::download);
  connect(ui->server_cb, QOverload<int>::of(&QComboBox::activated), [this](int){
    if (ui->server_cb->currentIndex() == 0) {ui->key_le->setEnabled(true);}
    else {ui->key_le->setEnabled(false);}
  });
  connect(m_FileDownloader, &FileDownloader::finished, [this]() {
    ui->start_pb->setText(tr("Скачать"));
    m_label->setText(tr("Остановлено"));
    ui->key_le->setEnabled(true);
    ui->server_cb->setEnabled(true);
    ui->threadsQty_le->setEnabled(true);
    for(auto * button : m_zoomButtons.buttons()) {
      button->setEnabled(true);
      button->setChecked(false);
    }
  });
  connect(m_FileDownloader, &FileDownloader::finished, m_timer, &QTimer::stop);
  connect(m_timer, &QTimer::timeout, [this](){
    m_label->setText(tr("Осталось: ") + QString::number(FileDownloader::currentNumber()));
  });
}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_label;
  delete m_FileDownloader;
}

void MainWindow::download()
{
  if (ui->start_pb->text() == tr("Остановить") ) {
    ui->start_pb->setText(tr("Скачать"));
    m_FileDownloader->stopDownload();
    m_tilesQue.clear();
    return;
  }
  //1. dirs formation
  QString tileUrl = "";
  if (ui->server_cb->currentIndex() == 0) {
    tileUrl = "https://tile.thunderforest.com/atlas/%1/%2/%3.png?apikey=" + ui->key_le->text();
  } else if (ui->server_cb->currentIndex() == 1) {
    tileUrl = "https://tile.openstreetmap.org/%1/%2/%3.png";
  } else if (ui->server_cb->currentIndex() == 2) {
    tileUrl = "https://tile.opentopomap.org/%1/%2/%3.png";
  } else if (ui->server_cb->currentIndex() == 3) {
    tileUrl = "http://mt0.google.com/vt/lyrs=m&hl=ru&x=%2&y=%3&z=%1&s=Ga";
  }
  m_tilesQue.clear();
  QDir().mkpath("map_cache");
  for(auto * button : m_zoomButtons.buttons()) {
    if (button->isChecked()) { // is checked only
      int z = m_zoomButtons.id(button);
      QDir().mkpath("map_cache/" + QString::number(z));
      int qty = std::pow(2, z);
      for (int x = 0; x < qty; ++x) {
        QDir().mkpath("map_cache/" + QString::number(z) + "/" + QString::number(x));
        for (int y = 0; y < qty; ++y) {
          QString url = tileUrl.arg(z).arg(x).arg(y);
          QString fileName = QString("map_cache/%1/%2/%3.png").arg(z).arg(x).arg(y);
          if (QFile::exists(fileName)) {
            continue;
          }
          m_tilesQue.push_back(QPair<QString, QString>(url, fileName));
        }
      }
    }
  }
  ui->start_pb->setText(tr("Остановить"));
  m_label->setText(tr("Осталось: ") + QString::number(m_tilesQue.size()));
  m_FileDownloader->startDownload(m_tilesQue, ui->threadsQty_le->text().toUInt() + 1);
  ui->key_le->setEnabled(false);
  ui->server_cb->setEnabled(false);
  ui->threadsQty_le->setEnabled(false);
  for(auto * button : m_zoomButtons.buttons()) {
    button->setEnabled(false);
  }
  m_timer->start();
}
