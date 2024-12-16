#ifndef PLAYGROUNDFIELDCONTROLER_HPP
#define PLAYGROUNDFIELDCONTROLER_HPP

/* INCLUDES */

#include <QObject>

#include "objects/entity.hpp"

/****************************************//**
 * @brief Playground Field Controler Class
 * @date 16.12.2024
 * @authors Segaman
 *******************************************/

class PlaygroundFieldControler : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name DEFINES
   *******************************************/
  /// @{
protected:

  struct Position
  {
    quint16 x, y;

    bool operator==(const Position &a_other)
    {
      return x == a_other.x
          && y == a_other.y;
    }
  };

  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  struct Private; Private *p;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  explicit PlaygroundFieldControler();
  ~PlaygroundFieldControler();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static PlaygroundFieldControler *instance();
  quint32 score() const;

  /* get entity */

  const Entity &at (quint16 a_x, quint16 a_y) const;
  Entity value (quint16 a_x, quint16 a_y) const;

  /* interact with entities */

  void createAt (quint16 a_x, quint16 a_y, Entity::Type a_type);
  void createRandom();
  bool select (quint16 a_x, quint16 a_y);
  bool moveTo (quint16 a_x, quint16 a_y);

  /* field control */

  void newGame (quint16 a_width, quint16 a_height);
  void clear();

  quint16 coordToIndex (Position a_position) const;
  quint16 coordToIndex (quint16 a_x, quint16 a_y) const;
  Position indexToCoord (quint16 a_index) const;
  bool invalidSelection() const;
  Position findFreeSlot() const;
  static Entity::Type getRandomType();
  void detectLine (Position a_position);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigCreated (Position a_pos);
  void sigSelected (Position a_selection);
  void sigUnselected();
  void sigMoved (Position a_from, Position a_to);
  void sigGotLine (QList<Position> a_positions);
  void sigGameOver();
  void sigScoreChanged (quint32 a_value);
  /// @}
};

/*-----------------------------------------*/
#endif // PLAYGROUNDFIELDCONTROLER_HPP
