#pragma once

#include <string>

using WRLD_COORD = std::pair<double, double>;
using TILE_POS = std::pair<int, int>;

/**
 * @class UTileCalculate
 * @brief Расчет плитки по координатам и увеличению */
class UTileCalculate
{
public:

  /**
   * @enum EPROJ
   * @brief Список проекций */
  enum EPROJ {
    P_3857 ///< @brief Прямоугольная географическая проекция
  , P_4326 ///< @brief Проекция WGS84
  };

  /**
   * @brief Конструктор по-умолчанию */
  explicit UTileCalculate(EPROJ proj = P_3857);
  /**
   * @brief Устанавливает проекцию */
  void setProjection(EPROJ proj);
  /**
   * @brief Функция расчета координат
   * @param [zoom] Увеличение 0-19
   * @param [latitude] Широта в градусах
   * @param [longitude] Долгота в градусах
   * @return Пара значений {X, Y} */
  TILE_POS calculate(int zoom, double latitude, double longitude);

private:
  EPROJ _Projection_en; ///< @brief Проекция

  TILE_POS worldToTile3857(int zoom, double latitude, double longitude) const;
  TILE_POS worldToTile4326(int zoom, double latitude, double longitude) const;

//  /**
//   * @brief Преобразует левый угол тайла в долготу
//   * @param [zoom] Увеличение
//   * @param [x] номер тайла по x
//   * @return Долгота в градусах*/
//  double lonFromX(int zoom, int x) const;
//  /**
//   * @brief Преобразует верхний угол тайла в широту
//   * @param [zoom] Увеличение
//   * @param [x] номер тайла по x
//   * @return Долгота в градусах*/
//  double latFromY(int zoom, int y) const;
};

