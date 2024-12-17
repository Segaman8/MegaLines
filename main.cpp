/* INCLUDES */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include "bridge/formbridgemanager.hpp"
#include "ctl/playgroundfieldcontroler.hpp"
#include "model/playgroundfieldmodel.hpp"

/********************************************
 * MAIN
 *******************************************/

int main (int argc, char *argv[])
{
  /* start app */
  QGuiApplication app (argc, argv);

  /* init systems */

  /* start qml engine */
  QQmlApplicationEngine engine;

  /* register types */
  qmlRegisterType<PlaygroundFieldModel> ("com.PlaygroundField", 1, 0, "PlaygroundFieldModel");

  /* setup qml context */
  QQmlContext *ctx  = engine.rootContext();
  ctx->setContextProperty ("fieldControler", PlaygroundFieldControler::instance());
  ctx->setContextProperty ("bridgeManager", FormBridgeManager::instance());

  PlaygroundFieldControler::instance()->createRandom();

  /* start qml form */
  const QUrl url (QStringLiteral ("qrc:/qml/main.qml"));
  QObject::connect (&engine, &QQmlApplicationEngine::objectCreated,
                    &engine, [url, &engine] (QObject * obj, const QUrl & objUrl)
  {
    if (!obj && url == objUrl)
      QCoreApplication::exit (-1);

    QQuickWindow *qmlWindow = qobject_cast<QQuickWindow *> (engine.rootObjects().first());
    if (qmlWindow)
      {

      }
  });
  engine.load (url);

  /* execute */
  return app.exec();
}

/*-----------------------------------------*/
