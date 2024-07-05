# GIS App
This project is a Geographic Information System (GIS) application developed in C++ using the Qt framework with OpenGL integration. 
The application aims to provide spatial data visualization and analysis capabilities. 
It utilizes Qt for its user interface components and OpenGL for rendering geographical data in a 2D context. 
The application supports reading and displaying GIS data shapefiles formats.
Key features include map rendering, layer management, scaling.


<img src="https://github.com/Anastasiia-Ni/RegGIS/blob/main/img/map.png" width="600">
<img src="https://github.com/Anastasiia-Ni/RegGIS/blob/main/img/table.png" width="600">


## Introduction
This repository contains the source code for the RegGIS application. RegGIS is designed for geographical information systems (GIS) 
and provides capabilities to load and displayspatial data.

## Features
- Layer Management: Allows loading and managing different types of geographical layers including points, lines, and polygons.
- Visualization: Renders spatial data using OpenGL for efficient and interactive visualization.
- Layer Styling: Supports styling of layers with customizable colors, transparency, and line widths.
- User Interaction: Enables user interaction such as panning, zooming, and toggling layer visibility.

## Requirements
- Qt Framework: Version 5.12 or higher.
- GDAL Library: Used for reading and manipulating geospatial data formats.

## Installation
1. Clone the repository:
``` git clone https://github.com/your-username/RegGIS.git ```

2. Build the project:

- Open the RegGIS.pro project file in Qt Creator.
- Configure the project for your development environment.
- Build and compile the project.

## Usage
1. Launching the Application:
- Open the built executable or run from Qt Creator.
2. Loading Layers:
- Сopy your layers (*.shp format) to the "layers" folder instead of those that are there for example.
- Rewrite the [Layers.json](https://github.com/Anastasiia-Ni/RegGIS/blob/main/add/Layers.json) file in accordance with the names of the layers and the desired display style.
3. Interacting with the Map:
- Panning: Click and drag with the left mouse button.
- Zooming: Double-click to zoom in at the cursor position.
- Layer Visibility: Toggle layer visibility using the layer list dialog.
4. Opening Layer Tables:
- Use the `Open Table` action from the `Table` menu or the toolbar to select the layer's attribute table.

<img src="https://github.com/Anastasiia-Ni/RegGIS/blob/main/img/TableDialog.png" width="300">

## Structure
- `mainwindow.cpp`: Contains the main window logic and user interface setup.
- `map.h` and `map.cpp`: Define the Map class responsible for managing GIS data and interactions.
- `mywidget.cpp`: Implements OpenGL widget for rendering the map.
- `layersinfo.h` and `layersinfo.cpp`: Define the layersInfo class for handling layer styling information.
- Other files: Additional utility functions and resources used in the application.
