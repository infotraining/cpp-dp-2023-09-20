#include "bitmap.hpp"
#include <algorithm>
#include <array>

using namespace std;

struct Bitmap::BitmapImpl
{
    std::vector<int> image_;
};

Bitmap::Bitmap(size_t size, char fill_char) : pimpl_{std::make_unique<BitmapImpl>()}
{
    pimpl_->image_.resize(size);
    fill_n(pimpl_->image_.begin(), size, fill_char);
}

Bitmap::~Bitmap() = default;

void Bitmap::draw()
{
    cout << "Image: ";
    for (size_t i = 0; i < pimpl_->image_.size(); ++i)
        cout << pimpl_->image_[i];
    cout << endl;
}
