/* INCLUDES */

#include "playgroundfieldmodel.hpp"
#include "ctl/playgroundfieldcontroler.hpp"

/* DEFINES */

#define FIELDDATA(a_name) { int (FieldId::a_name), #a_name }

enum class FieldId : qint32
{
  type,
  x,
  y,
  selected,
};

struct PlaygroundFieldModel::Private
{
  PlaygroundFieldControler *controler = nullptr;
};

static QHash<int, QByteArray> s_roles =
{
  FIELDDATA (type),
  FIELDDATA (x),
  FIELDDATA (y),
  FIELDDATA (selected),
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

PlaygroundFieldModel::PlaygroundFieldModel (QObject *a_parent)
  : QAbstractTableModel (a_parent)
  , p (new Private)
{

}

PlaygroundFieldModel::~PlaygroundFieldModel()
{
  delete p;
}

/********************************************
 * METHODS
 *******************************************/

void PlaygroundFieldModel::attachControler (PlaygroundFieldControler *a_controler)
{
  if (a_controler == nullptr)
    return;

  beginResetModel();

  p->controler  = a_controler;

  connect (a_controler, &PlaygroundFieldControler::sigCreated,
           this, &PlaygroundFieldModel::slotCreated);
  connect (a_controler, &PlaygroundFieldControler::sigSelected,
           this, &PlaygroundFieldModel::slotSelected);
  connect (a_controler, &PlaygroundFieldControler::sigUnselected,
           this, &PlaygroundFieldModel::slotUnselected);
  connect (a_controler, &PlaygroundFieldControler::sigMoved,
           this, &PlaygroundFieldModel::slotMoved);
  connect (a_controler, &PlaygroundFieldControler::sigGotLine,
           this, &PlaygroundFieldModel::slotGotLine);
  connect (a_controler, &PlaygroundFieldControler::sigGameOver,
           this, &PlaygroundFieldModel::slotGameOver);
  connect (a_controler, &PlaygroundFieldControler::sigNewGame,
           this, &PlaygroundFieldModel::slotNewGame);
  connect (a_controler, &PlaygroundFieldControler::sigFieldCreated,
           this, &PlaygroundFieldModel::slotFieldCreated);
  connect (a_controler, &PlaygroundFieldControler::sigScoreChanged ,
           this, &PlaygroundFieldModel::slotScoreChanged);

  endResetModel();
}

void PlaygroundFieldModel::attachControler (QVariant a_controler)
{
  auto ctl  = a_controler.value<PlaygroundFieldControler*>();
  if (ctl == nullptr)
    return;
  attachControler (ctl);
}

void PlaygroundFieldModel::_updateItemAtPos (Position a_pos)
{
  auto itemIndex  = p->controler->coordToIndex (a_pos);
  auto index      = this->index (itemIndex, 0);
  emit dataChanged (index, index);
}

/********************************************
 * SLOTS
 *******************************************/

void PlaygroundFieldModel::slotCreated (Position a_pos)
{
  _updateItemAtPos (a_pos);
}

void PlaygroundFieldModel::slotSelected (Position a_selection)
{
  _updateItemAtPos (a_selection);
}

void PlaygroundFieldModel::slotUnselected (Position a_unselection)
{
  _updateItemAtPos (a_unselection);
}

void PlaygroundFieldModel::slotMoved (Position a_from, Position a_to)
{
  _updateItemAtPos (a_from);
  _updateItemAtPos (a_to);
}

void PlaygroundFieldModel::slotGotLine (const QList<Position> &a_positions)
{
  for (auto &pos : a_positions)
    _updateItemAtPos (pos);
}

void PlaygroundFieldModel::slotGameOver()
{

}

void PlaygroundFieldModel::slotNewGame()
{
  beginResetModel();
  endResetModel();
}

void PlaygroundFieldModel::slotFieldCreated()
{

}

void PlaygroundFieldModel::slotScoreChanged (quint32 a_value)
{

}

/********************************************
 * METHODS
 *******************************************/

int PlaygroundFieldModel::columnCount (const QModelIndex &a_parent) const
{
  return s_roles.size();
}

int PlaygroundFieldModel::rowCount (const QModelIndex &a_parent) const
{
  if (p->controler == nullptr)
    return 0;
  return p->controler->amount();
}

QVariant PlaygroundFieldModel::data (const QModelIndex &a_index, int a_role) const
{
  if (p->controler == nullptr)
    return QVariant();

  if (!a_index.isValid())
    return QVariant();

  /* check boundaries */
  if (a_index.row() < 0
      || a_index.row() >= rowCount())
    return QVariant();

  /* get entity */
  auto entity = p->controler->atIndex (a_index.row());

  /* return fields */
  switch (FieldId (a_role))
    {
      case FieldId::type:     return int (entity.type());
      case FieldId::x:        return entity.x();
      case FieldId::y:        return entity.y();
      case FieldId::selected:
        return p->controler->selected()
            == p->controler->indexToCoord (a_index.row());
    }

  return QVariant();
}

QHash<int, QByteArray> PlaygroundFieldModel::roleNames() const
{
  return s_roles;
}

/*-----------------------------------------*/
