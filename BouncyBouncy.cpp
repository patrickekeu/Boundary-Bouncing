/*
Name(s): Patrick Ekeu
Date: March 28, 2024
Project 2: Bouncy Bouncy
Description: Draw a (small) 2-d object on the screen
             Draw a surrounding boundary around the object
             Have the smaller object start moving around the boundary,
             bouncing off the boundary walls when it hits them. The object
             should both translate and rotate about its center.
*/

#include <GL/glut.h> // OpenGL Utility Toolkit header
#include <vector> // Include vector for using std::vector
#include <cmath> // Include cmath for sqrt function

// Point class definition
class Point {
private:
    float x;
    float y;

public:
    // Constructor
    Point(float xCoord = 0.0f, float yCoord = 0.0f) : x(xCoord), y(yCoord) {}

    // Getter methods
    float getX() const { return x; }
    float getY() const { return y; }

    // Setter methods
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
};

// The coordinates of the boundary and object vertices
std::vector<Point> boundary; // Stores the boundary vertices
std::vector<Point> object; // Stores the object vertices

// Flags for translation and rotation
int translate = 0; // Flag to control translation
int rotate = 0; // Flag to control rotation

// The coordinates of the object
float x = 0.0f; // Object's x-coordinate
float y = 0.0f; // Object's y-coordinate

// The direction of movement
float dx = 0.02f; // Change in x per frame
float dy = 0.03f; // Change in y per frame

// Rotation angle
float angle = 0.0f; // Current rotation angle
float speed = 2.0f; // Rotation speed

int mode = 0; // Mode for defining boundary or object vertices

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // Draw the boundary
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    glBegin(GL_LINE_LOOP); // Begin drawing line loop
    for (const Point& point : boundary) { // Iterate through boundary points
        glVertex2f(point.getX(), point.getY()); // Set vertex
    }
    glEnd(); // End drawing

    // Draw the object
    glPushMatrix(); // Push current matrix onto stack
    // Translate the object to the center of its bounding box
    float centerX = 0.0f, centerY = 0.0f; // Initialize center coordinates
    for (const Point& point : object) { // Calculate center of object
        centerX += point.getX();
        centerY += point.getY();
    }
    centerX /= object.size(); // Average x-coordinate
    centerY /= object.size(); // Average y-coordinate
    glTranslatef(x, y, 0.0f); // Translate to current object position
    glRotatef(angle, centerX, centerY, 1.0f); // Rotate around the z-axis
    glTranslatef(-centerX, -centerY, 0.0f); // Translate back to original position
    glColor3f(1.0f, 0.0f, 0.0f); // Set color to red
    glBegin(GL_POLYGON); // Begin drawing polygon
    for (const Point& point : object) { // Iterate through object points
        glVertex2f(point.getX(), point.getY()); // Set vertex
    }
    glEnd(); // End drawing
    glPopMatrix(); // Pop matrix from stack

    glutSwapBuffers(); // Swap buffers
}

// Timer function
void timer(int) {
    // Update the position of the object
    if (translate) {
        // Store the new position of the object
        float newX = x + dx;
        float newY = y + dy;

        // Check for collision with each boundary line segment
        for (size_t i = 0; i < boundary.size(); ++i) {
            size_t nextIndex = (i + 1) % boundary.size();
            Point p1 = boundary[i];
            Point p2 = boundary[nextIndex];

            // Check collision with line segment (p1, p2)
            float minX = std::min(p1.getX(), p2.getX());
            float maxX = std::max(p1.getX(), p2.getX());
            float minY = std::min(p1.getY(), p2.getY());
            float maxY = std::max(p1.getY(), p2.getY());

            // Check if the object's bounding box intersects with the line segment's bounding box
            if (newX >= minX && newX <= maxX && newY >= minY && newY <= maxY) {
                // Calculate the normal vector of the line segment (p1, p2)
                float segmentNormalX = p2.getY() - p1.getY();
                float segmentNormalY = -(p2.getX() - p1.getX());

                // Normalize the normal vector
                float length = std::sqrt(segmentNormalX * segmentNormalX + segmentNormalY * segmentNormalY);
                segmentNormalX /= length;
                segmentNormalY /= length;

                // Calculate the dot product of the direction vector and the normal vector
                float dotProduct = dx * segmentNormalX + dy * segmentNormalY;

                // Calculate the reflected direction using the equation r = d - 2(d ⋅ n)n
                float reflectedDX = dx - 2 * dotProduct * segmentNormalX;
                float reflectedDY = dy - 2 * dotProduct * segmentNormalY;

                // Update the direction after collision
                dx = reflectedDX;
                dy = reflectedDY;

                // Move the object to the point of collision to prevent it from getting stuck
                newX = x + dx;
                newY = y + dy;

                // Break the loop after detecting one collision
                break;
            }
        }

        // Update the object's position after collision handling
        x = newX;
        y = newY;
    }

    // Update the rotation angle
    if (rotate) {
        angle += speed;
        if (angle > 360.0f) angle -= 360.0f;
    }

    glutPostRedisplay(); // Mark the current window for redisplay
    glutTimerFunc(1000 / 60, timer, 0); // 60 FPS
}

// Reshape function
void reshape(int w, int h) {
    glViewport(0, 0, w, h); // Set viewport
    glMatrixMode(GL_PROJECTION); // Set matrix mode to projection
    glLoadIdentity(); // Load identity matrix
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // Set orthographic projection
    glMatrixMode(GL_MODELVIEW); // Set matrix mode to modelview
}

// Keyboard function
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 't':
            translate = !translate; // Toggle translation
            break;
        case 'r':
            rotate = !rotate; // Toggle rotation
            break;
        case 'q':
            exit(0); // Exit program
    }
}

// Special keys function
void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP:
            dx *= 1.1f; // Increase velocity in y-direction
            dy *= 1.1f; // Increase velocity in x-direction
            break;
        case GLUT_KEY_DOWN:
            dx *= 0.9f; // Decrease velocity in y-direction
            dy *= 0.9f; // Decrease velocity in x-direction
            break;
        case GLUT_KEY_LEFT:
            speed *= 0.9f; // Decrease rotation speed
            break;
        case GLUT_KEY_RIGHT:
            speed *= 1.1f; // Increase rotation speed
            break;
    }
}

// Menu function
void menu(int item) {
    switch (item) {
        case 1:
            // Clear Boundary
            mode = 1; // Set mode to define boundary
            translate = 0; // Disable translation
            rotate = 0; // Disable rotation
            boundary.clear(); // Clear boundary vector
            object.clear(); // Clear object vector
            break;
        case 2:
            // Clear Object
            mode = 2; // Set mode to define object
            translate = 0; // Disable translation
            rotate = 0; // Disable rotation
            object.clear(); // Clear object vector
            break;
        case 3:
            // Start Movement
            translate = 1; // Enable translation
            break;
    }
    glutPostRedisplay(); // Mark the current window for redisplay
}

// Create menu function
void createMenu() {
    glutCreateMenu(menu); // Create menu
    glutAddMenuEntry("Define Boundary", 1); // Add menu entry for defining boundary
    glutAddMenuEntry("Define Object", 2); // Add menu entry for defining object
    glutAddMenuEntry("Start Movement", 3); // Add menu entry for starting movement
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Attach menu to right mouse button
}

// Mouse function
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) { // If mouse button is pressed
        float posX = (float)x / glutGet(GLUT_WINDOW_WIDTH) * 2.0f - 1.0f; // Convert x-coordinate to OpenGL coordinates
        float posY = 1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT) * 2.0f; // Convert y-coordinate to OpenGL coordinates
        if (mode == 1) { // If defining boundary
            boundary.emplace_back(posX, posY); // Add point to boundary vector
        } else if (mode == 2) { // If defining object
            object.emplace_back(posX, posY); // Add point to object vector
        }
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutCreateWindow("OpenGL Project 2: Bouncy Bouncy"); // Create window with title
    glutDisplayFunc(display); // Set display callback function
    glutReshapeFunc(reshape); // Set reshape callback function
    glutKeyboardFunc(keyboard); // Set keyboard callback function
    glutSpecialFunc(specialKeys); // Set special keys callback function
    glutMouseFunc(mouse); // Set mouse callback function
    glutTimerFunc(0, timer, 0); // Set timer callback function
    createMenu(); // Create menu
    glutMainLoop(); // Enter GLUT event processing loop
    return 0; // Return success
}
