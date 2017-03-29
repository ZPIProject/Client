#include "GameManager.h"



void GameManager::change_game_state(Game_states state)
{
	current_game_state = state;
}

void GameManager::draw()
{
	main_window.clear();

	main_window.draw(local_player->getShape());
	main_window.draw(non_local_player->getShape());
	
	main_window.display();
}

void GameManager::event_handler()
{
	sf::Event event;
	while (main_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			main_window.close();
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
	local_player->rotate(main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window)));
	local_player->move(directionX, directionY);
}

void GameManager::loging_menu()
{
	std::string username;
	std::string password;
	std::string ip_address;
	std::cout << "Podaj ip serwera: ";
	std::cin >> ip_address;
	//	std::cout << "/nPodaj haslo: ";
		//std::cin >> password;

	if (network_handler->connect(ip_address))
	{// tutaj sprawdzanie czy has³o zosta³o poprawnie wprowadzone i czy username jest z nim zgodne
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
	game_is_running = true;
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
	pack_all_and_send();
	recive_and_unpack_all();
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
	local_player = new Player(sf::Color::Red);
	int local_player_position_x = (WINDOW_WIDTH - local_player->getShape().getSize().x) / 2;
	int local_player_position_y = (WINDOW_HEIGHT - local_player->getShape().getSize().y);
	local_player->setPosition(local_player_position_x, local_player_position_y);

	non_local_player = new Player(sf::Color::Blue);
}

void GameManager::pack_player(sf::Packet& packet_to_send)
{
	
	packet_to_send << local_player->getPosition().x << local_player->getPosition().y << local_player->getShape().getRotation();
}

void GameManager::unpack_player(sf::Packet& recived_packet)
{
	float recived_position_x, recived_position_y, recived_rotation;
	recived_packet >> recived_position_x >> recived_position_y >> recived_rotation;
	//std::cout << recived_position_x << " " << recived_position_y << " " << recived_rotation;
	sf::Vector2f playerPosition;
	playerPosition.x = WINDOW_WIDTH - non_local_player->getShape().getSize().x / 2 - recived_position_x;
	playerPosition.y = WINDOW_HEIGHT - non_local_player->getShape().getSize().y / 2 - recived_position_y;
	non_local_player->setPosition(playerPosition);
	non_local_player->setRotation(recived_rotation);

}

void GameManager::pack_all_and_send()
{
	sf::Packet packet_to_send;
	pack_player(packet_to_send);
	network_handler->send_packet(packet_to_send);
}

void GameManager::recive_and_unpack_all()
{
	sf::Packet recived_packet = network_handler->recive_packet();

	unpack_player(recived_packet);
}
