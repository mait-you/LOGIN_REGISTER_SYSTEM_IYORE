#include "lib/iyore.h"
#include "lib/menu.h"

void printIYORE(void)
{
    putStr("   ______  ______  ___  ____\n");
    putStr("  /  _/\\ \\/ / __ \\/ _ \\/ __/\n");
    putStr(" _/ /   \\  / /_/ / , _/ _/\n");
    putStr("/___/   /_/\\____/_/|_/___/\n\n");
}

void printRegister(void)
{
    putStr("   ___           _     __\n");
    putStr("  / _ \\___ ___ _(_)__ / /____ ____\n");
    putStr(" / , _/ -_) _ `/ (_-</ __/ -_) __/\n");
    putStr("/_/|_|\\__/\\_, /_/___/\\__/\\__/_/\n");
    putStr("         /___/\n\n");
}

void printLogin(void)
{
    putStr("   __             _\n");
    putStr("  / /  ___  ___ _(_)__\n");
    putStr(" / /__/ _ \\/ _ `/ / _ \\\n");
    putStr("/____/\\___/\\_, /_/_//_/\n");
    putStr("          /___/\n\n");
}

void fullClear(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void lClear(int l)
{
    int i;

    i = 0;
    while (i < l)
    {
        putStr("\033[2K"); // مسح السطر
        if (i < l - 1)
            putStr("\033[A"); // الانتقال للاعلى
        i++;
    }
    putStr("\r"); // الرجوع لبداية السطر
}

void putChar(char c)
{
    write(1, &c, 1);
}

void putStr(const char *s)
{
    while (*s)
        write(1, s++, 1);
}

void putNbr(int nb)
{
    if (nb < 0)
    {
        putChar('-');
        if (nb == -2147483648)
            write(1, "2147483648", 10);
        else
        {
            nb = -nb;
            putNbr(nb);
        }
    }
    else if (nb > 9)
    {
        putNbr(nb / 10);
        putNbr(nb % 10);
    }
    else
        putChar(nb + '0');
}

char getChar(void)
{
    tcflush(0, 0);
    char c;
    read(0, &c, 1);
    return (c);
}

void getStr(char *s, size_t maxLen)
{
    tcflush(0, 0);

    size_t i = 0;

    while (i < maxLen - 1)
    {
        read(0, &s[i], 1);
        if (s[i] == '\n')
            break;
        i++;
    }
    s[i] = '\0';
}

int generateUniqueId(void)
{
    static int lastId = -1;
    int newId;

    do
    {
        newId = rand() % 9000 + 1000;
    } while (newId == lastId);

    lastId = newId;
    return newId;
}

// void putRegistingInfo(char *userName, char *password, int userId, int userNumbers, int usersFile)
// {
//     fPutNbr(userNumbers, usersFile);
//     fPutStr(":userId: ", usersFile);
//     fPutNbr(userId, usersFile);
//     fPutStr(" {\n", usersFile);
//     fPutStr("   username: ", usersFile);
//     fPutStr("\"", usersFile);
//     fPutStr(userName, usersFile);
//     fPutStr("\";\n", usersFile);
//     fPutStr("   password: ", usersFile);
//     fPutStr("\"", usersFile);
//     fPutStr(password, usersFile);
//     fPutStr("\";\n", usersFile);
//     fPutStr("};\n", usersFile);
// }

void storeUserData(User newUser, char *fileName)
{
    char *fileContent = NULL;
    long fileSize = 0;
    struct stat fileStat;

    // Open the users file to creat/append/read user data
    int file = open(fileName, O_APPEND | O_RDWR | O_CREAT, 0644);
    if (file < 0)
    {
        putStr("Error reading users file\n");
        return;
    }

    // Check the status of the file
    if (fstat(file, &fileStat) == 0)
    {
        // If the file is not empty, read its content
        if (fileStat.st_size > 0)
        {
            fileSize = fileStat.st_size;
            fileContent = malloc((fileSize + 1) * sizeof(char));
            if (!fileContent)
            {
                putStr("in ft putRegistingInfo: Error allocating fileContent memory\n");
                close(file);
                return;
            }
            ssize_t bytesRead = read(file, fileContent, fileSize);
            if (bytesRead < 0)
            {
                putStr("in ft putRegistingInfo: Error reading file\n");
                free(fileContent);
                close(file);
                return;
            }
            fileContent[fileSize] = '\0'; // Add null-terminator
        }
        else
            fileContent = ft_strdup("{}"); // If the file is empty, start with an empty JSON
    }
    else
        fileContent = ft_strdup("{}"); // If the file doesn't exist, start with an empty JSON

    // Parse the JSON content
    cJSON *json = cJSON_Parse(fileContent);
    if (!json)
    {
        putStr("Error parsing JSON: ");
        putStr(cJSON_GetErrorPtr());
        free(fileContent);
        close(file);
        return;
    }

    // Handle the users array in JSON
    cJSON *usersArray = cJSON_GetObjectItem(json, "users");
    if (!usersArray)
    {
        usersArray = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "users", usersArray);
    }

    // Create a new JSON object for the user
    cJSON *newUserJson = cJSON_CreateObject();
    cJSON_AddNumberToObject(newUserJson, "userId", newUser.userId);
    cJSON_AddStringToObject(newUserJson, "username", newUser.username);
    cJSON_AddStringToObject(newUserJson, "password", newUser.password);

    // Add the new user to the array
    cJSON_AddItemToArray(usersArray, newUserJson);

    // Convert the JSON object to text and write it back to the file
    char *updatedJson = cJSON_Print(json);
    if (updatedJson)
    {
        // Resize the file to zero before writing the new content
        ftruncate(file, 0);                            // Clear the file to 0 bytes
        lseek(file, 0, SEEK_SET);                      // Reset the file pointer to the beginning
        write(file, updatedJson, strlen(updatedJson)); // Write the updated JSON to the file
    }
    else
    {
        putStr("Error printing JSON\n");
    }

    // Clean up memory
    cJSON_Delete(json);
    free(updatedJson);
    free(fileContent);

    // Close the file
    close(file);
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, int x, int y)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL)
        return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        SDL_FreeSurface(surface);
        return;
    }

    int textWidth = surface->w;
    int textHeight = surface->h;
    SDL_Rect dstRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderImg(SDL_Renderer *renderer, SDL_Surface *img, int x, int y, int width, int height)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    if (texture == NULL)
    {
        SDL_FreeSurface(img);
        return;
    }

    SDL_Rect dstRect = {x, y, width, height};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

int isMouseOverButton(SDL_Rect *button, int mouseX, int mouseY)
{
    return (mouseX > button->x && mouseX < button->x + button->w && mouseY > button->y && mouseY < button->y + button->h);
}

int checkUserExists(const char *username, const char *fileName)
{
    char *fileContent = NULL;
    long fileSize = 0;
    struct stat fileStat;

    // Open the file
    int file = open(fileName, O_RDONLY);
    if (file < 0)
    {
        putStr("Error reading users file\n");
        return (0); // Return 0 if the file can't be opened
    }

    // Get file size
    if (fstat(file, &fileStat) == 0 && fileStat.st_size > 0)
    {
        fileSize = fileStat.st_size;
        fileContent = malloc((fileSize + 1) * sizeof(char));
        if (!fileContent)
        {
            putStr("Error allocating memory\n");
            close(file);
            return (0); // Return 0 if there's an error
        }

        ssize_t bytesRead = read(file, fileContent, fileSize);
        if (bytesRead < 0)
        {
            putStr("Error reading file\n");
            free(fileContent);
            close(file);
            return (0);
        }
        fileContent[fileSize] = '\0'; // Null-terminate the file content
    }
    else
    {
        free(fileContent);
        close(file);
        return (0); // Return 0 if the file is empty or has issues
    }

    // Parse JSON content
    cJSON *json = cJSON_Parse(fileContent);
    if (!json)
    {
        putStr("Error parsing JSON\n");
        free(fileContent);
        close(file);
        return (0);
    }

    // Get the users array
    cJSON *usersArray = cJSON_GetObjectItem(json, "users");
    if (!usersArray || !cJSON_IsArray(usersArray))
    {
        putStr("No users found\n");
        cJSON_Delete(json);
        free(fileContent);
        close(file);
        return (0);
    }

    // Iterate through the users array
    cJSON *user = NULL;
    cJSON_ArrayForEach(user, usersArray)
    {
        cJSON *userNameJson = cJSON_GetObjectItem(user, "username");
        if (cJSON_IsString(userNameJson) && strcmp(userNameJson->valuestring, username) == 0)
        {
            // If the username matches, user is found
            cJSON_Delete(json);
            free(fileContent);
            close(file);
            return (1); // Return 1 if user is found
        }
    }
    // int userNumbers = countUsers(usersFile) + 1; // Get number of users

    // User not found
    cJSON_Delete(json);
    free(fileContent);
    close(file);
    return (0); // Return 0 if user is not found
}

int isUserRegistered(const char *username, const char *password, const char *fileName)
{
    char *fileContent = NULL;
    long fileSize = 0;
    int returnValue = 0;
    struct stat fileStat;

    // Open the file
    int file = open(fileName, O_RDONLY);
    if (file < 0)
    {
        putStr("Error reading users file\n");
        return (0); // Return 0 if the file can't be opened
    }

    // Get file size
    if (fstat(file, &fileStat) == 0 && fileStat.st_size > 0)
    {
        fileSize = fileStat.st_size;
        fileContent = malloc((fileSize + 1) * sizeof(char));
        if (!fileContent)
        {
            putStr("Error allocating memory\n");
            close(file);
            return (0); // Return 0 if there's an error
        }

        ssize_t bytesRead = read(file, fileContent, fileSize);
        if (bytesRead < 0)
        {
            putStr("Error reading file\n");
            free(fileContent);
            close(file);
            return (0);
        }
        fileContent[fileSize] = '\0'; // Null-terminate the file content
    }
    else
    {
        free(fileContent);
        close(file);
        return (0); // Return 0 if the file is empty or has issues
    }

    // Parse JSON content
    cJSON *json = cJSON_Parse(fileContent);
    if (!json)
    {
        putStr("Error parsing JSON\n");
        free(fileContent);
        close(file);
        return (0);
    }

    // Get the users array
    cJSON *usersArray = cJSON_GetObjectItem(json, "users");
    if (!usersArray || !cJSON_IsArray(usersArray))
    {
        putStr("No users found\n");
        cJSON_Delete(json);
        free(fileContent);
        close(file);
        return (0);
    }

    // Iterate through the users array
    cJSON *user = NULL;
    cJSON_ArrayForEach(user, usersArray)
    {
        cJSON *userNameJson = cJSON_GetObjectItem(user, "username");
        if (cJSON_IsString(userNameJson) && ft_strcmp(userNameJson->valuestring, username) == 0)
        {
            returnValue += 10;
            cJSON *passWordJson = cJSON_GetObjectItem(user, "password");
            if (cJSON_IsString(passWordJson) && ft_strcmp(passWordJson->valuestring, password) == 0)
            {
                // If the username & password matches, user is found
                returnValue += 10;
                cJSON_Delete(json);
                free(fileContent);
                close(file);
                return (returnValue); // Return 20 if user & password is found
            }
            return (returnValue); // Return 10 if user is found
        }
    }

    // User & password not found
    cJSON_Delete(json);
    free(fileContent);
    close(file);
    return (0); // Return 0 if user & password is not found
}

char *ft_strncat(char *dest, const char *src, size_t n)
{
    char *d = dest;

    while (*d)
        d++;
    while (n-- && *src)
        *d++ = *src++;

    *d = '\0';

    return (dest);
}

char *ft_strstr(char *str, char *to_find)
{
    int i;
    int j;

    if (to_find[0] == '\0')
        return (str);

    i = 0;
    while (str[i] != '\0')
    {
        j = 0;
        while (str[i + j] == to_find[j])
        {
            if (to_find[j + 1] == '\0')
                return (&str[i]);
            j++;
        }
        i++;
    }
    return (NULL);
}

char *ft_strchr(char *str, char c)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
            return (&str[i]);
        i++;
    }

    if (c == '\0')
        return &str[i];

    return (NULL);
}

size_t ft_strlen(const char *s)
{
    size_t i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

void ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
}

void *ft_memset(void *b, int c, size_t len)
{
    size_t i;
    unsigned char *ptr;

    ptr = (unsigned char *)b;
    i = 0;
    while (i < len)
    {
        ptr[i] = (unsigned char)c;
        i++;
    }
    return (b);
}

char *ft_strdup(const char *s)
{
    size_t len = ft_strlen(s);
    char *dup = malloc(len + 1);

    if (dup == NULL)
        return (NULL);

    size_t i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[len] = '\0';

    return (dup);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
