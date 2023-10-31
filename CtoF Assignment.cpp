/*
Lab 1 - Input, Output and Reading and Writing to text files
Celcius to Farenheit - Farenheit to Celcius converter */

#include <iostream> // For our input and output
#include <fstream> // For our file stream objects
#include <string> // For our string objects


enum ConversionType {
    CELCIUS_TO_FARENHEIT, // By default,this is 0
    FARENHEIT_TO_CELCIUS  // By default this is 1
};

#define CtoF(celcius) ((9.0/5.0) * (celcius) + 32) // Macros for assignment requirements
#define FtoC(farenheit) ((5.0/9.0) * ((farenheit) - 32))

using namespace std;

// Function to display the menu
void displayMenu()
{
    // Then display the menu options after the console is cleared
    cout << "Please select from the following options:" << endl;
    cout << "\n1. Create a blank text file" << endl;
    cout << "\n2. Enter in a Range of Temperatures" << endl;
    cout << "\n3. Convert Range as Celcius to Farenheit" << endl;
    cout << "\n4. Convert Range as Farenheit to Celcius" << endl;
    cout << "\n5. Display to the console the contents of the text file" << endl;
    cout << "\n6. Open a text file with notepad" << endl;
    cout << "\n7. Exit \n" << endl; // Return 0 to exit the program
    cout << "Please enter your choice here: "; // Prompt the user to enter their choice via the while/switch statements
}

// Function to check if a file exists
bool fileExists(const string& filename) { // Using a const reference to avoid copying the string
    ifstream file(filename);
    return file.good();
}

/* Function to make a brand spanking new.txt file with a name of their choosing if the file already
exists we call fileExists function and tell the user it already exists and to choose another name
if it doesn't exist create a .txt with the name the user chose */
void createTxtFile() {
    // System("cls") I believe only works on Windows untested on other OS's
    system("cls"); // Clear the menu to make it look nicer for the user to keep relevant information on the screen
    string fileName;

    while (true) {
        cout << "Please enter the name of the file you would like to create (or type 'exit' to return to main menu): ";
        cin >> fileName; // User will enter the name of the file they want to create

        if (fileName == "exit") {
            cout << "Returning to main menu. Press enter to continue..." << endl;
            cin.get();
            system("cls");
            return; // Exit the function to return to the main menu incase user accidentally chose this option
        }

        fileName += ".txt"; // add the .txt extension to the file name the user chooses

        if (fileExists(fileName)) { // Check if the file already exists
            // If the file exist we loop back to the beginning to see if the user wants to try again or exit
            cout << "File already exists, please choose another name." << endl;
        }
        else {
            ofstream file(fileName);
            if (file.is_open()) {
                cout << "File created successfully!" << endl;
                file.close();
                break; // exit the while loop
            }
            else { // If there's an issue that isn't the file already existing
                cout << "Error creating file. Please try again." << endl;
            }
        }
    }
    cout << "Press enter to continue..." << endl;
    cin.get(); 
    /* Wait for user input to continue before we go back and clear the console
       this gives the user time to read the message at their own pace */
    system("cls"); // Clear the console to make it look nicer when looping back to the main menu
}

/* Function to handle if the user is outside of making a text file and enters in the name of a 
file that doesn't exist. We will ask them if they want to make the file or not to avoid having 
to cycle back to the first option in the main menu again. */
bool handleFileOpeningError(const string& fileName) { // Using a const reference to avoid copying the string
    // No clearing console here since this function is only called if there's an error
    char choice;
    if (!fileExists(fileName)) { // Here's where we check if the file exists or not
        // If the file doesn't exist we ask the user if they want to create it
        cout << "File '" << fileName << "' does not exist. Would you like to create it now? (y/n) ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            ofstream newFile(fileName);
            if (newFile.is_open()) {
                cout << "File '" << fileName << "' created successfully!" << endl;
                newFile.close();
                return true; // File was successfully created
            }
            else {
                cout << "The file was not created. Press enter to return to the main menu." << endl;
                cin.get();
                system("cls");
                return false; // File was not created
            }
        }
        else {
            // The user chose not to create the file.
            cout << "Returning to main menu. Press enter to continue..." << endl;
            cin.get();
            system("cls");
            return false; // File was not created
        }
    }
    else {
        cout << "Error opening file. Please try again." << endl;
    }
    return false; // File was not created
}

// Function to write a range of temperatures to a file
void writeRangeToFile() {
    system("cls");
    string fileName;
    int startTemp, endTemp;

    cout << "Please enter the name of the file you would like to write to: ";
    cin >> fileName;
    fileName += ".txt";

    ofstream file(fileName);

    if (file.is_open()) {
        cout << "Please enter the starting temperature: ";
        cin >> startTemp;
        cout << "Please enter the ending temperature: ";
        cin >> endTemp;

        // Loop through the range of temperatures and write them to the file
        for (int i = startTemp; i <= endTemp; i++) {
            file << i << endl;
        }
        file.close();
    }
    else if (!handleFileOpeningError(fileName)) {
        return; // Exit the function if the file was not created or there was another error.
    }
    cout << "Press enter to continue..." << endl;
    cin.get();
    system("cls");
}

/* Function to convert temperatures based on the ConversionType passed. The function first attempts to open the
input file (where the temperatures to convert are stored). If the input file does not exist, the function uses
handleFileOpeningError() to ask the user if they want to create it. If they do, or if the file exists, the function
then creates an output file to store the converted temperatures. It then reads temperatures from the input file,
converts them, and writes the converted temperatures to the output file. One minor bug that's sort of a non-issue
is if the file doesn't exist it will make two files one with the name and the proper append at the end but both
will be blank. It's not an issue since it can be overwritten when the user tries to convert again with the
original file name */
void convertTemperatures(ConversionType type) {
    system("cls");
    string fileName;  // Name of the file to convert
    double temp;  // Double to store each temperature read from the input file
    string line, inputFileName, outputFileName;  // Strings to store the full file names and each line read

    // Prompt the user for the name of the file to convert
    cout << "Please enter in the name of the file you would like to convert: ";
    cin >> fileName;
    inputFileName = fileName + ".txt";

    // Try to open the input file
    ifstream inputFile(inputFileName);

    // If the input file can't be opened, handle the file opening error
    if (!inputFile.is_open() && !handleFileOpeningError(inputFileName)) {
        return;  // Exit the function if the inputFile was not created or there was another error.
    }

    // Determine the name of the output file based on the conversion type
    if (type == CELCIUS_TO_FARENHEIT) {
        outputFileName = fileName + "_CtoF.txt";
    }
    else {
        outputFileName = fileName + "_FtoC.txt";
    }

    // Try to create the output file
    ofstream outputFile(outputFileName);

    // If the output file can't be created, display an error message and exit
    if (!outputFile.is_open()) {
        cout << "Error creating output file. Please try again." << endl;
        return;
    }

    // Read each line from the input file, convert the temperature, and write it to the output file
    while (getline(inputFile, line)) {
        temp = stod(line);
        if (type == CELCIUS_TO_FARENHEIT) {
            outputFile << CtoF(temp) << endl;
        }
        else {
            outputFile << FtoC(temp) << endl;
        }
    }

    // Notify the user that the conversion is complete and display the name of the output file
    cout << "Conversion complete! Converted values saved to: " << outputFileName << endl;

    // Close the input and output files
    inputFile.close();
    outputFile.close();

    // Prompt the user to press enter to continue and clear the console
    cout << "Press enter to continue..." << endl;
    cin.get();
    system("cls");
}

// Function to read a file and display the contents to the console
void readFromFile() {
    system("cls");
    string fileName;
    string line;
    char choice;
    bool isEmpty = true; // Assume the file is empty initially

    cout << "Please enter the name of the file you would like to read from: ";
    cin >> fileName;
    cin.ignore(); // Clear out any remaining newline
    fileName += ".txt";

    ifstream file(fileName);

    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
            isEmpty = false; // If we're here, then the file isn't empty
        }
        file.close();

        if (isEmpty) {
            cout << "I'd like to display the contents but I don't see anything in this file." << endl;
        }
    }
    else if (!handleFileOpeningError(fileName)) {
        return; // Exit the function if the file was not created or there was another error.
    }

    cout << "Press enter to continue..." << endl;
    cin.get();
    system("cls");
}

void openFileWithNotepad(const string& fileName) {
    string command = "notepad.exe " + fileName; // Create a string with the command to open the file with notepad
    system(command.c_str()); 
    /* c_str is part of the C standard library so we use this to convert the string to a const char* in this case 
    it's the file name we want to open with notepad */
}

int main() {
    string input;
    int choice;

    while (true) {
        displayMenu();
        getline(cin, input); // Read the entire line

        try { // Convert string to integer to avoid infinite loop if user enters a char or string
            choice = stoi(input); 

            switch (choice) {
            case 1:
                createTxtFile();
                break;
            case 2:
                writeRangeToFile();
                break;
            case 3:
                convertTemperatures(CELCIUS_TO_FARENHEIT); // Pass in the enum for celcius to farenheit
                break;
            case 4:
                convertTemperatures(FARENHEIT_TO_CELCIUS); // Pass in the enum for farenheit to celcius
                break;
            case 5:
                readFromFile();
                break;
            case 6:
                // Prompt the user for the name of the file to open with notepad
                cout << "Please enter the name of the file you would like to open with notepad: ";
                cin >> input;
                // Inform the user about the behavior
                cout << "Opening Notepad. The program will wait until you close Notepad to continue..." << endl;
                openFileWithNotepad(input);
                cout << "Press enter to continue..." << endl;
                cin.get(); // Wait for user input to continue to make sure they're read to loop back to the main menu
                system("cls"); // Clear the console after the user closes notepad
                break;
            case 7:
                cout << "Exiting program..." << endl;
                return 0;
            default:
                cout << "Hey! Last time I checked that wasn't a number between 1 and 7. Try again." << endl;
            }
        }
        /* I throw exceptions if the user enters something that isn't a number or if the number is too large I did
         this since entering in a char or string would cause an infinite loop since the input would never be a number */
        catch (const invalid_argument&) {
            // This exception will be thrown by stoi if conversion fails
            cout << "Hey that's not even a number! Maybe read the instructions again?" << endl;
        }
        catch (const out_of_range&) {
            // This exception will be thrown by stoi if the number is too large
            cout << "Hey! Last time I checked that wasn't a number between 1 and 7. Try again." << endl;
        }
    }

    return 0;
}