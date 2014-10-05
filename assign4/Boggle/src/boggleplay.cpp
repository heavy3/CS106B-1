// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "bogglegui.h"
#include <cmath>

Boggle setUpBoard(Lexicon& dictionary);
bool isValidBoardInput(string& input);
void printBoard(Boggle& board);
void printHumanState(Boggle &board);
void playHuman(Boggle &board);
void printStatusMessage(bool isValidWord, string word);

/**
 * @brief playOneGame
 * @param dictionary
 */
void playOneGame(Lexicon& dictionary) {
    // set up board
    BoggleGUI::initialize(4, 4);
    Boggle board = setUpBoard(dictionary);
    
    // print initial state
    clearConsole();
    BoggleGUI::labelAllCubes(board.getCurrentBoard());
    
    cout << "It's your turn!" << endl;
    playHuman(board);
}

/**
 * @brief playHuman
 * @param board
 */
void playHuman(Boggle &board) {
    // get and display words from human
    bool humanHasWords = true;
    bool isValidWord = false;
    string humanWord;
    
    while(humanHasWords) {
        // print current state of game
        printBoard(board);
        printHumanState(board);
        
        // prompt for word
        humanWord = getLine("Type a word (or Enter to stop): ");
        
        if (humanWord.empty()) {
            humanHasWords = false;
        } else {
            humanWord = toLowerCase(humanWord);
            // check to see if valid word
            isValidWord = board.humanWordSearch(humanWord);
            // update user state
            // update boggle board gui
            BoggleGUI::recordWord(humanWord, BoggleGUI::HUMAN);
            
            clearConsole();
            printStatusMessage(isValidWord, humanWord);
        }
    }
}

/**
 * @brief printStatusMessage
 * @param isValidWord
 * @param word
 */
void printStatusMessage(bool isValidWord, string word) {
    string message;
    
    if(isValidWord) {
        message = string("You found a new word! \"") + toUpperCase(word) + string("\"");
    } else {
        message = "You must enter an unfound 4+ letter word from the dictionary.";
    }
    
    cout << message << endl;
    BoggleGUI::setStatusMessage(message);
}

/**
 * @brief setUpBoard
 * Handles user interactions for board setup - either tells Boggle board
 * to randomly generate the letters or asks for a valid input string from
 * the user for a custom board. Then creates instance of Boggle board for 
 * use throughout the rest of the game.
 * @param dictionary - Lexicon to pass to Boggle constructor.
 * @return - Returns Boggle instance
 */
Boggle setUpBoard(Lexicon& dictionary) {
    bool randomBoard = getYesOrNo("Do you want to generate a random board? ");
    string userBoardText = "";
    
    // user generated board text
    if(!randomBoard) {
        while(!isValidBoardInput(userBoardText)) {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
    
    return Boggle(dictionary, userBoardText);
}

/**
 * @brief isValidBoardInput
 * Prompts user for board string and checks for the validity of the input 
 * for the custom board.
 * @param input - User input string, passed by reference from setUpBoard.
 * @return - Returns whether input string is valid, and therefore whether
 * setUpBoard should continue prompting the user.
 */
bool isValidBoardInput(string& input) {
    input = getLine("Type the 16 letters to appear on the board: ");
    
    // check length of string
    if (input.length() != Boggle::BOARD_SIZE) return false;
    
    // check that all characters are letters
    input = toUpperCase(input);
    for (int i = 0; i < input.length(); i++) {
        if(! ((input[i] >= 'A') && (input[i] <= 'Z'))) return false;
    }
    
    // passed all checks!
    return true;
}

/**
 * @brief printBoard
 * Prints current board layout to console.
 * @param board - Boggle instance being used for the current game.
 */
void printBoard(Boggle& board) {
    int rows = sqrt(Boggle::BOARD_SIZE);
    int cols = rows;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << board.getLetter(i, j);
        }
        cout << endl;
    }
}

/**
 * @brief printUserState
 * Prints user's current state in the game - numbers of words, list of used
 * words, and score.
 * @param board - Boggle instance being used for the current game.
 */
void printHumanState(Boggle &board) {
    cout << endl;
    cout << "Your words (" << board.getNumHumanWords() << "): " 
         << board.getHumanWords() << endl;
    cout << "Your score: " << board.humanScore() << endl;
    BoggleGUI::setScore(board.humanScore(), BoggleGUI::HUMAN);
}
