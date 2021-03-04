#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdint.h>
#include <QImage>
#include <QMessageBox>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage img;
    if (!img.load("../assets/tileset.png"))
    {
        QMessageBox::critical(this, "Converter", "Can't load assets");
        return;
    }
    img = img.convertToFormat(QImage::Format_ARGB32);

    std::stringstream stream_cpp;
    stream_cpp << "#include \"assets.h\"" << std::endl;
    stream_cpp << "#include <stdint.h>" << std::endl;
    stream_cpp << "#include <avr/pgmspace.h>" << std::endl;

    stream_cpp << std::endl;
    stream_cpp << "const uint8_t s_tiles[] PROGMEM  =" << std::endl;
    stream_cpp << "{" << std::endl;
    int index = 0;
    for (int yi = 0; yi < img.height()/8; ++yi)
    {
        for (int xi = 0; xi < img.width()/8; ++xi)
        {
            for (int x = 0; x < 8; ++x)
            {
                uint8_t value = 0;
                for (int y = 0; y < 8; ++y)
                {
                    uint32_t color = img.pixel(x + xi*8, y + yi*8);
                    if ( (color & 0xFF) > 128)
                        value |= 1 << y;
                }
                if (index % 32 == 0)
                    stream_cpp << "  ";
                stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)value)&0xFF) << ",";
                if (index % 32 == 31)
                    stream_cpp << std::endl;
                index ++;
            }
        }
    }
    stream_cpp << std::endl <<"};" << std::endl;

    QImage sprites;
    if (!sprites.load("../assets/spriteset.png"))
    {
        QMessageBox::critical(this, "Converter", "Can't load spriteset");
        return;
    }
    sprites = sprites.convertToFormat(QImage::Format_ARGB32);

    stream_cpp << std::endl;
    stream_cpp << "const uint8_t s_sprites[] PROGMEM  =" << std::endl;
    stream_cpp << "{" << std::endl;
    index = 0;
    for (int yi = 0; yi < sprites.height()/8; ++yi)
    {
        for (int xi = 0; xi < sprites.width()/8; ++xi)
        {
            if (index % 32 == 0)
                stream_cpp << "  ";
            stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << 8 << ",";
            if (index % 32 == 31)
                stream_cpp << std::endl;
            index ++;

            if (index % 32 == 0)
                stream_cpp << "  ";
            stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << 8 << ",";
            if (index % 32 == 31)
                stream_cpp << std::endl;
            index ++;

            for (int x = 0; x < 8; ++x)
            {
                uint8_t value = 0;
                uint8_t mask = 0;
                for (int y = 0; y < 8; ++y)
                {
                    uint32_t color = sprites.pixel(x + xi*8, y + yi*8);
                    uint8_t r = color & 0xFF;
                    uint8_t g = (color >> 8) & 0xFF;
                    uint8_t b = (color >> 16) & 0xFF;

                    if ( r > 200 && g > 200 && b > 200)
                    {
                        value |= 1 << y;
                        mask |= 1 << y;
                    }
                    if ( r < 32 && g < 32 && b < 32)
                        mask |= 1 << y;
                }
                if (index % 32 == 0)
                    stream_cpp << "  ";
                stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)value)&0xFF) << ",";
                if (index % 32 == 31)
                    stream_cpp << std::endl;
                index ++;

                if (index % 32 == 0)
                    stream_cpp << "  ";
                stream_cpp << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)mask)&0xFF) << ",";
                if (index % 32 == 31)
                    stream_cpp << std::endl;
                index ++;
            }
        }
    }
    stream_cpp << std::endl <<"};" << std::endl;


    FILE* file = fopen("../Bomberboy/assets.cpp", "wb");
    fwrite(stream_cpp.str().c_str(), 1, stream_cpp.str().length(), file);
    fclose(file);
}

MainWindow::~MainWindow()
{
    delete ui;
}
