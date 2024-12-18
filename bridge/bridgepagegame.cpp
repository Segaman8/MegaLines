/* INCLUDES */

#include "bridgepagegame.hpp"
#include "bridgemain.hpp"
#include "formbridgemanager.hpp"
#include "ctl/playgroundfieldcontroler.hpp"
#include "ctl/gamestoragecontroler.hpp"

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
  , m_form (nullptr)
{
  /* variables */
  auto ctl  = PlaygroundFieldControler::instance();

  /* connect signals */
  FormBridgeBase::connectSignalsAndSlots (ctl);
  // connect (ctl, &PlaygroundFieldControler::sigScoreChanged,
  //          this, &BridgePageGame::slotScoreChanged);

  // connect (ctl, &PlaygroundFieldControler::sigNewGame,
  //          this, &BridgePageGame::slotNewGame);
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
  m_form  = a_qmlForm;
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
    if (!ctl->createRandom())
      return;
}

/********************************************
 * SLOTS
 *******************************************/

void BridgePageGame::slotScoreChanged (quint32 a_value)
{
  emit sigScoreUpdate (a_value);
}

void BridgePageGame::slotNewGame()
{
  s_selected  = false;

  m_form->setProperty ("gameOver", false);

  emit sigScoreUpdate (0);
}

void BridgePageGame::slotGameOver()
{
  DEBUGINFO;

  m_form->setProperty ("gameOver", true);
}

void BridgePageGame::slotMoved (Position a_from, Position a_to)
{
  DEBUGINFO;

  auto ctl  = PlaygroundFieldControler::instance();
  auto type = ctl->at (a_to.x, a_to.y).type();

  if (type != Entity::Type::Free)
    emit sigMoved(
        a_from.x,
        a_from.y,
        a_to.x,
        a_to.y,
        int (type)
      );
}

void BridgePageGame::slotGotLine (const QList<Position> &a_positions)
{
  DEBUGINFO;

  /* collect positions into variant list */
  QVariantList list;
  for (auto pos : a_positions)
    list << QPoint { pos.x, pos.y };

  /* send to qml */
  emit sigDestroyed (list);
}

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
    /* moved */
    if (ctl->moveTo (pos.x, pos.y))
    {
      DEBUGINFO << "moving" << a_index;
      s_selected = false;
      _spawnNewEntities();
    }

    /* deselected */
    else
    {
      auto selected = ctl->selected();
      if (selected.x == ctl->width()
          || selected.y == ctl->height())
        s_selected = false;
      else
        DEBUGINFO << "failed moving" << a_index;
    }
  }
}

void BridgePageGame::slotMenuClicked()
{
  DEBUGINFO;

  GameStorageControler::instance()->save();
  FormBridgeManager::mainQml()->as<BridgeMain>()->switchToTab (0);

}

/*-----------------------------------------*/
