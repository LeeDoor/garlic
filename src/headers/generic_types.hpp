#pragma once

using Point = sf::Vector2f;
using Position = Point;
using Size = Point;
using Direction = sf::Angle; 
using PxPerMs = float;
using Time = sf::Time;
struct Rectangle : public sf::RectangleShape {
public:
    Rectangle() {
        setOutlineColor(sf::Color::Green);
        setOutlineThickness(2);
        setFillColor(sf::Color::Transparent);
    }
    static Rectangle from_position_and_size(Point position, Point size) {
        return Rectangle { position, size };
    }
private:
    Rectangle(Point position, Point size) : Rectangle() {
        setPosition(position);
        setSize(size);
    }
};
