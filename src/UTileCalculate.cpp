#include "UTileCalculate.h"

#include <cmath>

UTileCalculate::UTileCalculate(EPROJ proj)
  : _Projection_en(proj)
{

}

void UTileCalculate::setProjection(UTileCalculate::EPROJ proj)
{
  _Projection_en = proj;
}

TILE_POS UTileCalculate::calculate(int zoom, double latitude, double longitude)
{
  switch (_Projection_en) {
  case UTileCalculate::P_3857:
    return worldToTile3857(zoom, latitude, longitude);
  case UTileCalculate::P_4326:
    return worldToTile4326(zoom, latitude, longitude);
  }
  return TILE_POS(0,0);
}

TILE_POS UTileCalculate::worldToTile3857(int zoom, double latitude, double longitude) const
{
  TILE_POS retValue;
  retValue.first = (int)(floor((longitude + 180.0) / 360.0 * (1 << zoom)));
  double latrad = latitude * M_PI/180.0;
  retValue.second = (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << zoom)));
  return retValue;
}

TILE_POS UTileCalculate::worldToTile4326(int zoom, double latitude, double longitude) const
{

  double radiusA = 6378137.0; // Вертикальный радиус земли
  double radiusB = 6356752.0; // Горизонтальный радиус земли
  double latitudeInRadians = latitude * M_PI / 180.0;
  double yCompressionOfEllipsoid = sqrt( pow(radiusA, 2.0) - pow(radiusB, 2.0)) / radiusA;
  double m2 = log((1.0 + sin(latitudeInRadians)) / (1.0 - sin(latitudeInRadians))) / 2.0 - yCompressionOfEllipsoid
      * log((1.0 + yCompressionOfEllipsoid * sin(latitudeInRadians)) / (1.0 - yCompressionOfEllipsoid * sin(latitudeInRadians))) / 2.0;
  double xTilesCountForThisZoom = (double)(1 << zoom);
  // Номер тайла в проекции
  double xTileNumber = floor((longitude + 180.0) / 360.0 * xTilesCountForThisZoom);
  double yTileNumber = floor(xTilesCountForThisZoom / 2.0 - m2 * xTilesCountForThisZoom / 2.0 / M_PI);

  // Сдвиг в пикселях для конкретных координат
  // левого верхнего угла тайла
  //  double offsetX = floor(((longitude + 180.0) / 360.0 * xTilesCountForThisZoom - xTileNumber) * 256.0);
  //  double offsetY = floor(((xTilesCountForThisZoom / 2.0 - m2 * xTilesCountForThisZoom / 2.0 / M_PI) - yTileNumber) * 256.0);


  return TILE_POS((int)xTileNumber, (int)yTileNumber);
}

//double UTileCalculate::lonFromX(int zoom, int x) const
//{
//  return x / (double)(1 << zoom) * 360.0 - 180;
//}

//double UTileCalculate::latFromY(int zoom, int y) const
//{
//  double n = M_PI - 2.0 * M_PI * y / (double)(1 << zoom);
//  return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
//}

//-----------------------------------------------------------
/*
func getWGS84Position(latitude: Double, longitude: Double, zoom: Int) -> (x:Int, y:Int, offsetX:Int, offsetY:Int) {

        // Earth vertical and horisontal radiuses
        let radiusA = 6378137.0
        let radiusB = 6356752.0

        let latitudeInRadians = latitude * Double.pi / 180

        let yCompressionOfEllipsoid = sqrt( pow(radiusA, 2.0) - pow(radiusB, 2.0)) / radiusA

        // I really don't know what the name of this variable mean =(
        let m2 = log((1 + sin(latitudeInRadians)) / (1 - sin(latitudeInRadians))) / 2 - yCompressionOfEllipsoid * log((1 + yCompressionOfEllipsoid * sin(latitudeInRadians)) / (1 - yCompressionOfEllipsoid * sin(latitudeInRadians))) / 2

        // x count = y count
        let xTilesCountForThisZoom = Double(1 << zoom)

        //Tile numbers in WGS-84 proection
        let xTileNumber = floor((longitude + 180) / 360 * xTilesCountForThisZoom)
        let yTileNumber = floor(xTilesCountForThisZoom / 2 - m2 * xTilesCountForThisZoom / 2 / Double.pi)

        //Offset in pixels of the coordinate of the
        //left-top corner of the OSM tile
        //from the left-top corner of the WGS-84 tile
        let offsetX = floor(((longitude + 180) / 360 * xTilesCountForThisZoom - xTileNumber) * 256)
        let offsetY = floor(((xTilesCountForThisZoom / 2 - m2 * xTilesCountForThisZoom / 2 / Double.pi) - yTileNumber) * 256)

        return (Int(xTileNumber), Int(yTileNumber), Int(offsetX), Int(offsetY))
    }
*/
