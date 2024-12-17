#ifndef BRIDGEMAIN_H
#define BRIDGEMAIN_H

/* INCLUDES */

#include "formbridgebase.hpp"
#include <QVariant>

/* DEFS */

QT_FORWARD_DECLARE_CLASS(QQuickWindow)

/****************************************//**
 * @brief Bridge for main.qml
 * @date 07.01.2024
 * @authors Segaman
 *******************************************/

class BridgeMain : public FormBridgeBase
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BridgeMain (QObject *a_parent = nullptr);
  ~BridgeMain();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void connectSignalsAndSlots (QObject *a_qmlForm);
  QString formName() const;

  QQuickWindow *window() const;
  void setWindow (QQuickWindow *a_window);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigSwitchTab (QVariant a_index);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  /// @}
};

/*-----------------------------------------*/
#endif // BRIDGEMAIN_H
