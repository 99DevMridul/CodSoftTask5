#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Pixel {
    unsigned char r, g, b;
};

class Image {
    int width, height, maxColor;
    vector<Pixel> pixels;

public:
    bool load(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) return false;

        string format;
        file >> format;
        if (format != "P6") return false;

        file >> width >> height >> maxColor;
        file.ignore();

        pixels.resize(width * height);
        file.read((char*)pixels.data(), pixels.size() * sizeof(Pixel));
        file.close();
        return true;
    }

    bool save(const string& filename) const {
        ofstream file(filename, ios::binary);
        if (!file) return false;

        file << "P6\n" << width << " " << height << "\n" << maxColor << "\n";
        file.write((const char*)pixels.data(), pixels.size() * sizeof(Pixel));
        file.close();
        return true;
    }

    void toGrayscale() {
        for (auto& pixel : pixels) {
            unsigned char gray = 0.3 * pixel.r + 0.59 * pixel.g + 0.11 * pixel.b;
            pixel.r = pixel.g = pixel.b = gray;
        }
    }

    void adjustBrightness(int value) {
        for (auto& pixel : pixels) {
            pixel.r = min(max(pixel.r + value, 0), 255);
            pixel.g = min(max(pixel.g + value, 0), 255);
            pixel.b = min(max(pixel.b + value, 0), 255);
        }
    }
};

int main() {
    Image img;
    string inputFile, outputFile;
    int choice, brightnessValue;

    cout << "Enter the input image filename (PPM format): ";
    cin >> inputFile;

    if (!img.load(inputFile)) {
        cout << "Failed to load the image.\n";
        return 1;
    }

    cout << "Image Processing Options:\n";
    cout << "1. Convert to Grayscale\n";
    cout << "2. Adjust Brightness\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        img.toGrayscale();
    } else if (choice == 2) {
        cout << "Enter brightness adjustment value (-255 to 255): ";
        cin >> brightnessValue;
        img.adjustBrightness(brightnessValue);
    } else {
        cout << "Invalid choice.\n";
        return 1;
    }

    cout << "Enter the output image filename: ";
    cin >> outputFile;

    if (!img.save(outputFile)) {
        cout << "Failed to save the image.\n";
        return 1;
    }

    cout << "Image processed and saved successfully.\n";
    return 0;
}
