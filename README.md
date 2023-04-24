# 3DViewer v1.0

My own realization of 3DViewer (using C language for back and C++ for front) for Mac and Linux.

For build do make build (need to have qmake and core, gui, widgets, opengl modules for QT on Linux or core, gui, widgets, opengl, openglwidgets modules on Mac)


## How to install

For install do make install (this will create a directory 3DViewer_v1.0 with the application on your Desktop).


## How to uninstall

For uninstall do make uninstall (this will delete a directory 3DViewer_v1.0 with the application on your Desktop).


## Realized features 

The application provides a graphical 3DViewer with support for the following features:

1. Load a wireframe model from an obj format file (only vertexes and edges list support).

2. Move the model by a specified distance relative to the X, Y, Z axes.

3. Rotate the model by a specified angle relative to its X, Y, Z axes.

4. Scale the model by a specified value.

5. Choose the type of projection (parallel and central).

6. Choose the type (solid, dotted), color and thickness of the edges.

7. Choose the display method (absent, circle, square), color, and size of vertices.

8. Choose the background color.

9. Save the obtained ("rendered") images to a file in bmp and jpeg formats.

10. Record small "screencasts" as gif-animation.


## Manual:

1. To load a wireframe model from an obj file, click on menubar File->Open and chose file *.obj.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.50.30.png?raw=true)

2. To save the settings (projection type, type (solid, dotted), color and thickness of edges, display method (absent, circle, square), color and size of vertices, background color) click on menubar Settings->Save. Note: The settings will be saved automatically between program restarts and open new file.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.51.14.png?raw=true)

3. You can also load previously used settings simply by pressing on menubar Settings->Load, if you ran the application earlier.

4. In the menubar, select Media->Projection type: Parallel or Central.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.52.33.png?raw=true)

5. In the Edges menu, specify the thickness of the edges.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.53.33.png?raw=true)

6. In the Vertices menu, you can choose the vertex type (circle or square), as well as the vertex size.

7. In the Color menubar, you can choose the color of the vertices, edges, and background.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.51.39.png?raw=true)

8. In the  menubar Media->Type line you can choose the type of line (dotted or solid) for the edges.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.52.50.png?raw=true)

9. To move the model relative to the X, Y, Z axes, use the sliders or enter a value in the corresponding field in the Move menu.

10. To scale the model, use the slider "-/+" or enter a value in the corresponding field in the Move menu.

11. To rotate the model use Dial X, Y, Z axes or enter a value in the corresponding field.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.53.16.png?raw=true)

12. To create a screenshot, press on menubar Media->Save screenshoot. You can find the screenshot in the "media" folder at the root of the project.

13. In the menubar Media->Screenshot format, you can specify the file format (jpeg or bmp) to save the screenshot.

![](src/images/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202023-02-14%20%D0%B2%2010.52.10.png?raw=true)

14. To record a gif animation, press on menubar Media->Record GIF. Then within 5 seconds your gif-animation will be recorded. You can find the Gif animations in the "media" folder in the root of the project.

15. To close the program press on menubar File->Exit or press the red button in the upper left corner of the program.

