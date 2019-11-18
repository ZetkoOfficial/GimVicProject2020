#include "vectormatrix.hpp"
#include "bitmap.hpp"
#include <string>
#include <fstream>

using namespace std;

double roundDouble(double d){
    return (int)(d * pow(10, 5)) / (double) pow(10, 5);
}

Matrix2x2 createRotationMatrix(int steps) {
    double ang = 2.0 * M_PI / steps;
    
    Matrix2x2 rotMat ({cos(ang), -sin(ang)},
               {sin(ang), cos(ang)});

    return rotMat;
}

Vector2 getPixelCoord(Vector2 v, int w, double vMax){
    return ((w - 1)/2.0) * (Vector2(1, 1) + (1/vMax) * v);
}

vector<vector<bool>> encodeImg(BMPImage img, int w, int steps) {
    int progress = steps / 10;

    vector<vector<bool>> res (steps, vector<bool> (w / 2));
    
    vector<Vector2> vectors;
    for(int i = 0; i < w / 2; i++){
        vectors.push_back(Vector2(0, i + 1));
    }

    Matrix2x2 rotMatrix = createRotationMatrix(steps);
    for(int i = 0; i < steps; i++) {
        if(i % progress == 0) {std::cout << "|"; std::cout.flush();}

        for(int j = 0; j < vectors.size(); j++){
            Vector2 pixel = getPixelCoord(vectors[j], w, w / 2);
            res[i][j] = img.get_pixel(round(pixel[0]), round(pixel[1]));

            vectors[j] = rotMatrix * vectors[j];
        }
    }

    return res;
}

void writeToFile(vector<vector<bool>> enc, string fileName) {
    int progress = enc.size() / 10;
    
    fstream fs;
    fs.open(fileName, ios::out);

    for(int in = 0; in < enc.size(); in++) {
        if(in % progress == 0) {std::cout << "|"; std::cout.flush();}

        vector<bool> i = enc[in];
        for(bool j : i) {
            fs << j << " "; 
        }
        fs << "\n";
    }
    
    fs.close();
}

#ifdef TESTING
#include "bitmap_image.hpp"
void test(vector<vector<bool>> enc, BMPImage img, int steps){
    vector<Vector2> vectors;
    for(int i = 0; i < img.width() / 2; i++){
        vectors.push_back(Vector2(0, i + 1));
    }

    bitmap_image newImg (img.width(), img.width());
    Matrix2x2 rotMatrix = createRotationMatrix(steps);
    for(int i = 0; i < steps; i++){
        for(int j = 0; j < vectors.size(); j++){
            Vector2 pixel = getPixelCoord(vectors[j], img.width(), img.width() / 2);
            newImg.set_pixel(round(pixel[0]), round(pixel[1]), 255 * enc[i][j], 255 * enc[i][j], 255 * enc[i][j]);

            vectors[j] = rotMatrix * vectors[j];
        }
    }

    newImg.save_image("test.bmp");
}
#endif

int main(int argc, char* argv[] ) {

    if(argc < 4){
        std::cout << "Usage: [fileName] [steps] [targetFile]" << "\n";
        std::cout << "Anže Hočevar, November 2019" << "\n";
        return 0;
    }
    string fileName = argv[1];
    string stepString = argv[2];
    string targetFile = argv[3];

    int steps = stoi(stepString);
    BMPImage img; img.open(fileName);

    std::cout << "Encoding: "; std::cout.flush();
    vector<vector<bool>> enc = encodeImg(img, img.width(), steps);
    std::cout << "\n";

    #ifdef TESTING 
    test(enc, img, steps); 
    #endif

    std::cout << "Writing:  "; std::cout.flush();
    writeToFile(enc, targetFile);
    std::cout << "\n" << "Done!" << "\n";
}