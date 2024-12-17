/* INCLUDES */

#include "bridgepagetitle.hpp"
#include "bridgemain.hpp"
#include "formbridgemanager.hpp"

#include "ctl/gamestoragecontroler.hpp"
#include "ctl/playgroundfieldcontroler.hpp"

#include <QGuiApplication>
#include <QDebug>

/* DEFINES */

#define DEBUGINFO qDebug() << (QString ("BridgePageGame::") + __func__)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

BridgePageTitle::BridgePageTitle (QObject *a_parent)
  : FormBridgeBase (a_parent)
{

}

BridgePageTitle::~BridgePageTitle()
{

}

/********************************************
 * METHODS
 *******************************************/

void BridgePageTitle::connectSignalsAndSlots (QObject *a_qmlForm)
{
  FormBridgeBase::connectSignalsAndSlots (a_qmlForm);
}

QString BridgePageTitle::formName() const
{
  return "PageTitle";
}

/********************************************
 * SLOTS
 *******************************************/

void BridgePageTitle::slotContinueClicked()
{
  DEBUGINFO;

  /* variables */
  auto storage  = GameStorageControler::instance();
  auto mainQml  = FormBridgeManager::mainQml()->as<BridgeMain>();

  /* load state and start game */
  storage->load();
  mainQml->switchToTab (1);
}

void BridgePageTitle::slotNewGameClicked()
{
  DEBUGINFO;

  /* variables */
  auto ctl      = PlaygroundFieldControler::instance();
  auto mainQml  = FormBridgeManager::mainQml()->as<BridgeMain>();

  /* start new game */
  ctl->newGame (9, 9);

  /* create random entities */
  for (int i = 0; i < 3; i++)
    ctl->createRandom();

  /* start game */
  mainQml->switchToTab (1);
}

void BridgePageTitle::slotQuitClicked()
{
  DEBUGINFO;

  qApp->exit();
}

/*-----------------------------------------*/
