
#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <cmath>

/* THIS CODE IS A WORK IN PROGRESS, DOESNT WORK */

struct Cell{
    int id;
    std::pair<int, int> v1, v2, v3, v4;
};

std::vector<std::vector<Cell>> CreateMesh(int rows, int cols){
    std::vector<std::vector<Cell>> array(rows, std::vector<Cell>(cols));
    return array;
}

void loop(std::vector<std::vector<Cell>> array, int rows, int cols){
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            array[i][j].id = (i * cols) + j;
            array[i][j].v1 = {i, j};
            array[i][j].v2 = {i + 1, j};
            array[i][j].v3 = {i + 1, j + 1};
            array[i][j].v4 = {i, j + 1 };
        }
    }
}

void display(const std::vector<std::vector<Cell>> &array){
    for(const auto& row : array){
        for(const auto& cell : row){
            std::cout << "Cell ID: " << cell.id << std::endl;
            std::cout << "Coordinates: (" << cell.v1.first << ", " << cell.v1.second << ") " ;
            std::cout << "(" << cell.v2.first << ", " << cell.v2.second << ") ";
            std::cout << "(" << cell.v3.first << ", " << cell.v3.second << ") ";
            std::cout << "(" << cell.v4.first << ", " << cell.v4.second << ") ";
            //printf("Cell ID: %d\n Coordinates: (%d, %d) (%d, %d) (%d, %d) (%d, %d)". cell.id, cell.v1.first, cell.v1.second, cell.v2.first, cell.v3.first, cell.v3.second, cell.v4.first, cell.v4.second);
        }
        printf("\n\n");
    }
}


int main(){
    int rows, cols;
    
    printf("Please Provide the Following Information\n\nRows: ");
    scanf("%d", &rows);
    
    printf("Columns: ");
    scanf("%d", &cols);
    printf("\n\n");

    // 1) Save Coordinates of the Mesh
    auto array = CreateMesh(rows,cols);
    
    // 2) Now print Coordinates
    //display(array);
    
    // 3) Loop and Just print for now
    loop(array,rows,cols);
    display(array);
}