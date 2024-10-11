#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_LINES 1000
#define MAX_KEYWORDS 32
#define SCREEN_WIDTH 120
#define LEFT_COLUMN_WIDTH 60

// Global variables
int totalCorrect = 0;
int totalTyped = 0;
clock_t startTime, endTime;
char exerciseData[MAX_LINES][MAX_LINE_LENGTH];
char userInput[MAX_LINES][MAX_LINE_LENGTH];
int totalLines = 0;
int currentLine = 0;

const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// Function declarations
void showMainMenu();
void loadExerciseData(const char* filename);
void displayExercise();
void getUserInput();
void compareInput();
void playSound(int correct);
void practiceKeywords();
void practiceAlgorithms(int level);
void displayScore();
void displayRealTimeStats();
void displayEntireFunction();
int compareIgnoreExtraSpaces(const char* str1, const char* str2);

int main() {
    // Set console to full screen
    system("mode con cols=200 lines=50");
    srand((unsigned int)time(NULL));

    showMainMenu();

    return 0;
}

void showMainMenu() {
    int choice;
    do {
        system("cls");
        printf("Welcome to C Language Typing Practice\n");
        printf("1. Keyword Practice\n");
        printf("2. Beginner Algorithm Practice\n");
        printf("3. Intermediate Algorithm Practice\n");
        printf("4. Advanced Algorithm Practice\n");
        printf("0. Exit\n");
        printf("Please select a practice level: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch(choice) {
            case 1:
                totalCorrect = 0;
                totalTyped = 0;
                practiceKeywords();
                break;
            case 2:
                totalCorrect = 0;
                totalTyped = 0;
                practiceAlgorithms(1);
                break;
            case 3:
                totalCorrect = 0;
                totalTyped = 0;
                practiceAlgorithms(2);
                break;
            case 4:
                totalCorrect = 0;
                totalTyped = 0;
                practiceAlgorithms(3);
                break;
            case 0:
                printf("Thank you for using. Goodbye!\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 0);
}

void practiceKeywords() {
    system("cls");
    printf("Keyword practice started. Press ESC to return to main menu.\n\n");
    startTime = clock(); // Start timing

    while (1) {
        int index = rand() % MAX_KEYWORDS;
        printf("Please type the following keyword:\n");
        printf("%s\n", keywords[index]);

        char input[MAX_LINE_LENGTH];
        // Display timer
        DWORD startTick = GetTickCount();
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD tick;
        int elapsed = 0;

        // Clear standard input buffer
        fflush(stdin);

        // Get user input while updating timer
        int i = 0;
        while (1) {
            if (_kbhit()) {
                char c = _getch();
                if (c == 27) { // ESC key
                    endTime = clock();
                    displayScore();
                    return;
                }
                if (c == '\r') { // Enter key
                    input[i] = '\0';
                    break;
                }
                if (c == '\b') { // Backspace key
                    if (i > 0) {
                        i--;
                        printf("\b \b");
                    }
                } else {
                    input[i++] = c;
                    printf("%c", c);
                }
            }
            tick = GetTickCount();
            elapsed = (tick - startTick) / 1000;
            // Display time in title bar
            char title[100];
            sprintf(title, "Typing Practice - Time elapsed: %d seconds", elapsed);
            SetConsoleTitle(title);
        }

        printf("\n");

        totalTyped++;
        if (strcmp(input, keywords[index]) == 0) {
            totalCorrect++;
            playSound(1);
            printf("Correct!\n");
        } else {
            playSound(0);
            printf("Incorrect! The correct keyword is: %s\n", keywords[index]);
        }

        // Display real-time statistics after each input
        displayRealTimeStats();
    }
}

// Improved function to compare strings ignoring extra spaces
int compareIgnoreExtraSpaces(const char* str1, const char* str2) {
    while (*str1 || *str2) {
        // Skip all spaces
        while (isspace(*str1)) str1++;
        while (isspace(*str2)) str2++;

        // If we're at a special character that might have spaces around it
        if (*str1 && strchr("=<>!+-*/&|", *str1)) {
            if (*str1 != *str2) return *str1 - *str2;
            str1++;
            str2++;
            // Skip spaces after the special character
            while (isspace(*str1)) str1++;
            while (isspace(*str2)) str2++;
        } else if (*str1 != *str2) {
            return *str1 - *str2;
        } else if (*str1) {
            str1++;
            str2++;
        }
    }
    return 0;
}

void practiceAlgorithms(int level) {
    char filename[20];
    sprintf(filename, "level%d.txt", level);
    loadExerciseData(filename);
    
    while (currentLine < totalLines) {
        system("cls");
        
        int functionStart = currentLine;
        int functionLines = 0;
        // Count the number of lines in the current function
        while (currentLine + functionLines < totalLines && exerciseData[currentLine + functionLines][0] != '\0') {
            functionLines++;
        }
        
        clock_t functionStartTime = clock();
        
        // Display the entire function on the left side
        printf("Original Code:%-*sYour Input:\n", LEFT_COLUMN_WIDTH - 14, "");
        printf("--------------------%-*s--------------------\n", LEFT_COLUMN_WIDTH - 20, "");
        for (int i = 0; i < functionLines; i++) {
            printf("%-*.*s|\n", LEFT_COLUMN_WIDTH, LEFT_COLUMN_WIDTH, exerciseData[currentLine + i]);
        }
        
        // Move cursor to the start of input area
        printf("\033[3;%dH", LEFT_COLUMN_WIDTH + 2);
        
        int correctLines = 0;
        for (int i = 0; i < functionLines; i++) {
            char userLine[MAX_LINE_LENGTH];
            fgets(userLine, MAX_LINE_LENGTH, stdin);
            userLine[strcspn(userLine, "\n")] = 0;  // Remove newline
            
            // Move cursor to the end of the user input
            printf("\033[%d;%dH", 3 + i, LEFT_COLUMN_WIDTH + 2 + strlen(userLine));
            
            if (compareIgnoreExtraSpaces(exerciseData[currentLine + i], userLine) == 0) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
                printf(" Correct!");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                correctLines++;
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                printf(" Incorrect");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            
            // Move cursor to the next line for input
            if (i < functionLines - 1) {
                printf("\033[%d;%dH", 4 + i, LEFT_COLUMN_WIDTH + 2);
            }
        }
        
        clock_t functionEndTime = clock();
        double functionTime = ((double)(functionEndTime - functionStartTime)) / CLOCKS_PER_SEC;
        
        // Display results
        printf("\033[%d;0H\n", 3 + functionLines);
        printf("Function completed!\n");
        printf("Time taken: %.2f seconds\n", functionTime);
        printf("Correct lines: %d out of %d\n", correctLines, functionLines);
        printf("Accuracy: %.2f%%\n", (double)correctLines / functionLines * 100);
        
        printf("\nPress any key to continue to the next function...");
        getch();
        
        currentLine += functionLines + 1; // Move to the next function, skipping the empty line
    }
    
    printf("\nPractice for this level completed! Press any key to return to main menu...\n");
    getch();
    currentLine = 0;
    totalLines = 0;
}

// Load exercise data
void loadExerciseData(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }

    while (fgets(exerciseData[totalLines], MAX_LINE_LENGTH, file) != NULL) {
        exerciseData[totalLines][strcspn(exerciseData[totalLines], "\n")] = 0;
        totalLines++;
        if (totalLines >= MAX_LINES) break;
    }

    fclose(file);
}

// Display current exercise line
void displayExercise() {
    printf("Please type the following code (press Enter to confirm):\n");
    printf("%s\n", exerciseData[currentLine]);
}

// Get user input
void getUserInput() {
    fgets(userInput[currentLine], MAX_LINE_LENGTH, stdin);
    userInput[currentLine][strcspn(userInput[currentLine], "\n")] = 0;
}

// Compare user input with original data
void compareInput() {
    int correct = strcmp(exerciseData[currentLine], userInput[currentLine]) == 0;
    playSound(correct);

    if (!correct) {
        printf("Incorrect! The correct input is:\n");
        for (int i = 0; exerciseData[currentLine][i] != '\0'; i++) {
            if (exerciseData[currentLine][i] != userInput[currentLine][i]) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                printf("%c", exerciseData[currentLine][i]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                printf("%c", exerciseData[currentLine][i]);
            }
        }
        printf("\n");
        printf("Press any key to continue...");
        getch();
    }
}

// Play sound
void playSound(int correct) {
    if (correct) {
        Beep(1000, 100);  // Correct sound
    } else {
        Beep(500, 300);   // Incorrect sound
    }
}

// Display score
void displayScore() {
    double timeTaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    double accuracy = (totalTyped == 0) ? 0 : ((double)totalCorrect / totalTyped) * 100;
    int totalErrors = totalTyped - totalCorrect;
    double speed = (timeTaken > 0) ? totalTyped / timeTaken : 0;

    system("cls");
    printf("Practice Results:\n");
    printf("Total inputs: %d\n", totalTyped);
    printf("Correct inputs: %d\n", totalCorrect);
    printf("Errors: %d\n", totalErrors);
    printf("Accuracy: %.2f%%\n", accuracy);
    printf("Typing speed: %.2f inputs/second\n", speed);
    printf("Total time: %.2f seconds\n", timeTaken);
    printf("Press any key to return to main menu...");
    _getch();
}

// New function to display real-time statistics in one line
void displayRealTimeStats() {
    double timeTaken = ((double)(clock() - startTime)) / CLOCKS_PER_SEC;
    double accuracy = (totalTyped == 0) ? 0 : ((double)totalCorrect / totalTyped) * 100;
    int totalErrors = totalTyped - totalCorrect;
    double speed = (timeTaken > 0) ? (totalTyped / timeTaken) * 60 : 0; // Changed to inputs per minute

    printf("\nStats: Total: %d | Correct: %d | Errors: %d | Accuracy: %.2f%% | Speed: %.2f char/min | Time: %.2f s\n\n",
           totalTyped, totalCorrect, totalErrors, accuracy, speed, timeTaken);
}

void displayEntireFunction() {
    int i = currentLine;
    printf("Type the following function:\n\n");
    while (i < totalLines && exerciseData[i][0] != '\0') {
        printf("%s\n", exerciseData[i]);
        i++;
    }
    printf("\n");
}