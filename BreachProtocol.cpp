#include <iostream>
#include <ctime>
#include <vector>

class RowsAndCols
{
    private:
        int currRow, currCol;
    public:
        int getCurrRow()
        {
            return currRow;
        }
        void setCurrRow(int currRow)
        {
            this->currRow = currRow;
        }
        int getCurrCol()
        {
            return currCol;
        }
        void setCurrCol(int currCol)
        {
            this->currCol = currCol;
        }
};

void generateArray(std::string array[5][5], std::string possibleChoices[5]);
void displayArray(std::string array[5][5]);
void playGame(std::string array[5][5], std::string buffer[4], int bufferIDX, std::string solution[4]);
void displayBuffer(std::string buffer[4]);
void generateSolution(std::string solution[4], std::string possibleChoices[5]);
void displaySolution(std::string solution[4]);
void generateSolution(std::string array[5][5], std::string solution[4]);

int main()
{
    int bufferTries = 0, bufferIDX = 0, option = 0;

    // Minigame Array and array possible entries.
    std::string array[5][5];
    std::string possibleChoices[5] = {"BD", "1C", "7A", "55", "E9"};
    
    // User's chosen array entries and the provided solution to get.
    std::string buffer[4];
    int bufferSize = sizeof(buffer) / sizeof(buffer[0]);
    std::string solution[4];

    // Instructions.
    std::cout << "*********************************************************************\n";   
    std::cout << "************************** Breach Protocol **************************\n";
    std::cout << "*********************************************************************\n";
    std::cout << "**************************** How To Play ****************************\n";
    std::cout << "1. Choose an option from the top row to begin.\n";
    std::cout << "2. Your choices will now begin to switch between the current column and the current row.\n";
    std::cout << "2.1. Your choices will be 1 - 5, with 1 being the top or left and 5 being the bottom or right of the current row/col\n.";
    std::cout << "3. Try to choose a path through the array such that the buffer (your choices) matches the generated solution.\n";
    std::cout << "*********************************************************************\n";
    std::cout << "Press 1 if you would like to play, or 0 to exit.\n";
    std::cin >> option;
   
    while (option != 0 && option != 1)
    {
        std::cout << "Please enter a valid option.\n";
        std::cin >> option;
    }

    if (option == 0)
    {
        std::cout << "Hacking incomplete...\n";
        return 0;
    }
    
    generateArray(array, possibleChoices);
    displayArray(array);
    
    generateSolution(array, solution);
    displaySolution(solution);

    std::cout << "Please choose an entry on the top row to begin (1 - 5).\n";
    playGame(array, buffer, bufferIDX, solution);

    if (std::equal(buffer, buffer + 4, solution))
    {
        std::cout << "Hacking complete!\n";
    }
    else
    {
        std::cout << "Hacking failed...\n";
    }

    return 0;
}

void generateArray(std::string array[5][5], std::string possibleChoices[5])
{
    // Randomly generates the string array for the player to look through.
    srand(time(0));
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int randNum = rand() % 5;
            switch(randNum)
            {
                case 0:
                    array[i][j] = possibleChoices[0];
                    break;
                case 1:
                    array[i][j] = possibleChoices[1];
                    break;
                case 2:
                    array[i][j] = possibleChoices[2];
                    break;
                case 3:
                    array[i][j] = possibleChoices[3];
                    break;
                case 4:
                    array[i][j] = possibleChoices[4];
                    break;
            }
        }
    }
}

void displayArray(std::string array[5][5])
{
    // Displays the array to the user.
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            std::cout << array[i][j] << " ";
        }
        std::cout << '\n';
    }
}

void displayBuffer(std::string buffer[4])
{
    // Displays the player's current buffer (attempt at solution).
    std::cout << "Current buffer: [";
    for (int i = 0; i < 4; i++)
    {
        if (i < 3)
        {
            std::cout << buffer[i] << ", ";
        }
        else 
        {
            std::cout << buffer[i];
        }
    }
    std::cout << "]\n";
}

void playGame(std::string array[5][5], std::string buffer[4], int bufferIDX, std::string solution[4])
{
    int bufferTries = 0;
    RowsAndCols rc1;

    // Amount of turns (equal to length of player buffer).
    while (bufferTries != 4)
    {
        if (bufferTries != 0)
        {
            switch(bufferTries % 2)
            {
                case 0:
                    std::cout << "Select an element from the current row (" << rc1.getCurrRow() + 1 << ").\n";
                    break;
                case 1:
                    std::cout << "Select a element from the current column (" << rc1.getCurrCol() + 1 << ").\n";
                    break;
            }
        }

        int choice;
        std::cin >> choice;
        while (choice > 5 || choice < 1)
        {
            std::cout << "Please enter a valid integer between 1 and 5.\n";
            std::cin >> choice;
        }

        /*
            If the amount of tries is 0, the player chooses an option from the top row, otherwise it cycles between even and odd amount of tries
            to determine whether the row or column is what gets set between turns.
        */ 
        switch(bufferTries % 2)
        {
            case 0:
                if (bufferTries)
                {
                    rc1.setCurrRow(0);
                    rc1.setCurrCol(choice - 1);
                }
                else
                {
                    rc1.setCurrCol(choice - 1);
                }
            case 1:
                rc1.setCurrRow(choice - 1);
        }    
        
        // If the option the player chooses is occupied, stay in loop until they choose an option not occupied.
        while (array[rc1.getCurrRow()][rc1.getCurrCol()] == " X")
        {
            std::cout << "That location is currently occupied. Please enter another integer.\n";
            std::cin >> choice;
            switch(bufferTries % 2)
            {
                case 0:
                    if (bufferTries)
                    {
                        rc1.setCurrRow(0);
                        rc1.setCurrCol(choice - 1);
                    }
                    else
                    {
                        rc1.setCurrCol(choice - 1);
                    }
                case 1:
                    rc1.setCurrRow(choice - 1);
            }   
        }

        // Set current choice as occupied and add it to the player buffer.
        buffer[bufferIDX] = array[rc1.getCurrRow()][rc1.getCurrCol()];
        bufferTries++;
        bufferIDX++;
        array[rc1.getCurrRow()][rc1.getCurrCol()] = " X";
        displayArray(array);
        displaySolution(solution);
        displayBuffer(buffer);
    }
}

void generateSolution(std::string array[5][5], std::string solution[4])
{
    /*
        Similar to playing the game, it is possible to create a solution to the array 
        by having a computer randomly generate a path through the array.
    */
    int solutionTries = 0, choice = 0, solutionIDX = 0;
    RowsAndCols rc2;
    std::string findSolutionArray[5][5];
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            findSolutionArray[i][j] = array[i][j];
        }
    }
    
    while (solutionTries < 4)
    {
        srand(time(0));
        choice = (rand() % 5) + 1;

        switch(solutionTries % 2)
        {
            case 0:
                if (solutionTries)
                {
                    rc2.setCurrRow(0);
                    rc2.setCurrCol(choice - 1);
                }
                else
                {
                    rc2.setCurrCol(choice - 1);
                }
            case 1:
                rc2.setCurrRow(choice - 1);
        }        
        
        while (findSolutionArray[rc2.getCurrRow()][rc2.getCurrCol()] == " X")
        {
            // srand(time(0));
            choice = (rand() % 5) + 1;
            switch(solutionTries % 2)
            {
                case 0:
                    if (solutionTries)
                    {
                        rc2.setCurrRow(0);
                        rc2.setCurrCol(choice - 1);
                    }
                    else
                    {
                        rc2.setCurrCol(choice - 1);
                    }
                case 1:
                    rc2.setCurrRow(choice - 1);
            }    
        }
        
        solution[solutionIDX] = findSolutionArray[rc2.getCurrRow()][rc2.getCurrCol()];
        solutionTries++;
        solutionIDX++;
        findSolutionArray[rc2.getCurrRow()][rc2.getCurrCol()] = " X";
    }
}

void displaySolution(std::string solution[4])
{
    // Displays the solution array to the player.
    std::cout << "Solution: [";
    for (int i = 0; i < 4; i++)
    {
        if (i < 3)
        {
            std::cout << solution[i] << ", ";
        }
        else 
        {
            std::cout << solution[i];
        }
    }
    std::cout << "]\n";
}
