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
   * @name VARIABLES
   *******************************************/
  /// @{
protected:
  QObject *m_form;
  /// @}

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
  void sigMoved(
      QVariant a_fromX,
      QVariant a_fromY,
      QVariant a_toX,
      QVariant a_toY,
      QVariant a_type
    );
  void sigDestroyed (QVariant a_positions);
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
  void slotMoved (Position a_from, Position a_to);
  void slotGotLine (const QList<Position> &a_positions);

  /* GUI */

  void slotEntityClicked (int a_index);
  void slotMenuClicked();

  /// @}
};

/*-----------------------------------------*/
#endif // BRIDGEPAGEGAME_HPP
