QT += network widgets

CONFIG += c++11

# Capture whether this is a release/debug build.
CONFIG(debug, debug|release) {
    # Target name.
    TARGET = qmapcontrold
}
CONFIG(release, debug|release) {
    # Target name.
    TARGET = qmapcontrol
}

# Target version.
VERSION = 1.0.0

# Build a library.
TEMPLATE = lib

# Required defines.
DEFINES += QMAPCONTROL_LIBRARY      \
    # Windows: To force math constants to be defined in <cmath>
    _USE_MATH_DEFINES               \
    # Windows: Extend MSVC 11 to allow more than 5 arguments for its 'faux' variadic templates.
    # @todo remove once MSVC supports standards-based variadic templates.
    _VARIADIC_MAX=6


# Add header files.
HEADERS +=                                      \
    qmapcontrol_global.h                        \
    Geometry.h                                  \
    GeometryLineString.h                        \
    GeometryPoint.h                             \
    GeometryPointShape.h                        \
    GeometryPointShapeScaled.h                  \
    GeometryPointArrow.h                        \
    GeometryPointCircle.h                       \
    GeometryPointImage.h                        \
    GeometryPointImageScaled.h                  \
    GeometryPolygon.h                           \
    GeometryPolygonImage.h                      \
    GeometryWidget.h                            \
    GPS_Position.h                              \
    ImageManager.h                              \
    Layer.h                                     \
    LayerGeometry.h                             \
    LayerMapAdapter.h                           \
    MapAdapter.h                                \
    MapAdapterGoogle.h                          \
    MapAdapterBing.h                            \
    MapAdapterOSM.h                             \
    MapAdapterOTM.h                             \
    MapAdapterTile.h                            \
    MapAdapterWMS.h                             \
    MapAdapterYahoo.h                           \
    NetworkManager.h                            \
    Point.h                                     \
    Projection.h                                \
    ProjectionEquirectangular.h                 \
    ProjectionSphericalMercator.h               \
    QMapControl.h                               \
    QuadTreeContainer.h                         \
# Third-party headers: QProgressIndicator
    QProgressIndicator.h                        \

# Add source files.
SOURCES +=                                      \
    Geometry.cpp                                \
    GeometryLineString.cpp                      \
    GeometryPoint.cpp                           \
    GeometryPointShape.cpp                      \
    GeometryPointShapeScaled.cpp                \
    GeometryPointArrow.cpp                      \
    GeometryPointCircle.cpp                     \
    GeometryPointImage.cpp                      \
    GeometryPointImageScaled.cpp                \
    GeometryPolygon.cpp                         \
    GeometryPolygonImage.cpp                    \
    GeometryWidget.cpp                          \
    GPS_Position.cpp                            \
    ImageManager.cpp                            \
    Layer.cpp                                   \
    LayerGeometry.cpp                           \
    LayerMapAdapter.cpp                         \
    MapAdapter.cpp                              \
    MapAdapterGoogle.cpp                        \
    MapAdapterBing.cpp                          \
    MapAdapterOSM.cpp                           \
    MapAdapterOTM.cpp                           \
    MapAdapterTile.cpp                          \
    MapAdapterWMS.cpp                           \
    MapAdapterYahoo.cpp                         \
    NetworkManager.cpp                          \
    Projection.cpp                              \
    ProjectionEquirectangular.cpp               \
    ProjectionSphericalMercator.cpp             \
    QMapControl.cpp                             \
# Third-party sources: QProgressIndicator
    QProgressIndicator.cpp


# Capture whether this is a release/debug build.
CONFIG(debug, debug|release) {
    TARGET_TYPE = debug
}
CONFIG(release, debug|release) {
    TARGET_TYPE = release
}

# Install details.
# Has a prefix been specified?
!isEmpty(prefix) {
    # Change prefix to PREFIX.
    PREFIX = $${prefix}/$$TARGET_TYPE
}
isEmpty(PREFIX) {
    # Default to parent directory.
    PREFIX = ../../../$$TARGET_TYPE
}
# Install headers to $${PREFIX}/include/QMapControl
headers.path = $${PREFIX}/include/QMapControl
headers.files = $${HEADERS}
# Install target to $${PREFIX}/lib.
target.path = $${PREFIX}/lib
# Install headers and target.
INSTALLS += target headers
