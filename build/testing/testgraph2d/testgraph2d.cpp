#include <QtTest>
#include <QCoreApplication>
#include <graph2d.h>

class testgraph2d : public QObject
{
    Q_OBJECT

public:
    testgraph2d();
    ~testgraph2d();

private slots:
    // https://doc.qt.io/qt-6/qtest-overview.html
    void initTestCase();  // will be called before the first test function is executed
    void cleanupTestCase();  // will be called after the last test function was executed

    void init();  // will be called before each test function is executed
    void cleanup();  // will be called after every test function

    void initially_curve_map_empty();
    void add_curve_increment();
    void remove_curve_deincrement();
    void curve_id_starts_with_zero();

private:
    Graph2D<int>* _instance {nullptr};

};

testgraph2d::testgraph2d()
{

}

testgraph2d::~testgraph2d()
{

}

void testgraph2d::init()
{
    _instance = new Graph2D<int>();
    //_instance->hide();
}

void testgraph2d::cleanup()
{
    Q_ASSERT(_instance != nullptr);
    delete _instance;
    _instance = nullptr;
}

void testgraph2d::initTestCase()
{

}

void testgraph2d::cleanupTestCase()
{

}

void testgraph2d::initially_curve_map_empty()
{
    QCOMPARE(_instance->_curves.count(), 0);
}

void testgraph2d::add_curve_increment()
{
    _instance->addCurve(QVector<int>{ });
    QCOMPARE(_instance->_curves.count(), 1);
}

void testgraph2d::remove_curve_deincrement()
{
    auto const id {_instance->addCurve(QVector<int>{ })};
    _instance->removeCurve(id);
    QCOMPARE(_instance->_curves.count(), 0);
}

void testgraph2d::curve_id_starts_with_zero()
{
    auto const id {_instance->addCurve(QVector<int>{ })};
    QCOMPARE(id, 0);
}

QTEST_MAIN(testgraph2d)

#include "testgraph2d.moc"
