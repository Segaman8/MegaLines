#ifndef BRIDGEPAGEGAME_HPP
#define BRIDGEPAGEGAME_HPP

/* INCLUDES */

#include "formbridgebase.hpp"

/****************************************//**
 * @brief Bridge for PageGame
 * @date 17.12.2024
 * @authors Segaman
 *******************************************/

class BridgePageGame : public FormBridgeBase
{
  Q_OBJECT

  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
public:
  explicit BridgePageGame (QObject *a_parent = nullptr);
  ~BridgePageGame();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  void connectSignalsAndSlots (QObject *a_qmlForm);
  QString formName() const;
protected:
  void _spawnNewEntities();
  /// @}

  /****************************************//**
   * @name SIGNALS
   *******************************************/
  /// @{
signals:
  void sigScoreUpdate (int a_score);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:
  void slotEntityClicked (int a_index);
  /// @}
};

/*-----------------------------------------*/
#endif // BRIDGEPAGEGAME_HPP
