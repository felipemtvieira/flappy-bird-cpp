#include "Game.hpp"
#include "../include/register/UserManager.hpp"
#include "../include/dinamic/GameError.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;

int main() {
    UserManager userManager("users.txt");

    std::cout << "Welcome to Flappy Bird CLI!" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  create_user <name> <nickname> - Register a new player" << std::endl;
    std::cout << "  list_users                     - Show all registered players" << std::endl;
    std::cout << "  run_game <nickname>            - Start the game with a specific player" << std::endl;
    std::cout << "  exit                           - Quit the application" << std::endl;

    std::string command_line;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command_line);

        std::stringstream ss(command_line);
        std::string command;
        ss >> command;

        if (command == "create_user") {
            std::string name, nickname;
            if (ss >> name >> nickname) {
                userManager.createUser(name, nickname);
            } else {
                std::cout << "Usage: create_user <name> <nickname>" << std::endl;
            }
        } else if (command == "list_users") {
            userManager.listUsers();
        } else if (command == "run_game") {
            std::string nickname;
            if (ss >> nickname) {
                const User* currentUser = userManager.getUserByNickname(nickname);
                if (currentUser) {
                    std::cout << "Launching game for player: " << currentUser->name << " (" << currentUser->nickname << ")" << std::endl;

                    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
                    try {
                        game.initialize();
                        game.run(); 

                        int finalScore = game.getCurrentScore();
                        std::cout << "Game session ended. Final score: " << finalScore << std::endl;
                        userManager.updateUserStats(nickname, finalScore);
                    } catch (const GameError& e) {
                        std::cerr << "CRITICAL GAME ERROR: " << e.what() << std::endl;
                    } catch (const std::exception& e) { 
                        std::cerr << "UNEXPECTED ERROR: " << e.what() << std::endl;
                    } catch (...) { 
                        std::cerr << "UNKNOWN ERROR DURING GAME EXECUTION!" << std::endl;
                    }

                } else {
                    std::cout << "Error: Nickname '" << nickname << "' not found. Please create user first or list users." << std::endl;
                }
            } else {
                std::cout << "Usage: run_game <nickname>" << std::endl;
            }
        } else if (command == "exit") {
            std::cout << "Exiting Flappy Bird CLI. Goodbye!" << std::endl;
            break;
        } else if (command.empty()) {
            // Do nothing
        }
        else {
            std::cout << "Unknown command: '" << command << "'" << std::endl;
            std::cout << "Type 'help' for a list of commands." << std::endl;
        }
    }
    return 0;
}