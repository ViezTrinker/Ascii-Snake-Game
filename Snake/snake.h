#ifndef SNAKE_H
#define SNAKE_H

#include <array>
#include <cstdint>
#include <random>
#include <vector>

enum class EnableWallDeath : bool
{
	Yes = true,
	No = false
};

inline constexpr size_t Width = 25;
inline constexpr size_t Height = 25;
inline constexpr uint16_t StartLength = 4;
inline constexpr uint32_t DelayMs = 250;
inline constexpr EnableWallDeath Enabler = EnableWallDeath::No;
inline constexpr char Head = 'O';
inline constexpr char Tail = 'o';
inline constexpr char Wall = '#';
inline constexpr char Food = '+';

static_assert(StartLength < Width - 2);

class Snake
{
public:
	Snake();

	bool IsGameRunning(void);

	void Print(void) const;
	void Input(void);
	void Logic(void);
private:
	struct Position
	{
		uint16_t x;
		uint16_t y;
	};

	enum class Direction : uint8_t
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		NoDirection = 4
	};

	uint16_t GenerateRandomInteger(uint16_t start, uint16_t end);
	Position GenerateRandomPosition(void);

	void MoveSnake(void);
	void CheckMirrorPosition();

	void CollisionDetection(void);
	void CollisionWallDetection(void);
	void CollisionSelfDetection(void);
	void CollisionFoodDetection(void);
	
	char _board[Width][Height] = { };

	std::vector<Position> _snake{ {Width / 2, Height / 2} };
	Position _food;
	Direction _direction = Direction::NoDirection;

	bool _isGrowing = false;

	uint16_t _points = 0;
	bool _isGameRunning = true;
};

#endif // SNAKE_H