#ifndef ENTITY_HPP
#define ENTITY_HPP

/* INCLUDES */

#include <QObject>

/****************************************//**
 * @brief Ingame Entity Class
 * @date 16.12.2024
 * @authors Segaman
 *******************************************/

class Entity
{
  /****************************************//**
   * @name DEFINES
   *******************************************/
  /// @{
public:

  enum class Type : quint16
  {
    Red,
    Blue,
    Green,
    Yellow,

    SIZE,

    Free = SIZE,
  };

  /// @}

  /****************************************//**
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  Type m_type;
  quint16 m_x, m_y;
  /// @}

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  Entity();
  Entity (Type a_type, quint16 a_x, quint16 a_y);
  Entity (const Entity &a_src);
  Entity (Entity &&a_src);
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  Type type() const;
  void setType (Type a_value);

  quint16 x() const;
  void setX (quint16 a_value);

  quint16 y() const;
  void setY (quint16 a_value);

  void setPos (quint16 a_x, quint16 a_y);
  /// @}

  /****************************************//**
   * @name OPERATORS
   *******************************************/
  /// @{
public:
  Entity &operator= (const Entity &a_other);
  Entity &operator= (Entity &&a_other);
  /// @}
};

/*-----------------------------------------*/
#endif // ENTITY_HPP
