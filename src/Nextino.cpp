#include <Nextino.h>
#include <sstream>
#include <iomanip>

using namespace Nextino;

constexpr int clamp(int x, int min, int max)
{
    return (x > max) ? max : (x < min) ? min
                                       : x;
}

const uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t r5 = map(r, 0x00, 0xff, 0x00, 0x1f);
    uint16_t g6 = map(g, 0x00, 0xff, 0x00, 0x3f);
    uint16_t b5 = map(b, 0x00, 0xff, 0x00, 0x1f);

    return (r5 << 11) | (g6 << 5) | b5;
}

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
    std::stringstream name;
    name << "p[" << pageId << "].b[" << elementId << "]";

    _name = name.str();
}

void Element::setAttribute(std::string attribute, std::string value, bool quote)
{
    std::string msg = _name + "." + attribute + "=" + (quote ? "\"" + value + "\"" : value);
    _display.write(msg);
}

void Element::setAttribute(std::string attribute, int32_t value)
{
    std::stringstream msg;
    msg << _name << "." << attribute << "=" << value;

    _display.write(msg.str());
}

void Element::setAttribute(std::string attribute, uint32_t value)
{
    std::stringstream msg;
    msg << _name << "." << attribute << "=" << value;

    _display.write(msg.str());
}

void Element::setColor(std::string attribute, uint8_t r, uint8_t g, uint8_t b)
{
    setAttribute(attribute, rgb888_to_rgb565(r, g, b));
}

void Nextino::Element::setColor(std::string attribute, uint32_t color)
{
    setAttribute(attribute, rgb888_to_rgb565(((color >> 16) & 0xFF), ((color >> 8) & 0xFF), (color & 0xFF)));
}

void Element::setForegroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    setColor("pco", r, g, b);
}

void Nextino::Element::setForegroundColor(uint32_t color)
{
    setColor("pco", color);
}

void Element::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    setColor("bco", r, g, b);
}

void Nextino::Element::setBackgroundColor(uint32_t color)
{
    setColor("bco", color);
}

void Text::setText(std::string text)
{
    setAttribute("txt", text);
}

void Number::setValue(int value)
{
    setAttribute("val", value);
}

void XFloat::setValue(float value, int precision)
{
    std::stringstream vs;
    if (precision > 0)
        vs << std::setprecision(precision);
    vs << value;

    std::string val = vs.str();
    size_t dot = val.find('.');

    val.erase(dot, 1);

    int vvs = (value < 0) ? dot - 1 : dot;

    setAttribute("val", val, false);
    setAttribute("vvs0", vvs);
    setAttribute("vvs1", val.length() - vvs);
}

ProgressBar::ProgressBar(Display &display, std::string objname, int min, int max) : Element(display, objname), _min(min), _max(max)
{
}

Nextino::ProgressBar::ProgressBar(Display &display, int pageId, int elementId, int min, int max) : Element(display, pageId, elementId), _min(min), _max(max)
{
}

void ProgressBar::setValue(int value)
{
    value = clamp(value, _min, _max);

    setValue(map(value, _min, _max, 0, 100));
}

Nextino::Gauge::Gauge(Display &display, std::string objname, int min, int max, int start, int end) : Element(display, objname), _min(min), _max(max), _start(start), _end(end)
{
}

Nextino::Gauge::Gauge(Display &display, int pageId, int elementId, int min, int max, int start, int end) : Element(display, pageId, elementId), _min(min), _max(max), _start(start), _end(end)
{
}

void Gauge::setValue(int value)
{
    value = clamp(value, _min, _max);

    int mapped = map(value, _min, _max, _start, _end);
    mapped = clamp(mapped, _start, _end);

    setValue(mapped);
}
