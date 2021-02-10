#include "Pathfinder.h"

/*
 * @param L - A pointer to an int array
 * @param min - The minimum possible index to find n (inclusive) (starts at 0)
 * @param max - The maximum possible index to find n (exclusive) (starts at length of L)
 * @param n -  The value to find
 * @return - True if found, false otherwise
 */

string Pathfinder::toString() const {
		stringstream ss;
		
		for(int hei = 0; hei < HEIGHT_SIZE; hei++) {
			for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					if (col == 4) {
						ss << maze_grid[row][col][hei];
					}
					else {
						ss << maze_grid[row][col][hei] << " ";
					}
				}
				ss << endl;
			}	
			if (hei < 4) {
				ss << endl;
			}
		}
	return ss.str();	
}

	/*
	* createRandomMaze
	*
	* Generates a random maze and stores it as the current maze.
	*
	* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
	* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
	* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
	*/
	void Pathfinder::createRandomMaze() {
		for(int hei = 0; hei < HEIGHT_SIZE; hei++) {
			for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					maze_grid[row][col][hei] = rand()% 2;
					// cout << maze_grid[row][col][hei];
				}
				// cout << endl;
			}	
			// cout << endl;
		}
		maze_grid[0][0][0] = 1;
		maze_grid[4][4][4] = 1;
	}
	//-----------------------------------------------------------------------------------------
	
	//Part 2-----------------------------------------------------------------------------------
	/*
	* importMaze
	*
	* Reads in a maze from a file with the given file name and stores it as the current maze.
	* Does nothing if the file does not exist or if the file's data does not represent a valid
	* maze.
	*
	* The file's contents must be of the format described above to be considered valid.
	*
	* Parameter:	file_name
	*				The name of the file containing a maze
	* Returns:		bool
	*				True if the maze is imported correctly; false otherwise
	*/
	bool Pathfinder::importMaze(string file_name) {
		cout << "importMaze from "<<file_name<<endl;
		ifstream file (file_name.c_str());

		int test_maze[ROW_SIZE][COL_SIZE][HEIGHT_SIZE];

		int count = 0;
		if (file.is_open()) {
			string line;
			for(int hei = 0; hei < HEIGHT_SIZE; hei++) {
				for(int row = 0; row < ROW_SIZE; row++) {
						getline(file, line);
						if (file.bad() || file.fail()) {
							count += 1000;
						}
						for (int i = 0; i < line.size(); i++){
							if (!isdigit(line.at(i)) && line.at(i) != ' ') {
								count += 1000;
							}
						}
				
						stringstream ss(line);
						
						for(int col = 0; col < COL_SIZE; col++) {
							int value;
							ss >> value;
							if (value == 0 || value == 1) {
									count++;
							// cout << "["<<row<<"]["<<col<<"]["<<hei<<"]="<<value<<endl;
								test_maze[row][col][hei] = value;
							}
							else {
								cout << "mazes contains some number other than 0 and 1\n";
								count += 1000;
							}
						}
				}
						file.ignore();
			} 
			if (getline(file, line)) {
				count += 1000;
			}
				
			if (test_maze[0][0][0] != 1 || test_maze[4][4][4] != 1 || count != 125) {
					return false;
				}
			else {
				for(int hei = 0; hei < HEIGHT_SIZE; hei++) {
					for(int row = 0; row < ROW_SIZE; row++) {
						for(int col = 0; col < COL_SIZE; col++) {
								maze_grid[row][col][hei] = test_maze[row][col][hei];
							}			
					}	
				} 
			}
		}

		else {
			cout << "file is not open \n";
			return false;
		}
	}
	//-----------------------------------------------------------------------------------------

	
	
	bool Pathfinder::find_maze_path(int grid[ROW_SIZE][COL_SIZE][HEIGHT_SIZE], int r, int c, int h) {
	  cout << "find_maze_path ["<<r<<"]["<<c<<"]["<<h<<"]"<<endl;
	  cout << this->toString();
	  if (r < 0 || c < 0 || h < 0 || r >= ROW_SIZE || c >= COL_SIZE || h >= HEIGHT_SIZE)
	    return false;      // Cell is out of bounds.
	  else if (grid[r][c][h] != BACKGROUND)
	    return false;      // Cell is on barrier or dead end.
	  else if (r == ROW_SIZE - 1 && c == COL_SIZE - 1 && h == HEIGHT_SIZE - 1) {
	    grid[r][c][h] = PATH;         // Cell is on path
	    solution.push_back("("+to_string(r)+","+to_string(c)+","+to_string(h)+")");
	    return true;               // and is maze exit.
	  }
	  else { 
	    // Recursive case.
	    // Attempt to find a path from each neighbor.
	    // Tentatively mark cell as on path.
	    grid[r][c][h] = PATH;
	    if (find_maze_path(grid, r - 1, c, h)
	       || find_maze_path(grid, r + 1, c, h)
	       || find_maze_path(grid, r, c - 1, h)
	       || find_maze_path(grid, r, c + 1, h ) 
				 || find_maze_path(grid, r, c , h + 1)
				 || find_maze_path(grid, r, c , h - 1)) {
	      solution.push_back("("+to_string(r)+","+to_string(c)+","+to_string(h)+")");
	     return true;
	    }
	    else {
	     grid[r][c][h] = TEMPORARY;  // Dead end.
	     return false;
	   }
	  }
	}
	  //Part 3-----------------------------------------------------------------------------------
	/*
	* solveMaze
	*
	* Attempts to solve the current maze and returns a solution if one is found.
	*
	* A solution to a maze is a list of coordinates for the path from the entrance to the exit
	* (or an empty vector if no solution is found). This list cannot contain duplicates, and
	* any two consecutive coordinates in the list can only differ by 1 for only one
	* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
	* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
	* where x, y, and z are the integer coordinates of a cell.
	*
	* Understand that most mazes will contain multiple solutions
	*
	* Returns:		vector<string>
	*				A solution to the current maze, or an empty vector if none exists
	*/
	
	vector<string> Pathfinder::solveMaze()
	{
		find_maze_path(maze_grid, 0, 0, 0);
		for(auto s:solution) {
			cout <<s<<endl;
		}
	    return solution;
	}