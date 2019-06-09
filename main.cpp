#include <iostream>
#include <string>

#include "Effects.h"

using namespace std;

int main(void)
{
	// variables for user input
	int selection;
	string input_file, output_file;

	// prompt user for input / output file names
	cout << "PPM Image Manipulator" << endl;
	cout << "Enter input file: ";
	cin >> input_file;
	cout << "Enter output file: ";
	cin >> output_file;

	do
	{
		// display choice of effects
		cout << "Image Effects Menu" << endl;
		cout << "1. Remove Red\t\t 2. Remove Green\t 3. Remove Blue\t\t 4. Negate Red" << endl;
		cout << "5. Negate Green\t\t 6. Negate Blue\t\t 7. Add Random Noise\t 8. High Contrast" << endl;
		cout << "9. Grayscale\t\t 10. Flip Horizontally\t 11. Flip Vertically\t 12. Rotate 90" << endl;
		cout << "13. Blur\t\t 14. Pixelate\t\t 15. Exit Program\t 16. Fly Vision" << endl;
		cout << "17. Remember that zebra gum whose flavor lasted for like 8 seconds?" << endl << endl;
		cout << "Selection: ";

		// make sure the user input for selection is valid
		cin >> selection;
		while (selection < 1 || selection > 18)
		{
			cout << "Invalid selection, make a selection between 1 and 16 ";
			cin >> selection;
		}

		// do the operation
		Effects image_to_process{ input_file, output_file };
		if (selection != 15)
		{
			image_to_process.applyEffect(selection);

			cout << "Effect applied" << endl << endl;
		}
		else
		{
			cout << "Exiting program" << endl;
		}

	} 
	while (selection != 15);
}