#include "MapAdapterLocal.h"

namespace qmapcontrol
{
  namespace
  {
    std::set<projection::EPSG> supportedProjections()
    {
      std::set<projection::EPSG> projections;
      projections.insert(projection::EPSG::SphericalMercator);
      return projections;
    }
  }

  MapAdapterLocal::MapAdapterLocal(QObject* parent)
    : MapAdapterTile(QUrl("http://127.0.0.1:8081/%zoom/%x/%y.png"), supportedProjections(), 0, 17, 0, false, parent) /// @todo remove once MSVC supports initializer lists.
  {

  }
}
