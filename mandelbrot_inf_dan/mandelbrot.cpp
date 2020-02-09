#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

typedef pair<double, double> point;

int NUM_OF_ITER = 50;

int WIDTH = 1000, HEIGHT = 800;

extern "C" {FILE __imp___acrt_iob_func[3] = { *stdin,*stdout,*stderr };}

string create_override_string(int size) {
    string result = "";
    for(int i = 0; i < size; i++){
        result += "\b";
    }

    return result;
}

string PRINT_FINENESS = create_override_string(12);

int in_set(point z, point c) {
    int i = 0;

    for(i = 0; i < NUM_OF_ITER; i++) {
        z = {z.first * z.first - z.second * z.second + c.first, 2 * z.first * z.second + c.second};
        if(z.first * z.first + z.second * z.second > 100) break;
    }
    return 255 - ((double) i / NUM_OF_ITER) * 255;
}

void updateMandelbrotImage(Uint8* arr, point top, point bottom, double& factor) {
    factor = max((bottom.first - top.first) / WIDTH, (top.second - bottom.second) / HEIGHT);

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < WIDTH * HEIGHT; i++){
        int y = i / WIDTH;
        int x = i % WIDTH;
        auto data = in_set({0, 0}, {x * factor + top.first, -y * factor + top.second});

        arr[4 * (x + WIDTH * y) + 0] = data;
        arr[4 * (x + WIDTH * y) + 1] = data;
        arr[4 * (x + WIDTH * y) + 2] = data;
        arr[4 * (x + WIDTH * y) + 3] = 255;
    }

}

int main(int argc, char** argv) {
    if(argc != 3){
        cout << "Uporaba: [x-velikost] [y-velikost]" << endl;
        return 0;
    }
    WIDTH = stoi(argv[1]);
    HEIGHT = stoi(argv[2]);

    RenderWindow rw (VideoMode(WIDTH, HEIGHT), L"Mandelbrot fraktal :: Anže Hočevar");

    double factor;
    bool r0 = true, r1 = true;

    Texture mandelbrot_txt; mandelbrot_txt.create(WIDTH, HEIGHT);
    Uint8* arr = new Uint8 [4 * WIDTH * HEIGHT];

    Sprite mandelbrot;

    point top = {-2, 1};
    point bottom = {1, -1};

    while(rw.isOpen()) {
        Event event;
        while (rw.pollEvent(event)) {
            if (event.type == sf::Event::Closed) rw.close();
            if (event.type == Event::MouseButtonPressed){
                double mouseX = Mouse::getPosition(rw).x;
                double mouseY = Mouse::getPosition(rw).y;
                bottom = top;
                top = {top.first + mouseX * factor, top.second - mouseY * factor};
                r0 = true;
            }
            if (event.type == Event::MouseButtonReleased){
                double mouseX = Mouse::getPosition(rw).x;
                double mouseY = Mouse::getPosition(rw).y;
                bottom = {bottom.first + mouseX * factor, bottom.second - mouseY * factor};
                r1 = true;
            }
            if(event.type == Event::KeyPressed){
                if(event.key.code == Keyboard::Up) {
                    NUM_OF_ITER += 50;
                    cout << PRINT_FINENESS << "Finost: " << setw(4) << NUM_OF_ITER << flush;
                    r0 = true; r1 = true;
                }
                if(event.key.code == Keyboard::Down) {
                    if(NUM_OF_ITER > 0) NUM_OF_ITER -= 50;
                    cout << PRINT_FINENESS << "Finost: " << setw(4) << NUM_OF_ITER << flush;
                    r0 = true; r1 = true;
                }
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Z)){
            top = {-2, 2};
            bottom = {2, -2};
            r0 = true; r1 = true;
        }
        if(Keyboard::isKeyPressed(Keyboard::Escape)) {
            top = {-2, 1};
            bottom = {1, -1};
            r0 = true; r1 = true;
            factor = 0;
            NUM_OF_ITER = 50;
            cout << PRINT_FINENESS << "Finost: " << setw(4) << NUM_OF_ITER << flush;
        }
        
        if(r0 && r1) {
            updateMandelbrotImage(arr, top, bottom, factor);
            mandelbrot_txt.update(arr);
            mandelbrot.setTexture(mandelbrot_txt);

            r0 = false; r1 = false;
        }

        rw.clear();
        rw.draw(mandelbrot);
        rw.display();
    }
    return 0;
}