#include "graph.h"

Graph::Graph(std::string gTitle, std::string xTitle, std::string yTitle, int startValue){
    this->gTitle = gTitle;
    this->xTitle = xTitle;
    this->yTitle = yTitle;

    this->yScale = 10;

    setupAxis(startValue);
    addPoint(startValue);
}

void Graph::addPoint(int y){
    this->yVals.push_back(y);

    if(this->yVals.size() > xMax){
        yVals.erase(yVals.begin());
    }

    setupAxis(y);
}

void Graph::render(){
    printf("%s (%d, latest %d)\n", gTitle.c_str(), this->yVals.size(), yVals[yVals.size() - 1]);
    // Top border
    for(int i = 0; i < xMax + 2; i++) printf("=");
    printf(" (%s) \n", yTitle.c_str());
    // Each increment will be
    int inc = (yMax - yMin) / yScale;
    // Render graph
    for(int yR = yScale; yR > 0; yR--){
        printf("| ");
        for(int x = 0; x < xMax; x++){
            // Is there a value here
            if(this->yVals.size() > (xMax - x)){
                // is it at my y-value
                if(yVals[xMax - x] >= (yMin + (yR * inc - 1)) && yVals[xMax- x] <= yMin + (yR * (inc + 1))){
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
    // Bottom border
    for(int i = 0; i < xMax + 2; i++) printf("=");
    printf("\n");
}

//
// Internal funcs
//
void Graph::setupAxis(int startingPoint){
    if(startingPoint < 20){
        startingPoint = 25;
    }
    // Make it look somewhat even
    this->yMin = startingPoint - (startingPoint / 2);
    this->yMax = startingPoint + (startingPoint / 2);
    this->xMax = 50;
}