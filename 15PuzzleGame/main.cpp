#include <iostream>
#include <array>
#include <functional>

template <typename T, std::size_t Row, std::size_t Col>
using ArrayFlat2d = std::array<T, Row * Col>;

template <typename T, std::size_t Row, std::size_t Col>
class ArrayView2d
{
private:
    std::reference_wrapper<ArrayFlat2d<T, Row, Col>> m_arr{};

public:
    ArrayView2d(ArrayFlat2d<T, Row, Col)> &arr)
        : m_arr{ arr };
    {}

    T& operator[](int i) { return m_arr.get()[static_cast<std::size_t>(i)]; }
    const T& operator[](int i) const { return m_arr.get()[static_cast<std::size_t>(i)]; }
};

class Tile
{
private:
    int m_tileInfo{};
public:
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
    bool isEmpty() { return (m_tileInfo == 0); }
    int getNum() { return m_tileInfo; }
};

int main()
{
    Tile tile1{ 10 };
    Tile tile2{ 8 };
    Tile tile3{ 0 }; // the missing tile
    Tile tile4{ 1 };

    std::cout << "0123456789ABCDEF\n"; // to make it easy to see how many spaces are in the next line
    std::cout << tile1 << tile2 << tile3 << tile4 << '\n';

    std::cout << std::boolalpha << tile1.isEmpty() << ' ' << tile3.isEmpty() << '\n';
    std::cout << "Tile 2 has number: " << tile2.getNum() << "\nTile 4 has number: " << tile4.getNum() << '\n';

    return 0;
}