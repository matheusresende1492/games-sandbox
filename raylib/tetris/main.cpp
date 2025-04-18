#include <iostream>
#include <raylib.h>
#include <vector>

double lastUpdateTime = 0;
const int cellSize = 25;
const int cellCount = 30;

bool eventTrigerred(double interval);

class Piece {
	public:
		std::vector<Vector2> body;
		Color color;

		Piece() {
			this->GeneratePiece();
		}

		void Draw() {
			for(unsigned int i = 0; i < body.size(); i++) {
				float x = body[i].x;	
				float y = body[i].y;

				DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
			}
		}

		void Update() {
			this->MovePieceDownards();
		}

		void MovePieceDownards() {
			for(unsigned int i = 0; i < body.size(); i++) {
				body[i].y++;
			}
		}

		void MovePieceToTheRight() {
			if (this->CheckPossibleRightMovement()) {
				for(unsigned int i = 0; i < body.size(); i++) {	
					body[i].x++;
				}
			}
		}

		void MovePieceToTheLeft() {
			if (this->CheckPossibleLeftMovement()) {
				for(unsigned int i = 0; i < body.size(); i++) {	
					body[i].x--;
				}
			}
		}
		
		bool CheckPossibleLeftMovement() {
			for(unsigned int i = 0; i < body.size(); i++) {	
        		if (body[i].x - 1 < 0){
					return false;
				}
        	}
			return true;
		}

		bool CheckPossibleRightMovement() {
			for(unsigned int i = 0; i < body.size(); i++) {	
        		if (body[i].x + 1 >= cellCount){
					return false;
				}
        	}
			return true;
		}

		void GeneratePiece() {
			int pieceType =  GetRandomValue(1, 7);
			switch (pieceType) {
				case 1: {
					this->color = PINK;
					float starting_x = GetRandomValue(0, cellCount - 3);
					this->body = { 
						Vector2{starting_x, -2}, 
						Vector2{starting_x + 1, -2}, 
						Vector2{starting_x + 2, -2}, 
						Vector2{starting_x, -1}
					};
					break;
				}
				case 2: {
					this->color = RED;
					float starting_x = GetRandomValue(0, cellCount - 4);
					this->body = { 
						Vector2{starting_x, -1}, 
						Vector2{starting_x + 1, -1}, 
						Vector2{starting_x + 2, -1}, 
						Vector2{starting_x + 3, -1}
					};
					break;
				}
				case 3: {
					this->color = ORANGE;
					float starting_x = GetRandomValue(0, cellCount - 3);
					this->body = { 
						Vector2{starting_x, -2}, 
						Vector2{starting_x + 1, -2}, 
						Vector2{starting_x + 1, -1}, 
						Vector2{starting_x + 2, -2}
					};
					break;
				}
				case 4: {
					this->color = GREEN;
					float starting_x = GetRandomValue(0, cellCount - 3);
					this->body = { 
						Vector2{starting_x, -1}, 
						Vector2{starting_x + 1, -1}, 
						Vector2{starting_x + 1, -2}, 
						Vector2{starting_x + 2, -2}
					};
					break;
				}
				case 5: {
					this->color = BLUE;
					float starting_x = GetRandomValue(0, cellCount - 3);
					this->body = { 
						Vector2{starting_x, -2}, 
						Vector2{starting_x + 1, -1}, 
						Vector2{starting_x + 1, -2}, 
						Vector2{starting_x + 2, -1}
					};
					break;
				}
				case 6: {
					this->color = YELLOW;
					float starting_x = GetRandomValue(0, cellCount - 2);
					this->body = { 
						Vector2{starting_x, -1}, 
						Vector2{starting_x, -2}, 
						Vector2{starting_x + 1, -1}, 
						Vector2{starting_x + 1, -2}
					};
					break;
				}
				case 7: {
					this->color = WHITE;
					float starting_x = GetRandomValue(0, cellCount - 3);
					this->body = { 
						Vector2{starting_x, -2}, 
						Vector2{starting_x + 1, -2}, 
						Vector2{starting_x + 2, -1}, 
						Vector2{starting_x + 2, -2}
					};
					break;
				}
				default:
					//throw error, invalid piece!
					break;
			}
		}
};

class Game {
	public:
		Piece piece = Piece();
		bool game_matrix[][cellCount];

		Game() {
		}

		~Game() {}

		void Update() {
			piece.Update();
		}

		void Draw() {
			piece.Draw();
		}

		void CheckPieceColisionWithGameMatrix() {

		}
};

int main() {
	
	std::cout << "Starting TETRIS game..." << "\n";

	InitWindow(cellSize * cellCount, cellSize * cellCount, "TETRIS");
	SetTargetFPS(15);

	Game game = Game();

	while(WindowShouldClose() == false) {
		BeginDrawing();
		
		if (eventTrigerred(0.8)) {
			game.Update();
		}	

		if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
			game.piece.MovePieceToTheRight();
		}

		if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)) {
			game.piece.MovePieceToTheLeft();
		}

		game.Draw();

		ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

bool eventTrigerred(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}	
