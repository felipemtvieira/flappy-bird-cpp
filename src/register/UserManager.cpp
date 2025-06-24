#include "../../include/register/UserManager.hpp"
#include <iostream>
#include <sstream>
#include <limits>

UserManager::UserManager(const std::string& filename)
    : _filename(filename) {
    loadUsers();
    std::cout << "UserManager initialized. Loaded " << this->_users.size() << " users from " << this->_filename << std::endl;
}

UserManager::~UserManager() {
    saveUsers();
    std::cout << "UserManager shutting down. Saved " << this->_users.size() << " users to " << this->_filename << std::endl;
}

void UserManager::loadUsers() {
    this->_users.clear();
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

            if (parts.size() == 5) {
                try {
                    std::string name = parts[0];
                    std::string nickname = parts[1];
                    int score = std::stoi(parts[2]); 
                    int gamesPlayed = std::stoi(parts[3]); 
                    int bestScore = std::stoi(parts[4]);   
                    this->_users.emplace_back(name, nickname, score, gamesPlayed, bestScore);
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing user line: '" << line << "'. " << e.what() << std::endl;
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Warning: Could not open " << this->_filename << " for reading. Assuming no existing users." << std::endl;
    }
}

void UserManager::saveUsers() const {
    std::ofstream file(this->_filename);
    if (file.is_open()) {
        for (const auto& user : this->_users) {
            file << user.name << "," << user.nickname << "," << user.score << ","
                 << user.gamesPlayed << "," << user.bestScore << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Could not open " << _filename << " for writing. User data not saved." << std::endl;
    }
}

bool UserManager::createUser(const std::string& name, const std::string& nickname) {
    for (const auto& user : this->_users) {
        if (user.nickname == nickname) {
            std::cout << "Error: Nickname '" << nickname << "' already exists. Please choose a different one." << std::endl;
            return false;
        }
    }

    this->_users.emplace_back(name, nickname, 0, 0, 0);
    saveUsers();
    std::cout << "User '" << name << "' with nickname '" << nickname << "' created successfully!" << std::endl;
    return true;
}

void UserManager::listUsers() const {
    if (this->_users.empty()) {
        std::cout << "No users registered yet." << std::endl;
        return;
    }
    std::cout << "\n--- Registered Users ---" << std::endl;
    std::cout << "Name           Nickname       Games Played   Best Score" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    for (const auto& user : this->_users) {
        std::cout.width(15); std::cout << std::left << user.name;
        std::cout.width(15); std::cout << std::left << user.nickname;
        std::cout.width(15); std::cout << std::left << user.gamesPlayed; 
        std::cout << user.bestScore << std::endl;                      
    }
    std::cout << "------------------------------------------------------\n" << std::endl;
}

const User* UserManager::getUserByNickname(const std::string& nickname) const {
    for (const auto& user : this->_users) {
        if (user.nickname == nickname) {
            return &user;
        }
    }
    return nullptr;
}

bool UserManager::updateUserStats(const std::string& nickname, int currentScore) {
    for (auto& user : this->_users) {
        if (user.nickname == nickname) {
            user.gamesPlayed++;
            user.score = currentScore;

            if (currentScore > user.bestScore) {
                user.bestScore = currentScore;
                std::cout << "New high score for " << nickname << ": " << currentScore << "!" << std::endl;
            } else {
                 std::cout << "Score for " << nickname << ": " << currentScore << ". Best: " << user.bestScore << std::endl;
            }
            saveUsers();
            return true;
        }
    }
    std::cerr << "Error: Could not update stats for nickname '" << nickname << "'. User not found." << std::endl;
    return false;
}