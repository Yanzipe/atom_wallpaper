#include <array>
#include <cmath>

#include <SFML/Graphics.hpp>

sf::CircleShape makeElectron(sf::Color color, float radius, float angle, float ellipse_angle, sf::CircleShape orbit, sf::Vector2u origin)
{
	sf::CircleShape electron;
	electron.setFillColor(color);
	electron.setRadius(radius);

	sf::Vector2f position;

	float a = (orbit.getRadius() * orbit.getScale().x) + radius, b = (orbit.getRadius() * orbit.getScale().y) + radius/2;
	position.x = a * std::cos(angle) * std::cos(ellipse_angle) - b * std::sin(angle) * std::sin(ellipse_angle) + origin.x;
	position.y = a * std::cos(angle) * std::sin(ellipse_angle) + b * std::sin(angle) * std::cos(ellipse_angle) + origin.y;

	electron.setOrigin(radius, radius);
	electron.setPosition(position);

	return electron;
}
int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8U;
	sf::VideoMode screen = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(screen), "Atom wallpaper", 7U, settings);

	sf::Color back_color = sf::Color(148, 226, 238);

	sf::Color nucleus_color = sf::Color(235, 134, 62);
	sf::CircleShape nucleus(22.5f, 100);
	nucleus.setFillColor(nucleus_color);
	nucleus.setPosition(screen.width / 2 - nucleus.getRadius(), screen.height / 2 - nucleus.getRadius());

	sf::Color ellipse_color = sf::Color(1, 196, 182);
	sf::CircleShape orbital_base(30.f, 100);
	orbital_base.setScale(3.75, 1.5);
	orbital_base.setFillColor(sf::Color::Transparent);

	orbital_base.setOutlineThickness(5.0f);
	orbital_base.setOutlineColor(ellipse_color);

	orbital_base.setOrigin(sf::Vector2f{ orbital_base.getRadius(), orbital_base.getRadius() });
	orbital_base.setPosition(static_cast<float>(screen.width / 2), static_cast<float>(screen.height / 2));

	std::array<sf::CircleShape, 3> orbitals;
	for (std::size_t i = 0; i < orbitals.max_size(); i++)
	{
		sf::CircleShape orbital = orbital_base;
		orbital.rotate(static_cast<float>(90 + i * 60));
		orbitals[i] = orbital;
	}

	sf::Clock clock;
	float eccentric = 0.f;

	sf::Color electron_color = sf::Color(2, 155, 143);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(back_color);
		for (const sf::CircleShape& orbital : orbitals)
		{
			window.draw(orbital);
		}
		window.draw(makeElectron(electron_color, 10.f, eccentric, 0.5f, orbitals[1], { screen.width / 2, screen.height / 2 }));
		window.draw(makeElectron(electron_color, 10.f, eccentric+1, 1.575f, orbitals[1], { screen.width / 2, screen.height / 2 }));
		window.draw(makeElectron(electron_color, 10.f, eccentric+2, 2.6f, orbitals[1], { screen.width / 2, screen.height / 2 }));

		if (clock.getElapsedTime().asMilliseconds() >= 10)
		{
			eccentric += 0.05f;
			clock.restart();
		}

		window.draw(nucleus);
		window.display();
	}

	return 0;
}