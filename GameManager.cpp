#include "GameManager.h"
#include "Trap.h"
#include "Shield.h"
#include "Ball.h"
#include "ColidableObject.h"

void GameManager::change_game_state(Game_states state)
{
	current_game_state = state;
}

void GameManager::draw()
{
	main_window.clear(sf::Color(50,120,50));
	/*std::vector<ColidableObject*> objects_to_draw = collision_handler.get_all_objects();
	for (int i = 0; i < objects_to_draw.size(); i++)
	{
		if (Player* player = dynamic_cast<Player*>(objects_to_draw[i]))
		{
			main_window.draw(player->getShape());
		}
		if (Ball* ball = dynamic_cast<Ball*>(objects_to_draw[i]))
		{
			main_window.draw(ball->getShape());
		}
		if (Shield* shield = dynamic_cast<Shield*>(objects_to_draw[i]))
		{
			main_window.draw(shield->getShape());
		}
		if (Trap* trap = dynamic_cast<Trap*>(objects_to_draw[i]))
		{
			main_window.draw(trap->getShape()); 
		}

	}*/

	
	main_window.draw(local_player->getShape());
	main_window.draw(non_local_player->getShape());

	for (int i = 0; i < balls_vector.size(); i++)
	{
		main_window.draw(balls_vector[i].getShape());
		balls_vector[i].move();
	}

	for (int i = 0; i < trap_vector.size(); i++)
	{
		main_window.draw(trap_vector[i].getShape());
	}

	if(!local_shield.has_ended())
		main_window.draw(local_shield.getShape());
	if(!non_local_shield.has_ended())
		main_window.draw(non_local_shield.getShape());

	main_window.draw(Pattern.getArray());
	main_window.display();
}

void GameManager::event_handler()
{
	sf::Event event;
	while (main_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			main_window.close();
		else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){	
			is_pattern_drawn = true;		
		}
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
			is_pattern_drawn = false;

		}
	}
}

void GameManager::logic_handler()
{
	sf::Mouse mouse;
	int directionX = 0, directionY = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		directionX = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		directionX = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		directionY = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		directionY = 1;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		active_spell = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		active_spell = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		active_spell = 3;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		active_element = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
	{
		active_element = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{
		active_element = 2;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		active_element = 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		active_element = 4;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (active_spell == 1 && ball_cooldown.getElapsedTime().asMilliseconds() > 500)
		{
			//Ball_stats configuration
			float damage = 10;
			float speed = PROJECTAIL_STANDARD_SPEED;
			float radius = 10;
			Element e = (Element)active_element;

			sf::Vector2f fixed_ball_position = sf::Vector2f(local_player->getPosition().x, local_player->getPosition().y - 50);
			Ball ball = Ball(fixed_ball_position, Ball_stats(e,damage,speed,radius));
			ball.on_cast_set_direction(local_player->getShape().getRotation());
			balls_vector.push_back(ball);
			balls_to_send.push_back(ball);

			ball_cooldown.restart();
		}
		if (active_spell == 2 && trap_cooldown.getElapsedTime().asMilliseconds() > 500)
		{
			//Trap_stats configuration
			Element e = (Element)active_element;
			float radius = 5;
			float duration = 10.0f;
		
			Trap trap = Trap(sf::Mouse::getPosition(main_window), Trap_stats(e, duration, radius));
			trap_vector.push_back(trap);
			traps_to_send.push_back(trap);

			trap_cooldown.restart();
		}
		if (active_spell == 3 && !local_shield.has_ended())
		{
			Element e = (Element)active_element;
			float radius = SHIELD_RADIUS;
			float duration = 30.0f;

			local_shield = Shield(local_player->getPosition(), Shield_stats(e,duration,radius));

		}
	}

	local_player->rotate(main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window)));
	local_player->move(directionX, directionY);
	local_shield.move(local_player->getPosition());
	managePattern();


	balls_vector.erase(std::remove_if(balls_vector.begin(), balls_vector.end(), [](Ball b) { return !b.getActiveStatus(); }), balls_vector.end());
	trap_vector.erase(std::remove_if(trap_vector.begin(), trap_vector.end(), [](Trap t) { return t.has_ended(); }), trap_vector.end());

	collision_handler.clear();
	for (int i = 0; i < balls_vector.size(); i++)
		collision_handler.add_object(balls_vector[i]);
	for (int i = 0; i < trap_vector.size(); i++)
		collision_handler.add_object(trap_vector[i]);
	
	collision_handler.add_object(*non_local_player);
	collision_handler.add_object(*non_local_player);
	
	if(!local_shield.has_ended())
		collision_handler.add_object(local_shield);

	if (!non_local_shield.has_ended())
		collision_handler.add_object(non_local_shield);



	collision_handler.check_objects_which_colide();
}

void GameManager::managePattern()
{
	if (sf::Mouse::getPosition(main_window).x >= 0 && sf::Mouse::getPosition(main_window).y >= 0 && sf::Mouse::getPosition(main_window).x < main_window.getSize().x, sf::Mouse::getPosition(main_window).y < main_window.getSize().y) {
		double P_Double = Pattern.manage_pattern(sf::Mouse::getPosition(main_window).x, sf::Mouse::getPosition(main_window).y, is_pattern_drawn);
		if (P_Double > 0) std::cout << P_Double << std::endl;
	}


}

void GameManager::loging_menu()
{
	std::string ip_adress;
	std::string username;
	std::string password;

	std::cout << "Czy chcesz odpaliæ klienta z po³¹czeniem do serwera(tak) lub bez (nie) ";
	std::cin >> ip_adress;
	want_to_run_with_connection_to_server = ip_adress == "nie";
	if (want_to_run_with_connection_to_server)
	{
		std::cout << "\nPodaj ip serwera: ";
		std::cin >> ip_adress;
		std::cout << "Podaj username: ";
		std::cin >> username;
		std::cout << "Podaj haslo: ";
		std::cin >> password;


		if (network_handler->connect(ip_adress))
		{// tutaj sprawdzanie czy has³o zosta³o poprawnie wprowadzone i czy username jest z nim zgodne
			sf::Packet login_information_to_send;
			login_information_to_send << 0 << username << password; //0 = LOGIN_PACKET

			network_handler->send_packet(login_information_to_send);
			std::cout << "wys³ano pakiet\n";
			sf::Packet recived = network_handler->recive_packet();
			std::cout << "odebrano pakiet\n";
			bool logged;
			recived >> logged;
			std::cout << "logged:" << logged << "\n";
			if (logged)
			{
				change_game_state(GAME_IN_PROGRES); // tutaj powinno byc przejscie do connecting_to_server(), ale dla szybszego testowania jest od razu ³aczenie do gry
				main_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SignCaster", sf::Style::Close);
			}
		}
	}
	else
	{
		change_game_state(GAME_IN_PROGRES); // tutaj powinno byc przejscie do connecting_to_server(), ale dla szybszego testowania jest od razu ³aczenie do gry
		main_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SignCaster", sf::Style::Close);
	}
}
void GameManager::connecting_to_server()
{
}
void GameManager::main_menu()
{
}
void GameManager::connecting_to_game()
{
}
GameManager::GameManager()
{

	//Do rysowania znaków
	Pattern = Pattern_management(WINDOW_WIDTH, WINDOW_HEIGHT); //new empty array of points connected to form a line

	game_is_running = true;
	is_pattern_drawn = false;
	players_initialization();
	//ustawienie stanu pocz¹tkowego gry(menu logowania)
	current_game_state = LOGING_MENU;

	//inicjalizacja obs³ugi sieci
	network_handler = new NetworkHandler(PORT);
}
GameManager::~GameManager()
{
	delete network_handler;
	delete local_player;
	delete non_local_player;
}
void GameManager::run()
{
	while (game_is_running)
	{
		switch (current_game_state)
		{
		case LOGING_MENU:
			loging_menu();
			break;
		case CONNECTING_TO_SERVER:
			connecting_to_server();
			break;
		case MAIN_MENU:
			main_menu();
			break;
		case  CONNECTING_TO_GAME:
			connecting_to_game();
			break;
		case GAME_IN_PROGRES:
			game_in_progress();
			break;
		case  EXITING_GAME:
			exiting_game();
			break;
		case DISCONNECT:
			disconnect();
				break;



		}
	}
}
void GameManager::game_in_progress()
{
	
	event_handler();
	if (frame_rate_controller.getElapsedTime().asMilliseconds() > 10)
	{
		logic_handler();
		frame_rate_controller.restart();
	}
	if (want_to_run_with_connection_to_server)
	{
		pack_all_and_send();
		recive_and_unpack_all();
	}
	draw();
}

void GameManager::exiting_game()
{
}

void GameManager::disconnect()
{
}

void GameManager::players_initialization()
{
	local_player = new Player(sf::Color::Red, PLAYER_SIZE);
	int local_player_position_x = (WINDOW_WIDTH - local_player->getShape().getSize().x) / 2;
	int local_player_position_y = (WINDOW_HEIGHT - local_player->getShape().getSize().y);
	local_player->setPosition(local_player_position_x, local_player_position_y);

	non_local_player = new Player(sf::Color::Blue, PLAYER_SIZE);
	non_local_player->setPosition(400, 60);


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
	playerPosition.x = WINDOW_WIDTH - non_local_player->getShape().getSize().x / 2 - recived_position_x;
	playerPosition.y = WINDOW_HEIGHT - non_local_player->getShape().getSize().y / 2 - recived_position_y;
	non_local_player->setPosition(playerPosition);
	non_local_player->setRotation(recived_rotation);

}

void GameManager::pack_ball_objects(sf::Packet & packet_to_send)
{
	int balls_to_send_size = balls_to_send.size();
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
		packet_to_send << start_position_x << start_position_y << rotation << speed << radius << element << "\n";
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
		std::cout << "reciving: " << start_position_x << " " << start_position_y << " " << rotation << " " << speed << " " << radius << " " << element << "\n";
		Ball b = Ball(fixed_ball_position, Ball_stats(Element(element), 10, speed, radius));
		b.on_cast_set_direction(non_local_player->getShape().getRotation());
		b.setSpeed(-speed);
		balls_vector.push_back(b);

	}
}

void GameManager::pack_shield_object(sf::Packet & packet_to_send)
{
	int element = (int)local_shield.get_statistics().get_element();
	float radius = local_shield.get_statistics().get_radius();
	std::cout << "radius: " << radius << " element: " << element << "\n";
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
