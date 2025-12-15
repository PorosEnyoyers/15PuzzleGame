#include <iostream>
#include <array>
#include <functional>
#include "Random.h"
class Direction
{
public:
    enum Type
    {
        up,
        down,
        left,
        right,

        maxDirections
    };
private:
    Type m_direction{};

public:
    Direction(Type t) :
        m_direction{ t }
    {
    }
    Direction operator-()
    {
        switch (m_direction)
        {
        case (Type::up):
            return Direction {Type::down};
        case (Type::down):
            return Direction { Type::up };
        case (Type::left):
            return Direction { Type::right };
        case (Type::right):
            return Direction { Type::left };
        default:
            return Direction{ Type::maxDirections };
        }
    }
    friend std::ostream& operator<< (std::ostream& out, const Direction& d)
    {
        switch (d.m_direction)
        {
        case (Type::up): {
            out << "up";
            return out;
        }
        case (Type::down): {
            out << "down";
            return out;
        }
        case (Type::left): {
            out << "left";
            return out;
        }
        case (Type::right): {
            out << "right";
            return out;
        }
        default:
            return out;
        }
    }
    Type getDirection() { return m_direction; }
};
namespace Settings
{
    constexpr const std::size_t boardRow{ 4 };
    constexpr const std::size_t boardCol{ 4 };
    constexpr const std::size_t boardTiles{ boardRow * boardCol };
    Direction generateRandomDirection()
    {
        Direction temp{ static_cast<Direction::Type>(Random::get(0, 3)) };
        return temp;
    }
    static bool quitGame{ false };
    constexpr int g_consoleLines{ 25 };
}

template <typename T, std::size_t Row, std::size_t Col>
using ArrayFlat2d = std::array<T, Row * Col>;

template <typename T, std::size_t Row, std::size_t Col>
class ArrayView2d
{
private:
    std::reference_wrapper<ArrayFlat2d<T, Row, Col>> m_arr{};

public:
    ArrayView2d(ArrayFlat2d<T, Row, Col> &arr)
        : m_arr{ arr }
    {}

    T& operator[](int i) { return m_arr.get()[static_cast<std::size_t>(i)]; }
    const T& operator[](int i) const { return m_arr.get()[static_cast<std::size_t>(i)]; }

    T& operator()(int row, int col) { return m_arr.get()[static_cast<std::size_t>(row * cols() + col)]; }
    const T& operator() (int row, int col) const { return m_arr.get()[static_cast<std::size_t>(row * cols() + col)]; }

    int rows() const { return static_cast<int>(Row); }
    int cols() const { return static_cast<int>(Col); }
    int length() const { return static_cast<int>(Col * Row); }
};

class Tile
{
private:
    int m_tileInfo{};
public:
    Tile() = default;
    Tile(int x) :
        m_tileInfo{ x }
    {
    }
    friend std::ostream& operator<<(std::ostream& out, const Tile& tile)
    {
        if (tile.m_tileInfo == 0)
            out << "    ";
        else if (tile.m_tileInfo < 10)
            out << "  " << tile.m_tileInfo << " ";
        else
            out << " " << tile.m_tileInfo << " ";
        return out;
    }
    Tile& operator=(int x)
    {
        m_tileInfo = x;
        return *this;
    }
    Tile& operator=(const Tile& tile)
    {
        m_tileInfo = tile.m_tileInfo;
        return *this;
    }
    friend bool operator==(Tile& tile, int x)
    {
        return (tile.m_tileInfo == x);
    } 
    bool isEmpty() { return (m_tileInfo == 0); }
    int getNum() { return m_tileInfo; }

    operator int() const { return m_tileInfo; }
};

class Point
{
private:
    std::pair<int, int> m_point{};

public:
    Point(int row, int col) :
        m_point{ row, col }
    {
    }
    Point getAdjacentPoint(Direction d)
    {
        switch (d.getDirection())
        {
        case(Direction::up):
            return { m_point.first, m_point.second - 1 };
        case(Direction::down):
            return { m_point.first, m_point.second + 1 };
        case(Direction::left):
            return { m_point.first - 1, m_point.second };
        case(Direction::right):
            return { m_point.first + 1, m_point.second };
        default:
            return { m_point.first, m_point.second };
        }
    }
    friend bool operator==(const Point& p1, const Point& p2)
    {
        return (p1.m_point.first == p2.m_point.first) && (p1.m_point.second == p2.m_point.second);
    }
    friend bool operator!=(const Point& p1, const Point& p2)
    {
        return !(p1 == p2);
    }

    std::pair<int, int> getPoint(){ return m_point; }
};
class Board
{
private:
    ArrayFlat2d <Tile, Settings::boardRow, Settings::boardCol> m_board{};
    ArrayView2d<Tile, Settings::boardRow, Settings::boardCol> m_arrView{ m_board };
    bool m_exit{ false };
public:
    Board() :
        m_board{}
    {
        for (int x = 0; x < Settings::boardTiles; ++x)
        {
            m_board[x] = (x + 1);
        }
        m_board[Settings::boardTiles - 1] = 0;
    }
    friend std::ostream& operator<<(std::ostream& out, const Board& board)
    {
        for (int i = 0; i < Settings::g_consoleLines; ++i)
        {
            out << '\n';
        }
        for (int row{ 0 }; row < board.m_arrView.rows(); ++row)
        {
            for (int col{ 0 }; col < board.m_arrView.cols(); ++col)
                out << board.m_arrView(row, col) << " ";
            out << '\n';
        }

        out << '\n';
        return out;
    }
    void setExitTrue(){ m_exit = true; }
    bool getExit() { return m_exit; }
    bool isPointValid(Point p)
    {
        if ( p.getPoint().first < 0 || p.getPoint().second < 0 || p.getPoint().first > 3 || p.getPoint().second > 3)
        {
            return false;
        }
        return true;
    }
    Point emptyTileLocal()
    {
        for (int row{ 0 }; row < m_arrView.rows(); ++row)
        {
            for (int col{ 0 }; col < m_arrView.cols(); ++col)
            {
                if (m_arrView(row, col) == 0)
                    return Point{ col,row };
            }
        }
        return Point{ 0,0 };
    }
    void swapTile(Point a, Point b)
    {
        std::swap(m_arrView(a.getPoint().second, a.getPoint().first), m_arrView(b.getPoint().second, b.getPoint().first));
    }

    bool moveTile(Direction d)
    {
        Point emptyTile{ emptyTileLocal() };
        Point adjTile{ emptyTile.getAdjacentPoint(-d) };
        if(!(isPointValid(adjTile)))
        {
            return false;
        }
        swapTile(emptyTile, adjTile);
        return true;
    }
    void randomize()
    {
        for (int i{ 0 }; i < 10000; ++i)
        {
            Direction temp{ Settings::generateRandomDirection() };
            (void)bool{ moveTile(temp) };
        }
    }
    friend bool operator==(const Board& b1, const Board& b2)
    {
        for (int i{ 0 }; i < Settings::boardTiles; i++)
        {
            if (b1.m_arrView[i] != b2.m_arrView[i])
                return false;
        }
        return true;
    }
    bool won(const Board& b)
    {
        return(*this == b);
    }
};
namespace UserInput
{
    void ignoreLine()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    bool clearFailedExtraction()
    {
        if (!std::cin)
        {
            if (std::cin.eof())
                std::exit(0);

            std::cin.clear();
            return true;
        }
        return false;
    }
    bool hasUnextractedInput()
    {
        return(((!std::cin.eof())) && (std::cin.peek() != '\n'));
    }
    Direction convertToDirection(char c)
    {
        switch (c)
        {
        case 'w':
            return { Direction::up };
        case 'a':
            return { Direction::left };
        case 's':
            return { Direction::down };
        case 'd':
            return { Direction::right };
        default:
            return { Direction::maxDirections };
        }
    }
    Direction getUserDirection()
    {
        while(true)
        {
            char temp{};
            std::cin >> temp;
            if (hasUnextractedInput() || clearFailedExtraction() || (temp != 'w' && temp != 'a' && temp != 's' && temp != 'd' && temp != 'q'))
            {
                ignoreLine();
                continue;
            }
            if (temp == 'q')
            {
                Settings::quitGame = true;
                return { Direction::maxDirections };
            }
            Direction dir{convertToDirection(temp)};;
            return dir;
        }
    }
}
int main()
{
    Board board{};
    board.randomize();
    Board solvedBoard{};
    while((!(board.won(solvedBoard)) && !(Settings::quitGame)))
    {
        std::cout << board;
        Direction userDirection{ UserInput::getUserDirection() };
        if (board.moveTile(userDirection))
        {
            continue;
        }
        continue;
    }
    if (Settings::quitGame)
    {
        std::cout << "Bye!!!";
        return 0;
    }
    std::cout << "\n\n\n\nYou won!!! Congrats!!!\n\n\n";
    return 0;
}
