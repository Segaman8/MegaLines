#ifndef PLAYGROUNDFIELDMODEL_HPP
#define PLAYGROUNDFIELDMODEL_HPP

/* INCLUDES */

#include <QAbstractItemModel>

#include "util/position.hpp"

/* DEFINES */

class PlaygroundFieldControler;

/****************************************//**
 * @brief Playground Field Model Class
 * @date 16.12.2024
 * @authors Segaman
 *******************************************/

class PlaygroundFieldModel : public QAbstractTableModel
{
  Q_OBJECT

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
public:
  explicit PlaygroundFieldModel (QObject *a_parent = nullptr);
  ~PlaygroundFieldModel();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void attachControler (PlaygroundFieldControler *a_controler);
  Q_INVOKABLE void attachControler (QVariant a_controler);

protected:
  void _updateItemAtPos (Position a_pos);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotCreated (Position a_pos);
  void slotSelected (Position a_selection);
  void slotUnselected (Position a_unselection);
  void slotMoved (Position a_from, Position a_to);
  void slotGotLine (const QList<Position> &a_positions);
  void slotGameOver();
  void slotNewGame();
  void slotFieldCreated();
  void slotScoreChanged (quint32 a_value);
  /// @}

  /****************************************//**
   * @name OVERRIDE
   *******************************************/
  /// @{
public:
  int columnCount (const QModelIndex &a_parent = QModelIndex()) const;
  int rowCount (const QModelIndex &a_parent = QModelIndex()) const;
  QVariant data (const QModelIndex &a_index, int a_role = Qt::DisplayRole) const;
  QHash<int, QByteArray> roleNames() const;
  /// @}
};

/*-----------------------------------------*/
#endif // PLAYGROUNDFIELDMODEL_HPP
