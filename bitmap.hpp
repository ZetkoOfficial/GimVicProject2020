/*
    Reading black-white bitmaps.
    
    Author: Anže Hočevar
*/

#ifndef INCLUDE_BMPIMAGE
#define INCLUDE_BMPIMAGE

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class BMPImage {
    char info[54];
    vector<vector<bool>> bwImg;
    int w, h;
    int size;
public:
    void open(string filename) {
        fstream fs;
        fs.open(filename, ios::in);

        fs.read(info, 54);
        this->w = info[18];
        this->h = info[22];
        this->size = 3 * w * h;
        
        char data[size];
        fs.read(data, size); 

        this->bwImg = vector<vector<bool>> (w, vector<bool> (h));

        for(int i = 0; i < w; i++){
            for(int j = 0; j < h; j++){
                bwImg[i][j] = (unsigned char) (data[j * 3 * w + 3 * i]) < (unsigned int) 100;
            }
        }

        fs.close();
    }

    int width() {
        return w;
    }

    int height() {
        return h;
    }
    
    bool get_pixel(int x, int y) {
        return bwImg[x][h - y - 1];
    }

};

#endif