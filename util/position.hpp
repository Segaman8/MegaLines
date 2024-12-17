#ifndef POSITION_HPP
#define POSITION_HPP

/* INCLUDES */

#include <QtCore>

/****************************************//**
 * @brief Position Class
 * @date 17.12.2024
 * @authors Segaman
 *******************************************/

struct Position
{
  quint16 x, y;

  bool operator==(const Position &a_other)
  {
    return x == a_other.x
        && y == a_other.y;
  }
};

typedef QList<Position> PositionList;

/*-----------------------------------------*/
#endif // POSITION_HPP
