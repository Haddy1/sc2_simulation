#pragma once
#include <iostream>
#include <cmath>

class FixedPoint {
    private:
        long long value_ = 0;
        const size_t decPlaces_ = 4;                    // Nr of decimal places stored
        const int multiplier_ = pow(10,decPlaces_);     // Multiplier used to 'shift' decimal point

    public:
        FixedPoint();

        FixedPoint(long long intValue);

        FixedPoint(int intValue);

        FixedPoint(double fValue);

        FixedPoint(FixedPoint& other);

        FixedPoint(FixedPoint&& other);

        ~FixedPoint();

        FixedPoint operator=(FixedPoint& other);

        FixedPoint operator=(FixedPoint&& other);

        FixedPoint operator+(FixedPoint& other);

        FixedPoint operator+=(FixedPoint& other);

        FixedPoint operator-(FixedPoint& other);

        FixedPoint operator-=(FixedPoint& other);

        FixedPoint operator*(FixedPoint& other);

        FixedPoint operator*=(FixedPoint& other);

        FixedPoint operator/(FixedPoint& other);

        FixedPoint operator/=(FixedPoint& other);

        bool operator==(FixedPoint& other);

        bool operator!=(FixedPoint& other);

        bool operator>(FixedPoint& other);

        bool operator<(FixedPoint& other);

        bool operator<=(FixedPoint& other);

        bool operator>=(FixedPoint& other);
        
        int toInt();

        friend std::ostream& operator<<(std::ostream& os, const FixedPoint& number);
        
		
};
