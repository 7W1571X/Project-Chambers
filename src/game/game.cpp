#include "../stdafx.h"
#include "../audio/audio.h"
#include "../console/console.hpp"
#include "../menu/menu.hpp"

void game_main(void) {	
	std::array<short, 2> screen_res{ get_console_res() };
	auto scroll_cout = [](const char* text) {
		for (int i{}; i < strlen(text); ++i) {
			std::cout << text[i] << std::flush;
			sleep_for(30);
		}

		return;
	};

	create_tab(0, 0, ((screen_res[0] / 3) * 2), ((screen_res[1] / 3) * 2), "Visual");
	create_tab(0, ((screen_res[1] / 3) * 2), ((screen_res[0] / 3) * 2), 18, "Text");
	create_tab((((screen_res[0] / 3) * 2) + 1), 0, 69, screen_res[1], "Map");
	for (;;) {
		move_console_cursor(3, (((screen_res[1] / 3) * 2) + 2));
		scroll_cout("YOOO, its some animated text, thats pretty dope!");
		clear_rect(3, (((screen_res[1] / 3) * 2) + 2), 49, 1);

		sleep_for(1);
	}

	return;
}