/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qorganizerskeleton_p.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

QT_BEGIN_NAMESPACE_ORGANIZER

QOrganizerManagerEngine* QOrganizerItemSkeletonFactory::engine(const QMap<QString, QString>& parameters, QOrganizerManager::Error* error)
{
    Q_UNUSED(parameters);
    Q_UNUSED(error);

    /* TODO - if you understand any specific parameters. save them in the engine so that engine::managerParameters can return them */

    QOrganizerItemSkeletonEngine* ret = new QOrganizerItemSkeletonEngine(); // manager takes ownership and will clean up.
    return ret;
}

QOrganizerItemEngineId* QOrganizerItemSkeletonFactory::createItemEngineId(const QMap<QString, QString>& parameters, const QString& idString) const
{
    /*
      TODO

      Instantiate your engine-specific item id in this function.

      If idString is not empty, then you should deserialize the idString
      (the opposite of your QOrganizerItemEngineId derived-class'
      toString() function), otherwise you should instantiate an empty
      engine-specific collection id.

      This function allows clients to deserialize serialized ids from
      your engine.
     */
    Q_UNUSED(parameters);
    QOrganizerItemSkeletonEngineId* retn = new QOrganizerItemSkeletonEngineId;
    if (!idString.isEmpty())
        retn->m_itemId = idString.toUInt();
    return retn;
}

QOrganizerCollectionEngineId* QOrganizerItemSkeletonFactory::createCollectionEngineId(const QMap<QString, QString>& parameters, const QString& idString) const
{
    /*
      TODO

      Instantiate your engine-specific collection id in this function.

      If idString is not empty, then you should deserialize the idString
      (the opposite of your QOrganizerCollectionEngineId derived-class'
      toString() function), otherwise you should instantiate an empty
      engine-specific collection id.

      This function allows clients to deserialize serialized ids from
      your engine.
     */
    Q_UNUSED(parameters);
    QOrganizerCollectionSkeletonEngineId* retn = new QOrganizerCollectionSkeletonEngineId;
    if (!idString.isEmpty())
        retn->m_collectionId = idString.toUInt();
    return retn;
}

QString QOrganizerItemSkeletonFactory::managerName() const
{
    /* TODO - put your engine name here */
    return QStringLiteral("skeleton");
}

QOrganizerItemSkeletonEngineId::QOrganizerItemSkeletonEngineId()
    : QOrganizerItemEngineId(), m_itemId(0)
{
    /*
      TODO

      Initialize any data members of your engine-specific item id in the constructor.

      This default constructor should not be used when returning a null id,
      but is provided in order to allow use of the ids in a list, and as an
      enabler for the implementation of QOrganizerItemId.

      When returning a null id, the backend should simply return a default
      constructed QOrganizerItemId.

      In this example, we use just a single quint32 to identify the item,
      however your engine may require more information in order to uniquely
      identify an item within it (e.g., a collection identifier plus an item
      identifier, and perhaps a datastore identifier which identifies the
      datastore in which the collection can be found).
     */
}

QOrganizerItemSkeletonEngineId::QOrganizerItemSkeletonEngineId(quint32 itemId)
    : QOrganizerItemEngineId(), m_itemId(itemId)
{
    /*
      TODO

      Whatever data members your particular class has, should be passed as arguments
      to a ctor of this type.  This is the constructor which will be used by your
      engine code.

      In particular, you will most likely be returning to clients an id by calling:
      QOrganizerItemId id(new QOrganizerItemSkeletonEngineId(3));
      or something similar.  Note that the QOrganizerItemId constructor which
      takes a QOrganizerItemEngineId pointer as a parameter takes ownership
      of that pointer (and so controls its lifetime).
     */
}

QOrganizerItemSkeletonEngineId::QOrganizerItemSkeletonEngineId(const QOrganizerItemSkeletonEngineId& other)
    : QOrganizerItemEngineId(), m_itemId(other.m_itemId)
{
    /* TODO - implement a copy constructor for your engine-specific id class */
}

QOrganizerItemSkeletonEngineId::~QOrganizerItemSkeletonEngineId()
{
    /* TODO - Clean up any memory in use by your engine-specific id. */
}

bool QOrganizerItemSkeletonEngineId::isEqualTo(const QOrganizerItemEngineId* other) const
{
    /*
      TODO

      The isEqualTo(other) function is called by the QOrganizerItemId::operator==(other) function.
      You must implement this in terms of the data members which your class contains.

      An example implementation is provided below, for the case where only a single quint32
      is required to uniquely identify an item in a manager.
     */

    quint32 otherItemId = static_cast<const QOrganizerItemSkeletonEngineId*>(other)->m_itemId;
    if (m_itemId != otherItemId)
        return false;
    return true;
}

bool QOrganizerItemSkeletonEngineId::isLessThan(const QOrganizerItemEngineId* other) const
{
    /*
      TODO

      The isLessThan(other) function is called by the QOrganizerItemId::operator<(other) function.
      You must implement this in terms of the data members which your class contains.

      An example implementation is provided below, for the case where only a single quint32
      is required to uniquely identify an item in a manager.
     */

    quint32 otherItemId = static_cast<const QOrganizerItemSkeletonEngineId*>(other)->m_itemId;
    return (m_itemId < otherItemId);
}

QString QOrganizerItemSkeletonEngineId::managerUri() const
{
    // TODO: make this return the actual managerUri (including params) of the
    // engine it is associated with
    static const QString uri(QStringLiteral("qtorganizer:skeleton:"));
    return uri;
}

QOrganizerItemEngineId* QOrganizerItemSkeletonEngineId::clone() const
{
    /*
      TODO

      When a QOrganizerItemId is copied or assigned, it performs a clone of
      the engine-specific id.  This function is called in that case.

      Implement this function so that the data members of your engine-specific id
      are deep-copied.

      An example implementation for the case where an item can be uniquely identified
      with just a single quint32 is given below.
     */

    QOrganizerItemSkeletonEngineId *myClone = new QOrganizerItemSkeletonEngineId;
    myClone->m_itemId = m_itemId;
    return myClone;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug& QOrganizerItemSkeletonEngineId::debugStreamOut(QDebug& dbg) const
{
    /*
      TODO

      In order to allow clients to debug applications, you must implement this
      function.  We recommend streaming the name of your class followed by the
      values of the data members in your engine-specific id class in
      parentheses.

      An example implementation for the case where an item can be uniquely identified
      with just a single quint32 is given below.

      Note that you must include the #ifndef QT_NO_DEBUG_STREAM preprocessor
      directive block in order to ensure compilation in environments where that
      directive is defined.
     */

    dbg.nospace() << "QOrganizerItemSkeletonEngineId(" << m_itemId << ")";
    return dbg.maybeSpace();
}
#endif

QString QOrganizerItemSkeletonEngineId::toString() const
{
    /*
      TODO

      In order to allow clients to serialize QOrganizerItemId's, you must implement
      this function.

      An example implementation for the case where an item can be uniquely identified
      with just a single quint32 is given below.
     */

    return QString::number(m_itemId);
}

uint QOrganizerItemSkeletonEngineId::hash() const
{
    /*
      TODO

      Provide a hash function for your engine-specific id.
      Note that the hash doesn't strictly need to be unique, since isEqualTo()
      ensures that individual id's in a single hash-bucket can be uniquely
      determined; however a better hash function will result in better performance
      because the ids will be distributed more randomly in a hash table.

      In the example implementation below, we could simply return the id, since the
      id is a quint32.  In more complex id classes, however, you may need to
      qHash() individual data members and combine the results somehow.
     */

    return QT_PREPEND_NAMESPACE(qHash)(m_itemId);
}


QOrganizerCollectionSkeletonEngineId::QOrganizerCollectionSkeletonEngineId()
    : QOrganizerCollectionEngineId(), m_collectionId(0)
{
    /*
      TODO

      Initialize any data members of your engine-specific collection id in the constructor.

      This default constructor should not be used when returning a null id,
      but is provided in order to allow use of the ids in a list, and as an
      enabler for the implementation of QOrganizerCollectionId.

      When returning a null id, the backend should simply return a default
      constructed QOrganizerCollectionId.

      In this example, we use just a single quint32 to identify the collection,
      however your engine may require more information in order to uniquely
      identify a collection within it (e.g., a collection identifier plus a datastore
      identifier which identifies the datastore in which the collection can be found).
     */
}

QOrganizerCollectionSkeletonEngineId::QOrganizerCollectionSkeletonEngineId(quint32 collectionId)
    : QOrganizerCollectionEngineId(), m_collectionId(collectionId)
{
    /*
      TODO

      Whatever data members your particular class has, should be passed as arguments
      to a ctor of this type.  This is the constructor which will be used by your
      engine code.

      In particular, you will most likely be returning to clients an id by calling:
      QOrganizerCollectionId id(new QOrganizerCollectionSkeletonEngineId(3));
      or something similar.  Note that the QOrganizerCollectionId constructor which
      takes a QOrganizerCollectionEngineId pointer as a parameter takes ownership
      of that pointer (and so controls its lifetime).
     */
}

QOrganizerCollectionSkeletonEngineId::QOrganizerCollectionSkeletonEngineId(const QOrganizerCollectionSkeletonEngineId& other)
    : QOrganizerCollectionEngineId(), m_collectionId(other.m_collectionId)
{
    /* TODO - implement a copy constructor for your engine-specific id class */
}

QOrganizerCollectionSkeletonEngineId::~QOrganizerCollectionSkeletonEngineId()
{
    /* TODO - Clean up any memory in use by your engine-specific id. */
}

bool QOrganizerCollectionSkeletonEngineId::isEqualTo(const QOrganizerCollectionEngineId* other) const
{
    /*
      TODO

      The isEqualTo(other) function is called by the QOrganizerCollectionId::operator==(other) function.
      You must implement this in terms of the data members which your class contains.

      An example implementation is provided below, for the case where only a single quint32
      is required to uniquely identify a collection in a manager.
     */

    quint32 otherCollectionId = static_cast<const QOrganizerCollectionSkeletonEngineId*>(other)->m_collectionId;
    if (m_collectionId != otherCollectionId)
        return false;
    return true;
}

bool QOrganizerCollectionSkeletonEngineId::isLessThan(const QOrganizerCollectionEngineId* other) const
{
    /*
      TODO

      The isLessThan(other) function is called by the QOrganizerCollectionId::operator<(other) function.
      You must implement this in terms of the data members which your class contains.

      An example implementation is provided below, for the case where only a single quint32
      is required to uniquely identify a collection in a manager.
     */

    quint32 otherCollectionId = static_cast<const QOrganizerCollectionSkeletonEngineId*>(other)->m_collectionId;
    if (m_collectionId < otherCollectionId)
        return true;
    return false;
}

QString QOrganizerCollectionSkeletonEngineId::managerUri() const
{
    // TODO: make this return the actual managerUri (including params) of the
    // engine it is associated with
    static const QString uri(QStringLiteral("qtorganizer:skeleton:"));
    return uri;
}

QOrganizerCollectionEngineId* QOrganizerCollectionSkeletonEngineId::clone() const
{
    /*
      TODO

      When a QOrganizerCollectionId is copied or assigned, it performs a clone of
      the engine-specific id.  This function is called in that case.

      Implement this function so that the data members of your engine-specific id
      are deep-copied.

      An example implementation for the case where a collection can be uniquely identified
      with just a single quint32 is given below.
     */

    QOrganizerCollectionSkeletonEngineId *myClone = new QOrganizerCollectionSkeletonEngineId;
    myClone->m_collectionId = m_collectionId;
    return myClone;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug& QOrganizerCollectionSkeletonEngineId::debugStreamOut(QDebug& dbg) const
{
    /*
      TODO

      In order to allow clients to debug applications, you must implement this
      function.  We recommend streaming the name of your class followed by the
      values of the data members in your engine-specific id class in
      parentheses.

      An example implementation for the case where a collection can be uniquely identified
      with just a single quint32 is given below.

      Note that you must include the #ifndef QT_NO_DEBUG_STREAM preprocessor
      directive block in order to ensure compilation in environments where that
      directive is defined.
     */

    dbg.nospace() << "QOrganizerCollectionSkeletonEngineId(" << m_collectionId << ")";
    return dbg.maybeSpace();
}
#endif

QString QOrganizerCollectionSkeletonEngineId::toString() const
{
    /*
      TODO

      In order to allow clients to serialize QOrganizerCollectionId's, you must implement
      this function.

      An example implementation for the case where a collection can be uniquely identified
      with just a single quint32 is given below.
     */

    return QString::number(m_collectionId);
}

uint QOrganizerCollectionSkeletonEngineId::hash() const
{
    /*
      TODO

      Provide a hash function for your engine-specific id.
      Note that the hash doesn't strictly need to be unique, since isEqualTo()
      ensures that individual id's in a single hash-bucket can be uniquely
      determined; however a better hash function will result in better performance
      because the ids will be distributed more randomly in a hash table.

      In the example implementation below, we could simply return the id, since the
      id is a quint32.  In more complex id classes, however, you may need to
      qHash() individual data members and combine the results somehow.
     */

    return QT_PREPEND_NAMESPACE(qHash)(m_collectionId);
}



QOrganizerItemSkeletonEngine::~QOrganizerItemSkeletonEngine()
{
    /* TODO clean up your stuff.  Perhaps a QScopedPointer or QSharedDataPointer would be in order */
}

QString QOrganizerItemSkeletonEngine::managerName() const
{
    /* TODO - put your engine name here */
    return QStringLiteral("skeleton");
}

QMap<QString, QString> QOrganizerItemSkeletonEngine::managerParameters() const
{
    /* TODO - in case you have any actual parameters that are relevant that you saved in the factory method, return them here */
    return QMap<QString, QString>();
}

QList<QOrganizerItem> QOrganizerItemSkeletonEngine::itemOccurrences(const QOrganizerItem &parentItem,
                                                                    const QDateTime &startDateTime,
                                                                    const QDateTime &endDateTime, int maxCount,
                                                                    const QOrganizerItemFetchHint &fetchHint,
                                                                    QOrganizerManager::Error *error)
{
    /*
        TODO

        This function should create a list of occurrences that occur in the time period from the supplied item,
        generated from the parent item.

        The periodStart should always be valid, and either the periodEnd or the maxCount will be valid (if periodEnd is
        valid, use that.  Otherwise use the count).  It's permissible to limit the number of items returned...

        Basically, if the parent item is an Event, a list of EventOccurrences should be returned.  Similarly for
        Todo/TodoOccurrence.

        If there are no instances, return an empty list.

        The returned items should have a QOrganizerItemParent detail that points to the parentItem and the
        original instance that the event would have occurred on (e.g. with an exception).

        They should not have recurrence information details in them.

        We might change the signature to split up the periodStart + periodEnd / periodStart + maxCount cases.
    */

    return QOrganizerManagerEngine::itemOccurrences(parentItem, startDateTime, endDateTime, maxCount, fetchHint, error);
}

QList<QOrganizerItem> QOrganizerItemSkeletonEngine::itemsForExport(const QDateTime &startDateTime,
                                                                   const QDateTime &endDateTime,
                                                                   const QOrganizerItemFilter &filter,
                                                                   const QList<QOrganizerItemSortOrder> &sortOrders,
                                                                   const QOrganizerItemFetchHint &fetchHint,
                                                                   QOrganizerManager::Error *error)
{
    return QOrganizerManagerEngine::itemsForExport(startDateTime, endDateTime, filter, sortOrders, fetchHint, error);
}

QList<QOrganizerItemId> QOrganizerItemSkeletonEngine::itemIds(const QOrganizerItemFilter &filter,
                                                              const QDateTime &startDateTime,
                                                              const QDateTime &endDateTime,
                                                              const QList<QOrganizerItemSortOrder> &sortOrders,
                                                              QOrganizerManager::Error *error)
{
    /*
        TODO

        Given the supplied filter and sort order, fetch the list of items [not instances] that correspond, and return their ids.

        If you don't support the filter or sort orders, you can fetch a partially (or un-) filtered list and ask the helper
        functions to filter and sort it for you.

        If you do have to fetch, consider setting a fetch hint that restricts the information to that needed for filtering/sorting.
    */

    *error = QOrganizerManager::NotSupportedError; // TODO <- remove this

    QList<QOrganizerItem> partiallyFilteredItems; // = ..., your code here.. [TODO]
    QList<QOrganizerItem> ret;

    foreach(const QOrganizerItem& item, partiallyFilteredItems) {
        if (QOrganizerManagerEngine::isItemBetweenDates(item, startDateTime, endDateTime) && QOrganizerManagerEngine::testFilter(filter, item)) {
            QOrganizerManagerEngine::addSorted(&ret, item, sortOrders);
        }
    }

    return QOrganizerManager::extractIds(ret);
}

QList<QOrganizerItem> QOrganizerItemSkeletonEngine::items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                                          const QDateTime &endDateTime, int maxCount,
                                                          const QList<QOrganizerItemSortOrder> &sortOrders,
                                                          const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
{
    /*
        TODO

        Given the supplied filter and sort order, fetch the list of items [not instances] that correspond, and return them.

        If you don't support the filter or sort orders, you can fetch a partially (or un-) filtered list and ask the helper
        functions to filter and sort it for you.

        The fetch hint suggests how much of the item to fetch.  You can ignore the fetch hint and fetch everything (but you must
        fetch at least what is mentioned in the fetch hint).
    */

    Q_UNUSED(startDateTime)
    Q_UNUSED(endDateTime)
    Q_UNUSED(maxCount)
    Q_UNUSED(fetchHint)

    *error = QOrganizerManager::NotSupportedError; // TODO <- remove this

    QList<QOrganizerItem> partiallyFilteredItems; // = ..., your code here.. [TODO]
    QList<QOrganizerItem> ret;

    foreach(const QOrganizerItem& item, partiallyFilteredItems) {
        if (QOrganizerManagerEngine::isItemBetweenDates(item, startDateTime, endDateTime) && QOrganizerManagerEngine::testFilter(filter, item)) {
            QOrganizerManagerEngine::addSorted(&ret, item, sortOrders);
        }
    }

    /* An alternative formulation, depending on how your engine is implemented is just:

        foreach(const QOrganizerItemId& id, itemIds(filter, sortOrders, error)) {
            ret.append(item(id, fetchHint, error);
        }
     */

    return ret;
}

QList<QOrganizerItem> QOrganizerItemSkeletonEngine::items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                                          QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error* error)
{
    /*
        TODO

        Fetch a single item by id.

        The fetch hint suggests how much of the item to fetch.  You can ignore the fetch hint and fetch everything (but you must
        fetch at least what is mentioned in the fetch hint).

    */
    return QOrganizerManagerEngine::items(itemIds, fetchHint, errorMap, error);
}

bool QOrganizerItemSkeletonEngine::saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                                             QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    /*
        TODO

        Save a list of items into the collection specified in each (or their current collection
        if no collection is specified and they already exist, or the default collection
        if no collection is specified and they do not exist).

        For each item, convert it to your type, assign an item id, and update the
        QOrganizerItem's ID (in the list above - e.g. *items[idx] = updated item).
        Then, examine the collection id specified in each item and save the item in that collection.

        If you encounter an error (e.g. converting to type, or saving), insert an entry into
        the map above at the corresponding index (e.g. errorMap->insert(idx, QOIM::InvalidDetailError).
        You should set the "error" variable as well (first, last, most serious error etc).

        The item passed in should be validated according to the schema.
    */
    return QOrganizerManagerEngine::saveItems(items, detailMask, errorMap, error);
}

bool QOrganizerItemSkeletonEngine::removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                                               QOrganizerManager::Error *error)
{
    /*
        TODO

        Remove a list of items, given by their id.

        If you encounter an error, insert an error into the appropriate place in the error map,
        and update the error variable as well.

        DoesNotExistError should be used if the id refers to a non existent item.
    */
    return QOrganizerManagerEngine::removeItems(itemIds, errorMap, error);
}

QOrganizerCollection QOrganizerItemSkeletonEngine::defaultCollection(QOrganizerManager::Error* error)
{
    /*
        TODO

        This allows clients to determine which collection an item will be saved,
        if the item is saved via saveItems() without specifying a collection id
        of a collection in which to save the item, via item->setCollectionId().

        There is always at least one collection in a manager, and all items are
        saved in exactly one collection.
     */
    return QOrganizerManagerEngine::defaultCollection(error);
}

QOrganizerCollection QOrganizerItemSkeletonEngine::collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    /*
        TODO

        This allows clients to retrieve a collection by (manager) id.
        Prior to saving items, clients will set which collection the item will/should
        be saved by calling item->setCollectionId().
     */
    return QOrganizerManagerEngine::collection(collectionId, error);
}

QList<QOrganizerCollection> QOrganizerItemSkeletonEngine::collections(QOrganizerManager::Error* error)
{
    /*
        TODO

        This allows clients to retrieve a list of all of the collections currently
        in this manager.  Some backends will have a prepopulated list of valid
        collections, others will not.  A collection can have properties
        like colour, description, perhaps a priority, etc etc.
     */
    return QOrganizerManagerEngine::collections(error);
}

bool QOrganizerItemSkeletonEngine::saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error)
{
    /*
        TODO

        This allows clients to create or update collections if supported by the
        backend.
     */
    return QOrganizerManagerEngine::saveCollection(collection, error);
}

bool QOrganizerItemSkeletonEngine::removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    /*
        TODO

        This allows clients to remove collections if supported by the backend.

        When a collection is removed, all items in the collection are removed.
        That is, they are _not_ transferred to another collection.

        If the user attempts to remove the collection which is the default collection,
        the backend may decide whether to fail (with a permissions error) or to
        succeed and arbitrarily choose another collection to be the default collection.
     */
    return QOrganizerManagerEngine::removeCollection(collectionId, error);
}

bool QOrganizerItemSkeletonEngine::startRequest(QOrganizerAbstractRequest* req)
{
    /*
        TODO

        This is the entry point to the async API.  The request object describes the
        type of request (switch on req->type()).  Req will not be null when called
        by the framework.

        Generally, you can queue the request and process them at some later time
        (probably in another thread).

        Once you start a request, call the updateRequestState and/or the
        specific updateXXXXXRequest functions to mark it in the active state.

        If your engine is particularly fast, or the operation involves only in
        memory data, you can process and complete the request here.  That is
        probably not the case, though.

        Note that when the client is threaded, and the request might live on a
        different thread, you might need to be careful with locking.  In particular,
        the request might be deleted while you are still working on it.  In this case,
        your requestDestroyed function will be called while the request is still valid,
        and you should block in that function until your worker thread (etc) has been
        notified not to touch that request any more.

        We plan to provide some boiler plate code that will allow you to:

        1) implement the sync functions, and have the async versions call the sync
           in another thread

        2) or implement the async versions of the function, and have the sync versions
           call the async versions.

        It's not ready yet, though.

        Return true if the request can be started, false otherwise.  You can set an error
        in the request if you like.
    */
    return QOrganizerManagerEngine::startRequest(req);
}

bool QOrganizerItemSkeletonEngine::cancelRequest(QOrganizerAbstractRequest* req)
{
    /*
        TODO

        Cancel an in progress async request.  If not possible, return false from here.
    */
    return QOrganizerManagerEngine::cancelRequest(req);
}

bool QOrganizerItemSkeletonEngine::waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs)
{
    /*
        TODO

        Wait for a request to complete (up to a max of msecs milliseconds).

        Return true if the request is finished (including if it was already).  False otherwise.

        You should really implement this function, if nothing else than as a delay, since clients
        may call this in a loop.

        It's best to avoid processing events, if you can, or at least only process non-UI events.
    */
    return QOrganizerManagerEngine::waitForRequestFinished(req, msecs);
}

void QOrganizerItemSkeletonEngine::requestDestroyed(QOrganizerAbstractRequest* req)
{
    /*
        TODO

        This is called when a request is being deleted.  It lets you know:

        1) the client doesn't care about the request any more.  You can still complete it if
           you feel like it.
        2) you can't reliably access any properties of the request pointer any more.  The pointer will
           be invalid once this function returns.

        This means that if you have a worker thread, you need to let that thread know that the
        request object is not valid and block until that thread acknowledges it.  One way to do this
        is to have a QSet<QOIAR*> (or QMap<QOIAR, MyCustomRequestState>) that tracks active requests, and
        insert into that set in startRequest, and remove in requestDestroyed (or when it finishes or is
        cancelled).  Protect that set/map with a mutex, and make sure you take the mutex in the worker
        thread before calling any of the QOIAR::updateXXXXXXRequest functions.  And be careful of lock
        ordering problems :D

    */
    return QOrganizerManagerEngine::requestDestroyed(req);
}

QList<QOrganizerItemFilter::FilterType> QOrganizerItemSkeletonEngine::supportedFilters() const
{
    // TODO if you engine can natively support the filter, return true.  Otherwise you should emulate support in the item{Ids} functions.
    return QList<QOrganizerItemFilter::FilterType>();
}

QList<QOrganizerItemDetail::DetailType> QOrganizerItemSkeletonEngine::supportedItemDetails(QOrganizerItemType::ItemType itemType) const
{
    // TODO - return which [predefined] details this engine supports for this item type
    Q_UNUSED(itemType)

    return QList<QOrganizerItemDetail::DetailType>();
}

QList<QOrganizerItemType::ItemType> QOrganizerItemSkeletonEngine::supportedItemTypes() const
{
    // TODO - return which [predefined] types this engine supports
    QList<QOrganizerItemType::ItemType> ret;

    ret << QOrganizerItemType::TypeEvent;
    ret << QOrganizerItemType::TypeEventOccurrence;
    ret << QOrganizerItemType::TypeJournal;
    ret << QOrganizerItemType::TypeNote;
    ret << QOrganizerItemType::TypeTodo;
    ret << QOrganizerItemType::TypeTodoOccurrence;

    return ret;
}

#include "moc_qorganizerskeleton_p.cpp"

QT_END_NAMESPACE_ORGANIZER
