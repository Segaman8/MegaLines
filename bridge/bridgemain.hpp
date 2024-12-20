#ifndef BRIDGEMAIN_H
#define BRIDGEMAIN_H

/* INCLUDES */

#include "formbridgebase.hpp"
#include <QVariant>

/* DEFINES */

QT_FORWARD_DECLARE_CLASS(QQuickWindow)

/****************************************//**
 * @brief Bridge for main.qml
 * @date 17.12.2024
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

  void switchToTab (int a_tab);
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
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
