#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "colors.h"
#include "user.h"

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_header() {
    std::cout << COLOR_CYAN << R"(
 _____                      _         _____            _
|     | ___  ___  ___  ___ | | ___   |   __| _ _  ___ | |_  ___  _____
|   --|| . ||   ||_ -|| . || || -_|  |__   || | ||_ -||  _|| -_||     |
|_____||___||_|_||___||___||_||___|  |_____||_  ||___||_|  |___||_|_|_|
                                            |___|
    )" << COLOR_RESET << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << COLOR_RED << "Usage: " << argv[0] << " <command> [options]" << COLOR_RESET << std::endl;
        return 1;
    }

    User catalog;
    std::ifstream catalog_data("user.txt");
    if (catalog_data.is_open()) {
        catalog.deserialize(catalog_data);
    } else {
        std::cerr << COLOR_YELLOW << "Warning: Could not open catalog file 'user.txt'. Catalog will be empty." << COLOR_RESET << std::endl;
    }

    User user;
    std::ifstream user_data("user2.txt");
    if (user_data.is_open()) {
        user.deserialize(user_data);
    }

    char* command = argv[1];

    if (strcmp(command, "view_catalog") == 0) {
        clear_console();
        print_header();
        std::cout << "Full item catalog from user.txt:" << std::endl;
        catalog.printAllConsoles();
    } else if (strcmp(command, "view_consoles") == 0) {
        clear_console();
        print_header();
        user.printOwnedConsoles(catalog);
    } else if (strcmp(command, "acquire_console") == 0) {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " acquire_console <console id>" << COLOR_RESET << std::endl;
            return 1;
        }
        if (user.acquireConsole(atoi(argv[2]), catalog)) {
            std::cout << COLOR_GREEN << "Console acquired successfully." << COLOR_RESET << std::endl;
        }
    } else if (strcmp(command, "install_game") == 0) {
        if (argc < 4) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " install_game <console id> <game id>" << COLOR_RESET << std::endl;
            return 1;
        }
        if (user.installGame(atoi(argv[2]), atoi(argv[3]), catalog)) {
            std::cout << COLOR_GREEN << "Game installed successfully." << COLOR_RESET << std::endl;
        }
    } else if (strcmp(command, "uninstall_game") == 0) {
        if (argc < 4) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " uninstall_game <console id> <game id>" << COLOR_RESET << std::endl;
            return 1;
        }
        if (user.uninstallGame(atoi(argv[2]), atoi(argv[3]))) {
            std::cout << COLOR_GREEN << "Game uninstalled successfully." << COLOR_RESET << std::endl;
        }
    } else if (strcmp(command, "view_games") == 0) {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " view_games <console id>" << COLOR_RESET << std::endl;
            return 1;
        }
        clear_console();
        print_header();
        user.viewInstalledGames(atoi(argv[2]), catalog);
    } else {
        std::cerr << COLOR_RED << "Command '" << command << "' is not valid." << COLOR_RESET << std::endl;
        std::cout << "Valid commands: view_catalog, view_consoles, acquire_console, install_game, uninstall_game, view_games" << std::endl;
        return 1;
    }

    std::ofstream user_out("user2.txt");
    user.serialize(user_out);

    return 0;
}
