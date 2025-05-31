//Areeb Faisal
//23i-0709
//CS-E




#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {0};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);//This function draws the player sprite
void movePlayer(float player[],sf::Clock& playerClock,int m_freq, float mushroom[][32]);//This function is for the movement of player across the grid
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite, sf::Clock& bulletClock);//This function draws the bullet sprite
void moveBullet(float bullet[], sf::Clock& bulletClock);//This function is for moving the bullet across the map
void drawMushroom(sf::RenderWindow& window,float mushroom[][32],sf::Sprite& mushroomSprite,int m_freq,int mush_health,sf::Sprite& halfmushroomSprite);//This function draws the mushrooms full sprite and half sprite after being hit once
void spawnMushroom(float mushroom[][32],int m_freq,int mush_health);//this function spawns mushrooms at the start of the game and everytime when the mushrooms are all destroyed
void collisionMushroom(float bullet[],float mushroom[][32],int m_freq,int mush_health,int& score);//this function handles the collision between mushrooms and the bullets
void drawCentipede(sf::RenderWindow& window, float centipede[][12], sf::Sprite* centipedeSprite);//this function draws the centipede sprite
// void drawCentipede(sf::RenderWindow& window, float centipede[][12], sf::Sprite*& centipedeSprite);
void moveCentipede(float centipede[][12],sf::Clock& centipedeClock, int cent_dir_x,float mushroom[][32],sf::Sprite centipedeSprite, int& cent_tex_start_x, int& cent_tex_start_y, int& cent_tex_end_x, int& cent_tex_end_y,int cent_dir_y,int level);//this function handles the movement of centipede in the grid
void collisionCentipede(float centipede[][12],float bullet[],float mushroom[][32],int& mush_num,sf::Clock& centipedeClock,float player[],int& score);//This function handles the collision between the centipede and mushrooms as well as the centipede and the bullet
void spawnCentMushroom(float mushroom[][32],float centipede[][12] , int& mush_num, int i);//This function spawns a mushroom each time the centipede is hit with a bullet
void spawnCentipede(float centipede[][12], int& mush_num,int cent_dir_x,int cent_dir_y);//this function sppawns a centipede at the start of the game and also every time the last centipede gets killed
void refreshGame(float mushroom[][32],int& mush_num,int& level);//This function refreshes the board each time a centipede is killed
void showScore(sf::RenderWindow& window, int& score);//this function shows the live score of the game
int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 960)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(0);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[3] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	player[y] = (gameColumns * 4 / 5) * boxPixelsY;
	player[exists]=true;
	sf::Clock playerClock;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));


	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y];
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	const int m_freq = 20;//this is the frequency of mushrooms spawned
	int mush_num=1;
	const int mush_health = 3;//this is the health of the mushrooms
	float mushroom[4][32] = {0};
	sf::Texture mushroomTexture;
	sf::Texture halfmushroomTexture;
	sf::Sprite mushroomSprite;
	sf::Sprite halfmushroomSprite;
	sf::Clock mushroomClock;
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	mushroomSprite.setTexture(mushroomTexture);
	mushroomSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
	halfmushroomTexture.loadFromFile("Textures/mushroom.png");
	halfmushroomSprite.setTexture(halfmushroomTexture);
	halfmushroomSprite.setTextureRect(sf::IntRect(64, 32, 32, 32));
	spawnMushroom(mushroom,m_freq,mush_health);

	const int cent_dir_x = 3; //This is for the direction of the centipede along the x-axis with true being left and false being right
	const int cent_dir_y = 4;//This is for the direction of the centipede along the y-axis with true being downward and false being upward
	float centipede[5][12]={};
	centipede[x][0]=930;
	centipede[y][0]=0;
	for (int i=0;i<12;i++){
		centipede[exists][i]=false;
	}
	spawnCentipede(centipede,mush_num,cent_dir_x,cent_dir_y);
	int cent_tex_start_x=0;
	int cent_tex_start_y=0;
	int cent_tex_end_x=28;
	int cent_tex_end_y=32;
	sf::Texture centipedeTexture;
	sf::Sprite centipedeSprite[12];
	sf::Clock centipedeClock;
	centipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	for (int i=0;i<12;i++){
		centipedeSprite[i].setTexture(centipedeTexture);
		centipedeSprite[i].setTextureRect(sf::IntRect(cent_tex_start_x ,cent_tex_start_y ,cent_tex_end_x ,cent_tex_end_y));
	}
	int score =0;//this is the score of the game
	int level=1;//this is the level of the game
	while(window.isOpen()) {

		///////////////////////////////////////////////////////////////
		//                                                           //
		// Call Your Functions Here. Some have been written for you. //
		// Be vary of the order you call them, SFML draws in order.  //
		//                                                           //
		///////////////////////////////////////////////////////////////

		window.draw(backgroundSprite);
		drawMushroom(window,mushroom,mushroomSprite,m_freq,mush_health,halfmushroomSprite);
		collisionMushroom(bullet,mushroom,m_freq,mush_health,score);
		drawCentipede(window,centipede,centipedeSprite);
		moveCentipede(centipede,centipedeClock,cent_dir_x,mushroom,centipedeSprite[12], cent_tex_start_x,cent_tex_start_y,cent_tex_end_x,cent_tex_end_y,cent_dir_y,level);
		collisionCentipede(centipede,bullet,mushroom,mush_num,centipedeClock,player,score);
		showScore(window,score);
		for (int i=0,j=0;i<12;i++){
			if (centipede[exists][i]==false){
				j++;
			}
			if (j==12){
				mush_num=1;
				refreshGame(mushroom,mush_num,level);
				spawnCentipede(centipede,mush_num,cent_dir_x,cent_dir_y);
			}
		}
		if (bullet[exists] == false){
			bullet[x] = player[x];
			bullet[y] = player[y] - boxPixelsY;
		}
			drawBullet(window, bullet, bulletSprite, bulletClock);
		if (player[exists]==true){
			movePlayer(player,playerClock,m_freq,mushroom);
			drawPlayer(window, player, playerSprite);
		}


		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed || player[exists]==false) {
				return 0;
			}
		}
		window.display();
		window.clear();
	}
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}



void movePlayer(float player[],sf::Clock& playerClock,int m_freq, float mushroom[][32]) {
	if (playerClock.getElapsedTime().asMilliseconds() < 2)
		return;

	playerClock.restart();
	const int playerSpeed=1.0f;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[x] > 0 ) {
				player[x] -= playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[x] <resolutionX-boxPixelsX) {
				player[x] += playerSpeed;
	}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[y] > 0 && player[y] > resolutionY-boxPixelsY*5) {
				player[y] -= playerSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[y]< resolutionY-boxPixelsY) {
				player[y] += playerSpeed;

	}

}
void moveBullet(float bullet[], sf::Clock& bulletClock) {
    float bulletSpeed = 4.0f;  // This is the speed of the bullet

    if (bulletClock.getElapsedTime().asMilliseconds() >= 2) {
        bulletClock.restart();
        bullet[y] -= bulletSpeed ;
    }

    if (bullet[y] < -32) {
        bullet[exists] = false;
    }
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite, sf::Clock& bulletClock) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)){
			bullet[exists]=true;
	for (int i=0;i<30;i++){
		for (int j=0;j<30;j++){
			cout<<gameGrid[j][i];}cout<<endl;}
	}
	if (bullet[exists] == true)  {
		bulletSprite.setPosition(bullet[x], bullet[y]);
		window.draw(bulletSprite);
		moveBullet(bullet, bulletClock);
	}

}

void drawMushroom(sf::RenderWindow& window, float mushroom[][32], sf::Sprite& mushroomSprite,int m_freq,int mush_health,sf::Sprite& halfmushroomSprite) {

	for (int i=0 ; i < m_freq+12 ; i++){
		if (mushroom[exists][i]==true && mushroom[mush_health][i]==2){
			mushroomSprite.setPosition(mushroom[x][i], mushroom[y][i]);
			window.draw(mushroomSprite);
		}
		if (mushroom[exists][i]==true && mushroom[mush_health][i]==1){
			halfmushroomSprite.setPosition(mushroom[x][i], mushroom[y][i]);
			window.draw(halfmushroomSprite);
		}
	}
	for (int i=0,j=0; i< m_freq ; i++){
		if (mushroom[exists][i]==false)
			j++;

		if (j==20)
			spawnMushroom(mushroom,m_freq,mush_health);
	}



}
void spawnMushroom(float mushroom[][32],int m_freq,int mush_health){
	for (int i=0 ; i < m_freq ; i++){
		mushroom[exists][i] = true;
		mushroom[x][i] = rand()%gameRows * boxPixelsX;
		mushroom[y][i] = rand()%(gameColumns-5) * boxPixelsY;
		mushroom[mush_health][i]=2;
	}
	for (int i=m_freq ; i < m_freq+12 ; i++){
		mushroom[mush_health][i]=2;
	}

	for (int i = 0; i < m_freq+12; i++) {
		int gridX=mushroom[x][i]/boxPixelsX;
		int gridY=mushroom[y][i]/ boxPixelsY;

		if (mushroom[exists][i]==true) {
			gameGrid[gridX][gridY] = true;
		}
	}
}

void collisionMushroom(float bullet[], float mushroom[][32], int m_freq,int mush_health,int& score) {
    for (int i = 0; i < m_freq+12; i++) {
        if (bullet[x] + boxPixelsX > mushroom[x][i] && bullet[x] < mushroom[x][i] + boxPixelsX &&
            bullet[y] + boxPixelsY > mushroom[y][i] && bullet[y] < mushroom[y][i] + boxPixelsY &&
            mushroom[exists][i] == true && bullet[exists]==true) {
            mushroom[mush_health][i]-=1;
			if (mushroom[mush_health][i]==0){
				mushroom[exists][i] = false;
				score+=5;
				int gridX = static_cast<int>(mushroom[x][i] / boxPixelsX);
				int gridY = static_cast<int>(mushroom[y][i] / boxPixelsY);
				if (gridX >= 0 && gridX < gameColumns && gridY >= 0 && gridY < gameRows) {
					gameGrid[gridX][gridY] = 0;
				}
			}
            bullet[exists] = false;

        }
    }
}
void drawCentipede(sf::RenderWindow& window, float centipede[][12], sf::Sprite* centipedeSprite) {
    for (int i = 0; i < 12; i++) {
        if (centipede[exists][i] == true) {
            centipedeSprite[i].setPosition(centipede[x][i], centipede[y][i]);
            window.draw(centipedeSprite[i]);
        }
    }
}


void moveCentipede(float centipede[][12],sf::Clock& centipedeClock,int cent_dir_x, float mushroom[][32],sf::Sprite centipedeSprite, int& cent_tex_start_x, int& cent_tex_start_y, int& cent_tex_end_x, int& cent_tex_end_y,int cent_dir_y,int level){
	float centipedeSpeed = 1;
	if (centipedeClock.getElapsedTime().asMilliseconds() < 2)
		return;
	centipedeClock.restart();

	for (int i=0;i<12;i++){
		if(centipede[cent_dir_y][i]==true){
			if (centipede[cent_dir_x][i]==true && !(centipede[x][i]==0)){
				centipede[x][i]-=centipedeSpeed;
			}
			if (centipede[x][i]==0 || gameGrid[static_cast<int>(centipede[x][i]/32)][static_cast<int>(centipede[y][i]/32)]){
				centipede[y][i]+=boxPixelsY;
				centipede[cent_dir_x][i]=false;
			}
			if (centipede[cent_dir_x][i]==false && !(centipede[x][i]==930) ){
				centipede[x][i]+=centipedeSpeed;
			}
			if (centipede[x][i]==930 || gameGrid[static_cast<int>(centipede[x][i]/32)][static_cast<int>(centipede[y][i]/32)]){
				centipede[y][i]+=boxPixelsY;
				centipede[cent_dir_x][i]=true;
			}
			if (centipede[x][i]/boxPixelsX==29 && centipede[y][i]/boxPixelsY==29){
				centipede[cent_dir_y][i]=false;
			}
		}
		else {

			if (centipede[cent_dir_x][i]==true && !(centipede[x][i]==0)){
				centipede[x][i]-=centipedeSpeed;
			}
			if (centipede[x][i]==0 || gameGrid[static_cast<int>(centipede[x][i]/32)][static_cast<int>(centipede[y][i]/32)]){
				centipede[y][i]-=boxPixelsY;
				centipede[cent_dir_x][i]=false;
			}
			if (centipede[cent_dir_x][i]==false && !(centipede[x][i]==930) ){
				centipede[x][i]+=centipedeSpeed;
			}
			if (centipede[x][i]==930 || gameGrid[static_cast<int>(centipede[x][i]/32)][static_cast<int>(centipede[y][i]/32)]){
				centipede[y][i]-=boxPixelsY;
				centipede[cent_dir_x][i]=true;
			}
		}

	}


}


void collisionCentipede(float centipede[][12], float bullet[],float mushroom[][32],int& mush_num, sf::Clock& centipedeClock ,float player[],int& score) {
    for (int i = 0; i < 12; i++) {
        if (bullet[x] + boxPixelsX > centipede[x][i] && bullet[x] < centipede[x][i]+boxPixelsX  && bullet[y] +boxPixelsY > centipede[y][i] && bullet[y] < centipede[y][i]+boxPixelsY  && centipede[exists][i] == true && bullet[exists]==true) {
            centipede[exists][i] = false;
            bullet[exists] = false;
            score+=10;
            spawnCentMushroom(mushroom,centipede,mush_num,i);
			if (centipedeClock.getElapsedTime().asMilliseconds() < 10)
				return;
			centipedeClock.restart();
        }
		if (player[x] + boxPixelsX > centipede[x][i] && player[x] < centipede[x][i] +boxPixelsX && player[y] + boxPixelsY > centipede[y][i] && player[y] < centipede[y][i]+boxPixelsY && centipede[exists][i] == true && player[exists] == true) {
            player[exists] = false;
        }
    }
}

void spawnCentMushroom(float mushroom[][32], float centipede[][12], int& mush_num, int i) {
    int gridX = static_cast<int>(centipede[x][i] / boxPixelsX);
    int gridY = static_cast<int>(centipede[y][i] / boxPixelsY);

    if (gridX >= 0 && gridX < gameColumns && gridY >= 0 && gridY < gameRows) {
        mushroom[x][19 + mush_num] = centipede[x][i];
        mushroom[y][19 + mush_num] = centipede[y][i];
        mushroom[exists][19 + mush_num] = true;
        gameGrid[gridX][gridY] = true;
        mush_num++;
    }
}

void spawnCentipede(float centipede[][12],int& mush_num,int cent_dir_x, int cent_dir_y){
	for (int i=0;i<12;i++){
		centipede[x][i] = 570 +(i*boxPixelsX);
		centipede[y][i] = 0;
		centipede[cent_dir_x][i] = true;
		centipede[cent_dir_y][i]=true;
		centipede[exists][i]=true;
	}
}
void refreshGame(float mushroom[][32],int& mush_num,int& level){
	level+=1;
	for (int i=0;i<32;i++){
		mushroom[exists][i]=false;
		int gridX = static_cast<int>(mushroom[x][i] / boxPixelsX);
		int gridY = static_cast<int>(mushroom[y][i] / boxPixelsY);
		if (gridX >= 0 && gridX < gameColumns && gridY >= 0 && gridY < gameRows) {
			gameGrid[gridX][gridY] = 0;
		}
	}
}
void showScore(sf::RenderWindow& window, int& score){
	sf::Font font;
    font.loadFromFile("Fonts/VTCSundaykomix.ttf");


    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(10, 10);

    window.draw(scoreText);
}




































































