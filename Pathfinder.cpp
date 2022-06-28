#include "Pathfinder.h"
  
	string Pathfinder::toString() const
	{
    int rowCounter = 0;
    int lvlCounter = 0;
    if (onoff == true) {
      stringstream ss;
      for(int lvl = 0; lvl < LVL_SIZE; lvl++) {
        lvlCounter++;
			  for(int row = 0; row < ROW_SIZE; row++) {
          for(int col = 0; col < COL_SIZE; col++) {
            rowCounter++;
				    ss << 1;
            if (rowCounter < 5) {
            ss << " ";
            }
            else {
              rowCounter = 0;
            }
			    }
			    ss << endl;
	      }
        if (lvlCounter < 5) {
        ss << endl;
        }
      }
      return ss.str();
    }
    rowCounter = 0;
    lvlCounter = 0;
	    stringstream ss; 
	    for(int lvl = 0; lvl < LVL_SIZE; lvl++) {
        lvlCounter++;
			  for(int row = 0; row < ROW_SIZE; row++) {
          for(int col = 0; col < COL_SIZE; col++) {
            rowCounter++;
				    ss << maze_grid[lvl][row][col];
            if (rowCounter < 5) {
            ss << " ";
            }
            else {
              rowCounter = 0;
            }
			    }
			    ss << endl;
	      }
        if (lvlCounter < 5) {
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
	void Pathfinder::createRandomMaze()
	{
    srand ( time(NULL));
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 5; k++) {
          if (i == 0 && j == 0 && k == 0) {
            maze_grid[i][j][k] = 1;
          }
          else if (i == 4 && j == 4 && k == 4) {
            maze_grid[i][j][k] = 1;
          }
          else {
            maze_grid[i][j][k] = rand() % 2;
          }
        }
      }
    }
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
	bool Pathfinder::importMaze(string file_name)
	{
    onoff = false;
		cout << "importMaze from "<<file_name<<endl;
		ifstream file (file_name.c_str());
    ifstream file_2 (file_name.c_str());
    vector<string> ints;
		if (file.is_open()) {
			string line;
			int row_count = 0;

    while(!file_2.eof()) {
      string next;
      file_2 >> next;
      if (next == "0" || next == "1"){
        ints.push_back(next);
      }
    }
    if (ints.size() != 125) {
      return(false);
    }
    if (ints.at(124) != "1") {
      return(false);
    }

    for(int i = 0; i < LVL_SIZE; i++) {
      for(int j = 0; j < ROW_SIZE; j++) {
        getline(file, line);
        if (line == "") {
          getline(file, line);
        }
        stringstream ss(line);
          for(int k = 0; k < COL_SIZE; k++) {
            int value;
            ss >> value;
            //cout << "["<<i<<"]["<<j<<"]["<<k<<"]="<<value<<endl;
            maze_grid[i][j][k] = value;
          }
        }  
      }
    }
    else {
      return(false);
    }
	  return(true);
  }
	//----------------------------------------------------------------------------------------
	
	
	bool Pathfinder::find_maze_path(int grid[LVL_SIZE][ROW_SIZE][COL_SIZE], int l, int r, int c) {
	  //cout << "find_maze_path ["<<r<<"]["<<c<<"]["<<l<<"]" <<endl;
    //cout << this->toString();
	  if (r < 0 || c < 0 || l < 0 || r >= ROW_SIZE || c >= COL_SIZE || l >= LVL_SIZE)
	    return false;      // Cell is out of bounds.
	  else if (grid[l][r][c] != BACKGROUND)
	    return false;      // Cell is on barrier or dead end.
	  else if (r == ROW_SIZE - 1 && c == COL_SIZE - 1 && l == LVL_SIZE - 1) {
	    grid[l][r][c] = PATH;         // Cell is on path
	    solution.push_back("("+to_string(l)+", "+to_string(r)+", "+to_string(c)+")");
	    return true;               // and is maze exit.
	  }
	  else { 
	    // Recursive case.
	    // Attempt to find a path from each neighbor.
	    // Tentatively mark cell as on path.
	    grid[l][r][c] = PATH;
	    if (find_maze_path(grid, l, r - 1 , c)
	        || find_maze_path(grid, l, r + 1, c)
	        || find_maze_path(grid, l, r, c - 1)
	        || find_maze_path(grid, l, r, c + 1)
          || find_maze_path(grid, l - 1, r, c)
          || find_maze_path(grid, l + 1, r, c) ) {
	      solution.push_back("("+to_string(l)+", "+to_string(r)+", "+to_string(c)+")");
	      return true;
	    }
	    else {
	      grid[l][r][c] = TEMPORARY;  // Dead end.
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
    solution.clear();
    int maze_temp[LVL_SIZE][ROW_SIZE][COL_SIZE];
    for(int i = 0; i < LVL_SIZE; i++) {
      for(int j = 0; j < ROW_SIZE; j++) {
          for(int k = 0; k < COL_SIZE; k++) {
            maze_temp[i][j][k] = maze_grid[i][j][k];
          }
        }  
      }
		find_maze_path(maze_temp, 0,0,0);
		for(auto s:solution) {
//			cout <<s<<endl;
		}
    reverse(solution.begin(), solution.end());
	  return solution;
	}
