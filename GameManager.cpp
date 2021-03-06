#include "GameManager.h"
#include "Spell_Headers\Trap.h"
#include "Spell_Headers\Shield.h"
#include "Spell_Headers\Ball.h"
#include "Collider_Headers\ColidableObject.h"
#include <iomanip>
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

	if (local_shield.getActiveStatus())
		main_window->draw(local_shield.getShape());
	if (non_local_shield.getActiveStatus())
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && main_window->hasFocus()) {
		directionX = -1;
		step_sound.play();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && main_window->hasFocus()){
		directionX = 1;
	step_sound.play();
}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && main_window->hasFocus()){
		directionY = -1;
	step_sound.play();
}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && main_window->hasFocus()){
		directionY = 1;
	step_sound.play();
}

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


	balls_vector.erase(std::remove_if(balls_vector.begin(), balls_vector.end(), [](Ball b) { return !b.getActiveStatus(); }), balls_vector.end());
	trap_vector.erase(std::remove_if(trap_vector.begin(), trap_vector.end(), [](Trap t) { return t.has_ended(); }), trap_vector.end());
	if (local_shield_timer.getElapsedTime().asSeconds() > 15.0f)
	{
		local_shield.deactivate();
	}
	if (non_local_shield_timer.getElapsedTime().asSeconds() > 15.0f)
	{
		non_local_shield.deactivate();
	}
	if (non_local_shield.getActiveStatus())
	{
		sf::Vector2f pos = non_local_player->getPosition();
		non_local_shield.setPosition(pos.x, pos.y);
	}
	collision_handler.clear();
	for (int i = 0; i < balls_vector.size(); i++)
		collision_handler.add_object(balls_vector[i]);
	for (int i = 0; i < trap_vector.size(); i++)
		collision_handler.add_object(trap_vector[i]);

	collision_handler.add_object(*non_local_player);
	collision_handler.add_object(*local_player);

	collision_handler.check_objects_which_colide();

	if (std::find(vector.begin(), vector.end(), 5) != vector.end()) local_player->getPlayerStats()->set_speed(3);
	if (std::find(vector.begin(), vector.end(), 1) != vector.end()) local_player->getPlayerStats()->set_base_dmg(base_dmg);
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
				ball_sound.play();
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
				trap_sound.play();
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
		if (active_spell == 3 && !local_shield.getActiveStatus())
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
				local_shield.setActiveStatus(true);
				local_shield_timer.restart();
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

GameManager::GameManager(NetworkHandler * network, sf::RenderWindow & window, Tree& tree)
{
	this->main_window = &window;
	//Do rysowania znaków
	Pattern = Pattern_management(WINDOW_WIDTH, WINDOW_HEIGHT); //new empty array of points connected to form a line

	game_is_running = true;
	is_pattern_drawn = false;
	players_initialization();

	separator = new Separator();
	//ustawienie stanu pocz¹tkowego gry(menu logowania)
	current_game_state = GAME_IN_PROGRES;
	//want_to_run_with_connection_to_server = true;
	//inicjalizacja obs³ugi sieci
	network_handler = network;
	this->tree = &tree;

	background_tex = sf::Texture();
	background_tex.loadFromFile("Graphics/Screens/Battleground1_bg.png");
	background = sf::Sprite();
	background.setTexture(background_tex);

	sounds = SoundManager();


	ball.loadFromFile("Sounds/Ballsend.wav");
	ball_sound.setBuffer(ball);
	trap.loadFromFile("Sounds/Trap_set.wav");
	trap_sound.setBuffer(trap);
	step.loadFromFile("Sounds/Steps.wav");
	step_sound.setBuffer(step);

	local_shield.setActiveStatus(false);
	non_local_shield.setActiveStatus(false);
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
		managePattern();

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
	check_win_condition();
	draw();
}

void GameManager::check_win_condition()
{
	if (local_player->getPlayerStats()->get_current_health() < 0)
	{
		player_won = false;
		end_game = true;
	}
	if (non_local_player->getPlayerStats()->get_current_health() < 0)
	{
		player_won = true;
		end_game = true;
	}
}

void GameManager::players_initialization()
{

	stats = new Player_stats(100 , 100, 100, 100, 10, 100, 100, 100, 100, "Valium1");
	stats1 = new Player_stats(100, 100, 100, 100, 10, 100, 100, 100, 100, "Valium2");
	local_player = new Player(sf::Color::Red, PLAYER_SIZE, "Graphics/Character/player.png", stats);
	hud = new Player_Hud(stats, stats1);
	local_status_hud = new Status_Hud();
	non_local_status_hud = new Status_Hud();
	current_spell_hud = new Current_Spell_Hud();
	int local_player_position_x = (WINDOW_WIDTH / 2);
	int local_player_position_y = (WINDOW_HEIGHT - PLAYER_SIZE);
	sf::Vector2f position(local_player_position_x, local_player_position_y);
	local_player->setPosition(position);

	non_local_player = new Player(sf::Color::Blue, PLAYER_SIZE,"Graphics/Character/enemy.png", stats1);
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
	playerPosition.x = WINDOW_WIDTH - PLAYER_SIZE/2 - recived_position_x;
	playerPosition.y = WINDOW_HEIGHT - PLAYER_SIZE/2 - recived_position_y;
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
		float trap_position_x = traps_to_send[i].getPosition().x;
		float trap_position_y = traps_to_send[i].getPosition().y;
		int trap_element = (int)traps_to_send[i].getTrapStats().get_element();

		packet_to_send << trap_position_x << trap_position_y << trap_element;
	}
	traps_to_send.clear();
}

void GameManager::unpack_trap_objects(sf::Packet & recived_packet)
{
	int recive_traps_size;
	recived_packet >> recive_traps_size;

	for (int i = 0; i < recive_traps_size; i++)
	{
		float trap_position_x;
		float trap_position_y;
		int trap_element;
		float radius = 5;
		float duration = 10.0f;
		recived_packet >> trap_position_x >> trap_position_y >> trap_element;
		trap_vector.push_back(Trap(sf::Vector2i(WINDOW_WIDTH - trap_position_x, WINDOW_HEIGHT - trap_position_y), Trap_stats((Element)trap_element, duration, radius)));
	}
}

void GameManager::pack_shield_object(sf::Packet & packet_to_send)
{
	bool active_shield = local_shield.getActiveStatus();
	std::cout << "sending active_shield " << active_shield << "\n";
	if (local_shield.getActiveStatus())
	{
		int shield_element = (int)local_shield.get_statistics().get_element();
		packet_to_send >> active_shield >> shield_element;
	}
}

void GameManager::unpack_shield_object(sf::Packet & recived_packet)
{
	bool active_shield;
	
	recived_packet >> active_shield;
	std::cout << "recive active_shield " << active_shield << "\n";
	if (active_shield)
	{
		int shield_element;
		recived_packet >> shield_element;
		std::cout << "Shield recived " << shield_element << "\n";
		non_local_shield = Shield(non_local_player->getPosition(), Shield_stats((Element)shield_element, 30.0f, SHIELD_RADIUS));
		non_local_shield.setActiveStatus(true);
		non_local_shield_timer.restart();
	}

}

void GameManager::pack_all_and_send()
{
	sf::Packet packet_to_send;
	packet_to_send << 1;
	pack_player(packet_to_send);
	pack_ball_objects(packet_to_send);
	pack_trap_objects(packet_to_send);
	//pack_shield_object(packet_to_send);
	network_handler->send_packet(packet_to_send);
}

void GameManager::recive_and_unpack_all()
{
	sf::Packet recived_packet = network_handler->recive_packet();

	unpack_player(recived_packet);
	unpack_ball_objects(recived_packet);
	unpack_trap_objects(recived_packet);
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