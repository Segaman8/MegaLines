/* INCLUDES */

#include "bridgemain.hpp"

#include <QDebug>

/* DEFINES */

#define DEBUGINFO qDebug() << (QString ("BridgeMain::") + __func__)

enum Tab
{
  Projects,
};

/* VARIABLES */

static QQuickWindow *s_window = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

BridgeMain::BridgeMain (QObject *a_parent)
  : FormBridgeBase (a_parent)
{

}

BridgeMain::~BridgeMain()
{

}

/********************************************
 * METHODS
 *******************************************/

void BridgeMain::connectSignalsAndSlots (QObject *a_qmlForm)
{
  FormBridgeBase::connectSignalsAndSlots (a_qmlForm);

  a_qmlForm->setProperty ("visible", true);
}

QString BridgeMain::formName() const
{
  return "MainQml";
}

QQuickWindow *BridgeMain::window() const
{
  return s_window;
}

void BridgeMain::setWindow (QQuickWindow *a_window)
{
  s_window  = a_window;
}

/********************************************
 * SLOTS
 *******************************************/


/*-----------------------------------------*/
