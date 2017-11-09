#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <vector>
#include <cmath>

using std::endl;
using namespace std;
using std::set;
using std::cout;
using std::unique_ptr;

// TODO: Your Sudoku class goes here:
class Sudoku: public Searchable{

private:

    vector<vector<set<int>>> incompleteGrid;
    set<int> allowedValues;
    int size;
    int boxSize;
    bool exists = false;

public:

    Sudoku(int sizeIn) : boxSize(sqrt(sizeIn)), size(sizeIn){
        //resize grid
        incompleteGrid.resize(size, vector<set<int>>(size));

        //store allowedValues
        for(int i=0; i< size; ++i) {
            allowedValues.insert(i+1);
        }
        //initialise grid
         for (int col=0; col<size; ++col) {
            for (int row=0; row< size; ++row) {
                incompleteGrid[row][col]=allowedValues;
            }
         }
    }



    int getSquare(int row, int col) {
        if(incompleteGrid[row][col].size()==1) {
            return *incompleteGrid[row][col].begin();
        } else {

            //cout << "BFFS goes here" << endl;
            return -1;
        }

    }

    bool setSquare(int row, int col, int value) {

        do {

            incompleteGrid[row][col].clear();
            incompleteGrid[row][col].insert(value);
            exists = true;


            for (int col=0; col<size; col++) {
                for (int row=0; row<size; row++) {

                    if (incompleteGrid[row][col].size()==1) {

                        if (!eraseRow(*incompleteGrid[row][col].begin(), exists, row, col)) {
                            return false;
                        }
                        if (!eraseCol(*incompleteGrid[row][col].begin(), exists, row, col)) {
                            return false;
                        }
                        if (!eraseBox(*incompleteGrid[row][col].begin(), exists, row, col)) {
                            return false;
                        }

                        if (incompleteGrid[row][col].size()==0) {
                            return false;
                        }

                    }

                }

            }

        } while (exists == false);

        return true;

    }

    virtual bool isSolution() const override {

        for (int col=0; col<size; col++) {
            for (int row=0; row<size; row++) {
                if (incompleteGrid[row][col].size()!=1) {
                    return false;
                }
            }
        }
        return true;
    }

    virtual vector<unique_ptr<Searchable>> successors() const override {

        vector<unique_ptr<Searchable>> tempVector;

        for (int row=0; row<size; ++row) {
            for(int col=0; col<size; ++col) {
                if (incompleteGrid[row][col].size()>1) {

                    for (int value: incompleteGrid[row][col]) {

                        Sudoku *copySudoku = new Sudoku(*this);
                        bool result = copySudoku->setSquare(row,col,value);
                        if (result) {
                            tempVector.push_back(unique_ptr<Searchable>(copySudoku));
                        }
                        else {
                            delete copySudoku;
                        }
                    }
                    return tempVector;
                }
            }
        }

    }

    virtual int heuristicValue() const override{
        int heuristicValue = 0;
            for(auto i = 0; i < size; ++i) {
                for(auto j = 0; j < size; ++j) {
                    if(incompleteGrid[i][j].size() > 1)
                        ++heuristicValue;
                }
            }
            return heuristicValue;

    }

    virtual void write(ostream & os) const override{
    os << "-------------------------------------------------------" << endl;
      for (int row = 0; row < size; row++) {
          for (int col=0; col< size; col++) {
              os << "| ";
              for (int element: incompleteGrid[row][col]) {
                  os << " :" << element << ": ";
                  if (!((col+1) % size))
                      os << " | ";
              }
              if (!((row+1) %boxSize))
                  os << endl << "-------------------------------------------------------" << endl;
              else
                  os << " ";
        }
      }
    }

    bool eraseRow(const int & rmValue, bool exists, const int row, const int col) {
        for (int index = 0; index < size; ++index) {
            if (index != col) {
                if (incompleteGrid[row][index].find(rmValue) != incompleteGrid[row][index].end()) {
                    incompleteGrid[row][index].erase(rmValue);
                    this->exists = false;

                    // for (int val : copyOfGrid) {
                    //     cout << val << ", ";
                    // }
                    // cout << endl;
                }
            }

            if (incompleteGrid[row][index].size() == 0) {
                return false;
            }
        }
        return true;
    }

    bool eraseCol(const int & rmValue, bool exists, const int row, const int col) {
        for (int index = 0; index < size; ++index) {
            if (index != row) {
                if (incompleteGrid[index][col].find(rmValue) != incompleteGrid[index][col].end()) {
                    incompleteGrid[index][col].erase(rmValue);
                    this->exists = false;
                    // for (int val : copyOfGrid) {
                    //     cout << val << ", ";
                    // }
                    // cout << endl;
                }
            }

            if (incompleteGrid[index][col].size() == 0) {
                return false;
            }
        }
        return true;
    }



    bool eraseBox(const int & rmValue, bool exists, const int row, const int col) {
        int endRow = row%boxSize;
        int startBoxRow = row - endRow;
        int endBoxRow = startBoxRow + boxSize;
        int endCol = col%boxSize;
        int startBoxCol = col - endCol;
        int endBoxCol = startBoxCol + boxSize;


        for (int i=startBoxRow; i<endBoxRow; i++) {
            for (int j=startBoxCol; j<endBoxCol; j++) {

                if (i!=row) {
                    if (j!=col) {
                        if(incompleteGrid[i][j].find(rmValue) != incompleteGrid[i][j].end()) {
                            incompleteGrid[i][j].erase(rmValue);
                            this->exists = false;

                            if (incompleteGrid[i][j].size() == 0) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }

};

#endif
