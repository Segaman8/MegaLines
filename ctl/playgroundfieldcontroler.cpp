/* INCLUDES */

#include "playgroundfieldcontroler.hpp"

#include <QMap>
#include <QList>
#include <QSharedPointer>
#include <functional>

/* DEFINES */

struct PlaygroundFieldControler::Private
{
  QSet<quint16> freeIndexes;
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
  qRegisterMetaType<Position> ("Position");
  qRegisterMetaType<PositionList> ("PositionList");
  qRegisterMetaType<quint32> ("quint32");
  qRegisterMetaType<Entity::Type> ("EntityType");

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

void PlaygroundFieldControler::setScore (quint32 a_value)
{
  if (p->score == a_value)
    return;

  p->score  = a_value;
  emit sigScoreChanged (p->score);
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

const Entity &PlaygroundFieldControler::atIndex (quint16 a_index) const
{
  /* check boundaries */
  static const Entity dummy;
  if (a_index >= p->field.size())
    return dummy;

  /* get proper item */
  return p->field.at (a_index);
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

Entity PlaygroundFieldControler::valueAtIndex (quint16 a_index) const
{
  /* check boundaries */
  static const Entity dummy;
  if (a_index >= p->field.size())
    return dummy;

  /* get proper item */
  return p->field.at (a_index);
}

quint16 PlaygroundFieldControler::width() const
{
  return p->width;
}

quint16 PlaygroundFieldControler::height() const
{
  return p->height;
}

QSize PlaygroundFieldControler::size() const
{
  return QSize { p->width, p->height };
}

quint16 PlaygroundFieldControler::amount() const
{
  return width() * height();
}

Position PlaygroundFieldControler::selected() const
{
  return p->selected;
}


/* interact with entities */


bool PlaygroundFieldControler::createAt (quint16 a_x, quint16 a_y, Entity::Type a_type)
{
  /* check boundaries */
  if (a_x >= p->width
      || a_y >= p->height
      || a_type == Entity::Type::Free)
    return false;

  /* variables */
  auto index    = coordToIndex (a_x, a_y);
  auto &entity  = p->field[index];

  /* check if free */
  if (entity.type() != Entity::Type::Free)
    return false;

  /* remove from free set */
  p->freeIndexes.remove (index);

  /* update */
  entity.setType (a_type);

  emit sigCreated ({ a_x, a_y });
  if (p->freeIndexes.size() == 0)
    emit sigGameOver();
  return true;
}

bool PlaygroundFieldControler::createRandom()
{
  /* check if no free left */
  if (p->freeIndexes.size() == 0)
  {
    emit sigGameOver();
    return false;
  }

  /* variables */
  quint16 index;

  /* get free index */
  {
    quint16 freeIndexInt  = QRandomGenerator::global()->generate() % p->freeIndexes.size();
    auto freeIndex        = p->freeIndexes.begin() + freeIndexInt;
    index                 = *freeIndex;
    p->freeIndexes.erase (freeIndex);
  }

  /* get entity */
  auto &entity  = p->field[index];

  /* update and restore into used */
  entity.setType (getRandomType());

  emit sigCreated (indexToCoord (index));
  if (p->freeIndexes.size() == 0)
    emit sigGameOver();
  return true;
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

  /* unselect, if already selected */
  Position newSelection { a_x, a_y };
  if (p->selected == newSelection)
    {
      auto unselected = p->selected;
      p->selected = p->invalid;
      emit sigUnselected (unselected);
      return true;
    }

  /* fail, if not used */
  if (entity.type() == Entity::Type::Free)
    return false;

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
  if (_invalidSelection())
    return false;

  /* variables */
  Position current { a_x, a_y };

  /* get entity */
  auto index    = coordToIndex (current);
  auto &entity  = p->field[index];

  /* unselect, if already selected */
  Position newSelection { a_x, a_y };
  if (p->selected == newSelection)
    {
      auto unselected = p->selected;
      p->selected = p->invalid;
      emit sigUnselected (unselected);
      return false;
    }

  /* fail, if not free */
  if (entity.type() != Entity::Type::Free)
    return false;

  /* move */
  auto oldIndex       = coordToIndex (p->selected);
  p->field[index].setType (p->field[oldIndex].type());
  p->field[oldIndex].setType (Entity::Type::Free);

  /* switch used and free spaces */
  p->freeIndexes.remove (index);
  p->freeIndexes << oldIndex;

  auto movedItem  = p->selected;
  p->selected = p->invalid;
  _detectLine (current);
  emit sigMoved (movedItem, current);
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

  emit sigNewGame();
}

void PlaygroundFieldControler::clear()
{
  /* clear containers */

  p->field.clear();
  p->freeIndexes.clear();
  p->score    = 0;
  p->selected = p->invalid = Position { p->width, p->height };

  /* allocate field slots */

  p->field.reserve (p->height * p->height);
  for (int y = 0; y < p->height; y++)
    for (int x = 0; x < p->height; x++)
    {
      p->freeIndexes << coordToIndex (x, y);
      p->field.append (Entity (Entity::Type::Free, x, y));
    }

  emit sigFieldCreated();
  emit sigScoreChanged (p->score);
}

quint16 PlaygroundFieldControler::coordToIndex (Position a_position) const
{
  return a_position.x + a_position.y * p->width;
}

quint16 PlaygroundFieldControler::coordToIndex (quint16 a_x, quint16 a_y) const
{
  return a_x + a_y * p->width;
}

Position PlaygroundFieldControler::indexToCoord (quint16 a_index) const
{
  return
    Position
    {
      quint16 (a_index % p->width),
      quint16 (a_index / p->width)
    };
}

bool PlaygroundFieldControler::_invalidSelection() const
{
  return p->selected == p->invalid;
}

// Position PlaygroundFieldControler::findFreeSlot() const
// {
//   for (quint16 i = 0; i < p->field.size(); i++)
//     if (p->field.at (i).type() == Entity::Type::Free)
//       return indexToCoord (i);
//   return p->invalid;
// }

Entity::Type PlaygroundFieldControler::getRandomType()
{
  return Entity::Type (QRandomGenerator::global()->generate() % int (Entity::Type::SIZE));
}

void PlaygroundFieldControler::_detectLine (Position a_position)
{
  /* defines */

  enum Direction
  {
    Horizontal,
    Vertical,
    DiagonalLeft,
    DiagonalRight,
  };

  typedef std::function<void()> Lambda;

  /* variables */

  quint8 counter;
  Entity::Type desiredType;

  QList<Position> found[4] =
  {
    { a_position },
    { a_position },
    { a_position },
    { a_position },
  };

  /* get desired type */

  {
    auto index      = coordToIndex (a_position);
    Entity &entity  = p->field[index];
    desiredType     = entity.type();
  }

  /* lambda's */

  auto searchForItems =
    [this, a_position, &counter, &found, desiredType] (
      qint16 a_horAccel,
      qint16 a_verAccel,
      int a_category)
  {
    /* variables */
    Position position
    {
      quint16 (a_position.x + a_horAccel),
      quint16 (a_position.y + a_verAccel)
    };

    while (counter < 5
           && position.x < p->width
           && position.y < p->height)
    {
      /* variables */
      int index = coordToIndex (position);

      /* fail */
      if (p->field[index].type() != desiredType)
        return;

      /* found */
      found[a_category].append (position);
      counter++;

      /* apply accel */
      position.x = quint16 (position.x + a_horAccel);
      position.y = quint16 (position.y + a_verAccel);
    }
  };

  Lambda lambdaDirections[4] =
  {
    /* Horizontal */
    [&counter, searchForItems]()
    {
      counter = 1;
      searchForItems (-1, 0, Direction::Horizontal);
      searchForItems (1, 0, Direction::Horizontal);
    },

    /* Vertical */
    [&counter, searchForItems]()
    {
      counter = 1;
      searchForItems (0, -1, Direction::Vertical);
      searchForItems (0, 1, Direction::Vertical);
    },

    /* DiagonalLeft */
    [&counter, searchForItems]()
    {
      counter = 1;
      searchForItems (-1, -1, Direction::DiagonalLeft);
      searchForItems (1, 1, Direction::DiagonalLeft);
    },

    /* DiagonalRight */
    [&counter, searchForItems]()
    {
      counter = 1;
      searchForItems (1, -1, Direction::DiagonalRight);
      searchForItems (-1, 1, Direction::DiagonalRight);
    },
  };

  /* execute search in four directions */

  for (int i = 0; i < 4; i++)
  {
    /* execute direction search */

    auto lambda = lambdaDirections[i];
    lambda();

    /* check if found enough items */

    if (counter != 5)
      continue;

    /* send signal */

    _scored();
    _destroyEntities (found[i]);
    emit sigGotLine (std::move (found[i]));
    break;
  }
}

void PlaygroundFieldControler::_scored()
{
  p->score += 10;
  emit sigScoreChanged (p->score);
}

void PlaygroundFieldControler::_destroyEntities (const QList<Position> &a_positions)
{
  for (auto &pos : a_positions)
  {
    auto index  = coordToIndex (pos);
    p->field[index].setType (Entity::Type::Free);
    p->freeIndexes << index;
  }
}

/*-----------------------------------------*/
