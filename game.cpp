#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;

const int mapHeight = 12, mapWeight = 90;
float offsetX = 0, offsetY = 0;

String tileMap[mapHeight] =
{
	"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
	"0                                                                  V              V      0",
	"0             C       V         C                 V        c    C         C              0",
	"0        V                   c          C                                 OO             0",
	"0                   c    V        C            C       V           V ALP O  O    O   O   0",
	"0    C       C               c            c       V        c    c      c O      OOO OOO  0",
	"0      C        V                V     V              C          ALP     O  O    O   O   0",
	"0  C       c         V    C               V     v           O V   c       OO             0",
	"0 u         O         gr      u     C                       ALP   u                      0",
	"0 yS gBBHBmrOgBH     <G>      yugr gHBm    BBBrMgug   gBH        gyrmBB  gBBBBBBrgM      0",
	"<GGGGGGGGGGGGGGGWWWWWWWWWWWWWGGGGGGGGGGGGGGGGGGGGGGGGGGGGWWWWWWWGGGGGGGGGGGGGGGGGGGGGGGGG>",
	"0DDDDDDDDDDDDDDDUUUUUUUUUUUUUDDDDDDDDDDDDDDDDDDDDDDDDDDDDUUUUUUUDDDDDDDDDDDDDDDDDDDDDDDDD0",
};

class Player
{
public:
	float dx, dy;
	FloatRect rect;
	Texture texture;
	Sprite sprite;
	float currentFrame;
	bool onGround;
	Player()
	{
		texture.loadFromFile("p1_front.png");
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, 66, 92));
		rect = FloatRect(207, 207, 72, 97);
		dx = dy = 0;
		currentFrame = 0;

	}
	void dead()
	{
		rect.top = 207;
		rect.left = 207;
	}
	void collisionX()
	{
		for (int i = rect.top / 67; i<(rect.top + rect.height) / 67; i++)
			for (int j = rect.left / 67; j<(rect.left + rect.width) / 67; j++)
			{
				if ((tileMap[i][j] == 'G') || (tileMap[i][j] == '0') || (tileMap[i][j] == 'A') || (tileMap[i][j] == 'L') || (tileMap[i][j] == 'P')
					|| (tileMap[i][j] == '<') || (tileMap[i][j] == '>'))
				{
					if (dx > 0) rect.left = j * 67 - rect.width;
					if (dx < 0) rect.left = j * 67 + 67;
				}
			}
	}

	void collisionY()
	{
		for (int i = rect.top / 67; i<(rect.top + rect.height) / 67; i++)
			for (int j = rect.left / 67; j<(rect.left + rect.width) / 67; j++)
			{
				if ((tileMap[i][j] == 'G') || (tileMap[i][j] == '0') || (tileMap[i][j] == 'A') || (tileMap[i][j] == 'L') || (tileMap[i][j] == 'P')
					|| (tileMap[i][j] == '<') || (tileMap[i][j] == '>'))
				{
					if (dy > 0)
					{
						rect.top = i * 67 - rect.height;
						dy = 0;
						onGround = true;
					}
					if (dy < 0)
					{
						rect.top = i * 67 + 67;
						dy = 0;
					}
				}
				if ((tileMap[i][j] == 'W') || (tileMap[i][j] == 'U'))
					dead();
			}
	}


	void redraw(float time)
	{
		rect.left += dx*time;
		collisionX();

		if (!onGround)
		{
			dy = dy + 0.0005*time;
		}
		rect.top += dy*time;
		//onGround = false;
		collisionY();

		if (onGround)
		{
			currentFrame += 0.01*time;
			if (currentFrame > 11) currentFrame -= 11;

			if (dx < 0) sprite.setTextureRect(IntRect(72 * int(currentFrame) + 72, 0, -72, 97));
			if (dx > 0) sprite.setTextureRect(IntRect(72 * int(currentFrame), 0, 72, 97));
		}
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
		//std::cout<<"Y - "<<rect.top<<std::endl<<"X - "<<rect.left<<std::endl<<std::endl;
	}

};

class Enemy
{
public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool isAlive;

	Enemy(Texture &image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x, y, 50, 25);
		dx = 0.05;
		currentFrame = 0;
		isAlive = true;
	}

	void redraw(float time, int dead_x, int y, float w, float h)
	{
		rect.left += dx * time;

		collision();

		currentFrame += 0.005*time;
		if (currentFrame > 2) currentFrame -= 2;

		if (dx < 0) sprite.setTextureRect(IntRect(w*int(currentFrame), y, w, h));
		if (dx > 0) sprite.setTextureRect(IntRect(w*int(currentFrame) + w, y, -w, h));
		if (!isAlive) sprite.setTextureRect(IntRect(dead_x, y, w, h));


		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

	}


	void collision()
	{
		for (int i = rect.top / 67; i<(rect.top + rect.height) / 67; i++)
			for (int j = rect.left / 67; j<(rect.left + rect.width) / 67; j++)
				if ((tileMap[i][j] == 'm') || (tileMap[i][j] == 'B') || (tileMap[i][j] == 'G') || (tileMap[i][j] == 'c'))
				{
					if (dx > 0)
					{
						rect.left = j * 67 - rect.width;
						dx *= -1;
					}
					else if (dx < 0)
					{
						rect.left = j * 67 + 67;
						dx *= -1;
					}

				}
	}

};

int main()
{
	RenderWindow window(VideoMode(1024, 420), "My SFML Platformer");
	window.setPosition(Vector2i(50, 150));

	SoundBuffer buffer;
	buffer.loadFromFile("jump.ogg");
	Sound sound(buffer);

	Player player;
	Clock clock;

	Texture tileTexture;
	Sprite tileSprite;
	tileTexture.loadFromFile("world.png");
	tileSprite.setTexture(tileTexture);

	Texture enemyTexture;
	enemyTexture.loadFromFile("enemies.png");
	Enemy enemy1(enemyTexture, 2721, 640);
	Enemy enemy2(enemyTexture, 1138, 700);
	Enemy enemy3(enemyTexture, 4149, 325);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 800;

		if (player.onGround)
		{
			player.texture.loadFromFile("p1_front.png");
			player.sprite.setTexture(player.texture);
			player.sprite.setTextureRect(IntRect(0, 0, 66, 92));
		}

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (player.onGround)
			{
				player.texture.loadFromFile("player_walk.png");
				player.sprite.setTexture(player.texture);
				player.sprite.setTextureRect(IntRect(0, 0, 72, 97));
			}
			if (!player.onGround)
			{
				player.texture.loadFromFile("p1_jump.png");
				player.sprite.setTexture(player.texture);
				player.sprite.setTextureRect(IntRect(67, 0, -67, 94));
			}

			player.dx = -0.4;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (player.onGround)
			{
				player.texture.loadFromFile("player_walk.png");
				player.sprite.setTexture(player.texture);
				player.sprite.setTextureRect(IntRect(0, 0, 72, 97));
			}
			if (!player.onGround)
			{
				player.texture.loadFromFile("p1_jump.png");
				player.sprite.setTexture(player.texture);
				player.sprite.setTextureRect(IntRect(0, 0, 67, 94));
			}
			player.dx = 0.4;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (player.onGround)
			{
				player.texture.loadFromFile("p1_jump.png");
				player.sprite.setTexture(player.texture);
				player.sprite.setTextureRect(IntRect(0, 0, 67, 94));
				player.dy = -0.4;
				player.onGround = false;
				sound.play();
			}
		}

		player.redraw(time);
		enemy1.redraw(time, 105, 81, 50, 25);
		enemy2.redraw(time, 105, 36, 64.5, 41);
		enemy3.redraw(time, 147, 0, 70, 35);

		if (player.rect.left > 200) offsetX = player.rect.left - 200;
		if (player.rect.top > 195) offsetY = player.rect.top - 195;

		if (player.rect.intersects(enemy1.rect))
		{
			if (enemy1.isAlive)
			{
				if (player.dy > 0)
				{
					enemy1.dx = 0;
					player.dy = -0.2;
					enemy1.isAlive = false;
				}
				else player.sprite.setColor(Color::Red);
			}
		}

		if (player.rect.intersects(enemy3.rect))
		{
			if (enemy3.isAlive)
			{
				if (player.dy > 0)
				{
					enemy3.dx = 0;
					player.dy = -0.2;
					enemy3.isAlive = false;
				}
				else player.sprite.setColor(Color::Red);
			}
		}

		window.clear(Color(208, 244, 247));

		for (int i = 0; i < mapHeight; i++)
			for (int j = 0; j < mapWeight; j++)
			{
				if (tileMap[i][j] == 'G') tileSprite.setTextureRect(IntRect(503, 575, 69, 70));
				if (tileMap[i][j] == 'D') tileSprite.setTextureRect(IntRect(575, 863, 70, 70));
				if (tileMap[i][j] == 'A') tileSprite.setTextureRect(IntRect(576, 360, 70, 70));

				if (tileMap[i][j] == 'C') tileSprite.setTextureRect(IntRect(914, 0, 128, 70));
				if (tileMap[i][j] == 'c') tileSprite.setTextureRect(IntRect(914, 73, 128, 70));
				if (tileMap[i][j] == 'V') tileSprite.setTextureRect(IntRect(914, 146, 128, 70));

				if (tileMap[i][j] == 'L') tileSprite.setTextureRect(IntRect(576, 288, 70, 70));
				if (tileMap[i][j] == 'P') tileSprite.setTextureRect(IntRect(576, 216, 70, 70));

				if (tileMap[i][j] == 'B') tileSprite.setTextureRect(IntRect(648, 144, 70, 70));
				if (tileMap[i][j] == 'H') tileSprite.setTextureRect(IntRect(648, 72, 70, 70));

				if (tileMap[i][j] == 'g') tileSprite.setTextureRect(IntRect(914, 363, 70, 70));
				if (tileMap[i][j] == 'r') tileSprite.setTextureRect(IntRect(914, 219, 70, 70));
				if (tileMap[i][j] == 'M') tileSprite.setTextureRect(IntRect(986, 291, 70, 70));
				if (tileMap[i][j] == 'm') tileSprite.setTextureRect(IntRect(986, 219, 70, 70));

				if (tileMap[i][j] == 'u') tileSprite.setTextureRect(IntRect(842, 720, 70, 70));
				if (tileMap[i][j] == 'y') tileSprite.setTextureRect(IntRect(842, 792, 70, 70));

				if (tileMap[i][j] == 'O') tileSprite.setTextureRect(IntRect(0, 792, 70, 70));

				if (tileMap[i][j] == '>') tileSprite.setTextureRect(IntRect(575, 575, 70, 70));
				if (tileMap[i][j] == 'S') tileSprite.setTextureRect(IntRect(291, 215, 69, 70));
				if (tileMap[i][j] == '<') tileSprite.setTextureRect(IntRect(575, 719, 70, 70));

				if (tileMap[i][j] == 'W') tileSprite.setTextureRect(IntRect(431, 575, 70, 70));
				if (tileMap[i][j] == 'U') tileSprite.setTextureRect(IntRect(503, 215, 70, 70));

				if ((tileMap[i][j] == ' ') || (tileMap[i][j] == '0')) continue;

				tileSprite.setPosition(j * 67 - offsetX, i * 66 - offsetY);
				window.draw(tileSprite);
			}

		window.draw(player.sprite);
		window.draw(enemy1.sprite);
		window.draw(enemy2.sprite);
		window.draw(enemy3.sprite);

		window.display();

	}

	return 0;
}
