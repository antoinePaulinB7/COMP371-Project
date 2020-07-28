Group 14

Nicole Parmentier - 40040213
Laura Wheatley - 40034960
Melissa Lim - 27195842
Antoine Paulin-Bessette - 40098918
Briana Toia - 40097889

******************** CONTROLS *******************
• Using WASD while holding a shift key moves the models in the world, and 
• Using WASD without holding a shift key rotates the models.

================ CAMERA ================
• The camera has preset positions to view each model or the whole scene:
∗ Use the [<] [>] keys (or really [,] or [.] ) to flip between the various views.
• The user can pan and tilt the camera as follows:
∗ While [Right Mouse Button] is pressed, use mouse movement in x direction to pan;
∗ While [Middle Mouse Button] is pressed, use mouse movement in y direction to tilt.
• The user can zoom in and out of the scene:
∗ While [Left Mouse Button] is pressed, use mouse movement inwards/outwards to zoom in/out.
• The world orientation is changed by using keyboard input:
∗ left arrow → R(x), right arrow → R(-x), up arrow → R(y), down arrow → R(-y).

=============== CONTROLS ===============
• [B] to toggle shadows on/off.
• [X] to toggle textures on/off.
∗ [N] to toggle moving forward continuously [M] to toggle moving backwards continuously.
∗ [C] to move forward [V] to move backwards incrementally.
∗ [Shift] + [W] to move up, [Shift] + [S] to move down,
  [Left Shift] + [Right Shift] + [A] to move left, and [Left Shift] + [Right Shift] + [D] to move right.
∗ [a] to rotate around (-y), [d] to rotate around (y), [w] to rotate around (-x), and [s] to rotate around (x).
∗ [U] to scale-up and [J] to scale-down the model.

• [Home] or [tab] button should reset to the initial world position and orientation.
• Change rendering mode for the model, i.e. points, lines, triangles based on keyboard input:
∗ [P] for points, [L] for lines, and [T] for triangles.

======= CONTROL INDIVIDUAL MODELS =======
• While HOLDING [1], [2], [3], [4], OR [5], individually control models by pressing the following keyboard inputs:
∗ [Spacebar] to re-position the model at a random location on the grid.
∗ [Shift] + [W] to move up, [Shift] + [S] to move down,
  [Left Shift] + [Right Shift] + [A] to move left, and [Left Shift] + [Right Shift] + [D] to move right.
∗ [a] to rotate around (-y), [d] to rotate around (y), [w] to rotate around (-x), and [s] to rotate around (x).
∗ [U] to scale-up and [J] to scale-down the model.

********************** ROLES *********************
  Melissa - Role 1:
• Illuminate the scene by adding a point light source (white) 30 units above the
  model using the Phong illumination model.
• Double-check that all project requirements are met and that all controls are functional.
• Package the submission according to specifications, with the required README file.

  Antoine - Role 2:
• Find the 3 textures to use in the project.
• Render tiled texture on the ground mesh, box texture on the alphabet, and shiny metal finish for the digit.
• Set key ‘X’ for toggling texture on/off.

  Nicole - Role 3:
• Render the scene with shadows using the two-pass shadow algorithm.
• Set key ‘B’ to toggle shadows.
• Window resize handling: Handle window resize events and correctly adjust the aspect ratio accordingly. 
  This means that the meshes should not be distorted in anyway.

  Briana & Laura Role 4 + 5:
• Make sphere model and import in project.
• Enclose the entire model in a big sphere using triangle mesh representations. 
  Having the lower half of the model outside the bubble with 1/3 of the model sticking out at the bottom.
• Make the model move by moving the segments touching the ground (lower half of the model), 
  while also providing either lateral/vertical movement to the upper half of the model. 
  One key of your choice for one small forward movement, and other key for continuous forward movement, 
  same for reverse movement.
• Make the model turn right or left by a small amount (choose your own keys and the amount).
• Pressing the spacebar repositions the model at a random location on the grid.