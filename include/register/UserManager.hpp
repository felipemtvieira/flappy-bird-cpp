#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include "User.hpp" // Include the User struct
#include <vector>   // For std::vector
#include <string>   // For std::string
#include <fstream>  // For file operations

class UserManager {
private:
    std::vector<User> _users; // Stores all loaded users
    std::string _filename;    // The file where users are saved/loaded (e.g., "users.txt")

    // Helper method to load users from the file
    void loadUsers();
    // Helper method to save users to the file
    void saveUsers() const;

public:
    // Constructor: loads users from the specified file
    UserManager(const std::string& filename = "users.txt");
    ~UserManager();

    // Attempts to create a new user. Returns true on success, false if nickname already exists.
    bool createUser(const std::string& name, const std::string& nickname);

    // Lists all registered users to console.
    void listUsers() const;

    // Finds a user by nickname. Returns pointer to User if found, nullptr otherwise.
    // Note: Returns const pointer as it's read-only access for external modules.
    const User* getUserByNickname(const std::string& nickname) const;

    // NEW: Method to update user's game stats after a game session
    // Returns true if user was found and updated, false otherwise.
    bool updateUserStats(const std::string& nickname, int currentScore);
};

#endif // USER_MANAGER_HPP