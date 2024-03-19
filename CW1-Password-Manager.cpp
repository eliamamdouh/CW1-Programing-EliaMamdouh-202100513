#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

// Caesar cipher encryption function
std::string encryptPassword(const std::string& password, int shift) {
    std::string encryptedPassword = password;
    for (char& c : encryptedPassword) {
        if (isalnum(c)) { // Check if character is alphanumeric
            char base;
            if (isdigit(c)) {
                base = '0';
            }
            else if (isupper(c)) {
                base = 'A';
            }
            else {
                base = 'a';
            }
            c = ((c - base + shift) % 26) + base; 
        }
    }
    return encryptedPassword;
}

// Caesar cipher decryption function
std::string decryptPassword(const std::string& encryptedPassword, int shift) {
    return encryptPassword(encryptedPassword, 26 - shift); // Reversing the shift for decryption
}

// Function to sign up
void signUp() {
    std::ofstream usersFile("users.txt", std::ios::app);
    if (!usersFile) {
        std::cerr << "Error: Could not open users file." << std::endl;
        return;
    }

    std::string email, password;
    std::cout << "Enter your email: ";
    std::cin >> email;
    std::cout << "Enter your password: ";
    std::cin >> password;

    // Encrypt password
    password = encryptPassword(password, 3);

    // Write email and password to file
    usersFile << email << " " << password << std::endl;
    usersFile.close();

    std::cout << "Account created successfully!" << std::endl;
}

// Function to log in
bool login(std::string& email, std::string& password) {
    std::ifstream usersFile("users.txt");
    if (!usersFile) {
        std::cerr << "Error: Could not open users file." << std::endl;
        return false;
    }

    std::string storedEmail, storedPassword;
    std::cout << "Enter your email: ";
    std::cin >> email;
    std::cout << "Enter your password: ";
    std::cin >> password;

    while (usersFile >> storedEmail >> storedPassword) {
        if (email == storedEmail && password == decryptPassword(storedPassword, 3)) {
            std::cout << "Login successful!" << std::endl;
            usersFile.close();
            return true;
        }
    }

    std::cout << "Invalid email or password. Login failed!" << std::endl;
    usersFile.close();
    return false;
}

// Function to delete a user
void deleteUser(const std::string& email) {
    std::ifstream usersFileIn("users.txt");
    if (!usersFileIn) {
        std::cerr << "Error: Could not open users file." << std::endl;
        return;
    }

    std::ofstream usersFileOut("temp.txt");
    if (!usersFileOut) {
        std::cerr << "Error: Could not create temporary file." << std::endl;
        return;
    }

    std::string storedEmail, storedPassword;
    while (usersFileIn >> storedEmail >> storedPassword) {
        if (email != storedEmail) {
            usersFileOut << storedEmail << " " << storedPassword << std::endl;
        }
    }

    usersFileIn.close();
    usersFileOut.close();

    if (remove("users.txt") != 0) {
        std::cerr << "Error: Failed to delete user." << std::endl;
        return;
    }

    if (rename("temp.txt", "users.txt") != 0) {
        std::cerr << "Error: Failed to update users file." << std::endl;
        return;
    }

    std::cout << "User deleted successfully!" << std::endl;
}
// Function to store password for a website
void storePassword(const std::string& email) {
    std::ofstream passwordsFile("passwords.txt", std::ios::app);
    if (!passwordsFile) {
        std::cerr << "Error: Could not open passwords file." << std::endl;
        return;
    }

    std::string website, password;
    std::cout << "Enter website: ";
    std::cin >> website;

    int choice;
    std::cout << "Choose an option:\n1. Generate a random password\n2. Enter your own password\nEnter your choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1: {
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const int charsetSize = charset.size();
        const int passwordLength = 12; 

        password = "";
        for (int i = 0; i < passwordLength; ++i) {
            password += charset[rand() % charsetSize];
        }
        break;
    }
    case 2: {
        std::cout << "Enter password: ";
        std::cin >> password;
        break;
    }
    default:
        std::cout << "Invalid choice. Using a generated password." << std::endl;
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        const int charsetSize = charset.size();
        const int passwordLength = 12; 

        password = "";
        for (int i = 0; i < passwordLength; ++i) {
            password += charset[rand() % charsetSize];
        }
        break;
    }

    // Encrypt password
    password = encryptPassword(password, 4);

    // Write email, website, and password to file
    passwordsFile << email << " " << website << " " << password << std::endl;
    passwordsFile.close();

    std::cout << "Password for " << website << " stored successfully!" << std::endl;
}


// Function to show stored passwords
void showStoredPasswords(const std::string& email) {
    std::ifstream passwordsFile("passwords.txt");
    if (!passwordsFile) {
        std::cerr << "Error: Could not open passwords file." << std::endl;
        return;
    }

    std::string userEmail, website, encryptedPassword;
    while (passwordsFile >> userEmail >> website >> encryptedPassword) {
        if (email == userEmail) {
            std::cout << "Website: " << website << ", Password: " << decryptPassword(encryptedPassword, 4) << std::endl;
        }
    }

    passwordsFile.close();
}

int main() {
    int choice;
    std::string email, password;

    do {
        std::cout << "\nMenu:\n1. Sign up\n2. Log in\n3. Exit\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            signUp();
            break;
        case 2:
            if (login(email, password)) {
                int subChoice;
                std::cout << "\n1. Store you websites' passwords \n2. Show your stored passwords\n3. Delete user\n4. Return to main menu\nEnter your choice: ";
                std::cin >> subChoice;
                switch (subChoice) {
                case 1:
                    storePassword(email);
                    break;
                case 2:
                    showStoredPasswords(email);
                    break;
                case 3:
                    deleteUser(email);
                    return 0; // Exiting after deleting the user
                case 4:
                    // Returning to the main menu
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
            break;
        case 3:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 3);

    return 0;
}

