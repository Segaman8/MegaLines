/* INCLUDES */

#include "playgroundfieldcontroler.hpp"

#include <QMap>
#include <QList>
#include <QSharedPointer>

/* DEFINES */

struct PlaygroundFieldControler::Private
{
  QList<Entity> field;

  quint16 width   = 9;
  quint16 height  = 9;
  quint16 score   = 0;

  Position selected;
  Position invalid;
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

PlaygroundFieldControler::PlaygroundFieldControler()
  : QObject()
  , p (new Private)
{
  qRegisterMetaType<PlaygroundFieldControler::Position> ("Position");
  qRegisterMetaType<quint32> ("quint32");

  clear();
}

PlaygroundFieldControler::~PlaygroundFieldControler()
{
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

PlaygroundFieldControler *PlaygroundFieldControler::instance()
{
  static PlaygroundFieldControler i;
  return &i;
}

quint32 PlaygroundFieldControler::score() const
{
  return p->score;
}


/* get entity */


const Entity &PlaygroundFieldControler::at (quint16 a_x, quint16 a_y) const
{
  /* check boundaries */
  static const Entity dummy;
  if (a_x >= p->width
      || a_y >= p->height)
    return dummy;

  /* get proper item */
  return p->field.at (coordToIndex (a_x, a_y));
}

Entity PlaygroundFieldControler::value (quint16 a_x, quint16 a_y) const
{
  /* check boundaries */
  static const Entity dummy;
  if (a_x >= p->width
      || a_y >= p->height)
    return dummy;

  /* get proper item */
  return p->field.at (coordToIndex (a_x, a_y));
}


/* interact with entities */


void PlaygroundFieldControler::createAt (quint16 a_x, quint16 a_y, Entity::Type a_type)
{
  /* check boundaries */
  if (a_x >= p->width
      || a_y >= p->height)
    return;

  /* variables */
  auto index    = coordToIndex (a_x, a_y);
  auto &entity  = p->field[index];

  /* check if free */
  if (entity.type() != Entity::Type::Free)
    return;

  /* update */
  entity.setType (a_type);

  emit sigCreated ({ a_x, a_y });
}

void PlaygroundFieldControler::createRandom()
{
  /* variables */
  auto freeSlot  = findFreeSlot();
  if (freeSlot == p->invalid)
    return emit sigGameOver();

  /* get entity */
  int index     = coordToIndex (freeSlot);
  auto &entity  = p->field[index];

  /* update and restore into used */
  entity.setType (getRandomType());

  emit sigCreated (freeSlot);
}

bool PlaygroundFieldControler::select (quint16 a_x, quint16 a_y)
{
  /* check boundaries */
  if (a_x >= p->width
      || a_y >= p->height)
    return false;

  /* get entity */
  auto index    = coordToIndex (a_x, a_y);
  auto &entity  = p->field[index];

  /* fail, if not used */
  if (entity.type() == Entity::Type::Free)
    return false;

  /* unselect, if already selected */
  Position newSelection { a_x, a_y };
  if (p->selected == newSelection)
    {
      p->selected = p->invalid;
      emit sigUnselected();
      return true;
    }

  /* select */
  p->selected = newSelection;
  emit sigSelected (p->selected);
  return true;
}

bool PlaygroundFieldControler::moveTo (quint16 a_x, quint16 a_y)
{
  /* check boundaries */
  if (a_x >= p->width
      || a_y >= p->height)
    return false;

  /* fail, if nothing selected */
  if (invalidSelection())
    return false;

  /* variables */
  Position current { a_x, a_y };

  /* get entity */
  auto index    = coordToIndex (current);
  auto &entity  = p->field[index];

  /* fail, if not free */
  if (entity.type() != Entity::Type::Free)
    return false;

  /* move */
  auto oldIndex       = coordToIndex (p->selected);
  p->field[oldIndex]  = std::move (p->field[index]);

  emit sigMoved (p->selected, current);
  return true;
}


/* field control */


void PlaygroundFieldControler::newGame (quint16 a_width, quint16 a_height)
{
  if (a_width == 0 || a_width < 9
      || a_height == 0 || a_height < 9)
    return;

  p->width   = a_width;
  p->height  = a_height;
  clear();
}

void PlaygroundFieldControler::clear()
{
  /* clear containers */

  p->field.clear();
  p->score    = 0;
  p->selected = p->invalid = Position { p->width, p->height };

  /* allocate field slots */

  p->field.reserve (p->height * p->height);
  for (int y = 0; y < p->height; y++)
    for (int x = 0; x < p->height; x++)
      p->field.append (Entity (Entity::Type::Free, x, y));
}

quint16 PlaygroundFieldControler::coordToIndex (Position a_position) const
{
  return a_position.x + a_position.y * p->width;
}

quint16 PlaygroundFieldControler::coordToIndex (quint16 a_x, quint16 a_y) const
{
  return a_x + a_y * p->width;
}

PlaygroundFieldControler::Position PlaygroundFieldControler::indexToCoord (quint16 a_index) const
{
  return
    Position
    {
      quint16 (a_index % p->width),
      quint16 (a_index / p->width)
    };
}

bool PlaygroundFieldControler::invalidSelection() const
{
  return p->selected == p->invalid;
}

PlaygroundFieldControler::Position PlaygroundFieldControler::findFreeSlot() const
{
  for (quint16 i = 0; i < p->field.size(); i++)
    if (p->field.at (i).type() == Entity::Type::Free)
      return indexToCoord (i);
  return p->invalid;
}

Entity::Type PlaygroundFieldControler::getRandomType()
{
  return Entity::Type (qrand() % int (Entity::Type::SIZE));
}

/*-----------------------------------------*/
