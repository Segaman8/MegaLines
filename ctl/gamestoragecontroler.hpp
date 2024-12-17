#ifndef GAMESTORAGECONTROLER_HPP
#define GAMESTORAGECONTROLER_HPP

/* INCLUDES */

/****************************************//**
 * @brief Game Storage Controler
 *
 * Used to save/load game state
 *
 * @date 17.12.2024
 * @authors Segaman
 *******************************************/

class GameStorageControler
{
  /****************************************//**
   * @name CONSTRUCT/DESTRUCT
   *******************************************/
  /// @{
protected:
  GameStorageControler();
  /// @}

  /****************************************//**
   * @name METHODS
   *******************************************/
  /// @{
public:
  static GameStorageControler *instance();

  void save();
  void load();
  /// @}
};

/*-----------------------------------------*/
#endif // GAMESTORAGECONTROLER_HPP
