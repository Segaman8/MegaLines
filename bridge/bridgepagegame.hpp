#ifndef BRIDGEPAGEGAME_HPP
#define BRIDGEPAGEGAME_HPP

/* INCLUDES */

#include "formbridgebase.hpp"
#include "util/position.hpp"

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
  void sigScoreUpdate (QVariant a_score);
  /// @}

  /****************************************//**
   * @name SLOTS
   *******************************************/
  /// @{
public slots:

  /* Field */

  void slotScoreChanged (quint32 a_value);
  void slotNewGame();
  void slotGameOver();
  void slotGotLine (QList<Position> a_positions);

  /* GUI */

  void slotEntityClicked (int a_index);
  void slotMenuClicked();

  /// @}
};

/*-----------------------------------------*/
#endif // BRIDGEPAGEGAME_HPP
