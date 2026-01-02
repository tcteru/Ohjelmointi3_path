/*
 * Tämä on gameboard luokan otsikkotiedosto,
 * johon on määritelty luokan rakenne.
 * Ohjelman kirjoittaja
 * Nimi: Teemu Ruonakoski
 * Opiskelijanumero: 152116224
 * Käyttäjätunnus: tcteru
 * E-Mail: teemu.ruonakoski@tuni.fi
 * */
#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <vector>
#include <memory>
#include <iostream>
#include <set>
#include <string>
#include <queue>
#include <stdexcept>
#include <cstddef>
#include <map>
#include <algorithm>

using namespace std;

// Gameboard size
const unsigned int ROWS = 5;
const unsigned int COLUMNS = 4;

struct Location
{
    unsigned int x;
    unsigned int y;
    bool operator==(const Location& rhs) const
    {
        return x == rhs.x and y == rhs.y;
    }
    bool operator!=(const Location& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator<(const Location& rhs) const
    {
        return (x < rhs.x) or (x == rhs.x and y < rhs.y);
    }
};

struct Piece
{
    string type;
    Piece() = default;
    Piece(const string& t) : type(t) {}
};

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    virtual ~GameBoard();

    // Fill the gameboard initially: all green buttons are put on the top row
    // and all red buttons on the bottom rows, other locations are nullptrs.
    void initialFill();

    // Print the gameboard.
    void print() const;

    // Return true, if the game is over, i.e. if all red buttons are on the
    // top row and all green buttons are on the bottom row.
    bool isGameOver() const;

    // Check the validity of given locations. To be valid, a location must be
    // inside the gameboard, it must not be an unused location, and the start
    // must have a button, and the destination must not have a button.
    // (Implicitly also check that p1 and p2 are not the same points.)
    // Returns true, if both the given locations are valid.
    bool isValidLocations(const Location& start,
                          const Location& destination) const;

    // If possible, move a button from the start location to the destination
    // one. A move may consists of horizontal and vertical steps.
    // Return true, if move is possible, i.e. if there is a path between
    // the given locations.
    bool move(const Location& start, const Location& destination);

    // Lisätään uudet funktiot

    bool moveInRowOrCol(const Location& start,
                        const Location& destination) const;
    bool isInvalidPosition(const Location& pos) const;


private:

    vector<vector<unique_ptr<Piece>>> playing_area_;
    set<Location> invalid_positions_;  
};

#endif // GAMEBOARD_HH
