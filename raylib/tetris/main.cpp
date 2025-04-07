#include <iostream>
#include <raylib.h>
#include <vector>

double lastUpdateTime = 0;
int cellSize = 25;
int cellCount = 30;

bool eventTrigerred(double interval);

std::vector<Vector2> GetPieceBody(int pieceType) {
	switch (pieceType) {
				case 1:
					return {Vector2{2, 6}, Vector2{3, 6}, Vector2{4, 6}, Vector2{2, 7}};
				case 2:
					return {Vector2{10, 5}, Vector2{11, 5}, Vector2{12, 5}, Vector2{13, 5}};
				default:
					return {};
	}
}

class Piece {
	public:
		std::vector<Vector2> body;

		Piece(int format) {
			body = GetPieceBody(format);	
		}

		void Draw(Color color) {
			for(unsigned int i = 0; i < body.size(); i++) {
				float x = body[i].x;	
				float y = body[i].y;

				DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
			}
		}

		void Update() {
			this->MovePieceDownards();
		}

		int GetRandomPieceFormat() {
			return GetRandomValue(1, 2);
		}

		void MovePieceDownards() {
			for(unsigned int i = 0; i < body.size(); i++) {
				body[i].y++;
			}
		}

		void MovePieceToTheRight() {
			for(unsigned int i = 0; i < body.size(); i++) {	
				body[i].x++;
			}
		}

		void MovePieceToTheLeft() {
			for(unsigned int i = 0; i < body.size(); i++) {	
				body[i].x--;
			}
		}

		void TryMoveToTheLeft() {
			for(unsigned int i = 0; i < body.size(); i++) {	
        		if (body[i].x - 1 < 0){
					this->MovePieceToTheLeft();
				}
        	}
		}

		void TryMoveToTheRight() {
			for(unsigned int i = 0; i < body.size(); i++) {	
        		if (body[i].x + 1 >= cellCount){
					this->MovePieceToTheRight();
				}
        	}
		}
};

int main() {
	
	std::cout << "Starting TETRIS game..." << "\n";

	InitWindow(cellSize * cellCount, cellSize * cellCount, "TETRIS");
	SetTargetFPS(100);

	Piece piece1 = Piece(1);
	Piece piece2 = Piece(2);

	while(WindowShouldClose() == false) {
		BeginDrawing();
		
		if (eventTrigerred(0.2)) {
			piece1.Update();
			piece2.Update();
		}	

		bool moved = false;

		if (IsKeyPressed(KEY_RIGHT)) {
			piece1.TryMoveToTheRight();
			moved = true;
		}

		if (IsKeyPressed(KEY_LEFT)) {
			piece1.TryMoveToTheLeft();
			moved = true;
		}

		if (eventTrigerred(0.2)) {
			if (IsKeyDown(KEY_RIGHT) && !moved) {
				piece1.TryMoveToTheRight();
			}
		
			if (IsKeyDown(KEY_LEFT) && !moved) {
				piece1.TryMoveToTheLeft();
			}
		
			lastUpdateTime = GetTime();
		}
		


		piece1.Draw(BLACK);
		piece2.Draw(RED);


		ClearBackground(GRAY);

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
