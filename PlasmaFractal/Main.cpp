#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Config.h"

uint8_t frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
uint8_t YCbCrFrameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];

void setPixel(const uint16_t& x, const uint16_t& y, const uint32_t& color) {
    frameBuffer[y][x][0] = color >> 24;
    frameBuffer[y][x][1] = color >> 16;
    frameBuffer[y][x][2] = color >> 8;
    return;
}

void setYCbCrPixel(const uint16_t& x, const uint16_t& y, const uint8_t& Y, const uint8_t& Cb, const uint8_t& Cr) {
    YCbCrFrameBuffer[y][x][0] = Y;
    YCbCrFrameBuffer[y][x][1] = Cb;
    YCbCrFrameBuffer[y][x][2] = Cr;
    return;
}

uint32_t YCbCrToRGB(const uint8_t& Y, const uint8_t& Cb, const uint8_t& Cr, const uint8_t& alpha = 255) {
    uint8_t r = std::min(std::max((Y + 1.40200 * (Cr - 0x80)), 0.0), 255.0);
    uint8_t g = std::min(std::max((Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80)), 0.0), 255.0);
    uint8_t b = std::min(std::max((Y + 1.77200 * (Cb - 0x80)), 0.0), 255.0);
    return (r << 24) | (g << 16) | (b << 8) | alpha;
}

void setAvarage(const uint16_t& x1, const uint16_t& y1, const uint16_t& x2, const uint16_t& y2, const uint16_t& setX, const uint16_t& setY) {
    /*
    setYCbCrPixel(setX, setY, (YCbCrFrameBuffer[y1][x1][0] + YCbCrFrameBuffer[y2][x2][0]) / 2 + MAX_Y_CHANGE / (RAND_MAX / 2.0) * (std::rand() - RAND_MAX / 2.0),
        (YCbCrFrameBuffer[y1][x1][1] + YCbCrFrameBuffer[y2][x2][1]) / 2 + MAX_CB_CHANGE / (RAND_MAX / 2.0) * (std::rand() - RAND_MAX / 2.0),
        (YCbCrFrameBuffer[y1][x1][2] + YCbCrFrameBuffer[y2][x2][2]) / 2 + MAX_CR_CHANGE / (RAND_MAX / 2.0) * (std::rand() - RAND_MAX / 2.0));
    */
    
    double rnd = 0.5 + ((double)std::rand() / RAND_MAX - 0.5) * RANDOM_COEFFICIENT;
    setYCbCrPixel(setX, setY, YCbCrFrameBuffer[y1][x1][0] * rnd + YCbCrFrameBuffer[y2][x2][0] * (1 - rnd),
        YCbCrFrameBuffer[y1][x1][1] * rnd + YCbCrFrameBuffer[y2][x2][1] * (1 - rnd),
        YCbCrFrameBuffer[y1][x1][2] * rnd + YCbCrFrameBuffer[y2][x2][2] * (1 - rnd));
    
    return;
}

void setAvarage(const uint16_t& x1, const uint16_t& y1, const uint16_t& x2, const uint16_t& y2, const uint16_t& x3, const uint16_t& y3, const uint16_t& x4, const uint16_t& y4, const uint16_t& setX, const uint16_t& setY) {
    /*
    setYCbCrPixel(setX, setY, (YCbCrFrameBuffer[y1][x1][0] + YCbCrFrameBuffer[y2][x2][0] + YCbCrFrameBuffer[y3][x3][0] + YCbCrFrameBuffer[y4][x4][0]) / 4 + MAX_Y_CHANGE / (RAND_MAX / 2.0) * (std::rand() - RAND_MAX / 2.0),
        (YCbCrFrameBuffer[y1][x1][1] + YCbCrFrameBuffer[y2][x2][1] + YCbCrFrameBuffer[y3][x3][1] + YCbCrFrameBuffer[y4][x4][1]) / 4 + MAX_CB_CHANGE / (RAND_MAX / 2.0) * (std::rand() - RAND_MAX / 2.0),
        (YCbCrFrameBuffer[y1][x1][2] + YCbCrFrameBuffer[y2][x2][2] + YCbCrFrameBuffer[y3][x3][2] + YCbCrFrameBuffer[y4][x4][1]) / 4 + MAX_CR_CHANGE / (RAND_MAX / 2.0) * (std::rand() - RAND_MAX / 2.0));
    */
    
    double rnd1 = 0.5 + ((double)std::rand() / RAND_MAX - 0.5) * RANDOM_COEFFICIENT;
    double rnd2 = 0.5 + ((double)std::rand() / RAND_MAX - 0.5) * RANDOM_COEFFICIENT;
    setYCbCrPixel(setX, setY, (YCbCrFrameBuffer[y1][x1][0] * rnd1 + YCbCrFrameBuffer[y2][x2][0] * (1 - rnd1) + YCbCrFrameBuffer[y3][x3][0] * rnd2 + YCbCrFrameBuffer[y4][x4][0] * (1 - rnd2)) / 2,
        (YCbCrFrameBuffer[y1][x1][1] * rnd1 + YCbCrFrameBuffer[y2][x2][1] * (1 - rnd1) + YCbCrFrameBuffer[y3][x3][1] * rnd2 + YCbCrFrameBuffer[y4][x4][1] * (1 - rnd2)) / 2,
        (YCbCrFrameBuffer[y1][x1][2] * rnd1 + YCbCrFrameBuffer[y2][x2][2] * (1 - rnd1) + YCbCrFrameBuffer[y3][x3][2] * rnd2 + YCbCrFrameBuffer[y4][x4][2] * (1 - rnd2)) / 2);
    
    return;
}

void makePlasma(const uint16_t& x, const uint16_t& y, const uint16_t& width, const uint16_t& height) {
    if (height < 3 && width < 3) return;
    if (height < 2 || width < 2) return;

    uint16_t centerX = (width - 1) / 2.0 + x;
    uint16_t centerY = (height - 1) / 2.0 + y;
    if (width > 2) {
        setAvarage(x, y, x + width - 1, y, centerX, y);
        setAvarage(x, y + height - 1, x + width - 1, y + height - 1, centerX, y + height - 1);
    }
    if (height > 2) {
        setAvarage(x, y, x, y + height - 1, x, centerY);
        setAvarage(x + width - 1, y, x + width - 1, y + height - 1, x + width - 1, centerY);
    }

    if (width == 2) {
        makePlasma(x, y, width, centerY - y + 1);
        makePlasma(x, centerY, width, y + height - centerY);
        return;
    }
    if (height == 2) {
        makePlasma(x, y, centerX - x + 1, height);
        makePlasma(centerX, y, x + width - centerX, height);
        return;
    }
    setAvarage(centerX, y, centerX, y + height - 1, x, centerY, x + width - 1, centerY, centerX, centerY);
    makePlasma(x, y, centerX - x + 1, centerY - y + 1);
    makePlasma(centerX, y, x + width - centerX, centerY - y + 1);
    makePlasma(x, centerY, centerX - x + 1, y + height - centerY);
    makePlasma(centerX, centerY, x + width - centerX, y + height - centerY);
    return;
}

void setup() {
    std::srand(std::time(NULL));

    setYCbCrPixel(0, 0, std::rand() % 256, std::rand() % 256, std::rand() % 256);
    setYCbCrPixel(WINDOW_WIDTH - 1, 0, std::rand() % 256, std::rand() % 256, std::rand() % 256);
    setYCbCrPixel(0, WINDOW_HEIGHT - 1, std::rand() % 256, std::rand() % 256, std::rand() % 256);
    setYCbCrPixel(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, std::rand() % 256, std::rand() % 256, std::rand() % 256);

    makePlasma(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    for (uint16_t i = 0; i < WINDOW_HEIGHT; ++i) {
        for (uint16_t j = 0; j < WINDOW_WIDTH; ++j) {
            setPixel(j, i, YCbCrToRGB(YCbCrFrameBuffer[i][j][0], YCbCrFrameBuffer[i][j][1], YCbCrFrameBuffer[i][j][2]));
        }
    }
    return;
}

void eventProcess(sf::Window& window) {
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                setup();
            }
        }
    }
    return;
}

int main() {
#if !_DEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    try {
        setup();

        sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Plasma fractal", sf::Style::Close);
        window.setActive(true);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);

        while (window.isOpen()) {
            /*
            std::chrono::duration<double> t = std::chrono::system_clock::now().time_since_epoch();
            if (time > 0) {
                double fps = 1 / (t.count() - time);
                animation_speed = std::abs(animation_coefficient * fps / 60);
                window.setTitle("Practical 1 (x: " + std::to_string((int)std::round(cur_shift.first)) +
                    ", y: " + std::to_string((int)std::round(cur_shift.second)) +
                    ", " + std::to_string((int)std::round(cur_size)) + " pixels per unit, " +
                    std::to_string((int)fps) + " fps)");
            }
            time = t.count();
            */
            eventProcess(window);

            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, &frameBuffer);
            window.display();
        }
    }
    catch (...) {
        ShowWindow(GetConsoleWindow(), SW_SHOW);
        std::cout << "Something went wrong... We are sorry about that";
        system("pause");
    }
    return 0;
}