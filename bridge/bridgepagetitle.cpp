/* INCLUDES */

#include "bridgepagetitle.hpp"
#include "bridgemain.hpp"
#include "formbridgemanager.hpp"

#include "ctl/gamestoragecontroler.hpp"

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
  auto mainQml  = FormBridgeManager::mainQml()->as<BridgeMain>();

  /* start game */
  mainQml->switchToTab (1);
}

void BridgePageTitle::slotQuitClicked()
{
  DEBUGINFO;

  qApp->exit();
}

/*-----------------------------------------*/
