#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include "User.hpp"
#include <vector>
#include <string>
#include <fstream>

class UserManager {
private:
    std::vector<User> _users; // Armazena todos os usuários carregados
    std::string _filename;    // O arquivo onde os usuários são salvos/carregados (e.g., "users.txt")

    void loadUsers();
    void saveUsers() const;

public:
    // Constructor: carrega usuários do arquivo especificado
    UserManager(const std::string& filename = "users.txt");
    ~UserManager();

    bool createUser(const std::string& name, const std::string& nickname);

    void listUsers() const;

    const User* getUserByNickname(const std::string& nickname) const;

    bool updateUserStats(const std::string& nickname, int currentScore);
};

#endif