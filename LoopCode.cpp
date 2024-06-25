
#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <cmath>

int main(){
    int rows, cols;
    
    printf("Please Provide the Following Information\n\nRows: ");
    scanf("%d", &rows);
    
    printf("Columns: ");
    scanf("%d", &cols);
    printf("\n\n");
    std::pair<int, int> mesh[cols][rows];

    // 1) Save Coordinates of the Mesh
    for (int i = 0; i < rows; ++i){
        for(int j = 0; j < cols ; ++j){
            mesh[j][i] = {i,j};
            printf("(%d, %d)", mesh[i][j].first, mesh[i][j].second);
        }
    }
    
    // 2) Now print Coordinates
    // for(int i = 0; i < rows; ++i){
    //     for(int j = 0; j < cols; ++j){
    //         printf("(%d, %d)", mesh[i][j].first, mesh[i][j].second);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    
    // 3) Loop and Just print for now
}