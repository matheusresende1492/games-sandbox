#include <ostream>
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <deque>

Color retroGreen = { 173, 204, 96, 255 };
Color retroDarkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool eventTriggered(double interval);
bool ElementInDeque(Vector2 element, std::deque<Vector2> deque);

class Food {
	public:
		//x, y cordinates
		//Accessible by position.x and position.y 
		Vector2 position;
		Texture2D texture;

		//Constructor
		Food(std::deque<Vector2> snakeBody) {
			Image image = LoadImage("images/food.png");
			texture = LoadTextureFromImage(image);
			position = GenerateRandomPos(snakeBody);
			UnloadImage(image);
		}

		//Destructor
		~Food() {
			UnloadTexture(texture);
		}
		
		void Draw() {
			//x, y, w, h, color
			//DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, retroDarkGreen);
			//White for no color filter applied on top of the texture
			DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
		}

		Vector2 GenerateRandonCell() {
			float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};
		}
		
		Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody) {
			Vector2 position = GenerateRandonCell();
			while (ElementInDeque(position, snakeBody)) {
				position = GenerateRandonCell();
			}
			return position;
		}
};

class Snake {

	public:
		//deque is a data structure that allows removing and adding elements to head and tail
		//push_front(), pop_front(), push_back(), pop_back()
		std::deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
		Vector2 direction = {1, 0};
		bool addSegment = false;
		
		void Draw() {
			for(unsigned int i = 0; i < body.size(); i++) {
				float x = body[i].x;
				float y = body[i].y;

				//DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, retroDarkGreen);
				Rectangle segment = 
					Rectangle{
						offset + x * cellSize, 
						offset + y * cellSize, 
						(float) cellSize, 
						(float) cellSize
					};
				DrawRectangleRounded(segment, 0.5, 6, retroDarkGreen);
			}
		}

		void Update() {
			body.push_front(Vector2Add(body[0], direction));

			if (addSegment) {
				addSegment = false;
			} else {
				body.pop_back();
			}
		}

		void Reset() {
			body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
			direction = {1, 0};
		}
};

class Game {
	public:
		Snake snake = Snake();
		Food food = Food(snake.body);
		double snakeSpeed = 0.08;
		bool running = true;
		int score = 0;

		Sound eatSound;
		Sound wallSound;

		Game() {
			InitAudioDevice();
			eatSound = LoadSound("sounds/eat.mp3");
			wallSound = LoadSound("sounds/wall.mp3");
		}

		~Game() {
			UnloadSound(eatSound);
			UnloadSound(wallSound);
		}

		void Draw() {
			food.Draw();
			snake.Draw();
		}

		void Update() {
			if (running) {
				snake.Update();
				CheckCollisionWithFood();
				CheckCollisionWithTail();
				CheckCollisionWithMapEdges();
			}
		}	

		void CheckCollisionWithFood() {
			if (Vector2Equals(snake.body[0], food.position)) {
				food.position = food.GenerateRandomPos(snake.body);	
				snake.addSegment = true;
				score++;
				//snakeSpeed -= 0.01;
				PlaySound(eatSound);
			}
		}

		void CheckCollisionWithTail() {
			//Creates a copy of the snake's body
			std::deque<Vector2> headlessBody = snake.body;
			headlessBody.pop_front();

			if (ElementInDeque(snake.body[0], headlessBody)) {
				GameOver();	
			}
		}

		void CheckCollisionWithMapEdges() {
			if (snake.body[0].x == cellCount || snake.body[0].x == -1) { GameOver(); }
			if (snake.body[0].y == cellCount || snake.body[0].y == -1) { GameOver(); }
		}

		void GameOver() {
			snake.Reset();
			food.position = food.GenerateRandomPos(snake.body);
			snakeSpeed = 0.08;
			running = false;
			score = 0;
			PlaySound(wallSound);
		}
};

int main() {
	
	std::cout << "Starting the game.." << "\n";
	InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake Game");
	SetTargetFPS(100);

	Game game = Game();

	// while until press "ESC" or close window
	while (WindowShouldClose() == false) {
		//Start drawing on canvas
		BeginDrawing();

		//triggers the event every 200 millis
		if (eventTriggered(game.snakeSpeed)) {
			game.Update();
		}
		
		//Apply movement and blocks superposition 
		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) { game.snake.direction = {0, -1}; game.running = true; }
		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) { game.snake.direction = {0, 1}; game.running = true; }
		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) { game.snake.direction = {-1, 0}; game.running = true; }
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) { game.snake.direction = {1, 0}; game.running = true; }
	
		//Drawing
		ClearBackground(retroGreen);
		DrawRectangleLinesEx( 
			Rectangle{
				(float) offset - 5, 
				(float) offset - 5, 
				(float) cellSize * cellCount + 10, 
				(float) cellSize * cellCount + 10},
			5,
			retroDarkGreen
		);

		//Drawtext(text, posX, posY, fontSize, color);
		DrawText("Retro Snake", offset - 5, 20, 40, retroDarkGreen);
		DrawText(TextFormat("%i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, retroDarkGreen);

		game.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

//trigger an event every x amount of times
bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque) {
	for(unsigned int i = 0; i < deque.size(); i++) {
		if (Vector2Equals(element, deque[i])) {
			return true;
		}
	}
	return false;
}
