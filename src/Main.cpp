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

int main() { // Reverted: No argc, argv here
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

                    // *** LAUNCH THE GRAPHICAL GAME ***
                    // No debugMode parameter needed for Game constructor here
                    Game game(SCREEN_WIDTH, SCREEN_HEIGHT);
                    try { // NEW: Try block for game initialization
                        if (!game.initialize()) {
                            // This path should ideally not be taken if initialize() throws on error
                            std::cerr << "Game initialization failed for " << currentUser->nickname << " (returned false)!" << std::endl;
                        } else {
                            game.run(); // This blocks until game window closes

                            int finalScore = game.getCurrentScore();
                            std::cout << "Game session ended. Final score: " << finalScore << std::endl;
                            userManager.updateUserStats(nickname, finalScore);
                        }
                    } catch (const GameError& e) { // Catch our custom game errors
                        std::cerr << "CRITICAL GAME ERROR: " << e.what() << std::endl;
                        // Depending on the severity, you might want to exit the entire program here
                        // For now, it returns to the CLI, allowing other commands.
                    } catch (const std::exception& e) { // Catch any other standard exceptions
                        std::cerr << "UNEXPECTED ERROR: " << e.what() << std::endl;
                    } catch (...) { // Catch any unknown exceptions
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