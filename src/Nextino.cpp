#include <Nextino.h>

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
    std::cout << msg << std::endl;
#endif

    _outStream << msg << "\xff\xff\xff";
}

std::string Nextino::Display::read()
{
    std::string ret;
    std::getline(_inStream, ret, '\xff');

    return ret;
}

Component::Component(Display &display, uint8_t pageId, uint8_t componentId) : _display(display), _pageId(pageId), _componentId(componentId)
{
}

void Component::setAttribute(std::string attribute, std::string value, bool quote)
{
    std::stringstream msg;
    msg << "p[" << _pageId << "].b[" << _componentId << "." << attribute << "=";

    if (quote)
        msg << '"';

    msg << value;

    if (quote)
        msg << '"';

    _display.write(msg.str());
}

void Component::setColor(std::string attribute, uint8_t r, uint8_t g, uint8_t b)
{
    setAttribute(attribute, (uint32_t)rgb888_to_rgb565(r, g, b));
}

void Nextino::Component::setColor(std::string attribute, uint32_t color)
{
    setAttribute(attribute, (uint32_t)rgb888_to_rgb565(((color >> 16) & 0xFF), ((color >> 8) & 0xFF), (color & 0xFF)));
}

void Component::setForegroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    setColor("pco", r, g, b);
}

void Nextino::Component::setForegroundColor(uint32_t color)
{
    setColor("pco", color);
}

void Component::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    setColor("bco", r, g, b);
}

void Nextino::Component::setBackgroundColor(uint32_t color)
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

Nextino::ProgressBar::ProgressBar(Display &display, uint8_t pageId, uint8_t elementId, int min, int max) : Component(display, pageId, elementId), _min(min), _max(max)
{
}

void ProgressBar::setValue(int value)
{
    value = clamp(value, _min, _max);

    setValue(map(value, _min, _max, 0, 100));
}

Nextino::Gauge::Gauge(Display &display, uint8_t pageId, uint8_t elementId, int min, int max, int start, int end) : Component(display, pageId, elementId), _min(min), _max(max), _start(start), _end(end)
{
}

void Gauge::setValue(int value)
{
    value = clamp(value, _min, _max);

    int mapped = map(value, _min, _max, _start, _end);
    mapped = clamp(mapped, _start, _end);

    setValue(mapped);
}
