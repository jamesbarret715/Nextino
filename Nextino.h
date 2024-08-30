#ifndef NEXTINO_H
#define NEXTINO_H

#include <string>

#include <Arduino.h>
#include <SoftwareSerial.h>

namespace Nextion
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

        void write(std::string msg)
        {
#ifdef NEXTION_DEBUG
            Serial.println(msg.c_str());
#endif

            _stream->print(msg.c_str());
            _stream->print("\xff\xff\xff");
        }
    };

    class Element
    {
    private:
        Display &_display;
        std::string _name;

    public:
        Element(Display &display, std::string objname) : _display(display), _name(objname)
        {
        }

        Element(Display &display, uint8_t pageId, uint8_t elementId) : _display(display)
        {
            char pageNum[3], elementNum[3];
            itoa(pageId, pageNum, 10);
            itoa(elementId, elementNum, 10);

            _name = std::string("p[") + pageNum + "].b[" + elementNum + "]";
        }

        void setProperty(std::string property, std::string value)
        {
            std::string msg = _name + "." + property + "=\"" + value + "\"";
            _display.write(msg);
        }

        void setProperty(std::string property, int value)
        {
            char buf[10];
            itoa(value, buf, 10);

            std::string msg = _name + "." + property + "=" + buf;
            _display.write(msg);
        }
    };

    class Text : public Element
    {
    public:
        using Element::Element;

        void setText(std::string text)
        {
            setProperty("txt", text);
        }
    };

    class Number : public Element
    {
    public:
        using Element::Element;

        void setValue(int value)
        {
            setProperty("val", value);
        }
    };
};

#endif // NEXTINO_H