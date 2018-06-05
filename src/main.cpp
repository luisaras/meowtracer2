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

/*

1/n * \sum Li(p, i) * fr (i, o) * cos / p(o)

Li(o, i) = Ka * Ia + Kd * Id...


fr(i, o, n) = F (i, hr) * G (i, o, hr) * D (hr) / (4 * |i * n| * | o * n|)

Or = 2 * | i * m | m - i

weight (o) = fr (i, o, n) ( |o * n| / Po (o) )

Po (o) = Pm (M) / 4 | o * hr |

*/

void testImage() {
	string in = "in.png";
	string out = "out.png";
	Image img(in);
	img.save(out);
}

void printScene(string &fileName) {
	string inputFile = "meow/" + fileName + ".meow";
	string outputFile = "png/" + fileName + ".png";
	Parser parser;
	if (parser.load(inputFile)) {
		time_t before = time(NULL);
		//Color* colors = data.renderer->render(data.scene, data.colCount, data.rowCount);
		time_t after = time(NULL);
		double seconds = difftime(after, before);
		cout << "Rendering done in " << seconds << " seconds." << endl; 
		//Image img(data.colCount, data.rowCount, colors);
		//img.save(outputFile);
	}
}

int main(int argc, char* argv[]) {
	srand (time(NULL));
	if (argc > 1) {
		string fileName = argv[1];
		//printScene(fileName);
		testImage();
	} else {
		cout << "File name must be specified." << endl;
	}
	return 0;
}

#endif