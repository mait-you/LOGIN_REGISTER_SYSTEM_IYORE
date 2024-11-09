#include "lib/iyore.h"
#include "lib/menu.h"

// Set text colors
SDL_Color bgColor = {246, 245, 255, 255};
SDL_Color textColor = {26, 26, 26, 255};
SDL_Color bgTextColor = {111, 111, 111, 255};
SDL_Color titleColor = {94, 99, 238, 255};
SDL_Color topicsColor = {0, 0, 0, 255};
SDL_Color buttonColor = {228, 233, 247, 255};
SDL_Color redColor = {255, 0, 0, 255};
SDL_Color greenColor = {0, 255, 0, 255};

SDL_Color activeButtonColor;
SDL_Color inactiveButtonColor;

// Button positions and sizes
SDL_Rect _registerButton = {10, WINDOW_HEIGHT - 70, (WINDOW_WIDTH / 2) - 10, 60};
SDL_Rect _loginButton = {WINDOW_WIDTH / 2, WINDOW_HEIGHT - 70, (WINDOW_WIDTH / 2) - 10, 60};
SDL_Rect userNameInputButton = {50, 160, WINDOW_WIDTH - 100, 30};
SDL_Rect passwordInputButton = {50, 240, WINDOW_WIDTH - 100, 30};
SDL_Rect _eyeButton = {WINDOW_WIDTH - 70, 245, 40, 20};

// Variables
User newUser;
char hidingPassword[20]; // Hidden password displayed with '*'
int hidePassword = 1;    // 1 = hidden, 0 = shown
int inputField = 0;      // 0 => username, 1 => password
int emptyUsername;
int emptyPassword;

int Login(void)
{
    // Set colors
    activeButtonColor.r = 197;
    activeButtonColor.g = 201;
    activeButtonColor.b = 212;
    activeButtonColor.a = 255;

    inactiveButtonColor.r = 28;
    inactiveButtonColor.g = 54;
    inactiveButtonColor.b = 97;
    inactiveButtonColor.a = 255;

    // Clean up and finalize the registration process
    fullClear();
    printLogin();
    srand(time(NULL));

    // Variables
    SDL_Event event;
    int textWidth, textHeight;
    int running = 1;
    int userNameExists = -1; // -1 means untested, 0 means available, 1 means exists
    int passWordExists = -1; // -1 means untested, 0 means available, 1 means exists

    // Main event loop
    while (running)
    {
        while (SDL_PollEvent(&event)) // Poll events
        {
            if (event.type == SDL_QUIT)
                running = 0; // Quit if SDL_QUIT event is received

            // Handle text input
            if (event.type == SDL_TEXTINPUT)
            {
                // Typing in the username field
                if (inputField == 0)
                {
                    ft_strncat(newUser.username, event.text.text, sizeof(newUser.username) - ft_strlen(newUser.username) - 1);
                }
                // Typing in the password field
                if (inputField == 1)
                {
                    ft_strncat(newUser.password, event.text.text, sizeof(newUser.password) - ft_strlen(newUser.password) - 1);
                    hidingPassword[ft_strlen(hidingPassword)] = '*';
                    // strncat(hidingPassword, "•", sizeof(hidingPassword) - strlen(hidingPassword) - 1);
                    hidingPassword[ft_strlen(hidingPassword) + 1] = '\0';
                }
            }

            // Mouse click events
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Check if user clicked on username input field
                if (isMouseOverButton(&userNameInputButton, mouseX, mouseY))
                {
                    inputField = 0; // Focus on username input field
                    putStr("Username Input\n");
                }

                // Check if user clicked on password input field
                if (isMouseOverButton(&passwordInputButton, mouseX, mouseY))
                {
                    inputField = 1; // Focus on password input field
                    putStr("Password Input\n");
                }

                // Check if user clicked on the register button
                if (isMouseOverButton(&_registerButton, mouseX, mouseY))
                {
                    return (1);
                }

                // Toggle password visibility on eye button click
                if (isMouseOverButton(&_eyeButton, mouseX, mouseY))
                {
                    hidePassword = !hidePassword; // Toggle between 0 and 1
                    putStr(hidePassword ? "Password hiding Enabled\n" : "Password hiding Disabled\n");
                }

                // Login button click (can be implemented separately)
                if (isMouseOverButton(&_loginButton, mouseX, mouseY))
                {
                    emptyUsername = 0;
                    emptyPassword = 0;
                    userNameExists = 0;

                    if (newUser.username[0] == '\0') // If username is empty
                    {
                        emptyUsername = 1;
                        putStr("Username field is empty !!\n");
                    }
                    if (newUser.password[0] == '\0') // If password is empty
                    {
                        emptyPassword = 1;
                        putStr("Password field is empty !!\n");
                    }
                    if (isUserRegistered(newUser.username, newUser.password, FILE_NAME) == 0 && emptyUsername == 0) // Check if username already exists
                    {
                        inputField = 0;
                        userNameExists = 0;
                        passWordExists = 0;
                        putStr("Username & password not exists\n");
                        ft_memset(newUser.username, '\0', sizeof(newUser.username));
                        ft_memset(newUser.password, '\0', sizeof(newUser.password));
                        ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                    }
                    if (isUserRegistered(newUser.username, newUser.password, FILE_NAME) == 10 && emptyUsername == 0) // Check if username already exists
                    {
                        inputField = 0;
                        userNameExists = 1;
                        passWordExists = 0;
                        putStr("Password not exists\n");
                        ft_memset(newUser.username, '\0', sizeof(newUser.username));
                        ft_memset(newUser.password, '\0', sizeof(newUser.password));
                        ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                    }
                    if (isUserRegistered(newUser.username, newUser.password, FILE_NAME) == 20 && emptyUsername == 0 && emptyPassword == 0 && userNameExists == 0)
                    {
                        inputField = 0;
                        userNameExists = 0;
                        passWordExists = 0;
                        emptyUsername = 0;
                        emptyPassword = 0;
                        inputField = 0;
                        putStr("Login successful!\n");
                        ft_memset(newUser.username, '\0', sizeof(newUser.username));
                        ft_memset(newUser.password, '\0', sizeof(newUser.password));
                        ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                    }
                }
            }

            // Handle keyboard navigation
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_DOWN)
                    inputField = !inputField; // Switch to password field => inputField = (inputField == 0) ? 1 : 0;
                if (event.key.keysym.sym == SDLK_UP)
                    inputField = !inputField;            // Switch to username field
                if (event.key.keysym.sym == SDLK_RETURN) // Move to next field on Enter key
                {
                    if (inputField == 1)
                    {
                        emptyUsername = 0;
                        emptyPassword = 0;
                        userNameExists = 0;

                        if (newUser.username[0] == '\0') // If username is empty
                        {
                            emptyUsername = 1;
                            putStr("Username field is empty !!\n");
                        }
                        if (newUser.password[0] == '\0') // If password is empty
                        {
                            emptyPassword = 1;
                            putStr("Password field is empty !!\n");
                        }
                        if (isUserRegistered(newUser.username, newUser.password, FILE_NAME) == 0 && emptyUsername == 0) // Check if username already exists
                        {
                            inputField = 0;
                            userNameExists = 0;
                            passWordExists = 0;
                            putStr("Username & password not exists\n");
                            ft_memset(newUser.username, '\0', sizeof(newUser.username));
                            ft_memset(newUser.password, '\0', sizeof(newUser.password));
                            ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                        }
                        if (isUserRegistered(newUser.username, newUser.password, FILE_NAME) == 10 && emptyUsername == 0) // Check if username already exists
                        {
                            inputField = 1;
                            userNameExists = 1;
                            passWordExists = 0;
                            putStr("Password not exists\n");
                            ft_memset(newUser.password, '\0', sizeof(newUser.password));
                            ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                        }
                        if (isUserRegistered(newUser.username, newUser.password, FILE_NAME) == 20 && emptyUsername == 0 && emptyPassword == 0 && userNameExists == 0)
                        {
                            inputField = 0;
                            userNameExists = 1;
                            passWordExists = 1;
                            emptyUsername = 0;
                            emptyPassword = 0;
                            inputField = 0;
                            putStr("Login successful!\n");
                            ft_memset(newUser.username, '\0', sizeof(newUser.username));
                            ft_memset(newUser.password, '\0', sizeof(newUser.password));
                            ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                        }
                    }
                    else
                        inputField = 1;
                }

                // If the user is typing in the username field
                if (inputField == 0)
                {
                    // Dealing with deletion (Backspace):
                    if (event.key.keysym.sym == SDLK_BACKSPACE && ft_strlen(newUser.username) > 0)
                    {
                        newUser.username[ft_strlen(newUser.username) - 1] = '\0'; // Remove last character
                        // tempCursorPos[ft_strlen(username) - 1] = '\0'; // Remove last character
                    }
                }

                // If the user is typing in the password field
                if (inputField == 1)
                {
                    // Dealing with deletion (Backspace):
                    if (event.key.keysym.sym == SDLK_BACKSPACE && ft_strlen(newUser.password) > 0)
                    {
                        newUser.password[ft_strlen(newUser.password) - 1] = '\0'; // Remove last character
                        hidingPassword[ft_strlen(hidingPassword) - 1] = '\0';     // Remove asterisk
                    }
                }
                // if (event.key.keysym.sym == SDLK_LEFT)
                // {
                //     if (inputField == 0 && cursorPosition > 0)
                //         cursorPosition--; // Move cursor to the left
                // }
                // else if (event.key.keysym.sym == SDLK_RIGHT)
                // {
                //     if (inputField == 0 && cursorPosition < (int)ft_strlen(username))
                //         cursorPosition++; // Move cursor to the right
                // }
            }
        }

        // Clear screen and set background color
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);

        // Render page title
        TTF_SizeText(titleFont, "IYORE", &textWidth, &textHeight);
        renderImg(renderer, IRiconImgSF, (WINDOW_WIDTH - 93 - textWidth) / 2, 5, 100, 100);
        renderText(renderer, titleFont, titleColor, "IYORE", (WINDOW_WIDTH - textWidth + 93) / 2, 20);

        // Render "Register" heading
        renderText(renderer, topicsFont, topicsColor, "Login", 40, textHeight + 30);

        /*
            Render username input section
        */
        TTF_SizeText(textfont, "Enter username: ", &textWidth, &textHeight);
        renderText(renderer, textfont, bgTextColor, "Enter username: ", 50, 160);
        renderText(renderer, textfont, textColor, newUser.username, textWidth + 50, 160);

        // Dealing with cursor:
        // ft_strncpy(tempCursorPos, username, cursorPosition);
        TTF_SizeText(textfont, newUser.username, &textWidth, &textHeight);
        if (inputField == 0)
        {
            int cursorX = textWidth + 203;
            if (SDL_GetTicks() % 1000 < 500) // Make cursor blink
            {
                SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
                SDL_RenderDrawLine(renderer, cursorX, 163, cursorX, 183);
            }
        }

        SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
        for (int i = 0; i < 2; i++)
            SDL_RenderDrawLine(renderer, 50, 190 + i, WINDOW_WIDTH - 50, 190 + i);

        // Show error messages if needed
        if (emptyUsername)
            renderText(renderer, smallfont, redColor, "Username field is empty !!", 50, 195);
        if (userNameExists == 0)
            renderText(renderer, smallfont, redColor, "Username not exists !!", 50, 195);

        /*
            Render password input section
        */
        TTF_SizeText(textfont, "Enter password: ", &textWidth, &textHeight);
        renderText(renderer, textfont, bgTextColor, "Enter password: ", 50, 240);
        renderImg(renderer, hidePassword ? eyeSlashImgSF : eyeImgSF, WINDOW_WIDTH - 70, 245, 20, 20);
        renderText(renderer, hidePassword ? hidingPasswordFont : textfont, textColor, hidePassword ? hidingPassword : newUser.password, textWidth + 55, 240);

        // Dealing with cursor:
        // ft_strncpy(tempCursorPos, username, cursorPosition);
        TTF_SizeText(textfont, newUser.password, &textWidth, &textHeight);
        if (inputField == 1)
        {
            int cursorX = textWidth + 203;
            if (SDL_GetTicks() % 1000 < 500) // Make cursor blink
            {
                SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
                SDL_RenderDrawLine(renderer, cursorX, 243, cursorX, 263);
            }
        }

        SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
        for (int i = 0; i < 2; i++)
            SDL_RenderDrawLine(renderer, 50, 270 + i, WINDOW_WIDTH - 50, 270 + i);

        // Show error messages if needed
        if (emptyPassword)
            renderText(renderer, smallfont, redColor, "Password field is empty !!", 50, 275);

        if (passWordExists == 0)
            renderText(renderer, smallfont, redColor, "Password not exists !!", 50, 275);

        TTF_SizeText(textfont, "Login successful !!", &textWidth, &textHeight);
        if (emptyUsername == 0 && emptyPassword == 0 && userNameExists == 1 && passWordExists == 1)
            renderText(renderer, textfont, greenColor, "Login successful !!", (WINDOW_WIDTH / 2) - (textWidth / 2), 290);

        // Render register button
        TTF_SizeText(topicsFont, "Register", &textWidth, &textHeight);
        SDL_SetRenderDrawColor(renderer, activeButtonColor.r, activeButtonColor.g, activeButtonColor.b, 255);
        SDL_RenderFillRect(renderer, &_registerButton);
        renderText(renderer, topicsFont, textColor, "Register", _registerButton.x + (_registerButton.w - textWidth) / 2, _registerButton.y + (_registerButton.h - textHeight) / 2);

        // Render login button
        TTF_SizeText(topicsFont, "Login", &textWidth, &textHeight);
        SDL_SetRenderDrawColor(renderer, inactiveButtonColor.r, inactiveButtonColor.g, inactiveButtonColor.b, 255);
        SDL_RenderFillRect(renderer, &_loginButton);
        renderText(renderer, topicsFont, textColor, "Login", _loginButton.x + (_loginButton.w - textWidth) / 2, _loginButton.y + (_loginButton.h - textHeight) / 2);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Print entered username and password (for debugging purposes)
    putStr(newUser.username);
    putChar('\n');
    putStr(newUser.password);
    putChar('\n');
    return (0);
}

/*
    Function to handle user registration
 */
int Register(void)
{
    // Set colors
    activeButtonColor.r = 28;
    activeButtonColor.g = 54;
    activeButtonColor.b = 97;
    activeButtonColor.a = 255;

    inactiveButtonColor.r = 197;
    inactiveButtonColor.g = 201;
    inactiveButtonColor.b = 212;
    inactiveButtonColor.a = 255;

    // Clean up and finalize the registration process
    fullClear();
    printRegister();
    srand(time(NULL));

    // Variables
    newUser.userId = generateUniqueId(); // Generate a unique user ID
    SDL_Event event;
    int textWidth, textHeight;
    int running = 1;
    int userNameExists = -1; // -1 means untested, 0 means available, 1 means exists

    // Main event loop
    while (running)
    {
        while (SDL_PollEvent(&event)) // Poll events
        {
            if (event.type == SDL_QUIT)
                running = 0; // Quit if SDL_QUIT event is received

            // Handle text input
            if (event.type == SDL_TEXTINPUT)
            {
                // Typing in the username field
                if (inputField == 0)
                {
                    ft_strncat(newUser.username, event.text.text, sizeof(newUser.username) - ft_strlen(newUser.username) - 1);
                }
                // Typing in the password field
                if (inputField == 1)
                {
                    ft_strncat(newUser.password, event.text.text, sizeof(newUser.password) - ft_strlen(newUser.password) - 1);
                    hidingPassword[ft_strlen(hidingPassword)] = '*';
                    // strncat(hidingPassword, "•", sizeof(hidingPassword) - strlen(hidingPassword) - 1);
                    hidingPassword[ft_strlen(hidingPassword) + 1] = '\0';
                }
            }

            // Mouse click events
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Check if user clicked on username input field
                if (isMouseOverButton(&userNameInputButton, mouseX, mouseY))
                {
                    inputField = 0; // Focus on username input field
                    putStr("Username Input\n");
                }

                // Check if user clicked on password input field
                if (isMouseOverButton(&passwordInputButton, mouseX, mouseY))
                {
                    inputField = 1; // Focus on password input field
                    putStr("Password Input\n");
                }

                // Check if user clicked on the register button
                if (isMouseOverButton(&_registerButton, mouseX, mouseY))
                {
                    emptyUsername = 0;
                    emptyPassword = 0;
                    userNameExists = 0;

                    // Set colors
                    activeButtonColor.r = 28;
                    activeButtonColor.g = 54;
                    activeButtonColor.b = 97;
                    activeButtonColor.a = 255;

                    inactiveButtonColor.r = 197;
                    inactiveButtonColor.g = 201;
                    inactiveButtonColor.b = 212;
                    inactiveButtonColor.a = 255;

                    if (newUser.username[0] == '\0') // If username is empty
                    {
                        emptyUsername = 1;
                        putStr("Username field is empty\n");
                    }
                    if (newUser.password[0] == '\0') // If password is empty
                    {
                        emptyPassword = 1;
                        putStr("Password field is empty\n");
                    }
                    if (checkUserExists(newUser.username, FILE_NAME) && emptyUsername == 0) // Check if username already exists
                    {
                        userNameExists = 1;
                        putStr("Username already exists\n");
                        ft_memset(newUser.username, '\0', sizeof(newUser.username));
                        ft_memset(newUser.password, '\0', sizeof(newUser.password));
                        ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                    }
                    if (!checkUserExists(newUser.username, FILE_NAME) && emptyUsername == 0 && emptyPassword == 0 && userNameExists == 0)
                    {
                        userNameExists = 0;
                        emptyUsername = 0;
                        emptyPassword = 0;
                        storeUserData(newUser, FILE_NAME);
                        putStr("Registration successful!\n");
                        ft_memset(newUser.username, '\0', sizeof(newUser.username));
                        ft_memset(newUser.password, '\0', sizeof(newUser.password));
                        ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                    }
                }

                // Toggle password visibility on eye button click
                if (isMouseOverButton(&_eyeButton, mouseX, mouseY))
                {
                    hidePassword = !hidePassword; // Toggle between 0 and 1
                    putStr(hidePassword ? "Password hiding Enabled\n" : "Password hiding Disabled\n");
                }

                // Login button click (can be implemented separately)
                if (isMouseOverButton(&_loginButton, mouseX, mouseY))
                {
                    return (1);
                }
            }

            // Handle keyboard navigation
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_DOWN)
                    inputField = !inputField; // Switch to password field => inputField = (inputField == 0) ? 1 : 0;
                if (event.key.keysym.sym == SDLK_UP)
                    inputField = !inputField;            // Switch to username field
                if (event.key.keysym.sym == SDLK_RETURN) // Move to next field on Enter key
                {
                    if (inputField == 1)
                    {
                        emptyUsername = 0;
                        emptyPassword = 0;
                        userNameExists = 0;

                        if (newUser.username[0] == '\0') // If username is empty
                        {
                            emptyUsername = 1;
                            putStr("Username field is empty !!\n");
                        }
                        if (newUser.password[0] == '\0') // If password is empty
                        {
                            emptyPassword = 1;
                            putStr("Password field is empty !!\n");
                        }
                        if (checkUserExists(newUser.username, FILE_NAME) && emptyUsername == 0) // Check if username already exists
                        {
                            inputField = 0;
                            userNameExists = 1;
                            putStr("Username already exists\n");
                            ft_memset(newUser.username, '\0', sizeof(newUser.username));
                            ft_memset(newUser.password, '\0', sizeof(newUser.password));
                            ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                        }
                        if (!checkUserExists(newUser.username, FILE_NAME) && emptyUsername == 0 && emptyPassword == 0 && userNameExists == 0)
                        {
                            inputField = 0;
                            userNameExists = 0;
                            emptyUsername = 0;
                            emptyPassword = 0;
                            storeUserData(newUser, FILE_NAME);
                            putStr("Registration successful!\n");
                            ft_memset(newUser.username, '\0', sizeof(newUser.username));
                            ft_memset(newUser.password, '\0', sizeof(newUser.password));
                            ft_memset(hidingPassword, '\0', sizeof(hidingPassword));
                        }
                    }
                    else
                        inputField = 1;
                }

                // If the user is typing in the username field
                if (inputField == 0)
                {
                    // Dealing with deletion (Backspace):
                    if (event.key.keysym.sym == SDLK_BACKSPACE && ft_strlen(newUser.username) > 0)
                    {
                        newUser.username[ft_strlen(newUser.username) - 1] = '\0'; // Remove last character
                        // tempCursorPos[ft_strlen(username) - 1] = '\0'; // Remove last character
                    }
                }

                // If the user is typing in the password field
                if (inputField == 1)
                {
                    // Dealing with deletion (Backspace):
                    if (event.key.keysym.sym == SDLK_BACKSPACE && ft_strlen(newUser.password) > 0)
                    {
                        newUser.password[ft_strlen(newUser.password) - 1] = '\0'; // Remove last character
                        hidingPassword[ft_strlen(hidingPassword) - 1] = '\0';     // Remove asterisk
                    }
                }
                // if (event.key.keysym.sym == SDLK_LEFT)
                // {
                //     if (inputField == 0 && cursorPosition > 0)
                //         cursorPosition--; // Move cursor to the left
                // }
                // else if (event.key.keysym.sym == SDLK_RIGHT)
                // {
                //     if (inputField == 0 && cursorPosition < (int)ft_strlen(username))
                //         cursorPosition++; // Move cursor to the right
                // }
            }
        }

        // Clear screen and set background color
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);

        // Render page title
        TTF_SizeText(titleFont, "IYORE", &textWidth, &textHeight);
        renderImg(renderer, IRiconImgSF, (WINDOW_WIDTH - 93 - textWidth) / 2, 5, 100, 100);
        renderText(renderer, titleFont, titleColor, "IYORE", (WINDOW_WIDTH - textWidth + 93) / 2, 20);

        // Render "Register" heading
        renderText(renderer, topicsFont, topicsColor, "Register", 40, textHeight + 30);

        /*
            Render username input section
        */
        TTF_SizeText(textfont, "Enter username: ", &textWidth, &textHeight);
        renderText(renderer, textfont, bgTextColor, "Enter username: ", 50, 160);
        renderText(renderer, textfont, textColor, newUser.username, textWidth + 50, 160);

        // Dealing with cursor:
        // ft_strncpy(tempCursorPos, username, cursorPosition);
        TTF_SizeText(textfont, newUser.username, &textWidth, &textHeight);
        if (inputField == 0)
        {
            int cursorX = textWidth + 203;
            if (SDL_GetTicks() % 1000 < 500) // Make cursor blink
            {
                SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
                SDL_RenderDrawLine(renderer, cursorX, 163, cursorX, 183);
            }
        }

        SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
        for (int i = 0; i < 2; i++)
            SDL_RenderDrawLine(renderer, 50, 190 + i, WINDOW_WIDTH - 50, 190 + i);

        // Show error messages if needed
        if (emptyUsername)
            renderText(renderer, smallfont, redColor, "Username field is empty !!", 50, 195);
        if (userNameExists == 1)
            renderText(renderer, smallfont, redColor, "Username already exists !!", 50, 195);

        /*
            Render password input section
        */
        TTF_SizeText(textfont, "Enter password: ", &textWidth, &textHeight);
        renderText(renderer, textfont, bgTextColor, "Enter password: ", 50, 240);
        renderImg(renderer, hidePassword ? eyeSlashImgSF : eyeImgSF, WINDOW_WIDTH - 70, 245, 20, 20);
        renderText(renderer, hidePassword ? hidingPasswordFont : textfont, textColor, hidePassword ? hidingPassword : newUser.password, textWidth + 55, 240);

        // Dealing with cursor:
        // ft_strncpy(tempCursorPos, username, cursorPosition);
        TTF_SizeText(textfont, newUser.password, &textWidth, &textHeight);
        if (inputField == 1)
        {
            int cursorX = textWidth + 203;
            if (SDL_GetTicks() % 1000 < 500) // Make cursor blink
            {
                SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
                SDL_RenderDrawLine(renderer, cursorX, 243, cursorX, 263);
            }
        }

        SDL_SetRenderDrawColor(renderer, bgTextColor.r, bgTextColor.g, bgTextColor.b, bgTextColor.a);
        for (int i = 0; i < 2; i++)
            SDL_RenderDrawLine(renderer, 50, 270 + i, WINDOW_WIDTH - 50, 270 + i);

        // Show error messages if needed
        if (emptyPassword)
            renderText(renderer, smallfont, redColor, "Password field is empty !!", 50, 275);

        TTF_SizeText(textfont, "Registration successful !!", &textWidth, &textHeight);
        if (emptyUsername == 0 && emptyPassword == 0 && userNameExists == 0)
            renderText(renderer, textfont, greenColor, "Registration successful !!", (WINDOW_WIDTH / 2) - (textWidth / 2), 290);

        // Render register button
        TTF_SizeText(topicsFont, "Register", &textWidth, &textHeight);
        SDL_SetRenderDrawColor(renderer, activeButtonColor.r, activeButtonColor.g, activeButtonColor.b, 255);
        SDL_RenderFillRect(renderer, &_registerButton);
        renderText(renderer, topicsFont, textColor, "Register", _registerButton.x + (_registerButton.w - textWidth) / 2, _registerButton.y + (_registerButton.h - textHeight) / 2);

        // Render login button
        TTF_SizeText(topicsFont, "Login", &textWidth, &textHeight);
        SDL_SetRenderDrawColor(renderer, inactiveButtonColor.r, inactiveButtonColor.g, inactiveButtonColor.b, 255);
        SDL_RenderFillRect(renderer, &_loginButton);
        renderText(renderer, topicsFont, textColor, "Login", _loginButton.x + (_loginButton.w - textWidth) / 2, _loginButton.y + (_loginButton.h - textHeight) / 2);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Print entered username and password (for debugging purposes)
    putStr(newUser.username);
    putChar('\n');
    putStr(newUser.password);
    putChar('\n');
    return (0);
}

/*
    Menu function: initializes the application and calls the Register function
 */
void menu(void)
{
    fullClear();  // Clear the screen
    printIYORE(); // Print the app's logo

// Call the registration process
REGISTER:
    if (Register() == 1)
    {
        if (Login() == 1)
        {
            goto REGISTER;
        }
    }
}