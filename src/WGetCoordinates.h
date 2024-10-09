#pragma once

#include <QDialog>
#include <QPointF>

#include "QMapControl/Point.h"

namespace qmapcontrol
{
  class QMapControl;
  class PointWorldCoord;
}

/**
 * @class WGetCoordinates
 * @date 09.10.2024
 * @brief Класс отображения карты для выбора области интереса
 *
 * @details Выбор осуществляется нажатием и удержанием правой кнопки мыши.
 * После выбора диалог будет закрыт. */
class WGetCoordinates : public QDialog
{
  Q_OBJECT
public:
  /** @brief Конструктор */
  explicit WGetCoordinates(QWidget *parent = nullptr);
  /** @brief Деструктор */
  virtual ~WGetCoordinates();
  /**
   * @brief Возвращает начальные координаты
   * @return Точка с координатами [lat;lon] */
  QPointF beginPoint() const;
  /**
   * @brief Возвращает конечные координаты
   * @return Точка с координатами [lat;lon] */
  QPointF endPoint() const;

protected:
  /** @brief Действия при перерисовки */
  virtual void resizeEvent(QResizeEvent * event) override;

private:
  qmapcontrol::QMapControl* _MapControl_po; ///< @brief Окно вывода карты

  QPointF m_beginPoint; ///< @brief Начальные координаты
  QPointF m_endPoint; ///< @brief Конечные координаты

private slots:
  /**
   * @brief Действия при отпускании кнопки мышки
   * @param [mouse_event] Параметры действия мышка
   * @param [press_coordinate] координаты нажатия мышки
   * @param [release_coordinate] координаты отпускания мышки */
  void mouseEventReleaseCoordinate(QMouseEvent* mouse_event, qmapcontrol::PointWorldCoord press_coordinate, qmapcontrol::PointWorldCoord release_coordinate);
};

