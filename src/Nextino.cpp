#include <Nextino.h>

using namespace Nextino;

void Display::write(std::string msg)
{
#ifdef NEXTINO_DEBUG
    Serial.println(msg.c_str());
#endif

    _stream->print(msg.c_str());
    _stream->print("\xff\xff\xff");
}

Element::Element(Display &display, std::string objname) : _display(display), _name(objname)
{
}

Element::Element(Display &display, uint8_t pageId, uint8_t elementId) : _display(display)
{
    char pageNum[3], elementNum[3];
    itoa(pageId, pageNum, 10);
    itoa(elementId, elementNum, 10);

    _name = std::string("p[") + pageNum + "].b[" + elementNum + "]";
}

void Element::setProperty(std::string property, std::string value)
{
    std::string msg = _name + "." + property + "=\"" + value + "\"";
    _display.write(msg);
}

void Element::setProperty(std::string property, int value)
{
    char buf[10];
    itoa(value, buf, 10);

    std::string msg = _name + "." + property + "=" + buf;
    _display.write(msg);
}

void Text::setText(std::string text)
{
    setProperty("txt", text);
}

void Number::setValue(int value)
{
    setProperty("val", value);
}
