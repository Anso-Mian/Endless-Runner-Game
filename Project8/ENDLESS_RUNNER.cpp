//OUR GAME THE ENDLESS RUNNER FILLED WITH THRILL AND SUSPENSE
//DEVELOPERS: F24-0779 AND F24-0599


#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <mmsystem.h>
#include <sstream> // For string buffering

using namespace std;

#pragma comment(lib, "winmm.lib")

// Game constants
const int MAP_WIDTH = 40; // Width of game map
const int MAP_HEIGHT = 20; // Height of game map
const float INITIAL_SPEED = 1.0f; // Starting speed multiplier
const float MAX_SPEED = 2.5f; // Max speed cap
const int COIN_VALUE = 10; // Points per coin
const float SPEED_INCREMENT = 0.1f; // Reduced speed increase per milestone
const int FIXED_FRAME_DELAY = 16; // Frame delay for ~60 FPS (ms)
const int MILESTONE_1 = 200; // Distance milestones
const int MILESTONE_2 = 400;
const int MILESTONE_3 = 600;
const int MILESTONE_4 = 800;
const int MILESTONE_5 = 1000;
const int MILESTONE_6 = 1200;
const int MILESTONE_7 = 1400;
const int MILESTONE_8 = 1600;
const int MILESTONE_9 = 1800;
const int MILESTONE_10 = 2000;
const int MILESTONE_11 = 2500;
const int MILESTONE_12 = 3000;
const int MILESTONE_13 = 3500;
const int MILESTONE_14 = 4000;
const int MILESTONE_15 = 4500;
const int MILESTONE_16 = 5000;
const int MILESTONE_17 = 6000;
const int MILESTONE_18 = 7000;
const int MILESTONE_19 = 8000;
const int MILESTONE_20 = 9000;
const int MILESTONE_21 = 10000;
const int MAX_OBSTACLES = 100; // Max obstacles on screen
const int MAX_COINS = 100; // Max coins on screen
const int GRACE_PERIOD = 3; // Frames of invulnerability after hit
const int MESSAGE_START_X = MAP_WIDTH + 2; // Message display x-position
const int MESSAGE_START_Y = 2; // Message display y-position
const int MESSAGE_WIDTH = 30; // Message area width
const int CONSOLE_WIDTH = 80; // Console width for centering text
const int COMP_INITIAL_MOVE_DELAY = 10; // Computer player move delay
const int COMP_MAX_SPEED = 3; // Max computer player speed

							  // Player variables
int player_x; // Player x-position
int player_y; // Player y-position
int player_lives; // Player lives
int player_score; // Player score
int player_distance; // Distance traveled
string player_name; // Player name
int comp_x; // Computer player x-position
int comp_y; // Computer player y-position
bool comp_active; // Computer player active flag
int comp_move_counter; // Counter for computer movement
int comp_speed; // Computer speed

				// Message arrays
string message_texts[5]; // Message texts
int message_colors[5]; // Message colors
int message_display_times[5]; // Message display durations
int message_start_times[5]; // Message start times
bool message_actives[5]; // Message active flags

						 // Obstacle arrays
int obstacle_xs[MAX_OBSTACLES]; // Obstacle x-positions
int obstacle_ys[MAX_OBSTACLES]; // Obstacle y-positions
bool obstacle_actives[MAX_OBSTACLES]; // Obstacle active flags
int obstacle_count; // Number of active obstacles

					// Coin arrays
int coin_xs[MAX_COINS]; // Coin x-positions
int coin_ys[MAX_COINS]; // Coin y-positions
bool coin_collecteds[MAX_COINS]; // Coin collected flags
bool coin_actives[MAX_COINS]; // Coin active flags
int coin_count; // Number of active coins

				// Game state variables
int coin_streak = 0; // Coin collection streak
int last_coin_time = 0; // Time of last coin collection
int last_milestone = 0; // Last milestone reached
bool is_jumping = false; // Jumping state
float jump_velocity = 0.0f; // Jump velocity
float gravity = 0.5f; // Gravity for jump
float jump_power = 1.5f; // Jump strength
float player_y_float; // Float y-position for smooth jumping
int grace_frames = 0; // Invulnerability frames
float game_speed = INITIAL_SPEED; // Current speed multiplier

								  // Set console text color
void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Print centered text with color
void print_centered(string text, int color = 7) {
	int padding = (CONSOLE_WIDTH - text.length()) / 2;
	set_color(color);
	cout << setw(padding + text.length()) << text << endl;
}

// Move cursor to (x, y)
void goto_xy(int x, int y) {
	COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	//x for column and y for row
}

// Hide console cursor
void hide_cursor() {
	CONSOLE_CURSOR_INFO cursor = { 100, false };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Set console window and buffer size
void set_console_size() {
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT window_size = { 0, 0, MAP_WIDTH + MESSAGE_WIDTH + 5, MAP_HEIGHT + 5 };
	SetConsoleWindowInfo(h_console, TRUE, &window_size);
	COORD buffer_size = { MAP_WIDTH + MESSAGE_WIDTH + 5, MAP_HEIGHT + 5 };
	SetConsoleScreenBufferSize(h_console, buffer_size);
}

// Show welcome screen
void show_welcome_screen() {
	set_console_size();
	system("cls");
	set_color(11);
	cout << setfill('=') << setw(MAP_WIDTH) << "=" << endl;
	setfill(' ');

	// Display title
	print_centered(" ______ _   _ ____  _      ______  _____ _____  ", 10);
	print_centered("|  ____| \\ | |  _ \\| |    |  ____|/ ____/ ____| ", 10);
	print_centered("| |__  |  \\| | | | | |    | |__  | (___| (___   ", 11);
	print_centered("|  __| | . ` | | | | |    |  __|  \\___ \\\\___ \\  ", 12);
	print_centered("| |____| |\\  | |_| | |____| |____ ____) |___) | ", 14);
	print_centered("|______|_| \\_|____/|______|______|_____/_____/  ", 12);

	print_centered(" _____  _    _ _   _ _   _ ______ _____  ", 10);
	print_centered("|  __ \\| |  | | \\ | | \\ | |  ____|  __ \\ ", 10);
	print_centered("| |__) | |  | |  \\| |  \\| | |__  | |__) |", 11);
	print_centered("|  _  /| |  | | . ` | . ` |  __| |  _  / ", 12);
	print_centered("| | \\ \\| |__| | |\\  | |\\  | |____| | \\ \\ ", 14);
	print_centered("|_|  \\_\\\\____/|_| \\_|_| \\_|______|_|  \\_\\", 12);

	cout << endl;
	print_centered("WELCOME TO THE ENDLESS RUNNER", 14);
	print_centered("Enter your Name and Press ENTER key to start...", 9);
	set_color(11);
	cout << setfill('=') << setw(MAP_WIDTH) << "=" << endl;
	set_color(7);
}

// Add message to queue
void add_message(string text, int color, int display_time = 5000) {
	int slot = -1;
	// Find empty slot
	for (int i = 0; i < 5; i++) {
		if (!message_actives[i]) {
			slot = i;
			break;
		}
	}

	// If no empty slot, replace oldest
	if (slot == -1) {
		int oldest_time = GetTickCount();
		int oldest_index = 0;
		for (int i = 0; i < 5; i++) {
			if (message_start_times[i] < oldest_time) {
				oldest_time = message_start_times[i];
				oldest_index = i;
			}
		}
		slot = oldest_index;
	}

	// Set message properties
	message_texts[slot] = text;
	message_colors[slot] = color;
	message_display_times[slot] = display_time;
	message_start_times[slot] = GetTickCount();
	message_actives[slot] = true;
}

// Update message timers
void update_messages() {
	int current_time = GetTickCount();
	for (int i = 0; i < 5; i++) {
		if (message_actives[i] && current_time - message_start_times[i] > message_display_times[i]) {
			message_actives[i] = false;
		}
	}
}

// Display active messages
void display_messages() {
	// Clear message area
	for (int y = 0; y < MAP_HEIGHT; y++) {
		goto_xy(MESSAGE_START_X, y);
		cout << string(MESSAGE_WIDTH, ' ');
	}

	// Draw active messages
	int y_offset = 0;
	for (int i = 0; i < 5; i++) {
		if (message_actives[i]) {
			goto_xy(MESSAGE_START_X, MESSAGE_START_Y + y_offset);
			set_color(message_colors[i]);
			cout << message_texts[i];
			set_color(7);
			y_offset += 2;
		}
	}
}

// Initialize message arrays
void initialize_messages() {
	for (int i = 0; i < 5; i++) {
		message_texts[i] = "";
		message_colors[i] = 7;
		message_display_times[i] = 0;
		message_start_times[i] = 0;
		message_actives[i] = false;
		//resets all messages to defualt so that the system starts fresh
	}
}

// Message arrays for events
const string DISTANCE_MESSAGES[] = {
	"KEEP RUNNING!", "IMPRESSIVE DISTANCE!", "MARATHON RUNNER!",
	"GOING THE DISTANCE!", "UNSTOPPABLE JOURNEY!", "ULTRAMARATHON STATUS!",
	"TRANSCONTINENTAL RUNNER!", "BREAKING RECORDS!", "LEGENDARY DISTANCE!",
	"COSMIC DISTANCE ACHIEVED!"
};
const string SPEED_INCREASE_MESSAGES[] = {
	"SPEED BOOST!", "GETTING FASTER!", "TURBO MODE!", "BREAKING LIMITS!",
	"HYPERSPEED ENGAGED!", "SUPERSONIC SPEED!", "WARP SPEED ACTIVATED!",
	"TIME-SPACE DISTORTION!", "LIGHT SPEED ACHIEVED!", "BEYOND PHYSICS!"
};
const string LIVES_MESSAGES[] = {
	"LAST CHANCE!", "BE CAREFUL!", "STAY ALIVE!"
};
const string SCORE_MESSAGES[] = {
	"NICE GOING!", "AWESOME MOVES!", "YOU'RE ON FIRE!",
	"INCREDIBLE SKILLS!", "UNSTOPPABLE!", "LEGENDARY PLAYER!"
};
const string COLLISION_MESSAGES[] = {
	"OUCH! THAT HURT!", "WATCH OUT!", "BOOM! COLLISION!",
	"OH NO! YOU GOT HIT!", "CRASH! BE CAREFUL!"
};

// Show collision message
void handle_collision_message() {
	add_message(COLLISION_MESSAGES[rand() % 5], 12, 5000);
}

// Show computer player activation message
void handle_computer_player_message() {
	add_message("!!! DANGER !!!", 12, 8000);
	add_message("Computer Player Activated!", 14, 8000);
}

// Check distance milestones
void check_distance_milestones() {
	int milestones[] = {
		MILESTONE_1, MILESTONE_3, MILESTONE_5, MILESTONE_7, MILESTONE_9,
		MILESTONE_11, MILESTONE_13, MILESTONE_16, MILESTONE_19, MILESTONE_21
	};
	int message_indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int i = 0; i < 10; i++) {
		if (player_distance == milestones[i]) {
			add_message(DISTANCE_MESSAGES[message_indices[i]], 11 + (i % 5), 5000);
			break;
		}
	}
}

// Show speed increase message
void display_speed_increase_message(int milestone_index) {
	int message_index = milestone_index % 10;
	int color_index = 9 + (milestone_index % 7);
	add_message(SPEED_INCREASE_MESSAGES[message_index], color_index, 4000);
}

// Warn when lives are low
void check_lives_warning() {
	if (player_lives == 1) {
		static int flash_counter = 0;
		flash_counter = (flash_counter + 1) % 10;
		if (flash_counter == 0) {
			add_message(LIVES_MESSAGES[rand() % 3], 12, 3000);
		}
	}
}

// Show coin streak message
void display_coin_streak_message(int coins_collected) {
	if (coins_collected >= 3) {
		add_message("COIN STREAK: " + to_string(coins_collected) + "x!", 14, 3000);
	}
}

// Check score milestones
void check_score_milestones(int previous_score, int current_score) {
	if (previous_score < 50 && current_score >= 50) {
		add_message(SCORE_MESSAGES[0], 10, 5000);
	}
	else if (previous_score < 100 && current_score >= 100) {
		add_message(SCORE_MESSAGES[1], 11, 5000);
	}
	else if (previous_score < 200 && current_score >= 200) {
		add_message(SCORE_MESSAGES[2], 14, 5000);
	}
	else if (previous_score < 300 && current_score >= 300) {
		add_message(SCORE_MESSAGES[3], 13, 5000);
	}
	else if (previous_score < 500 && current_score >= 500) {
		add_message(SCORE_MESSAGES[4], 12, 5000);
	}
	else if (previous_score < 1000 && current_score >= 1000) {
		add_message(SCORE_MESSAGES[5], 15, 5000);
	}
}

// Initialize game state
void initialize_game() {
	// Set player starting position
	player_x = MAP_WIDTH / 2;
	player_y = MAP_HEIGHT - 2;
	player_y_float = static_cast<float>(player_y); //float conversion to make smooth jumping 
	player_lives = 3;
	player_score = 0;
	player_distance = 0;
	is_jumping = false;
	jump_velocity = 0.0f;
	grace_frames = 0;

	// Set computer player
	comp_x = MAP_WIDTH / 2;
	comp_y = MAP_HEIGHT - 10;
	comp_active = false;
	comp_move_counter = 0;
	comp_speed = COMP_INITIAL_MOVE_DELAY;  //Delay computer player for easy level

	// Reset game state
	coin_streak = 0;
	last_coin_time = 0;
	last_milestone = 0;
	game_speed = INITIAL_SPEED;

	// Initialize obstacles
	obstacle_count = 0;
	for (int i = 0; i < 12 && obstacle_count < MAX_OBSTACLES; i++) {
		obstacle_xs[obstacle_count] = 1 + rand() % (MAP_WIDTH - 2);
		obstacle_ys[obstacle_count] = rand() % (MAP_HEIGHT - 10);
		obstacle_actives[obstacle_count] = true;
		obstacle_count++;
	}

	// Initialize coins
	coin_count = 0;
	for (int i = 0; i < 20 && coin_count < MAX_COINS; i++) {
		coin_xs[coin_count] = 1 + rand() % (MAP_WIDTH - 2);
		coin_ys[coin_count] = rand() % MAP_HEIGHT;
		coin_collecteds[coin_count] = false;
		coin_actives[coin_count] = true;
		coin_count++;
	}
}

// Update game map
void update_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
	// Draw borders and clear map
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (x == 0 || x == MAP_WIDTH - 1) map[y][x] = '|';
			else if (y == 0 || y == MAP_HEIGHT - 1) map[y][x] = '-';
			else map[y][x] = ' ';
		}
	}
}

// Draw game map with buffering
void draw_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
	char temp_map[MAP_HEIGHT][MAP_WIDTH];
	memcpy(temp_map, map, sizeof(temp_map));

	// Place player and computer
	temp_map[player_y][player_x] = 'P';
	if (comp_active) temp_map[comp_y][comp_x] = 'C';

	// Place obstacles
	for (int i = 0; i < obstacle_count; i++) {
		if (obstacle_actives[i] && obstacle_ys[i] >= 0 && obstacle_ys[i] < MAP_HEIGHT &&
			obstacle_xs[i] >= 0 && obstacle_xs[i] < MAP_WIDTH) {
			temp_map[obstacle_ys[i]][obstacle_xs[i]] = 'X';
		}
	}

	// Place coins
	for (int i = 0; i < coin_count; i++) {
		if (coin_actives[i] && !coin_collecteds[i] && coin_ys[i] >= 0 && coin_ys[i] < MAP_HEIGHT &&
			coin_xs[i] >= 0 && coin_xs[i] < MAP_WIDTH) {
			temp_map[coin_ys[i]][coin_xs[i]] = 'O';
		}
	}

	// Buffer output to reduce cout calls
	stringstream buffer;
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			buffer << temp_map[y][x];
		}
		buffer << '\n';
	}

	// Draw buffered map
	goto_xy(0, 0);
	set_color(7); // Reset color
	cout << buffer.str();

	// Apply colors (minimized calls)
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			char ch = temp_map[y][x];
			if (ch == 'P' || ch == 'X' || ch == 'O' || ch == 'C' || ch == '|' || ch == '-') {
				goto_xy(x, y);
				switch (ch) {
				case 'P': set_color(10); break;
				case 'X': set_color(12); break;
				case 'O': set_color(14); break;
				case 'C': set_color(9); break;
				case '|': set_color(12); break;
				case '-': set_color(11); break;
				}
				cout << ch;
			}
		}
	}
	set_color(7);
}

// Move player based on input
void move_player(char key, char map[MAP_HEIGHT][MAP_WIDTH]) {
	if (key == 'a' || key == 'A') {
		if (player_x - 1 > 1) player_x -= 1;
		else player_x = 1; // Keep within left bound
	}
	else if (key == 'd' || key == 'D') {
		if (player_x + 1 < MAP_WIDTH - 2) player_x += 1;
		else player_x = MAP_WIDTH - 2; // Keep within right bound
	}

	if (!is_jumping) {
		if (key == 'w' || key == 'W') {
			if (player_y - 1 > 1) player_y -= 1;
			else player_y = 1; // Keep within top bound
			player_y_float = static_cast<float>(player_y);
		}
		else if (key == 's' || key == 'S') {
			if (player_y + 1 < MAP_HEIGHT - 2) player_y += 1;
			else player_y = MAP_HEIGHT - 2; // Keep within bottom bound
			player_y_float = static_cast<float>(player_y);
		}
		else if (key == ' ') {
			is_jumping = true;
			jump_velocity = -jump_power;
		}
	}
}

// Process player jump
void process_jump() {
	if (is_jumping) {
		jump_velocity += gravity;
		player_y_float += jump_velocity;
		int new_y = static_cast<int>(player_y_float);

		if (jump_velocity < 0) {
			player_y = new_y;
			if (new_y <= 1) {
				player_y = 1;
				player_y_float = 1.0f;
				jump_velocity = 0;
				is_jumping = false;
			}
		}
		else {
			player_y = new_y;
			player_y_float = static_cast<float>(new_y);
			is_jumping = false;
			jump_velocity = 0;
		}
	}
}

// Move computer player
void move_computer_player(float game_speed) {
	int speed_boost = 1 + static_cast<int>(game_speed - 1.0f);
	if (speed_boost > COMP_MAX_SPEED) speed_boost = COMP_MAX_SPEED; // Cap computer speed

																	// Chase player with some randomness
	if (rand() % 100 < 65) {
		if (comp_x < player_x) comp_x += speed_boost;
		else if (comp_x > player_x) comp_x -= speed_boost;
		if (comp_y < player_y) comp_y += speed_boost;
		else if (comp_y > player_y) comp_y -= speed_boost;
	}
	else {
		comp_x += (rand() % 3) - 1;
		comp_y += (rand() % 3) - 1;
	}

	// Keep within bounds
	if (comp_x < 1) comp_x = 1;
	else if (comp_x > MAP_WIDTH - 2) comp_x = MAP_WIDTH - 2;
	if (comp_y < 1) comp_y = 1;
	else if (comp_y > MAP_HEIGHT - 2) comp_y = MAP_HEIGHT - 2;
}

// Check for collisions
bool check_collision() {
	if (!is_jumping && grace_frames == 0) {
		for (int i = 0; i < obstacle_count; i++) {
			if (obstacle_actives[i] && player_x == obstacle_xs[i] && player_y == obstacle_ys[i]) {
				return true;
			}
		}
	}
	return comp_active && player_x == comp_x && player_y == comp_y;
}

// Collect coins
void collect_coins() {
	int current_time = GetTickCount();
	for (int i = 0; i < coin_count; i++) {
		if (coin_actives[i] && !coin_collecteds[i] && player_x == coin_xs[i] && player_y == coin_ys[i]) {
			coin_collecteds[i] = true;
			player_score += COIN_VALUE;

			// Update streak
			if (current_time - last_coin_time < 2000) {
				coin_streak++;
			}
			else {
				coin_streak = 1;
			}
			last_coin_time = current_time;
			if (coin_streak >= 3) display_coin_streak_message(coin_streak);
		}
	}
}

// Generate new obstacles
void generate_obstacles() {
	int num_new = 1 + rand() % 3;
	for (int i = 0; i < num_new && obstacle_count < MAX_OBSTACLES; i++) {
		obstacle_xs[obstacle_count] = 1 + rand() % (MAP_WIDTH - 2);
		obstacle_ys[obstacle_count] = 1;
		obstacle_actives[obstacle_count] = true;
		obstacle_count++;
	}
}

// Generate new coins
void generate_coins() {
	if (coin_count < 95) {
		int num_new = 2 + rand() % 4;
		for (int i = 0; i < num_new && coin_count < MAX_COINS; i++) {
			coin_xs[coin_count] = 1 + rand() % (MAP_WIDTH - 2);
			coin_ys[coin_count] = 1;
			coin_collecteds[coin_count] = false;
			coin_actives[coin_count] = true;
			coin_count++;
			// Debug: Uncomment to confirm coin generation
			// add_message("Generated " + to_string(num_new) + " coins", 14, 1000);
		}
	}
}

// Move obstacles and coins
void move_game_elements() {
	static float accumulated_speed = 0.0f;
	accumulated_speed += game_speed;

	// Move elements when threshold reached
	if (accumulated_speed >= 4.0f) { // Increased to slow movement
		for (int i = 0; i < obstacle_count; i++) {
			if (obstacle_actives[i]) {
				obstacle_ys[i]++;
				if (obstacle_ys[i] >= MAP_HEIGHT) obstacle_actives[i] = false;
			}
		}
		for (int i = 0; i < coin_count; i++) {
			if (coin_actives[i] && !coin_collecteds[i]) {
				coin_ys[i]++;
				if (coin_ys[i] >= MAP_HEIGHT) coin_actives[i] = false;
			}
		}
		accumulated_speed -= 4.0f;
	}
}

// Generate obstacles and coins
void generate_game_elements() {
	static float obstacle_accumulator = 0.0f;
	static float coin_accumulator = 0.0f;
	obstacle_accumulator += game_speed;
	coin_accumulator += game_speed;

	// Generate obstacles
	if (obstacle_accumulator >= 12.0f) { // Increased to reduce spawn frequency
		generate_obstacles();
		obstacle_accumulator -= 12.0f;
	}

	// Generate coins
	if (coin_accumulator >= 10.0f) { // Increased to reduce spawn frequency
		generate_coins();
		coin_accumulator -= 10.0f;
	}
}

// Clean up inactive obstacles and coins
void cleanup_game_elements() {
	// Clean obstacles
	int new_obstacle_count = 0;
	for (int i = 0; i < obstacle_count; i++) {
		if (obstacle_actives[i]) {
			obstacle_xs[new_obstacle_count] = obstacle_xs[i];
			obstacle_ys[new_obstacle_count] = obstacle_ys[i];
			obstacle_actives[new_obstacle_count] = true;
			new_obstacle_count++;
		}
	}
	obstacle_count = new_obstacle_count;

	// Clean coins
	int new_coin_count = 0;
	for (int i = 0; i < coin_count; i++) {
		if (coin_actives[i] && !coin_collecteds[i]) {
			coin_xs[new_coin_count] = coin_xs[i];
			coin_ys[new_coin_count] = coin_ys[i];
			coin_collecteds[new_coin_count] = false;
			coin_actives[new_coin_count] = true;
			new_coin_count++;
		}
	}
	coin_count = new_coin_count;
}

// Adjust game speed at milestones
void adjust_game_speed() {
	int milestones[] = {
		MILESTONE_1, MILESTONE_2, MILESTONE_3, MILESTONE_4, MILESTONE_5,
		MILESTONE_6, MILESTONE_7, MILESTONE_8, MILESTONE_9, MILESTONE_10,
		MILESTONE_11, MILESTONE_12, MILESTONE_13, MILESTONE_14, MILESTONE_15,
		MILESTONE_16, MILESTONE_17, MILESTONE_18, MILESTONE_19, MILESTONE_20,
		MILESTONE_21
	};
	for (int i = 0; i < 21; i++) {
		if (player_distance >= milestones[i] && last_milestone < milestones[i]) {
			game_speed += SPEED_INCREMENT;
			if (game_speed > MAX_SPEED) game_speed = MAX_SPEED; // Cap speed
			last_milestone = milestones[i];
			display_speed_increase_message(i);
			break;
		}
	}
}

// Save high score
void save_high_score() {
	const int MAX_HIGH_SCORES = 5;
	string names[MAX_HIGH_SCORES];
	int scores[MAX_HIGH_SCORES];
	int count = 0;

	// Read existing scores
	ifstream score_file("highscores.txt");
	string name;
	int value;
	while (score_file >> name >> value && count < MAX_HIGH_SCORES) {
		names[count] = name;
		scores[count] = value;
		count++;
	}
	score_file.close();

	// Add new score
	if (count < MAX_HIGH_SCORES) {
		names[count] = player_name;
		scores[count] = player_score;
		count++;
	}
	else {
		for (int i = 0; i < MAX_HIGH_SCORES; i++) {
			if (player_score > scores[i]) {
				for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
					names[j] = names[j - 1];
					scores[j] = scores[j - 1];
				}
				names[i] = player_name;
				scores[i] = player_score;
				break;
			}
		}
	}

	// Sort scores
	for (int i = 0; i < count; i++) {
		for (int j = i + 1; j < count; j++) {
			if (scores[j] > scores[i]) {
				swap(scores[i], scores[j]);
				swap(names[i], names[j]);
			}
		}
	}

	// Save scores
	ofstream out_file("highscores.txt");
	for (int i = 0; i < count; i++) {
		out_file << names[i] << " " << scores[i] << endl;
	}
	out_file.close();
}

// Display high scores
void display_high_scores() {
	cout << "\nHigh Scores:\n------------\n";
	ifstream score_file("highscores.txt");
	if (!score_file) {
		cout << "No high scores yet!\n";
		return;
	}

	string name;
	int score;
	int rank = 1;
	while (score_file >> name >> score && rank <= 5) {
		cout << rank << ". " << name << ": " << score << endl;
		rank++;
	}
	score_file.close();
}

int main() {
	// Initialize audio
	char mci_buffer[256];
	string music_relative_path = ".\\MUSIC\\music1.mp3";
	bool audio_playing = false;

	srand(static_cast<unsigned int>(time(0)));
	hide_cursor();

	char map[MAP_HEIGHT][MAP_WIDTH];
	bool game_over = false;
	char key;
	int frame_counter = 0;

	// Show welcome and get name
	show_welcome_screen();
	cout << "Enter your name: ";
	getline(cin, player_name);
	Beep(800, 800);

	// Initialize game
	initialize_game();
	initialize_messages();
	system("cls");

	// Try playing audio
	string mp3_command = "open \"" + music_relative_path + "\" type mpegvideo alias gamemusic";
	if (mciSendString(mp3_command.c_str(), mci_buffer, sizeof(mci_buffer), NULL) == 0) {
		if (mciSendString("play gamemusic repeat", mci_buffer, sizeof(mci_buffer), NULL) == 0) {
			audio_playing = true;
			cout << "Playing MP3 music..." << endl;
		}
	}

	if (!audio_playing) {
		string wav_relative_path = ".\\MUSIC\\music.wav";
		string wav_command = "open \"" + wav_relative_path + "\" type mpegvideo alias gamemusic";
		if (mciSendString(wav_command.c_str(), mci_buffer, sizeof(mci_buffer), NULL) == 0) {
			if (mciSendString("play gamemusic repeat", mci_buffer, sizeof(mci_buffer), NULL) == 0) {
				audio_playing = true;
				cout << "Playing WAV music..." << endl;
			}
		}
	}

	if (!audio_playing) {
		char exe_path[MAX_PATH];
		GetModuleFileName(NULL, exe_path, MAX_PATH);
		string exe_path_str(exe_path);
		size_t last_slash_pos = exe_path_str.find_last_of('\\');
		string exe_dir = last_slash_pos != string::npos ? exe_path_str.substr(0, last_slash_pos + 1) : "";
		string full_path = exe_dir + "MUSIC\\music1.mp3";
		string cmd = "open \"" + full_path + "\" type mpegvideo alias gamemusic";

		if (mciSendString(cmd.c_str(), mci_buffer, sizeof(mci_buffer), NULL) == 0) {
			if (mciSendString("play gamemusic repeat", mci_buffer, sizeof(mci_buffer), NULL) == 0) {
				audio_playing = true;
				cout << "Playing music from executable path..." << endl;
			}
		}
	}

	if (!audio_playing) {
		DWORD error = GetLastError();
		mciGetErrorString(error, mci_buffer, sizeof(mci_buffer));
		cerr << "Failed to play audio. Error: " << error << " - " << mci_buffer << endl;
	}

	// Main game loop
	int previous_score = player_score;
	while (!game_over) {
		frame_counter++;

		// Update game state
		update_map(map);
		if (is_jumping) process_jump();
		if (grace_frames > 0) grace_frames--;
		draw_map(map);
		update_messages();
		display_messages();
		check_distance_milestones();
		check_lives_warning();

		// Display stats
		goto_xy(0, MAP_HEIGHT + 1);
		cout << "Player: " << player_name << "  Lives: " << player_lives
			<< "  Score: " << player_score << "  Distance: " << player_distance
			<< "  Speed: " << fixed << setprecision(1) << game_speed << "x" << endl;

		// Handle input
		if (_kbhit()) {
			key = _getch();
			move_player(key, map);
			if (key == 'q' || key == 'Q') game_over = true;
		}

		// Move computer player
		if (comp_active) {
			comp_move_counter++;
			int move_delay = COMP_INITIAL_MOVE_DELAY - static_cast<int>(game_speed - 1.0f);
			if (move_delay < 3) move_delay = 3; // Ensure minimum delay
			if (comp_move_counter >= move_delay) {
				move_computer_player(game_speed);
				comp_move_counter = 0;
			}
		}

		// Check collisions
		if (check_collision()) {
			player_lives--;
			handle_collision_message();
			player_x = MAP_WIDTH / 2;
			player_y = MAP_HEIGHT - 2;
			player_y_float = static_cast<float>(player_y);
			is_jumping = false;
			jump_velocity = 0.0f;
			grace_frames = GRACE_PERIOD;
			if (player_lives <= 0) game_over = true;
		}

		// Update game
		collect_coins();
		check_score_milestones(previous_score, player_score);
		previous_score = player_score;
		player_distance += 5;

		// Activate computer player
		if (player_distance >= MILESTONE_10 && !comp_active) {
			comp_active = true;
			handle_computer_player_message();
		}

		// Update speed and elements
		adjust_game_speed();
		move_game_elements();
		generate_game_elements();

		// Clean up every 10 frames to reduce lag
		if (frame_counter % 10 == 0) cleanup_game_elements();

		Sleep(FIXED_FRAME_DELAY);
	}

	// Show game over screen
	system("cls");
	cout << "Game Over!\nPlayer: " << player_name
		<< "\nScore: " << player_score
		<< "\nDistance: " << player_distance << "m" << endl;

	// Save and show high scores
	save_high_score();
	display_high_scores();

	// Clean up audio
	if (audio_playing) mciSendString(TEXT("close gamemusic"), NULL, 0, NULL);
	system("pause");
	return 0;
}