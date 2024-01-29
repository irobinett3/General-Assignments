// Ian Robinett
/* This program creates a crossword. It takes up to 20 word inputs from a user
and will generate a crossword on an ensuing board, and output it, creating a blank board
for the user to play, a solution board, and a list of clues. 
The words may be up to 15 letters long.
*/


// add needed libraries
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// define needed constants for length of words and number of words
#define board_size 15
#define num_words 20

// defines a struct that contains the position and direction of a word in the crossword
typedef struct{
	int x;
	int y;
	int dir; // a value of 0 corresponds to across and a value of 1 corresponds to down
} Location;


// defines needed functions
void initialize_userboard(char user_board[board_size][board_size]);
void intialize_solutionboard(char solution_board[board_size][board_size]);
void display_board(char board[board_size][board_size]);
void display_board_ext(char board[board_size][board_size], FILE *fp);
void upper_word(char *word);
void sort_input(char input[num_words][board_size], char output[num_words][board_size], int word_count);
int batch_readWords(FILE *fp, char input[num_words][board_size]);
int interactive_readWords(char input[num_words][board_size]);
void generate_clues(int word_count, char clues[word_count][board_size], char output[word_count][board_size]);
void scramble(char* word);
void place_word(char word[], char user_board[board_size][board_size], char solution_board[board_size][board_size], Location loc_arr[], int word_track[]);
void place_words(int word_count, char output[num_words][board_size], char user_board[board_size][board_size], char solution_board[board_size][board_size], Location loc_arr[], int word_track[]);
void display_clues(int word_count, Location loc_arr[], char clues[word_count][board_size], int word_track[]);
int count_neighbors(int x_loc, int y_loc, int dir, int length, char solution_board[][board_size]);
int count_tot(int x_loc, int y_loc, int dir, int length, int marker, char solution_board[][board_size]);
void display_clues_ext(int word_count, Location loc_arr[], char clues[word_count][board_size], int word_track[], FILE *fp);
void display_words(char output[][board_size], int word_count);
void display_words_ext(char output[][board_size], int word_count, FILE *fp);



// main function
int main(int argc, char *argv[]){

	// input will store the list of words input by the user in a 2D array
	char input[num_words][board_size];
	
	// output will sotre the list of words input by the user in a 2D array, but in order from longest to shortest
	char output[num_words][board_size];
	
	// defines 2D arrays for both boards that will be displayed to the user
	char solution_board[board_size][board_size];
	char user_board[board_size][board_size];

	// calls intialize board functions to create the two boards for the user
	initialize_userboard(user_board);
	intialize_solutionboard(solution_board);

	
	// asks the user for input if they only input one command line argument
	if(argc == 1){
		// asks the user for input
		printf("Please enter the words for the crossword followed by a "".""\n");
		// defines variable word_count to count the number of words input by user
		int word_count = 0;
		// calls interactive_readWords to read the words input by the user and obtain a value for word_count
		word_count = interactive_readWords(input);
		// calls sort_input to sort the words by length in descending order and store them in output
		sort_input(input, output, word_count);
		// defines 2D array clues to hold clues that will be displayed to user
		char clues[word_count][board_size];
		// calls generate_clues to store clues in array clues
		generate_clues(word_count, clues, output);
		// defines an array of Location data types, one for each word input by the user
		Location loc_arr[word_count];
		// defines array word_track, which will be used to track if words are printed to the crossword
		int word_track[word_count];
		// sets every value in word_track equal to 0
		for(int i = 0; i < word_count; i++){
			word_track[i] = 0;
		}
		// calls display_words to display all the words input by the user
		display_words(output, word_count);
		// calls place_word to place the longest word input by the user
		place_word(output[0], user_board, solution_board, loc_arr, word_track);
		// calls place_words to place all of the words input by the user
		place_words(word_count, output, user_board, solution_board, loc_arr, word_track);
		// prints the two crossword boards to the user
		printf("\nSolution:\n");
		display_board(solution_board);
		printf("\nCrossword Puzzle:\n");
		display_board(user_board);
		// displays the clues to the user
		display_clues(word_count, loc_arr, clues, word_track);
	// if the user inputs two command line arguments, the second is a file with words that need to be read. The second file will be read
	}else if (argc == 2){
		// creates a string to hold the name of the file input by the user
		char filename[20];
		// defines the file that will be opened
		FILE *fp;
		// copies the second command line argument input by the user into filename
		strcpy(filename, argv[1]);
		// opens the file with name input by the user, and displays an error message if the file does not exist
		fp = fopen(filename, "r");
		if(fp == NULL){
			printf("%s is an invalid file name: Please enter a valid name.\n", filename);
	
		}else{
			// defines variable word_count to count the number of words input by user
			int word_count;
			// calls batch_readWords to read the file input by the user and store the words in input, as well as obtain a value for word_count
			word_count = batch_readWords(fp, input);
			// closes the file
			fclose(fp);
			// calls sort_input to sort the words input by the user and store them in output
			sort_input(input, output, word_count);
			// creates 2D array to hold clues
			char clues[word_count][board_size];
			// calls generate_clues to create the clues and store them in the array clues
			generate_clues(word_count, clues, output);
			// creates an array of Location data types, with one Location for each word input by the user
			Location loc_arr[word_count];
			// creates array word_track to track which words have been placed in the crossword
			int word_track[word_count];
			// sets every element of word_track equal to 0
                	for(int i = 0; i < word_count; i++){
                	        word_track[i] = 0;
                	}
			// displays the words input by the user
			display_words(output, word_count);
			// places the longest word input by the user
			place_word(output[0], user_board, solution_board, loc_arr, word_track);
			// places all the words input by the user
			place_words(word_count, output, user_board, solution_board, loc_arr, word_track);
			// displays the two boards
			printf("\nSolution:\n");
			display_board(solution_board);
			printf("\nCrossword Puzzle:\n");
			display_board(user_board);
			// displays the clues
			display_clues(word_count, loc_arr, clues, word_track);
		}
	// if the user inputs three command line arguments, the second is a file that will be read, and the third is a file where the output will be printed.
	}else if(argc == 3){
		
		// creates a string to hold the name of the file input by the user
		char filename[20];
		// creates a file that will be opened
                FILE *fp;
		// copies the second argument input by the user into the variable that holds the filename input by the user
                strcpy(filename, argv[1]);
		// opens the file and displays an error message if it is null
                fp = fopen(filename, "r");
                if(fp == NULL){
                        printf("%s is an invalid fiile name: Please enter a valid name.\n", filename);
               	}else{
			// word_count tracks the number of words input by the user
                	int word_count;
			// calls batch_readWords to read the file input by the user, store the words in input, and obtain a value for word_count
                	word_count = batch_readWords(fp, input);
			// closes the file
			fclose(fp);
			// sorts the words in order of descending length and stores them in output
			sort_input(input, output, word_count);	
			// creates a variable to hold the name of the second file input by the user
			char file[20];
			// copies the third argument input by the user and stores it in file
			strcpy(file, argv[2]);
			// holds the length of the file
			int len = strlen(file);
			// gets rid of the null characters
			if(len > 0 && file[len - 1] == '\n'){
				file[len - 1] = '\0';
			}
			// opens file input by user
			FILE *fp2 = fopen(file, "w");
			// creates 2D array to hold clues
			char clues[word_count][board_size];
			// generates clues and stores them in clues
			generate_clues(word_count, clues, output);
			// creates an array of Locations with as many elements as there are words
			Location loc_arr[word_count];
			// creates array word_track to track if a word has been placed in the crossword
			int word_track[word_count];
			// sets each element of word_track equal to 0
                	for(int i = 0; i < word_count; i++){
                	        word_track[i] = 0;
                	}
			// displays words input by user in new file
			display_words_ext(output, word_count, fp2);
			// places the first word input by the user
			place_word(output[0], user_board, solution_board, loc_arr, word_track);
			// places all the words input by user
			place_words(word_count, output, user_board, solution_board, loc_arr, word_track);
			// displays the two boards in file input by user
			fprintf(fp2, "\nSolution:\n");
			display_board_ext(solution_board, fp2);
			fprintf(fp2, "\nCrossword Puzzle:\n");
			display_board_ext(user_board, fp2);
			// displays the clues in file input by user
			display_clues_ext(word_count, loc_arr, clues, word_track, fp2);
			// closes file
			fclose(fp2);
		}
	// tells the user to input a valid number of arguments if they have not input the correct amount
	}else{
		printf("Please enter a valid number of arguments.\n");
	}


	return 0;

}

// creates board for user
void initialize_userboard(char user_board[board_size][board_size]){

	// for loops change all values of the board passed in to be hashmarks
	for(int i = 0; i < board_size; i++){
                for(int j = 0; j < board_size; j++){
                        user_board[i][j] = '#';
                }
        }


}

// creates solution board
void intialize_solutionboard(char solution_board[board_size][board_size]){
	
	// for loops change all values of 2D array passed to be a period
	for(int i = 0; i < board_size; i++){
		for(int j = 0; j < board_size; j++){
			solution_board[i][j] = '.';
		}
	}

}

// prints a crossword board with proper formatting
void display_board(char board[board_size][board_size]){

	// prints a border at the top of the page
	for(int i = 0; i < board_size + 2; i++){
		printf("-");
	}

	// new line
	printf("\n");

	// for each row, prints a border at the start and then prints the contents of the board, and then prints another board
	for(int i = 0; i < board_size; i++){
		printf("|");
		for(int j = 0; j < board_size; j++){
			printf("%c", board[i][j]);
		}
		printf("|\n");
	}

	// prints the bottom border
	for(int i = 0; i < board_size + 2; i++){
		printf("-");
	}
	
	// new line
	printf("\n");

}

// displays the board for the user on an external file input by the user
void display_board_ext(char board[board_size][board_size], FILE *fp){

	// prints a border at the top of the page
        for(int i = 0; i < board_size + 2; i++){
                fprintf(fp, "-");
        }
        
        // new line
        fprintf(fp, "\n");
        
        // for each row, prints a border at the start and then prints the contents of the board, and then prints another board
        for(int i = 0; i < board_size; i++){
                fprintf(fp, "|");
                for(int j = 0; j < board_size; j++){
                        fprintf(fp, "%c", board[i][j]);
                }
                fprintf(fp, "|\n");
        }
        
        // prints the bottom border
        for(int i = 0; i < board_size + 2; i++){
                fprintf(fp, "-");
        }
        
        // new line
        fprintf(fp, "\n");



}

// function changes all the letters of a word to uppercase
void upper_word(char *word){

	// holds length of the word
	int len = strlen(word);

	// changes each character in the word to be uppercase
	for(int i = 0; i < len; i++){
		word[i] = toupper(word[i]);
	}

}

// function sorts the words input by the user in descending order in terms of length
void sort_input(char input[num_words][board_size], char output[num_words][board_size], int word_count){

	// creates an array with the same number of elements as the number of words
	int arr[word_count];

	// sets each value of array to be equal to the length of the string at the corresponding index in the array of words
	for(int i = 0; i < word_count; i++){
		arr[i] = strlen(input[i]);
	}

	// uses a bubble sort algorithm to sort the array of lengths in descending order
	for (int i = 0; i < word_count - 1; i++) {
     		for (int j = 0; j < word_count - i - 1; j++) {
         		if (arr[j] < arr[j + 1]) {
                		int temp = arr[j];
               			arr[j] = arr[j + 1];
                		arr[j + 1] = temp;
            		}
        	}
    	}

	// creates an array called seen and sets each value in it to 0. This array will be used to check if a value in the array of words has already been swapped
	int seen[word_count];
	for(int i = 0; i < word_count; i++){
		seen[i] = 0;
	}	

	// loops through word in the array of words input by the user and sorts them in descending order
	for(int i = 0; i < word_count; i++){
		// value is equal to the length of the string at that index
		int value = arr[i];
		// sets j = 0, j will be used as an increment in a while loop
		int j = 0;
		// while loop runs until j is equal to word_count
		while(j < word_count){
			// if the length of the string at index i is equal to the length of the string at index j, and the value at index j has not been seen before, places the value at index j into a new 2D array of the same length, but this time at index i.
			if(value == strlen(input[j]) && seen[j] == 0){
				strcpy(output[i], input[j]);
				// increments seen[j] so that the program can identify j has already been seen
				seen[j]++;
				// breaks out of the loop if value changed
				break;
			}
			// increments j
			j++;
		}
	}


}

// function reads all words from file fp into 2D array input
int batch_readWords(FILE *fp, char input[num_words][board_size]){
	// sets word_count equal to 0 and defines a temporary string
	int word_count = 0;
	char temp[board_size];

	// loop runs while word_count is less than num_words
	while(word_count < num_words){
		// scans line
		fscanf(fp, "%s", &temp);
		// breaks out of loop if user enters a period
		if(strcmp(temp, ".") == 0) break;
	
		int flag = 1;
		for(int i = 0; i < strlen(temp); i++){
			if(!isalpha(temp[i])){
				flag = 0;
				break;
			}
		}



		// lets user know if they have invalid input
		if(strlen(temp) < 2 || strlen(temp) > board_size || !flag){
			printf("%s is not a valid word and will be ignored\n", temp);
		// places word in input if it is valid
		}else{
			upper_word(temp);
			strcpy(input[word_count], temp);
			// increments word_count
			word_count++;
		}
	}

	// returns word_count
	return word_count;

}

// function reads the words input by the user
int interactive_readWords(char input[num_words][board_size]){
	
	// word_count tracks number of words input
	int word_count = 0;
	// temp holds value of each word on each line
	char temp[board_size];
	

	// runs until the user inputs 20 words
	while(word_count < num_words){
		// scans current line and stores it in temp
		scanf("%s", &temp);
		// breaks if the user enters a period
		if(strcmp(temp, ".") == 0) break;

		// flag tracks if the word is valid
		int flag = 1;
		// checks to make sure only letters are included in the string
		for(int i = 0; i < strlen(temp); i++){
			if(!isalpha(temp[i])){
				flag = 0;
				break;
			}
		}

		// displays a message that the word is not valid if it is invalid. otherwise, capitalizes the word and stores it in input
		if(strlen(temp) < 2 || strlen(temp) > board_size || !flag){
			printf("%s is not a valid word and will be ignored\n", temp);
		}else{
			upper_word(temp);
			strcpy(input[word_count], temp);
			word_count++;
		}
	}

	return word_count;

}

// function generates the anagram clues and stores them in array clues
void generate_clues(int word_count, char clues[word_count][board_size], char output[word_count][board_size]){

	// loops through each word in array containing words, scrambles them, and stores it in clues
	for(int i = 0; i < word_count; i++){
		// defines temporary string
		char temp[board_size];
		// copies value at index i of output into temp
		strcpy(temp, output[i]);
		// scrambles temp by calling scramble function
		scramble(temp);
		// copies scrambled temp into clues at index i
		strcpy(clues[i], temp);
	}

}

// scrambles a given word
void scramble(char* word){

	// holds length of given word
	int len = strlen(word);
	
	// loops in descending order, generating random numbers and swapping them with the value at index i
	for(int i = len - 1; i > 0; i--){
		int j = rand() % (i+1);
		char temp = word[i];
		word[i] = word[j];
		word[j] = temp;
	}

}

// places the first word and longest word in the array
void place_word(char word[], char user_board[board_size][board_size], char solution_board[board_size][board_size], Location loc_arr[], int word_track[]){

	// row will always be 7 for the first word
	int row = 7;
	// holds length of string
	int len = strlen(word);

	// sets the starting location of the word so that it is in the middle of the row
	int loc = len / 2;
	int start = 7 - loc;
	
	// stores x location, y location, and direction in the beginning index of the array of locations
	loc_arr[0].x = row;
	loc_arr[0].y = start;
	loc_arr[0].dir = 0;

	word_track[0] = 1;

	// changes the values of both boards in the appropriate locations to be equal to the corresponding values of the first word
	for(int i = 0; i < len; i++){
		solution_board[row][start + i] = word[i];
		user_board[row][start+i] = ' ';
	}


}

// places all words in the crossword
void place_words(int word_count, char output[num_words][board_size], char user_board[board_size][board_size], char solution_board[board_size][board_size], Location loc_arr[], int word_track[]){

	// pcount tracks the number of words that have been placed
	int pcount = 0;
	// marker tracks if the word being placed is being placed on the first or last letter of the previous word, or if it is being placed on a central letter in the previous word
	int marker;

	// loops through each word input by the user to try and place it
	for(int i = 1; i < word_count; i++){
		// placed holds the value of 0 if the word has not been placed, and 1 if it has been placed
		int placed = 0;
		// loops through each word already placed on the crossword to check if the word being checked is a valid match
		for(int m = 1; m <=i; m++){
			// temp2 holds the word at the current index of i, temp1 holds the word that it is being compared to at this index of m
			char temp1[board_size];
			char temp2[board_size];
			strcpy(temp1, output[i-m]);
			strcpy(temp2, output[i]);
			// hold lengths of string 1 and string 2
			int len1 = strlen(temp1);
			int len2 = strlen(temp2);
		
			// loops through each character in string 1 to compare it to string 2
			for(int j = 0; j < len1; j++){
				// if the character is either at the start or the end of the string, marker = 1; otherwise, marker = 0
				if(j == 0 || j == len1 - 1){
					marker = 1;
				}else{
					marker = 0;
				}
		
				//loops through each character in string 2 to compare to string 1
				for(int k = 0; k < len2; k++){
					
					// performs a series of tests to attempt to place the word if the current character of string 1 equals the current character of string 2
					if(temp1[j] == temp2[k]){
						// stores the direction of the previous word that temp2 is trying to be placed on
						int dir = loc_arr[i - m].dir;

						// if direction equals 0, the previous word was horizontal, so temp2 needs to be placed vertically
						if(dir == 0){
							// gets the starting x and y location based on the x and y location of the previous word and the indexes currently being used
							int y_loc = loc_arr[i-m].y + j;
                                                	int x_loc = loc_arr[i-m].x - k;
							// attempts to place the word if the word will fit on the board at that location
							if(x_loc >= 0 && x_loc + len2 <= board_size){
								// count tracks the number of open squares next to the location the word should be placed
								// tot_count tracks the total number of open squares that need to be present in order for the word to be placed
								int count, tot_count;
								// calls functions to get a value for count and tot_count
								count = count_neighbors(x_loc, y_loc, dir, len2, solution_board);
								tot_count = count_tot(x_loc, y_loc, dir, len2, marker, solution_board);
								// if count is greater than or equal to count, the word will be placed on the board
								if(count >= tot_count){
									// increments word track to identify that the word has been placed
									word_track[i] = 1;
									// increments placed to identify that the word has been placed
									placed++;
									// places the word on both boards and sets the direction, x location, and y location of the current word
									for(int l = x_loc; l < x_loc + len2; l++){
										loc_arr[i].y = y_loc;
										loc_arr[i].x = x_loc;
										loc_arr[i].dir = 1;
										solution_board[l][y_loc] = output[i][l - x_loc];
										user_board[l][y_loc] = ' ';
									}
								}
							}
						// if direction equals 1, the previous word was vertical, so temp2 needs to be placed horizontally
						}else if(dir == 1){
							// gets the starting x and y location based on the x and y location of the previous word and the indexes currently being used
							int y_loc = loc_arr[i-m].y - k;
                                                	int x_loc = loc_arr[i-m].x + j;
							// attempts to place the word if it will fit on the board
							if(y_loc >= 0 && y_loc + len2 <= board_size){
								// count tracks the number of open squares next to the location the word will be placed
								// tot_count tracks the total number of open squares that need to be present for the word to be placed
								int count, tot_count;
								// calls functions to get a value for count and tot_count
								count = count_neighbors(x_loc, y_loc, dir, len2, solution_board);
								tot_count = count_tot(x_loc, y_loc, dir, len2, marker, solution_board);
								// places the word if count is greater than or equal to tot_count
								if(count >= tot_count){
									// increments word_track to identify that the word has been placed
									word_track[i] = 1;
									// increments placed to identify that the word has been placed
									placed++;
									// places the word on both boards and stores the direction, x location, and y location of the word
									for(int l = y_loc; l < y_loc + len2; l++){
										loc_arr[i].y = y_loc;
										loc_arr[i].x = x_loc;
										loc_arr[i].dir = 0;
										solution_board[x_loc][l] = output[i][l - y_loc];
										user_board[x_loc][l] = ' ';
									}
								}
							}
						}
					}
					// increments pcount if place has been incremented	
					pcount += placed;			
					// breaks out of the loop if i = pcount
					if(pcount == i) break;
				}
				// breaks out of the loop if i = pcount
				if(pcount == i) break;
			}
			// breaks out of the loop if i = pcount
			if(pcount == i) break;
		}
	}


	// very similar to the program above, but runs again to check if words that could not be placed earlier can now be placed

	// fcount tracks the number of words that have been placed
	int fcount = 0;

	// checks each word input by the user to try and place it if it has not already been placed
	for(int i = 1; i < word_count; i++){
		
		// enters into the program if the value of word track at i is 0, or if the word has not already been placed
		if(word_track[i] == 0){
			// sets placed = 0 to track if the word has been placed
			int placed = 0;
			// loops through every word in the list to see if the word we are checking can be placed on that word
        	       	for(int m = 0; m < word_count; m++){
				// only checks the word at the index of m if it has already been placed (its value at index m of word_track is not 0)
				if(word_track[m] != 0){
					// stores the words we are comparing in temp1 and temp2, and stores their lengths in len1 and len2
        	               		char temp1[board_size];
                        		char temp2[board_size];
                        		strcpy(temp1, output[m]);
                        		strcpy(temp2, output[i]);
                        		int len1 = strlen(temp1);
                        		int len2 = strlen(temp2);
					// loops through each letter of temp1 to compare it to temp2 
                        		for(int j = 0; j < len1; j++){
						// sets marker equal to 1 if it is the first or last letter of temp1
                                		if(j == 0 || j == len1 - 1){
                                		        marker = 1;
                                		}else{
                                		        marker = 0;
                                		}
						// compares each letter of temp2 to temp1
                                		for(int k = 0; k < len2; k++){
							// attempts to place the word if the letter in temp1 equals the letter in temp2
                                        		if(temp1[j] == temp2[k]){
								// obtains the direction of the previous word
                         	              	       		int dir = loc_arr[m].dir;
								// if the direction of the previous word is 0, then it is horizontal, and the next word will be placed vertically
                                        		        if(dir == 0){
									// stores the x and y locations of where the word will be placed based on the x and y location of the previous word and the current indices being used
                                                	        	int y_loc = loc_arr[m].y + j;
                                                        		int x_loc = loc_arr[m].x - k;
									// attempts to place the word if it will fit on the board
                                                        		if(x_loc >= 0 && x_loc + len2 <= board_size){
										// count tracks the number of open squares adjacent to and in where the word will be placed
										// tot_count tracks the number of open squares that need to be present for the word to be placed
                                                        	        	int count, tot_count;
										// calls count_neighbors and count_tot to get values for count and tot_count
                                                        	        	count = count_neighbors(x_loc, y_loc, dir, len2, solution_board);
                                                        	        	tot_count = count_tot(x_loc, y_loc, dir, len2, marker, solution_board);
										// places the word if count is greater than or equal to tot_count
                                                        	        	if(count >= tot_count){
                                                                	        	// increments the value at the current index of word_track to indicate that the word has been placed
											word_track[i] = 1;
											// increments placed to indicate that the word has been placed
											placed++;
											// places the word on both boards and stores the x and y location and the direction of the word being placed
                                                                	        	for(int l = x_loc; l < x_loc + len2; l++){
                                                                        	        	loc_arr[i].y = y_loc;
                                                                                		loc_arr[i].x = x_loc;
                                                                                		loc_arr[i].dir = 1;
                                                                                		solution_board[l][y_loc] = output[i][l - x_loc];
                                                                        	        	user_board[l][y_loc] = ' ';
                                                        	                	}
                                                	                	}
                                        	                	}
								// if the direction of the previous word is 1, then it is vertical, and the next word will be placed horizontally
                                	                	}else if(dir == 1){
									// stores the x and y locations of where the word will be palced based on the x and y locations of the previous word and the current indices being used
                                	                        	int y_loc = loc_arr[m].y - k;
                                	                        	int x_loc = loc_arr[m].x + j;
									// attempts to place the word if it will fit on the board
                                	                        	if(y_loc >= 0 && y_loc + len2 <= board_size){
										// count tracks the number of open squares adjacent to and in where the word will be placed
										// tot_count tracks the number of open squares that need to be present for the word to be placed
                                	                        	        int count, tot_count;
										// calls count_neighbors and count_tot to get values for count and tot_count
                                	                        	        count = count_neighbors(x_loc, y_loc, dir, len2, solution_board);
                                	                        	        tot_count = count_tot(x_loc, y_loc, dir, len2, marker, solution_board);
										// places the word if count is greater than or equal to tot_count
                                	                        	        if(count >= tot_count){
											// increments the value at the current index of word_track to indicate the word has been placed
                                	                        	                word_track[i] = 1;
											// increments placed to indicate that the word has been placed
                                	                        	                placed++;
											// places the word on both boards and stores the x and y location and the direction of the word being placed
											for(int l = y_loc; l < y_loc + len2; l++){
                                	                                	                loc_arr[i].y = y_loc;
                                	                                        	        loc_arr[i].x = x_loc;
                                	                                                	loc_arr[i].dir = 0;
                                	                                                	solution_board[x_loc][l] = output[i][l - y_loc];
                                	                                                	user_board[x_loc][l] = ' ';
                                	                                        	}	
                                	                                	}
                                	                        	}
                                	                	}
							}
							// increments fcount if placed has been incremented
							fcount += placed;
							// breaks out of the loop if fcount is equal to i
							if(fcount == i) break;
						}
						// breaks out of the loop if fcount is equal to i
						if(fcount == i) break;
					}
					// breaks out of the loop is fcount is equal to i
					if(fcount == i) break;
				}
			}
			// displays if the word cannot be placed
			if(placed == 0){
				printf("\nIt was not possible to place %s: it will be ignored.\n", output[i]);
			}
		}
	}
}

// counts the number of empty spaces in and around where the word should be place
int count_neighbors(int x_loc, int y_loc, int dir, int length, char solution_board[][board_size]){

	// stores the count of open spaces
	int count = 0;

	// counts vertically if the previous word was placed horizontally
	if(dir == 0){
		// loops through all of the indexes where the word would be placed, as well as all of the indexes adjacent to them
		for(int l = x_loc; l < x_loc + length; l++){
			if(solution_board[l][y_loc] == '.'){
                       	        count++;
                       	}
                       	if(solution_board[l][y_loc + 1] == '.' && y_loc < board_size - 1){
                       	        count++;
                       	}
                       	if(solution_board[l][y_loc - 1] == '.' && y_loc > 0){
                       	        count++;
                       	}
               	}
		// checks the square directly above where the word is placed
                if(solution_board[x_loc - 1][y_loc] == '.' && x_loc > 0){
                        count++;
                }
		// checks the square directly below where the word is placed
                if(solution_board[x_loc + length][y_loc] == '.' && (x_loc + length) <= (board_size - 1)){
                        count++;
                }
	//counts horizontally if the previous word was placed vertically
	}else if(dir == 1){
		// loops through all of the indexes where the word would be placed, as well as all of the indexes adjacent to those
        	for(int l = y_loc; l < y_loc + length; l++){
        	        if(solution_board[x_loc][l] == '.'){
        	        	count++;
        	        }
               		if(solution_board[x_loc + 1][l] == '.' && x_loc < board_size - 1){
                		count++;
                	}
                	if(solution_board[x_loc - 1][l] == '.' && x_loc > 0){
        			count++;
        		}
        	}
		// checks the square directly to the left of where the word is placed
        	if(solution_board[x_loc][y_loc - 1] == '.' && y_loc > 0){
        		count++;
        	}
		// checks the square directly to the right of where the word is placed
        	if(solution_board[x_loc][y_loc + length] == '.' && (y_loc + length) < (board_size)){
        		count++;
        	}
	}
	// returns the value of count
	return count;

}

// checks the number of squares that need to be open in order for a word to be placed
int count_tot(int x_loc, int y_loc, int dir, int length, int marker, char solution_board[][board_size]){


	// tracks number of squares
        int tot_count = 0;

	// counts vertically if the previous word was placed horizontally
        if(dir == 0){
		// checks all of the indices where the word could be placed as well as those adjacent to the word
                for(int l = x_loc; l < x_loc + length; l++){
                        tot_count++;
                        if(y_loc < board_size - 1){
                                tot_count++;
                        }
                        if(y_loc > 0){
                                tot_count++;
                        }
                }
		// increments tot_count if there is an available square above where the word would be placed
                if(x_loc > 0){
                        tot_count++;
                }
		// increments tot_count if there is an available square below where the word would be placed
                if((x_loc + length) <= (board_size - 1)){
                        tot_count++;

                }
		// subtracts 2 from tot_count to account for the 2 overlapping squares with the previous word if the word is being placed on the edge of the board
		if(y_loc <= 0 || y_loc >= board_size - 1){
			tot_count -= 2;
		}else{
			// subtracts 2 from tot_count to account for the 2 overlapping squares with the previous word if the word is being placed on the first or last letter of the previous word
			if(marker == 1){
				tot_count -= 2;
			// subtracts 3 from tot_count to account for the overlapping letters for the previous word otherwise
			}else if(marker == 0){
				tot_count -= 3;
			}
		}
	// counts horizontally if the previous word was placed vertically
        }else if(dir == 1){
		// checks all of the indices where the word could be placed as well as those adjacent to the word
                for(int l = y_loc; l < y_loc + length; l++){
                        tot_count++;
                        if(x_loc < board_size - 1){
                                tot_count++;
                        }
                        if(x_loc > 0){
              		        tot_count++;
                        }
                }
		// increments tot_count if there is an available square to the left of where the word would be placed
                if(y_loc > 0){
                        tot_count++;
                }
		// increments tot_count if there is an available square to the right of where the word would be placed
                if((y_loc + length) < (board_size)){
                        tot_count++;
                }
		// subtracts 2 from tot_count to account for the 2 overlapping squares with the previous word if the word is being placed on the edge of the board
		if(x_loc <= 0 || x_loc >= board_size -1){
			tot_count -= 2;
		}else{
			// subtracts 2 from tot_count to account for the 2 overlapping squares with the previous word if the word is being placed on the first or last letter of the previous word
			if(marker == 1){
				tot_count -= 2;
			// subtracts 3 from tot_count to account for the overlapping letters for the previous word otherwise
			}else if(marker == 0){
				tot_count -= 3;
			}
		}
        }
	// returns the value of tot_count
        return tot_count;
}

// displays the clues to the user
void display_clues(int word_count, Location loc_arr[], char clues[word_count][board_size], int word_track[]){

	
	// informs the user what is displayed
	printf("\nClues: \n");
	printf("Location   | Direction | Anagram\n");
	// Loops through each word placed in the crossword and displays its coordinates, direction, and a scrambled form of it
	for(int i = 0; i < word_count; i++){
		if(word_track[i] != 0){
			if(loc_arr[i].dir == 0){
				printf("%d,\t%2d | Across    | %s\n", loc_arr[i].y, loc_arr[i].x, clues[i]);
			}else{
				printf("%d,\t%2d | Down      | %s\n", loc_arr[i].y, loc_arr[i].x, clues[i]);
			}
		}
	}


}

// displays the clues to an external file
void display_clues_ext(int word_count, Location loc_arr[], char clues[word_count][board_size], int word_track[], FILE *fp){


        // informs the user what is displayed
        fprintf(fp, "\nClues: \n");
        fprintf(fp, "Location   | Direction | Anagram\n");
        // Loops through each word placed in the crossword and displays its coordinates, direction, and a scrambled form of it
        for(int i = 0; i < word_count; i++){
                if(word_track[i] != 0){
                        if(loc_arr[i].dir == 0){
                                fprintf(fp, "%d,\t%2d | Across    | %s\n", loc_arr[i].y, loc_arr[i].x, clues[i]);
                        }else{
                                fprintf(fp, "%d,\t%2d | Down      | %s\n", loc_arr[i].y, loc_arr[i].x, clues[i]);
                        }
                }
        }


}

// displays all of the words input by the user
void display_words(char output[][board_size], int word_count){

	// title for the crossword
	printf("\nAnagram Crossword Puzzle Generator\n");
	printf("----------------------------------\n");

	printf("\nYour list of words:\n");
	// loops through each word in the list and prints it
	for(int i = 0; i < word_count; i++){
		printf("%s\n", output[i]);
	}


}

// prints all of the words input by the user to an external file
void display_words_ext(char output[][board_size], int word_count, FILE *fp){

	// title for the crossword
	fprintf(fp, "\nAnagram Crossword Puzzle Generator\n");
        fprintf(fp, "----------------------------------\n");


	// loops through each word in the list and prints it
        fprintf(fp, "\nYour list of words:\n");
        for(int i = 0; i < word_count; i++){
                fprintf(fp, "%s\n", output[i]);
        }


}