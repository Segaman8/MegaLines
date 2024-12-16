/* INCLUDES */

#include "entity.hpp"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

Entity::Entity()
  : m_type (Type::Free)
  , m_x (0)
  , m_y (0)
{

}

Entity::Entity (Type a_type, quint16 a_x, quint16 a_y)
  : m_type (a_type)
  , m_x (a_x)
  , m_y (a_y)
{

}

Entity::Entity (const Entity &a_src)
{
  operator= (a_src);
}

Entity::Entity (Entity &&a_src)
{
  operator= (std::move (a_src));
}

/********************************************
 * METHODS
 *******************************************/

quint16 Entity::type() const
{
  return quint16 (m_type);
}

void Entity::setType (quint16 a_value)
{
  if (quint16 (m_type) == a_value)
    return;

  m_type  = Type (a_value);
  emit sigTypeChanged();
}



quint16 Entity::x() const
{
  return m_x;
}

void Entity::setX (quint16 a_value)
{
  if (m_x == a_value)
    return;

  m_x  = a_value;
  emit sigPosChanged();
}



quint16 Entity::y() const
{
  return m_y;
}

void Entity::setY (quint16 a_value)
{
  if (m_y == a_value)
    return;

  m_y  = a_value;
  emit sigPosChanged();
}

void Entity::setPos (quint16 a_x, quint16 a_y)
{
  if (m_x == a_x
      && m_y == a_y)
    return;

  m_x = a_x;
  m_y = a_y;
  emit sigPosChanged();
}

/********************************************
 * OPERATORS
 *******************************************/

Entity &Entity::operator= (const Entity &a_other)
{
  if (this != &a_other)
  {
    m_type  = a_other.m_type;
    m_x     = a_other.m_x;
    m_y     = a_other.m_y;
  }
  return *this;
}

Entity &Entity::operator= (Entity &&a_other)
{
  if (this != &a_other)
  {
    m_type  = std::move (a_other.m_type);
    m_x     = std::move (a_other.m_x);
    m_y     = std::move (a_other.m_y);
  }
  return *this;
}

/*-----------------------------------------*/
