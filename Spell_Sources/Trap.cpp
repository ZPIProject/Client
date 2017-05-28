#include "..\Spell_Headers\Trap.h"
#include "..\Collider_Headers\CircleCollider.h"

Trap::Trap(sf::Vector2i position, Trap_stats stats) : ColidableObject(new CircleCollider(stats.get_radius())){
	statistics = stats;

	trap_shape.setRadius(stats.get_radius());
	trap_shape.setPosition(static_cast<sf::Vector2f>(position));
	trap_shape.setFillColor(set_fill_color(stats.get_element()));
	trap_shape.setOrigin(stats.get_radius(), stats.get_radius());


	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(static_cast<sf::Vector2f>(position));
	}
}

Trap::Trap(const Trap & other) : ColidableObject(other)
{
	statistics = other.statistics;
	trap_shape = other.trap_shape;
}

Trap::~Trap() {
	
}

sf::Vector2f Trap::getPosition()
{
	return trap_shape.getPosition();
}

bool Trap::has_ended()
{
	return statistics.has_ended();
}

void Trap::onCollision(ColidableObject * object)
{
	is_active = false;
}

sf::CircleShape Trap::getShape()
{
	return trap_shape;
}

sf::Color Trap::set_fill_color(Element e)
{
	sf::Color color = sf::Color::Black;

	switch (e)
	{
	case Element::FIRE:
		color = sf::Color::Red;
		break;
	case Element::WIND:
		color = sf::Color::Green;
		break;
	case Element::THUNDER:
		color = sf::Color::Yellow;
		break;
	case Element::EARTH:
		color = sf::Color(148, 62, 15);
		break;
	case Element::WATER:
		color = sf::Color::Blue;
		break;
	}
	return color;
}