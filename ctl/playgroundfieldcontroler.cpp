/* INCLUDES */

#include "playgroundfieldcontroler.hpp"

#include <QMap>
#include <QList>
#include <QSharedPointer>

/* DEFINES */

typedef QSharedPointer<Entity> EntityPtr;

struct PlaygroundFieldControler::Private
{
  struct
  {
    QMap<quint16, EntityPtr> free;
    QMap<quint16, EntityPtr> used;
    QList<EntityPtr> field;

    quint16 width = 9,
            height = 9;
  } slot;

  quint16 score = 0;

  Position selected;
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


const PlaygroundFieldControler::EntityPtr &PlaygroundFieldControler::at (quint16 a_x, quint16 a_y) const
{
  /* check boundaries */
  static const PlaygroundFieldControler::EntityPtr dummy;
  if (a_x >= p->slot.width
      || a_y >= p->slot.height)
    return dummy;

  /* get proper item */
  return p->slot.field.at (coordToIndex (a_x, a_y));
}

Entity PlaygroundFieldControler::value (quint16 a_x, quint16 a_y) const
{
  /* check boundaries */
  static const Entity dummy;
  if (a_x >= p->slot.width
      || a_y >= p->slot.height)
    return dummy;

  /* get proper item */
  return *p->slot.field.at (coordToIndex (a_x, a_y)).get();
}


/* interact with entities */


void PlaygroundFieldControler::createAt (quint16 a_x, quint16 a_y, Entity::Type a_type)
{
  /* variables */
  auto index  = coordToIndex (a_x, a_y);

  /* check if free */
  if (!p->slot.free.contains (index))
    return;

  /* take entity from free */
  auto entity = p->slot.free.take (index);

  /* update and restore into used */
  entity->setType (quint16 (a_type));
  p->slot.used.insert (index, entity);

  /* put on field */
  p->slot.field[index] = entity;

  emit sigCreated ({ a_x, a_y });
}

void PlaygroundFieldControler::createRandom()
{
  /* variables */
  int size  = p->slot.free.size();
  if (size == 0)
    return emit sigGameOver();

  int index = qrand() % size;

  /* take entity */
  auto entity = p->slot.free.take (index);

  /* update and restore into used */
  entity->setType (quint16 (getRandomType()));
  p->slot.used.insert (index, entity);

  /* put on field */
  p->slot.field[index] = entity;

  emit sigCreated (indexToCoord (index));
}

bool PlaygroundFieldControler::select (quint16 a_x, quint16 a_y)
{
  /* fail, if not used */
  auto index  = coordToIndex (a_x, a_y);
  if (!p->slot.used.contains (index))
    return false;

  /* unselect, if already selected */
  Position newSelection { a_x, a_y };
  if (p->selected == newSelection)
    {
      p->selected = Position { p->slot.width, p->slot.height };
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
  /* fail, if nothing selected */
  if (invalidSelection())
    return false;

  /* fail, if not free */
  auto index  = coordToIndex (a_x, a_y);
  if (p->slot.used.contains (index))
    return false;

  /* move */
  auto oldIndex = coordToIndex (p->selected.x, p->selected.y);
  auto entity   = p->slot.used.take (oldIndex);
  p->slot.used.insert (index, entity);

  p->slot.field[oldIndex] = std::move (p->slot.field[index]);

  emit sigMoved (p->selected, Position { a_x, a_y });
  return true;
}


/* field control */


void PlaygroundFieldControler::setFieldSize (quint16 a_width, quint16 a_height)
{
  if (a_width == 0 || a_width < 9
      || a_height == 0 || a_height < 9)
    return;

  p->slot.width   = a_width;
  p->slot.height  = a_height;
  clear();
}

void PlaygroundFieldControler::clear()
{
  /* clear objects */

  p->slot.free.clear();
  p->slot.used.clear();
  p->score  = 0;

  /* allocate free and field slots */

  for (int y = 0; y < p->slot.height; y++)
    for (int x = 0; x < p->slot.height; x++)
      {
        quint16 index     = coordToIndex (x, y);

        /* free slot */
        p->slot.free.insert (index, EntityPtr::create (Entity::Type::Free, x, y));

        /* field slot */
        p->slot.field.append (EntityPtr::create (Entity::Type::Free, x, y));
      }
}

quint16 PlaygroundFieldControler::coordToIndex (Position a_position) const
{
  return a_position.x + a_position.y * p->slot.width;
}

quint16 PlaygroundFieldControler::coordToIndex (quint16 a_x, quint16 a_y) const
{
  return a_x + a_y * p->slot.width;
}

PlaygroundFieldControler::Position PlaygroundFieldControler::indexToCoord (quint16 a_index) const
{
  return
    Position
    {
      quint16 (a_index % p->slot.width),
      quint16 (a_index / p->slot.width)
    };
}

bool PlaygroundFieldControler::invalidSelection() const
{
  return p->selected.x == p->slot.width
         || p->selected.y == p->slot.height;
}

Entity::Type PlaygroundFieldControler::getRandomType()
{
  return Entity::Type (qrand() % int (Entity::Type::SIZE));
}

/*-----------------------------------------*/
