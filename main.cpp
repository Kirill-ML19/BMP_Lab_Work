#include <iostream>
#include <string>
#include "BMP.h"

void printUsage(const std::string& programName) {
    std::cerr << "Usage:\n"
              << "  " << programName << " input.bmp output.bmp --rotate <90|180|270>\n"
              << "  " << programName << " input.bmp output.bmp --blur\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string operation = argv[3];

    BMP image;
    if (!image.read(inputFile)) {
        std::cerr << "Failed to read " << inputFile << "\n";
        return 1;
    }

    if (operation == "--rotate") {
        if (argc < 5) {
            std::cerr << "Missing rotation angle (90, 180, 270)\n";
            return 1;
        }

        int angle;
        try {
            angle = std::stoi(argv[4]);
        } catch (const std::invalid_argument&) {
            std::cerr << "Rotation angle must be a number (90, 180, or 270)\n";
            return 1;
        }

        if (angle != 90 && angle != 180 && angle != 270) {
            std::cerr << "Invalid angle: must be 90, 180, or 270\n";
            return 1;
        }

        image.rotate(angle);
    } else if (operation == "--blur") {
        image.blur();
    } else {
        std::cerr << "Unknown operation: " << operation << "\n";
        printUsage(argv[0]);
        return 1;
    }

    if (!image.write(outputFile)) {
        std::cerr << "Failed to write " << outputFile << "\n";
        return 1;
    }

    std::cout << "Successfully saved result to " << outputFile << "\n";
    return 0;
}
