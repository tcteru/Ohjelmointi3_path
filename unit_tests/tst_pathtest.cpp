/*
 * Tämä on gameboard luokan yksikkötestaus.
 * Tänne on toteutettu plussasta löytyvän
 * mallin mukaan tälle luokalle sopiva
 * yksikkötestaus.
 * Ohjelman kirjoittaja
 * Nimi: Teemu Ruonakoski
 * Opiskelijanumero: 152116224
 * Käyttäjätunnus: tcteru
 * E-Mail: teemu.ruonakoski@tuni.fi
 * */
#include <QtTest>
#include "../gameboard.hh"
#include <memory>

using namespace std;

class GameBoardTest : public QObject
{
    Q_OBJECT

public:
    GameBoardTest();
    ~GameBoardTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    void testSuccessfulMove();
    void testFailedMove();
    void testIsValidLocations();
    void testIsValidLocations_data();

private:
    shared_ptr<GameBoard> testBoard_;
};

GameBoardTest::GameBoardTest() {}
GameBoardTest::~GameBoardTest() {}

void GameBoardTest::initTestCase() {}
void GameBoardTest::cleanupTestCase() {}

void GameBoardTest::init()
{
    testBoard_ = make_shared<GameBoard>();
    testBoard_->initialFill();
}

void GameBoardTest::testSuccessfulMove()
{
    Location start = {1, 0};
    Location destination = {1, 2};
    QVERIFY2(testBoard_->move(start, destination), "Move should succeed when path is clear");
}

void GameBoardTest::testFailedMove()
{
    Location start = {0, 0};
    Location destination = {2, 1}; // Invalid location
    QVERIFY2(!testBoard_->move(start, destination), "Move should fail due to an invalid destination");
}

void GameBoardTest::testIsValidLocations()
{
    QFETCH(Location, start);
    QFETCH(Location, destination);
    QFETCH(bool, expectedResult);
    QVERIFY2(testBoard_->isValidLocations(start, destination) == expectedResult, "Validation failed");
}

void GameBoardTest::testIsValidLocations_data()
{
    QTest::addColumn<Location>("start");
    QTest::addColumn<Location>("destination");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid move") << Location{1, 0} << Location{1, 2} << true;
    QTest::newRow("Move to invalid position") << Location{0, 0} << Location{2, 1} << false;
    QTest::newRow("Start from invalid position") << Location{3, 3} << Location{1, 2} << false;
    QTest::newRow("Start from empty location") << Location{1, 1} << Location{2, 2} << false;
    QTest::newRow("Move to occupied location") << Location{0, 0} << Location{1, 4} << false;
}

QTEST_APPLESS_MAIN(GameBoardTest)

#include "tst_pathtest.moc"
