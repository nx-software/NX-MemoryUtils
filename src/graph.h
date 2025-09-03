/*
	NX-Software Memory Utilities
	graph.h - Terminal graphing
    Copyright (C) 2025 Electro-Corp

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <vector>

//
// Scrolling sequential graph
//
class Graph{
private:
    std::string gTitle, xTitle, yTitle;

    int xMax, yMax, yMin, yScale;

    std::vector<int> yVals;

    void setupAxis(int startingPoint);
public:
    Graph(std::string gTitle, std::string xTitle, std::string yTitle, int startValue);

    void addPoint(int y);

    void render();

};