/*
 * Tämä on gameboard luokan lähdetiedosto,
 * joka toteuttaa luokan toiminnan.
 * Ohjelman kirjoittaja
 * Nimi: Teemu Ruonakoski
 * Opiskelijanumero: 152116224
 * Käyttäjätunnus: tcteru
 * E-Mail: teemu.ruonakoski@tuni.fi
 * */
#include "gameboard.hh"

GameBoard::GameBoard() :
    invalid_positions_{{0,1}, {2,1}, {3,1}, {0,2},
                       {3,2}, {0,3}, {2,3}, {3,3}}
{
    playing_area_ = std::vector<std::vector<unique_ptr<Piece>>>(ROWS);
    for (unsigned int row =0; row < ROWS; row++)
    {
        for (unsigned int col = 0; col < COLUMNS; col++)
        {
            playing_area_[row].push_back(std::make_unique<Piece>());
        }
    }
}

GameBoard::~GameBoard() {}

void GameBoard::initialFill()
{
    //Sijoitetaan nappulat
    for (unsigned int col = 0; col < COLUMNS; col++)
    {
        playing_area_[0][col] = make_unique<Piece>("G");
        playing_area_[ROWS -1][col] = make_unique<Piece>("R");
    }
    //Lisätään tyhjät paikat
    for (unsigned int row =1; row < ROWS -1; row++)
    {
        for (unsigned int col =0; col < COLUMNS; col++)
        {
            playing_area_[row][col] = nullptr;
        }
    }
}

void GameBoard::print() const
{
    cout << "===============" << endl;
    cout << "|   | 1 2 3 4 |" << endl;
    cout << "---------------" << endl;
    for (unsigned int row = 0; row < ROWS; row++)
    {
        cout << "| " << row + 1 << " | ";
        for (unsigned int col = 0; col < COLUMNS; col++)
        {
            if (playing_area_[row][col] != nullptr)
            {
                cout << playing_area_[row][col]->type << " ";
            }
            else
            {
                if (invalid_positions_.count({col, row}))
                {
                    cout << "  ";
                }
                else
                {
                    cout << "o ";
                }
            }
        }
        cout << "|" << endl;
    }
    cout << "===============" << endl;
}

bool GameBoard::isGameOver() const
{
    //Ylärivi
    for (unsigned int col = 0; col < COLUMNS; col++)
    {
        if (playing_area_[0][col] == nullptr ||
            playing_area_[0][col]->type == "G")
        {
            return false;
        }
    }
    //Alarivi
    for (unsigned int col = 0; col < COLUMNS; col++)
    {
        if (playing_area_[4][col] == nullptr ||
            playing_area_[4][col]->type == "R")
        {
            return false;
        }
    }
    return true;
}

bool GameBoard::isValidLocations(const Location& start,
                                 const Location& destination) const
{
    // Tarkastetaan, että löytyvätkö molemmat pisteet pelilaudalta.
    if ( start.y > ROWS -1 || start.x > COLUMNS-1||
         destination.y > ROWS -1 || destination.x > COLUMNS -1)
    {
        return false;
    }

    // Katsotaan, onko siirto sallittu.
    if (invalid_positions_.count({destination.x, destination.y}))
    {
        return false;
    }

    // Tarkastetaan, että onko aloituspisteessä nappula.
    if (playing_area_[start.y][start.x] == nullptr)
    {
        return false;
    }

    // Tarkastetaan, onko loppupiste tyhjä.
    if (playing_area_[destination.y][destination.x] != nullptr)
    {
        return false;
    }

    // Tarkastetaan, onko alkupiste sama kuin loppupiste.
    if (start.x == destination.x && start.y == destination.y)
    {
        return false;
    }
    return true;
}

bool GameBoard::move(const Location& start, const Location& destination)
{
    // Tarkistetaan suora liike.
    if (moveInRowOrCol(start, destination))
    {
        playing_area_[destination.y][destination.x] = std::move(playing_area_[start.y][start.x]);
        return true;
    }

    // Etsitään reitti.
    queue<Location> q;
    set<Location> visited;
    map<Location, Location> parent;

    // Lisätään aloituspiste.
    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        Location current = q.front();
        q.pop();

        // Jos pääsimme määränpäähän löysimme reitin.
        if (current == destination)
        {
            // Jäljitä polku takaisin ja tarkista, ovatko kaikki liikkeet sallittuja.
            Location path_point = current;
            bool valid_path = true;

            // Tarkista reitin jokainen vaihe.
            while (parent.count(path_point) && path_point != start)
            {
                Location prev_point = parent[path_point];
                if (!moveInRowOrCol(prev_point, path_point))
                {
                    valid_path = false;
                    break;
                }
                path_point = prev_point;
            }

            // Jos reitti on sallittu suoritetaan siirto.
            if (valid_path)
            {
                playing_area_[destination.y][destination.x] = std::move(playing_area_[start.y][start.x]);
                return true;
            }
            return false;
        }

        //Tarkista viereiset paikat.
        const vector<pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        for (const auto& dir : directions) {
            int next_x = current.x + dir.first;
            int next_y = current.y + dir.second;

            // Tarkista, onko paikka sallittu.
            if (next_x < 0 || next_x >= static_cast<int>(COLUMNS) || next_y < 0 || next_y >= static_cast<int>(ROWS))
            {
                continue;
            }

            Location next_pos = {static_cast<unsigned int>(next_x), static_cast<unsigned int>(next_y)};

            // Ohitetaan, jos ei ole sallittu tai jo täytetty.
            if (isInvalidPosition(next_pos) || visited.count(next_pos))
            {
                continue;
            }

            // Ohita jos paikka on tyhjä.
            if (next_pos != destination && playing_area_[next_y][next_x] != nullptr)
            {
                continue;
            }

            // Tarkista jos voimme liikkua seuraavaan paikkaan.
            if (!moveInRowOrCol(current, next_pos))
            {
                continue;
            }

            // Lisätään jonoon.
            q.push(next_pos);
            visited.insert(next_pos);
            parent[next_pos] = current;
        }
    }
    return false;
}

bool GameBoard::isInvalidPosition(const Location& pos) const
{
    // Tarkistetaan näytetäänkö paikka tyhjänä.
    bool is_invalid = invalid_positions_.count({pos.x, pos.y}) > 0;
    return is_invalid;
}

bool GameBoard::moveInRowOrCol(const Location& start, const Location& destination) const
{
    // Tarkista onko liike samalla rivillä.
    if (start.y == destination.y)
    {
        // Liikutaan horisontaalisesti.
        unsigned int min_x = min(start.x, destination.x);
        unsigned int max_x = max(start.x, destination.x);

        // Tarkistetaan, ovatko kaikki paikat tyhjinä.
        for (unsigned int x = min_x + 1; x < max_x; ++x)
        {
            Location current = {x, start.y};
            // Ohitetaan laittomat paikat.
            if (isInvalidPosition(current))
            {
                continue;
            }
            // Jos paikka ei ole tyhjä, reitti on kelvoton.
            if (playing_area_[start.y][x] != nullptr)
            {
                return false;
            }
        }
        return true;
    }

    // Tarkistetaan, onko liike samalla kolumnilla.
    if (start.x == destination.x)
    {
        // Liikutaan vertikaalisesti.
        unsigned int min_y = min(start.y, destination.y);
        unsigned int max_y = max(start.y, destination.y);

        // Tarkistetaan, ovatko kaikki paikat tyhjiä.
        for (unsigned int y = min_y + 1; y < max_y; ++y)
        {
            Location current = {start.x, y};
            // Ohitetaan laittomat paikat.
            if (isInvalidPosition(current))
            {
                continue;
            }
            // Jos paikka ei ole tyhjä reitti on estetty.
            if (playing_area_[y][start.x] != nullptr)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
