#include "../../include/register/UserManager.hpp"
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip> // For std::setw, std::left

UserManager::UserManager(const std::string& filename)
    : _filename(filename) {
    loadUsers();
    std::cout << "UserManager initialized. Loaded " << _users.size() << " users from " << _filename << std::endl;
}

UserManager::~UserManager() {
    saveUsers();
    std::cout << "UserManager shutting down. Saved " << _users.size() << " users to " << _filename << std::endl;
}

void UserManager::loadUsers() {
    _users.clear();
    std::ifstream file(_filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string part;
            std::vector<std::string> parts;
            while (std::getline(ss, part, ',')) {
                parts.push_back(part);
            }

            // Expecting NEW format: name,nickname,score,gamesPlayed,bestScore
            if (parts.size() == 5) { // <--- Changed to 5 parts
                try {
                    std::string name = parts[0];
                    std::string nickname = parts[1];
                    int score = std::stoi(parts[2]); // This is the 'last game score'
                    int gamesPlayed = std::stoi(parts[3]); // <--- New field
                    int bestScore = std::stoi(parts[4]);   // <--- New field
                    _users.emplace_back(name, nickname, score, gamesPlayed, bestScore);
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing user line: '" << line << "'. " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Invalid line format in " << _filename << ": '" << line << "'. Skipping." << std::endl;
                // For existing "users.txt" files with 3 parts, this will print warnings.
                // You might add logic here to handle old formats, but for now, it's a warning.
            }
        }
        file.close();
    } else {
        std::cerr << "Warning: Could not open " << _filename << " for reading. Assuming no existing users." << std::endl;
    }
}

void UserManager::saveUsers() const {
    std::ofstream file(_filename);
    if (file.is_open()) {
        for (const auto& user : _users) {
            // NEW: Save all 5 fields
            file << user.name << "," << user.nickname << "," << user.score << ","
                 << user.gamesPlayed << "," << user.bestScore << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Could not open " << _filename << " for writing. User data not saved." << std::endl;
    }
}

bool UserManager::createUser(const std::string& name, const std::string& nickname) {
    for (const auto& user : _users) {
        if (user.nickname == nickname) {
            std::cout << "Error: Nickname '" << nickname << "' already exists. Please choose a different one." << std::endl;
            return false;
        }
    }

    // NEW: Initialize new user with 0 games played and 0 best score
    _users.emplace_back(name, nickname, 0, 0, 0);
    saveUsers();
    std::cout << "User '" << name << "' with nickname '" << nickname << "' created successfully!" << std::endl;
    return true;
}

void UserManager::listUsers() const {
    if (_users.empty()) {
        std::cout << "No users registered yet." << std::endl;
        return;
    }
    std::cout << "\n--- Registered Users ---" << std::endl;
    std::cout << "Name           Nickname       Games Played   Best Score" << std::endl; // <--- Updated header
    std::cout << "------------------------------------------------------" << std::endl;
    for (const auto& user : _users) {
        std::cout.width(15); std::cout << std::left << user.name;
        std::cout.width(15); std::cout << std::left << user.nickname;
        std::cout.width(15); std::cout << std::left << user.gamesPlayed; // <--- Display new field
        std::cout << user.bestScore << std::endl;                        // <--- Display new field
    }
    std::cout << "------------------------------------------------------\n" << std::endl;
}

const User* UserManager::getUserByNickname(const std::string& nickname) const {
    for (const auto& user : _users) {
        if (user.nickname == nickname) {
            return &user;
        }
    }
    return nullptr;
}

// NEW: Implementation of updateUserStats
bool UserManager::updateUserStats(const std::string& nickname, int currentScore) {
    // Iterate through users to find the one by nickname (need non-const access)
    for (auto& user : _users) { // Note: `auto& user` (not `const auto&`)
        if (user.nickname == nickname) {
            user.gamesPlayed++; // Increment games played
            user.score = currentScore; // Update last game score

            if (currentScore > user.bestScore) {
                user.bestScore = currentScore; // Update best score if record broken
                std::cout << "New high score for " << nickname << ": " << currentScore << "!" << std::endl;
            } else {
                 std::cout << "Score for " << nickname << ": " << currentScore << ". Best: " << user.bestScore << std::endl;
            }
            saveUsers(); // Save changes immediately
            return true;
        }
    }
    std::cerr << "Error: Could not update stats for nickname '" << nickname << "'. User not found." << std::endl;
    return false;
}