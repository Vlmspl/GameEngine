#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @class File
 * @brief Handles file operations
 */
class File {
public:
    /** @brief The path to the file. */
    std::string filePath;

    /**
     * @brief Reads the entire content of the file.
     *
     * This function opens the file, reads its content into a string, and returns it.
     * In case of an error (e.g., file not found), an empty string is returned.
     *
     * @return A string containing the contents of the file.
     */
    std::string read() const {
        std::ifstream file(filePath);
        if (!file) {
            std::cerr << "ERROR: Unable to open file: " << filePath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /**
     * @brief Checks if the file exists.
     *
     * @return True if the file exists, false otherwise.
     */
    bool exists() const {
        std::ifstream file(filePath);
        return file.good();
    }

    /**
     * @brief Gets the file path associated with this File object.
     *
     * @return The file path.
     */
    std::string getPath() const {
        return filePath;
    }

    /**
     * @brief Static method to create a File object if the file exists at the specified path.
     *
     * This method checks if the file exists at the given path and returns a File object if valid.
     * If the file does not exist, it returns a nullptr.
     *
     * @param path The file path to check and associate with the File object.
     * @return A pointer to a File object if the file exists, nullptr otherwise.
     */
    static File* find(const std::string& path) {
        std::ifstream file(path);
        if (file.good()) {
            return new File(path); // File exists, return a new File object
        } else {
            std::cerr << "ERROR: File not found at: " << path << std::endl;
            return nullptr; // File does not exist
        }
    }

private:
    /**
     * @brief Private constructor to restrict direct instantiation.
     *
     * This ensures that a File object can only be created through the static `find` method.
     *
     * @param path The file path to be associated with the File object.
     */
    File(const std::string &path) : filePath(path) {}

    // No additional private members are necessary for this simple class
};
