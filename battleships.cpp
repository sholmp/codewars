// Kata 'Battleship field validator : https://www.codewars.com/kata/52bb6539a4cf1b12d90005b7

#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <inttypes.h>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

struct Point
{
  Point(int x, int y) : x(x), y(y) {}
  int x, y;
  Point operator+(const Point &rhs) const
  {
    return {x + rhs.x, y + rhs.y};
  }

  bool operator==(const Point &rhs) const
  {
    return x == rhs.x && y == rhs.y;
  }

  bool operator<(const Point &rhs) const
  {
    return x < rhs.x || (x == rhs.x && y < rhs.y);
  }
};

enum ShipType
{
  BattleShip = 4,
  Cruiser = 3,
  Destroyer = 2,
  Submarine = 1
};

class Ship
{
public:
  Ship(const Point &seed, int length, bool horizontal) : type_(static_cast<ShipType>(length)) // Shiptypes underlying enum value is its length
  {
    points_.push_back(seed);
    for (int i = 1; i < length; i++)
    {
      points_.push_back(horizontal ? seed + Point{0, i} : seed + Point{i, 0});
    }
  }
  ShipType type() const { return type_; }

  set<Point> boundary()
  {
    static vector<Point> unitDirections = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    set<Point> boundary;
    for (const auto &p : points_)
    {
      for (const auto &d : unitDirections)
      {
        Point adjacent = p + d;
        if (std::find(points_.begin(), points_.end(), adjacent) == points_.end()) // if the point is not part of the ship's core, then add it to the boundary
        {
          boundary.insert(adjacent);
        }
      }
    }

    return boundary;
  }

  friend bool operator==(const Ship &ship, const vector<Point> &points)
  {
    return ship.points_ == points;
  }

  friend ostream &operator<<(ostream &os, Ship &rhs)
  {
    os << "[ ";
    for (auto p : rhs)
    {
      os << "(" << p.x << ", " << p.y << "), ";
    }
    os << "]" << endl;
    return os;
  }

  vector<Point>::iterator begin() { return points_.begin(); }
  vector<Point>::iterator end() { return points_.end(); }

private:
  vector<Point> points_;
  ShipType type_;
};

void transposeField(vector<vector<int>> &field)
{
  int N = field.size();
  for (int i = 0; i < N; i++)
  {
    for (int j = i + 1; j < N; j++)
    {
      swap(field[i][j], field[j][i]);
    }
  }
}

vector<Ship> findShips(vector<vector<int>> field)
{
  vector<Ship> shipsFound;
  int width = field[0].size();

  for (auto shipType : {BattleShip, Cruiser, Destroyer, Submarine}) // Ordering is important. Largest --> smallest
  {
    vector<int> ship(shipType, 1);
    for (bool horizontal : {true, false})
    {
      for (int i = 0; i < field.size(); i++) // for every row
      {
        auto &row = field[i];
        for (int j = 0; j <= width - shipType; j++)
        {
          vector<int> mask(row.begin() + j, row.begin() + j + shipType);
          if (mask == ship)
          {
            if (horizontal)
              shipsFound.push_back(Ship(Point(i, j), shipType, horizontal));
            else
              shipsFound.push_back(Ship(Point(j, i), shipType, horizontal)); // flip point coords if field has been transposed

            cout << "Found ship: " << shipsFound.back() << endl;
            fill(row.begin() + j, row.begin() + j + shipType, 0); //  clear the elements in which we found a ship
          }
        }
      }
      transposeField(field);
    }
  }

  return shipsFound;
}

map<ShipType, uint8_t> countShipTypes(const vector<Ship> &ships)
{
  map<ShipType, uint8_t> shipTypesCounter;
  for (const auto &ship : ships)
  {
    shipTypesCounter[ship.type()]++;
  }
  return shipTypesCounter;
}

bool validate_battlefield(std::vector<std::vector<int>> field)
{
  // Write your magic here ;)
  auto ships = findShips(field);
  auto shipTypeCounts = countShipTypes(ships);
  bool correctNumberOfShipsFound = shipTypeCounts[BattleShip] == 1 && shipTypeCounts[Cruiser] == 2 && shipTypeCounts[Destroyer] == 3 && shipTypeCounts[Submarine] == 4;

  if (!correctNumberOfShipsFound)
    return false;

  return true;
}

// TEST(BattleShipClass, SanityConstructionCheck)
// {

//   Ship horizontalCruiser(Point{0, 0}, 3, true);
//   vector<Point> shipShouldBeThis = {{0, 0}, {0, 1}, {0, 2}};
//   bool result = horizontalCruiser == shipShouldBeThis;
//   EXPECT_EQ(result, true);

//   Ship verticalCruiser(Point{0, 0}, 3, false);
//   shipShouldBeThis = {{0, 0}, {1, 0}, {2, 0}};

//   EXPECT_EQ(verticalCruiser == shipShouldBeThis, true);
// }

// TEST(SaenkeSlagskib, GoodMap)
// {
//   std::vector<std::vector<int>> map = {
//       std::vector<int>{1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
//       std::vector<int>{1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
//       std::vector<int>{1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
//       std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
//       std::vector<int>{0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
//       std::vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

//   std::vector<std::vector<int>> map2 = {
//       std::vector<int>{1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
//       std::vector<int>{0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
//       std::vector<int>{1, 1, 1, 0, 1, 1, 1, 1, 1, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
//       std::vector<int>{0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
//       std::vector<int>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
//       std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

//   EXPECT_EQ(validate_battlefield(map2), true);
// }
TEST(SaenkeSlagSkib, CorrectBoundaryPoints)
{
  set<Point> expectedBoundary = {{-1, -1}, {-1, 0}, {-1, 1}, {-1, 2}, {-1, 3}, {0, -1}, {0, 3}, {1, -1}, {1, 0}, {1, 1}, {1, 2}, {1, 3}};

  Ship ship(Point{0, 0}, Cruiser, true);
  set<Point> shipBoundary = ship.boundary();
  for (auto p : expectedBoundary)
  {
    cout << "{" << p.x << ", " << p.y << "}, ";
  }
  cout << endl;
  for (auto p : shipBoundary)
  {
    cout << "{" << p.x << ", " << p.y << "}, ";
  }
  bool equal = ship.boundary() == expectedBoundary;
  EXPECT_EQ(equal, true);
}

TEST(Transpose, TransposeSomethingSimple)
{
  vector<vector<int>> T = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  auto &ref = T[1];
  ref[0] = 321;
  transposeField(T);

  EXPECT_EQ(T[0][1], 321);
}

TEST(SaenkeSlagSkib, TestTransposable)
{
  std::vector<std::vector<int>> map2 = {
      std::vector<int>{0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
      std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
      std::vector<int>{0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
      std::vector<int>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  auto ships = findShips(map2);
  auto counts = countShipTypes(ships);

  EXPECT_EQ(counts[BattleShip], 2);
  EXPECT_EQ(counts[Cruiser], 1);
  EXPECT_EQ(counts[Destroyer], 1);
}
