// Shows every Canvas drawing function (Member 6) and both export formats
// (Member 7). Saves data/output/shapes_example.png and .ppm.
// Run from the project root: ./build/examples/shapes_example

#include <vizlib/canvas.hpp>
#include <vizlib/canvas_export.hpp>
#include <vizlib/text.hpp>

#include <exception>
#include <filesystem>
#include <iostream>

int main()
{
    try
    {
        const vizlib::Color white{255, 255, 255, 255};
        const vizlib::Color dark{60, 60, 60, 255};
        const vizlib::Color blue{31, 119, 180, 255};
        const vizlib::Color orange{255, 127, 14, 255};
        const vizlib::Color red{214, 39, 40, 255};
        vizlib::Canvas canvas(600, 260, white);
        const vizlib::TextRenderer text("third_party/fonts/DejaVuSans.ttf");

        // lines: Bresenham, 1 and 5 pixels thick, in two directions
        canvas.draw_line({30, 60}, {150, 200}, dark);
        canvas.draw_line({30, 200}, {150, 60}, blue, 5);

        // rectangles: an outline and a filled one
        canvas.draw_rect({190, 60, 120, 140}, dark);
        canvas.fill_rect({210, 80, 80, 100}, orange);

        // circles: midpoint outline and filled
        canvas.draw_circle({390, 130}, 60, dark);
        canvas.fill_circle({520, 130}, 50, red);

        // a single pixel, and a shape that runs off the edge (clipped safely)
        canvas.set_pixel(300, 230, dark);
        canvas.fill_circle({600, 250}, 30, blue);

        text.draw_text(canvas, "draw_line", {50, 40}, 14, dark);
        text.draw_text(canvas, "draw_rect, fill_rect", {180, 40}, 14, dark);
        text.draw_text(canvas, "draw_circle", {350, 40}, 14, dark);
        text.draw_text(canvas, "fill_circle", {485, 40}, 14, dark);

        const char* png = "data/output/shapes_example.png";
        const char* ppm = "data/output/shapes_example.ppm";
        if (!vizlib::save_png(canvas, png) || !vizlib::save_ppm(canvas, ppm))
        {
            std::cerr << "Could not save the images\n";
            return 1;
        }
        std::cout << "Saved " << png << " and " << ppm << " ("
                  << std::filesystem::file_size(ppm) << " bytes: a 15-byte header plus 600 x 260 x 3)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
