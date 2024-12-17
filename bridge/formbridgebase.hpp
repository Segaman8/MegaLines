#ifndef FORMBRIDGEBASE_H
#define FORMBRIDGEBASE_H

/* INCLUDES */

#include <QObject>
#include <QDebug>

/****************************************//**
 * @brief Base class for Qml Form Bridge
 * @date 06.01.2024
 * @authors Segaman
 *******************************************/

class FormBridgeBase : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit FormBridgeBase (QObject *a_parent = nullptr);
  virtual ~FormBridgeBase();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  /**
   * @brief connects signals (starts with "sig")
   * to slots (starts with "slot") in both directions
   */
  virtual void connectSignalsAndSlots (QObject *a_object);

  /**
   * @brief returns associated form name
   */
  virtual QString formName() const;

  template<class T>
  T *findChild (QObject *a_object, QString a_objectName, QString a_errorPrefix)
  {
    T* result = a_object->findChild<T*> (a_objectName);

    if (result == nullptr)
      {
        qDebug() << QString ("%1 : unable to find element \"%2\"")
                    .arg (a_errorPrefix, a_objectName);
        return nullptr;
      }

    return result;
  }

  template<class T>
  T *as () const
  {
    return dynamic_cast<T*> (this);
  }

private:
  void _connectDestSignalsToBridgeSlots (QObject *a_object);
  void _connectBridgeSignalsToDestSlots (QObject *a_object);
  /// @}
};

/*-----------------------------------------*/
#endif // FORMBRIDGEBASE_H
