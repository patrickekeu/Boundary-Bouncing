# Boundary-Bouncing
Draw a 2D object on the screen that bounces based on a drawn boundary

The object of this project is to allow the user to

1.  Draw a (small) 2-d object on the screen
2.  Draw a surrounding boundary around the object
3.  Have the smaller object start moving around the boundary, bouncing off the boundary walls when it hits them.  The object should both translate and rotate about its center.

Your project should have the following functionality:
- It  should  have  a  menu  that  containing  three  options: Define Boundary, Define Object, Start Movement.  
The first two allow you to define a boundary and object using a series of mouse clicks and the third starts the object in motion.
You should be able to use either of the first two options any time during the execution of the program (NOTE: If you don’t get this last functionality working, you will still get most of the points for the project).  You may assume that both the object and boundary are convex and the points are given in clock-wise order.

- Resizing the window should keep the boundary and objects in the lower left corner of the new window and allow you to redefine either the boundary or the object in the new window (this can be done easily with correct calls to gluOrtho2D and glViewport)
- Pressing 't' or 'r' should toggle the translation or rotation of the object on and off.  Pressing’q’should exit the program.
- Pressing the up arrow and down arrow should effect the speed of translation.  Pressing the right and left arrow keys should effect the speed of rotation.
- Your  code  should  make  use  of  the  OpenGL  transformation  methods _glTranslatef_ and _glRotatef_ to move the object.
- While you can rely on glTranslatef and glRotatef to move object correctly, you will need to determine the locations of the transformed vertices in order to determine when they hit or go beyond the boundary. For this, you need access to the transformation matrix, so you can perform the matrix-vertex multiplication by hand. OpenGL has a generic method to obtain a variety of information about the current state, including the transformation matrix.  The call to get this is

  double matrix[16];
  
  ...
  
  glGetDoublev(GL_MODELVIEW_MATRIX, matrix);

  After this call, matrix will store the 16 values in the 4×4 transformation matrix in column-major order.

