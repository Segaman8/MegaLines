/* INCLUDES */

#include "bridgepagegame.hpp"
#include "ctl/playgroundfieldcontroler.hpp"

#include <QDebug>

/* DEFINES */

#define DEBUGINFO qDebug() << (QString ("BridgePageGame::") + __func__)

/* VARIABLES */

static bool s_selected  = false;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

BridgePageGame::BridgePageGame (QObject *a_parent)
  : FormBridgeBase (a_parent)
{
  /* variables */
  auto ctl  = PlaygroundFieldControler::instance();

  /* connect signals */
  connect (ctl, &PlaygroundFieldControler::sigScoreChanged,
           this, [this] (quint32 a_value)
  {
    emit sigScoreUpdate (a_value);
  });

  connect (ctl, &PlaygroundFieldControler::sigNewGame,
           this, [this] ()
  {
    s_selected  = false;
    emit sigScoreUpdate (0);
  });
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

void BridgePageGame::_spawnNewEntities()
{
  DEBUGINFO;

  auto ctl  = PlaygroundFieldControler::instance();

  for (int i = 0; i < 3; i++)
    ctl->createRandom();
}

/********************************************
 * SLOTS
 *******************************************/

void BridgePageGame::slotEntityClicked (int a_index)
{
  auto ctl  = PlaygroundFieldControler::instance();
  auto pos  = ctl->indexToCoord (a_index);

  if (!s_selected)
  {
    s_selected  = ctl->select (pos.x, pos.y);
    DEBUGINFO << "selecting" << a_index << s_selected;
  }
  else
  {
    if (ctl->moveTo (pos.x, pos.y))
    {
      DEBUGINFO << "moving" << a_index;
      s_selected = false;
      _spawnNewEntities();
    }
    else
      DEBUGINFO << "failed moving" << a_index;
  }
}

/*-----------------------------------------*/