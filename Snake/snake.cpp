#include "snake.h"

#include <conio.h>
#include <iostream>

Snake::Snake(void)
{
	// Upper wall line
	for (size_t index = 0; index < Width; index++)
	{
		_board[index][0] = Wall;
	}
	// Lower wall line
	for (size_t index = 0; index < Width; index++)
	{
		_board[index][Height - 1] = Wall;
	}
	// Left wall side
	for (size_t index = 0; index < Height; index++)
	{
		_board[0][index] = Wall;
	}
	// Right wall line
	for (size_t index = 0; index < Height; index++)
	{
		_board[Width - 1][index] = Wall;
	}
	// The rest filled with empty spaces
	for (size_t indexY = 1; indexY < Height -1; indexY++)
	{
		for (size_t indexX = 1; indexX < Width - 1; indexX++)
		{
			_board[indexX][indexY] = ' ';
		}
	}

	// Initialize snake
	for (uint16_t index = 1; index < StartLength; index++)
	{
		_snake.push_back({ Width / 2 + index, Height / 2 });
	}

	// Initialize food
	_food = GenerateRandomPosition();

}

void Snake::Print(void) const
{
	// Clear console
	system("cls");

	bool isSnakePos = false;
	bool isHead = false;
	
	for (size_t indexY = 0; indexY < Height; indexY++)
	{
		for (size_t indexX = 0; indexX < Width; indexX++)
		{
			if (indexX == 0 || indexX == Width - 1 || indexY == 0 || indexY == Height - 1)
			{
				std::cout << Wall;
				continue;
			}
			
			isSnakePos = false;
			isHead = false;
			for (size_t indexSnake = 0; indexSnake < _snake.size(); indexSnake++)
			{
				if (_snake[indexSnake].x == indexX && _snake[indexSnake].y == indexY)
				{
					isSnakePos = true;
					if (indexSnake == 0)
					{
						isHead = true;
					}
				}
			}
			if (!isSnakePos)
			{
				if (_food.x == indexX && _food.y == indexY)
				{
					std::cout << Food;
				}
				else
				{
					std::cout << _board[indexX][indexY];
				}
			}
			else
			{
				if (!isHead)
				{
					std::cout << Tail;
				}
				else
				{
					std::cout << Head;
				}
			}
		}
		std::cout << '\n';
	}
	std::cout << "Points: " << _points << '\n';
	if (!_isGameRunning)
	{
		std::cout << "You lost!\n\n\n";
	}
}

bool Snake::IsGameRunning(void)
{
	return _isGameRunning;
}

uint16_t Snake::GenerateRandomInteger(uint16_t start, uint16_t end)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(start, end);

	uint16_t randomNumber = distrib(gen);
	return randomNumber;
}

Snake::Position Snake::GenerateRandomPosition(void)
{
	bool isPositionGood = false;
	Position position = { 0,0 };

	while (!isPositionGood)
	{
		isPositionGood = true;
		position.x = GenerateRandomInteger(1, Width - 2);
		position.y = GenerateRandomInteger(1, Height - 2);

		for (size_t index = 0; index < _snake.size(); index++)
		{
			if (_snake[index].x == position.x && _snake[index].y == position.y)
			{
				isPositionGood = false;
			}
		}
	}
	return position;
}

void Snake::Input(void)
{
	if (_kbhit())
	{
		int ch = _getch();
		if (ch == 224)
		{
			int arrow = _getch();
			switch (arrow) { // the real value
			case 72:
				if (_direction == Direction::Down)
				{
					break;
				}
				_direction = Direction::Up;
				break;
			case 80:
				if (_direction == Direction::Up)
				{
					break;
				}
				_direction = Direction::Down;
				break;
			case 77:
				if (_direction == Direction::Left || _direction == Direction::NoDirection)
				{
					break;
				}
				_direction = Direction::Right;
				break;
			case 75:
				if (_direction == Direction::Right)
				{
					break;
				}
				_direction = Direction::Left;
				break;
			default:
				break; // Keep last direction;
			}
		}
	}
}

void Snake::Logic(void)
{
	MoveSnake();
	CollisionDetection();
}

void Snake::MoveSnake(void)
{
	if (_direction == Direction::NoDirection)
	{
		// We are not moving, nothing to check here
		return;
	}

	// Save last tail position
	Position lastTail;
	lastTail = _snake[_snake.size() - 1];


	for (size_t index = _snake.size() - 1; index > 0; index--)
	{
		_snake[index] = _snake[index - 1];
	}

	// Grow if necessary
	if (_isGrowing)
	{
		_snake.push_back(lastTail);
		_isGrowing = false;
	}
	
	switch (_direction)
	{
	case Direction::Up:
		_snake[0].y--;
		break;
	case Direction::Down:
		_snake[0].y++;
		break;
	case Direction::Right:
		_snake[0].x++;
		break;
	case Direction::Left:
		_snake[0].x--;
		break;
	}

	if (Enabler == EnableWallDeath::No)
	{
		CheckMirrorPosition();
	}
}

void Snake::CheckMirrorPosition(void)
{
	for (size_t index = 0; index < _snake.size(); index++)
	{
		if (_snake[index].x == 0)
		{
			_snake[index].x = Width - 2;
		}
		if (_snake[index].x == Width - 1)
		{
			_snake[index].x = 1;
		}
		if (_snake[index].y == 0)
		{
			_snake[index].y = Height - 2;
		}
		if (_snake[index].y == Height - 1)
		{
			_snake[index].y = 1;
		}
	}
}

void Snake::CollisionDetection(void)
{
	CollisionWallDetection();
	CollisionSelfDetection();
	CollisionFoodDetection();
}

void Snake::CollisionWallDetection(void)
{
	if (Enabler == EnableWallDeath::No)
	{
		return;
	}

	if (_snake[0].x == 0 || _snake[0].x == Width - 1 || _snake[0].y == 0 || _snake[0].y == Height - 1)
	{
		_isGameRunning = false;
	}
}

void Snake::CollisionSelfDetection(void)
{
	for (size_t index = 1; index < _snake.size(); index++)
	{
		if (_snake[0].x == _snake[index].x && _snake[0].y == _snake[index].y)
		{
			_isGameRunning = false;
		}
	}
}

void Snake::CollisionFoodDetection(void)
{
	// No collision with food, return
	if (_snake[0].x != _food.x || _snake[0].y != _food.y)
	{
		return;
	}

	// Increase points
	_points++;

	// We have collided with food, generate new food
	_food = GenerateRandomPosition();

	// Set Grow flag
	_isGrowing = true;
}