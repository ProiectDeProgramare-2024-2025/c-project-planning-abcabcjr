#include <iostream>
#include <fstream>

#include "colors.h"
#include "console.h"
#include "user.h"

void clear_console() {
#if TARGET_OS_WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main(int argc, char* argv[]) {
    std::string header = R"(
 _____                      _         _____            _
|     | ___  ___  ___  ___ | | ___   |   __| _ _  ___ | |_  ___  _____
|   --|| . ||   ||_ -|| . || || -_|  |__   || | ||_ -||  _|| -_||     |
|_____||___||_|_||___||___||_||___|  |_____||_  ||___||_|  |___||_|_|_|
                                            |___|

    )";

    clear_console();
    std::cout << COLOR_CYAN << header << COLOR_RESET << std::endl;

    std::ifstream data("user.txt");

    User user;

    if (data.is_open())
        user.deserialize(data);

    if (argc < 2) {
        std::cerr << COLOR_RED << "Usage: " << argv[0] << " <command>" << COLOR_RESET << std::endl;
        return 1;
    }

    char* command = argv[1];

    if (strcmp(command, "view_consoles") == 0) {
        std::cout << "The following consoles are available:" << std::endl;
        user.printAllConsoles();
    } else if (strcmp(command, "add_console") == 0) {
        if (argc < 5) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " add_console <id> <name> <storage space>" << COLOR_RESET << std::endl;
            return 1;
        }

        Console console(atoi(argv[2]), atoi(argv[4]), argv[3]);

        if (user.getConsole(console.getId()) != nullptr) {
            std::cerr << COLOR_RED << "A console with ID " << console.getId() << " already exists!" << COLOR_RESET << std::endl;
            return 1;
        }
        user.addConsole(console);

        std::cout << COLOR_GREEN << "Console added successfully." << COLOR_RESET << std::endl;
    } else if (strcmp(command, "modify_console") == 0) {
        if (argc < 5) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " modify_console <id> <name> <storage space>" << COLOR_RESET << std::endl;
            return 1;
        }

        Console* console = user.getConsole(atoi(argv[2]));
        if (console == nullptr) {
            std::cerr << COLOR_RED << "There is no console with this ID." << COLOR_RESET << std::endl;
            return 1;
        }

        console->modify(argv[3], atoi(argv[4]));

        std::cout << COLOR_GREEN << "Console modified successfully: " << COLOR_RESET << std::endl;
        console->printEntry();
    } else if (strcmp(command, "delete_console") == 0) {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " delete_console <id> " << COLOR_RESET << std::endl;
            return 1;
        }

        if (!user.removeConsole(atoi(argv[2])) != 0) {
            std::cerr << COLOR_RED << "There is no console with this ID." << COLOR_RESET << std::endl;
            return 1;
        }

        std::cout << COLOR_GREEN << "Console deleted successfully." << COLOR_RESET << std::endl;
    } else if (strcmp(command, "add_game") == 0) {
        if (argc < 6) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " add_game <console id> <id> <name> <required space>" << COLOR_RESET << std::endl;
            return 1;
        }

        Console* console = user.getConsole(atoi(argv[2]));
        if (console == nullptr) {
            std::cerr << COLOR_RED << "There is no console with this ID." << COLOR_RESET << std::endl;
            return 1;
        }

        Game game(atoi(argv[3]), atoi(argv[2]), atoi(argv[5]), argv[4]);

        if (!console->addGame(game)) {
            std::cerr << COLOR_RED << "A game with this ID already exists for this console." << COLOR_RESET << std::endl;
            return 1;
        }

        std::cout << COLOR_GREEN << "Game added successfully." << COLOR_RESET << std::endl;
    } else if (strcmp(command, "delete_game") == 0) {
        if (argc < 4) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " delete_game <console id> <game id>" << COLOR_RESET << std::endl;
            return 1;
        }

        Console* console = user.getConsole(atoi(argv[2]));
        if (console == nullptr) {
            std::cerr << COLOR_RED << "There is no console with this ID." << COLOR_RESET << std::endl;
            return 1;
        }

        if (!console->removeGame(atoi(argv[3]))) {
            std::cerr << COLOR_RED << "A game with this ID does not exist on this console." << COLOR_RESET << std::endl;
            return 1;
        }

        std::cout << COLOR_GREEN << "Game deleted successfully." << COLOR_RESET << std::endl;
    } else if (strcmp(command, "view_games") == 0) {
        if (argc < 3) {
            std::cerr << COLOR_RED << "Usage: " << argv[0] << " view_games <console id>" << COLOR_RESET << std::endl;
            return 1;
        }

        Console* console = user.getConsole(atoi(argv[2]));
        if (console == nullptr) {
            std::cerr << COLOR_RED << "There is no console with this ID." << COLOR_RESET << std::endl;
            return 1;
        }

        std::cout << "The following games are available:" << std::endl;
        console->printAllGames();
    } else {
        std::cerr << COLOR_RED << "Command " << command << " is not a valid command." << COLOR_RESET << std::endl;
        return 1;
    }

    std::ofstream out("user.txt");
    user.serialize(out);

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.