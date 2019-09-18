static Janet cfun_LoadImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    const char *fileName = janet_getcstring(argv, 0);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = LoadImage(fileName);
    return janet_wrap_abstract(image);
}

static Janet cfun_LoadImageEx(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    JanetView pixels = janet_getindexed(argv, 0);
    Color *raw_pixels = janet_smalloc(sizeof(Color) * pixels.len);
    for (int32_t i = 0; i < pixels.len; i++) {
        raw_pixels[i] = jaylib_getcolor(pixels.items, i);
    }
    int width = janet_getinteger(argv, 1);
    int height = janet_getinteger(argv, 2);
    LoadImageEx(raw_pixels, width, height);
    return janet_wrap_nil();
}

static Janet cfun_ExportImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    const char *fileName = janet_getcstring(argv, 1);
    ExportImage(image, fileName);
    return janet_wrap_nil();
}

static Janet cfun_ExportImageAsCode(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    const char *fileName = janet_getcstring(argv, 1);
    ExportImageAsCode(image, fileName);
    return janet_wrap_nil();
}

static Janet cfun_LoadTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    const char *fileName = janet_getcstring(argv, 0);
    Texture2D *texture = janet_abstract(&AT_Texture2D, sizeof(Texture2D));
    *texture = LoadTexture(fileName);
    return janet_wrap_abstract(texture);
}

static Janet cfun_LoadTextureFromImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    Texture2D *texture = janet_abstract(&AT_Texture2D, sizeof(Texture2D));
    *texture = LoadTextureFromImage(image);
    return janet_wrap_abstract(texture);
}

static Janet cfun_LoadTextureCubemap(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    const uint8_t *kw = janet_getkeyword(argv, 1);
    int layoutType = 0;
    if (!janet_cstrcmp(kw, "auto-detect")) {
        layoutType = CUBEMAP_AUTO_DETECT;
    } else if (!janet_cstrcmp(kw, "line-vertical")) {
        layoutType = CUBEMAP_LINE_VERTICAL;
    } else if (!janet_cstrcmp(kw, "line-horizontal")) {
        layoutType = CUBEMAP_LINE_HORIZONTAL;
    } else if (!janet_cstrcmp(kw, "3x4")) {
        layoutType = CUBEMAP_CROSS_THREE_BY_FOUR;
    } else if (!janet_cstrcmp(kw,  "4x3")) {
        layoutType = CUBEMAP_CROSS_FOUR_BY_THREE;
    } else if (!janet_cstrcmp(kw, "panorama")) {
        layoutType = CUBEMAP_PANORAMA;
    } else {
        janet_panicf("unknown layout type %v", argv[1]);
    }
    TextureCubemap *cubemap = janet_abstract(&AT_TextureCubemap, sizeof(TextureCubemap));
    *cubemap = LoadTextureCubemap(image, layoutType);
    return janet_wrap_abstract(cubemap);
}

static Janet cfun_LoadRenderTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    int width = janet_getinteger(argv, 0);
    int height = janet_getinteger(argv, 1);
    RenderTexture *texture = janet_abstract(&AT_RenderTexture, sizeof(RenderTexture));
    *texture = LoadRenderTexture(width, height);
    return janet_wrap_abstract(texture);
}

static Janet cfun_UnloadImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    UnloadImage(image);
    return janet_wrap_nil();
}

static Janet cfun_UnloadTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    UnloadTexture(texture);
    return janet_wrap_nil();
}

static Janet cfun_UnloadRenderTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    RenderTexture texture = *jaylib_getrendertexture(argv, 0);
    UnloadRenderTexture(texture);
    return janet_wrap_nil();
}

static Janet cfun_GetTextureData(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GetTextureData(texture);
    return janet_wrap_abstract(image);
}

static Janet cfun_GetScreenData(int32_t argc, Janet *argv) {
    (void) argv;
    janet_fixarity(argc, 0);
    Image *image = janet_abstract(&AT_Image, sizeof(Image));
    *image = GetScreenData();
    return janet_wrap_abstract(image);
}

static Janet cfun_ImageCopy(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image image = *jaylib_getimage(argv, 0);
    Image *newImage = janet_abstract(&AT_Image, sizeof(Image));
    *newImage = ImageCopy(image);
    return janet_wrap_abstract(newImage);
}

static Janet cfun_ImageFromImage(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image image = *jaylib_getimage(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    Image *newImage = janet_abstract(&AT_Image, sizeof(Image));
    *newImage = ImageFromImage(image, rect);
    return janet_wrap_abstract(newImage);
}

static Janet cfun_ImageToPOT(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Color color = jaylib_getcolor(argv, 1);
    ImageToPOT(image, color);
    return argv[0];
}

static Janet cfun_ImageFormat(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    int format = jaylib_getpixelformat(argv, 1);
    ImageFormat(image, format);
    return argv[0];
}

static Janet cfun_ImageAlphaMask(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Image *mask = jaylib_getimage(argv, 1);
    ImageAlphaMask(image, *mask);
    return argv[0];
}

static Janet cfun_ImageAlphaClear(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    Color color = jaylib_getcolor(argv, 1);
    float threshold = (float) janet_getnumber(argv, 2);
    ImageAlphaClear(image, color, threshold);
    return argv[0];
}

static Janet cfun_ImageAlphaCrop(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    float threshold = (float) janet_getnumber(argv, 1);
    ImageAlphaCrop(image, threshold);
    return argv[0];
}

static Janet cfun_ImageAlphaPremultiply(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageAlphaPremultiply(image);
    return argv[0];
}

static Janet cfun_ImageCrop(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    ImageCrop(image, rect);
    return argv[0];
}

static Janet cfun_ImageResize(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    int newWidth = janet_getinteger(argv, 1);
    int newHeight = janet_getinteger(argv, 2);
    ImageResize(image, newWidth, newHeight);
    return argv[0];
}

static Janet cfun_ImageResizeNN(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Image *image = jaylib_getimage(argv, 0);
    int newWidth = janet_getinteger(argv, 1);
    int newHeight = janet_getinteger(argv, 2);
    ImageResizeNN(image, newWidth, newHeight);
    return argv[0];
}

static Janet cfun_ImageResizeCanvas(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    Image *image = jaylib_getimage(argv, 0);
    int newWidth = janet_getinteger(argv, 1);
    int newHeight = janet_getinteger(argv, 2);
    int offsetX = janet_getinteger(argv, 3);
    int offsetY = janet_getinteger(argv, 4);
    Color color = jaylib_getcolor(argv, 5);
    ImageResizeCanvas(image, newWidth, newHeight, offsetX, offsetY, color);
    return argv[0];
}

static Janet cfun_ImageMipmaps(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    Image *image = jaylib_getimage(argv, 0);
    ImageMipmaps(image);
    return argv[0];
}

static Janet cfun_ImageDither(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Image *image = jaylib_getimage(argv, 0);
    int rBpp = janet_getinteger(argv, 1);
    int gBpp = janet_getinteger(argv, 2);
    int bBpp = janet_getinteger(argv, 3);
    int aBpp = janet_getinteger(argv, 4);
    ImageDither(image, rBpp, gBpp, bBpp, aBpp);
    return argv[0];
}

static Janet cfun_ImageExtractPalette(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    Image *image = jaylib_getimage(argv, 0);
    int maxPaletteSize = janet_getinteger(argv, 1);
    int extractCount = 0;
    Color *colors = ImageExtractPalette(*image, maxPaletteSize, &extractCount);
    JanetArray *acolors = janet_array(extractCount);
    for (int i = 0; i < extractCount; i++) {
        Color c = colors[i];
        Janet *t = janet_tuple_begin(4);
        t[0] = janet_wrap_integer(c.r);
        t[1] = janet_wrap_integer(c.g);
        t[2] = janet_wrap_integer(c.b);
        t[3] = janet_wrap_integer(c.a);
        janet_array_push(acolors, janet_wrap_tuple(janet_tuple_end(t)));
    }
    return janet_wrap_array(acolors);
}

/*
// Image manipulation functions
RLAPI Image ImageText(const char *text, int fontSize, Color color);                                      // Create an image from text (default font)
RLAPI Image ImageTextEx(Font font, const char *text, float fontSize, float spacing, Color tint);         // Create an image from text (custom sprite font)
RLAPI void ImageDraw(Image *dst, Image src, Rectangle srcRec, Rectangle dstRec, Color tint);             // Draw a source image within a destination image (tint applied to source)
RLAPI void ImageDrawRectangle(Image *dst, Rectangle rec, Color color);                                   // Draw rectangle within an image
RLAPI void ImageDrawRectangleLines(Image *dst, Rectangle rec, int thick, Color color);                   // Draw rectangle lines within an image
RLAPI void ImageDrawText(Image *dst, Vector2 position, const char *text, int fontSize, Color color);     // Draw text (default font) within an image (destination)
RLAPI void ImageDrawTextEx(Image *dst, Vector2 position, Font font, const char *text, float fontSize, float spacing, Color color); // Draw text (custom sprite font) within an image (destination)
RLAPI void ImageFlipVertical(Image *image);                                                              // Flip image vertically
RLAPI void ImageFlipHorizontal(Image *image);                                                            // Flip image horizontally
RLAPI void ImageRotateCW(Image *image);                                                                  // Rotate image clockwise 90deg
RLAPI void ImageRotateCCW(Image *image);                                                                 // Rotate image counter-clockwise 90deg
RLAPI void ImageColorTint(Image *image, Color color);                                                    // Modify image color: tint
RLAPI void ImageColorInvert(Image *image);                                                               // Modify image color: invert
RLAPI void ImageColorGrayscale(Image *image);                                                            // Modify image color: grayscale
RLAPI void ImageColorContrast(Image *image, float contrast);                                             // Modify image color: contrast (-100 to 100)
RLAPI void ImageColorBrightness(Image *image, int brightness);                                           // Modify image color: brightness (-255 to 255)
RLAPI void ImageColorReplace(Image *image, Color color, Color replace);                                  // Modify image color: replace color
*/


static Janet cfun_DrawTexture(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 4);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    int x = janet_getinteger(argv, 1);
    int y = janet_getinteger(argv, 2);
    Color color = jaylib_getcolor(argv, 3);
    DrawTexture(texture, x, y, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureV(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 3);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Vector2 position = jaylib_castvec2(argv[1]);
    Color color = jaylib_getcolor(argv, 2);
    DrawTextureV(texture, position, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureEx(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Vector2 position = jaylib_castvec2(argv[1]);
    float rotation = (float) janet_getnumber(argv, 2);
    float scale = (float) janet_getnumber(argv, 3);
    Color color = jaylib_getcolor(argv, 4);
    DrawTextureEx(texture, position, rotation, scale, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureRec(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 4);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    Vector2 position = jaylib_castvec2(argv[2]);
    Color color = jaylib_getcolor(argv, 3);
    DrawTextureRec(texture, rect, position, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTextureQuad(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 5);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Vector2 tiling = jaylib_castvec2(argv[1]);
    Vector2 offset = jaylib_castvec2(argv[2]);
    Rectangle quad = jaylib_getrect(argv, 3);
    Color color = jaylib_getcolor(argv, 3);
    DrawTextureQuad(texture, tiling, offset, quad, color);
    return janet_wrap_nil();
}

static Janet cfun_DrawTexturePro(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 6);
    Texture2D texture = *jaylib_gettexture2d(argv, 0);
    Rectangle rect = jaylib_getrect(argv, 1);
    Rectangle dest = jaylib_getrect(argv, 2);
    Vector2 position = jaylib_castvec2(argv[3]);
    float rotation = janet_getnumber(argv, 4);
    Color color = jaylib_getcolor(argv, 5);
    DrawTexturePro(texture, rect, dest, position, rotation, color);
    return janet_wrap_nil();
}

/*
// Image/Texture2D data loading/unloading/saving functions
RLAPI Image LoadImagePro(void *data, int width, int height, int format);                                 // Load image from raw data with parameters
RLAPI Image LoadImageRaw(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
RLAPI Color *GetImageData(Image image);                                                                  // Get pixel data from image as a Color struct array
RLAPI Vector4 *GetImageDataNormalized(Image image);                                                      // Get pixel data from image as Vector4 array (float normalized)
RLAPI Rectangle GetImageAlphaBorder(Image image, float threshold);                                       // Get image alpha border rectangle
RLAPI int GetPixelDataSize(int width, int height, int format);                                           // Get pixel data size in bytes (image or texture)
RLAPI void UpdateTexture(Texture2D texture, const void *pixels);                                         // Update GPU texture with new data

// Image generation functions
RLAPI Image GenImageColor(int width, int height, Color color);                                           // Generate image: plain color
RLAPI Image GenImageGradientV(int width, int height, Color top, Color bottom);                           // Generate image: vertical gradient
RLAPI Image GenImageGradientH(int width, int height, Color left, Color right);                           // Generate image: horizontal gradient
RLAPI Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer);      // Generate image: radial gradient
RLAPI Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2);    // Generate image: checked
RLAPI Image GenImageWhiteNoise(int width, int height, float factor);                                     // Generate image: white noise
RLAPI Image GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale);           // Generate image: perlin noise
RLAPI Image GenImageCellular(int width, int height, int tileSize);                                       // Generate image: cellular algorithm. Bigger tileSize means bigger cells

// Texture2D configuration functions
RLAPI void GenTextureMipmaps(Texture2D *texture);                                                        // Generate GPU mipmaps for a texture
RLAPI void SetTextureFilter(Texture2D texture, int filterMode);                                          // Set texture scaling filter mode
RLAPI void SetTextureWrap(Texture2D texture, int wrapMode);                                              // Set texture wrapping mode

// Texture2D drawing functions
RLAPI void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle destRec, Vector2 origin, float rotation, Color tint);  // Draws a texture (or part of it) that stretches or shrinks nicely
*/

static const JanetReg image_cfuns[] = {
    {"load-image-1", cfun_LoadImage, NULL}, // load-image is janet core function, don't want to overwrite if we use (use jaylib)
    {"load-image-ex", cfun_LoadImageEx, NULL},
    {"export-image", cfun_ExportImage, NULL},
    {"export-image-as-code", cfun_ExportImageAsCode, NULL},
    {"load-texture", cfun_LoadTexture, NULL},
    {"load-texture-from-image", cfun_LoadTextureFromImage,  NULL},
    {"load-texture-cubemap", cfun_LoadTextureCubemap, NULL},
    {"load-render-texture", cfun_LoadRenderTexture, NULL},
    {"unload-image", cfun_UnloadImage, NULL},
    {"unload-texture", cfun_UnloadTexture, NULL},
    {"unload-render-texture", cfun_UnloadRenderTexture, NULL},
    {"get-texture-data", cfun_GetTextureData, NULL},
    {"get-screen-data", cfun_GetScreenData, NULL},
    {"image-copy", cfun_ImageCopy, NULL},
    {"image-from-image", cfun_ImageFromImage, NULL},
    {"image-to-pot", cfun_ImageToPOT, NULL},
    {"image-format", cfun_ImageFormat, NULL},
    {"image-alpha-mask", cfun_ImageAlphaMask, NULL},
    {"image-alpha-clear", cfun_ImageAlphaClear, NULL},
    {"image-alpha-crop", cfun_ImageAlphaCrop, NULL},
    {"image-alpha-premultiply", cfun_ImageAlphaPremultiply, NULL},
    {"image-crop", cfun_ImageCrop, NULL},
    {"image-resize", cfun_ImageResize, NULL},
    {"image-resize-nn", cfun_ImageResizeNN, NULL},
    {"image-resize-canvas", cfun_ImageResizeCanvas, NULL},
    {"image-mipmaps", cfun_ImageMipmaps, NULL},
    {"image-dither", cfun_ImageDither, NULL},
    {"image-extract-pallete", cfun_ImageExtractPalette, NULL},
    {"draw-texture", cfun_DrawTexture, NULL},
    {"draw-texture-v", cfun_DrawTextureV, NULL},
    {"draw-texture-ex", cfun_DrawTextureEx, NULL},
    {"draw-texture-pro", cfun_DrawTexturePro, NULL},
    {"draw-texture-quad", cfun_DrawTextureQuad, NULL},
    {"draw-texture-rec", cfun_DrawTextureRec, NULL},
    {NULL, NULL, NULL}
};
