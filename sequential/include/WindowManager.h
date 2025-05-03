#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <string>

#include <Vector.h>
#include <Average.h>
#include <Texture.h>
#include <DoubleBuffer.h>

/**
 * @brief Manages the window operations.
 *
 * The WindowManager class provides functionality for creating and managing a window,
 * handling user input, and updating the frames per second (FPS).
 * It encapsulates the X11 window system and provides an interface to interact with it.
 */
class WindowManager
{
public:
    /**
     * @brief Constructs a WindowManager object.
     * @param doubleBuffer The double buffer used to draw to the window.
     */
    WindowManager(DoubleBuffer &doubleBuffer);

    /**
     * @brief Destructor for the WindowManager object.
     */
    ~WindowManager();

    /**
     * @brief Retrieves the current keys pressed.
     * @return The current keys pressed.
     */
    unsigned int getKeysPressed();

    /**
     * @brief Updates the window display with the back buffer from the double buffer.
     */
    void updateDisplay();

    /**
     * @brief Updates the internal key state with the user input.
     */
    void updateInput();

    static unsigned int const KEY_UP = (1 << 0);    // Bit mask for the up key (arrow).
    static unsigned int const KEY_DOWN = (1 << 1);  // Bit mask for the down key (arrow).
    static unsigned int const KEY_RIGHT = (1 << 2); // Bit mask for the right key (arrow).
    static unsigned int const KEY_LEFT = (1 << 3);  // Bit mask for the left key (arrow).
    static unsigned int const KEY_ESC = (1 << 4);   // Bit mask for the escape key.

private:
    DoubleBuffer &doubleBuffer; // The double buffer used to draw to the window.

    int *imgBuffer; // The buffer for the window image.
    int width;      // The width of the window.
    int height;     // The height of the window.
    XImage *img;    // The X11 image for the window.

    int screen;       // The screen number of the window.
    Display *display; // The display of the window.
    Window window;    // The window.
    GC gc;            // The graphics context of the window.

    unsigned int keysPressed; // The current state of the keys (which keys are pressed or not pressed).

    /**
     * @brief Converts a KeySym to one of the bit masks.
     * @param key The KeySym to convert.
     * @return The converted bit mask.
     */
    unsigned int convertKey(KeySym key);
};

#endif