/* INCLUDES */

#include "formbridgemanager.hpp"

#include "bridgemain.hpp"

#include <QVariant>

/* VARIABLES */

struct Bridges
{
  BridgeMain        mainQml;
};

static QHash<QString, FormBridgeBase*> s_bridgesMap;
static Bridges *s_bridges = nullptr;

/* FUNCTIONS */

static void pushBridge (FormBridgeBase &a_bridge);

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

FormBridgeManager::FormBridgeManager()
  : QObject()
{
  /* create all bridges */
  s_bridges = new Bridges;

  /* build a bridge map */
  pushBridge (s_bridges->mainQml);

  /* connect some bridges to each other */
  //s_bridges->projects.connectMainQml (&s_bridges->mainQml);
}

/********************************************
 * METHODS
 *******************************************/

FormBridgeManager *FormBridgeManager::instance()
{
  static FormBridgeManager i;
  return &i;
}

void FormBridgeManager::connectQmlForm (QObject *a_qmlForm)
{
  auto formName = a_qmlForm->property ("formName").toString();
  auto *bridge  = s_bridgesMap.value (formName, nullptr);

  if (bridge)
    return bridge->connectSignalsAndSlots (a_qmlForm);
}

FormBridgeBase *FormBridgeManager::mainQml()
{
  return &s_bridges->mainQml;
}

/*-----------------------------------------*/

void pushBridge (FormBridgeBase &a_bridge)
{
  s_bridgesMap.insert (a_bridge.formName(), &a_bridge);
}

/*-----------------------------------------*/
