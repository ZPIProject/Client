#include "Player.h"
#include "Spell_Headers\Ball.h"
#include "Spell_Headers\Shield.h"
#include "Spell_Headers\Trap.h"


Player::Player(sf::Color players_color, float player_size, Player_stats* stts) : ColidableObject(new CircleCollider(player_size/2)),stats(stts)
{
	player_shape.setPosition(sf::Vector2f(0, 0));
	player_shape.setFillColor(players_color);
	player_shape.setRadius(player_size/2);
	player_shape.setOrigin(sf::Vector2f(player_size /2, player_size /2));
	stats = stts;
	stats->set_speed(3); // daæ to jako parametr i ewentualnie daæ metode setColor

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
		player_shape.move(stats->get_speed()*(cosinus), stats->get_speed()*(sinus));
	else if(directionX == 1) //D
		player_shape.move(-stats->get_speed()*(cosinus), -stats->get_speed()*sinus);
	if (directionY == 1) //S
		player_shape.move(stats->get_speed()*(-sinus), stats->get_speed()*cosinus);
	else if (directionY == -1)//W
		player_shape.move(stats->get_speed()*sinus, stats->get_speed()*(-cosinus));
		//player_shape.setPosition(playerPosition.x + stats.get_speed()*sinus, playerPosition.y + stats.get_speed()*(-cosinus));
		//dzia³a tak samo jak player_shape.move(stats.get_speed()*sinus, stats.get_speed()*(-cosinus));
}

void Player::move(int directionX, int directionY)
{
	player_shape.move(stats->get_speed()*(directionX), stats->get_speed()*(directionY));

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

sf::CircleShape Player::getShape()
{
	return player_shape;
}

void Player::onCollision(ColidableObject* object)
{

	if (Ball* ball = dynamic_cast<Ball*>(object))
	{
		std::cout << "Ball hitted player\n";

		//obs³uga kolizji z ball'em
	}
	else if (Trap* trap = dynamic_cast<Trap*>(object))
	{
		//std::cout << "Trap hitted player\n";
		//obs³uga kolizji z trap'em
		int element = (int)trap->getTrapStats().get_element();
		switch (element)
		{
		case 1:
			if (!hasElement(current_status, 1)) current_status.push_back(StatusStringHud(1, "Debuff", 15));
			else current_status[getIdInVectorStatus(1)].restartDuration();
			break;
		case 4:
			if (!hasElement(current_status, 4)) current_status.push_back(StatusStringHud(4, "Stun", 15));
			else current_status[getIdInVectorStatus(4)].restartDuration();
			break;
		case 5:
			if (!hasElement(current_status, 5)) current_status.push_back(StatusStringHud(5, "Damage", 15));
			else current_status[getIdInVectorStatus(5)].restartDuration();
			break;
		case 6:
			if (!hasElement(current_status, 6)) current_status.push_back(StatusStringHud(6, "Confusion", 15));
			else current_status[getIdInVectorStatus(6)].restartDuration();
			break;
		case 9:
			if (!hasElement(current_status, 9)) current_status.push_back(StatusStringHud(9, "Slow", 15));
			else current_status[getIdInVectorStatus(9)].restartDuration();
			break;
		default:
			break;
		}
	}
}
std::vector<StatusStringHud> Player::getCurrent_status()
{
	return current_status;
}

int Player::getIdInVectorStatus(int id)
{
	int result;
	for (int i = 0; i < current_status.size(); i++) {
		if (current_status[i].getId() == id) result = i;
	}
	return result;
}

bool Player::hasElement(std::vector<StatusStringHud> vec, int id) {
	bool result = false;
	std::vector<int> vector;
	for (StatusStringHud s : vec) {
		vector.push_back(s.getId());
	}
	for (int i : vector) {
		if (i == id) result = true;
	}
	return result;
}

void Player::decMana(float val)
{
	stats->set_current_mana(stats->get_current_mana() - val);
}

void Player::incMana()
{
	stats->set_current_mana(stats->get_current_mana() + 0.05);
	if (stats->get_current_mana() > stats->get_MAX_mana())stats->set_current_mana(stats->get_MAX_mana());
}

