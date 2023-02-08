#include "../stdafx.h"
#include "../console/console.hpp"
#include "../audio/audio.h"
#include "../game/game.hpp"

void clear_rect(short x, short y, int width, int height) {
	for (int h{}; h < height; ++h) {
		move_console_cursor(x, y + h);
		for (int w{}; w < width; ++w) {
			printf(" ");
		}
	}

	return;
}

void create_tab(short x, short y, int width, int height, const char* name) {
	for (int h{}; h < height; ++h) {
		move_console_cursor(x, y + h);
		if (h != 0) {
			printf("|");
		}
		
		for (int w{}; w < width - 2; ++w) {
			if (h != 0 && h != height - 1) {
				printf(" ");
			}
			else {
				if (w == 0 && h == 0) {
					move_console_cursor(x + 1, y);
				}

				printf("_");
			}
		}

		if (h != 0) {
			printf("|");
		}
	}

	move_console_cursor(x + 2, y);
	printf("%s", name);
	return;
}

void print_art(short x, short y, std::vector<const char*> art, const char* color = "\033[0m") {
	for (int i{}; i < art.size(); ++i) {
		move_console_cursor(x, y + i);
		printf("%s%s\033[0m", color, art[i]);
	}

	return;
}

int main(void) {
	constexpr auto default_menu_padding{ 4 }, selected_menu_padding{ 6 }, menu_spacing{ 32 }; // (Debug: 32, Release: 10)
	constexpr auto default_menu_color{ "\033[2;37m" }, selected_menu_color{ "\033[1;37m" };
	constexpr auto menu_hover_cursor{ "      \n      \n  --> \n      \n      " };
	constexpr auto menu_music{ "menu-music.wav" }, menu_select_sound{ "opt-select.wav" }, menu_hover_sound{ "opt-hover.wav" };

	const std::array<short, 2> screen_res{ get_console_res() };
	float music_volume{ 1.0f }, sound_volume{ 1.0f };
	short current_menu_opt{}, current_menu_tab{};

	Audio game_music;
	game_music.setLoop(true);
	game_music.setVolume(music_volume);

	Audio game_sound;
	game_sound.setVolume(sound_volume);

	const std::vector<const char*> text_phantom_chambers{
	R"(__________.__                   __                   _________ .__                  ___.                        )",
	R"(\______   \  |__ _____    _____/  |_  ____   _____   \_   ___ \|  |__ _____    _____\_ |__   ___________  ______)",
	R"( |     ___/  |  \\__  \  /    \   __\/  _ \ /     \  /    \  \/|  |  \\__  \  /     \| __ \_/ __ \_  __ \/  ___/)",
	R"( |    |   |   Y  \/ __ \|   |  \  | (  (_) )  Y Y  \ \     \___|   Y  \/ __ \|  Y Y  \ \_\ \  ___/|  | \/\___ \ )",
	R"( |____|   |___|  (____  /___|  /__|  \____/|__|_|  /  \______  /___|  (____  /__|_|  /___  /\___  >__|  /____  >)",
	R"(               \/     \/     \/                  \/          \/     \/     \/      \/    \/     \/           \/ )"
	};

	const std::vector<const char*> text_play{
		R"( _____ _         )",
		R"(|  _  | |___ _ _ )",
		R"(|   __| | .'| | |)",
		R"(|__|  |_|__,|_  |)",
		R"(            |___|)"
	};

	const std::vector<const char*> text_settings{
		R"( _____     _   _   _             )",
		R"(|   __|___| |_| |_|_|___ ___ ___ )",
		R"(|__   | -_|  _|  _| |   | . |_ -|)",
		R"(|_____|___|_| |_| |_|_|_|_  |___|)",
		R"(                        |___|    )"
	};

	const std::vector<const char*> text_credits{
		R"( _____           _ _ _       )",
		R"(|     |___ ___ _| |_| |_ ___ )",
		R"(|   --|  _| -_| . | |  _|_ -|)",
		R"(|_____|_| |___|___|_|_| |___|)"
	};

	const std::vector<const char*> text_exit{
		R"( _____     _ _   )",
		R"(|   __|_ _|_| |_ )",
		R"(|   __|_'_| |  _|)",
		R"(|_____|_,_|_|_|  )"
	};

	const std::vector<const char*> text_back{
		R"( _____         _   )",
		R"(| __  |___ ___| |_ )",
		R"(| __ -| .'|  _| '_|)",
		R"(|_____|__,|___|_,_|)"
	};

	const std::vector<const char*> text_video{
		R"( _____ _   _         )",
		R"(|  |  |_|_| |___ ___ )",
		R"(|  |  | | . | -_| . |)",
		R"( \___/|_|___|___|___|)",
	};

	const std::vector<const char*> text_audio{
		R"( _____       _ _     )",
		R"(|  _  |_ _ _| |_|___ )",
		R"(|     | | | . | | . |)",
		R"(|__|__|___|___|_|___|)",
	};

	auto init_menu_opts = [&](short opt_limit) {
		show_console_cursor(false);
		if (up_input) {
			game_sound.Play(menu_hover_sound);
			current_menu_opt--;
			if (current_menu_opt < 0) {
				current_menu_opt = opt_limit;
			}
		}

		if (down_input && current_menu_opt < opt_limit + 1) {
			game_sound.Play(menu_hover_sound);
			current_menu_opt++;
			if (current_menu_opt == opt_limit + 1) {
				current_menu_opt = 0;
			}
		}

		return;
	};

	auto show_menu_opt = [&](std::vector<const char*> art, int opt) {
		if (current_menu_opt == opt) {
			move_console_cursor(0, (screen_res[1] - menu_spacing) + (5 * current_menu_opt));
			printf("%s", menu_hover_cursor);
			print_art(selected_menu_padding, (screen_res[1] - menu_spacing) + (5 * current_menu_opt), art, selected_menu_color);
			return true;
		}
		else {
			print_art(default_menu_padding, (screen_res[1] - menu_spacing) + (5 * opt), art, default_menu_color);
			clear_rect(2, (screen_res[1] - menu_spacing) + (5 * opt) + 2, 2, 1);
			clear_rect(default_menu_padding + strlen(art[0]), (screen_res[1] - menu_spacing) + (5 * opt), 2, art.size());
			return false;
		}
	};

	short eye_blink_timer{};
	game_music.Play(menu_music);
	set_buffer_size(screen_res[0], screen_res[1]);
	for (;;) {
		if (current_menu_tab == 0) {
			print_art(default_menu_padding, 1, text_phantom_chambers);
			move_console_cursor((strlen(text_phantom_chambers[0]) + default_menu_padding), text_phantom_chambers.size());
#if NDEBUG
			printf("\033[2;37m[Release]v0.0.1\033[0m");
#else
			printf("\033[2;37m[Debug]v0.0.1\033[0m");
#endif

			for (;;) {
				init_menu_opts(3);
				if (eye_blink_timer > 210) {
					clear_rect(44, 3, 1, 1);
					move_console_cursor(43, 4);
					printf("<->");

					if (eye_blink_timer > 218) {
						eye_blink_timer = 0;
					}
				}
				else {
					move_console_cursor(44, 3);
					printf("_");
					move_console_cursor(43, 4);
					printf("(_)");
				}

				show_menu_opt(text_play, 0);
				show_menu_opt(text_settings, 1);
				show_menu_opt(text_credits, 2);
				show_menu_opt(text_exit, 3);
				if (GetAsyncKeyState(VK_RETURN) & 1) {
					current_menu_tab = (current_menu_opt + 1);
					break;
				}

				eye_blink_timer++;
				Sleep(1);
			}

			current_menu_opt = 0;
			system("cls");
		}

		if (current_menu_tab == 2) {
			print_art(default_menu_padding, 1, text_settings);
			for (;;) {
				init_menu_opts(2);
				show_menu_opt(text_video, 0);
				if ((GetAsyncKeyState(VK_RETURN) && current_menu_opt == 0) & 1) {
					create_tab(((screen_res[0] / 2) - 10), ((screen_res[1] / 2) - 10), 20, 4, "Video");
					move_console_cursor(((screen_res[0] / 2) - 7), ((screen_res[1] / 2) - 8));
					printf("Back");
					for (;;) {
						init_menu_opts(0);
						if (current_menu_opt == 0) {
							move_console_cursor(((screen_res[0] / 2) - 9), ((screen_res[1] / 2) - 8));
							printf("*");
							if (GetAsyncKeyState(VK_RETURN) & 1) {
								system("cls");
								print_art(default_menu_padding, 1, text_settings);
								break;
							}
						}
						else {
							clear_rect(((screen_res[0] / 2) - 9), ((screen_res[1] / 2) - 8), 1, 1);
						}
					}
				}

				show_menu_opt(text_audio, 1);
				if ((GetAsyncKeyState(VK_RETURN) && current_menu_opt == 1) & 1) {
					current_menu_opt = 0;
					create_tab(((screen_res[0] / 2) - 10), ((screen_res[1] / 2) - 10), 20, 4, "Audio");
					move_console_cursor(((screen_res[0] / 2) - 7), ((screen_res[1] / 2) - 8));
					printf("Back");
					for (;;) {
						init_menu_opts(0);
						if (current_menu_opt == 0) {
							move_console_cursor(((screen_res[0] / 2) - 9), ((screen_res[1] / 2) - 8));
							printf("*");
							if (GetAsyncKeyState(VK_RETURN) & 1) {
								system("cls");
								current_menu_opt = 1;
								print_art(default_menu_padding, 1, text_settings);
								break;
							}
						}
						else {
							clear_rect(((screen_res[0] / 2) - 9), ((screen_res[1] / 2) - 8), 1, 1);
						}
					}
				}

				show_menu_opt(text_back, 2);
				if ((GetAsyncKeyState(VK_RETURN) && current_menu_opt == 2) & 1) {
					current_menu_tab = 0;
					break;
				}
				
				Sleep(1);
			}
		}

		if (current_menu_tab == 3) {
			print_art(default_menu_padding, 1, text_credits);
			create_tab(((screen_res[0] / 2) - 10), ((screen_res[1] / 2) - 10), 20, 6, "Credits");
			move_console_cursor(((screen_res[0] / 2) - 8), ((screen_res[1] / 2) - 8));
			printf("Made by Twis7ed!");
			move_console_cursor(((screen_res[0] / 2) - 9), ((screen_res[1] / 2) - 6));
			printf("* Back");
			for (;;) {
				if (GetAsyncKeyState(VK_RETURN)) {
					current_menu_tab = 0;
					break;
				}
			}
		}

		if (current_menu_tab == 4) {
			break;
		}

		current_menu_opt = 0;
		system("cls");
		Sleep(1);
	}

	return 0;
}