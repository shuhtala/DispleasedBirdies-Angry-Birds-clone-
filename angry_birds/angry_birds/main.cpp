#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
/*
using namespace sf;
int main()
{
    float a = 5;
    float x = 0;
    ContextSettings settings;
    settings.antialiasingLevel = 4;
    RenderWindow window(VideoMode(1920, 1080), "RENDER");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        CircleShape circle((3 * cos(x) + sqrt(x) * 0.8 - 13) / 1.1);
        circle.setFillColor(Color(sin(x) * 255, 35, 75));
        circle.move(x * sin(x / 10), x * cos(x / 10));
        window.draw(circle);
        window.display();
        if (x < 2150)
        {
            x += a;
        }
        else
        {
            x = 0;
            a += 0.005;
            window.clear(Color(75 * sin((a - 5) / 10), 0, 0, 0));
        }
    }
    return 0;
}*/
/*
int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Angry Birds");

	while (window.isOpen())
	{
		// handle events

		//update gamelogic

		window.clear();

		// draw objects

		window.display();
	}


}*/

int main()
{

    // image

    sf::RectangleShape shape(sf::Vector2f(100.0f, 100.0f));
    shape.setFillColor(sf::Color(255, 0, 0));
    sf::Texture lintu;
    lintu.loadFromFile("bird.png");

    shape.setPosition(150.f, 400.f);

    shape.setTexture(&lintu);

    // update the texture from the current contents of the window

    // window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // mouse
    bool mouseYes = false;

    int X = 0;
    int Y = 0;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                mouseYes = true;
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                mouseYes = false;
            }

            if (event.type == sf::Event::MouseMoved)
            {
                X = event.mouseMove.x;
                Y = event.mouseMove.y;
            }
            if (mouseYes == true)
            {
                shape.setPosition(X, Y);
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(shape);

        window.display();
    }

    return 0;
}