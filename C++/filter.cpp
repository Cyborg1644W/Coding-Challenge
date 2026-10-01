#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct RGBTRIPLE
{
    uint8_t rgbtBlue;
    uint8_t rgbtGreen;
    uint8_t rgbtRed;
};
#pragma pack(pop)

class BMPImage
{
public:
    BITMAPFILEHEADER fileHeader{};
    BITMAPINFOHEADER infoHeader{};
    std::vector<std::vector<RGBTRIPLE>> pixels; // pixels[row][col], row 0 = top of image

    bool load(const std::string &filename)
    {
        std::ifstream in(filename, std::ios::binary);
        if (!in)
        {
            std::cerr << "Error: could not open input file \"" << filename << "\"\n";
            return false;
        }

        in.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
        in.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

        if (fileHeader.bfType != 0x4D42 || infoHeader.biBitCount != 24 || infoHeader.biCompression != 0)
        {
            std::cerr << "Error: only uncompressed 24-bit BMP files are supported.\n";
            return false;
        }

        int width = infoHeader.biWidth;
        int height = std::abs(infoHeader.biHeight);
        bool bottomUp = infoHeader.biHeight > 0;

        int rowSize = ((width * 3 + 3) / 4) * 4; // BMP rows are padded to a multiple of 4 bytes
        int padding = rowSize - width * 3;

        pixels.assign(height, std::vector<RGBTRIPLE>(width));

        in.seekg(fileHeader.bfOffBits, std::ios::beg);

        for (int i = 0; i < height; i++)
        {
            int row = bottomUp ? (height - 1 - i) : i;
            for (int j = 0; j < width; j++)
            {
                in.read(reinterpret_cast<char *>(&pixels[row][j]), sizeof(RGBTRIPLE));
            }
            in.ignore(padding);
        }

        return true;
    }

    bool save(const std::string &filename)
    {
        std::ofstream out(filename, std::ios::binary);
        if (!out)
        {
            std::cerr << "Error: could not open output file \"" << filename << "\"\n";
            return false;
        }

        int height = static_cast<int>(pixels.size());
        int width = height > 0 ? static_cast<int>(pixels[0].size()) : 0;
        int rowSize = ((width * 3 + 3) / 4) * 4;
        int padding = rowSize - width * 3;

        // Always write bottom-up with a positive height, regardless of how the
        // source file stored it.
        BITMAPINFOHEADER outInfo = infoHeader;
        outInfo.biWidth = width;
        outInfo.biHeight = height;

        out.write(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
        out.write(reinterpret_cast<char *>(&outInfo), sizeof(outInfo));

        static const uint8_t padBytes[3] = {0, 0, 0};
        for (int i = height - 1; i >= 0; i--)
        {
            for (int j = 0; j < width; j++)
            {
                out.write(reinterpret_cast<const char *>(&pixels[i][j]), sizeof(RGBTRIPLE));
            }
            out.write(reinterpret_cast<const char *>(padBytes), padding);
        }

        return true;
    }

    int width() const { return pixels.empty() ? 0 : static_cast<int>(pixels[0].size()); }
    int height() const { return static_cast<int>(pixels.size()); }
};

static uint8_t clamp255(double value)
{
    return static_cast<uint8_t>(std::round(std::min(255.0, std::max(0.0, value))));
}

void applyGrayscale(BMPImage &img)
{
    for (auto &row : img.pixels)
    {
        for (auto &p : row)
        {
            uint8_t avg = clamp255((p.rgbtRed + p.rgbtGreen + p.rgbtBlue) / 3.0);
            p.rgbtRed = p.rgbtGreen = p.rgbtBlue = avg;
        }
    }
}

void applySepia(BMPImage &img)
{
    for (auto &row : img.pixels)
    {
        for (auto &p : row)
        {
            double r = p.rgbtRed, g = p.rgbtGreen, b = p.rgbtBlue;
            uint8_t sepiaRed = clamp255(0.393 * r + 0.769 * g + 0.189 * b);
            uint8_t sepiaGreen = clamp255(0.349 * r + 0.686 * g + 0.168 * b);
            uint8_t sepiaBlue = clamp255(0.272 * r + 0.534 * g + 0.131 * b);
            p.rgbtRed = sepiaRed;
            p.rgbtGreen = sepiaGreen;
            p.rgbtBlue = sepiaBlue;
        }
    }
}

void applyReflect(BMPImage &img)
{
    for (auto &row : img.pixels)
    {
        std::reverse(row.begin(), row.end());
    }
}

void applyBlur(BMPImage &img)
{
    auto original = img.pixels;
    int h = img.height(), w = img.width();

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            double sumR = 0, sumG = 0, sumB = 0;
            int count = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < h && nj >= 0 && nj < w)
                    {
                        sumR += original[ni][nj].rgbtRed;
                        sumG += original[ni][nj].rgbtGreen;
                        sumB += original[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }
            img.pixels[i][j].rgbtRed = clamp255(sumR / count);
            img.pixels[i][j].rgbtGreen = clamp255(sumG / count);
            img.pixels[i][j].rgbtBlue = clamp255(sumB / count);
        }
    }
}

void applyEdges(BMPImage &img)
{
    auto original = img.pixels;
    int h = img.height(), w = img.width();

    static const int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    static const int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            double gxR = 0, gxG = 0, gxB = 0;
            double gyR = 0, gyG = 0, gyB = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < h && nj >= 0 && nj < w)
                    {
                        const RGBTRIPLE &p = original[ni][nj];
                        int kx = gx[di + 1][dj + 1];
                        int ky = gy[di + 1][dj + 1];

                        gxR += kx * p.rgbtRed;
                        gxG += kx * p.rgbtGreen;
                        gxB += kx * p.rgbtBlue;

                        gyR += ky * p.rgbtRed;
                        gyG += ky * p.rgbtGreen;
                        gyB += ky * p.rgbtBlue;
                    }
                }
            }

            img.pixels[i][j].rgbtRed = clamp255(std::sqrt(gxR * gxR + gyR * gyR));
            img.pixels[i][j].rgbtGreen = clamp255(std::sqrt(gxG * gxG + gyG * gyG));
            img.pixels[i][j].rgbtBlue = clamp255(std::sqrt(gxB * gxB + gyB * gyB));
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: ./filter [flag] infile outfile\n"
                   << "Flags:\n"
                   << "  -g  grayscale\n"
                   << "  -s  sepia\n"
                   << "  -r  reflect (mirror)\n"
                   << "  -b  blur\n"
                   << "  -e  edges\n";
        return 1;
    }

    std::string flag = argv[1];
    std::string infile = argv[2];
    std::string outfile = argv[3];

    BMPImage img;
    if (!img.load(infile))
    {
        return 1;
    }

    if (flag == "-g")
        applyGrayscale(img);
    else if (flag == "-s")
        applySepia(img);
    else if (flag == "-r")
        applyReflect(img);
    else if (flag == "-b")
        applyBlur(img);
    else if (flag == "-e")
        applyEdges(img);
    else
    {
        std::cerr << "Error: invalid filter flag \"" << flag << "\"\n";
        return 1;
    }

    if (!img.save(outfile))
    {
        return 1;
    }

    std::cout << "Filter applied successfully. Saved to \"" << outfile << "\"\n";
    return 0;
}