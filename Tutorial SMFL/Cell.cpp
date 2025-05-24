#include "Cell.h"

Cell::Cell(int _id, const sf::Color& _color)
    : id(_id), color(_color), tokensInCell(0), render(true)
{
    CreateNormalCell();

    if (nextCells.empty())
        CreateSafeZone();
}

void Cell::CreateNormalCell()
{
    square.setSize({ 48.f, 48.f });
    square.setFillColor(color);
    square.setOutlineThickness(2.f);
    square.setOutlineColor(sf::Color::Black);
}

void Cell::CreateSafeZone()
{
    safeZone = std::make_unique<sf::ConvexShape>();
    safeZone->setPointCount(3);
    safeZone->setFillColor(square.getFillColor());
    safeZone->setOutlineThickness(2.f);
    safeZone->setOutlineColor(sf::Color::Black);
}

void Cell::Render(sf::RenderWindow& window)
{
    if (!render)
        return;

    if (!nextCells.empty())
        window.draw(square);
    else
        window.draw(*safeZone);
}

void Cell::SetPositionSquare(const sf::Vector2f& _position)
{
    position = _position;
    square.setPosition(position);
}

void Cell::SetPositionTriangles(const sf::Vector2f& _position, const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Vector2f& point3, float angle)
{
    safeZone->setPoint(0, point1);
    safeZone->setPoint(1, point2);
    safeZone->setPoint(2, point3);
    safeZone->setRotation(sf::degrees(angle));
    
    position = _position;
    safeZone->setPosition(position);
}

sf::Vector2f Cell::SetTokenInCell(const float quotient) const
{
    sf::Vector2f result = position;

    if (quotient == -1)
    {
        result.x += square.getSize().x / 2;
        result.y += square.getSize().y / 2;
    }
    else
    {
        result.x += square.getSize().x * quotient / 4;
        result.y += square.getSize().y * quotient / 4;
    }

    return result;
}
