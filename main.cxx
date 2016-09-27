#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <complex>
#include <cmath>

sf::Color ToHSV(double x)
{
	x -= std::trunc(x);

	int Sector = x * 6;

	double Rising = (x - Sector / 6.0) * 6;
	double Falling = 1 - Rising;

	double Red;
	double Green;
	double Blue;

	switch (Sector)
	{
	case 0:
		Red = 1;
		Green = Rising;
		Blue = 0;
		break;

	case 1:
		Red = Falling;
		Green = 1;
		Blue = 0;
		break;

	case 2:
		Red = 0;
		Green = 1;
		Blue = Rising;
		break;

	case 3:
		Red = 0;
		Green = Falling;
		Blue = 1;
		break;

	case 4:
		Red = Rising;
		Green = 0;
		Blue = 1;
		break;

	case 5:
		Red = 1;
		Green = 0;
		Blue = Falling;
		break;

	default:
		throw -1;
	}

	return sf::Color(Red * 255, Green * 255, Blue * 255);
}

int main()
{
	static const std::size_t Width = 1600;
	static const std::size_t Height = 900;

	static const std::size_t Threshold = 1000;

	sf::RenderWindow Window(sf::VideoMode(Width, Height), "test");

	sf::Image Image;
	Image.create(Width, Height);

	/*for (std::size_t x = 0; x < Width; ++x)
		for (std::size_t y = 0; y < Height; ++y)
			Image.setPixel(x, y, ToHSV(static_cast<double>(y) / Height));*/

	for (std::size_t x = 0; x < Width; ++x)
		for (std::size_t y = 0; y < Height; ++y)
		{
			unsigned Cycles = 0;

			std::complex<double> c;
			c.real((x / static_cast<double>(Width - 1) - 0.5) * 4.0);
			c.imag((y / static_cast<double>(Height - 1) - 0.5) * (4.0 / Width * Height));

			std::complex<double> z(0, 0);

			while (++Cycles < Threshold && std::abs(z) < 2)
			{
				z = z * z + c;
			}

			sf::Color Result;
			if (Cycles == Threshold)
			{
				Result = sf::Color::Black;
			}
			else
			{
				Result = ToHSV(Cycles / 50.0);
			}
			Image.setPixel(x, y, Result);
		}

	Image.saveToFile("output.png");

	sf::Texture Texture;
	Texture.create(Width, Height);
	Texture.update(Image);

	sf::Sprite Sprite;
	Sprite.setTexture(Texture);

	while (Window.isOpen())
	{
		sf::Event Event;
		while (Window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				Window.close();
				return 0;
			}
		}

		Window.clear();
		Window.draw(Sprite);
		Window.display();
	}
}
