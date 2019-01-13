#pragma once
#include <iostream>
#include <cmath>

class FixedPoint {
    private:
        long value_ = 0;
        static const unsigned short decPlaces_ = 5;                    // Nr of decimal places stored
        static const int multiplier_ = 100000;//pow(10,decPlaces_);     // Multiplier used to 'shift' decimal point

    public:
        FixedPoint();

        FixedPoint(long intValue);

        FixedPoint(int intValue);

        FixedPoint(double fValue);
		
		FixedPoint(float fValue);

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
