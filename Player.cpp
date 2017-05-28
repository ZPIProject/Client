#include "Player.h"
#include "Spell_Headers\Ball.h"
#include "Spell_Headers\Shield.h"
#include "Spell_Headers\Trap.h"


Player::Player(sf::Color players_color, float player_size, Player_stats stts) : ColidableObject(new CircleCollider(player_size/2)),stats(stts)
{
	player_shape.setPosition(sf::Vector2f(0, 0));
	player_shape.setFillColor(players_color);
	player_shape.setSize(sf::Vector2f(player_size, player_size));
	player_shape.setOrigin(sf::Vector2f(player_size /2, player_size /2));
	stats.set_speed(3); // da� to jako parametr i ewentualnie da� metode setColor

	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(player_shape.getPosition());
		//std::cout << "Player_constructor: " << circle->getPosition().x << " " << circle->getPosition().y << "\n";
	}
	

}


Player::~Player()
{
}


void Player::setPosition(double x, double y)
{
	player_shape.setPosition(x, y);
	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(player_shape.getPosition());
	//	std::cout << "Player_setPosition: " << circle->getPosition().x << " " << circle->getPosition().y << "\n";
	}
}

void Player::setPosition(sf::Vector2f position)
{
	player_shape.setPosition(position);
}

void Player::setRotation(double rot)
{
	player_shape.setRotation(rot);
}

void Player::move_to_mouse(int directionX, int directionY)
{
	sf::Vector2f playerPosition = player_shape.getPosition();
	float rotation = player_shape.getRotation();
	float	sinus = sin((rotation * M_PI) / 180.0f);
	float	cosinus = cos((rotation * M_PI) / 180.0f);
	//std::cout << sinus << " " << cosinus << std::endl;
	if (directionX == -1) //A
		player_shape.move(stats.get_speed()*(cosinus), stats.get_speed()*(sinus));
	else if(directionX == 1) //D
		player_shape.move(-stats.get_speed()*(cosinus), -stats.get_speed()*sinus);
	if (directionY == 1) //S
		player_shape.move(stats.get_speed()*(-sinus), stats.get_speed()*cosinus);
	else if (directionY == -1)//W
		player_shape.move(stats.get_speed()*sinus, stats.get_speed()*(-cosinus));
		//player_shape.setPosition(playerPosition.x + stats.get_speed()*sinus, playerPosition.y + stats.get_speed()*(-cosinus));
		//dzia�a tak samo jak player_shape.move(stats.get_speed()*sinus, stats.get_speed()*(-cosinus));

	
}

void Player::move(int directionX, int directionY)
{
	player_shape.move(stats.get_speed()*(directionX), stats.get_speed()*(directionY));

	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(player_shape.getPosition());
		//std::cout << "Player_movement: " << circle->getPosition().x << " " << circle->getPosition().y << "\n";
	}
}

void Player::rotate(sf::Vector2f mousePosition)
{
	float rotation = 0;
	float	Dx = sqrt((mousePosition.x - player_shape.getPosition().x)*(mousePosition.x - player_shape.getPosition().x)),
			Dy = sqrt((mousePosition.y - player_shape.getPosition().y)*(mousePosition.y - player_shape.getPosition().y));
	if (Dx == 0) rotation = 0;
	if (Dx != 0 && Dy != 0)
	{
		if (player_shape.getPosition().x < mousePosition.x && player_shape.getPosition().y > mousePosition.y)
			rotation = ((atan2f(Dy, Dx) * -360) / M_PI) / 2 + 90;
		else if (player_shape.getPosition().x < mousePosition.x && player_shape.getPosition().y < mousePosition.y)
			rotation = ((atan2f(-1 * Dy, Dx) * -360) / M_PI) / 2 + 90;
		else if (player_shape.getPosition().x > mousePosition.x && player_shape.getPosition().y > mousePosition.y)
			rotation = ((atan2f(-1 * Dx, Dy) * 360) / M_PI) / 2;
		else if (player_shape.getPosition().x > mousePosition.x && player_shape.getPosition().y < mousePosition.y)
			rotation = ((atan2f(-1 * Dy, Dx) * 360) / M_PI) / 2 + 270;
	}

	player_shape.setRotation(rotation);


}

sf::Vector2f Player::getPosition()
{
	return player_shape.getPosition();
}

sf::RectangleShape Player::getShape()
{
	return player_shape;
}

void Player::onCollision(ColidableObject* object)
{

	if (Ball* ball = dynamic_cast<Ball*>(object))
	{
		std::cout << "Ball hitted player\n";

		//obs�uga kolizji z ball'em
	}
	else if (Trap* trap = dynamic_cast<Trap*>(object))
	{
		std::cout << "Trap hitted player\n";
		//obs�uga kolizji z trap'em
	}
}