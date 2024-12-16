#ifndef PLAYGROUNDFIELDMODEL_HPP
#define PLAYGROUNDFIELDMODEL_HPP

/* INCLUDES */

#include <QAbstractItemModel>

/****************************************//**
 * @brief Playground Field Model Class
 * @date 16.12.2024
 * @authors Segaman
 *******************************************/

class PlaygroundFieldModel : public QAbstractItemModel
{
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
