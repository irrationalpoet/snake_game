#include <stdio.h>          // Includes the standard input/output library
#include <stdlib.h>         // Includes the standard library
#include <stdbool.h>        // Includes the boolean type library
#include <SDL.h>            // Includes the SDL library
#include <SDL2/SDL_ttf.h> 	// Includes the SDL TTF library for font rendering
#include <SDL_filesystem.h> // Includes the SDL filesystem library
#include <time.h>           // Includes the time library

typedef struct {		// Defines a structure named Point
	int x, y;			// Members x and y, both integers
} Point;

SDL_Window* window = NULL;     	// Declares a pointer to an SDL window, initialized to NULL
SDL_Renderer* renderer = NULL; 	// Declares a pointer to an SDL renderer, initialized to NULL
TTF_Font* font = NULL;         	// Declares a pointer to a TTF font, initialized to NULL

const int FONT_SIZE = 96;           // Defines a constant integer for the font size
const int BLOCK_SIZE = 20;          // Defines a constant integer for the block size
const int MAX_SNAKE_LENGTH = 512;   // Defines a constant integer for the maximum snake length
const int SNAKE_SPEED = 35;         // Defines a constant integer for the snake speed (in milliseconds)

int windowWidth;	// Declares an integer for the window width
int windowHeight;	// Declares an integer for the window height

Point snake[MAX_SNAKE_LENGTH];	// Declares an array of Point structures for the snake
int snakeLength;             	// Declares an integer for the current length of the snake
Point food;                  	// Declares a Point structure for the food position
int direction;               	// Declares an integer for the snake's direction
int score;                   	// Declares an integer for the player's score
bool gameOver;               	// Declares a boolean to indicate if the game is over

void createWindow();                                 		// Declares a function to create the game window
void drawText(const char* text, int x, int y);       		// Declares a function to draw text on the screen
void startGameScreen();                              		// Declares a function to display the start game screen
void startGame();                                    		// Declares a function to handle the start of the game
void handleInput();                                  		// Declares a function to handle user input
void moveSnake();                                    		// Declares a function to move the snake
void generateFood();                                 		// Declares a function to generate the food position
bool checkCollision(int windowWidth, int windowHeight); 	// Declares a function to check for collisions
void gameOverScreen();                               		// Declares a function to display the game over screen
void playAgain();                                    		// Declares a function to handle the play again logic
void initializeGame();                               		// Declares a function to initialize the game state
void draw();                                         		// Declares a function to draw game elements
void updateScore();                                  		// Declares a function to update and display the score

/* Main */
int main() {
	createWindow();												// Creates window
	startGameScreen();											// Displays start screen for player
	startGame();												// Waits for key press to start the game
	initializeGame();											// Initializes/resets new game values

	while (true) {												// Main game loop
		handleInput();   										// Handles user input
		moveSnake();     										// Moves the snake

		if (checkCollision(windowWidth, windowHeight)) { 		// Checks if a collision occurred
			gameOverScreen(); 									// Displays the game over screen
			playAgain();      									// Handles the play again logic
			initializeGame(); 									// Initializes the game for a new round
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);			// Sets the render draw color to black
		SDL_RenderClear(renderer);                     			// Clears the renderer

		draw();													// Draws the game elements
		updateScore();											// Updates and displays the score

		SDL_RenderPresent(renderer);							// Updates the screen with the rendered content
		SDL_Delay(SNAKE_SPEED);									// Pauses the execution for the specified snake speed
	}
}

/* Creates game window */
void createWindow() {							
	SDL_Init(SDL_INIT_VIDEO);														// Initializes the SDL video
	SDL_DisplayMode displayMode;													// Declares a variable to store display mode information
	SDL_GetDesktopDisplayMode(0, &displayMode);										// Gets the display mode of the primary display
	windowWidth = displayMode.w;                									// Sets the window width to the display width
	windowHeight = displayMode.h;													// Sets the window height to the display height

	window = SDL_CreateWindow("Snake Game",                      					// Creates the game window
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 						// Sets the initial window position to undefined
			windowWidth, windowHeight,                       						// Sets the window dimensions
			SDL_WINDOW_FULLSCREEN_DESKTOP);                 						// Sets the window to fullscreen desktop mode
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);			// Creates the renderer for the window, using hardware acceleration if available

	TTF_Init();                                                						// Initializes the SDL TTF
	char* basePath = SDL_GetBasePath();                        						// Gets the base path
	char fontPath[1024];                                       						// Declares a string to store the font path
	snprintf(fontPath, sizeof(fontPath), "%s/BabyPlums-rv2gL.ttf", basePath);		// Formats the font path
	SDL_free(basePath);                                        						// Frees the memory allocated for the base path
	font = TTF_OpenFont(fontPath, FONT_SIZE);                  						// Opens the specified font at the defined font size

	SDL_ShowCursor(SDL_FALSE);														// Hides the mouse cursor
	SDL_SetRelativeMouseMode(SDL_TRUE);												// Sets relative mouse mode, capturing mouse motion within the window
}

/* Draws text to screen */
void drawText(const char* text, int x, int y) {									
	SDL_Color textColor = { 255, 255, 255 };											// Sets the text color to white
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);				// Creates a surface with the rendered text
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);		// Creates a texture from the surface
	SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h }; 					// Defines the rectangle where the text will be rendered
	SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);    						// Copies the texture to the renderer
	SDL_FreeSurface(textSurface);                               						// Frees the memory allocated for the text surface
	SDL_DestroyTexture(textTexture);                           							// Destroys the text texture
}


/* Start game screen */
void startGameScreen() {                                               												// Sets the render draw color to black		
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);             													// Clears the renderer
	SDL_RenderClear(renderer);                                 														// Draws the start game text
	drawText("Press Enter to start game", windowWidth / 2 - windowWidth / 3, windowHeight / 2 - windowHeight / 3);	// Draws the pause text
	drawText("Press Space to pause", windowWidth / 2 - windowWidth / 4, windowHeight / 2);                    	 	// Draws the exit text
	drawText("Press ESC to exit", windowWidth / 2 - windowWidth / 5, windowHeight / 2 + windowHeight / 3);     		// Updates the screen with the rendered content
	SDL_RenderPresent(renderer);                               															
}

/* Handles start of game */
void startGame() {
	bool gameStarted = false; 															// Initializes a boolean to track if the game has started
	while (!gameStarted) {                                    							// Loop until the game starts
		SDL_Event e;                                       								// Declares an SDL event variable
		while (SDL_PollEvent(&e) != 0) {                   								// Polls for events
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {				// If a key is pressed and it's the Return key
				gameStarted = true;                        								// Set gameStarted to true
				break;                                     								// Exit the inner loop
			} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {		// If a key is pressed and it's the Escape key
				exit(0);                                   								// Exit the game
			}
		}
	}
}

/* Game over screen */
void gameOverScreen() {
	char final_score_txt[50];                                 														// Declares an array to store the final score text
	sprintf(final_score_txt, "Final Score: %d", score);       														// Formats the final score text
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);             													// Sets the render draw color to black
	SDL_RenderClear(renderer);                                 														// Clears the renderer
	drawText("Game Over!", windowWidth / 2 - windowWidth / 3, windowHeight / 2 - windowHeight / 3);					// Draws the game over text
	drawText(final_score_txt, windowWidth / 2 - windowWidth / 3, windowHeight / 2);									// Draws the final score
	drawText("Play again? (y / n)", windowWidth / 2 - windowWidth / 3, windowHeight / 2 + windowHeight / 3);		// Draws the play again prompt
	SDL_RenderPresent(renderer);                               														// Updates the screen with the rendered content
}

/* Handles game over input */
void playAgain() {
	while (true) {								// Infinite loop until a valid input is received
		SDL_Event e;                            // Declares an SDL event variable
		while (SDL_PollEvent(&e) != 0) {        // Polls for events
			if (e.type == SDL_KEYDOWN) {       	// If a key is pressed
				switch (e.key.keysym.sym) {		// Switch based on the pressed key
					case SDLK_y:                // If the 'y' key is pressed
						return;                 // Return from the function
					case SDLK_n:                // If the 'n' key is pressed
					case SDLK_ESCAPE:           // If the Escape key is pressed
						exit(0);              	// Exit the game
				}
			}
		}
		SDL_Delay(10);							// Small delay to prevent thread-lock
	}
}

/* Initialize food, snake position, length, score, direction, and gameOver condition */
void initializeGame() {
	srand(time(NULL)); 					// Seeds the random number generator
	generateFood();						// Generates initial food
	snake[0].x = windowWidth / 2;		// Initializes snake's x position at midpoint of window width
	snake[0].y = windowHeight / 2;		// Initializes snake's y position at midpoint of window height
	snakeLength = 3;					// Initializes snake's length to 3
	score = 0;							// Initializes score to 0
	direction = 0;						// Initalizes direction to the right (change this later to randomize direction)
}

/* Handles user input */
void handleInput() {
	SDL_Event e;                                        												// Declares an SDL event variable
	while (SDL_PollEvent(&e) != 0) {                    												// Polls for events
		if (e.type == SDL_KEYDOWN) {                    												// If a key is pressed
			switch (e.key.keysym.sym) {                													// Switch based on the pressed key
				case SDLK_LEFT:                         												// If the Left Arrow key is pressed
				case SDLK_a:                            												// Or the 'a' key is pressed
					if (direction != 0) direction = 2;													// If not moving right, set direction to left
					break;
				case SDLK_RIGHT:                        												// If the Right Arrow key is pressed
				case SDLK_d:                            												// Or the 'd' key is pressed
					if (direction != 2) direction = 0;  												// If not moving left, set direction to right
					break;
				case SDLK_UP:                           												// If the Up Arrow key is pressed
				case SDLK_w:                            												// Or the 'w' key is pressed
					if (direction != 1) direction = 3;  												// If not moving down, set direction to up
					break;
				case SDLK_DOWN:                         												// If the Down Arrow key is pressed
				case SDLK_s:                            												// Or the 's' key is pressed
					if (direction != 3) direction = 1; 													// If not moving up, set direction to down
					break;
				case SDLK_ESCAPE:                       												// If the Escape key is pressed
					exit(0);                           													// Exit game
				case SDLK_SPACE:                        												// If the Spacebar is pressed
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 										// Clear screen
				SDL_RenderClear(renderer);                     											// Clears the renderer
					drawText("Game Paused", windowWidth / 2 - windowWidth / 6, windowHeight / 2);		// Draws the pause message
					SDL_RenderPresent(renderer);                   										// Updates the screen

					// Wait for resume/exit key press
					bool paused = true;                       											// Initializes a boolean to track pause state
					while (paused) {                           											// Loop while paused
						while (SDL_PollEvent(&e) != 0) {      											// Polls for events
							if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) { 				// If Spacebar is pressed
								paused = false;              											// Resume game
							} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {		// If Escape is pressed
								exit(0);                     											// Exit game
							}
						}
					}
			}

		}
	}
}

/* Moves snake */
void moveSnake() {
	for (int i = snakeLength - 1; i > 0; i--) {				// Iterate through the snake segments
		snake[i] = snake[i - 1];               				// Move each segment to the position of the segment in front of it
	}

	switch (direction) {                     				// Switch based on the current direction
		case 0:                           					// Right
			snake[0].x += BLOCK_SIZE;      					// Move the head to the right
			break;
		case 1:                           					// Down
			snake[0].y += BLOCK_SIZE;      					// Move the head down
			break;
		case 2:                           					// Left
			snake[0].x -= BLOCK_SIZE;      					// Move the head to the left
			break;
		case 3:                           					// Up
			snake[0].y -= BLOCK_SIZE;      					// Move the head up
			break;
	}

	if (snake[0].x == food.x && snake[0].y == food.y) {		// If the snake's head position matches the food's position
		snakeLength++;                            			// Increase the snake's length
		score++;                                  			// Increase the score
		generateFood();                           			// Generate new food
	}
}

/* Generates food */
void generateFood() {
	int foodX, foodY;                                    						// Declares integers for the food's x and y coordinates
	int scoreAreaWidth, scoreAreaHeight;                						// Declares integers for the width and height of the score area
	bool collision;                                      						// Declares a boolean to track if the food collides with the snake

	TTF_SizeText(font, "Score: 000", &scoreAreaWidth, &scoreAreaHeight);		// Gets the size of the score text

	do {
		collision = false;                                 						// Reset collision flag
		foodX = (rand() % (windowWidth / BLOCK_SIZE)) * BLOCK_SIZE;   			// Generate a random x coordinate for the food
		foodY = (rand() % (windowHeight / BLOCK_SIZE)) * BLOCK_SIZE;			// Generate a random y coordinate for the food
		if (foodX < scoreAreaWidth && foodY < scoreAreaHeight) { 				// If the food spawns in the score area
		collision = true;                              							// Mark as collision
		}

		for (int i = 0; i < snakeLength; i++) {            						// Iterate through the snake segments
			if (foodX == snake[i].x && foodY == snake[i].y) { 					// If the food's position matches a snake segment's position
				collision = true;                          						// Mark as collision
				break;                                   						// Exit the inner loop
			}
		}
	} while (collision);                                   						// Continue generating food until no collision

	food.x = foodX;                                        						// Set the food's x coordinate
	food.y = foodY;                                        						// Set the food's y coordinate
}

/* Draws game elements */
void draw() {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);       						// Sets the render draw color to green
	for (int i = 0; i < snakeLength; i++) {               							// Iterate through the snake segments
		SDL_Rect rect = { snake[i].x, snake[i].y, BLOCK_SIZE, BLOCK_SIZE };			// Create a rectangle for the snake segment
		SDL_RenderFillRect(renderer, &rect);               							// Fill the rectangle with the current draw color
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);       						// Sets the render draw color to red
	SDL_Rect foodRect = { food.x, food.y, BLOCK_SIZE, BLOCK_SIZE }; 				// Create a rectangle for the food
	SDL_RenderFillRect(renderer, &foodRect);                     					// Fill the rectangle with the current draw color
}

/* Checks for collisions */
bool checkCollision(int windowWidth, int windowHeight) {
	if (snake[0].x < 0 || snake[0].x >= windowWidth || snake[0].y < 0 || snake[0].y >= windowHeight) {		// Checks if snake collides with wall
		return true;                                                             
	}

	for (int i = 1; i < snakeLength; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
			return true;                                                          							// Checks if snake collides with self
		}
	}
	return false;                                                                 							// Returns false if no collision detected
}

/* Updates game score */
void updateScore() {
	char scoreText[50];                                          	// Declares a string to store the score text
	snprintf(scoreText, sizeof(scoreText), "Score: %d", score);		// Formats the score tet
	drawText(scoreText, 10, 10);                                	// Draws the score on the screen
}
