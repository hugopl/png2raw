#include <png.h>
#include <cstdlib>
#include <cstdio>

static void error(const char* msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

int main(int argc, const char** argv)
{
    if (argc < 2)
        error("Use png2raw PNGFILE");

    const char* fileName = argv[1];
    FILE* fp = fopen(fileName, "r");
    if (!fp)
        error("file not found.");

    unsigned char sig[8];
    fread(sig, 1, 8, fp);

    if (!png_check_sig(sig, 8))
        error("This isn't a png file.");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop info = png_create_info_struct(png);

//     if (setjmp(png->jmpbuf)) {
//         png_destroy_read_struct(&png, &info, 0);
//         return 4;
//     }    

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    png_uint_32 width = png_get_image_width(png, info);
    png_uint_32 height = png_get_image_height(png, info);
    png_uint_32 bitDepth = png_get_bit_depth(png, info);
    png_uint_32 channels = png_get_channels(png, info);

    // Read png data
    png_bytep* rowPtrs = new png_bytep[height];
    unsigned long dataSize = width * height * bitDepth * channels / 8;
    unsigned char* data = new unsigned char[dataSize];
    // This is the length in bytes, of one row.
    const unsigned int stride = width * bitDepth * channels / 8;
    // Set row pointer to values on data
    for (size_t i = 0; i < height; ++i) {
        png_uint_32 q = (height - i - 1) * stride;
        rowPtrs[i] = (png_bytep)data + q;
    }
    png_read_image(png, rowPtrs);

    printf("tex. width: %d\n", width);
    printf("tex. height: %d\n", height);
    printf("tex. channels: %d\n", channels);

    puts("Saving raw data on texture.raw ...");
    FILE* out = fopen("texture.raw", "w");
    fwrite(data, 1, dataSize, out);
    fclose(out);

    delete[] rowPtrs;
    delete[] data;
    png_destroy_read_struct(&png, &info, 0);
    fclose(fp);
}
