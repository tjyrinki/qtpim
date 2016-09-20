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

#include "qorganizerjsondbdatastorage.h"

#include <QtCore/qdebug.h>
#include <QtCore/qmutex.h>

#include <QtOrganizer/qorganizeritemcollectionfilter.h>
#include <QtOrganizer/qorganizermanagerengine.h>

#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring.h"

QT_BEGIN_NAMESPACE_ORGANIZER

QOrganizerJsonDbDataStorage::QOrganizerJsonDbDataStorage()
    : m_waitMutex(0)
    , m_jsonDbConnection(0)
    , m_mandatoryStorageLocationMissing(false)
{
    m_availableStorageLocations<<QOrganizerJsonDbEngine::UserDataStorage<<QOrganizerJsonDbEngine::SystemStorage;
    m_availableStorageLocationsFlag = m_converter.storageLocationListToFlag(m_availableStorageLocations);
    clearRequestData();
}

QOrganizerJsonDbDataStorage::~QOrganizerJsonDbDataStorage()
{
    delete m_waitMutex;
}

void QOrganizerJsonDbDataStorage::saveItems(QMap<int, QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error, QOrganizerJsonDbEngine::StorageLocation storageLocation)
{
    if (items->isEmpty())
        return;
    initRequestData(SaveItems, errorMap, error);
    m_saveToStorageLocation = storageLocation;
    m_resultItems = items;
    processRequest();
    clearRequestData();
}

QList<QOrganizerItem> QOrganizerJsonDbDataStorage::items(const QDateTime &startDate, const QDateTime &endDate, const QOrganizerItemFilter &filter,
                                                         const QList<QOrganizerItemSortOrder> &sortOrders, const QOrganizerItemFetchHint &fetchHint,
                                                         QOrganizerManager::Error *error, QOrganizerJsonDbEngine::StorageLocations storageLocations,
                                                         FetchType type, const QOrganizerItemId &parentId)
{
    Q_UNUSED(sortOrders);
    Q_UNUSED(fetchHint);
    initRequestData(Items, 0, error);
    m_start = startDate;
    m_end = endDate;
    m_filter = filter;
    if (type == FetchItemOccurrences)
        m_parentItemId = parentId;
    m_fetchType = type;
    m_fetchFromStorageLocations = storageLocations;
    processRequest();
    QList<QOrganizerItem> fetchedItems = m_items;
    clearRequestData();
    return fetchedItems;
}

QList<QOrganizerItem> QOrganizerJsonDbDataStorage::itemsById(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap,
                                                             QOrganizerManager::Error *error, QOrganizerJsonDbEngine::StorageLocations storageLocations)
{
    if (itemIds.isEmpty())
        return QList<QOrganizerItem>();
    initRequestData(ItemsById, errorMap, error);
    m_itemIds = itemIds;
    m_fetchFromStorageLocations = storageLocations;
    processRequest();
    QList<QOrganizerItem> fetchedItems = m_items;
    clearRequestData();
    return fetchedItems;
}

void QOrganizerJsonDbDataStorage::removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    if (itemIds.isEmpty())
        return;
    initRequestData(RemoveItems, errorMap, error);
    m_itemIds = itemIds;
    processRequest();
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::saveCollections(QMap<int, QOrganizerCollection>* collections, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error, QOrganizerJsonDbEngine::StorageLocation storageLocation)
{
    if (collections->isEmpty())
        return;
    initRequestData(SaveCollections, errorMap, error);
    m_resultCollections = collections;
    m_isDefaultCollection = false;
    m_saveToStorageLocation = storageLocation;
    processRequest();
    clearRequestData();
}

QList<QOrganizerCollection> QOrganizerJsonDbDataStorage::collections(QOrganizerManager::Error *error, QOrganizerJsonDbEngine::StorageLocations storageLocations)
{
    initRequestData(Collections, 0, error);
    m_fetchFromStorageLocations = storageLocations;
    processRequest();
    QList<QOrganizerCollection> fetchedCollections;
    if (*error == QOrganizerManager::NoError)
        fetchedCollections = m_collections;
    clearRequestData();
    return fetchedCollections;
}

void QOrganizerJsonDbDataStorage::removeCollections(const QMap<int, QOrganizerCollectionId>& collectionIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    if (collectionIds.isEmpty())
        return;
    initRequestData(RemoveCollections, errorMap, error);
    m_removeCollectionIds = collectionIds;
    processRequest();
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::createDefaultCollection(QOrganizerCollection* defaultCollection, QOrganizerManager::Error* error)
{
    QMap<int, QOrganizerManager::Error> errorMap;
    initRequestData(SaveCollections, &errorMap, error);
    QMap<int, QOrganizerCollection> collections;
    collections.insert(0, *defaultCollection);
    m_resultCollections = &collections;
    m_isDefaultCollection = true;
    m_saveToStorageLocation = QOrganizerJsonDbEngine::UserDataStorage;
    processRequest();
    *defaultCollection = m_resultCollections->value(0);
    m_defaultCollection = *defaultCollection;
    clearRequestData();
}

QOrganizerCollection QOrganizerJsonDbDataStorage::defaultCollection()
{
    // Fetching collections is the first operation done with the backend
    // and the availability of storage locations is also checked.
    if (m_defaultCollection.id().isNull()) {
        // m_defaultCollection is updated every time collections are fetched
        QOrganizerManager::Error error;
        collections(&error, m_converter.storageLocationListToFlag(m_availableStorageLocations));
    }
    return m_defaultCollection;
}

QSet<QOrganizerCollectionId> QOrganizerJsonDbDataStorage::collectionIds()
{
    if (m_collectionIds.isEmpty()) {
        // m_collectionIds is updated every time collections are fetched
        QOrganizerManager::Error error;
        collections(&error, m_converter.storageLocationListToFlag(m_availableStorageLocations));
    }
    return m_collectionIds;
}

QOrganizerJsonDbEngine::StorageLocations QOrganizerJsonDbDataStorage::availableStorageLocationsFlag() const
{
    return m_availableStorageLocationsFlag;
}

void QOrganizerJsonDbDataStorage::run()
{
    m_waitMutex = new QMutex();

    m_jsonDbConnection = new QJsonDbConnection(this);
    connect(m_jsonDbConnection, SIGNAL(error(QtJsonDb::QJsonDbConnection::ErrorCode,QString)),
            this, SLOT(onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode,QString)));
    m_jsonDbConnection->connectToServer();

    registerForNotifications();

    connect(this, SIGNAL(requestInitialized()), this, SLOT(handleRequest()));

    emit threadInitialized();
    QThread::run();
}

////////////////////////////////////////////////////////////////////////////////
// These functions are run in the context of QOrganizerJsonDbDataStorage thread
////////////////////////////////////////////////////////////////////////////////

void QOrganizerJsonDbDataStorage::registerForNotifications()
{
    const QStringList storageLocations = m_converter.storageLocationsFlagToStrings(
                QOrganizerJsonDbEngine::UserDataStorage | QOrganizerJsonDbEngine::SystemStorage);

    const int count = storageLocations.count();
    QJsonDbWatcher *watcher;
    for (int i=0;i<count;i++) {
        watcher = new QJsonDbWatcher(this);
        watcher->setWatchedActions(QJsonDbWatcher::All);
        watcher->setQuery(QOrganizerJsonDbStr::jsonDbNotificationQuery());
        watcher->setPartition(storageLocations.at(i));
        connect(watcher, SIGNAL(error(QtJsonDb::QJsonDbWatcher::ErrorCode,QString)),
                this, SLOT(onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode,QString)));
        connect(watcher, SIGNAL(notificationsAvailable(int)),
                this, SLOT(onJsonDbWatcherNotificationsAvailable()));
        m_jsonDbConnection->addWatcher(watcher);
        m_watchers.append(watcher);
    }
}

void QOrganizerJsonDbDataStorage::onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode error, const QString &message)
{
    Q_UNUSED(error)
    Q_UNUSED(message)

    // currently there's no real errors from JsonDb
    Q_ASSERT(m_converter.jsonDbConnectionErrorToOrganizerError(error) == QOrganizerManager::NoError);
}

void QOrganizerJsonDbDataStorage::onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode error, const QString &message)
{
    Q_UNUSED(message)

    // if the error() signal is emitted, the finished() signal won't be emitted, so need to call handleResponse()
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (request) {
        QOrganizerManager::Error organizerError = m_converter.jsonDbRequestErrorToOrganizerError(error);
        // check first the storage location
        if (QOrganizerManager::UnspecifiedError == organizerError && !m_mandatoryStorageLocationMissing) {
            const QOrganizerJsonDbEngine::StorageLocation requestStorageLocation = m_converter.storageLocationStringToEnum(request->partition());
            m_availableStorageLocations.removeOne(requestStorageLocation);
            m_availableStorageLocationsFlag = m_converter.storageLocationListToFlag(m_availableStorageLocations);
            if (QOrganizerJsonDbEngine::UserDataStorage == requestStorageLocation) {
                qCritical("Organizer - JsonDb backend does not work without UserDataStorage!");
                m_mandatoryStorageLocationMissing = true;
                organizerError = QOrganizerManager::UnspecifiedError;
            }
        }
        handleResponse(organizerError, request);
    }
}

void QOrganizerJsonDbDataStorage::onJsonDbRequestFinished()
{
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (request)
        handleResponse(QOrganizerManager::NoError, request);
}

void QOrganizerJsonDbDataStorage::onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode error, const QString &message)
{
    if (QJsonDbWatcher::NoError != error) {
        QJsonDbWatcher *watcher = qobject_cast<QJsonDbWatcher *>(sender());
        if (watcher) {
            qCritical("Organizer - Not able to register for jsondb-notifications on '%s'!", qPrintable(watcher->partition()));
            qCritical("Organizer - Error: '%s'.", qPrintable(message));
            m_watchers.removeOne(watcher);
            watcher->deleteLater();
        }
    }
}

void QOrganizerJsonDbDataStorage::onJsonDbWatcherNotificationsAvailable()
{
    // no mutex is needed since this slot doesn't touch any member variables
    QJsonDbWatcher *watcher = qobject_cast<QJsonDbWatcher *>(sender());
    if (watcher) {
        const QList<QJsonDbNotification> &notifications = watcher->takeNotifications();
        for (int i = 0; i < notifications.size(); ++i) {
            QJsonObject jsonDbObject = notifications.at(i).object();
            QString jsonDbType = m_converter.jsonDbNotificationObjectToOrganizerType(jsonDbObject);
            const QOrganizerJsonDbEngine::StorageLocation storageLocation = m_converter.storageLocationStringToEnum(watcher->partition());
            if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventType()
                || jsonDbType == QOrganizerJsonDbStr::jsonDbEventViewType()
                || jsonDbType == QOrganizerJsonDbStr::jsonDbEventOccurrenceType()
                || jsonDbType == QOrganizerJsonDbStr::jsonDbTodoType()
                || jsonDbType == QOrganizerJsonDbStr::jsonDbTodoOccurrenceType()) {
                switch (notifications.at(i).action()) {
                    case QJsonDbWatcher::Created:
                        emit itemAdded(m_converter.jsonDbNotificationObjectToItemId(jsonDbObject, storageLocation));
                        break;
                    case QJsonDbWatcher::Updated:
                        emit itemChanged(m_converter.jsonDbNotificationObjectToItemId(jsonDbObject, storageLocation));
                        break;
                    case QJsonDbWatcher::Removed:
                        emit itemRemoved(m_converter.jsonDbNotificationObjectToItemId(jsonDbObject, storageLocation));
                        break;
                    default:
                        break;
                }
            } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbCollectionType()) {
                switch (notifications.at(i).action()) {
                case QJsonDbWatcher::Created: {
                    QOrganizerCollectionId newlyAddedId = m_converter.jsonDbNotificationObjectToCollectionId(jsonDbObject, storageLocation);
                    m_collectionIds.insert(newlyAddedId);
                    emit collectionAdded(newlyAddedId);
                    break;
                }
                case QJsonDbWatcher::Updated: {
                    emit collectionChanged(m_converter.jsonDbNotificationObjectToCollectionId(jsonDbObject, storageLocation));
                    break;
                }
                case QJsonDbWatcher::Removed: {
                    QOrganizerCollectionId removedId = m_converter.jsonDbNotificationObjectToCollectionId(jsonDbObject, storageLocation);
                    m_collectionIds.remove(removedId);
                    emit collectionRemoved(removedId);
                    break;
                }
                default:
                    break;
                }
            }
        }
    }
}

void QOrganizerJsonDbDataStorage::handleRequest()
{
    switch (m_requestType) {
    case SaveItems:
        handleSaveItemsRequest();
        break;
    case Items:
        handleItemsRequest();
        break;
    case ItemsById:
        handleItemsByIdRequest();
        break;
    case RemoveItems:
        handleRemoveItemsRequest();
        break;
    case SaveCollections:
        handleSaveCollectionsRequest();
        break;
    case Collections:
        handleCollectionsRequest();
        break;
    case RemoveCollections:
        handleRemoveCollectionsRequest();
        break;
    default:
        break;
    }
}

void QOrganizerJsonDbDataStorage::handleResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    int index = -1;
    QMap<QJsonDbRequest *, int>::iterator i = m_requestIndexMap.find(request);
    if (i != m_requestIndexMap.end()) {
        index = i.value();
        m_requestIndexMap.erase(i);
    } else {
        // It might fall here if handling of some request was stopped due to timeout,
        // then remaining responses may come when no request is processed and m_requestIndexMap
        // has been cleared. In such cases, do nothing.
        return;
    }

    switch (m_requestType) {
    case SaveItems:
        handleSaveItemsResponse(index, error, request);
        break;
    case Items:
        handleItemsResponse(error, request);
        break;
    case ItemsById:
        handleItemsByIdResponse(error, request);
        break;
    case RemoveItems:
        handleRemoveItemsResponse(index, error);
        break;
    case SaveCollections:
        handleSaveCollectionsResponse(index, error, request);
        break;
    case Collections:
        handleCollectionsResponse(error, request);
        break;
    case RemoveCollections:
        handleRemoveCollectionsResponse(error);
        break;
    case Invalid:
        // no active request at the moment, internal variables have been cleared and some pointers have
        // NULL value, so response is just ignored
    default:
        break;
    }

    delete request;
}

void QOrganizerJsonDbDataStorage::handleSaveItemsRequest()
{
    bool requestSent = false;
    QMap<int, QOrganizerItem>::const_iterator i = m_resultItems->begin();
    while (i != m_resultItems->end()) {
        QOrganizerItemId itemId = i.value().id();
        bool itemIsNew = itemId.isNull();

        QJsonObject jsonDbItem;
        if (m_converter.itemToJsonDbObject(i.value(), &jsonDbItem)) {
            JsonDbRequestType requestType;
            QOrganizerJsonDbEngine::StorageLocation storageLocation(QOrganizerJsonDbEngine::UserDataStorage);
            if (itemIsNew) {
                requestType = JsonDbCreateRequest;
                // first save, storage location given in request. if not, by default store to UserData.
                storageLocation = m_saveToStorageLocation ? m_saveToStorageLocation : QOrganizerJsonDbEngine::UserDataStorage;
            } else {
                requestType = JsonDbUpdateRequest;
                const QOrganizerJsonDbItemId *itemIdPtr = static_cast<const QOrganizerJsonDbItemId *>(QOrganizerManagerEngine::engineItemId(itemId));
                if (!itemIdPtr) {
                    *m_error = QOrganizerManager::UnspecifiedError;
                    m_errorMap->insert(i.key(), *m_error);
                } else {
                    // item has already been saved before, use location from id
                    storageLocation = itemIdPtr->storageLocation();
                }
            }
            if (makeJsonDbRequest(requestType, i.key(), storageLocation, QString(), QList<QJsonObject>() << jsonDbItem))
                requestSent = true;
        } else {
            *m_error = QOrganizerManager::InvalidItemTypeError;
            m_errorMap->insert(i.key(), *m_error);
        }

        ++i;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveItemsResponse(int index, QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        if (results.size() > 0) {
            QOrganizerItem item = m_resultItems->value(index);
            QJsonObject jsonDbItem = results.at(0);
            bool gotUuid = !item.id().isNull();
            bool gotVersion = false;
            QJsonObject::const_iterator i = jsonDbItem.constBegin();
            while (i != jsonDbItem.constEnd()) {
                if (!gotUuid && i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
                    // it's a new item, and we need to set the item ID
                    gotUuid = true;
                    QString jsonDbUuid = i.value().toString();
                    if (!jsonDbUuid.isEmpty()) {
                        QOrganizerJsonDbItemId* itemId = new QOrganizerJsonDbItemId;
                        itemId->setJsonDbUuid(jsonDbUuid);
                        itemId->setStorageLocation(m_converter.storageLocationStringToEnum(request->partition()));
                        item.setId(QOrganizerItemId(itemId));
                    }
                } else if (!gotVersion && i.key() == QOrganizerJsonDbStr::jsonDbVersion()) {
                    gotVersion = true;
                    QString jsonDbVersion = i.value().toString();
                    if (!jsonDbVersion.isEmpty()) {
                        QOrganizerItemVersion itemVersion = item.detail(QOrganizerItemDetail::TypeVersion);
                        m_converter.jsonDbVersionToItemVersion(jsonDbVersion, &itemVersion);
                        item.saveDetail(&itemVersion);
                    }
                }
                if (gotUuid && gotVersion)
                    break;
                ++i;
            }

            m_resultItems->insert(index, item);
        }
    } else {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleItemsRequest()
{
    QString jsonDbQuery;

    switch (m_fetchType) {
    case FetchItems:
        // break;
    case FetchItemIds:
    {
        // TODO: it would be enough to get just a list of uuids from db
        // For now we fetch the whole item, even though more optimal would be to fetch
        // only uuid, startdate and enddate fields

        // for now, this is common for FetchItems and FetchItemIds
        // Apply Filter and get jsondb query expression
        QList<QString> jsonDbQueryList;
        if (m_converter.createJsonDbQuery(m_filter, m_start, m_end, &jsonDbQueryList)) {
            foreach (const QString &jsonDbQuery, jsonDbQueryList) {
                foreach (QOrganizerJsonDbEngine::StorageLocation location, m_availableStorageLocations) {
                    if (m_fetchFromStorageLocations & location)
                        makeJsonDbRequest(JsonDbReadRequest, 0, location, jsonDbQuery);
                }
            }
        } else {
            *m_error = QOrganizerManager::BadArgumentError;
        }
        break;
    }
    case FetchParents:
    {
        // This is used for fetching all parent items
        foreach (QOrganizerJsonDbEngine::StorageLocation location, m_availableStorageLocations) {
            if (m_fetchFromStorageLocations & location) {
                if (makeJsonDbRequest(JsonDbReadRequest, 0, location, QOrganizerJsonDbStr::jsonDbQueryParentItems())) {
                    if (location == QOrganizerJsonDbEngine::SystemStorage) {
                        // can't query normal object and view object at the same time
                        // TODO only query view objects for when needed
                        makeJsonDbRequest(JsonDbReadRequest, 0, location, QOrganizerJsonDbStr::jsonDbQueryEventViewParentItems());
                    }
                }
            }
        }
        break;
    }
    case FetchItemOccurrences:
    {
        const QOrganizerItemEngineId *itemIdPtr = QOrganizerManagerEngine::engineItemId(m_parentItemId);
        if (itemIdPtr) {
            jsonDbQuery = QOrganizerJsonDbStr::jsonDbQueryOccurrenceItemsByParent()
                              .arg(itemIdPtr->toString());
            foreach (QOrganizerJsonDbEngine::StorageLocation location, m_availableStorageLocations) {
                if (m_fetchFromStorageLocations & location)
                    makeJsonDbRequest(JsonDbReadRequest, 0, location, jsonDbQuery);
            }
        }
        break;
    }
    default:
        break;
    }

    if (m_requestIndexMap.isEmpty())
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleItemsResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        for (int i = 0; i < results.size(); ++i) {
            QOrganizerItem item;
            if (m_converter.jsonDbObjectToItem(results.at(i), &item, m_converter.storageLocationStringToEnum(request->partition()))) {
                if (m_start.isValid() || m_end.isValid()) {
                    QOrganizerItemType::ItemType itemType = item.type();
                    if ((QOrganizerItemType::TypeTodo == itemType || QOrganizerItemType::TypeTodoOccurrence == itemType)
                        && !QOrganizerManagerEngine::isItemBetweenDates(item, m_start, m_end)) {
                            continue;
                    }
                }
                // if m_fetchType is FetchParents, items are always added to m_items, because m_start and m_end
                // are default constructed QDateTimes
                m_items.append(item);
            }
        }
    } else {
        *m_error = error;
    }

    // when querying view objects, 2 queries are fired, so need to check
    if (m_requestIndexMap.isEmpty())
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleItemsByIdRequest()
{
    const QString uuidTemplate(QStringLiteral("\"%1\","));
    QString itemQuery;
    for (int i = 0; i < m_itemIds.size(); ++i)
        itemQuery += uuidTemplate.arg(m_itemIds.at(i).isNull() ?
            QString() : QOrganizerJsonDbItemId(m_itemIds.at(i).toString()).jsondbUuid());

    // remove the last ","
    itemQuery.truncate(itemQuery.length() - 1);

    QString newJsonDbQuery(QOrganizerJsonDbStr::jsonDbQueryAllItems());
    newJsonDbQuery.append(QOrganizerJsonDbStr::jsonDbQueryUuidsTemplate().arg(itemQuery));

    QString viewObjectJsonDbQuery(QOrganizerJsonDbStr::jsonDbQueryEventViews());
    viewObjectJsonDbQuery.append(QOrganizerJsonDbStr::jsonDbQueryUuidsTemplate().arg(itemQuery));

    foreach (QOrganizerJsonDbEngine::StorageLocation location, m_availableStorageLocations) {
        if (m_fetchFromStorageLocations & location) {
            makeJsonDbRequest(JsonDbReadRequest, 0, location, newJsonDbQuery);
            if (location == QOrganizerJsonDbEngine::SystemStorage)
                makeJsonDbRequest(JsonDbReadRequest, 0, location, viewObjectJsonDbQuery);
        }
    }

    if (m_requestIndexMap.isEmpty())
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleItemsByIdResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        for (int i = 0; i < results.size(); ++i) {
            QOrganizerItem *item = new QOrganizerItem;
            if (m_converter.jsonDbObjectToItem(results.at(i), item, m_converter.storageLocationStringToEnum(request->partition()))) {
                m_idItemMap.insert(item->id(), *item);
            }
        }
    } else {
        *m_error = error;
    }

    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed

        for (int i = 0; i < m_itemIds.size(); ++i) {
            QOrganizerItem item = m_idItemMap.value(m_itemIds[i], QOrganizerItem());
            m_items.append(item);
            if (item.id().isNull()) {
                *m_error = QOrganizerManager::DoesNotExistError;
                m_errorMap->insert(i, *m_error);
            }
        }

        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsRequest()
{
    bool requestSent = false;
    for (int i = 0; i < m_itemIds.size(); ++i) {
        QJsonObject jsonDbItem;
        if (m_itemIds.at(i).isNull()) {
            *m_error = QOrganizerManager::DoesNotExistError;
            m_errorMap->insert(i, QOrganizerManager::DoesNotExistError);
            continue;
        }
        const QOrganizerJsonDbItemId jsondbItemId(m_itemIds.at(i).toString());
        jsonDbItem.insert(QOrganizerJsonDbStr::jsonDbUuid(), jsondbItemId.jsondbUuid());
        if (makeJsonDbRequest(JsonDbRemoveRequest, i, jsondbItemId.storageLocation(), QString(), QList<QJsonObject>() << jsonDbItem))
            requestSent = true;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsResponse(int index, QOrganizerManager::Error error)
{
    if (error != QOrganizerManager::NoError) {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleSaveCollectionsRequest()
{
    bool requestSent = false;
    QMap<int, QOrganizerCollection>::const_iterator i = m_resultCollections->constBegin();
    while (i != m_resultCollections->constEnd()) {
        QOrganizerCollection collection = i.value();
        bool collectionIsNew = collection.id().isNull();

        // check whether this is a default collection
        // if we are creating a new default collection, then m_isDefaultCollection is true
        // if we are modifying an existing default collection, then collection id should be the same
        // as m_defaultCollection id.

        // ignore storageLocation on request, if collection is updated (it has been saved already atleast once)

        bool convertToDefaultCollection;
        QOrganizerJsonDbEngine::StorageLocation storageLocation(QOrganizerJsonDbEngine::UserDataStorage);
        if (collectionIsNew) {
            convertToDefaultCollection = m_isDefaultCollection;
            // first save, storage location given in request. if not, by default store to UserData.
            storageLocation = m_saveToStorageLocation ? m_saveToStorageLocation : QOrganizerJsonDbEngine::UserDataStorage;
        } else {
            const QOrganizerJsonDbCollectionId *collectionIdPtr = static_cast<const QOrganizerJsonDbCollectionId *>(QOrganizerManagerEngine::engineCollectionId(collection.id()));
            if (collectionIdPtr) {
                convertToDefaultCollection = (collection.id() == m_defaultCollection.id());
                // collection has already been saved before, so we get storage location from id
                storageLocation = collectionIdPtr->storageLocation();
            }
        }

        QJsonObject jsonDbCollection;
        if (m_converter.collectionToJsonDbObject(collection, convertToDefaultCollection, &jsonDbCollection)) {
            JsonDbRequestType requestType;
            if (collectionIsNew)
                requestType = JsonDbCreateRequest;
            else
                requestType = JsonDbUpdateRequest;
            if (makeJsonDbRequest(requestType, i.key(), storageLocation, QString(), QList<QJsonObject>() << jsonDbCollection))
                requestSent = true;
        } else {
            *m_error = QOrganizerManager::InvalidCollectionError;
            m_errorMap->insert(i.key(), *m_error);
        }
        ++i;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveCollectionsResponse(int index, QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QOrganizerCollection collection = m_resultCollections->value(index);
        if (collection.id().isNull()) {
            QList<QJsonObject> results = request->takeResults();
            if (results.size() > 0) {
                QString jsonUuid = results.at(0).value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
                if (!jsonUuid.isEmpty()) {
                    QOrganizerJsonDbCollectionId *collectionId = new QOrganizerJsonDbCollectionId;
                    collectionId->setJsonDbUuid(jsonUuid);
                    collectionId->setStorageLocation(m_converter.storageLocationStringToEnum(request->partition()));
                    collection.setId(QOrganizerCollectionId(collectionId));
                    m_collectionIds.insert(collection.id());
                    // have we modified default collection
                    if (collection.id() == m_defaultCollection.id())
                        m_defaultCollection = collection;
                }
            }
            m_resultCollections->insert(index, collection);
        }
    } else {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleCollectionsRequest()
{
    foreach (QOrganizerJsonDbEngine::StorageLocation location, m_availableStorageLocations) {
        if (m_fetchFromStorageLocations & location)
            makeJsonDbRequest(JsonDbReadRequest, 0, location, QOrganizerJsonDbStr::jsonDbQueryAllCollections());
    }

    if (m_requestIndexMap.isEmpty())
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleCollectionsResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        for (int i = 0; i < results.size(); ++i) {
            QOrganizerCollection collection;
            bool isDefaultCollection(false);
            if (m_converter.jsonDbObjectToCollection(results.at(i), &collection, &isDefaultCollection, m_converter.storageLocationStringToEnum(request->partition()))) {
                if (isDefaultCollection)
                    m_defaultCollection = collection;
                m_collections.append(collection);
                m_collectionIds.insert(collection.id());
            }
        }
    } else {
        *m_error = error;
    }

    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleRemoveCollectionsRequest()
{
    QMap<int, QOrganizerCollectionId>::const_iterator i = m_removeCollectionIds.constBegin();
    while (i != m_removeCollectionIds.constEnd()) {
        QJsonObject jsonDbObject;
        const QOrganizerCollectionEngineId *collectionIdPtr = QOrganizerManagerEngine::engineCollectionId(i.value());
        if (collectionIdPtr) {
            QOrganizerJsonDbCollectionId jsondbCollectionId(collectionIdPtr->toString());
            jsonDbObject.insert(QOrganizerJsonDbStr::jsonDbUuid(), jsondbCollectionId.jsondbUuid());
            makeJsonDbRequest(JsonDbRemoveRequest, 0, jsondbCollectionId.storageLocation(), QString(), QList<QJsonObject>() << jsonDbObject);
        }
        ++i;
    }

    if (m_requestIndexMap.isEmpty())
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveCollectionsResponse(QOrganizerManager::Error error)
{
    if (error == QOrganizerManager::NoError) {
        QMap<int, QOrganizerCollectionId>::const_iterator i = m_removeCollectionIds.constBegin();
        while (i != m_removeCollectionIds.constEnd()) {
            m_collectionIds.remove(i.value());
            ++i;
        }
    } else {
        *m_error = error;
        QMap<int, QOrganizerCollectionId>::const_iterator i = m_removeCollectionIds.constBegin();
        while (i != m_removeCollectionIds.constEnd()) {
            m_errorMap->insert(i.key(), error);
            ++i;
        }
    }
    m_syncWaitCondition.wakeAll();
}

bool QOrganizerJsonDbDataStorage::makeJsonDbRequest(
        JsonDbRequestType jsonDbRequestType, int index, QOrganizerJsonDbEngine::StorageLocation storageLocation, const QString &query, const QList<QJsonObject> &objects)
{
    QJsonDbRequest *request = 0;
    switch (jsonDbRequestType) {
    case JsonDbReadRequest:
        request = new QJsonDbReadRequest(query, this);
        break;
    case JsonDbCreateRequest:
        request = new QJsonDbCreateRequest(objects, this);
        break;
    case JsonDbUpdateRequest:
        request = new QJsonDbUpdateRequest(objects, this);
        break;
    case JsonDbRemoveRequest:
        request = new QJsonDbRemoveRequest(objects, this);
        break;
    default:
        return false;
    }
    QJsonDbWriteRequest *writeRequest = qobject_cast<QJsonDbWriteRequest*>(request);
    if (writeRequest)
        writeRequest->setConflictResolutionMode(QJsonDbWriteRequest::Replace);
    const QStringList storageLocationStrings = m_converter.storageLocationsFlagToStrings(storageLocation);
    request->setPartition(storageLocationStrings.isEmpty() ?
                              m_converter.storageLocationsFlagToStrings(QOrganizerJsonDbEngine::UserDataStorage).first() :
                              storageLocationStrings.first());

    connect(request, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)),
            this, SLOT(onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode,QString)));
    connect(request, SIGNAL(finished()), this, SLOT(onJsonDbRequestFinished()));

    if (m_jsonDbConnection->send(request)) {
        m_requestIndexMap.insert(request, index);
        return true;
    } else {
        delete request;
        *m_error = QOrganizerManager::UnspecifiedError;
        if (m_errorMap)
            m_errorMap->insert(index, QOrganizerManager::UnspecifiedError);
    }
    return false;
}

void QOrganizerJsonDbDataStorage::processRequest()
{
    // storage location related checks
    if (m_mandatoryStorageLocationMissing) {
        *m_error = QOrganizerManager::UnspecifiedError;
        return;
    }

    emit requestInitialized();
    QMutexLocker locker(m_waitMutex);
    int msecs = 10000; // TODO: handle timeout nicely
    bool requestFinished = m_syncWaitCondition.wait(m_waitMutex, msecs);
    if (!requestFinished) {
        qWarning() << "Timeout, no response received!!!";
        // timeout TODO: check which items have completed and update error map accordingly...
    }
}

void QOrganizerJsonDbDataStorage::initRequestData(RequestType requestType, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    m_requestType = requestType;
    m_errorMap = errorMap;
    m_error = error;
    m_requestIndexMap.clear();
    m_resultItems = 0;
    m_resultCollections = 0;
    m_idItemMap.clear();
    m_items.clear();
    QOrganizerItemId tmpId;
    m_parentItemId = tmpId;
    m_fetchType = NoFetch;
    m_itemIds.clear();
    m_collections.clear();
    m_removeItemCollectionIds.clear();
    m_removeItemParentIds.clear();
    m_removeCollectionIds.clear();
    m_isDefaultCollection = false;
    m_saveToStorageLocation = QOrganizerJsonDbEngine::UserDataStorage;
    m_fetchFromStorageLocations = 0;
}

void QOrganizerJsonDbDataStorage::clearRequestData()
{
    m_requestType = Invalid;
    m_errorMap = 0;
    m_error = 0;
    m_requestIndexMap.clear();
    m_resultItems = 0;
    m_resultCollections = 0;
    m_idItemMap.clear();
    m_items.clear();
    QOrganizerItemId tmpId;
    m_parentItemId = tmpId;
    m_fetchType = NoFetch;
    m_itemIds.clear();
    m_collections.clear();
    m_removeItemCollectionIds.clear();
    m_removeItemParentIds.clear();
    m_removeCollectionIds.clear();
    m_isDefaultCollection = false;
    m_saveToStorageLocation = QOrganizerJsonDbEngine::UserDataStorage;
    m_fetchFromStorageLocations = 0;
}

#include "moc_qorganizerjsondbdatastorage.cpp"

QT_END_NAMESPACE_ORGANIZER
