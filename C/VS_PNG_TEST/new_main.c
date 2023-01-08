
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"
#include "SDL.h"
#include "png.h"


int SDL_initializations(FILE* fp, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, png_structpp* png, png_infopp* info, int width, int height);


int main(int argc, char* argv[])
{
    SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;


    // Open the PNG file
    FILE* fp;
    if (fopen_s(&fp, ".\\menacing.png", "rb") != 0) 
    {
        fprintf(stderr, "cannot open file '%s'\n", "menacing.png");
    }


    // Read the PNG header
    png_byte header[8];
    fread(header, sizeof(char), 8, fp);
    if (png_sig_cmp(header, 0, 8) != 0) {
        fprintf(stderr, "Error: Not a PNG file\n");
        fclose(fp);
        return 1;
    }

    // Initialize the PNG struct
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fprintf(stderr, "Error: Could not initialize PNG struct\n");
        fclose(fp);
        return 1;
    }

    // Initialize the info struct
    png_infop info = png_create_info_struct(png);
    if (!info) {
        fprintf(stderr, "Error: Could not initialize PNG info struct\n");
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return 1;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png))) {
        fprintf(stderr, "Error: An error occurred while reading the PNG file\n");
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return 1;
    }

    // Set up the input control
    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);

    // Read the image info
    png_read_info(png, info);
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    //png_byte color_type = png_get_color_type(png, info);
    //png_byte bit_depth = png_get_bit_depth(png, info);
    

    // Read the image data
    png_bytep* row_pointers = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);

    // Initializes the Window, Renderer and Texture
    if(SDL_initializations(&fp, &window, &renderer, &texture, png, info, width, height) != 0)
    {
        return 1;
    }

    // Set the texture data
    void* pixels = NULL;
    int pitch = NULL;
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
        fprintf(stderr, "Error: Could not lock texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return 1;
    }

    for (int y = 0; y < height; y++) {
        memcpy((char*)pixels + y * pitch, row_pointers[y], png_get_rowbytes(png, info));
    }
    SDL_UnlockTexture(texture);


    // Clear the window
    SDL_RenderClear(renderer);

    // Render the texture
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Update the window
    SDL_RenderPresent(renderer);


    // Waits for the user to close the window
    int done = 0;
    SDL_Event event;
    while (!done) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            }
            else if (event.type == SDL_WINDOWEVENT) 
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    // Update the aspect ratio of the window
                    int window_w, window_h;
                    SDL_GetWindowSize(window, &window_w, &window_h);
                    float aspect_ratio = (float)window_h / (float)window_w;

                    // Adjust the scale of the image to match the new aspect ratio
                    int image_w, image_h;
                    SDL_QueryTexture(texture, NULL, NULL, &image_w, &image_h);
                    float const image_aspect_ratio = (float)image_h / (float)image_w;
                    if (aspect_ratio > image_aspect_ratio) {
                        SDL_RenderSetScale(renderer, aspect_ratio / image_aspect_ratio, 1.0);
                    }
                    else {
                        SDL_RenderSetScale(renderer, 1.0, image_aspect_ratio / aspect_ratio);
                    }
                }
            }
        }

        // Clear the window
        SDL_RenderClear(renderer);

        // Render the texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Update the window
        SDL_RenderPresent(renderer);
    }



    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return 0;
}


int SDL_initializations(FILE* fp, SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, png_structpp *png, png_infopp* info, int width, int height)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: Could not initialize SDL: %s\n", SDL_GetError());
        png_destroy_read_struct(*png, *info, NULL);
        fclose(fp);
        return 1;
    }

    // Create the window
    *window = SDL_CreateWindow("PNG Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        fprintf(stderr, "Error: Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        png_destroy_read_struct(*png, *info, NULL);
        fclose(fp);
        return 1;
    }

    // Create the renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error: Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        png_destroy_read_struct(*png, *info, NULL);
        fclose(fp);
        return 1;
    }

    // Create the texture
    *texture = SDL_CreateTexture(*renderer, SDL_GetWindowPixelFormat(*window), SDL_TEXTUREACCESS_STREAMING, width, height);//SDL_PIXELFORMAT_RGBA8888
    if (!texture) {
        fprintf(stderr, "Error: Could not create texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        png_destroy_read_struct(*png, *info, NULL);
        fclose(fp);
        return 1;
    }

    return 0;
}