# LOGIN_REGISTER_SYSTEM_IYORE

## Description
LOGIN_REGISTER_SYSTEM_IYORE is a simple login and registration system implemented in C using SDL2 and SDL2_TTF libraries. The project aims to provide a user-friendly graphical interface that allows users to register a new account or log in to an existing account. 

## Features
- **Graphical User Interface (GUI):** 
  - Intuitive and interactive interface for user input (username and password).
  - Buttons for registration and login actions.

- **Secure Login System:**
  - Validates user credentials during login.
  - Password masking with asterisks during input for security.

- **New User Registration:**
  - Allows users to create new accounts.
  - Checks for existing usernames to prevent duplicates.
  - Stores user information (username and password) in a text file.

- **Enhanced User Experience:**
  - Smooth interactions with UI elements, including hover effects.
  - Dynamic text input handling with a cursor display.

## Technologies Used
- **Programming Language:** C
- **Libraries:**
  - SDL2 (for creating the graphical interface and handling events)
  - SDL2_TTF (for rendering text)
- **File Management:** Stores user data in a text file for persistence.

## Files Overview
- `main.c`: The main entry point of the application.
- `iyore.c`: Contains helper functions for managing the user interface.
- `menu.c`: Handles menu display and registration/login operations.
- `iyore.h`: Header file with function declarations and structure definitions.

## Getting Started
To compile and run the project, ensure you have the SDL2 and SDL2_TTF libraries installed. Use the provided Makefile to build the application.

## Contributing
Contributions are welcome! Please feel free to open issues or submit pull requests.