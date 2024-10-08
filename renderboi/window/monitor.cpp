#include "monitor.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace rb::Window {

unsigned int Monitor::_count = 0;

Monitor::Monitor(const std::string name) :
    id(_count++),
    name(name)
{

}

Monitor::~Monitor() {
    
}

Monitor::GammaRamp::GammaRamp(const unsigned int size) :
    _size(size)
{

}

Monitor::GammaRamp::GammaRamp(const unsigned int size, const std::vector<unsigned short> ramp) :
    _size(size),
    _red(ramp),
    _green(ramp),
    _blue(ramp)
{
    if (ramp.size() != size) throw std::runtime_error("GammaRamp: inconsistent array size.");
}

Monitor::GammaRamp::GammaRamp(
    const unsigned int size,
    const std::vector<unsigned short> red,
    const std::vector<unsigned short> green,
    const std::vector<unsigned short> blue
) :
    _size(size),
    _red(red),
    _green(green),
    _blue(blue)
{
    if (red.size()   != size ||
        green.size() != size ||
        blue.size()  != size)
        throw std::runtime_error("GammaRamp: inconsistent array size.");
}

unsigned int Monitor::GammaRamp::getSize() const {
    return _size;
}

const std::vector<unsigned short>& Monitor::GammaRamp::getRed() const {
    return _red;
}

void Monitor::GammaRamp::setRed(const std::vector<unsigned short> red) {
    if (red.size() != _size) throw std::runtime_error("GammaRamp: cannot set red ramp component of different size.");

    _red = red;
}

const std::vector<unsigned short>& Monitor::GammaRamp::getGreen() const {
    return _green;
}

void Monitor::GammaRamp::setGreen(const std::vector<unsigned short> green) {
    if (green.size() != _size) throw std::runtime_error("GammaRamp: cannot set green ramp component of different size.");

    _green = green;
}

const std::vector<unsigned short>& Monitor::GammaRamp::getBlue() const {
    return _blue;
}

void Monitor::GammaRamp::setBlue(const std::vector<unsigned short> blue) {
    if (blue.size() != _size) throw std::runtime_error("GammaRamp: cannot set blue ramp component of different size.");

    _blue = blue;
}

Monitor::GammaRamp Monitor::GammaRamp::FromExponent(const unsigned int size, const float e) {
    return FromExponents(size, e, e, e);
}

Monitor::GammaRamp Monitor::GammaRamp::FromExponents(
    const unsigned int size,
    const float eRed,
    const float eGreen,
    const float eBlue
) {
    std::vector<unsigned short> red = std::vector<unsigned short>(size);
    std::vector<unsigned short> green = std::vector<unsigned short>(size);
    std::vector<unsigned short> blue = std::vector<unsigned short>(size);

    for (unsigned int i = 0; i < size; i++)
    {
        red.push_back(   (unsigned short) ( pow((float) i, (1.0f / eRed)   )));
        green.push_back( (unsigned short) ( pow((float) i, (1.0f / eGreen) )));
        blue.push_back(  (unsigned short) ( pow((float) i, (1.0f / eBlue)  )));
    }

    return GammaRamp(size, red, green, blue);
}

} // namespace rb::Window