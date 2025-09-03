#include "graph.h"

Graph::Graph(std::string gTitle, std::string xTitle, std::string yTitle, int startValue){
    this->gTitle = gTitle;
    this->xTitle = xTitle;
    this->yTitle = yTitle;

    this->yScale = 10;

    addPoint(startValue);
    setupAxis(startValue);
}

void Graph::addPoint(int y){
    this->yVals.push_back(y);
}

void Graph::render(){
    printf("%s\n", gTitle.c_str());
    for(int i = 0; i < xMax + 2; i++) printf("=");
    printf("\n");
    // Each increment will be
    int inc = (yMax - yMin) / yScale;
    for(int yR = yScale; yR > 0; yR--){
        printf("| ");
        for(int x = 0; x < xMax; x++){
            // Is there a value here
            if(this->yVals.size() > x){
                // is it at my y-value
                if(yVals[yVals.size() - x] >= (yMin + (yR * inc - 1)) && yVals[yVals.size() - x] <= yMin + (yR * (inc + 1))){
                    printf("#");
                }else{
                    printf(" ");
                }
            }else{
                printf(" ");
            }
        }
        printf("| %d \n", yMin + (yR * inc));
    }
    for(int i = 0; i < xMax + 2; i++) printf("=");
    printf("\n");
}

//
// Internal funcs
//
void Graph::setupAxis(int startingPoint){
    // Make it look somewhat even
    this->yMin = startingPoint - (startingPoint / 2);
    this->yMax = startingPoint + (startingPoint / 2);
    this->xMax = 50;
}