/* INCLUDES */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>


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

  /* setup qml context */
  //QQmlContext *ctx  = engine.rootContext();
  //ctx->setContextProperty ("bridgeManager", FormBridgeManager::instance());

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
