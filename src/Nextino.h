#ifndef NEXTINO_H
#define NEXTINO_H

#include <ArduinoSTL.h>
#include <iomanip>
#include <sstream>
#include <string>

namespace Nextino
{
    class Display
    {
    private:
        std::ihserialstream _inStream;
        std::ohserialstream _outStream;

    public:
        template <typename T>
        Display(T &stream) : _inStream(stream), _outStream(stream)
        {
        }

        void write(std::string msg);
        std::string read();
    };

    class Component
    {
    private:
        Display &_display;
        uint8_t _pageId, _componentId;

    public:
        // TODO: get cid from the display using the objname
        // Component(Display &display, uint8_t pageId, std::string objname);
        Component(Display &display, uint8_t pageId, uint8_t componentId);

        void setAttribute(std::string attribute, std::string value, bool quote = true);

        template <typename T>
        void setAttribute(std::string attribute, T value, int precision = 0)
        {
            std::stringstream msg;
            if (precision > 0)
                msg << std::setprecision(precision);
            msg << value;

            setAttribute(attribute, msg.str(), false);
        }

        void setColor(std::string attribute, uint8_t r, uint8_t g, uint8_t b);
        void setColor(std::string attribute, uint32_t color);

        void setForegroundColor(uint8_t r, uint8_t g, uint8_t b);
        void setForegroundColor(uint32_t color);

        void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
        void setBackgroundColor(uint32_t color);
    };

    class Text : public Component
    {
    public:
        using Component::Component;

        void setText(std::string text);
    };

    class Number : public Component
    {
    public:
        using Component::Component;

        void setValue(int value);
    };

    class XFloat : public Component
    {
    public:
        using Component::Component;

        void setValue(float value, int precision = 0);
    };

    class ProgressBar : public Component
    {
    private:
        int _min, _max;

    public:
        // ProgressBar(Display &display, uint8_t pageId, std::string objname, int min = 0, int max = 100);
        ProgressBar(Display &display, uint8_t pageId, uint8_t elementId, int min = 0, int max = 100);

        void setValue(int value);
    };

    class Gauge : public Component
    {
    private:
        int _min, _max, _start, _end;

    public:
        // Gauge(Display &display, uint8_t pageId, std::string objname, int min, int max, int start = 0, int end = 360);
        Gauge(Display &display, uint8_t pageId, uint8_t elementId, int min, int max, int start = 0, int end = 360);

        void setValue(int value);
    };
};

#endif // NEXTINO_H