#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "StringSplitter.h"
#include <random>
#include <time.h>

class Effects
{
private:
	string _input_file, _output_file, _magic_number, _dimensions, _max_brightness, _length_out, width_out;
	int _selection, _length_in, _width_in;
	vector<string> _ppm_in_file, _dimensions_as_vector;
	vector<string> _table;

	vector<string> readFile(ifstream& some_file)
	{
		vector<string> result{};
		//is the file open?
		if (some_file.is_open() == true)
		{
			while (some_file.good() == true)
			{
				//two ways to grab data
				string data;
				//first_names >> data; //CIN style
				getline(some_file, data); //more robust method
				result.push_back(data);
			}
		}
		return result;
	}
	vector<string> readFile(ifstream&& some_file)
	{
		ifstream& local_file = some_file;

		//auto is an automatically inferred variable type
		//in our case, vector<string>
		auto result = readFile(local_file);
		local_file.close();
		return result;
	}
	vector<string> readFile(string file_name)
	{
		return readFile(ifstream{ file_name });
	}

public:
	Effects(string input_file, string output_file)
	{
		_input_file = input_file;
		_output_file = output_file;
		_ppm_in_file = readFile(_input_file);
		//_ppm_out_file = ofstream(_output_file);
	}

	void getHeader()
	{
		_dimensions_as_vector = StringSplitter::split(_ppm_in_file[1], " ");

		_magic_number = _ppm_in_file[0];
		_width_in = stoi(_dimensions_as_vector[0]);
		_length_in = stoi(_dimensions_as_vector[1]);
		_max_brightness = _ppm_in_file[2];
	}

	//void writeHeader()
	//{
	//	getHeader();
	//	ofstream ppm_out_file(_output_file);
	//	ppm_out_file << _magic_number << endl;
	//	ppm_out_file << _dimensions << endl;
	//	ppm_out_file << _max_brightness << endl;
	//}

	// function to apply the desired effect
	void applyEffect(int selection)
	{
		// get the header so that it can be written in the new file
		getHeader();
		switch (selection)
		{
			case 1:
				removeColor(0);
				break;
			case 2:
				removeColor(1);
				break;
			case 3:
				removeColor(2);
				break;
			case 4: 
				negateColor(0);
				break;
			case 5:
				negateColor(1);
				break;
			case 6:
				negateColor(2);
				break;
			case 7:
				randomNoise();
				break;
			case 8:
				highContrast();
				break;
			case 9:
				grayScale();
				break;
			case 10:
				flipHorizontal();
				break;
			case 11:
				flipVertical();
				break;
			case 12:
				rotate90();
				break;
			case 13:
				blur();
				break;
			case 14:
				pixelate();
				break;
			case 15:
				break;
			case 16:
				flyVision();
				break;
			case 17:
				zebraGumVision();
				break;
			default:
				break;
		}
	}

	// different swap functions that might be useful
	void swap(string& first, string& second)
	{
		string temp;
		temp = first;
		first = second;
		second = temp;
	}
	void swap(vector<string>& first, vector<string>& second)
	{
		vector<string> temp;
		temp = first;
		first = second;
		second = temp;
	}
	void swap(int& first, int& second)
	{
		int temp;
		temp = first;
		first = second;
		second = temp;
	}

	// method to create a table of color values
	vector<vector<int>> makeTable()
	{
		vector<vector<int>> table;
		table.push_back(vector<int>{});
		int counter = 0;
		int row = 0;
		for (unsigned int i = 3; i < _ppm_in_file.size(); i++)
		{
			vector<string> pieces = StringSplitter::split(_ppm_in_file[i], " ");
			for (auto piece : pieces)
			{
				if (piece != "")
				{
					try
					{
						table[row].push_back(stoi(piece));
						counter++;
						if (counter == _width_in * 3)
						{
							counter = 0;
							row++;
							table.push_back(vector<int>{});
						}
					}
					catch (...)
					{
					}
				}
			}
		}
		return table;
	}

	void removeColor(const int color_to_change)
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;
	
		for (unsigned int i = 3; i < _ppm_in_file.size(); i++)
		{
			// split the strings into vectors
			vector<string> color_row = StringSplitter::split(_ppm_in_file[i], " ");

			// remove the color by changing the value to 0, 
			if (color_to_change == 0)
			{
				for (unsigned int j = color_to_change; j < color_row.size() - 1; j += 3)
				{
					color_row[j] = "0";
				}
				for (unsigned int k = 0; k < color_row.size() - 1; k++)
				{
					ppm_out_file << color_row[k] << " ";
				}
			}
			else
			{
				for (unsigned int j = color_to_change; j < color_row.size(); j += 3)
				{
					color_row[j] = "0";
				}
				for (unsigned int k = 0; k < color_row.size(); k++)
				{
					ppm_out_file << color_row[k] << " ";
				}
			}
			ppm_out_file << endl;
		}
	}
	void negateColor(int color_to_change)
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		for (unsigned int i = 3; i < _ppm_in_file.size(); i++)
		{
			vector<string> color_row = StringSplitter::split(_ppm_in_file[i], " ");

			if (color_to_change == 0)
			{
				// negate the color by changing the value to (255 - old value), 
				for (unsigned int j = color_to_change; j < color_row.size() - 1; j += 3)
				{
					color_row[j] = to_string(255 - stoi(color_row[j]));
				}

				// write the values to the output file specified by the user
				for (auto val : color_row)
				{
					ppm_out_file << val << " ";
				}
				ppm_out_file << endl;
			}
			else
			{
				// negate the color by changing the value to 255 - old value, 
				for (unsigned int j = color_to_change; j < color_row.size(); j += 3)
				{
					color_row[j] = to_string(255 - stoi(color_row[j]));
				}

				// write the values to the output file specified by the user
				for (auto val : color_row)
				{
					ppm_out_file << val << " ";
				}
				ppm_out_file << endl;
			}
		}

	}
	void randomNoise()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		for (unsigned int i = 3; i < _ppm_in_file.size(); i++)
		{
			vector<string> color_row = StringSplitter::split(_ppm_in_file[i], " ");


			for (auto val : color_row) 
			{
				int random_noise_value = (rand() % 20) - 10;

				if (val != "")
				{
					int new_color = stoi(val) + random_noise_value;
					ppm_out_file << new_color << " ";
				}
			}
			ppm_out_file << endl;
		}
	}
	void highContrast()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		for (unsigned int i = 3; i < _ppm_in_file.size(); i++)
		{
			vector<string> color_row = StringSplitter::split(_ppm_in_file[i], " ");

			for (auto val : color_row)
			{
				if (val != "")
				{
					if (stoi(val) >= 128)
					{
						val = "255";
						ppm_out_file << val << " ";
					}
					else
					{
						val = "0";
						ppm_out_file << val << " ";
					}
				}
			}
			ppm_out_file << endl;
		}
	}
	void grayScale()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		for (unsigned int i = 3; i < _ppm_in_file.size(); i++)
		{
			vector<string> color_row = StringSplitter::split(_ppm_in_file[i], " ");

			for (unsigned int j = 0; j < color_row.size(); j += 3)
			{
				if (color_row[j] != "")
				{
					int val = (stoi(color_row[j]) + stoi(color_row[j + 1]) + stoi(color_row[j + 2])) / 3;
					color_row[j] = to_string(val);
					color_row[j + 1] = to_string(val);
					color_row[j + 2] = to_string(val);
				}
			}

			for (auto val : color_row)
			{
				ppm_out_file << val << " ";
			}
			ppm_out_file << endl;
		}
	}
	void flipHorizontal()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();

		for (auto row : table)
		{
			for (int i = row.size() - 1 ; i >= 0; i -= 3)
			{
				ppm_out_file << row[i - 2] << " " << row[i - 1] << " " << row[i] << " ";
			}
			ppm_out_file << endl;
		}
	}
	void flipVertical()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();

		for (unsigned int i = table.size() - 1; i > 0; i--)
		{
			for (auto val : table[i])
			{
				ppm_out_file << val << " ";
			}
			ppm_out_file << endl;
		}
	}
	void rotate90()
	{
		//getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _length_in << " " << _width_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();

		for (int i = 0; i < 3 * _width_in; i += 3)
		{
			for (unsigned int j = 0; j < table.size(); j++)
			{
				if (table[j].size() == 3 * _width_in)
				{
					try
					{
						ppm_out_file << table[j][i] << " " << table[j][i + 1] << " " << table[j][i + 2] << " ";
					}
					catch (...)
					{ }
				}
			}
			ppm_out_file << endl;
		}

		
	}
	void blur()
	{
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();

		// write the first row
		ppm_out_file << (table[0][0] + table[0][3] + table[1][0]) / 3 << " "; // first red val
		ppm_out_file << (table[0][1] + table[0][3] + table[1][0]) / 3 << " "; // first green val
		ppm_out_file << (table[0][2] + table[0][3] + table[1][0]) / 3 << " "; // first blue val

		for (unsigned int i = 3; i < table[0].size() - 3; i++)
		{
			ppm_out_file << (table[0][i] + table[0][i - 3] + table[0][i + 3] + table[1][i]) / 4 << " ";
		}

		ppm_out_file << (table[0][0] + table[0][3] + table[1][0]) / 3 << " "; // last red val
		ppm_out_file << (table[0][1] + table[0][3] + table[1][0]) / 3 << " "; // last green val
		ppm_out_file << (table[0][2] + table[0][3] + table[1][0]) / 3 << " "; // last blue val

		ppm_out_file << endl;

		// write the pixels between the first and last rows
		for (unsigned int i = 1; i < table.size(); i++)
		{
			if (table[i].size() != 0 && table[i + 1].size() != 0)
			{
				ppm_out_file << (table[i][0] + table[i][3] + table[i + 1][0] + table[i - 1][0]) / 4 << " "; // first red value of each row
				ppm_out_file << (table[i][1] + table[i][4] + table[i + 1][1] + table[i - 1][1]) / 4 << " "; // first green value of each row
				ppm_out_file << (table[i][2] + table[i][5] + table[i + 1][2] + table[i - 1][2]) / 4 << " "; // first blue value of each row
				
				for (unsigned int j = 3; j < table[i].size() - 3; j++)
				{
					ppm_out_file << (table[i][j] + table[i][j + 3] + table[i][j - 3] + table[i - 1][j] + table[i + 1][j]) / 5 << " ";
				}

				ppm_out_file << (table[i][table[i].size() - 3] + table[i][table[i].size() - 6] + table[i - 1][table[i].size() - 3]) / 4 << " "; // last red value of each row
				ppm_out_file << (table[i][table[i].size() - 2] + table[i][table[i].size() - 5] + table[i - 1][table[i].size() - 2]) / 4 << " "; // last green value of each row
				ppm_out_file << (table[i][table[i].size() - 1] + table[i][table[i].size() - 4] + table[i - 1][table[i].size() - 1]) / 4 << " "; // last blue value of each row

				ppm_out_file << endl;
			}
		}

		// write the last row
		ppm_out_file << (table[table.size() - 2][0] + table[table.size() - 2][3] + table[table.size() - 3][0]) / 3 << " ";
		ppm_out_file << (table[table.size() - 2][1] + table[table.size() - 2][4] + table[table.size() - 3][1]) / 3;
		ppm_out_file << (table[table.size() - 2][2] + table[table.size() - 2][4] + table[table.size() - 3][2]) / 3;

		for (unsigned int i = 3; i < table[0].size() - 3; i++)
		{
			ppm_out_file << (table[table.size() - 2][i] + table[table.size() - 2][i + 3] + table[table.size() - 2][i - 3] + table[table.size() - 3][i]) / 4 << " ";
		}
		ppm_out_file << endl;

		ppm_out_file << (table[table.size() - 2][table[0].size() - 3] + table[table.size() - 2][table[0].size() - 6] + table[table.size() - 3][table[0].size() - 3]) / 3 << " ";
		ppm_out_file << (table[table.size() - 2][table[0].size() - 2] + table[table.size() - 2][table[0].size() - 5] + table[table.size() - 3][table[0].size() - 2]) / 3 << " ";
		ppm_out_file << (table[table.size() - 2][table[0].size() - 1] + table[table.size() - 2][table[0].size() - 4] + table[table.size() - 3][table[0].size() - 3]) / 3 << " ";

		ppm_out_file << endl;
	}
	void pixelate()
	{
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();
		string reference_pixel = "";
		reference_pixel += to_string(table[0][0]) + " "  + to_string(table[0][1]) + " " + to_string(table[0][2]);

		int dist = 5;

		for (int r = 0; r < table.size() - 1; r++)
		{
			if (table[r].size() > 0)
			{
				for (int i = 0; i < table[r].size() - 15; i ++)
				{
					ppm_out_file << reference_pixel << " ";
					if (i % 15 == 0)
					{
						reference_pixel = to_string(table[r][i]) + " " + to_string(table[r][i + 1]) + " " + to_string(table[r][i + 2]);
					}
				}
				ppm_out_file << endl;
			}
		}
	}
	void flyVision()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _width_in << " " << _length_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();

		for (auto row : table)
		{
			for (int i = row.size() - 1; i >= 3; i -= 2)
			{
				ppm_out_file << row[i - 3] << " " << row[i - 2] << " " << row[i - 1] << " ";
			}
			ppm_out_file << endl;
		}
	}
	void zebraGumVision()
	{
		getHeader();
		ofstream ppm_out_file(_output_file);

		ppm_out_file << _magic_number << endl;
		ppm_out_file << _length_in << " " << _width_in << endl;
		ppm_out_file << _max_brightness << endl;

		vector<vector<int>> table = makeTable();

		for (int i = 0; i < 3 * _width_in; i++)
		{
			for (unsigned int j = 0; j < table.size() - 2; j += 3)
			{
				if (table[j].size() > 0)
				{
					try
					{
						ppm_out_file << table[j][i] << " " << table[j][i + 1] << " " << table[j][i + 2] << " ";
					}
					catch (...)
					{
					}
				}
			}
			ppm_out_file << endl;
		}

	}
};