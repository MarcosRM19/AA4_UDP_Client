#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Cell
{
private:
	int id;
	sf::Color color;
	std::vector<std::shared_ptr<Cell>> nextCells;
	sf::Vector2f position;
	sf::RectangleShape square;
	std::unique_ptr<sf::ConvexShape> safeZone;
	int tokensInCell = 0;

	bool render = true;

	void CreateNormalCell();
	void CreateSafeZone();

public:
	Cell(int _id, const sf::Color& _color);

	void Render(sf::RenderWindow& window);
	void SetPositionSquare(const sf::Vector2f& _position);
	void SetPositionTriangles(const sf::Vector2f& _position, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Vector2f& point3, float angle);
	sf::Vector2f SetTokenInCell(float quotient = -1) const;

	inline int GetId() const { return id; }
	inline int GetTokensInCell() const { return tokensInCell; }
	inline sf::Color GetColor() const { return color; }
	inline const std::vector<std::shared_ptr<Cell>>& GetNextCells() const { return nextCells; }
	inline const sf::Vector2f& GetPosition() const { return position; }
	inline const sf::RectangleShape& GetSquare() const { return square; }

	inline void AddNextCell(std::shared_ptr<Cell> nextCell) { nextCells.push_back(nextCell); }
	inline void SetRender(const bool _render) { render = _render; }
	inline void AddTokensInCell(const int quantity) { tokensInCell += quantity; }
};
