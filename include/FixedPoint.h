#pragma once
#include <iostream>
#include <cmath>

class FixedPoint {
    private:
        long long value_ = 0;
        const size_t decPlaces_ = 5;                    // Nr of decimal places stored
        const int multiplier_ = pow(10,decPlaces_);     // Multiplier used to 'shift' decimal point

    public:
        FixedPoint();

        FixedPoint(long long intValue);

        FixedPoint(int intValue);

        FixedPoint(double fValue);

        FixedPoint(const FixedPoint& other);

        FixedPoint(FixedPoint&& other);

        ~FixedPoint();

        FixedPoint& operator=(const FixedPoint& other);

        FixedPoint& operator=(FixedPoint&& other);

        FixedPoint operator+(const FixedPoint& other);

        FixedPoint& operator+=(const FixedPoint& other);

        FixedPoint operator-(const FixedPoint& other);

        FixedPoint& operator-=(const FixedPoint& other);

        FixedPoint operator*(const FixedPoint& other);

        FixedPoint& operator*=(const FixedPoint& other);

        FixedPoint operator/(const FixedPoint& other);

        FixedPoint& operator/=(const FixedPoint& other);

        bool operator==(const FixedPoint& other);

        bool operator!=(const FixedPoint& other);

        bool operator>(const FixedPoint& other);

        bool operator<(const FixedPoint& other);

        bool operator<=(const FixedPoint& other);

        bool operator>=(const FixedPoint& other);
        
        int toInt() const ;

        friend std::ostream& operator<<(std::ostream& os, const FixedPoint& number);
		
		FixedPoint operator*(const int other);
        
		
};
