// #include "noise.hpp"
#define FNL_IMPL
#include "FastNoiseLite.h"
#include <SDL3/SDL.h>
#include <iostream>

    fnl_state noise = fnlCreateState();
    float *noiseBuff;
    // noise.persistence = 0.25;
    // noise.octaves = 1;
    // noise.frequency = 2;
    // noise.amplitude = 5;

void drawSlider(SDL_Renderer* renderer, float x, float y, float width, float height, float& value) {
    // Slider track
    SDL_FRect track = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light grey track
    SDL_RenderFillRect(renderer, &track);

    // Slider thumb
    float thumbWidth = 20.0;
    float thumbPosX = x + value * (width - thumbWidth) / 100.0;
    SDL_FRect thumb = {thumbPosX, y - 5, thumbWidth, height + 10};
    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // Blue thumb
    SDL_RenderFillRect(renderer, &thumb);
}
void drawButton(SDL_Renderer *ren, float x, float y, float width, float height) {
    SDL_FRect button = {x, y, width, height};
    SDL_SetRenderDrawColor(ren, 0, 128, 255, 255);
    SDL_RenderFillRect(ren, &button);
}
void handleSliderInput(SDL_Event& e, float x, float y, float width, float height, float& value) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check if mouse is inside slider track
        if (mouseX >= x && mouseX <= x + width && mouseY >= y - 5 && mouseY <= y + height + 5) {
            value = (mouseX - x) * 100 / width; // Calculate the value based on mouse position
        }
    }
    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Update value while dragging the thumb
        if (e.motion.state & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) {
            if (mouseX >= x && mouseX <= x + width && mouseY >= y - 5 && mouseY <= y + height + 5) {
                value = (mouseX - x) * 100 / width; // Recalculate value based on the mouse's position
            }
        }
    }
}
void updateNoise() {
    for (float y = 0; y < 400; y++){
            for (float x = 0; x < 500; x++){
                float nx = x/64, ny = y/64;
                // convert to color
                noiseBuff[(int)(y * 500 + x)] = fnlGetNoise2D(&noise,x, y);
            }
        }
}
void handleBtn(SDL_Event& e, float x, float y, float width, float height) {
    // check if mouse is within button
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check if mouse is inside button
        if (mouseX >= x && mouseX <= x + width && mouseY >= y - 5 && mouseY <= y + height + 5) {
           updateNoise();
        }
    }
}

int main() {
    noise.noise_type = FNL_NOISE_PERLIN;
    // malloc buffer
    noiseBuff = (float*)malloc(500*400*sizeof(float));
    // Set fractal type to FBm
    noise.fractal_type = FNL_FRACTAL_FBM;
    // Fractal parameters
    noise.octaves = 4;
    noise.lacunarity = 2.0f;
    noise.gain = 0.5f;


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *wind;
    SDL_Renderer *rend;
    SDL_CreateWindowAndRenderer("perlin", 1000, 800, 0, &wind, &rend);

    SDL_Event e;
    int running = 1;
    float slider_value = 2;

    updateNoise();
    // print noise values for debugging
    // for (int y = 0; y < 400; y++) {
    //     for (int x = 0; x < 500; x++) {
    //         std::cout << "value: " << noiseBuff[y*500+x] << std::endl;
    //     }
    // }
    
    while(running) {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = 0;
            handleSliderInput(e, 100, 600, 400, 10, slider_value);
            handleBtn(e, 100.0, 650.0, 400.0, 30.0);
        }
        // update noise value
        // noise.frequency = slider_value;

        SDL_SetRenderDrawColor(rend, 1, 1, 1, 255);
        SDL_RenderClear(rend);

        for (int y = 0; y < 400; y++){
            for (int x = 0; x < 500; x++){
                float samp = noiseBuff[(int)(y * 500 + x)];
                unsigned char col = static_cast<uint8_t>((samp + 1.0f) * 0.5f * 255.0f);
                SDL_SetRenderDrawColor(rend, col, col, col, 255);
                SDL_RenderPoint(rend, x, y);
            }
        }
        drawButton(rend, 100.0, 650.0, 400.0, 30.0);
        // Draw the slider
        drawSlider(rend, 100.0, 600.0, 400.0, 10.0, slider_value);
        SDL_RenderPresent(rend);
        // button to update slider
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(wind);
    SDL_Quit();
}