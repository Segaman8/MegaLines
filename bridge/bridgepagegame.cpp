/* INCLUDES */

#include "bridgepagegame.hpp"

#include <QDebug>

/* DEFINES */

#define DEBUGINFO qDebug() << (QString ("BridgePageGame::") + __func__)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

BridgePageGame::BridgePageGame (QObject *a_parent)
  : FormBridgeBase (a_parent)
{

}

BridgePageGame::~BridgePageGame()
{

}

/********************************************
 * METHODS
 *******************************************/

void BridgePageGame::connectSignalsAndSlots (QObject *a_qmlForm)
{
  FormBridgeBase::connectSignalsAndSlots (a_qmlForm);
}

QString BridgePageGame::formName() const
{
  return "PageGame";
}

/********************************************
 * SLOTS
 *******************************************/

void BridgePageGame::slotEntityClicked (int a_index)
{
  DEBUGINFO << a_index;
}

/*-----------------------------------------*/
