#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

struct Tile {
	char display;
	bool isMine;
	int numMines;
};

vector<vector<Tile>>board;

void setDirections(int row, int col, int totRows, int totCols) {
	if (row > 0) {
		board[row - 1][col].numMines++;
	}
	if (row < (totRows - 1)) {
		board[row + 1][col].numMines++;
	}
	if (col > 0) {
		board[row][col - 1].numMines++;
	}
	if (col < (totCols - 1) ){
		board[row][col+1].numMines++;
	}
}

int initializeBoard(int numRows, int numCols, int startRow, int startColumn) {
	
	for (int j = 0; j < numRows; j++) {
		vector<Tile>row;
		for (int i = 0; i < numCols; i++) {
			row.push_back({ '-',false, 0 });
		}
		board.push_back(row);
	}
	srand(time(0));
	int numMines = rand() % (numRows * numCols/2);
	int times = 0;
	while(times<numMines) {
		int row = rand() % numRows;
		int col = rand() % numCols;
		if (!board[row][col].isMine &&(row!=startRow || col!=startColumn)) {
			//if random position is valid, add a mine
			board[row][col].isMine = true;
			setDirections(row, col, numRows, numCols);
			times++;
		}
	}
	return numMines;

}

int displayBoardHelper(int row, int col, int totRows, int totCols) {
	int numZeroes = 0;
	if (board[row][col].numMines > 0 || board[row][col].isMine) {
		return 0; //base case
	}
	else if (row<0 || col<0 || row>=totRows || col>=totCols) {
		return 0;
	} //second base case, row col is invalid tile
	else {
		if (row > 0 && !board[row-1][col].visited) {
			board[row - 1][col].display = '0' + board[row-1][col].numMines;
			board[row - 1][col].visited = true;
			numZeroes++;
			numZeroes+=displayBoardHelper(row - 1, col, totRows, totCols);
		}
		if (row < (totRows - 1) && !board[row + 1][col].visited) {
			board[row + 1][col].display = '0' + board[row+1][col].numMines;
			board[row + 1][col].visited = true;
			numZeroes++;
			numZeroes+=displayBoardHelper(row + 1, col, totRows, totCols);
		}
		if (col > 0 && !board[row][col - 1].visited) {
			board[row][col - 1].display = '0' + board[row][col-1].numMines;
			board[row][col - 1].visited = true;
			numZeroes++;
			numZeroes+=displayBoardHelper(row, col - 1, totRows, totCols);
		}
		if (col < (totCols - 1) && !board[row][col + 1].visited) {
			board[row][col + 1].display = '0' + board[row][col+1].numMines;
			board[row][col + 1].visited = true;
			numZeroes++;
			numZeroes += displayBoardHelper(row, col + 1, totRows, totCols);
		}
	}
	return numZeroes;
}

int displayBoard(int row, int col, int numRows, int numCols) {
	if (!board[row][col].isMine) {
		board[row][col].display = '0' + board[row][col].numMines;
	}
	else {
		board[row][col].display = 'd';
	}
	board[row][col].visited = true;
	int val=displayBoardHelper(row, col, numRows, numCols);
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			cout << board[i][j].display<<" ";
		}
		cout << "\n";
	}
	return ++val;
}
bool verifyValidBegin(int row, int col, int numRows, int numCols) {
	bool valid = true;
	if (row >= numRows) valid = false;
	if (col >= numCols) valid = false;
	if (!valid) {
		cout << "Sorry, but the row and column you have entered is invalid. Please try again\n";
	}
	return valid;
}
bool verifyValidColRow(int row, int col, int numRows, int numCols) {
	bool valid = true;
	if (row >= numRows) valid = false;
	if (col >= numCols) valid = false;
	if (valid && board[row][col].display != '-') valid = false;
	if (!valid) {
		cout << "Sorry, but the row and column you have entered is invalid. Please try again\n";
	}
	return valid;
}

bool checkMine(int row, int col) {
	return board[row][col].isMine;
}



int main(int argc, char* argv[]) {
	bool lost = false;
	cout << "How big is your board?" << "\n";
	cout << "Number of rows: \n";
	int numRows;
	cin >> numRows;
	cout << "Number of columns: \n";
	int numColumns;
	cin >> numColumns;
	cout << "Please indicate your position by typing in {row column}" << "\n";
	int startRow;
	int startCol;
	cin >> startRow;
	cin >> startCol;
	while(!verifyValidBegin(startRow, startCol, numRows, numColumns)){
		cout << "Please indicate your position by typing in {row column}" << "\n";
		cin >> startRow;
		cin >> startCol;
	};
	int mines=initializeBoard(numRows, numColumns, startRow, startCol);
	int init = displayBoard(startRow, startCol, numRows, numColumns);
	int totTimes = numRows * numColumns - mines-1-init;
	int i = 0;
	while(i<totTimes) {
		cout << "Please indicate your position by typing in {row column}" << "\n";
		int row;
		int column;
		cin >> row;
		cin >> column;
		if (verifyValidColRow(row, column, numRows, numColumns)) {
			i+=displayBoard(row, column, numRows, numColumns);
			lost = checkMine(row, column);
			
		}
		if (lost) {
			cout << "You have hit a mine and exploded. Please try again" << "\n";
			break;
		}
	}
	if (!lost) {
		cout<< "Congradulations! You have found all the mines!";
	}
}
