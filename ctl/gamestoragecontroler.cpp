/* INCLUDES */

#include "gamestoragecontroler.hpp"
#include "ctl/playgroundfieldcontroler.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/* DEFINES */

#define DATABASE_NAME ("save.sqlite")

#define DEBUGINFO qDebug() << (QString ("GameStorageControler::") + __func__)

struct DatabaseControler
{
  /* VARIABLES */
  QSqlDatabase db;

  /* CONSTRUCT/DESTRUCT */
  DatabaseControler()
    : db (QSqlDatabase::addDatabase ("QSQLITE"))
  {
    db.setDatabaseName (DATABASE_NAME);

    if (!db.open())
      {
        DEBUGINFO << "Database connection failed:" << db.lastError().text();
        return;
      }
  }
  ~DatabaseControler()
  {
    db.close();
  }

  /* METHODS */
  void createTables();
  void storeGameData();
  void restoreGameData();
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

GameStorageControler::GameStorageControler()
{

}

/********************************************
 * METHODS
 *******************************************/

GameStorageControler *GameStorageControler::instance()
{
  static GameStorageControler i;
  return &i;
}

void GameStorageControler::save()
{
  DatabaseControler ctl;
  ctl.createTables();
  ctl.storeGameData();
}

void GameStorageControler::load()
{
  DatabaseControler ctl;
  ctl.restoreGameData();
}

/*-----------------------------------------*/

void DatabaseControler::createTables()
{
  QSqlQuery query;

  /* drop */

  query.exec ("DROP TABLE IF EXISTS entities");
  query.exec ("DROP TABLE IF EXISTS config");

  /* create */

  query.exec(
      "CREATE TABLE entities ("
      "id INT PRIMARY KEY, "
      "x INT NOT NULL, "
      "y INT NOT NULL, "
      "type int NOT NULL)"
    );
  query.exec(
      "CREATE TABLE config ("
      "width INT NOT NULL, "
      "height INT NOT NULL, "
      "score int NOT NULL)"
    );
}

void DatabaseControler::storeGameData()
{
  /* variables */
  auto ctl    = PlaygroundFieldControler::instance();
  auto width  = ctl->width();
  auto height = ctl->height();
  auto amount = ctl->amount();
  QSqlQuery query;

  /*-----------------------------------------*/
  /* STORE CONFIG */
  /*-----------------------------------------*/

  /* setup request */
  query.prepare ("INSERT INTO config (width, height, score) VALUES (:width, :height, :score)");
  query.bindValue (":width", width);
  query.bindValue (":height", height);
  query.bindValue (":score", ctl->score());

  /* execute */
  if (!query.exec())
    {
      DEBUGINFO << "Failed to insert data:" << query.lastError().text();
      DEBUGINFO << query.lastQuery();
      return;
    }

  /*-----------------------------------------*/
  /* STORE FIELD */
  /*-----------------------------------------*/

  /* start transaction */
  db.transaction();

  /* setup request */
  query.prepare ("INSERT INTO entities (id, x, y, type) VALUES (:id, :x, :y, :type)");

  /* cycle thru every entity on the field */
  for (int i = 0; i < amount; i++)
    {
      /* get entity */
      auto &entity  = ctl->atIndex (i);

      /* bind values */
      query.bindValue (":id", i);
      query.bindValue (":x", entity.x());
      query.bindValue (":y", entity.y());
      query.bindValue (":type", int (entity.type()));

      /* execute request */
      if (!query.exec())
        {
          DEBUGINFO << "Failed to insert data:" << query.lastError().text();
          DEBUGINFO << query.lastQuery();
          return;
        }
    }

  /* finish transaction */
  db.commit();

  /*-----------------------------------------*/

  DEBUGINFO << "Success!";
}

void DatabaseControler::restoreGameData()
{
  /* variables */
  auto ctl      = PlaygroundFieldControler::instance();
  QSqlQuery query;
  bool success  = false;

  /*-----------------------------------------*/
  /* RESTORE CONFIG */
  /*-----------------------------------------*/

  /* select items */
  if (!query.exec ("SELECT width, height, score FROM config LIMIT 1"))
    {
      DEBUGINFO << "Query execution failed:" << query.lastError().text();
      return;
    }

  /* read data */
  while (query.next())
    {
      quint16 width   = query.value ("width").toUInt();
      quint16 height  = query.value ("height").toUInt();
      quint32 score   = query.value ("score").toUInt();

      DEBUGINFO << QString ("config: %1x%2 score: %3")
                   .arg (width)
                   .arg (height)
                   .arg (score);

      success = true;
    }

  /* check if no data */
  if (success == false)
  {
    DEBUGINFO << "Unable to get save state config";
    return;
  }

  /*-----------------------------------------*/
  /* RESTORE ENTITIES */
  /*-----------------------------------------*/

  /* select items */
  if (!query.exec ("SELECT x, y, type FROM entities"))
    {
      DEBUGINFO << "Query execution failed:" << query.lastError().text();
      return;
    }

  /* read data */
  while (query.next())
    {
      //quint16 index     = query.value ("id").toUInt();
      quint16 x         = query.value ("x").toUInt();
      quint16 y         = query.value ("y").toUInt();
      Entity::Type type = Entity::Type (query.value ("type").toUInt());

      ctl->createAt (x, y, type);
    }

  DEBUGINFO<< "Success!";
}

/*-----------------------------------------*/
