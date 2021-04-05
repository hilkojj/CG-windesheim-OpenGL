#pragma once


namespace key_input
{

	// callback for GLUT
	void keyPressedHandler(unsigned char key, int, int);

	// callback for GLUT
	void keyUpHandler(unsigned char key, int, int);

	// should be called at end of render() function, to update key statuses.
	void update();

	bool justPressed(unsigned char key);

	bool pressed(unsigned char key);

	bool justReleased(unsigned char key);

};

