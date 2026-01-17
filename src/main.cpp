#include <SFML/Graphics.hpp>
#include <cmath>
#include "car.hpp"

int main()
{
    int constexpr SCREEN_WIDTH = 3440, SCREEN_HEIGHT = 1440,
        WINDOW_WIDTH = 600,  WINDOW_HEIGHT = 600;
    float constexpr CIRCLE_RAD = 25, 
          CIRCLE_ROTATION_RAD = 25, 
          CIRCLE_ROTATION_SPEED = 0.005;
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "SFML works!");
    int posX = SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2;
    int posY = SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2;
    window.setPosition(sf::Vector2i(posX, posY));
    window.setFramerateLimit(144);
    sf::Clock clock;
    Car car(Point{300, 200});
    Time elapsed_prev = Time::Zero;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        Time elapsed = clock.getElapsedTime();
        Time delta = elapsed - elapsed_prev;
        elapsed_prev = elapsed;
        std::cout << "\r" << delta.asMicroseconds() << "mcs" << std::flush;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            car.roolie(delta, -1);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            car.roolie(delta, 1);
        else car.roolie(delta, 0);
        car.drive_tick(delta);


        window.clear();
        window.draw(car);


        window.display();
    }
}
