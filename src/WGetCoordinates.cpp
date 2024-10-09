#include "WGetCoordinates.h"

#include <memory>
#include <QVBoxLayout>

#include "QMapControl/MapAdapterOSM.h"
#include "QMapControl/QMapControl.h"
#include "QMapControl/LayerMapAdapter.h"

using namespace qmapcontrol;

WGetCoordinates::WGetCoordinates(QWidget *parent)
  : QDialog(parent)
  , _MapControl_po{new QMapControl(QSizeF(400.0, 590.0))}
{
  setLayout(new QVBoxLayout);
  layout()->addWidget(_MapControl_po);
  layout()->setMargin(0);

  _MapControl_po->enableZoomControls(true, false); // Включить управление увеличением
  std::shared_ptr<MapAdapter> map_adapter(std::make_shared<MapAdapterOSM>());
  _MapControl_po->addLayer(std::make_shared<LayerMapAdapter>("Map Layer", map_adapter));

  connect(_MapControl_po, &QMapControl::mouseEventReleaseCoordinate, this, &WGetCoordinates::mouseEventReleaseCoordinate);
}

WGetCoordinates::~WGetCoordinates()
{

}

QPointF WGetCoordinates::beginPoint() const
{
  return m_beginPoint;
}

QPointF WGetCoordinates::endPoint() const
{
  return m_endPoint;
}

void WGetCoordinates::resizeEvent(QResizeEvent * event)
{
  _MapControl_po->setViewportSize(event->size());
}

void WGetCoordinates::mouseEventReleaseCoordinate(QMouseEvent * mouse_event, PointWorldCoord press_coordinate, PointWorldCoord release_coordinate)
{
  if(mouse_event->button() == Qt::MouseButton::RightButton) {
    m_beginPoint.rx() = press_coordinate.latitude();
    m_beginPoint.ry() = press_coordinate.longitude();
    m_endPoint.rx() = release_coordinate.latitude();
    m_endPoint.ry() = release_coordinate.longitude();
    accept();
  }
}
