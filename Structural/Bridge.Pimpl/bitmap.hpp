#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Bitmap
{
public:
    struct BitmapImpl;
    std::unique_ptr<BitmapImpl> pimpl_;
    Bitmap(size_t size, char fill_char = '*');
    ~Bitmap();

    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;

    Bitmap(Bitmap&&) = delete;
    Bitmap& operator=(Bitmap&&) = delete;

    void draw();
};

// inline void destroy_impl(Bitmap::BitmapImpl* ptr)
// {
//     delete ptr;
// }

#endif // BITMAP_HPP
