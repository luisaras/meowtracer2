#ifndef __MAIN__
#define __MAIN__

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Parser/Parser.h"
#include "Image/Image.h"

using std::string;
using std::cout;
using std::endl;
using namespace std::chrono;

/*

1/n * \sum Li(p, i) * fr (i, o) * cos / p(o)

Li(o, i) = Ka * Ia + Kd * Id...


fr(i, o, n) = F (i, hr) * G (i, o, hr) * D (hr) / (4 * |i * n| * | o * n|)

Or = 2 * | i * m | m - i

weight (o) = fr (i, o, n) ( |o * n| / Po (o) )

Po (o) = Pm (M) / 4 | o * hr |

*/

void printScene(string &fileName) {
	string inputFile = "meow/" + fileName + ".meow";
	string outputFile = "png/" + fileName + ".png";
	Parser parser;
	if (parser.load(inputFile)) {

		auto before = high_resolution_clock::now(); // or use steady_clock if high_resolution_clock::is_steady is false

		Color* colors = parser.renderer->render(parser.colCount, parser.rowCount);

		auto after = high_resolution_clock::now();
		double time = duration<double>(after - before).count();

		cout << "Rendering done in " << time << " seconds." << endl; 
		Image img(parser.colCount, parser.rowCount, colors);
		img.save(outputFile);
		delete [] colors;
	}
}

int main(int argc, char* argv[]) {
	srand (time(NULL));
	if (argc > 1) {
		string fileName = argv[1];
		printScene(fileName);
	} else {
		cout << "File name must be specified." << endl;
	}
	return 0;
}

#endif