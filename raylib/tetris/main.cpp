#include <iostream>
#include <raylib.h>
#include <vector>

double lastUpdateTime = 0;
const int cellSize = 25;
const int widthCellCount = 15;
const int heightCellCount = 30;
const int playableWidth = widthCellCount * cellSize;
const int playableHeigth = heightCellCount * cellSize;
const int offset = 75;
const int screenWidth = 3 * offset + cellSize * widthCellCount;
const int screenHeight = cellSize * heightCellCount;

struct matrix_tile {
	bool on;
	Color color;
};

matrix_tile game_matrix[widthCellCount][heightCellCount];

void rotacionate(std::vector<Vector2>& body, bool clockwise); 
bool eventTrigerred(double interval);

class Piece {
	public:
		std::vector<Vector2> body;
		Color color;
		//add enum of piece type

		Piece() {
			GeneratePiece();
		}

		~Piece(){}

		void rotate(bool clockwise) {
			rotacionate(body, clockwise);	
		}

		void Draw() {
			for(unsigned int i = 0; i < body.size(); i++) {
				if (body[i].y >= (int)(offset/cellSize)) {
					float x = body[i].x;	
					float y = body[i].y;

					DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
				}	
			}
		}

		void Update() {
			MovePieceDownards();
		}

		void MovePieceDownards() {
			for(unsigned int i = 0; i < body.size(); i++) {
				body[i].y++;
			}
		}

		void MoveToTheRight() {
			if (CheckPossibleRightMovement()) {
				for(unsigned int i = 0; i < body.size(); i++) {	
					body[i].x++;
				}
			}
		}

		void MoveToTheLeft() {
			if (CheckPossibleLeftMovement()) {
				for(unsigned int i = 0; i < body.size(); i++) {	
					body[i].x--;
				}
			}
		}
		
		bool CheckPossibleLeftMovement() {
			for(unsigned int i = 0; i < body.size(); i++) {	
        		if ((body[i].x - 1) * cellSize < offset || game_matrix[(int)body[i].x - 1][(int)body[i].y].on){
					return false;
				}
        	}
			return true;
		}

		bool CheckPossibleRightMovement() {
			for(unsigned int i = 0; i < body.size(); i++) {	
        		if (body[i].x + 1 >= widthCellCount || game_matrix[(int)body[i].x + 1][(int)body[i].y].on){
					return false;
				}
        	}
			return true;
		}

		void GeneratePiece() {
			int pieceType =  GetRandomValue(1, 7);
			int relative_y_0 = (int)(offset/cellSize);
			switch (pieceType) {
				case 1: {
					this->color = PINK;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 3);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 2, (float) relative_y_0 - 2}, 
						Vector2{starting_x, (float) relative_y_0 -1}
					};
					break;
				}
				case 2: {
					this->color = RED;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 4);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 -1}, 
						Vector2{starting_x + 1, (float) relative_y_0 -1}, 
						Vector2{starting_x + 2, (float) relative_y_0 -1}, 
						Vector2{starting_x + 3, (float) relative_y_0 -1}
					};
					break;
				}
				case 3: {
					this->color = ORANGE;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 3);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 1}, 
						Vector2{starting_x + 2, (float) relative_y_0 - 2}
					};
					break;
				}
				case 4: {
					this->color = GREEN;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 3);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 -1}, 
						Vector2{starting_x + 1, (float) relative_y_0 -1}, 
						Vector2{starting_x + 1, (float) relative_y_0 -2}, 
						Vector2{starting_x + 2, (float) relative_y_0 -2}
					};
					break;
				}
				case 5: {
					this->color = BLUE;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 3);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 1}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 2, (float) relative_y_0 - 1}
					};
					break;
				}
				case 6: {
					this->color = YELLOW;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 2);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 - 1}, 
						Vector2{starting_x, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 1}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 2}
					};
					break;
				}
				case 7: {
					this->color = WHITE;
					float starting_x = GetRandomValue(offset/cellSize, widthCellCount - 3);
					this->body = { 
						Vector2{starting_x, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 1, (float) relative_y_0 - 2}, 
						Vector2{starting_x + 2, (float) relative_y_0 - 1}, 
						Vector2{starting_x + 2, (float) relative_y_0 - 2}
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
		bool running = true;
		Piece piece = Piece();

		Game() {
			InitializeGameMatrix();
		}

		~Game() {}

		void Update() {
			if (running) {
				if (CheckPieceColisionX()) {
					TransferPieceToGameMatrix(piece);
					piece = Piece();
				} else {
					piece.Update();
				}
			}
		}

		void Draw() {
			piece.Draw();
			DrawMatrix();
		}
		
		void TransferPieceToGameMatrix(Piece piece) {
			for(unsigned int i = 0; i < piece.body.size(); i++) {	
				game_matrix[(int)piece.body[i].x][(int)piece.body[i].y].on = true;
				game_matrix[(int)piece.body[i].x][(int)piece.body[i].y].color = piece.color;
        	}
		}

		bool CheckPieceColisionX() {
			for(unsigned int i = 0; i < piece.body.size(); i++) {
				int x = (int)piece.body[i].x;
				int y = (int)piece.body[i].y;
				if (y < 0) {
					return false;
				}
				if(CheckPieceColisionWithFloor(y) || CheckColisionWithGameMatrix(x, y)) {
					return true;
				}
			}
			return false;
		}

		bool CheckPieceColisionWithFloor(int piece_y) {
    		return piece_y + 1 >= heightCellCount;
		}

		bool CheckColisionWithGameMatrix(int piece_x, int piece_y) {
			return game_matrix[piece_x][piece_y + 1].on;
		}

		void DrawBackgrond() {
			ClearBackground(BLACK);

			// Draw vertical lines
			for (int x = 0 + offset; x <= widthCellCount * cellSize; x += cellSize) {
				DrawLine(x, 0 + offset, x, heightCellCount * cellSize, LIGHTGRAY);
			}

			// Draw horizontal lines
			for (int y = 0 + offset; y <=  heightCellCount * cellSize; y += cellSize) {
				DrawLine(0 + offset, y, widthCellCount * cellSize, y, LIGHTGRAY);
			}
		}
		
		void InitializeGameMatrix() {
			for(unsigned int i = 0; i < widthCellCount; i++) {
				for(unsigned int j = 0; j < heightCellCount; j++) {
					game_matrix[i][j].on = false;
					game_matrix[i][j].color = BLACK;
				}
			}
		}

		void DrawMatrix() {
			for(unsigned int i = 0; i < widthCellCount; i++) {
				for(unsigned int j = 0; j < heightCellCount; j++) {
					if (game_matrix[i][j].on) {
						float x = i;	
						float y = j;	

						DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, game_matrix[i][j].color);
					}	
				}
			}	
		}

		void MovePieceToTheLeft() {
			if(!CheckPieceColisionX()) {
				piece.MoveToTheLeft();
			}
		}

		void MovePieceToTheRight() {
			if(!CheckPieceColisionX()) {
				piece.MoveToTheRight();
			}
		}
};

int main() {
	
	std::cout << "Starting TETRIS game..." << "\n";

	InitWindow(screenWidth, screenHeight, "Tetris");
	SetTargetFPS(20);

	Game game = Game();

	while(!WindowShouldClose()) {
		BeginDrawing();
		
		if (eventTrigerred(0.8)) {
			game.Update();
		}	

		if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT)) {
			game.MovePieceToTheRight();
		}

		if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT)) {
			game.MovePieceToTheLeft();
		}

		if (IsKeyPressed(KEY_UP)) {
			game.piece.rotate(false);
		}

		if (IsKeyPressed(KEY_DOWN)) {
			game.piece.rotate(true);
		}

		if (IsKeyPressed(KEY_SPACE) || IsKeyDown(KEY_SPACE)) {
			game.Update();
		}

		game.Draw();
		game.DrawBackgrond();

		DrawText("Tetris Game", offset, 20, 40, WHITE);

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

void rotacionate(std::vector<Vector2>& body, bool clockwise) {
	Vector2 pivot = body[2];
	for(unsigned int i = 0; i < body.size(); i++) {
		float x = body[i].x - pivot.x;
		float y = body[i].y - pivot.y;

		float rotatedX = clockwise ? y : -y;
		float rotatedY = clockwise ? -x : x;

		body[i].x = pivot.x + rotatedX;
		body[i].y = pivot.y + rotatedY;
	}
}
