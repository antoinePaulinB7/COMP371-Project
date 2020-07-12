Group 14

Nicole Parmentier - 
Laura Wheatley - 40034960
Melissa - 
Antoine Paulin-Bessette -
Briana -


The controls in our project meet the specified requirements, with the following points of note:
• Using WASD while holding a shift key moves the models in the world, and using WASD without
	holding a shift key rotates the models.
• The camera has preset positions to view each model or the whole scene, use the < > keys 
	(or really , . ) to flip between the various views.


The functionality of the project was distributed into roles as follows:

Everyone:
• Create a letter & digit model by transforming a unit cube
• Organize the model using hierarchical modelling

Melissa - Role 1:
• 100x100 square grid (ground surface) in the XZ plane centered at the origin.
• Creates a set of three lines 5 grid units in length, in 3 different colors, representing
each coordinate axis in virtual world space, centered at the origin.

Laura - Role 2:
• The user can pan and tilt the camera as follows:
∗ While right button is pressed → use mouse movement in x direction to pan;
and
∗ While middle button is pressed → use mouse movement in y direction to tilt.
• The user can zoom in and out of the scene - while left button is pressed → use
mouse movement to move into/out of the scene.
• Use a perspective view and enable hidden surface removal.

Briana - Role 3:
• The world orientation is changed by using keyboard input i.e. left arrow → Rx,
right arrow → R-x, up arrow → Ry, down arrow → R-y. (Rx denotes a small anti-
clockwise rotation about positive x axis, R-x about negative x axis, etc.)
Pressing
• “Home” button should reset to the initial world position and orientation.
• The user can change rendering mode for the model, i.e. points, lines, triangles
based on keyboard input, namely, key ‘P’ for points, key ‘L’ for lines, key ‘T’ for
triangles.

Antoine - Role 4:
• The user can press ‘U’ to scale-up and ‘J’ to scale-down the model.
• The user can control the model position and orientation using keyboard input,
i.e., A → move left, D → move right, W → move up, S → move down, a → rotate
left 5 degrees about Y axis, d → rotate right 5 degrees about Y axis.

Nicole - Role 5:
• Create a GLFW window of size 1024x768 with double buffering support.
• Place the virtual camera with the world space origin as the point of focus, with
the coordinate axis, ground and all the models in the window.
• Place the models in the corners and center of the grid as described in the project.
• Implement preset camera positions to focus on each model or the whole scene
• Organize the work from the other Roles within the single project.
• Double-check that all project requirements are met.
• Package the submission according to specifications, with the required readme file.
