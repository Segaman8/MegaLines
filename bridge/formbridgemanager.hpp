#ifndef FORMBRIDGEMANAGER_H
#define FORMBRIDGEMANAGER_H

/* INCLUDES */

#include <QObject>

/* DEFINES */

class FormBridgeBase;

/****************************************//**
 * @brief Manager for all bridge classes
 * @date 06.01.2024
 * @authors Segaman
 *******************************************/

class FormBridgeManager : public QObject
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  FormBridgeManager();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static FormBridgeManager *instance();

  Q_INVOKABLE void connectQmlForm (QObject *a_qmlForm);
  static FormBridgeBase *mainQml();
  /// @}
};

/*-----------------------------------------*/
#endif // FORMBRIDGEMANAGER_H
