/* Polku
 *
 * Kuvaus:
 *  Ohjelma toteuttaa Polku-älypelin. Pelilauta on kooltaan ROWS x COLUMNS
 * (5x4), ja joka kierroksella pelaaja antaa lähtö- ja kohdepisteet (yhteensä
 * neljä lukua), minkä jälkeen kyseinen nappula siirretään kohdepisteeseen
 * (mikäli mahdollista). Ohjelma tarkistaa, voidaanko käyttäjän antama siirto
 * tehdä.
 *  Aluksi kaikki vihreät nappulat ovat ylimmällä rivillä ja kaikki punaiset
 * nappulat alimmalla. Peli päättyy, kun kaikki vihreät nappulat on siirretty
 * alimmalle riville ja kaikki punaiset ylimmälle. Nappulat eivät saa hyppiä
 * toistensa yli.
 *  Pelilauta piirretään niin, että 'G' tarkoittaa vihreää (green) nappulaa,
 * 'R' punaista (red) nappulaa, 'o' tyhjää kohtaa ja välilyönti kohtaa, joka
 * ei ole käytössä.
 *
 * Ohjelman kirjoittaja
 * Nimi: Teemu Ruonakoski
 * Opiskelijanumero: 152116224
 * Käyttäjätunnus: tcteru
 * E-Mail: teemu.ruonakoski@tuni.fi
 * Palautteen kieli: fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 *
 * */
#include "gameboard.hh"
#include <iostream>

/* Pohjustuksena projektille käytetään viikkoharkoissa käytyä
 * esimerkkiä, sekä plussasta tehtävänannosta löytyvää
 * esimerkkirakennetta. Ohjelmalle on luotu yksikkötestaus,
 * joka kuuluu projektin vaatimuksiin.
 * */

using namespace std;

// Constant outputs
const string INPUT_TEXT = "Enter start point (x, y) and destination point (x, y), or q to quit: ";
const string INVALID_POINT = "Invalid start/destination point.";
const string IDENTICAL_POINTS = "Given points are the same.";   // Actually useless
const string CANNOT_MOVE = "There is no path between start point and destination point.";
const string GAME_OVER = "Game over. Congratulations!";
const string MOVES_MADE = " move(s) made.";


// Enable the user to play the game.
// Until the game is over, read user commands and move the given buttons,
// if possible.
// After each move, print the game board again.
// As the game is over or the user gives the quit command,
// print the number of moves made.
void playGame(GameBoard& gb)
{
    Location p1 = {0, 0};
    Location p2 = {0, 0};
    string command = "";
    unsigned int total_moves = 0;
    gb.print();

    while (!gb.isGameOver())
    {
        cout << INPUT_TEXT;
        cin >> command;
        if (command == "q" || command == "Q")
        {
            cout << total_moves << MOVES_MADE << endl;
            return;
        }
        try
        {
            p1.x = stoi(command) - 1;
            cin >> command;
            p1.y = stoi(command) - 1;
            cin >> command;
            p2.x = stoi(command) - 1;
            cin >> command;
            p2.y = stoi(command) - 1;
        }
        catch (invalid_argument const& ex)
        {
            cout << "Invalid argument: " << ex.what() << endl;
            return;
        }

        // Check validity of the locations
        if (!gb.isValidLocations(p1, p2))
        {
            cout << INVALID_POINT << endl;
            continue;
        }

        // Try to move
        if(gb.move(p1, p2))
        {
            ++total_moves;
            gb.print();
        }
        else
        {
            cout << CANNOT_MOVE << endl;
        }
    }
    cout << GAME_OVER << endl;
    cout << total_moves << MOVES_MADE << endl;
}


int main()
{
    GameBoard board;
    board.initialFill();
    playGame(board);
    return 0;
}
