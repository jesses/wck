OVERVIEW:

There are two .jsfl files provded with this project:

1. Polygon Decomposition - Get Points.jsfl
2. Polygon Decomposition - Clear Outline.jsfl

The first command - "Get Points" - will extract vertex information from shapes drawn within the Flash IDE and decompose those shapes into covex polygons that can be used with Box2d. Once run, the decomposition will both be:

a) drawn to the screen for instant visual feedback of how the shape was decomposed
b) printed to the output panel (for easy copy-pasting into code).

The second command - "Clear Outline" - will remove the decomposition outline from the screen.

INSTALLATION:

To install the *.jsfl files, copy them to the appropriate folder depending on your operating system:

Vista:
*boot drive*\Users\*username*\Local Settings\Application Data\Adobe\Flash CS3\*language*\Configuration\Commands/

XP:
*boot drive*\Documents and Settings\*username*\Local Settings\Application Data\Adobe\Flash CS3\*language*\Configuration\Commands/

Mac OS X:
*boot drive*/Users/*username*/Library/Application Support/Adobe/Flash CS3/*language*/Configuration/Commands/

Once installed, you can run these commands from the "commands" menu within Flash.

USAGE:

1. Draw a shape within Flash, preferably with a tool that creates straight edges (the Pen Tool, for example).

2. Select your entire shape.

3. Run the "Polygon Decomposition - Get Points" command (which should be under the "Commands" menu).

4. The decomposition will be printed to the output panel. Copy the output into your Shape class (see wck/DemoPoly.as for an example).

5. Run the "Clear Outline" command to clear the outline from the stage.