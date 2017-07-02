#include "GuiHandler.h"
#include "ConstantValues.h"

GuiHandler::~GuiHandler()
{
	delete gui;
	delete gm;
	//delete window;
}

GuiHandler::GuiHandler(sf::RenderWindow & win)
{
	window = &win;
	gui = new tgui::Gui(*window);
	network_handler = new NetworkHandler(PORT);

	active_gui = CurrentActiveGUI::LOGIN;
	login();
	did_active_gui_changed = false;
	gm = new GameManager(network_handler, *window);
}

void GuiHandler::handle_event(sf::Event & event)
{
	gui->handleEvent(event);
}

void GuiHandler::character_selection()
{
	//Background
	tgui::Picture::Ptr background = tgui::Picture::create("Graphics/Screens/Menu_bg.png");
	background->setSize(800, 600);
	background->setPosition(0, 0);
	gui->add(background);

	std::vector<std::string> vector_of_images;
	std::vector<std::string> character_list;
	sf::Packet character_list_request;
	int current_character = 0;
	int character_count;

	character_list_request << 13 << current_logged_account;
	network_handler->send_packet(character_list_request);

	sf::Packet recived_list = network_handler->recive_packet();
	recived_list >> character_count;
	for (int i = 0; i < character_count; i++)
	{
		std::string character_name;
		recived_list >> character_name;
		character_list.push_back(character_name);
		std::string image = "Graphics/character/Char";
		image += std::to_string(i % 3);
		image += ".png";
		vector_of_images.push_back(image);
	}

	tgui::Button::Ptr button0 = tgui::Button::create("Enter");
	tgui::Button::Ptr button1 = tgui::Button::create("Logout");
	tgui::Button::Ptr button2 = tgui::Button::create("<");
	tgui::Button::Ptr button3 = tgui::Button::create(">");

	//tgui::Picture::Ptr characterPicture = tgui::Picture::create("../tempChar.png");
	//tgui::Label::Ptr labelPictureTemp;
	tgui::Picture::Ptr characterPictureTemp;
	tgui::Label::Ptr labelCharacterName;
	if (vector_of_images.size() > 0 && character_list.size() > 0)
	{
		characterPictureTemp = tgui::Picture::create(vector_of_images[0].c_str());
		//labelPictureTemp = tgui::Label::create(vector_of_images[0]);
		labelCharacterName = tgui::Label::create(character_list[0]);
		set_current_character(character_list[0]);
	}

	characterPictureTemp->setSize(160, 160);
	characterPictureTemp->setPosition(320, 220);
	gui->add(characterPictureTemp);

	labelCharacterName->setPosition(340, 180);
	labelCharacterName->setTextColor("white");
	gui->add(labelCharacterName);

	button0->setSize(200, 50);
	button0->setPosition(300, 500);
	button0->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button0);

	button1->setSize(50, 50);
	button1->setPosition(700, 500);
	button1->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::LOGIN);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button1);

	for (int i = 0; i < vector_of_images.size(); i++)
	{
		tgui::Button::Ptr buttonChar = tgui::Button::create(std::to_string(i));
		buttonChar->setSize(30, 30);
		buttonChar->setPosition(200 + (i * 50), 450);
		gui->add(buttonChar);

		buttonChar->connect("pressed", [=]() {
			std::cout << "-->" << i << " pressed() " << std::endl;

			characterPictureTemp->setTexture(vector_of_images[i].c_str());
			labelCharacterName->setText(character_list[i]);
			set_current_character(character_list[i]);
		});
	}
}

void GuiHandler::main_menu()
{
	//Background
	tgui::Picture::Ptr main_menuBackground = tgui::Picture::create("Graphics/Screens/Menu_bg.png");
	main_menuBackground->setSize(800, 600);
	main_menuBackground->setPosition(0, 0);
	gui->add(main_menuBackground);

	sf::Packet skillpoints_request;
	sf::Packet skills_request;

	tree = new Tree();

	//Set values
	skillpoints_request << 4 << current_picked_character;
	network_handler->send_packet(skillpoints_request);

	sf::Packet recived_skillpoints = network_handler->recive_packet();

	tree->setSkillPoints(tree->AvailablePoints(recived_skillpoints));

	skills_request << 2 << current_picked_character;
	network_handler->send_packet(skills_request);

	sf::Packet recived_skills = network_handler->recive_packet();

	tree->databaseUpdate(recived_skills);
	tree->count_all();

	gm->setTree(tree);

	std::cout << "main_menu\n";
	tgui::Button::Ptr button0 = tgui::Button::create("Play");
	tgui::Button::Ptr button1 = tgui::Button::create("Stats");
	tgui::Button::Ptr button2 = tgui::Button::create("How to play?");
	tgui::Button::Ptr button3 = tgui::Button::create("Character Screen");

	tgui::Button::Ptr button4 = tgui::Button::create("Logout");
	tgui::Button::Ptr button5 = tgui::Button::create("Exit");

	button0->setSize(300, 40);
	button0->setPosition(250, 190);
	button0->connect("pressed", [=]() {
		std::cout << "-->playPressed() " << std::endl;
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::GAME);
	});
	gui->add(button0);

	button1->setSize(300, 40);
	button1->setPosition(250, 250);
	button1->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::PLAYERSTATISTIC);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button1);

	button2->setSize(300, 40);
	button2->setPosition(250, 310);
	button2->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::TUTORIAL);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button2);

	button3->setSize(300, 40);
	button3->setPosition(250, 370);
	button3->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::CHARACTERSELECTION);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button3);


	button4->setSize(300, 40);
	button4->setPosition(250, 470);
	button4->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::LOGIN);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button4);

	button5->setSize(300, 40);
	button5->setPosition(250, 530);
	button5->connect("pressed", [&]() { window->close(); });
	gui->add(button5);
}

void GuiHandler::login()
{
	//Background
	tgui::Picture::Ptr loginBackground = tgui::Picture::create("Graphics/Screens/Login_bg.png");
	loginBackground->setSize(800, 600);
	loginBackground->setPosition(0, 0);
	gui->add(loginBackground);

	tgui::Button::Ptr button0 = tgui::Button::create("login");
	tgui::Button::Ptr button1 = tgui::Button::create("exit");
	tgui::EditBox::Ptr editbox0 = tgui::EditBox::create();
	tgui::EditBox::Ptr editbox1 = tgui::EditBox::create();
	tgui::EditBox::Ptr editbox2 = tgui::EditBox::create();

	editbox0->setSize(200, 30);
	editbox0->setPosition(550, 370);
	editbox0->setDefaultText("IP address");
	gui->add(editbox0);

	editbox1->setSize(200, 30);
	editbox1->setPosition(550, 420);
	editbox1->setDefaultText("username");
	gui->add(editbox1);

	editbox2->setSize(200, 30);
	editbox2->setPosition(550, 470);
	editbox2->setDefaultText("password");
	editbox2->setPasswordCharacter('*');
	gui->add(editbox2);

	button0->setSize(90, 30);
	button0->setPosition(550, 520);
	button0->connect("pressed", [=]()
	{
		NetworkHandler* nh = this->get_network_handler();
		nh->connect(editbox0->getText().toAnsiString());

		sf::Packet packet;
		packet << 0 << editbox1->getText().toAnsiString() << editbox2->getText().toAnsiString();
		nh->send_packet(packet);
		packet.clear();

		packet = nh->recive_packet();
		bool logged;
		packet >> logged;

		//if (logged)
		//{
		set_active_gui_changed(logged);
		change_active_gui(CurrentActiveGUI::CHARACTERSELECTION);
		this->set_account_name(editbox1->getText().toAnsiString());
		//}


	});
	/*	button0->connect("pressed", [](tgui::EditBox::Ptr editbox0, tgui::EditBox::Ptr editbox1) {
	std::cout << "-->login get: " << std::endl;
	std::cout << "username: " << editbox0->getText().toAnsiString() << std::endl;
	std::cout << "password: " << editbox1->getText().toAnsiString() << std::endl;
	});*/

	/*button0->connect("pressed", [=]() {
	this->set_active_gui_changed(true);
	this->change_active_gui(GuiHandler::CHARACTERSELECTION);
	std::cout << "change status " << this->get_status() << " DONE\n";
	});*/
	gui->add(button0);

	button1->setSize(90, 30);
	button1->setPosition(660, 520);
	button1->connect("pressed", [&]() { window->close(); });
	gui->add(button1);
}

void GuiHandler::tutorial()
{
	tgui::Picture::Ptr tutorialPicture = tgui::Picture::create("Graphics/tutorial.png");
	tgui::Button::Ptr button0 = tgui::Button::create("Back");

	tutorialPicture->setSize(800, 600);
	tutorialPicture->setPosition(0, 0);
	gui->add(tutorialPicture);

	button0->setSize(300, 40);
	button0->setPosition(250, 550);
	button0->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button0);
}

void GuiHandler::end_game(bool current_player_won)
{
	std::cout << current_picked_character << (current_player_won ? " won" : " lost" )<< "\n";
	tgui::Label::Ptr end_game_text = tgui::Label::create();
	tgui::Button::Ptr main_menu_button = tgui::Button::create("Menu");

	std::string tmp = "Game has ended";
	end_game_text->setText(tmp);
	end_game_text->setTextSize(20);
	end_game_text->setPosition(325, 300);
	gui->add(end_game_text);

	

	main_menu_button->setSize(100, 30);
	main_menu_button->setPosition(350, 350);
	main_menu_button->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
	}
	);
	gui->add(main_menu_button);
}


void GuiHandler::game()
{
	sf::Packet send_packet = gm->pack_maxHP();
	network_handler->send_packet(send_packet);
	sf::Packet recived_packet = network_handler->recive_packet();
	int enemy_max_hp = gm->unpack_maxHP(recived_packet);
	//std::cout << enemy_max_hp << std::endl;
	gm->setEnemyStats(enemy_max_hp);
	gm->players_initialization();
	gm->run();
	this->change_active_gui(GuiHandler::ENDGAME);
	gui->removeAllWidgets();
	end_game(gm->get_player_win_status());
}

void GuiHandler::draw()
{
	change_status();
	gui->draw();
}

void GuiHandler::main_loop()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			handle_event(event);
		}
		window->clear();
		draw();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && (active_gui == CurrentActiveGUI::PLAYERSTATISTIC || active_gui == CurrentActiveGUI::TUTORIAL)) {
			did_active_gui_changed = true;
			active_gui = CurrentActiveGUI::MAINMENU;
		}
		window->display();
	}
}

void GuiHandler::change_active_gui(CurrentActiveGUI gui)
{
	//change_status();
	active_gui = gui;
	std::cout << "change_active_gui" << (int)gui << "\n";
}

void GuiHandler::buySkill(int idS, std::string character, Tree* tree)
{
	sf::Packet request_packet = tree->buyRequest(character, idS + 1);
	network_handler->send_packet(request_packet);
	sf::Packet request_result_packet = network_handler->recive_packet();
	bool request_result;
	request_result_packet >> request_result;
	std::cout << request_result << std::endl;

	if (request_result) {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::PLAYERSTATISTIC);
		std::cout << "change status " << this->get_status() << " DONE\n";
		//tree->buySkill(idS);
	}
}

void GuiHandler::on_character_select_button()
{
	std::cout << "-->characterPressed() " << std::endl;
	did_active_gui_changed = true;
	active_gui = CurrentActiveGUI::MAINMENU;
}

void GuiHandler::change_status()
{

	if (did_active_gui_changed)
	{
	
		switch (active_gui)
		{
		case MAINMENU:
			gui->removeAllWidgets();
			main_menu();
			break;
		case LOGIN:
			gui->removeAllWidgets();
			login();
			break;
		case CHARACTERSELECTION:
			gui->removeAllWidgets();
			character_selection();
			break;
		case PLAYERSTATISTIC:
			gui->removeAllWidgets();
			statistics();
			break;
		case TUTORIAL:
			gui->removeAllWidgets();
			tutorial();
			break;
		case GAME:
			gui->removeAllWidgets();
			game();
			break;
		case ENDGAME:
			gui->removeAllWidgets();
			end_game(gm->get_player_win_status());
			break;
		default:
			break;
		}
		did_active_gui_changed = false;
	}
}
void GuiHandler::statistics()
{
	sf::Packet skillpoints_request;
	sf::Packet skills_request;

	tree = new Tree();

	//Set values
	skillpoints_request << 4 << current_picked_character;
	network_handler->send_packet(skillpoints_request);

	sf::Packet recived_skillpoints = network_handler->recive_packet();

	tree->setSkillPoints(tree->AvailablePoints(recived_skillpoints));

	skills_request << 2 << current_picked_character;
	network_handler->send_packet(skills_request);

	sf::Packet recived_skills = network_handler->recive_packet();

	tree->databaseUpdate(recived_skills);
	tree->count_all();

	//Background
	tgui::Picture::Ptr statisticsPicture = tgui::Picture::create("Graphics/Screens/SkillTree_bg.png");
	statisticsPicture->setSize(800, 600);
	statisticsPicture->setPosition(0, 0);
	gui->add(statisticsPicture);

	int knoValue_base = tree->getKnowledge();
	int wisValue_base = tree->getWisdom();
	int vitValue_base = tree->getVitality();

	int apValue_base = tree->getSkillPoints();

	int hpValue_base = 100 + vitValue_base * 5;
	int mpValue_base = 100 + wisValue_base * 5;

	//Buttons
	tgui::Button::Ptr button0 = tgui::Button::create("Save");
	tgui::Button::Ptr button1 = tgui::Button::create("Reset");
	tgui::Button::Ptr button2 = tgui::Button::create("Back");
	tgui::Label::Ptr label0 = tgui::Label::create("Player's stats:");

	//Info labels
	tgui::Label::Ptr label1 = tgui::Label::create("HP:");
	tgui::Label::Ptr label11 = tgui::Label::create(std::to_string(hpValue_base));
	tgui::Label::Ptr label2 = tgui::Label::create("MP:");
	tgui::Label::Ptr label21 = tgui::Label::create(std::to_string(mpValue_base));

	tgui::Label::Ptr label3 = tgui::Label::create("Player's atributes:");
	tgui::Label::Ptr label4 = tgui::Label::create("Knowledge:");
	tgui::Label::Ptr label41 = tgui::Label::create(std::to_string(knoValue_base));
	tgui::Label::Ptr label5 = tgui::Label::create("Wisdom:");
	tgui::Label::Ptr label51 = tgui::Label::create(std::to_string(wisValue_base));
	tgui::Label::Ptr label6 = tgui::Label::create("Vitality:");
	tgui::Label::Ptr label61 = tgui::Label::create(std::to_string(vitValue_base));

	tgui::Label::Ptr label7 = tgui::Label::create("Points to spend:");
	tgui::Label::Ptr label71 = tgui::Label::create(std::to_string(apValue_base));

	button0->setSize(80, 30);
	button0->setPosition(20, 370);
	button0->connect("pressed", []() {
		std::cout << "-->savePressed() " << std::endl;
	});
	gui->add(button0);

	button1->setSize(80, 30);
	button1->setPosition(120, 370);
	button1->connect("pressed", []() {
		std::cout << "-->resetPressed() " << std::endl;
	});
	gui->add(button1);

	button2->setSize(80, 30);
	button2->setPosition(20, 550);
	button2->connect("pressed", [=]() {
		this->set_active_gui_changed(true);
		this->change_active_gui(GuiHandler::MAINMENU);
		std::cout << "change status " << this->get_status() << " DONE\n";
	});
	gui->add(button2);


	label0->setPosition(20, 110);
	label0->setTextColor("white");
	gui->add(label0);

	label1->setPosition(20, 140);
	label1->setTextColor("white");
	gui->add(label1);
	label11->setPosition(170, 140);
	label11->setTextColor("white");
	gui->add(label11);


	label2->setPosition(20, 170);
	label2->setTextColor("white");
	gui->add(label2);
	label21->setPosition(170, 170);
	label21->setTextColor("white");
	gui->add(label21);

	label3->setPosition(20, 220);
	label3->setTextColor("white");
	gui->add(label3);

	label6->setPosition(20, 250);
	label6->setTextColor("white");
	gui->add(label6);
	label61->setPosition(170, 250);
	label61->setTextColor("white");
	gui->add(label61);

	label4->setPosition(20, 280);
	label4->setTextColor("white");
	gui->add(label4);
	label41->setPosition(170, 280);
	label41->setTextColor("white");
	gui->add(label41);

	label5->setPosition(20, 310);
	label5->setTextColor("white");
	gui->add(label5);
	label51->setPosition(170, 310);
	label51->setTextColor("white");
	gui->add(label51);

	label7->setPosition(20, 340);
	label7->setTextColor("white");
	gui->add(label7);
	label71->setPosition(170, 340);
	label71->setTextColor("white");
	gui->add(label71);
	//------------------------------------------------------------//
	//Tree Buttons

	//Fire Buttons
	if (tree->getStatus(1) == 2) {
		tgui::Picture::Ptr buttonFK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K_got.png");
		buttonFK1->setSize(24, 24);
		buttonFK1->setPosition(538, 266);
		gui->add(buttonFK1);
	}
	else if (tree->getStatus(1) == 1) {
		tgui::Picture::Ptr buttonFK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFK1->setSize(24, 24);
		buttonFK1->setPosition(538, 266);
		gui->add(buttonFK1);
	}
	else {
		tgui::Picture::Ptr buttonFK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K.png");
		buttonFK1->setSize(24, 24);
		buttonFK1->setPosition(538, 266);
		buttonFK1->connect("clicked", &GuiHandler::buySkill, this, 1, current_picked_character, tree);
		gui->add(buttonFK1);
	}
	//
	if (tree->getStatus(2) == 2) {
		tgui::Picture::Ptr buttonFV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V_got.png");
		buttonFV1->setSize(24, 24);
		buttonFV1->setPosition(570, 266);
		gui->add(buttonFV1);
	}
	else if (tree->getStatus(2) == 1) {
		tgui::Picture::Ptr buttonFV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFV1->setSize(24, 24);
		buttonFV1->setPosition(570, 266);
		gui->add(buttonFV1);
	}
	else {
		tgui::Picture::Ptr buttonFV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V.png");
		buttonFV1->setSize(24, 24);
		buttonFV1->setPosition(570, 266);
		buttonFV1->connect("clicked", &GuiHandler::buySkill, this, 2, current_picked_character, tree);
		gui->add(buttonFV1);
	}
	//
	if (tree->getStatus(3) == 2) {
		tgui::Picture::Ptr buttonFW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_W_got.png");
		buttonFW1->setSize(24, 24);
		buttonFW1->setPosition(554, 250);
		gui->add(buttonFW1);
	}
	else if (tree->getStatus(3) == 1) {
		tgui::Picture::Ptr buttonFW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFW1->setSize(24, 24);
		buttonFW1->setPosition(554, 250);
		gui->add(buttonFW1);
	}
	else {
		tgui::Picture::Ptr buttonFW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_W.png");
		buttonFW1->setSize(24, 24);
		buttonFW1->setPosition(554, 250);
		buttonFW1->connect("clicked", &GuiHandler::buySkill, this, 3, current_picked_character, tree);
		gui->add(buttonFW1);
	}
	//
	if (tree->getStatus(4) == 2) {
		tgui::Picture::Ptr buttonFV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V_got.png");
		buttonFV2->setSize(24, 24);
		buttonFV2->setPosition(522, 250);
		gui->add(buttonFV2);
	}
	else if (tree->getStatus(4) == 1) {
		tgui::Picture::Ptr buttonFV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFV2->setSize(24, 24);
		buttonFV2->setPosition(522, 250);
		gui->add(buttonFV2);
	}
	else {
		tgui::Picture::Ptr buttonFV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V.png");
		buttonFV2->setSize(24, 24);
		buttonFV2->setPosition(522, 250);
		buttonFV2->connect("clicked", &GuiHandler::buySkill, this, 4, current_picked_character, tree);
		gui->add(buttonFV2);
	}
	//
	if (tree->getStatus(5) == 2) {
		tgui::Picture::Ptr buttonFK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K_got.png");
		buttonFK2->setSize(24, 24);
		buttonFK2->setPosition(586, 250);
		gui->add(buttonFK2);
	}
	else if (tree->getStatus(5) == 1) {
		tgui::Picture::Ptr buttonFK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFK2->setSize(24, 24);
		buttonFK2->setPosition(586, 250);
		gui->add(buttonFK2);
	}
	else {
		tgui::Picture::Ptr buttonFK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K.png");
		buttonFK2->setSize(24, 24);
		buttonFK2->setPosition(586, 250);
		buttonFK2->connect("clicked", &GuiHandler::buySkill, this, 5, current_picked_character, tree);
		gui->add(buttonFK2);
	}
	//
	if (tree->getStatus(6) == 2) {
		tgui::Picture::Ptr buttonFB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_FB_got.png");
		buttonFB2->setSize(24, 24);
		buttonFB2->setPosition(506, 234);
		gui->add(buttonFB2);
	}
	else if (tree->getStatus(6) == 1) {
		tgui::Picture::Ptr buttonFB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFB2->setSize(24, 24);
		buttonFB2->setPosition(506, 234);
		gui->add(buttonFB2);
	}
	else {
		tgui::Picture::Ptr buttonFB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_FB.png");
		buttonFB2->setSize(24, 24);
		buttonFB2->setPosition(506, 234);
		buttonFB2->connect("clicked", &GuiHandler::buySkill, this, 5, current_picked_character, tree);
		gui->add(buttonFB2);
	}
	//
	if (tree->getStatus(7) == 2) {
		tgui::Picture::Ptr buttonFT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_FT_got.png");
		buttonFT->setSize(24, 24);
		buttonFT->setPosition(602, 234);
		gui->add(buttonFT);
	}
	else if (tree->getStatus(7) == 1) {
		tgui::Picture::Ptr buttonFT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFT->setSize(24, 24);
		buttonFT->setPosition(602, 234);
		gui->add(buttonFT);
	}
	else {
		tgui::Picture::Ptr buttonFT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_FT.png");
		buttonFT->setSize(24, 24);
		buttonFT->setPosition(602, 234);
		buttonFT->connect("clicked", &GuiHandler::buySkill, this, 7, current_picked_character, tree);
		gui->add(buttonFT);
	}
	//
	if (tree->getStatus(8) == 2) {
		tgui::Picture::Ptr buttonFW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_W_got.png");
		buttonFW2->setSize(24, 24);
		buttonFW2->setPosition(522, 218);
		gui->add(buttonFW2);
	}
	else if (tree->getStatus(8) == 1) {
		tgui::Picture::Ptr buttonFW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFW2->setSize(24, 24);
		buttonFW2->setPosition(522, 218);
		gui->add(buttonFW2);
	}
	else {
		tgui::Picture::Ptr buttonFW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_W.png");
		buttonFW2->setSize(24, 24);
		buttonFW2->setPosition(522, 218);
		buttonFW2->connect("clicked", &GuiHandler::buySkill, this, 8, current_picked_character, tree);
		gui->add(buttonFW2);
	}
	//
	if (tree->getStatus(9) == 2) {
		tgui::Picture::Ptr buttonFK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K_got.png");
		buttonFK3->setSize(24, 24);
		buttonFK3->setPosition(586, 218);
		gui->add(buttonFK3);
	}
	else if (tree->getStatus(9) == 1) {
		tgui::Picture::Ptr buttonFK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFK3->setSize(24, 24);
		buttonFK3->setPosition(586, 218);
		gui->add(buttonFK3);
	}
	else {
		tgui::Picture::Ptr buttonFK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K.png");
		buttonFK3->setSize(24, 24);
		buttonFK3->setPosition(586, 218);
		buttonFK3->connect("clicked", &GuiHandler::buySkill, this, 9, current_picked_character, tree);
		gui->add(buttonFK3);
	}
	//
	if (tree->getStatus(10) == 2) {
		tgui::Picture::Ptr buttonFV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V_got.png");
		buttonFV3->setSize(24, 24);
		buttonFV3->setPosition(538, 202);
		gui->add(buttonFV3);
	}
	else if (tree->getStatus(10) == 1) {
		tgui::Picture::Ptr buttonFV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFV3->setSize(24, 24);
		buttonFV3->setPosition(538, 202);
		gui->add(buttonFV3);
	}
	else {
		tgui::Picture::Ptr buttonFV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V.png");
		buttonFV3->setSize(24, 24);
		buttonFV3->setPosition(538, 202);
		buttonFV3->connect("clicked", &GuiHandler::buySkill, this, 10, current_picked_character, tree);
		gui->add(buttonFV3);
	}
	//
	if (tree->getStatus(11) == 2) {
		tgui::Picture::Ptr buttonFV4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V_got.png");
		buttonFV4->setSize(24, 24);
		buttonFV4->setPosition(570, 202);
		gui->add(buttonFV4);
	}
	else if (tree->getStatus(11) == 1) {
		tgui::Picture::Ptr buttonFV4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFV4->setSize(24, 24);
		buttonFV4->setPosition(570, 202);
		gui->add(buttonFV4);
	}
	else {
		tgui::Picture::Ptr buttonFV4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_V.png");
		buttonFV4->setSize(24, 24);
		buttonFV4->setPosition(570, 202);
		buttonFV4->connect("clicked", &GuiHandler::buySkill, this, 11, current_picked_character, tree);
		gui->add(buttonFV4);
	}
	//
	if (tree->getStatus(12) == 2) {
		tgui::Picture::Ptr buttonFK4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K_got.png");
		buttonFK4->setSize(24, 24);
		buttonFK4->setPosition(538, 177);
		gui->add(buttonFK4);
	}
	else if (tree->getStatus(12) == 1) {
		tgui::Picture::Ptr buttonFK4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFK4->setSize(24, 24);
		buttonFK4->setPosition(538, 177);
		gui->add(buttonFK4);
	}
	else {
		tgui::Picture::Ptr buttonFK4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_K.png");
		buttonFK4->setSize(24, 24);
		buttonFK4->setPosition(538, 177);
		buttonFK4->connect("clicked", &GuiHandler::buySkill, this, 12, current_picked_character, tree);
		gui->add(buttonFK4);
	}
	//
	if (tree->getStatus(13) == 2) {
		tgui::Picture::Ptr buttonFW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_W_got.png");
		buttonFW3->setSize(24, 24);
		buttonFW3->setPosition(570, 177);
		gui->add(buttonFW3);
	}
	else if (tree->getStatus(13) == 1) {
		tgui::Picture::Ptr buttonFW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFW3->setSize(24, 24);
		buttonFW3->setPosition(570, 177);
		gui->add(buttonFW3);
	}
	else {
		tgui::Picture::Ptr buttonFW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_W.png");
		buttonFW3->setSize(24, 24);
		buttonFW3->setPosition(570, 177);
		buttonFW3->connect("clicked", &GuiHandler::buySkill, this, 13, current_picked_character, tree);
		gui->add(buttonFW3);
	}
	//
	if (tree->getStatus(14) == 2) {
		tgui::Picture::Ptr buttonFS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_FS_got.png");
		buttonFS->setSize(24, 24);
		buttonFS->setPosition(554, 161);
		gui->add(buttonFS);
	}
	else if (tree->getStatus(14) == 1) {
		tgui::Picture::Ptr buttonFS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonFS->setSize(24, 24);
		buttonFS->setPosition(554, 161);
		gui->add(buttonFS);
	}
	else {
		tgui::Picture::Ptr buttonFS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeFire_FS.png");
		buttonFS->setSize(24, 24);
		buttonFS->setPosition(554, 161);
		buttonFS->connect("clicked", &GuiHandler::buySkill, this, 14, current_picked_character, tree);
		gui->add(buttonFS);
	}
	//
	if (tree->getStatus(16) == 2) {
		tgui::Picture::Ptr buttonWiW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_W_got.png");
		buttonWiW1->setSize(24, 24);
		buttonWiW1->setPosition(612, 308);
		gui->add(buttonWiW1);
	}
	else if (tree->getStatus(16) == 1) {
		tgui::Picture::Ptr buttonWiW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiW1->setSize(24, 24);
		buttonWiW1->setPosition(612, 308);
		gui->add(buttonWiW1);
	}
	else {
		tgui::Picture::Ptr buttonWiW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_W.png");
		buttonWiW1->setSize(24, 24);
		buttonWiW1->setPosition(612, 308);
		buttonWiW1->connect("clicked", &GuiHandler::buySkill, this, 16, current_picked_character, tree);
		gui->add(buttonWiW1);
	}
	//
	if (tree->getStatus(17) == 2) {
		tgui::Picture::Ptr buttonWiK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_K_got.png");
		buttonWiK1->setSize(24, 24);
		buttonWiK1->setPosition(628, 292);
		gui->add(buttonWiK1);
	}
	else if (tree->getStatus(17) == 1) {
		tgui::Picture::Ptr buttonWiK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiK1->setSize(24, 24);
		buttonWiK1->setPosition(628, 292);
		gui->add(buttonWiK1);
	}
	else {
		tgui::Picture::Ptr buttonWiK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_K.png");
		buttonWiK1->setSize(24, 24);
		buttonWiK1->setPosition(628, 292);
		buttonWiK1->connect("clicked", &GuiHandler::buySkill, this, 17, current_picked_character, tree);
		gui->add(buttonWiK1);
	}
	//
	if (tree->getStatus(18) == 2) {
		tgui::Picture::Ptr buttonWiW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_W_got.png");
		buttonWiW2->setSize(24, 24);
		buttonWiW2->setPosition(653, 292);
		gui->add(buttonWiW2);
	}
	else if (tree->getStatus(18) == 1) {
		tgui::Picture::Ptr buttonWiW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiW2->setSize(24, 24);
		buttonWiW2->setPosition(653, 292);
		gui->add(buttonWiW2);
	}
	else {
		tgui::Picture::Ptr buttonWiW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_W.png");
		buttonWiW2->setSize(24, 24);
		buttonWiW2->setPosition(653, 292);
		buttonWiW2->connect("clicked", &GuiHandler::buySkill, this, 18, current_picked_character, tree);
		gui->add(buttonWiW2);
	}
	//
	if (tree->getStatus(19) == 2) {
		tgui::Picture::Ptr buttonWiV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_V_got.png");
		buttonWiV1->setSize(24, 24);
		buttonWiV1->setPosition(678, 292);
		gui->add(buttonWiV1);
	}
	else if (tree->getStatus(19) == 1) {
		tgui::Picture::Ptr buttonWiV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiV1->setSize(24, 24);
		buttonWiV1->setPosition(678, 292);
		gui->add(buttonWiV1);
	}
	else {
		tgui::Picture::Ptr buttonWiV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_V.png");
		buttonWiV1->setSize(24, 24);
		buttonWiV1->setPosition(678, 292);
		buttonWiV1->connect("clicked", &GuiHandler::buySkill, this, 19, current_picked_character, tree);
		gui->add(buttonWiV1);
	}
	//
	if (tree->getStatus(20) == 2) {
		tgui::Picture::Ptr buttonWiB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_AB_got.png");
		buttonWiB2->setSize(24, 24);
		buttonWiB2->setPosition(694, 276);
		gui->add(buttonWiB2);
	}
	else if (tree->getStatus(20) == 1) {
		tgui::Picture::Ptr buttonWiB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiB2->setSize(24, 24);
		buttonWiB2->setPosition(694, 276);
		gui->add(buttonWiB2);
	}
	else {
		tgui::Picture::Ptr buttonWiB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_AB.png");
		buttonWiB2->setSize(24, 24);
		buttonWiB2->setPosition(694, 276);
		buttonWiB2->connect("clicked", &GuiHandler::buySkill, this, 20, current_picked_character, tree);
		gui->add(buttonWiB2);
	}
	//
	if (tree->getStatus(21) == 2) {
		tgui::Picture::Ptr buttonWiV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_V_got.png");
		buttonWiV2->setSize(24, 24);
		buttonWiV2->setPosition(694, 308);
		gui->add(buttonWiV2);
	}
	else if (tree->getStatus(21) == 1) {
		tgui::Picture::Ptr buttonWiV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiV2->setSize(24, 24);
		buttonWiV2->setPosition(694, 308);
		gui->add(buttonWiV2);
	}
	else {
		tgui::Picture::Ptr buttonWiV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_V.png");
		buttonWiV2->setSize(24, 24);
		buttonWiV2->setPosition(694, 308);
		buttonWiV2->connect("clicked", &GuiHandler::buySkill, this, 21, current_picked_character, tree);
		gui->add(buttonWiV2);
	}
	//
	if (tree->getStatus(22) == 2) {
		tgui::Picture::Ptr buttonWiW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_W_got.png");
		buttonWiW3->setSize(24, 24);
		buttonWiW3->setPosition(694, 333);
		gui->add(buttonWiW3);
	}
	else if (tree->getStatus(22) == 1) {
		tgui::Picture::Ptr buttonWiW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiW3->setSize(24, 24);
		buttonWiW3->setPosition(694, 333);
		gui->add(buttonWiW3);
	}
	else {
		tgui::Picture::Ptr buttonWiW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_W.png");
		buttonWiW3->setSize(24, 24);
		buttonWiW3->setPosition(694, 333);
		buttonWiW3->connect("clicked", &GuiHandler::buySkill, this, 24, current_picked_character, tree);
		gui->add(buttonWiW3);
	}
	//
	if (tree->getStatus(23) == 2) {
		tgui::Picture::Ptr buttonWiT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_AT_got.png");
		buttonWiT->setSize(24, 24);
		buttonWiT->setPosition(710, 349);
		gui->add(buttonWiT);
	}
	else if (tree->getStatus(23) == 1) {
		tgui::Picture::Ptr buttonWiT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiT->setSize(24, 24);
		buttonWiT->setPosition(710, 349);
		gui->add(buttonWiT);
	}
	else {
		tgui::Picture::Ptr buttonWiT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_AT.png");
		buttonWiT->setSize(24, 24);
		buttonWiT->setPosition(710, 349);
		buttonWiT->connect("clicked", &GuiHandler::buySkill, this, 23, current_picked_character, tree);
		gui->add(buttonWiT);
	}
	//
	if (tree->getStatus(24) == 2) {
		tgui::Picture::Ptr buttonWiV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_V_got.png");
		buttonWiV3->setSize(24, 24);
		buttonWiV3->setPosition(678, 349);
		gui->add(buttonWiV3);
	}
	else if (tree->getStatus(24) == 1) {
		tgui::Picture::Ptr buttonWiV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiV3->setSize(24, 24);
		buttonWiV3->setPosition(678, 349);
		gui->add(buttonWiV3);
	}
	else {
		tgui::Picture::Ptr buttonWiV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_V.png");
		buttonWiV3->setSize(24, 24);
		buttonWiV3->setPosition(678, 349);
		buttonWiV3->connect("clicked", &GuiHandler::buySkill, this, 24, current_picked_character, tree);
		gui->add(buttonWiV3);
	}
	//
	if (tree->getStatus(25) == 2) {
		tgui::Picture::Ptr buttonWiK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_K_got.png");
		buttonWiK2->setSize(24, 24);
		buttonWiK2->setPosition(653, 349);
		gui->add(buttonWiK2);
	}
	else if (tree->getStatus(25) == 1) {
		tgui::Picture::Ptr buttonWiK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiK2->setSize(24, 24);
		buttonWiK2->setPosition(653, 349);
		gui->add(buttonWiK2);
	}
	else {
		tgui::Picture::Ptr buttonWiK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_K.png");
		buttonWiK2->setSize(24, 24);
		buttonWiK2->setPosition(653, 349);
		buttonWiK2->connect("clicked", &GuiHandler::buySkill, this, 25, current_picked_character, tree);
		gui->add(buttonWiK2);
	}
	//
	if (tree->getStatus(26) == 2) {
		tgui::Picture::Ptr buttonWiS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_AS_got.png");
		buttonWiS->setSize(24, 24);
		buttonWiS->setPosition(653, 324);
		gui->add(buttonWiS);
	}
	else if (tree->getStatus(26) == 1) {
		tgui::Picture::Ptr buttonWiS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWiS->setSize(24, 24);
		buttonWiS->setPosition(653, 324);
		gui->add(buttonWiS);
	}
	else {
		tgui::Picture::Ptr buttonWiS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeAir_AS.png");
		buttonWiS->setSize(24, 24);
		buttonWiS->setPosition(653, 324);
		buttonWiS->connect("clicked", &GuiHandler::buySkill, this, 26, current_picked_character, tree);
		gui->add(buttonWiS);
	}
	//
	if (tree->getStatus(28) == 2) {
		tgui::Picture::Ptr buttonTK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K_got.png");
		buttonTK1->setSize(24, 24);
		buttonTK1->setPosition(600, 369);
		gui->add(buttonTK1);
	}
	else if (tree->getStatus(28) == 1) {
		tgui::Picture::Ptr buttonTK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTK1->setSize(24, 24);
		buttonTK1->setPosition(600, 369);
		gui->add(buttonTK1);
	}
	else {
		tgui::Picture::Ptr buttonTK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K.png");
		buttonTK1->setSize(24, 24);
		buttonTK1->setPosition(600, 369);
		buttonTK1->connect("clicked", &GuiHandler::buySkill, this, 28, current_picked_character, tree);
		gui->add(buttonTK1);
	}
	//
	if (tree->getStatus(29) == 2) {
		tgui::Picture::Ptr buttonTW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_W_got.png");
		buttonTW1->setSize(24, 24);
		buttonTW1->setPosition(616, 385);
		gui->add(buttonTW1);
	}
	else if (tree->getStatus(29) == 1) {
		tgui::Picture::Ptr buttonTW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTW1->setSize(24, 24);
		buttonTW1->setPosition(616, 385);
		gui->add(buttonTW1);
	}
	else {
		tgui::Picture::Ptr buttonTW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_W.png");
		buttonTW1->setSize(24, 24);
		buttonTW1->setPosition(616, 385);
		buttonTW1->connect("clicked", &GuiHandler::buySkill, this, 29, current_picked_character, tree);
		gui->add(buttonTW1);
	}
	//
	if (tree->getStatus(30) == 2) {
		tgui::Picture::Ptr buttonTK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K_got.png");
		buttonTK2->setSize(24, 24);
		buttonTK2->setPosition(600, 401);
		gui->add(buttonTK2);
	}
	else if (tree->getStatus(30) == 1) {
		tgui::Picture::Ptr buttonTK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTK2->setSize(24, 24);
		buttonTK2->setPosition(600, 401);
		gui->add(buttonTK2);
	}
	else {
		tgui::Picture::Ptr buttonTK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K.png");
		buttonTK2->setSize(24, 24);
		buttonTK2->setPosition(600, 401);
		buttonTK2->connect("clicked", &GuiHandler::buySkill, this, 30, current_picked_character, tree);
		gui->add(buttonTK2);
	}
	//
	if (tree->getStatus(31) == 2) {
		tgui::Picture::Ptr buttonTB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_TB_got.png");
		buttonTB2->setSize(24, 24);
		buttonTB2->setPosition(632, 401);
		gui->add(buttonTB2);
	}
	else if (tree->getStatus(31) == 1) {
		tgui::Picture::Ptr buttonTB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTB2->setSize(24, 24);
		buttonTB2->setPosition(632, 401);
		gui->add(buttonTB2);
	}
	else {
		tgui::Picture::Ptr buttonTB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_TB.png");
		buttonTB2->setSize(24, 24);
		buttonTB2->setPosition(632, 401);
		buttonTB2->connect("clicked", &GuiHandler::buySkill, this, 31, current_picked_character, tree);
		gui->add(buttonTB2);
	}
	//
	if (tree->getStatus(32) == 2) {
		tgui::Picture::Ptr buttonTV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_V_got.png");
		buttonTV1->setSize(24, 24);
		buttonTV1->setPosition(584, 417);
		gui->add(buttonTV1);
	}
	else if (tree->getStatus(32) == 1) {
		tgui::Picture::Ptr buttonTV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTV1->setSize(24, 24);
		buttonTV1->setPosition(584, 417);
		gui->add(buttonTV1);
	}
	else {
		tgui::Picture::Ptr buttonTV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_V.png");
		buttonTV1->setSize(24, 24);
		buttonTV1->setPosition(584, 417);
		buttonTV1->connect("clicked", &GuiHandler::buySkill, this, 32, current_picked_character, tree);
		gui->add(buttonTV1);
	}
	//
	if (tree->getStatus(33) == 2) {
		tgui::Picture::Ptr buttonTW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_W_got.png");
		buttonTW2->setSize(24, 24);
		buttonTW2->setPosition(648, 417);
		gui->add(buttonTW2);
	}
	else if (tree->getStatus(33) == 1) {
		tgui::Picture::Ptr buttonTW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTW2->setSize(24, 24);
		buttonTW2->setPosition(648, 417);
		gui->add(buttonTW2);
	}
	else {
		tgui::Picture::Ptr buttonTW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_W.png");
		buttonTW2->setSize(24, 24);
		buttonTW2->setPosition(648, 417);
		buttonTW2->connect("clicked", &GuiHandler::buySkill, this, 33, current_picked_character, tree);
		gui->add(buttonTW2);
	}
	//
	if (tree->getStatus(34) == 2) {
		tgui::Picture::Ptr buttonTK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K_got.png");
		buttonTK3->setSize(24, 24);
		buttonTK3->setPosition(600, 433);
		gui->add(buttonTK3);
	}
	else if (tree->getStatus(34) == 1) {
		tgui::Picture::Ptr buttonTK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTK3->setSize(24, 24);
		buttonTK3->setPosition(600, 433);
		gui->add(buttonTK3);
	}
	else {
		tgui::Picture::Ptr buttonTK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K.png");
		buttonTK3->setSize(24, 24);
		buttonTK3->setPosition(600, 433);
		buttonTK3->connect("clicked", &GuiHandler::buySkill, this, 34, current_picked_character, tree);
		gui->add(buttonTK3);
	}
	//
	if (tree->getStatus(35) == 2) {
		tgui::Picture::Ptr buttonTT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_TT_got.png");
		buttonTT->setSize(24, 24);
		buttonTT->setPosition(632, 433);
		gui->add(buttonTT);
	}
	else if (tree->getStatus(35) == 1) {
		tgui::Picture::Ptr buttonTT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTT->setSize(24, 24);
		buttonTT->setPosition(632, 433);
		gui->add(buttonTT);
	}
	else {
		tgui::Picture::Ptr buttonTT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_TT.png");
		buttonTT->setSize(24, 24);
		buttonTT->setPosition(632, 433);
		buttonTT->connect("clicked", &GuiHandler::buySkill, this, 35, current_picked_character, tree);
		gui->add(buttonTT);
	}
	//
	if (tree->getStatus(36) == 2) {
		tgui::Picture::Ptr buttonTS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_TS_got.png");
		buttonTS->setSize(24, 24);
		buttonTS->setPosition(584, 449);
		gui->add(buttonTS);
	}
	else if (tree->getStatus(36) == 1) {
		tgui::Picture::Ptr buttonTS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonTS->setSize(24, 24);
		buttonTS->setPosition(584, 449);
		gui->add(buttonTS);
	}
	else {
		tgui::Picture::Ptr buttonTS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_TS.png");
		buttonTS->setSize(24, 24);
		buttonTS->setPosition(584, 449);
		buttonTS->connect("clicked", &GuiHandler::buySkill, this, 36, current_picked_character, tree);
		gui->add(buttonTS);
	}
	//
	if (tree->getStatus(38) == 2) {
		tgui::Picture::Ptr buttonEW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W_got.png");
		buttonEW1->setSize(24, 24);
		buttonEW1->setPosition(508, 369);
		gui->add(buttonEW1);
	}
	else if (tree->getStatus(38) == 1) {
		tgui::Picture::Ptr buttonEW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEW1->setSize(24, 24);
		buttonEW1->setPosition(508, 369);
		gui->add(buttonEW1);
	}
	else {
		tgui::Picture::Ptr buttonEW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_W.png");
		buttonEW1->setSize(24, 24);
		buttonEW1->setPosition(508, 369);
		buttonEW1->connect("clicked", &GuiHandler::buySkill, this, 38, current_picked_character, tree);
		gui->add(buttonEW1);
	}
	//
	if (tree->getStatus(39) == 2) {
		tgui::Picture::Ptr buttonEV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_V_got.png");
		buttonEV1->setSize(24, 24);
		buttonEV1->setPosition(492, 385);
		gui->add(buttonEV1);
	}
	else if (tree->getStatus(39) == 1) {
		tgui::Picture::Ptr buttonEV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEV1->setSize(24, 24);
		buttonEV1->setPosition(492, 385);
		gui->add(buttonEV1);
	}
	else {
		tgui::Picture::Ptr buttonEV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_V.png");
		buttonEV1->setSize(24, 24);
		buttonEV1->setPosition(492, 385);
		buttonEV1->connect("clicked", &GuiHandler::buySkill, this, 39, current_picked_character, tree);
		gui->add(buttonEV1);
	}
	//
	if (tree->getStatus(40) == 2) {
		tgui::Picture::Ptr buttonEV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_V_got.png");
		buttonEV2->setSize(24, 24);
		buttonEV2->setPosition(524, 385);
		gui->add(buttonEV2);
	}
	else if (tree->getStatus(40) == 1) {
		tgui::Picture::Ptr buttonEV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEV2->setSize(24, 24);
		buttonEV2->setPosition(524, 385);
		gui->add(buttonEV2);
	}
	else {
		tgui::Picture::Ptr buttonEV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_V.png");
		buttonEV2->setSize(24, 24);
		buttonEV2->setPosition(524, 385);
		buttonEV2->connect("clicked", &GuiHandler::buySkill, this, 40, current_picked_character, tree);
		gui->add(buttonEV2);
	}
	//
	if (tree->getStatus(41) == 2) {
		tgui::Picture::Ptr buttonEK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_K_got.png");
		buttonEK1->setSize(24, 24);
		buttonEK1->setPosition(476, 401);
		gui->add(buttonEK1);
	}
	else if (tree->getStatus(41) == 1) {
		tgui::Picture::Ptr buttonEK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEK1->setSize(24, 24);
		buttonEK1->setPosition(476, 401);
		gui->add(buttonEK1);
	}
	else {
		tgui::Picture::Ptr buttonEK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeThunder_K.png");
		buttonEK1->setSize(24, 24);
		buttonEK1->setPosition(476, 401);
		buttonEK1->connect("clicked", &GuiHandler::buySkill, this, 41, current_picked_character, tree);
		gui->add(buttonEK1);
	}
	//
	if (tree->getStatus(42) == 2) {
		tgui::Picture::Ptr buttonEW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W_got.png");
		buttonEW2->setSize(24, 24);
		buttonEW2->setPosition(540, 401);
		gui->add(buttonEW2);
	}
	else if (tree->getStatus(42) == 1) {
		tgui::Picture::Ptr buttonEW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEW2->setSize(24, 24);
		buttonEW2->setPosition(540, 401);
		gui->add(buttonEW2);
	}
	else {
		tgui::Picture::Ptr buttonEW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W.png");
		buttonEW2->setSize(24, 24);
		buttonEW2->setPosition(540, 401);
		buttonEW2->connect("clicked", &GuiHandler::buySkill, this, 42, current_picked_character, tree);
		gui->add(buttonEW2);
	}
	//
	if (tree->getStatus(43) == 2) {
		tgui::Picture::Ptr buttonEB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_EB_got.png");
		buttonEB2->setSize(24, 24);
		buttonEB2->setPosition(476, 426);
		gui->add(buttonEB2);
	}
	else if (tree->getStatus(43) == 1) {
		tgui::Picture::Ptr buttonEB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEB2->setSize(24, 24);
		buttonEB2->setPosition(476, 426);
		gui->add(buttonEB2);
	}
	else {
		tgui::Picture::Ptr buttonEB2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_EB.png");
		buttonEB2->setSize(24, 24);
		buttonEB2->setPosition(476, 426);
		buttonEB2->connect("clicked", &GuiHandler::buySkill, this, 43, current_picked_character, tree);
		gui->add(buttonEB2);
	}
	//
	if (tree->getStatus(44) == 2) {
		tgui::Picture::Ptr buttonET = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_ET_got.png");
		buttonET->setSize(24, 24);
		buttonET->setPosition(540, 426);
		gui->add(buttonET);
	}
	else if (tree->getStatus(44) == 1) {
		tgui::Picture::Ptr buttonET = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonET->setSize(24, 24);
		buttonET->setPosition(540, 426);
		gui->add(buttonET);
	}
	else {
		tgui::Picture::Ptr buttonET = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_ET.png");
		buttonET->setSize(24, 24);
		buttonET->setPosition(540, 426);
		buttonET->connect("clicked", &GuiHandler::buySkill, this, 44, current_picked_character, tree);
		gui->add(buttonET);
	}
	//
	if (tree->getStatus(45) == 2) {
		tgui::Picture::Ptr buttonEW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W_got.png");
		buttonEW3->setSize(24, 24);
		buttonEW3->setPosition(476, 451);
		gui->add(buttonEW3);
	}
	else if (tree->getStatus(45) == 1) {
		tgui::Picture::Ptr buttonEW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEW3->setSize(24, 24);
		buttonEW3->setPosition(476, 451);
		gui->add(buttonEW3);
	}
	else {
		tgui::Picture::Ptr buttonEW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W.png");
		buttonEW3->setSize(24, 24);
		buttonEW3->setPosition(476, 451);
		buttonEW3->connect("clicked", &GuiHandler::buySkill, this, 45, current_picked_character, tree);
		gui->add(buttonEW3);
	}
	//
	if (tree->getStatus(46) == 2) {
		tgui::Picture::Ptr buttonEK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_K_got.png");
		buttonEK2->setSize(24, 24);
		buttonEK2->setPosition(540, 451);
		gui->add(buttonEK2);
	}
	else if (tree->getStatus(46) == 1) {
		tgui::Picture::Ptr buttonEK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEK2->setSize(24, 24);
		buttonEK2->setPosition(540, 451);
		gui->add(buttonEK2);
	}
	else {
		tgui::Picture::Ptr buttonEK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_K.png");
		buttonEK2->setSize(24, 24);
		buttonEK2->setPosition(540, 451);
		buttonEK2->connect("clicked", &GuiHandler::buySkill, this, 46, current_picked_character, tree);
		gui->add(buttonEK2);
	}
	//
	if (tree->getStatus(47) == 2) {
		tgui::Picture::Ptr buttonEV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_V_got.png");
		buttonEV3->setSize(24, 24);
		buttonEV3->setPosition(492, 467);
		gui->add(buttonEV3);
	}
	else if (tree->getStatus(47) == 1) {
		tgui::Picture::Ptr buttonEV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEV3->setSize(24, 24);
		buttonEV3->setPosition(492, 467);
		gui->add(buttonEV3);
	}
	else {
		tgui::Picture::Ptr buttonEV3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_V.png");
		buttonEV3->setSize(24, 24);
		buttonEV3->setPosition(492, 467);
		buttonEV3->connect("clicked", &GuiHandler::buySkill, this, 47, current_picked_character, tree);
		gui->add(buttonEV3);
	}
	//
	if (tree->getStatus(48) == 2) {
		tgui::Picture::Ptr buttonEK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_K_got.png");
		buttonEK3->setSize(24, 24);
		buttonEK3->setPosition(524, 467);
		gui->add(buttonEK3);
	}
	else if (tree->getStatus(48) == 1) {
		tgui::Picture::Ptr buttonEK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEK3->setSize(24, 24);
		buttonEK3->setPosition(524, 467);
		gui->add(buttonEK3);
	}
	else {
		tgui::Picture::Ptr buttonEK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_K.png");
		buttonEK3->setSize(24, 24);
		buttonEK3->setPosition(524, 467);
		buttonEK3->connect("clicked", &GuiHandler::buySkill, this, 48, current_picked_character, tree);
		gui->add(buttonEK3);
	}
	//
	if (tree->getStatus(49) == 2) {
		tgui::Picture::Ptr buttonEW4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W_got.png");
		buttonEW4->setSize(24, 24);
		buttonEW4->setPosition(508, 451);
		gui->add(buttonEW4);
	}
	else if (tree->getStatus(49) == 1) {
		tgui::Picture::Ptr buttonEW4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonEW4->setSize(24, 24);
		buttonEW4->setPosition(508, 451);
		gui->add(buttonEW4);
	}
	else {
		tgui::Picture::Ptr buttonEW4 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_W.png");
		buttonEW4->setSize(24, 24);
		buttonEW4->setPosition(508, 451);
		buttonEW4->connect("clicked", &GuiHandler::buySkill, this, 49, current_picked_character, tree);
		gui->add(buttonEW4);
	}
	//
	if (tree->getStatus(50) == 2) {
		tgui::Picture::Ptr buttonES = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_ES_got.png");
		buttonES->setSize(24, 24);
		buttonES->setPosition(508, 426);
		gui->add(buttonES);
	}
	else if (tree->getStatus(50) == 1) {
		tgui::Picture::Ptr buttonES = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonES->setSize(24, 24);
		buttonES->setPosition(508, 426);
		gui->add(buttonES);
	}
	else {
		tgui::Picture::Ptr buttonES = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeEarth_ES.png");
		buttonES->setSize(24, 24);
		buttonES->setPosition(508, 426);
		buttonES->connect("clicked", &GuiHandler::buySkill, this, 50, current_picked_character, tree);
		gui->add(buttonES);
	}
	//
	if (tree->getStatus(52) == 2) {
		tgui::Picture::Ptr buttonWaW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_W_got.png");
		buttonWaW1->setSize(24, 24);
		buttonWaW1->setPosition(487, 324);
		gui->add(buttonWaW1);
	}
	else if (tree->getStatus(52) == 1) {
		tgui::Picture::Ptr buttonWaW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaW1->setSize(24, 24);
		buttonWaW1->setPosition(487, 324);
		gui->add(buttonWaW1);
	}
	else {
		tgui::Picture::Ptr buttonWaW1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_W.png");
		buttonWaW1->setSize(24, 24);
		buttonWaW1->setPosition(487, 324);
		buttonWaW1->connect("clicked", &GuiHandler::buySkill, this, 52, current_picked_character, tree);
		gui->add(buttonWaW1);
	}
	//
	if (tree->getStatus(53) == 2) {
		tgui::Picture::Ptr buttonWaV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_V_got.png");
		buttonWaV1->setSize(24, 24);
		buttonWaV1->setPosition(471, 308);
		gui->add(buttonWaV1);
	}
	else if (tree->getStatus(53) == 1) {
		tgui::Picture::Ptr buttonWaV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaV1->setSize(24, 24);
		buttonWaV1->setPosition(471, 308);
		gui->add(buttonWaV1);
	}
	else {
		tgui::Picture::Ptr buttonWaV1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_V.png");
		buttonWaV1->setSize(24, 24);
		buttonWaV1->setPosition(471, 308);
		buttonWaV1->connect("clicked", &GuiHandler::buySkill, this, 53, current_picked_character, tree);
		gui->add(buttonWaV1);
	}
	//
	if (tree->getStatus(54) == 2) {
		tgui::Picture::Ptr buttonWaK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_K_got.png");
		buttonWaK1->setSize(24, 24);
		buttonWaK1->setPosition(471, 283);
		gui->add(buttonWaK1);
	}
	else if (tree->getStatus(54) == 1) {
		tgui::Picture::Ptr buttonWaK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaK1->setSize(24, 24);
		buttonWaK1->setPosition(471, 283);
		gui->add(buttonWaK1);
	}
	else {
		tgui::Picture::Ptr buttonWaK1 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_K.png");
		buttonWaK1->setSize(24, 24);
		buttonWaK1->setPosition(471, 283);
		buttonWaK1->connect("clicked", &GuiHandler::buySkill, this, 54, current_picked_character, tree);
		gui->add(buttonWaK1);
	}
	//
	if (tree->getStatus(55) == 2) {
		tgui::Picture::Ptr buttonWaW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_W_got.png");
		buttonWaW2->setSize(24, 24);
		buttonWaW2->setPosition(455, 267);
		gui->add(buttonWaW2);
	}
	else if (tree->getStatus(55) == 1) {
		tgui::Picture::Ptr buttonWaW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaW2->setSize(24, 24);
		buttonWaW2->setPosition(455, 267);
		gui->add(buttonWaW2);
	}
	else {
		tgui::Picture::Ptr buttonWaW2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_W.png");
		buttonWaW2->setSize(24, 24);
		buttonWaW2->setPosition(455, 267);
		buttonWaW2->connect("clicked", &GuiHandler::buySkill, this, 55, current_picked_character, tree);
		gui->add(buttonWaW2);
	}
	//
	if (tree->getStatus(56) == 2) {
		tgui::Picture::Ptr buttonWaB = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_WB_got.png");
		buttonWaB->setSize(24, 24);
		buttonWaB->setPosition(439, 251);
		gui->add(buttonWaB);
	}
	else if (tree->getStatus(56) == 1) {
		tgui::Picture::Ptr buttonWaB = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaB->setSize(24, 24);
		buttonWaB->setPosition(439, 251);
		gui->add(buttonWaB);
	}
	else {
		tgui::Picture::Ptr buttonWaB = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_WB.png");
		buttonWaB->setSize(24, 24);
		buttonWaB->setPosition(439, 251);
		buttonWaB->connect("clicked", &GuiHandler::buySkill, this, 56, current_picked_character, tree);
		gui->add(buttonWaB);
	}
	//
	if (tree->getStatus(57) == 2) {
		tgui::Picture::Ptr buttonWaV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_V_got.png");
		buttonWaV2->setSize(24, 24);
		buttonWaV2->setPosition(414, 251);
		gui->add(buttonWaV2);
	}
	else if (tree->getStatus(57) == 1) {
		tgui::Picture::Ptr buttonWaV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaV2->setSize(24, 24);
		buttonWaV2->setPosition(414, 251);
		gui->add(buttonWaV2);
	}
	else {
		tgui::Picture::Ptr buttonWaV2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_V.png");
		buttonWaV2->setSize(24, 24);
		buttonWaV2->setPosition(414, 251);
		buttonWaV2->connect("clicked", &GuiHandler::buySkill, this, 57, current_picked_character, tree);
		gui->add(buttonWaV2);
	}
	//
	if (tree->getStatus(58) == 2) {
		tgui::Picture::Ptr buttonWaK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_K_got.png");
		buttonWaK2->setSize(24, 24);
		buttonWaK2->setPosition(414, 276);
		gui->add(buttonWaK2);
	}
	else if (tree->getStatus(58) == 1) {
		tgui::Picture::Ptr buttonWaK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaK2->setSize(24, 24);
		buttonWaK2->setPosition(414, 276);
		gui->add(buttonWaK2);
	}
	else {
		tgui::Picture::Ptr buttonWaK2 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_K.png");
		buttonWaK2->setSize(24, 24);
		buttonWaK2->setPosition(414, 276);
		buttonWaK2->connect("clicked", &GuiHandler::buySkill, this, 58, current_picked_character, tree);
		gui->add(buttonWaK2);
	}
	//
	if (tree->getStatus(59) == 2) {
		tgui::Picture::Ptr buttonWaW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_W_got.png");
		buttonWaW3->setSize(24, 24);
		buttonWaW3->setPosition(430, 292);
		gui->add(buttonWaW3);
	}
	else if (tree->getStatus(59) == 1) {
		tgui::Picture::Ptr buttonWaW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaW3->setSize(24, 24);
		buttonWaW3->setPosition(430, 292);
		gui->add(buttonWaW3);
	}
	else {
		tgui::Picture::Ptr buttonWaW3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_W.png");
		buttonWaW3->setSize(24, 24);
		buttonWaW3->setPosition(430, 292);
		buttonWaW3->connect("clicked", &GuiHandler::buySkill, this, 59, current_picked_character, tree);
		gui->add(buttonWaW3);
	}
	//
	if (tree->getStatus(60) == 2) {
		tgui::Picture::Ptr buttonWaT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_WT_got.png");
		buttonWaT->setSize(24, 24);
		buttonWaT->setPosition(430, 317);
		gui->add(buttonWaT);
	}
	else if (tree->getStatus(60) == 1) {
		tgui::Picture::Ptr buttonWaT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaT->setSize(24, 24);
		buttonWaT->setPosition(430, 317);
		gui->add(buttonWaT);
	}
	else {
		tgui::Picture::Ptr buttonWaT = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_WT.png");
		buttonWaT->setSize(24, 24);
		buttonWaT->setPosition(430, 317);
		buttonWaT->connect("clicked", &GuiHandler::buySkill, this, 60, current_picked_character, tree);
		gui->add(buttonWaT);
	}
	//
	if (tree->getStatus(61) == 2) {
		tgui::Picture::Ptr buttonWaK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_K_got.png");
		buttonWaK3->setSize(24, 24);
		buttonWaK3->setPosition(405, 317);
		gui->add(buttonWaK3);
	}
	else if (tree->getStatus(61) == 1) {
		tgui::Picture::Ptr buttonWaK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaK3->setSize(24, 24);
		buttonWaK3->setPosition(405, 317);
		gui->add(buttonWaK3);
	}
	else {
		tgui::Picture::Ptr buttonWaK3 = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_K.png");
		buttonWaK3->setSize(24, 24);
		buttonWaK3->setPosition(405, 317);
		buttonWaK3->connect("clicked", &GuiHandler::buySkill, this, 61, current_picked_character, tree);
		gui->add(buttonWaK3);
	}
	//
	if (tree->getStatus(62) == 2) {
		tgui::Picture::Ptr buttonWaS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_WS_got.png");
		buttonWaS->setSize(24, 24);
		buttonWaS->setPosition(389, 301);
		gui->add(buttonWaS);
	}
	else if (tree->getStatus(62) == 1) {
		tgui::Picture::Ptr buttonWaS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeButton_locked.png");
		buttonWaS->setSize(24, 24);
		buttonWaS->setPosition(389, 301);
		gui->add(buttonWaS);
	}
	else {
		tgui::Picture::Ptr buttonWaS = tgui::Picture::create("Graphics/SkillTreeButtons/SkillTreeWater_WS.png");
		buttonWaS->setSize(24, 24);
		buttonWaS->setPosition(389, 301);
		buttonWaS->connect("clicked", &GuiHandler::buySkill, this, 52, current_picked_character, tree);
		gui->add(buttonWaS);
	}
}
