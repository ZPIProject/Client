#include "GameManager.h"
#include "Spell_Headers\Trap.h"
#include "Spell_Headers\Shield.h"
#include "Spell_Headers\Ball.h"
#include "Collider_Headers\ColidableObject.h"
#include <iomanip>
#include <thread>
#include <cmath>
#include <cstdio>
#include <limits>

void GameManager::change_game_state(Game_states state)
{
	current_game_state = state;
}

void GameManager::draw()
{
	main_window->clear();

	main_window->draw(background);
	//main_window->draw(separator->getShape());

	main_window->draw(local_player->getShape());
	main_window->draw(non_local_player->getShape());

	if (current_spell_hud->getElement() > 0 && current_spell_hud->getSpell() > 0) {
		current_spell_hud->setPosition(sf::Vector2f(sf::Mouse::getPosition(*main_window).x + 10, sf::Mouse::getPosition(*main_window).y + 10));
		main_window->draw(current_spell_hud->getText());
	}

	if (local_player->getCurrent_status().size() != 0) {
		std::vector<StatusStringHud> status_local = local_player->getCurrent_status();
		std::string result;
		std::vector<std::string> vector;
		for (StatusStringHud s : status_local) {
			if (!s.has_ended()) vector.push_back(s.getName());
		}
		for (std::string s : vector) {
			result += s + "\n";
		}
		local_status_hud->setPosition(sf::Vector2f(local_player->getPosition().x - PLAYER_SIZE / 2, local_player->getPosition().y - 5 - status_local.size() * 13));
		local_status_hud->setText(result);
		main_window->draw(local_status_hud->getText());
	}

	if (non_local_player->getCurrent_status().size() != 0) {
		std::vector<StatusStringHud> status_non_local = non_local_player->getCurrent_status();
		std::string result;
		std::vector<std::string> vector;
		for (StatusStringHud s : status_non_local) {
			if (!s.has_ended()) vector.push_back(s.getName());
		}
		for (std::string s : vector) {
			result += s + "\n";
		}
		non_local_status_hud->setPosition(sf::Vector2f(non_local_player->getPosition().x - PLAYER_SIZE / 2, non_local_player->getPosition().y - 5 - status_non_local.size() * 13));
		non_local_status_hud->setText(result);

		main_window->draw(non_local_status_hud->getText());
	}

	std::vector<sf::Text> texts = hud->getTexts();

	for (sf::Text t : texts) {
		main_window->draw(t);
	}

	for (int i = 0; i < balls_vector.size(); i++)
	{
		main_window->draw(balls_vector[i].getShape());
		balls_vector[i].move();
	}

	for (int i = 0; i < trap_vector.size(); i++)
	{
		main_window->draw(trap_vector[i].getShape());
	}

	if (!local_shield.has_ended())
		main_window->draw(local_shield.getShape());
	if (!non_local_shield.has_ended())
		main_window->draw(non_local_shield.getShape());

	main_window->draw(Pattern.getArray());
	main_window->display();
}

void GameManager::event_handler()
{
	sf::Event event;
	while (main_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			main_window->close();
		else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && main_window->hasFocus()) {
			is_pattern_drawn = true;
		}
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right && main_window->hasFocus()) {
			is_pattern_drawn = false;
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape && main_window->hasFocus())
			{
				player_won = false;
				exited_from_game = true;
				end_game = true;
			}
		}
	}
}

void GameManager::logic_handler()
{
	hud->setLocalMpText(hud->Convert(stats->get_current_mana()) + "/" + hud->Convert(stats->get_MAX_mana()) + ":MP");
	hud->setLocalHpText(hud->Convert(stats->get_current_health()) + "/" + hud->Convert(stats->get_MAX_health()) + ":HP");
	hud->setOpponentHpText("HP:" + hud->Convert((stats1->get_current_health() / stats1->get_MAX_health()) * 100) + "%");

	local_player->incMana();

	std::vector<StatusStringHud> status_local = local_player->getCurrent_status();
	std::vector<int> vector;
	for (StatusStringHud s : status_local) {
		if (!s.has_ended()) {
			int id = s.getId();
			if (id == 1) vector.push_back(1);		//Debuff
			else if (id == 4) { vector.push_back(2); s.setDuration(1); }//Stun
			else if (id == 5) vector.push_back(3);//Damage
			else if (id == 6) vector.push_back(4);//Confusion
			else if (id == 9) vector.push_back(5);//Slow
		}
	}

	if (std::find(vector.begin(), vector.end(), 5) != vector.end()) local_player->getPlayerStats()->set_speed(1.5);
	float base_dmg = local_player->getPlayerStats()->get_base_dmg();
	if (std::find(vector.begin(), vector.end(), 1) != vector.end()) local_player->getPlayerStats()->set_base_dmg(base_dmg*0.75);
	if (std::find(vector.begin(), vector.end(), 3) != vector.end()) local_player->getPlayerStats()->set_current_health(local_player->getPlayerStats()->get_current_health() - 0.01);

	current_spell_hud->setElement(active_element);
	current_spell_hud->setSpell(active_spell);

	sf::Mouse mouse;
	int directionX = 0, directionY = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && main_window->hasFocus())
		directionX = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && main_window->hasFocus())
		directionX = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && main_window->hasFocus())
		directionY = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && main_window->hasFocus())
		directionY = 1;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && main_window->hasFocus() ) {
		cast_spell();
	}

	local_player->rotate(main_window->mapPixelToCoords(sf::Mouse::getPosition(*main_window)));

	if (std::find(vector.begin(), vector.end(), 4) != vector.end()) {
		directionX = -1 * directionX;
		directionY = -1 * directionY;
	}

	if (std::find(vector.begin(), vector.end(), 2) != vector.end());
	else {
		if (local_player->getPosition().x + directionX >= PLAYER_SIZE / 2 &&
			local_player->getPosition().x + directionX <= WINDOW_WIDTH - PLAYER_SIZE / 2);
		else directionX = 0;
		if (local_player->getPosition().y + directionY <= WINDOW_HEIGHT - PLAYER_SIZE / 2 - 1 &&
			local_player->getPosition().y + directionY >= WINDOW_HEIGHT / 2 + SEPARATOR_HEIGHT / 2 + PLAYER_SIZE / 2 + 2);
		else directionY = 0;
		local_player->move(directionX, directionY);
		local_shield.move(local_player->getPosition());
	}

	managePattern();

	balls_vector.erase(std::remove_if(balls_vector.begin(), balls_vector.end(), [](Ball b) { return !b.getActiveStatus(); }), balls_vector.end());
	trap_vector.erase(std::remove_if(trap_vector.begin(), trap_vector.end(), [](Trap t) { return t.has_ended(); }), trap_vector.end());

	//collision_handler.clear();
	/*for (int i = 0; i < balls_vector.size(); i++)
		collision_handler.add_object(balls_vector[i]);
	for (int i = 0; i < trap_vector.size(); i++)
		collision_handler.add_object(trap_vector[i]);

	collision_handler.add_object(*non_local_player);
	//collision_handler.add_object(*local_player);

	if (!local_shield.has_ended())
		collision_handler.add_object(local_shield);

	if (!non_local_shield.has_ended())
		collision_handler.add_object(non_local_shield);

	collision_handler.check_objects_which_colide();

	if (std::find(vector.begin(), vector.end(), 5) != vector.end()) local_player->getPlayerStats()->set_speed(3);
	if (std::find(vector.begin(), vector.end(), 1) != vector.end()) local_player->getPlayerStats()->set_base_dmg(base_dmg);*/
}

void GameManager::managePattern()
{
	if (sf::Mouse::getPosition(*main_window).x >= 0 && sf::Mouse::getPosition(*main_window).y >= 0 && sf::Mouse::getPosition(*main_window).x < main_window->getSize().x, sf::Mouse::getPosition(*main_window).y < main_window->getSize().y) {
		double P_Double = Pattern.manage_pattern(sf::Mouse::getPosition(*main_window).x, sf::Mouse::getPosition(*main_window).y, is_pattern_drawn);
		if (P_Double>0)std::cout << P_Double << std::endl;
		setActiveSpellData(P_Double);
	}
}

void GameManager::cast_spell()
{
	if (active_spell>0 && active_element>0) {
		if (active_spell == 7 && ball_cooldown.getElapsedTime().asMilliseconds() > 500)
		{
			if (local_player->getMana() >= 10)
			{
				local_player->decMana(10);
				//Ball_stats configuration
				float damage = local_player->getPlayerStats()->get_base_dmg();
				if (draw_precision_spell > 0.95 && draw_precision_element > 0.95) damage *= 1.5;
				else if (draw_precision_spell > 0.9 || draw_precision_element > 0.9) damage *= 1.2;
				else if (draw_precision_spell < 0.7 && draw_precision_element > 0.7) damage *= 0.8;

				float speed = PROJECTILE_STANDARD_SPEED;
				float radius = 10;
				Element e = (Element)active_element;

				sf::Vector2f fixed_ball_position = sf::Vector2f(local_player->getPosition().x, local_player->getPosition().y - 50);
				Ball ball = Ball(fixed_ball_position, Ball_stats(e, damage, speed, radius));
				ball.on_cast_set_direction(local_player->getShape().getRotation());
				balls_vector.push_back(ball);
				balls_to_send.push_back(ball);
				std::cout << "ball_to_send size " << balls_to_send.size() << "\n";
				ball_cooldown.restart();
			}
		}
		if (active_spell == 8 && trap_cooldown.getElapsedTime().asMilliseconds() > 500)
		{
			if (local_player->getMana() >= 40)
			{
				local_player->decMana(40);

				//Trap_stats configuration
				Element e = (Element)active_element;
				float radius = 5;
				float duration = 10.0f;

				if (draw_precision_spell > 0.95 && draw_precision_element > 0.95) { radius *= 1.5; duration *= 1.2; }
				else if (draw_precision_spell > 0.9 || draw_precision_element > 0.9) radius *= 1.2;
				else if (draw_precision_spell < 0.7 && draw_precision_element > 0.7) duration *= 0.8;

				Trap trap = Trap(sf::Mouse::getPosition(*main_window), Trap_stats(e, duration, radius));
				trap_vector.push_back(trap);
				traps_to_send.push_back(trap);

				trap_cooldown.restart();
			}
		}
		if (active_spell == 3 && !local_shield.has_ended())
		{
			if (local_player->getMana() >= 20)
			{
				local_player->decMana(20);

				Element e = (Element)active_element;
				float radius = SHIELD_RADIUS;
				float duration = 30.0f;
				if (draw_precision_spell > 0.95 && draw_precision_element > 0.95) duration *= 1.5;
				else if (draw_precision_spell > 0.90 && draw_precision_element > 0.90) duration *= 1.2;
				else if (draw_precision_spell < 0.70 && draw_precision_element > 0.70) duration *= 0.8;

				local_shield = Shield(local_player->getPosition(), Shield_stats(e, duration, radius));
			}
		}
		active_element = 0;
		draw_precision_spell = draw_precision_element = 0;
	}

}

void GameManager::setActiveSpellData(double value)
{
	if (fmod(value, 1) >= 0.70) {
		if (value > 0) {
			if ((int)value == 2 || (int)value == 3 || (int)value == 7 || (int)value == 8) {
				active_spell = (int)value;
				draw_precision_spell = fmod(value, 1);

			}
			else {
				active_element = (int)value;
				draw_precision_element = fmod(value, 1);

			}
			std::cout << draw_precision_spell << " " << draw_precision_element << std::endl;
			std::cout << active_spell << " " << active_element << std::endl;
		}
	}
}

GameManager::GameManager(NetworkHandler * network, sf::RenderWindow & window)
{
	this->main_window = &window;
	//Do rysowania znaków
	Pattern = Pattern_management(WINDOW_WIDTH, WINDOW_HEIGHT); //new empty array of points connected to form a line

	game_is_running = true;
	is_pattern_drawn = false;
	//players_initialization();

	separator = new Separator();
	//ustawienie stanu pocz¹tkowego gry(menu logowania)
	current_game_state = GAME_IN_PROGRES;
	//want_to_run_with_connection_to_server = true;
	//inicjalizacja obs³ugi sieci
	network_handler = network;

	background_tex = sf::Texture();
	background_tex.loadFromFile("Graphics/Screens/Battleground1_bg.png");
	background = sf::Sprite();
	background.setTexture(background_tex);
}
GameManager::~GameManager()
{
	delete network_handler;
	delete local_player;
	delete non_local_player;
}
void GameManager::run()
{
	while (!end_game)
	{

		game_in_progress();
	}
}
void GameManager::game_in_progress()
{

	event_handler();
	if (frame_rate_controller.getElapsedTime().asMilliseconds() > 20)
	{
		logic_handler();
		frame_rate_controller.restart();
	}
	if (want_to_run_with_connection_to_server)
	{
		pack_all_and_send();
		recive_and_unpack_all();
	}
	
	check_if_player_has_left();
	draw();
}

sf::Packet GameManager::pack_maxHP()
{
	sf::Packet packet_to_send;
	packet_to_send << 1 << 100 + tree->getVitality() * 5;
	return packet_to_send;
}

int GameManager::unpack_maxHP(sf::Packet recived_packet)
{
	int packet_type = 0;
	int maxHP = 0;
	recived_packet >> packet_type >> maxHP;
	return maxHP;
}

void GameManager::players_initialization()
{
	stats = new Player_stats(100 + tree->getVitality() * 5, 100 + tree->getVitality() * 5, 100 + tree->getWisdom() * 5, 100 + tree->getWisdom() * 5, 10 + tree->getKnowledge() * 0.1, 100, 100, 100, 100, "Valium1");
	//stats1 = new Player_stats(100, 100, 100, 100, 10, 100, 100, 100, 100, "Valium2");
	local_player = new Player(sf::Color::Red, PLAYER_SIZE, stats);
	hud = new Player_Hud(stats, stats1);
	local_status_hud = new Status_Hud();
	non_local_status_hud = new Status_Hud();
	current_spell_hud = new Current_Spell_Hud();
	int local_player_position_x = (WINDOW_WIDTH / 2);
	int local_player_position_y = (WINDOW_HEIGHT - 2 * local_player->getShape().getRadius());
	local_player->setPosition(local_player_position_x, local_player_position_y);

	non_local_player = new Player(sf::Color::Blue, PLAYER_SIZE, stats1);
	non_local_player->setPosition(400, 160);


	//dodanie obiektów do collision_handler;
	//collision_handler.add_object(*non_local_player);
	//collision_handler.add_object(*local_player);
}

void GameManager::pack_player(sf::Packet& packet_to_send)
{
	packet_to_send << local_player->getPosition().x << local_player->getPosition().y << local_player->getShape().getRotation();
}

void GameManager::unpack_player(sf::Packet& recived_packet)
{
	int packet_type;
	float recived_position_x, recived_position_y, recived_rotation;
	recived_packet >> packet_type >> recived_position_x >> recived_position_y >> recived_rotation;
	//std::cout << recived_position_x << " " << recived_position_y << " " << recived_rotation;
	sf::Vector2f playerPosition;
	playerPosition.x = WINDOW_WIDTH - non_local_player->getShape().getRadius() / 2 - recived_position_x;
	playerPosition.y = WINDOW_HEIGHT - non_local_player->getShape().getRadius() / 2 - recived_position_y;
	non_local_player->setPosition(playerPosition);
	non_local_player->setRotation(recived_rotation);

}

void GameManager::pack_ball_objects(sf::Packet & packet_to_send)
{
	int balls_to_send_size = balls_to_send.size();
//	std::cout << balls_to_send.size() << "\n";
	packet_to_send << balls_to_send_size;
	for (int i = 0; i < balls_to_send_size; i++)
	{
		float start_position_x = balls_to_send[i].getPosition().x;
		float start_position_y = balls_to_send[i].getPosition().y;
		float rotation = local_player->getShape().getRotation();
		float speed = balls_to_send[i].get_speed();
		float radius = balls_to_send[i].get_radius();
		int element = (int)balls_to_send[i].get_element();
		//std::cout << "sending: " << start_position_x << " " << start_position_y << " " << rotation << " " << speed << " " << radius << " " << element;
		packet_to_send << start_position_x << start_position_y << rotation << speed << radius << element;
	}
	balls_to_send.clear();
}

void GameManager::unpack_ball_objects(sf::Packet & recived_packet)
{
	int recived_balls_size;
	recived_packet >> recived_balls_size;
	for (int i = 0; i < recived_balls_size; i++)
	{
		float	start_position_x,
			start_position_y,
			speed,
			radius,
			rotation;
		int		element;
		recived_packet >> start_position_x >> start_position_y >> rotation >> speed >> radius >> element;
		sf::Vector2f fixed_ball_position;
		fixed_ball_position.x = WINDOW_WIDTH - PLAYER_SIZE / 2 - start_position_x;
		fixed_ball_position.y = WINDOW_HEIGHT - PLAYER_SIZE / 2 - start_position_y;
		//std::cout << "reciving: " << start_position_x << " " << start_position_y << " " << rotation << " " << speed << " " << radius << " " << element << "\n";
		Ball b = Ball(fixed_ball_position, Ball_stats(Element(element), 10, speed, radius));
		b.on_cast_set_direction(non_local_player->getShape().getRotation());
		b.setSpeed(-speed);
		balls_vector.push_back(b);

	}
}

void GameManager::pack_trap_objects(sf::Packet & packet_to_send)
{
	int traps_to_send_size = traps_to_send.size();
	
	packet_to_send << traps_to_send_size;
	for (int i = 0; i < traps_to_send_size; i++)
	{

	}
}

void GameManager::unpack_trap_objects(sf::Packet & recived_packet)
{
}

void GameManager::pack_shield_object(sf::Packet & packet_to_send)
{
	int element = (int)local_shield.get_statistics().get_element();
	float radius = local_shield.get_statistics().get_radius();
	//std::cout << "radius: " << radius << " element: " << element << "\n";
	packet_to_send << local_shield.getPosition().x << local_shield.getPosition().y << element << radius;
}

void GameManager::unpack_shield_object(sf::Packet & recived_packet)
{
	float shield_position_x, shield_position_y, shield_radius;
	int element;

	recived_packet >> shield_position_x >> shield_position_y >> element >> shield_radius;

	Element e = (Element)element;
	sf::Vector2f shield_position;
	shield_position.x = WINDOW_WIDTH - shield_position_x;
	shield_position.y = WINDOW_HEIGHT - shield_position_y;
	std::cout << "shield recived: " << element << " " << shield_radius << "\b";
	non_local_shield = Shield(shield_position, Shield_stats(e, 10, SHIELD_RADIUS));

}

void GameManager::pack_all_and_send()
{
	sf::Packet packet_to_send;
	packet_to_send << 1;
	pack_player(packet_to_send);
	pack_ball_objects(packet_to_send);
	//pack_shield_object(packet_to_send);
	network_handler->send_packet(packet_to_send);
}

void GameManager::recive_and_unpack_all()
{
	sf::Packet recived_packet = network_handler->recive_packet();

	unpack_player(recived_packet);
	unpack_ball_objects(recived_packet);
	//unpack_shield_object(recived_packet);
}

void GameManager::check_if_player_has_left()
{
	sf::Packet game_has_ended_packet;
	game_has_ended_packet << 1 << exited_from_game;
	network_handler->send_packet(game_has_ended_packet);

	
	sf::Packet recive_end_game_packet;
	recive_end_game_packet = network_handler->recive_packet();
	
	bool opponent_has_left;
	int packet_type;
	recive_end_game_packet >> packet_type >> opponent_has_left;

	if (opponent_has_left)
	{
		player_won = opponent_has_left;
		end_game = true;
	}
}