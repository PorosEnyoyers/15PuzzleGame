constexpr int g_consoleLines{ 25 };

#include <iostream>
#include <array>
#include <functional>
#include <algorithm>
#include <numeric>

namespace Settings
{
    constexpr const std::size_t boardRow{ 4 };
    constexpr const std::size_t boardCol{ 4 };
    constexpr const std::size_t boardTiles{ boardRow * boardCol };
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
    Tile& operator=(Tile& tile)
    {
        m_tileInfo = tile.m_tileInfo;
        return *this;
    }
    bool isEmpty() { return (m_tileInfo == 0); }
    int getNum() { return m_tileInfo; }
};

class Board
{
private:
    ArrayFlat2d <Tile, Settings::boardRow, Settings::boardCol> m_board{};
    ArrayView2d<Tile, Settings::boardRow, Settings::boardCol> m_arrView{ m_board };
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
        for (int i = 0; i < g_consoleLines; ++i)
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
};

int main()
{
    Board board{};
    std::cout << board;

    return 0;
}