// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtTest>
#include <QIfAbstractFeature>
#include <QIfServiceManager>
#include <QIfFilterAndBrowseModel>
#include <QIfFilterAndBrowseModelInterface>
#include <QIfStandardItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

bool operator<=(const QVariant &left, const QVariant &right)
{
    if (left.canConvert(QMetaType::fromType<double>()) && right.canConvert(QMetaType::fromType<double>()))
        return left.toDouble() <= right.toDouble();
    else if (left.canConvert(QMetaType::fromType<QString>()) && right.canConvert(QMetaType::fromType<QString>()))
        return left.toString() <= right.toString();

    qCritical() << "Can't compare these types:" << left << right;
    return false;
}

bool operator>=(const QVariant &left, const QVariant &right)
{
    if (left.canConvert(QMetaType::fromType<double>()) && right.canConvert(QMetaType::fromType<double>()))
        return left.toDouble() >= right.toDouble();
    else if (left.canConvert(QMetaType::fromType<QString>()) && right.canConvert(QMetaType::fromType<QString>()))
        return left.toString() >= right.toString();

    qCritical() << "Can't compare these types:" << left << right;
    return false;
}

bool operator<(const QVariant &left, const QVariant &right)
{
    return !(left >= right);
}

bool operator>(const QVariant &left, const QVariant &right)
{
    return !(left <= right);
}

QT_END_NAMESPACE

class TestBackend : public QIfFilterAndBrowseModelInterface
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
        m_lists.insert("simple", createItemList("simple"));
    }

    //Adds multiple sets of data which can be used for the navigation
    void initializeNavigationData()
    {
        static const QStringList types = { "levelOne", "levelTwo", "levelThree" };
        for (const QString &type : types)
            m_lists.insert(type, createItemList(type));
    }

    //Adds a data set which can be filtered and sorted
    void initializeFilterData()
    {
        QString type("filter");

        QList<QIfStandardItem> list;
        for (int i=0; i<100; i++) {
            QIfStandardItem item;
            item.setId(QString::number(i));
            QVariantMap map;
            map.insert("type", type);
            map.insert("index", i);
            item.setData(map);
            list.append(item);
        }
        m_lists.insert(type, list);
    }

    QList<QIfStandardItem> createItemList(const QString &name)
    {
        QList<QIfStandardItem> list;
        for (int i=0; i<100; i++) {
            QIfStandardItem item;
            item.setId(name + QLatin1String(" ") + QString::number(i));
            QVariantMap map;
            map.insert("type", name);
            map.insert("index", i);
            item.setData(map);
            list.append(item);
        }
        return list;
    }

    void initialize() override
    {
        emit availableContentTypesChanged(m_lists.keys());
        emit initializationDone();
    }

    void registerInstance(const QUuid &identifier) override
    {
        Q_UNUSED(identifier)
    }

    void unregisterInstance(const QUuid &identifier) override
    {
        Q_UNUSED(identifier)
    }

    void setContentType(const QUuid &identifier, const QString &contentType) override
    {
        Q_UNUSED(identifier)
        m_contentType = contentType;
        if (contentType == "levelTwo" || contentType == "levelThree")
            emit canGoBackChanged(identifier, true);
        emit queryIdentifiersChanged(identifier, identifiersFromItem<QIfStandardItem>());
        emit contentTypeChanged(identifier, contentType);
    }

    void setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms) override
    {
        Q_UNUSED(identifier)
        m_filterTerm = term;
        m_orderTerms = orderTerms;
    }

    virtual void fetchData(const QUuid &identifier, int start, int count) override
    {
        emit supportedCapabilitiesChanged(identifier, m_caps);

        if (!m_lists.contains(m_contentType))
            return;

        QList<QIfStandardItem> list = m_lists.value(m_contentType);

        if (m_caps.testFlag(QtInterfaceFrameworkModule::SupportsFiltering) && m_filterTerm) {
            if (m_filterTerm->type() != QIfAbstractQueryTerm::FilterTerm) {
                qWarning("Only filtering is supported");
                return;
            }

            if (m_orderTerms.count() > 1) {
                qWarning("Only one order term is supported");
                return;
            }

            QIfFilterTerm *filterTerm = static_cast<QIfFilterTerm*>(m_filterTerm);

            if (filterTerm->operatorType() == QIfFilterTerm::EqualsCaseInsensitive) {
                qWarning("case insensitive comparison is not supported");
                return;
            }

            const QMetaObject mo = QIfStandardItem::staticMetaObject;
            int mpi = mo.indexOfProperty(filterTerm->propertyName().toUtf8());
            Q_ASSERT(mpi != -1);

            QMetaProperty mp = mo.property(mpi);

            QList<QIfStandardItem> resultList;
            for (const QIfStandardItem &item : std::as_const(list)) {
                QVariant value = mp.readOnGadget(&item);
                if (value.canConvert(filterTerm->value().metaType()))
                    value.convert(filterTerm->value().metaType());

                bool filterCondition = (filterTerm->operatorType() == QIfFilterTerm::Equals && value == filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIfFilterTerm::GreaterThan && value > filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIfFilterTerm::GreaterEquals && value >= filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIfFilterTerm::LowerThan && value < filterTerm->value()) ||
                                       (filterTerm->operatorType() == QIfFilterTerm::LowerEquals && value <= filterTerm->value());

                if (filterTerm->isNegated())
                    filterCondition = !filterCondition;

                if (filterCondition)
                    resultList.append(item);
            }

            list = resultList;
        }

        if (m_caps.testFlag(QtInterfaceFrameworkModule::SupportsSorting) && m_orderTerms.count()) {
            if (m_orderTerms.count() > 1) {
                qWarning("Only one order term is supported");
                return;
            }

            const QMetaObject mo = QIfStandardItem::staticMetaObject;
            int mpi = mo.indexOfProperty(m_orderTerms.first().propertyName().toUtf8());
            Q_ASSERT(mpi != -1);

            QMetaProperty mp = mo.property(mpi);

            std::sort(list.begin(), list.end(), [mp, this](const QIfStandardItem &s1, const QIfStandardItem &s2) {
                QVariant var1 = mp.readOnGadget(&s1);
                QVariant var2 = mp.readOnGadget(&s2);
                if (var1.canConvert(QMetaType::fromType<int>()) && var1.canConvert(QMetaType::fromType<int>())) {
                    var1.convert(QMetaType::fromType<int>());
                    var2.convert(QMetaType::fromType<int>());
                }

                bool lower = var1 > var2;
                if (m_orderTerms.first().isAscending())
                    lower = !lower;

                return lower;
            });
        }

        if (m_caps.testFlag(QtInterfaceFrameworkModule::SupportsGetSize))
            emit countChanged(identifier, list.count());

        QVariantList requestedItems;

        int size = qMin(start + count, list.count());
        for (int i = start; i < size; i++)
            requestedItems.append(QVariant::fromValue(list.at(i)));

        emit dataFetched(identifier, requestedItems, start, start + count < list.count());

        //All but the first item can go forward
        if (m_contentType == "levelOne" || m_contentType == "levelTwo")
            emit canGoForwardChanged(identifier, QVector<bool>(requestedItems.count() - 1, true), start + 1);
    }

    virtual QIfPendingReply<QString> goBack(const QUuid &identifier) override
    {
        Q_UNUSED(identifier)

        if (m_contentType == "levelThree")
            return QIfPendingReply<QString>("levelTwo");
        else if (m_contentType == "levelTwo")
            return QIfPendingReply<QString>("levelOne");

        return QIfPendingReply<QString>::createFailedReply();
    }

    virtual QIfPendingReply<QString> goForward(const QUuid &identifier, int index) override
    {
        Q_UNUSED(identifier)
        Q_UNUSED(index)

        if (m_contentType == "levelOne")
            return QIfPendingReply<QString>("levelTwo");
        else if (m_contentType == "levelTwo")
            return QIfPendingReply<QString>("levelThree");

        return QIfPendingReply<QString>::createFailedReply();
    }

    virtual QIfPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &var) override
    {
        const QIfStandardItem *item = qtif_gadgetFromVariant<QIfStandardItem>(this, var);
        if (!item)
            return QIfPendingReply<void>::createFailedReply();

        QList<QIfStandardItem> list = m_lists.value(m_contentType);

        list.insert(index, *item);
        QVariantList variantList = { QVariant::fromValue(*item) };

        m_lists.insert(m_contentType, list);

        emit dataChanged(identifier, variantList, index, 0);

        QIfPendingReply<void> reply;
        reply.setSuccess();
        return reply;
    }

    virtual QIfPendingReply<void> remove(const QUuid &identifier, int index) override
    {
        QList<QIfStandardItem> list = m_lists.value(m_contentType);

        list.removeAt(index);
        m_lists.insert(m_contentType, list);

        emit dataChanged(identifier, QVariantList(), index, 1);

        QIfPendingReply<void> reply;
        reply.setSuccess();
        return reply;
    }

    virtual QIfPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override
    {
        QList<QIfStandardItem> list = m_lists.value(m_contentType);

        int min = qMin(currentIndex, newIndex);
        int max = qMax(currentIndex, newIndex);

        list.move(currentIndex, newIndex);
        QVariantList variantLIst;
        for (int i = min; i <= max; i++)
            variantLIst.append(QVariant::fromValue(list.at(i)));

        m_lists.insert(m_contentType, list);

        emit dataChanged(identifier, variantLIst, min, max - min + 1);

        QIfPendingReply<void> reply;
        reply.setSuccess();
        return reply;
    }

    virtual QIfPendingReply<int> indexOf(const QUuid &identifier, const QVariant &var) override
    {
        Q_UNUSED(identifier)
        const QIfStandardItem *item = qtif_gadgetFromVariant<QIfStandardItem>(this, var);
        if (!item)
            return QIfPendingReply<int>::createFailedReply();

        QList<QIfStandardItem> list = m_lists.value(m_contentType);

        QIfPendingReply<int> reply;
        reply.setSuccess(list.indexOf(*item));
        return reply;
    }

private:
    QHash<QString, QList<QIfStandardItem>> m_lists;
    QtInterfaceFrameworkModule::ModelCapabilities m_caps;
    QString m_contentType;
    QIfAbstractQueryTerm *m_filterTerm = nullptr;
    QList<QIfOrderTerm> m_orderTerms;
};

class TestServiceObject : public QIfServiceObject
{
    Q_OBJECT

public:
    explicit TestServiceObject(QObject *parent = nullptr) :
        QIfServiceObject(parent)
    {
        m_backend = new TestBackend;
        m_interfaces << QIfFilterAndBrowseModel_iid;
    }

    QStringList interfaces() const override { return m_interfaces; }
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override
    {
        if (interface == QIfFilterAndBrowseModel_iid)
            return testBackend();
        else
            return 0;
    }

    TestBackend *testBackend() const
    {
        return m_backend;
    }

private:
    QStringList m_interfaces;
    TestBackend *m_backend;
};

void verifyQml(QQmlEngine *engine, const QByteArray &qml)
{
    QQmlComponent component(engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));
}

class tst_QIfFilterAndBrowseModel : public QObject
{
    Q_OBJECT

public:
    tst_QIfFilterAndBrowseModel();

private Q_SLOTS:
    void cleanup();

    void testWithoutBackend();
    void testClearServiceObject();

    //TODO it would be great if we can have a shared test class as most of these tests are also
    //     part of the qifpagingmodel autotest
    void testBasic_qml();
    void testGetAt();
    void testFetchMore_data();
    void testFetchMore();
    void testDataChangedMode();
    void testReload();
    void testDataChangedMode_jump();
    void testNavigation_data();
    void testNavigation();
    void testFilter_data();
    void testFilter();
    void testEditing();
    void testIndexOf_qml();
    void testInputErrors();
    void testMissingCapabilities();

private:
    QIfServiceManager *manager;
};

tst_QIfFilterAndBrowseModel::tst_QIfFilterAndBrowseModel()\
    : manager(QIfServiceManager::instance())
{
}

void tst_QIfFilterAndBrowseModel::cleanup()
{
    manager->unloadAllBackends();
}

void tst_QIfFilterAndBrowseModel::testWithoutBackend()
{
    QIfFilterAndBrowseModel model;

    QCOMPARE(model.discoveryMode(), QIfAbstractFeature::NoAutoDiscovery);
    QTest::ignoreMessage(QtWarningMsg, "No backend connected");
    QVERIFY(!model.canGoBack());
    model.goBack();

    QVERIFY(!model.canGoForward(0));
    QVERIFY(!model.goForward(0, QIfFilterAndBrowseModel::InModelNavigation));

    QTest::ignoreMessage(QtWarningMsg, "Can't move items without a connected backend");
    model.move(0, 0);

    QTest::ignoreMessage(QtWarningMsg, "Can't remove items without a connected backend");
    model.remove(0);

    QTest::ignoreMessage(QtWarningMsg, "Can't insert items without a connected backend");
    model.insert(0, QVariant::fromValue(QIfStandardItem()));

    QTest::ignoreMessage(QtWarningMsg, "Can't get the index without a connected backend");
    auto reply = model.indexOf(QVariant::fromValue(QIfStandardItem()));
    QVERIFY(reply.isResultAvailable());
    QVERIFY(!reply.isSuccessful());

    QVERIFY(model.availableContentTypes().isEmpty());
}

void tst_QIfFilterAndBrowseModel::testClearServiceObject()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);

    QIfFilterAndBrowseModel defaultModel;
    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);

    model.setLoadingType(QIfFilterAndBrowseModel::DataChanged);
    model.setChunkSize(20);
    model.setContentType("simple");
    model.setFetchMoreThreshold(20);

    QSignalSpy chunkSizeSpy(&model, &QIfFilterAndBrowseModel::chunkSizeChanged);
    QVERIFY(model.chunkSize() != defaultModel.chunkSize());
    QSignalSpy contentTypeSpy(&model, &QIfFilterAndBrowseModel::contentTypeChanged);
    QVERIFY(model.contentType() != defaultModel.contentType());
    QSignalSpy thresholdSpy(&model, &QIfFilterAndBrowseModel::fetchMoreThresholdChanged);
    QVERIFY(model.fetchMoreThreshold() != defaultModel.fetchMoreThreshold());
    QSignalSpy availableContentTypeSpy(&model, &QIfFilterAndBrowseModel::availableContentTypesChanged);
    QVERIFY(model.availableContentTypes() != defaultModel.availableContentTypes());
    QSignalSpy capabilitiesSpy(&model, &QIfFilterAndBrowseModel::capabilitiesChanged);
    QVERIFY(model.capabilities() != defaultModel.capabilities());
    QSignalSpy loadingTypeSpy(&model, &QIfFilterAndBrowseModel::loadingTypeChanged);
    QVERIFY(model.loadingType() != defaultModel.loadingType());
    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QVERIFY(model.rowCount() != defaultModel.rowCount());

    QVERIFY(model.setServiceObject(nullptr));


    QVERIFY(model.chunkSize() == defaultModel.chunkSize());
    QCOMPARE(chunkSizeSpy.count(), 1);
    QVERIFY(model.contentType() == defaultModel.contentType());
    QCOMPARE(contentTypeSpy.count(), 1);
    QVERIFY(model.fetchMoreThreshold() == defaultModel.fetchMoreThreshold());
    QCOMPARE(thresholdSpy.count(), 1);
    QVERIFY(model.availableContentTypes() == defaultModel.availableContentTypes());
    QCOMPARE(availableContentTypeSpy.count(), 1);
    QVERIFY(model.capabilities() == defaultModel.capabilities());
    QCOMPARE(capabilitiesSpy.count(), 1);
    QVERIFY(model.loadingType() == defaultModel.loadingType());
    QCOMPARE(loadingTypeSpy.count(), 1);
    QVERIFY(model.rowCount() == defaultModel.rowCount());
    QCOMPARE(resetSpy.count(), 1);
}

void tst_QIfFilterAndBrowseModel::testBasic_qml()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("testBackend", service);
    verifyQml(&engine, "import QtQuick; import QtInterfaceFramework; FilterAndBrowseModel{}");
    verifyQml(&engine, "import QtQuick; import QtInterfaceFramework; FilterAndBrowseModel{ \
                            serviceObject: testBackend \n\
                        }");
    verifyQml(&engine, "import QtQuick; import QtInterfaceFramework; FilterAndBrowseModel{ \
                            serviceObject: testBackend \n\
                            contentType: 'simple' \n\
                        }");
}

void tst_QIfFilterAndBrowseModel::testGetAt()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QIfStandardItem item = model.at<QIfStandardItem>(0);
    QCOMPARE(item.id(), QLatin1String("simple 0"));

    QVariant var = model.get(0);
    QCOMPARE(var.value<QIfStandardItem>().id(), item.id());
}

void tst_QIfFilterAndBrowseModel::testFetchMore_data()
{
    QTest::addColumn<int>("chunkSize");
    QTest::addColumn<int>("fetchMoreThreshold");
    QTest::newRow("normal") << -1 << -1;
    QTest::newRow("custom chunkSize") << 20 << -1;
    QTest::newRow("custom fetchMoreThreshold") << -1 << 2;
}

void tst_QIfFilterAndBrowseModel::testFetchMore()
{
    QFETCH(int, chunkSize);
    QFETCH(int, fetchMoreThreshold);

    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);

    if (chunkSize != -1) {
        QSignalSpy chunkSizeChangedSpy(&model, SIGNAL(chunkSizeChanged(int)));
        model.setChunkSize(model.chunkSize());
        QVERIFY(!chunkSizeChangedSpy.count());

        model.setChunkSize(chunkSize);
        QCOMPARE(model.chunkSize(), chunkSize);
        QVERIFY(chunkSizeChangedSpy.count());
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

    // Set the contentType
    QSignalSpy contentTypeSpy(&model, SIGNAL(contentTypeChanged(QString)));
    QVERIFY(model.availableContentTypes().contains("simple"));
    model.setContentType("simple");
    QCOMPARE(model.contentType(), QString("simple"));
    QVERIFY(contentTypeSpy.count());
    contentTypeSpy.clear();

    // Change it again to make sure to not get the change signal
    model.setContentType("simple");
    QVERIFY(!contentTypeSpy.count());

    QVERIFY(model.serviceObject());
    QCOMPARE(model.loadingType(), QIfFilterAndBrowseModel::FetchMore);

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

void tst_QIfFilterAndBrowseModel::testDataChangedMode()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);

    QVERIFY(model.availableContentTypes().contains("simple"));
    model.setContentType("simple");
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

void tst_QIfFilterAndBrowseModel::testReload()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);

    QVERIFY(model.serviceObject());

    QVERIFY(model.availableContentTypes().contains("simple"));
    QSignalSpy countChangedSpy(&model, SIGNAL(countChanged()));
    model.setContentType("simple");
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

void tst_QIfFilterAndBrowseModel::testDataChangedMode_jump()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsGetSize);
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);

    QVERIFY(model.availableContentTypes().contains("simple"));
    model.setContentType("simple");
    QVERIFY(model.serviceObject());

    QSignalSpy loadingTypeChangedSpy(&model, SIGNAL(loadingTypeChanged(QIfPagingModel::LoadingType)));
    model.setLoadingType(model.loadingType());
    QVERIFY(!loadingTypeChangedSpy.count());

    model.setLoadingType(QIfPagingModel::DataChanged);
    QCOMPARE(model.loadingType(), QIfFilterAndBrowseModel::DataChanged);
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

void tst_QIfFilterAndBrowseModel::testNavigation_data()
{
    QTest::addColumn<QIfFilterAndBrowseModel::NavigationType>("navigationType");
    QTest::newRow("InModelNavigation") << QIfFilterAndBrowseModel::InModelNavigation;
    QTest::newRow("OutOfModelNavigation") << QIfFilterAndBrowseModel::OutOfModelNavigation;
}

void tst_QIfFilterAndBrowseModel::testNavigation()
{
    QFETCH(QIfFilterAndBrowseModel::NavigationType, navigationType);

    QStack<QIfFilterAndBrowseModel*> modelStack;
    TestServiceObject *service = new TestServiceObject();
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::SupportsStatelessNavigation);
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeNavigationData();

    QIfFilterAndBrowseModel *model = new QIfFilterAndBrowseModel();
    model->setServiceObject(service);

    QVERIFY(model->availableContentTypes().contains("levelOne"));
    model->setContentType("levelOne");
    QVERIFY(model->serviceObject());

    QCOMPARE(model->rowCount(), model->chunkSize());
    QCOMPARE(model->at<QIfStandardItem>(1).id(), QLatin1String("levelOne ") + QString::number(1));
    QCOMPARE(model->data(model->index(1), QIfFilterAndBrowseModel::CanGoForwardRole).toBool(), true);

    // Check that navigating deeper on the first item is not possible
    QVERIFY(!model->canGoForward(0));

    // Navigate Deeper
    QVERIFY(model->canGoForward(1));
    QIfFilterAndBrowseModel *newModel = model->goForward(1, navigationType);
    if (newModel) {
        modelStack.push(model);
        model = newModel;
    }
    if (navigationType == QIfFilterAndBrowseModel::InModelNavigation)
        QVERIFY(model->canGoBack());
    QCOMPARE(model->at<QIfStandardItem>(1).id(), QLatin1String("levelTwo ") + QString::number(1));
    QCOMPARE(model->data(model->index(1), QIfFilterAndBrowseModel::CanGoForwardRole).toBool(), true);

    // Check that navigating deeper on the first item is not possible
    QVERIFY(!model->canGoForward(0));

    // Navigate to the last level
    QVERIFY(model->canGoForward(1));
    newModel = model->goForward(1, navigationType);
    if (newModel) {
        modelStack.push(model);
        model = newModel;
    }
    if (navigationType == QIfFilterAndBrowseModel::InModelNavigation)
        QVERIFY(model->canGoBack());
    QCOMPARE(model->at<QIfStandardItem>(1).id(), QLatin1String("levelThree ") + QString::number(1));
    QCOMPARE(model->data(model->index(1), QIfFilterAndBrowseModel::CanGoForwardRole).toBool(), false);

    // Check that we can't got forward anymore
    QVERIFY(!model->canGoForward(0));
    QVERIFY(!model->canGoForward(1));
    QTest::ignoreMessage(QtWarningMsg, "Can't go forward anymore");
    QVERIFY(!model->goForward(1, navigationType));

    if (navigationType == QIfFilterAndBrowseModel::InModelNavigation) {
        // Navigate back to previous level
        QVERIFY(model->canGoBack());

        model->goBack();
        QCOMPARE(model->at<QIfStandardItem>(1).id(), QLatin1String("levelTwo ") + QString::number(1));

        QVERIFY(model->canGoBack());

        model->goBack();
        QCOMPARE(model->at<QIfStandardItem>(1).id(), QLatin1String("levelOne ") + QString::number(1));

        QVERIFY(!model->canGoBack());
        QTest::ignoreMessage(QtWarningMsg, "Can't go backward anymore");
        model->goBack();
    }

    delete model;
    qDeleteAll(modelStack);
}

// If more complex queries are added here you also need to make sure the backend can handle it.
void tst_QIfFilterAndBrowseModel::testFilter_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<QString>("expectedContent");
    QTest::newRow("filter") << QString("id>10") << QString("11");
    //TODO Enable once only sorting is possible
    //QTest::newRow("filter") << QString("[\id]") << QString("99");

    QTest::newRow("order") << QString("id>=0[\\id]") << QString("99");
    QTest::newRow("filter + order") << QString("id<50[\\id]") << QString("49");
}

void tst_QIfFilterAndBrowseModel::testFilter()
{
    QFETCH(QString, query);
    QFETCH(QString, expectedContent);

    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::ModelCapabilities( QtInterfaceFrameworkModule::SupportsFiltering |
                                                                                QtInterfaceFrameworkModule::SupportsSorting));
    service->testBackend()->initializeFilterData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("filter");

    // Check that we have a unfiltered content before setting the filter
    QCOMPARE(model.at<QIfStandardItem>(0).id(), QString::number(0));

    // Set the query
    QSignalSpy queryChangedSpy(&model, SIGNAL(queryChanged(QString)));
    model.setQuery(model.query());
    QVERIFY(!queryChangedSpy.count());

    model.setQuery(query);
    QCOMPARE(model.query(), query);
    QVERIFY(queryChangedSpy.count());

    // Check the filtered content
    QCOMPARE(model.at<QIfStandardItem>(0).id(), expectedContent);

    // Reset to unfiltered
    model.setQuery(QString());
    QCOMPARE(model.query(), QString());

    // Check that we have a unfiltered content after removing the filter
    QCOMPARE(model.at<QIfStandardItem>(0).id(), QString::number(0));
}

void tst_QIfFilterAndBrowseModel::testEditing()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::ModelCapabilities( QtInterfaceFrameworkModule::SupportsInsert |
                                                                                QtInterfaceFrameworkModule::SupportsRemove |
                                                                                QtInterfaceFrameworkModule::SupportsMove));
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QCOMPARE(model.at<QIfStandardItem>(0).id(), QLatin1String("simple 0"));

    QIfStandardItem newItem;
    newItem.setId(QLatin1String("testItem"));

    // Add a new Item
    QSignalSpy insertSpy(&model, SIGNAL(rowsInserted(const QModelIndex &, int , int )));
    model.insert(0, QVariant::fromValue(newItem));
    QVERIFY(insertSpy.count());
    QCOMPARE(insertSpy.at(0).at(1).toInt(), 0);
    QCOMPARE(insertSpy.at(0).at(2).toInt(), 0);

    QCOMPARE(model.at<QIfStandardItem>(0).id(), newItem.id());

    // Move the item to a new location
    QSignalSpy moveSpy(&model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
    int newIndex = 10;
    model.move(0, newIndex);
    QVERIFY(moveSpy.count());
    QCOMPARE(moveSpy.at(0).at(0).toModelIndex().row(), 0);
    QCOMPARE(moveSpy.at(0).at(1).toModelIndex().row(), newIndex);

    QCOMPARE(model.at<QIfStandardItem>(newIndex).id(), newItem.id());

    // Remove the item again
    QSignalSpy removedSpy(&model, SIGNAL(rowsRemoved(const QModelIndex &, int , int )));
    model.remove(newIndex);
    QVERIFY(removedSpy.count());
    QCOMPARE(removedSpy.at(0).at(1).toInt(), newIndex);
    QCOMPARE(removedSpy.at(0).at(2).toInt(), newIndex);

    QCOMPARE(model.at<QIfStandardItem>(newIndex).id(), QLatin1String("simple 10"));
}

void tst_QIfFilterAndBrowseModel::testIndexOf_qml()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/testdata/listview.qml")));
    QObject *obj = component.create();

    QVERIFY2(obj, qPrintable(component.errorString()));
    QSignalSpy indexOfSpy(obj, SIGNAL(indexOfItem(int)));

    obj->setProperty("model", QVariant::fromValue(&model));

    QIfStandardItem item = model.at<QIfStandardItem>(25);
    QCOMPARE(item.id(), QLatin1String("simple 25"));

    QVERIFY(QMetaObject::invokeMethod(obj, "callIndexOf", Q_ARG(QVariant, QVariant::fromValue(item))));

    // the result signal is connected using a queued connection internally
    qApp->processEvents();

    QVERIFY(indexOfSpy.count());
    QCOMPARE(indexOfSpy.at(0).at(0).toInt(), 25);
}

void tst_QIfFilterAndBrowseModel::testInputErrors()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeSimpleData();
    service->testBackend()->setCapabilities(QtInterfaceFrameworkModule::ModelCapabilities( QtInterfaceFrameworkModule::SupportsFiltering |
                                                                                QtInterfaceFrameworkModule::SupportsSorting));

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("simple");

    QCOMPARE(model.rowCount(model.index(0,0)), 0);
    QCOMPARE(model.data(QModelIndex(), Qt::DisplayRole), QVariant());
    QCOMPARE(model.data(model.index(0,0), Qt::DecorationRole), QVariant());
    QVERIFY(!model.canFetchMore(model.index(0,0)));

    QSignalSpy countChanged(&model, SIGNAL(countChanged()));
    model.fetchMore(model.index(0,0));
    QVERIFY(!countChanged.count());

    // Invalid query
    QTest::ignoreMessage(QtWarningMsg, "Got end of file but expected on of the following types:\n     integer\n     float\nid>\n  ^");
    model.setQuery(QString("id>"));
}

void tst_QIfFilterAndBrowseModel::testMissingCapabilities()
{
    TestServiceObject *service = new TestServiceObject();
    manager->registerService(service, service->interfaces());
    service->testBackend()->initializeNavigationData();

    QIfFilterAndBrowseModel model;
    model.setServiceObject(service);
    model.setContentType("levelOne");

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support the DataChanged loading type. This call will have no effect");
    model.setLoadingType(QIfFilterAndBrowseModel::DataChanged);
    QCOMPARE(model.loadingType(), QIfFilterAndBrowseModel::FetchMore);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support the OutOfModelNavigation");
    QVERIFY(!model.goForward(1, QIfFilterAndBrowseModel::OutOfModelNavigation));

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support moving of items");
    model.move(0, 0);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support removing of items");
    model.remove(0);

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support inserting items");
    model.insert(0, QVariant::fromValue(QIfStandardItem()));

    QTest::ignoreMessage(QtWarningMsg, "The backend doesn't support filtering or sorting. Changing the query will have no effect");
    model.setQuery(QString("id>10"));
}

QTEST_MAIN(tst_QIfFilterAndBrowseModel)

#include "tst_qiffilterandbrowsemodel.moc"

