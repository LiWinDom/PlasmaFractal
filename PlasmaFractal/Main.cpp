#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Config.h"

uint8_t frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
uint16_t HSVframeBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];

void setPixel(const uint16_t& x, const uint16_t& y, const uint32_t& color) {
    frameBuffer[y][x][0] = color >> 24;
    frameBuffer[y][x][1] = color >> 16;
    frameBuffer[y][x][2] = color >> 8;
    return;
}

void setHSVpixel(const uint16_t& x, const uint16_t& y, const uint16_t& hue, const uint8_t& saturation, const uint8_t& value) {
    HSVframeBuffer[y][x][0] = hue % 360;
    HSVframeBuffer[y][x][1] = std::min(saturation, (uint8_t)100);
    HSVframeBuffer[y][x][2] = std::min(value, (uint8_t)100);
    return;
}

uint32_t HSVtoRGB(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t alpha = 255) {
    hue %= 360;
    saturation = std::min(std::max(saturation, (uint8_t)0), (uint8_t)100);
    value = std::min(std::max(value, (uint8_t)0), (uint8_t)100);
    alpha = std::min(std::max(alpha, (uint8_t)0), (uint8_t)100);

    double s = saturation / 100.0;
    double v = value / 100.0;

    double C = s * v;
    double X = C * (1 - abs(std::fmod(hue / 60.0, 2) - 1));
    double m = v - C;

    if (hue < 60) {
        uint8_t r = (C + m) * 255;
        uint8_t g = (X + m) * 255;
        uint8_t b = m * 255;
        return (r << 24) | (g << 16) | (b << 8) | alpha;
    }
    if (hue < 120) {
        uint8_t r = (X + m) * 255;
        uint8_t g = (C + m) * 255;
        uint8_t b = m * 255;
        return (r << 24) | (g << 16) | (b << 8) | alpha;
    }
    if (hue < 180) {
        uint8_t r = m * 255;
        uint8_t g = (C + m) * 255;
        uint8_t b = (X + m) * 255;
        return (r << 24) | (g << 16) | (b << 8) | alpha;
    }
    if (hue < 240) {
        uint8_t r = m * 255;
        uint8_t g = (X + m) * 255;
        uint8_t b = (C + m) * 255;
        return (r << 24) | (g << 16) | (b << 8) | alpha;
    }
    if (hue < 300) {
        uint8_t r = (X + m) * 255;
        uint8_t g = m * 255;
        uint8_t b = (C + m) * 255;
        return (r << 24) | (g << 16) | (b << 8) | alpha;
    }
    uint8_t r = (C + m) * 255;
    uint8_t g = m * 255;
    uint8_t b = (X + m) * 255;
    return (r << 24) | (g << 16) | (b << 8) | alpha;
}

void setAvarage(const uint16_t& x1, const uint16_t& y1, const uint16_t& x2, const uint16_t& y2, const uint16_t& setX, const uint16_t& setY) {
    if (HSVframeBuffer[setY][setX][0] != 0 || HSVframeBuffer[setY][setX][1] != 0 || HSVframeBuffer[setY][setX][2] != 0) return;
    double rnd = 0.5 + ((double)std::rand() / RAND_MAX - 0.5) * RANDOM_COEFFICIENT;
    setHSVpixel(setX, setY, HSVframeBuffer[y1][x1][0] * rnd + HSVframeBuffer[y2][x2][0] * (1 - rnd),
        HSVframeBuffer[y1][x1][1] * rnd + HSVframeBuffer[y2][x2][1] * (1 - rnd),
        HSVframeBuffer[y1][x1][2] * rnd + HSVframeBuffer[y2][x2][2] * (1 - rnd));
    return;
}

void setAvarage(const uint16_t& x1, const uint16_t& y1, const uint16_t& x2, const uint16_t& y2, const uint16_t& x3, const uint16_t& y3, const uint16_t& x4, const uint16_t& y4, const uint16_t& setX, const uint16_t& setY) {
    if (HSVframeBuffer[setY][setX][0] != 0 || HSVframeBuffer[setY][setX][1] != 0 || HSVframeBuffer[setY][setX][2] != 0) return;
    double rnd1 = 0.5 + ((double)std::rand() / RAND_MAX - 0.5) * RANDOM_COEFFICIENT;
    double rnd2 = 0.5 + ((double)std::rand() / RAND_MAX - 0.5) * RANDOM_COEFFICIENT;
    setHSVpixel(setX, setY, (HSVframeBuffer[y1][x1][0] * rnd1 + HSVframeBuffer[y2][x2][0] * (1 - rnd1) + HSVframeBuffer[y3][x3][0] * rnd2 + HSVframeBuffer[y4][x4][0] * (1 - rnd2)) / 2,
        (HSVframeBuffer[y1][x1][1] * rnd1 + HSVframeBuffer[y2][x2][1] * (1 - rnd1) + HSVframeBuffer[y3][x3][1] * rnd2 + HSVframeBuffer[y4][x4][1] * (1 - rnd2)) / 2,
        (HSVframeBuffer[y1][x1][2] * rnd1 + HSVframeBuffer[y2][x2][2] * (1 - rnd1) + HSVframeBuffer[y3][x3][2] * rnd2 + HSVframeBuffer[y4][x4][2] * (1 - rnd2)) / 2);
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
    for (uint16_t i = 0; i < WINDOW_HEIGHT; ++i) {
        for (uint16_t j = 0; j < WINDOW_WIDTH; ++j) {
            setHSVpixel(j, i, 0, 0, 0);
        }
    }

    setHSVpixel(0, 0, std::rand() % 360, std::rand() % 50 + 50, std::rand() % 50 + 50);
    setHSVpixel(WINDOW_WIDTH - 1, 0, std::rand() % 360, std::rand() % 50 + 50, std::rand() % 50 + 50);
    setHSVpixel(0, WINDOW_HEIGHT - 1, std::rand() % 360, std::rand() % 50 + 50, std::rand() % 50 + 50);
    setHSVpixel(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1, std::rand() % 360, std::rand() % 50 + 50, std::rand() % 50 + 50);
    
    makePlasma(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    for (uint16_t i = 0; i < WINDOW_HEIGHT; ++i) {
        for (uint16_t j = 0; j < WINDOW_WIDTH; ++j) {
            setPixel(j, i, HSVtoRGB(HSVframeBuffer[i][j][0], HSVframeBuffer[i][j][1], HSVframeBuffer[i][j][2]));
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