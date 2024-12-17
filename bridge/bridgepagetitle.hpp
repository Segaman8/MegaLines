#ifndef BRIDGEPAGETITLE_HPP
#define BRIDGEPAGETITLE_HPP

/* INCLUDES */

#include "formbridgebase.hpp"

/****************************************//**
 * @brief Bridge for PageTitle
 * @date 17.12.2024
 * @authors Segaman
 *******************************************/

class BridgePageTitle : public FormBridgeBase
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BridgePageTitle (QObject *a_parent = nullptr);
  ~BridgePageTitle();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void connectSignalsAndSlots (QObject *a_qmlForm);
  QString formName() const;
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
  void slotContinueClicked();
  void slotNewGameClicked();
  void slotQuitClicked();
  /// @}
};

/*-----------------------------------------*/
#endif // BRIDGEPAGETITLE_HPP
