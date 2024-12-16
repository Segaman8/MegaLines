/* INCLUDES */

#include "playgroundfieldmodel.hpp"

/* DEFINES */

#define FIELDDATA(a_name) { int (FieldId::a_name), #a_name }

enum class FieldId : qint32
{
  filename,
};

struct PlaygroundFieldModel::Private
{
  QStringList values;
};

static QHash<int, QByteArray> s_roles =
{
  FIELDDATA (filename),
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

PlaygroundFieldModel::PlaygroundFieldModel (QObject *a_parent)
  : QAbstractItemModel (a_parent)
{

}

PlaygroundFieldModel::~PlaygroundFieldModel()
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
  return p->values.size();
}

QVariant PlaygroundFieldModel::data (const QModelIndex &a_index, int a_role) const
{
  if (!a_index.isValid())
    return QVariant();

  /* check boundaries */
  if (a_index.row() < 0
      || a_index.row() >= p->values.size())
    return QVariant();

  /* return fields */
  switch (FieldId (a_role))
    {
    case FieldId::filename: return p->values.at (a_index.row());
    }

  return QVariant();
}

QHash<int, QByteArray> PlaygroundFieldModel::roleNames() const
{
  return s_roles;
}

/*-----------------------------------------*/
