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

        void setProperty(std::string property, std::string value);

        void setProperty(std::string property, int value);
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
};

#endif // NEXTINO_H