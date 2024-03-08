// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtTest>
#include <QIfAbstractFeature>
#include <QIfServiceManager>
#include <QIfPagingModel>
#include <private/qifpagingmodel_p.h>
#include <QIfPagingModelInterface>
#include <QIfStandardItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QScopedPointer>
#include <private/qobject_p.h>

//TODO Add test with multiple model instances, requesting different data at the same time
//TODO Test the signal without a valid identifier

class PagingTestGadget
{
    Q_GADGET
};

class PagingTestBackend : public QIfPagingModelInterface
{
    Q_OBJECT

public:

    //Sets the capabilities this instance should report
    void setCapabilities(QtInterfaceFrameworkModule::ModelCapabilities capabilities)
    {
        m_caps = capabilities;
    }

    //Adds very simple Data which can be used for most of the unit tests
    void initializeSimpleData()
    {
        m_list = createItemList("simple");
    }

    QVariantList createItemList(const QString &name)
    {
        QVariantList list;
        for (int i=0; i<100; i++) {
            QIfStandardItem item;
            item.setId(name + QLatin1String(" ") + QString::number(i));
            QVariantMap map;
            map.insert("type", name);
            map.insert("index", i);
            item.setData(map);
            list.append(QVariant::fromValue(item));
        }
        return list;
    }

    void initializeBrokenData()
    {
        m_list.append(QVariant());
        m_list.append(QVariant("string"));
        m_list.append(QVariant::fromValue(PagingTestGadget()));
    }

    void initialize() override
    {
        emit initializationDone();
    }

    void registerInstance(const QUuid &identifier) override
    {
        emit registerInstanceCalled(identifier);
    }

    void unregisterInstance(const QUuid &identifier) override
    {
        emit unregisterInstanceCalled(identifier);
    }

    void fetchData(const QUuid &identifier, int start, int count) override
    {
        emit supportedCapabilitiesChanged(identifier, m_caps);

        if (m_caps.testFlag(QtInterfaceFrameworkModule::SupportsGetSize))
            emit countChanged(identifier, m_list.count());

        QVariantList requestedItems;

        int size = qMin(start + count, m_list.count());
        for (int i = start; i < size; i++)
            requestedItems.append(m_list.at(i));

        emit dataFetched(identifier, requestedItems, start, start + count < m_list.count());
    }

    void insert(int index, const QIfStandardItem item)
    {
        m_list.insert(index, QVariant::fromValue(item));
        QVariantList variantList = { QVariant::fromValue(item) };

        emit dataChanged(QUuid(), variantList, index, 0);
    }

    void remove(int index)
    {
        m_list.removeAt(index);

        emit dataChanged(QUuid(), QVariantList(), index, 1);
    }

    void move(int currentIndex, int newIndex)
    {
        int min = qMin(currentIndex, newIndex);
        int max = qMax(currentIndex, newIndex);

        m_list.move(currentIndex, newIndex);
        QVariantList variantLIst;
        for (int i = min; i <= max; i++)
            variantLIst.append(m_list.at(i));

        emit dataChanged(QUuid(), variantLIst, min, max - min + 1);
    }

Q_SIGNALS:
    void registerInstanceCalled(const QUuid &identifier);
    void unregisterInstanceCalled(const QUuid &identifier);

private:
    QVariantList m_list;
    QtInterfaceFrameworkModule::ModelCapabilities m_caps;
};

class PagingTestServiceObject : public QIfServiceObject
{
    Q_OBJECT

public:
    explicit PagingTestServiceObject(QObject *parent = nullptr) :
        QIfServiceObject(parent)
    {
        m_backend = new PagingTestBackend;
        m_backend->setParent(this);
        m_interfaces << QIfPagingModel_iid;
    }

    QStringList interfaces() const override { return m_interfaces; }
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override
    {
        if (interface == QIfPagingModel_iid)
            return testBackend();
        else
            return 0;
    }

    PagingTestBackend *testBackend() const
    {
        return m_backend;
    }

private:
    QStringList m_interfaces;
    PagingTestBackend *m_backend;
};

void verifyQml(QQmlEngine *engine, const QByteArray &qml)
{
    QQmlComponent component(engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));
}

class tst_QIfPagingModel : public QObject
{
    Q_OBJECT

public:
    tst_QIfPagingModel();

private Q_SLOTS:
    void cleanup();

    void testClearServiceObject();

    void testRegisterInstance();
    void testBasic_qml();
    void testBrokenData();
    void testGetAt();
    void testFetchMore_data();
    void testFetchMore();
    void testDataChangedMode();
    void testReload();
    void testDataChangedMode_jump();
    void testEditing();
    void testMissingCapabilities();

private:
    QIfServiceManager *manager;
};

tst_QIfPagingModel::tst_QIfPagingModel()
    : manager(QIfServiceManager::instance())
{
}

void tst_QIfPagingModel::cleanup()
{
    manager->unloadAllBackends();
}

void tst_QIfPagingModel::testClearServiceObject()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);

    QIfPagingModel defaultModel;
    QIfPagingModel model;
    model.setServiceObject(service);

    model.setLoadingType(QIfPagingModel::DataChanged);
    model.setChunkSize(20);
    model.setFetchMoreThreshold(20);

    QSignalSpy chunkSizeSpy(&model, &QIfPagingModel::chunkSizeChanged);
    QVERIFY(model.chunkSize() != defaultModel.chunkSize());
    QSignalSpy thresholdSpy(&model, &QIfPagingModel::fetchMoreThresholdChanged);
    QVERIFY(model.fetchMoreThreshold() != defaultModel.fetchMoreThreshold());
    QSignalSpy capabilitiesSpy(&model, &QIfPagingModel::capabilitiesChanged);
    QVERIFY(model.capabilities() != defaultModel.capabilities());
    QSignalSpy loadingTypeSpy(&model, &QIfPagingModel::loadingTypeChanged);
    QVERIFY(model.loadingType() != defaultModel.loadingType());
    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QVERIFY(model.rowCount() != defaultModel.rowCount());

    QVERIFY(model.setServiceObject(nullptr));

    QVERIFY(model.chunkSize() == defaultModel.chunkSize());
    QCOMPARE(chunkSizeSpy.count(), 1);
    QVERIFY(model.fetchMoreThreshold() == defaultModel.fetchMoreThreshold());
    QCOMPARE(thresholdSpy.count(), 1);
    QVERIFY(model.capabilities() == defaultModel.capabilities());
    QCOMPARE(capabilitiesSpy.count(), 1);
    QVERIFY(model.loadingType() == defaultModel.loadingType());
    QCOMPARE(loadingTypeSpy.count(), 1);
    QVERIFY(model.rowCount() == defaultModel.rowCount());
    QCOMPARE(resetSpy.count(), 1);
}

void tst_QIfPagingModel::testRegisterInstance()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QSignalSpy registerSpy(service->testBackend(), SIGNAL(registerInstanceCalled(QUuid)));
    QIfPagingModel firstModel;
    firstModel.setServiceObject(service);
    QCOMPARE(registerSpy.count(), 1);
    auto *firstModelPrivate = reinterpret_cast<QIfPagingModelPrivate*> (QObjectPrivate::get(&firstModel));
    QUuid firstModelIdentifier = firstModelPrivate->m_identifier;
    QCOMPARE(registerSpy.at(0).at(0).toUuid(), firstModelIdentifier);

    QIfPagingModel secondModel;
    secondModel.setServiceObject(service);
    QCOMPARE(registerSpy.count(), 2);
    auto *secondModelPrivate = reinterpret_cast<QIfPagingModelPrivate*> (QObjectPrivate::get(&secondModel));
    QUuid secondModelIdentifier = secondModelPrivate->m_identifier;
    QCOMPARE(registerSpy.at(1).at(0).toUuid(), secondModelIdentifier);

    QSignalSpy unregisterSpy(service->testBackend(), SIGNAL(unregisterInstanceCalled(QUuid)));
    secondModel.setServiceObject(nullptr);
    QCOMPARE(unregisterSpy.count(), 1);
    QCOMPARE(unregisterSpy.at(0).at(0).toUuid(), secondModelIdentifier);

    firstModel.setServiceObject(nullptr);
    QCOMPARE(unregisterSpy.count(), 2);
    QCOMPARE(unregisterSpy.at(1).at(0).toUuid(), firstModelIdentifier);
}

void tst_QIfPagingModel::testBasic_qml()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("testBackend", service);
    verifyQml(&engine, "import QtQuick; import QtInterfaceFramework; PagingModel{}");
    verifyQml(&engine, "import QtQuick; import QtInterfaceFramework; PagingModel{ \
                            serviceObject: testBackend \n\
                        }");
}

void tst_QIfPagingModel::testBrokenData()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeBrokenData();

    QIfPagingModel model;
    model.setServiceObject(service);

    QTest::ignoreMessage(QtWarningMsg, "The passed QVariant is undefined");
    QIfStandardItem item = model.at<QIfStandardItem>(0);

    QTest::ignoreMessage(QtWarningMsg, "The passed QVariant needs to use the Q_GADGET macro");
    QIfStandardItem item2 = model.at<QIfStandardItem>(1);

    QTest::ignoreMessage(QtWarningMsg, "The passed QVariant is not derived from QIfStandardItem");
    QIfStandardItem item3 = model.at<QIfStandardItem>(2);
}

void tst_QIfPagingModel::testGetAt()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    model.setServiceObject(service);

    QIfStandardItem item = model.at<QIfStandardItem>(0);
    QCOMPARE(item.id(), QLatin1String("simple 0"));

    QVariant var = model.get(0);
    QCOMPARE(var.value<QIfStandardItem>().id(), item.id());
}

void tst_QIfPagingModel::testFetchMore_data()
{
    QTest::addColumn<int>("chunkSize");
    QTest::addColumn<int>("fetchMoreThreshold");
    QTest::newRow("normal") << -1 << -1;
    QTest::newRow("custom chunkSize") << 20 << -1;
    QTest::newRow("custom fetchMoreThreshold") << -1 << 2;
}

void tst_QIfPagingModel::testFetchMore()
{
    QFETCH(int, chunkSize);
    QFETCH(int, fetchMoreThreshold);

    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    model.setServiceObject(service);

    if (chunkSize != -1) {
        QSignalSpy chunkSizeChangedSpy(&model, SIGNAL(chunkSizeChanged(int)));
        model.setChunkSize(model.chunkSize());
        QVERIFY(!chunkSizeChangedSpy.count());

        model.setChunkSize(chunkSize);
        QCOMPARE(model.chunkSize(), chunkSize);
        QVERIFY(chunkSizeChangedSpy.count());
        model.reload();
    }

    // Set the threshold
    if (fetchMoreThreshold != -1) {
        QSignalSpy fetchMoreThresholdChangedSpy(&model, SIGNAL(fetchMoreThresholdChanged(int)));
        model.setFetchMoreThreshold(model.fetchMoreThreshold());
        QVERIFY(!fetchMoreThresholdChangedSpy.count());

        model.setFetchMoreThreshold(fetchMoreThreshold);
        QCOMPARE(model.fetchMoreThreshold(), fetchMoreThreshold);
        QVERIFY(fetchMoreThresholdChangedSpy.count());
    }

    QVERIFY(model.serviceObject());
    QCOMPARE(model.loadingType(), QIfPagingModel::FetchMore);

    QSignalSpy fetchMoreThresholdSpy(&model, SIGNAL(fetchMoreThresholdReached()));

    // Ask for an item before the threshold, shouldn't trigger the threshold reached signal and fetch new data.
    int offset = model.fetchMoreThreshold() + 1;
    QCOMPARE(model.at<QIfStandardItem>(model.chunkSize() - offset).id(),
             QLatin1String("simple ") + QString::number(model.chunkSize() - offset));
    QVERIFY(!fetchMoreThresholdSpy.count());

    QCOMPARE(model.rowCount(), model.chunkSize());
    // By using model.at we already prefetch the next chunk of data
    QCOMPARE(model.at<QIfStandardItem>(model.chunkSize() - 1).id(), QLatin1String("simple ") + QString::number(model.chunkSize() - 1));

    QVERIFY(fetchMoreThresholdSpy.count());
    fetchMoreThresholdSpy.clear();
    QCOMPARE(model.rowCount(), model.chunkSize() * 2);

//    QListView view;
//    view.setModel(&model);
//    view.show();
//    QVERIFY(QTest::qWaitForWindowExposed(&view));
    //view.scrollTo(model.index(model.chunkSize()));

    //qDebug() << model.rowCount();
}

void tst_QIfPagingModel::testDataChangedMode()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    model.setServiceObject(service);
    QVERIFY(model.serviceObject());

    //TODO remove this section once we have fixed the capability race
    QSignalSpy fetchMoreThresholdSpy(&model, SIGNAL(fetchMoreThresholdReached()));
    QCOMPARE(model.rowCount(), model.chunkSize());
    QCOMPARE(model.at<QIfStandardItem>(model.chunkSize() - 1).id(), QLatin1String("simple ") + QString::number(model.chunkSize() - 1));
    QVERIFY(fetchMoreThresholdSpy.count());
    fetchMoreThresholdSpy.clear();

    QSignalSpy loadingTypeChangedSpy(&model, SIGNAL(loadingTypeChanged(QIfPagingModel::LoadingType)));
    model.setLoadingType(model.loadingType());
    QVERIFY(!loadingTypeChangedSpy.count());

    model.setLoadingType(QIfPagingModel::DataChanged);
    QCOMPARE(model.loadingType(), QIfPagingModel::DataChanged);
    QVERIFY(loadingTypeChangedSpy.count());

    QCOMPARE(model.rowCount(), 100);
    // model.chunkSize() when workaround is removed
    int testIndex = model.chunkSize() - 1;

    // Asking for an item near inside the threshold range should trigger a new fetch.
    QSignalSpy fetchDataSpy(service->testBackend(), SIGNAL(dataFetched(const QUuid &, const QList<QVariant> &, int , bool )));
    QCOMPARE(model.at<QIfStandardItem>(testIndex).id(), QLatin1String("simple ") + QString::number(testIndex));
    QVERIFY(fetchMoreThresholdSpy.count());
    QVERIFY(fetchDataSpy.count());

    // Test that we really fetched new data
    QCOMPARE(fetchDataSpy.at(0).at(2).toInt(), testIndex + 1);
}

void tst_QIfPagingModel::testReload()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    QSignalSpy countChangedSpy(&model, SIGNAL(countChanged()));

    model.setServiceObject(service);
    QVERIFY(model.serviceObject());
    countChangedSpy.wait(100);
    QVERIFY(countChangedSpy.count());

    QCOMPARE(model.rowCount(), model.chunkSize());
    countChangedSpy.clear();

    QSignalSpy resetSpy(&model, SIGNAL(modelReset()));
    model.reload();
    countChangedSpy.wait(100);
    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countChangedSpy.count(), 2);
    QCOMPARE(model.rowCount(), model.chunkSize());
}

void tst_QIfPagingModel::testDataChangedMode_jump()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    model.setServiceObject(service);
    QVERIFY(model.serviceObject());

    QSignalSpy loadingTypeChangedSpy(&model, SIGNAL(loadingTypeChanged(QIfPagingModel::LoadingType)));
    model.setLoadingType(model.loadingType());
    QVERIFY(!loadingTypeChangedSpy.count());

    model.setLoadingType(QIfPagingModel::DataChanged);
    QCOMPARE(model.loadingType(), QIfPagingModel::DataChanged);
    QVERIFY(loadingTypeChangedSpy.count());

    QCOMPARE(model.rowCount(), 100);

    // Ask for the last item. This should just fetch the chunk which is needed.
    QSignalSpy dataChangedSpy(&model, SIGNAL(dataChanged(const QModelIndex, const QModelIndex, const QVector<int>)));
    QSignalSpy fetchDataSpy(service->testBackend(), SIGNAL(dataFetched(const QUuid &, const QList<QVariant> &, int , bool )));
    model.get(99);
    dataChangedSpy.wait();
    QCOMPARE(model.at<QIfStandardItem>(99).id(), QLatin1String("simple ") + QString::number(99));
    QVERIFY(fetchDataSpy.count());

    // Test that we really fetched new data
    int chunkBegin = int(99 / model.chunkSize()) * model.chunkSize();
    QCOMPARE(fetchDataSpy.at(0).at(2).toInt(), chunkBegin);
}

void tst_QIfPagingModel::testEditing()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    model.setServiceObject(service);

    QCOMPARE(model.at<QIfStandardItem>(0).id(), QLatin1String("simple 0"));

    QIfStandardItem newItem;
    newItem.setId(QLatin1String("testItem"));

    // Add a new Item
    QSignalSpy insertSpy(&model, SIGNAL(rowsInserted(const QModelIndex &, int , int )));
    service->testBackend()->insert(0, newItem);
    QVERIFY(insertSpy.count());
    QCOMPARE(insertSpy.at(0).at(1).toInt(), 0);
    QCOMPARE(insertSpy.at(0).at(2).toInt(), 0);

    QCOMPARE(model.at<QIfStandardItem>(0).id(), newItem.id());

    // Move the item to a new location
    QSignalSpy moveSpy(&model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
    int newIndex = 10;
    service->testBackend()->move(0, newIndex);
    QVERIFY(moveSpy.count());
    QCOMPARE(moveSpy.at(0).at(0).toModelIndex().row(), 0);
    QCOMPARE(moveSpy.at(0).at(1).toModelIndex().row(), newIndex);

    QCOMPARE(model.at<QIfStandardItem>(newIndex).id(), newItem.id());

    // Remove the item again
    QSignalSpy removedSpy(&model, SIGNAL(rowsRemoved(const QModelIndex &, int , int )));
    service->testBackend()->remove(newIndex);
    QVERIFY(removedSpy.count());
    QCOMPARE(removedSpy.at(0).at(1).toInt(), newIndex);
    QCOMPARE(removedSpy.at(0).at(2).toInt(), newIndex);

    QCOMPARE(model.at<QIfStandardItem>(newIndex).id(), QLatin1String("simple 10"));
}

void tst_QIfPagingModel::testMissingCapabilities()
{
    PagingTestServiceObject *service = new PagingTestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfPagingModel model;
    model.setServiceObject(service);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support the DataChanged loading type. This call will have no effect");
    model.setLoadingType(QIfPagingModel::DataChanged);
    QCOMPARE(model.loadingType(), QIfPagingModel::FetchMore);
}

QTEST_MAIN(tst_QIfPagingModel)

#include "tst_qifpagingmodel.moc"

