/* INCLUDES */

#include "formbridgebase.hpp"

#include <QMetaObject>
#include <QMetaMethod>
#include <QHash>
#include <QDebug>

/* DEFINES */

#define DEBUGINFO qDebug() << (QString ("FormBridgeBase::") + __func__)

#define ENABLE_CONNECTIONS_DEBUG

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

FormBridgeBase::FormBridgeBase (QObject *a_parent)
  : QObject{a_parent}
{

}

FormBridgeBase::~FormBridgeBase()
{

}

/********************************************
 * METHODS
 *******************************************/

void FormBridgeBase::connectSignalsAndSlots (QObject *a_object)
{
  _connectDestSignalsToBridgeSlots (a_object);
  _connectBridgeSignalsToDestSlots (a_object);
}

QString FormBridgeBase::formName() const
{
  return QString();
}

void FormBridgeBase::_connectDestSignalsToBridgeSlots (QObject *a_object)
{
  /* get meta object data for the QML form and bridge class */

  const QMetaObject *qmlMetaObject  = a_object->metaObject();
  const QMetaObject *thisMetaObject = metaObject();

  /* create a hash table to store QML methods for quick search */

  QHash<QString, QMetaMethod> qmlMethods;
  for (int j = 0; j < qmlMetaObject->methodCount(); ++j)
    {
      QMetaMethod qmlMethod = qmlMetaObject->method (j);
      if (qmlMethod.methodType() != QMetaMethod::Signal)
        continue;

      QString qmlMethodName = qmlMethod.methodSignature();
      if (qmlMethodName.startsWith ("sig"))
        qmlMethods.insert (qmlMethod.methodSignature(), qmlMethod);
    }

  /* iterate through the bridge class methods */

  for (int i = 0; i < thisMetaObject->methodCount(); ++i)
    {
      QMetaMethod thisMethod = thisMetaObject->method (i);

      /* check if method is a slot */

      if (thisMethod.methodType() != QMetaMethod::Slot)
        continue;

      /* check slot name */

      QString thisMethodName = thisMethod.methodSignature();
      if (thisMethodName.startsWith ("slot"))
        {
          /* find a matching signal in the QML form based on the slot name */

          QString matchingSignalName = "sig" + thisMethodName.mid (4);
          QMetaMethod matchingSignal = qmlMethods.value (matchingSignalName);

          /* if a matching signal is found, connect it to the slot */

          if (matchingSignal.methodType() == QMetaMethod::Signal)
            {
#ifdef ENABLE_CONNECTIONS_DEBUG
              DEBUGINFO << QString ("%2 -> %1").arg (thisMethodName, matchingSignalName);
#endif // ENABLE_CONNECTIONS_DEBUG

              connect (a_object, matchingSignal,
                       this, thisMethod);
            }
        }
    }
}

void FormBridgeBase::_connectBridgeSignalsToDestSlots (QObject *a_object)
{
  /* get meta object data for the QML form and bridge class */

  const QMetaObject *qmlMetaObject  = a_object->metaObject();
  const QMetaObject *thisMetaObject = metaObject();

  /* populate the local signal map for quick lookup */

  QHash<QString, QMetaMethod> signalMap;
  for (int i = 0; i < thisMetaObject->methodCount(); ++i)
    {
      QMetaMethod bridgeSignal = thisMetaObject->method (i);

      if (bridgeSignal.methodType() == QMetaMethod::Signal)
        signalMap.insert (bridgeSignal.methodSignature(), bridgeSignal);
    }

  /* iterate through the signals of this bridge */

  for (auto i = signalMap.cbegin(); i != signalMap.cend(); i++)
    {
      /* get data */

      const QString &signalName = i.key();
      const QMetaMethod &signal = i.value();

      /* check name */

      if (signalName.startsWith ("sig"))
        {
          QString slotName = "slot" + signalName.mid (3);

          /* find the corresponding method in the QML form */

          int slotIndex = qmlMetaObject->indexOfMethod (slotName.toUtf8().constData());
          if (slotIndex != -1)
            {
              QMetaMethod qmlMethod = qmlMetaObject->method (slotIndex);

              /* if a matching signal is found, connect it to the slot */

              if (qmlMethod.methodType() == QMetaMethod::Slot)
                {
#ifdef ENABLE_CONNECTIONS_DEBUG
                  DEBUGINFO << QString ("%1 -> %2").arg (signalName, slotName);
#endif // ENABLE_CONNECTIONS_DEBUG

                  /* connect the signal to the found QML slot */

                  connect (this, signal,
                           a_object, qmlMethod);
                }
            }
        }
    }
}

/*-----------------------------------------*/
