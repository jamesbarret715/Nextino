#ifndef NEXTINO_H
#define NEXTINO_H

#include <ArduinoSTL.h>
#include <string>

namespace Nextino
{
    class Display
    {
    private:
        Stream *_stream;

    public:
        template <typename T>
        Display(T &stream) : _stream(&stream)
        {
        }

        void write(std::string msg);
    };

    class Element
    {
    private:
        Display &_display;
        std::string _name;

    public:
        Element(Display &display, std::string objname);
        Element(Display &display, uint8_t pageId, uint8_t elementId);

        void setAttribute(std::string attribute, std::string value, bool quote = true);
        void setAttribute(std::string attribute, int32_t value);
        void setAttribute(std::string attribute, uint32_t value);

        void setColor(std::string attribute, uint8_t r, uint8_t g, uint8_t b);
        void setColor(std::string attribute, uint32_t color);

        void setForegroundColor(uint8_t r, uint8_t g, uint8_t b);
        void setForegroundColor(uint32_t color);

        void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
        void setBackgroundColor(uint32_t color);
    };

    class Text : public Element
    {
    public:
        using Element::Element;

        void setText(std::string text);
    };

    class Number : public Element
    {
    public:
        using Element::Element;

        void setValue(int value);
    };

    class XFloat : public Element
    {
    public:
        using Element::Element;

        void setValue(float value, int precision = 0);
    };

    class ProgressBar : public Element
    {
    private:
        int _min, _max;

    public:
        ProgressBar(Display &display, std::string objname, int min = 0, int max = 100);
        ProgressBar(Display &display, int pageId, int elementId, int min = 0, int max = 100);

        void setValue(int value);
    };

    class Gauge : public Element
    {
    private:
        int _min, _max, _start, _end;

    public:
        Gauge(Display &display, std::string objname, int min, int max, int start = 0, int end = 360);
        Gauge(Display &display, int pageId, int elementId, int min, int max, int start = 0, int end = 360);

        void setValue(int value);
    };
};

#endif // NEXTINO_H