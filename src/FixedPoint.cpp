#include <utility>
#include <string>
#include "../include/FixedPoint.h"

FixedPoint::FixedPoint():
    value_(0) {}

FixedPoint::FixedPoint(long long intValue)
{
    // Add digits after decimal point
    value_ = intValue * multiplier_;
}

FixedPoint::FixedPoint(int intValue)
{
    // Add digits after decimal point
    value_ = static_cast<long>(intValue * multiplier_);
}
FixedPoint::FixedPoint(double fValue)
{
    // Add digits after decimal point
    value_ = static_cast<long long>(fValue * multiplier_);
}
FixedPoint::FixedPoint(FixedPoint& other):
    value_(other.value_) {}


FixedPoint::FixedPoint(FixedPoint&& other):
    value_(std::move(other.value_)) {}

FixedPoint::~FixedPoint() {}


FixedPoint& FixedPoint::operator=( FixedPoint& other)
{
    value_ = other.value_;
    return *this;
}
FixedPoint& FixedPoint::operator=( FixedPoint&& other)
{
    value_ = other.value_;
    return *this;
}

FixedPoint FixedPoint::operator+( FixedPoint& other)
{
	FixedPoint result;
	result.value_ = value_ + other.value_;
	return result;
    //return FixedPoint(value_ + other.value_);
}
FixedPoint& FixedPoint::operator+=( FixedPoint& other)
{
    value_ = value_ + other.value_;
    return *this;
}

FixedPoint FixedPoint::operator-( FixedPoint& other)
{   
    return FixedPoint(value_ - other.value_);
}

FixedPoint& FixedPoint::operator-=( FixedPoint& other)
{
    value_ = value_ - other.value_;
    return *this;
}

FixedPoint FixedPoint::operator*( FixedPoint& other)
{
    FixedPoint result;
    // Need to shift decimal point to the left after multiplication
    result.value_ = value_ * other.value_ / multiplier_;
    return result;
}

FixedPoint& FixedPoint::operator*=( FixedPoint& other)
{
    // Need to shift decimal point to the left after multiplication
    value_ = value_ * other.value_ / multiplier_;
    return *this;
}

FixedPoint FixedPoint::operator/( FixedPoint& other)
{
    FixedPoint result;
    if (value_ < other.value_)
        // To avoid cutoff from integer division 
        // apply multiplier first to value
        result.value_ = (multiplier_ * value_) / other.value_;
    else
        result.value_ = (value_ / other.value_) * multiplier_;

    return result;
}

FixedPoint& FixedPoint::operator/=( FixedPoint& other)
{
    if (value_ < other.value_)
        // To avoid cutoff from integer division 
        // apply multiplier first to value
        value_ = (multiplier_ * value_) / other.value_;
    else
        value_ = (value_ / other.value_) * multiplier_;

    return *this;
}

bool FixedPoint::operator==( FixedPoint& other)
{
    return (value_ == other.value_);
}

bool FixedPoint::operator!=( FixedPoint& other)
{
    return (value_ != other.value_);
}

bool FixedPoint::operator>( FixedPoint& other)
{
    return (value_ > other.value_);
}

bool FixedPoint::operator<( FixedPoint& other)
{
    return (value_ < other.value_);
}

bool FixedPoint::operator>=( FixedPoint& other)
{
    return (value_ > other.value_ || value_ == other.value_);
}

bool FixedPoint::operator<=( FixedPoint& other)
{
    return (value_ < other.value_ || value_ == other.value_);
}

int FixedPoint::toInt() {
	return static_cast<int>(value_ / multiplier_);
}

std::ostream& operator<<(std::ostream& os, const FixedPoint& number)
{
    std::string numberStr = std::to_string(number.value_);

    // Print digits before decimal point
    if (numberStr.length() > number.decPlaces_)
    {
        std::cout << numberStr.substr(0, numberStr.length() - number.decPlaces_);
    }
    else 
    {
        // Print "0" if number smaller 0
        std::cout << "0";
    }
    std::cout << ".";

    if (numberStr.length() >= number.decPlaces_)
    {
        std::cout << numberStr.substr(numberStr.length() - number.decPlaces_);
    }
    else
    {
        // Fill subdecimal digits with zero until first existing digit
        for (size_t c = numberStr.length(); c < number.decPlaces_; ++c)
        {
            std::cout << "0";
        }
        std::cout << numberStr;
    }

    return os;

}


