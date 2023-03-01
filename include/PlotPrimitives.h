/*! Definition of the plot primitives. */

#pragma once

#include <TAttText.h>
#include <TH1.h>
#include <TColor.h>
#include <boost/regex.hpp>

#include "EnumNameMap.h"
#include "NumericPrimitives.h"

namespace root_ext {

using ::analysis::operator<<;
using ::analysis::operator>>;

template<typename T, size_t n_dim, bool positively_defined = false>
struct Point;

template<typename T, bool positively_defined>
struct Point<T, 1, positively_defined> {
    Point() : _x(0) {}
    Point(const T& x)
        : _x(x)
    {
        if(!IsValid(x))
            throw analysis::exception("Invalid point %1%.") % x;
    }

    bool operator ==(const Point<T, 1, positively_defined>& other) const
    {
        return x() == other.x();
    }
    bool operator !=(const Point<T, 1, positively_defined>& other) const { return !(*this == other); }

    const T& x() const { return _x; }
    operator T() const { return _x; }

    template<bool pos_def>
    Point<T, 1, positively_defined && pos_def> operator+(const Point<T, 1, pos_def>& other) const
    {
        return Point<T, 1, positively_defined && pos_def>(x() + other.x());
    }

    template<bool pos_def>
    Point<T, 1, positively_defined && pos_def> operator-(const Point<T, 1, pos_def>& other) const
    {
        return Point<T, 1, positively_defined && pos_def>(x() - other.x());
    }

    template<bool pos_def>
    Point<T, 1, positively_defined && pos_def> operator*(const Point<T, 1, pos_def>& other) const
    {
        return Point<T, 1, positively_defined && pos_def>(x() * other.x());
    }

    static bool IsValid(const T& x) { return !positively_defined || x >= 0; }

    std::string ToString() const { return analysis::ToString(x()); }

    static Point<T, 1, positively_defined> Parse(const std::string& str)
    {
        const auto x = analysis::Parse<T>(str);
        return Point<T, 1, positively_defined>(x);
    }

private:
    T _x;
};

template<typename T, bool positively_defined>
struct Point<T, 2, positively_defined> {
    static const std::string& separator() { static const std::string sep = ","; return sep; }

    Point() : _x(0), _y(0) {}
    Point(const T& x, const T& y)
        : _x(x), _y(y)
    {
        if(!IsValid(x, y))
            throw analysis::exception("Invalid point (%1%, %2%).") % x % y;
    }

    bool operator ==(const Point<T, 2, positively_defined>& other) const
    {
        return x() == other.x() && y() == other.y();
    }
    bool operator !=(const Point<T, 2, positively_defined>& other) const { return !(*this == other); }

    const T& x() const { return _x; }
    const T& y() const { return _y; }

    template<bool pos_def>
    Point<T, 2, positively_defined && pos_def> operator+(const Point<T, 2, pos_def>& other) const
    {
        return Point<T, 2, positively_defined && pos_def>(x() + other.x(), y() + other.y());
    }

    template<bool pos_def>
    Point<T, 2, positively_defined && pos_def> operator-(const Point<T, 2, pos_def>& other) const
    {
        return Point<T, 2, positively_defined && pos_def>(x() - other.x(), y() - other.y());
    }

    template<bool pos_def>
    Point<T, 2, positively_defined && pos_def> operator*(const Point<T, 2, pos_def>& other) const
    {
        return Point<T, 2, positively_defined && pos_def>(x() * other.x(), y() * other.y());
    }

    Point<T, 2, false> flip_x() const { return Point<T, 2, false>(-x(), y()); }
    Point<T, 2, false> flip_y() const { return Point<T, 2, false>(x(), -y()); }

    static bool IsValid(const T& x, const T& y) { return !positively_defined || (x >= 0 && y >= 0); }

    std::string ToString() const { return analysis::ToString(x()) + separator() + analysis::ToString(y()); }

    static Point<T, 2, positively_defined> Parse(const std::string& str)
    {
        const auto coord_list = analysis::SplitValueList(str, true, separator(), false);
        if(coord_list.size() != 2) throw analysis::exception("Invalid 2D point '%1%'.") % str;
        const auto x = analysis::Parse<T>(coord_list.at(0));
        const auto y = analysis::Parse<T>(coord_list.at(1));
        return Point<T, 2, positively_defined>(x, y);
    }

private:
    T _x, _y;
};

template<>
struct Point<bool, 2, false> {
    static const std::string& separator() { static const std::string sep = ","; return sep; }

    Point() : _x(false), _y(false) {}
    Point(bool x, bool y) : _x(x), _y(y) {}

    bool operator ==(const Point<bool, 2, false>& other) const
    {
        return x() == other.x() && y() == other.y();
    }
    bool operator !=(const Point<bool, 2, false>& other) const { return !(*this == other); }

    const bool& x() const { return _x; }
    const bool& y() const { return _y; }

    Point<bool, 2, false> flip_x() const { return Point<bool, 2, false>(!x(), y()); }
    Point<bool, 2, false> flip_y() const { return Point<bool, 2, false>(x(), !y()); }

    std::string ToString() const { return analysis::ToString(x()) + separator() + analysis::ToString(y()); }

    static Point<bool, 2, false> Parse(const std::string& str)
    {
        const auto coord_list = analysis::SplitValueList(str, true, separator(), false);
        if(coord_list.size() != 2) throw analysis::exception("Invalid 2D point '%1%'.") % str;
        const auto x = analysis::Parse<bool>(coord_list.at(0));
        const auto y = analysis::Parse<bool>(coord_list.at(1));
        return Point<bool, 2, false>(x, y);
    }

private:
    bool _x, _y;
};

template<typename T, size_t n_dim>
using Size = Point<T, n_dim, true>;

template<typename T, size_t n_dim, bool positively_defined>
std::ostream& operator<<(std::ostream& s, const Point<T, n_dim, positively_defined>& p)
{
    s << p.ToString();
    return s;
}

template<typename T, size_t n_dim, bool positively_defined>
std::istream& operator>>(std::istream& s, Point<T, n_dim, positively_defined>& p)
{
    std::string str;
    s >> str;
    p = Point<T, n_dim, positively_defined>::Parse(str);
    return s;
}

template<typename T>
struct Box {
    using Position = Point<T, 2>;
    static const std::string& separator() { return Position::separator(); }

    Box() {}

    Box(const Position& left_bottom, const Position& right_top) : _left_bottom(left_bottom), _right_top(right_top)
    {
        CheckValidity();
    }

    Box(const T& left_bottom_x, const T& left_bottom_y, const T& right_top_x, const T& right_top_y)
        : _left_bottom(left_bottom_x, left_bottom_y), _right_top(right_top_x, right_top_y)
    {
        CheckValidity();
    }

    bool operator ==(const Box<T>& other) const
    {
        return left_bottom() == other.left_bottom() && right_top() == other.right_top();
    }
    bool operator !=(const Box<T>& other) const { return !(*this == other); }

    const Position& left_bottom() const { return _left_bottom; }
    const Position& right_top() const { return _right_top; }
    const T& left_bottom_x() const { return left_bottom().x(); }
    const T& left_bottom_y() const { return left_bottom().y(); }
    const T& right_top_x() const { return right_top().x(); }
    const T& right_top_y() const { return right_top().y(); }

    static bool IsValid(const Position& left_bottom, const Position& right_top)
    {
        return IsValid(left_bottom.x(), left_bottom.y(), right_top.x(), right_top.y());
    }

    static bool IsValid(const T& left_bottom_x, const T& left_bottom_y, const T& right_top_x, const T& right_top_y)
    {
        return analysis::Range<T>::IsValid(left_bottom_x, right_top_x)
                && analysis::Range<T>::IsValid(left_bottom_y, right_top_y);
    }

private:
    void CheckValidity() const
    {
        if(!IsValid(_left_bottom, _right_top))
            throw analysis::exception("Invalid box [%1% %2%]") % _left_bottom % _right_top;
    }

private:
    Position _left_bottom, _right_top;
};

template<typename T>
std::ostream& operator<<(std::ostream& s, const Box<T>& b)
{
    s << b.left_bottom() << Box<T>::separator() << b.right_top();
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, Box<T>& b)
{
    std::string str;
    s >> str;
    const auto v_list = analysis::SplitValueList(str, true, Box<T>::separator(), false);
    if(v_list.size() != 4)
        throw analysis::exception("Invalid box.");
    const T left_bottom_x = analysis::Parse<T>(v_list.at(0));
    const T left_bottom_y = analysis::Parse<T>(v_list.at(1));
    const T right_top_x = analysis::Parse<T>(v_list.at(2));
    const T right_top_y = analysis::Parse<T>(v_list.at(3));
    b = Box<T>(left_bottom_x, left_bottom_y, right_top_x, right_top_y);
    return s;
}

template<typename T>
struct MarginBox {
    static const std::string& separator() { return Box<T>::separator(); }
    MarginBox() : _left(0), _bottom(0), _right(0), _top(0) {}

    MarginBox(const T& left, const T& bottom, const T& right, const T& top)
        : _left(left), _bottom(bottom), _right(right), _top(top)
    {
        CheckValidity();
    }

    const T& left() const { return _left; }
    const T& bottom() const { return _bottom; }
    const T& right() const { return _right; }
    const T& top() const { return _top; }

    static bool IsValid(const T& left, const T& bottom, const T& right, const T& top)
    {
        return IsValidMarginValue(left) && IsValidMarginValue(bottom)
                && IsValidMarginValue(right) && IsValidMarginValue(top);
    }

    static bool IsValidMarginValue(const T& v) { return v >= 0 && v <= 1; }

private:
    void CheckValidity() const
    {
        if(!IsValid(_left, _bottom, _right, _top))
            throw analysis::exception("Invalid margin box: (left, bottom, right, top) = (%1%, %2%, %3%, %4%).")
                % _left % _bottom % _right % _top;
    }

private:
    T _left, _bottom, _right, _top;
};

template<typename T>
std::ostream& operator<<(std::ostream& s, const MarginBox<T>& b)
{
    const auto& sep = MarginBox<T>::separator();
    s <<  b.left() << sep << b.bottom() << sep << b.right() << sep << b.top();
    return s;
}

template<typename T>
std::istream& operator>>(std::istream& s, MarginBox<T>& b)
{
    std::string str;
    s >> str;
    const auto v_list = analysis::SplitValueList(str, true, MarginBox<T>::separator(), false);
    if(v_list.size() != 4)
        throw analysis::exception("Invalid margin box.");
    const T left = analysis::Parse<T>(v_list.at(0));
    const T bottom = analysis::Parse<T>(v_list.at(1));
    const T right = analysis::Parse<T>(v_list.at(2));
    const T top = analysis::Parse<T>(v_list.at(3));
    b = MarginBox<T>(left, bottom, right, top);
    return s;
}

class ReferenceColorCollection {
public:
    using ColorNameMap = analysis::EnumNameMap<EColor>;
    using ColorRelativeRangeMap = std::unordered_map<EColor, analysis::RelativeRange<int>, ColorNameMap::EnumHash>;
    using ColorRangeMap = std::unordered_map<EColor, analysis::Range<int>, ColorNameMap::EnumHash>;

    static const ColorNameMap& GetReferenceColorNames()
    {
        static const ColorNameMap names = ColorNameMap("EColor", {
            { kWhite, "kWhite" }, { kBlack, "kBlack" }, { kGray, "kGray" }, { kRed, "kRed" }, { kGreen, "kGreen" },
            { kBlue, "kBlue" }, { kYellow, "kYellow" }, { kMagenta, "kMagenta" }, { kCyan, "kCyan" },
            { kOrange, "kOrange" }, { kSpring, "kSpring" }, { kTeal, "kTeal" }, { kAzure, "kAzure" },
            { kViolet, "kViolet" }, { kPink, "kPink" }
        });
        return names;
    }

    static const ColorRelativeRangeMap& GetReferenceColorRelativeRanges()
    {
        static const ColorRelativeRangeMap ranges = {
            { kWhite, { 0, 0 } }, { kBlack, { 0, 0 } }, { kGray, { 0, 3 } }, { kRed, { -10, 4 } },
            { kGreen, { -10, 4 } }, { kBlue, { -10, 4 } }, { kYellow, { -10, 4 } }, { kMagenta, { -10, 4 } },
            { kCyan, { -10, 4 } }, { kOrange, { -9, 10 } }, { kSpring, { -9, 10 } }, { kTeal, { -9, 10 } },
            { kAzure, { -9, 10 } }, { kViolet, { -9, 10 } }, { kPink, { -9, 10 } }
        };
        return ranges;
    }

    static const ColorRangeMap& GetReferenceColorRanges()
    {
        static const auto init_ranges = []() {
            ColorRangeMap ranges;
            for(const auto& rel_range : GetReferenceColorRelativeRanges())
                ranges[rel_range.first] = rel_range.second.ToAbsoluteRange(static_cast<int>(rel_range.first));
            return ranges;
        };
        static const ColorRangeMap ranges = init_ranges();
        return ranges;
    }

    static bool FindReferenceColor(int color_id, EColor& e_color, int& shift)
    {
        for(const auto& range : GetReferenceColorRanges()) {
            if(range.second.Contains(color_id)) {
                e_color = range.first;
                shift = color_id - static_cast<int>(range.first);
                return true;
            }
        }
        return false;
    }

    static bool IsReferenceColor(int color_id)
    {
        EColor e_color;
        int shift;
        return FindReferenceColor(color_id, e_color, shift);
    }

    static int ToColorId(EColor e_color, int shift)
    {
        return static_cast<int>(e_color) + shift;
    }

    static std::string ToString(EColor e_color, int shift)
    {
        std::ostringstream ss;
        ss << GetReferenceColorNames().EnumToString(e_color);
        if(shift > 0)
            ss << "+" << shift;
        else if(shift < 0)
            ss << shift;
        return ss.str();
    }

    static std::string ToString(int color_id)
    {
        EColor e_color;
        int shift;
        if(!FindReferenceColor(color_id, e_color, shift))
            throw analysis::exception("Unable to find a reference color for the color with id = %1%.") % color_id;
        return ToString(e_color, shift);
    }

    static bool TryParse(const std::string& str, EColor& e_color, int& shift)
    {
        if(GetReferenceColorNames().TryParse(str, e_color)) {
            shift = 0;
            return true;
        }
        std::vector<std::string> sub_str;
        boost::split(sub_str, str, boost::is_any_of("+-"), boost::token_compress_on);
        if(sub_str.size() != 2 || !GetReferenceColorNames().TryParse(sub_str.at(0), e_color))
            return false;
        std::istringstream ss(sub_str.at(1));
        ss >> shift;
        int sign = std::find(str.begin(), str.end(), '-') == str.end() ? 1 : -1;
        shift *= sign;
        return !ss.fail();
    }
};

class Color {
public:
    Color() { RetrieveTColor(kBlack); }
    explicit Color(int color_id) { RetrieveTColor(color_id); }
    explicit Color(const std::string& hex_color)
    {
        static const boost::regex hex_color_pattern("^#(?:[0-9a-fA-F]{2}){3}$");
        if(!boost::regex_match(hex_color, hex_color_pattern))
            throw analysis::exception("Invalid color '%1%'.") % hex_color;
        const int color_id = TColor::GetColor(hex_color.c_str());
        RetrieveTColor(color_id);
    }

    bool IsSimple() const { return ReferenceColorCollection::IsReferenceColor(GetColorId()); }
    int GetColorId() const { return GetTColor().GetNumber(); }
    Color_t GetColor_t() const { return static_cast<Color_t>(GetColorId()); }
    const TColor& GetTColor() const { return *t_color; }

    std::string ToString() const
    {
        if(IsSimple())
            return ReferenceColorCollection::ToString(GetColorId());
        return GetTColor().AsHexString();
    }
    Color CreateTransparentCopy(float alpha) const
    {
        return Color(TColor::GetColorTransparent(GetColorId(), alpha));
    }

    static Color Parse(const std::string& name)
    {
        EColor e_color;
        int shift;
        if(ReferenceColorCollection::TryParse(name, e_color, shift))
            return Color(ReferenceColorCollection::ToColorId(e_color, shift));
        return Color(name);
    }

private:
    static void CheckComponent(const std::string& name, int value)
    {
        static const analysis::Range<int> color_range(0, 255);
        if(!color_range.Contains(value))
            throw analysis::exception("Invalid color %1% component value = %2%") % name % value;
    }

    void RetrieveTColor(int color_id)
    {
        t_color = gROOT->GetColor(color_id);
        if(!t_color)
            throw analysis::exception("Color with id %1% is not defined.") % color_id;
    }

private:
    const TColor* t_color;
};

std::ostream& operator<<(std::ostream& s, const Color& c)
{
    s << c.ToString();
    return s;
}
std::istream& operator>>(std::istream& s, Color& c)
{
    std::string name;
    s >> name;
    EColor e_color;
    int shift;
    if(ReferenceColorCollection::TryParse(name, e_color, shift)) {
        c = Color(ReferenceColorCollection::ToColorId(e_color, shift));
    } else
        c = Color(name);
    return s;
}

class Font {
public:
    using Integer = short;

    Font() : _number(4), _precision(2) {}
    explicit Font(Integer font_code)
    {
        ParseFontCode(font_code, _number, _precision);
        CheckValidity();
    }
    Font(Integer font_number, Integer precision)
        : _number(font_number), _precision(precision)
    {
        CheckValidity();
    }

    Integer code() const { return MakeFontCode(_number, _precision); }
    Integer number() const { return _number; }
    Integer precision() const { return _precision; }

    static Integer MakeFontCode(Integer font_number, Integer precision) { return font_number * 10 + precision; }
    static void ParseFontCode(Integer font_code, Integer& font_number, Integer& precision)
    {
        font_number = font_code / 10;
        precision = font_code % 10;
    }
    static bool IsValid(Integer font_number, Integer precision)
    {
        static const analysis::Range<Integer> font_number_range(1, 15);
        static const analysis::Range<Integer> precision_range(0, 3);
        return font_number_range.Contains(font_number) && precision_range.Contains(precision);
    }
    static bool IsValid(Integer font_code)
    {
        Integer font_number, precision;
        ParseFontCode(font_code, font_number, precision);
        return IsValid(font_number, precision);
    }

private:
    void CheckValidity() const
    {
        if(!IsValid(number(), precision()))
            throw analysis::exception("Invalid font code = %1%.") % code();
    }

private:
    Integer _number, _precision;
};

std::ostream& operator<<(std::ostream& s, const Font& f)
{
    s << f.code();
    return s;
}
std::istream& operator>>(std::istream& s, Font& f)
{
    Font::Integer font_code;
    s >> font_code;
    f = Font(font_code);
    return s;
}

enum class TextAlign { LeftBottom = kHAlignLeft + kVAlignBottom, LeftCenter = kHAlignLeft + kVAlignCenter,
                       LeftTop = kHAlignLeft + kVAlignTop, CenterBottom = kHAlignCenter + kVAlignBottom,
                       Center = kHAlignCenter + kVAlignCenter, CenterTop = kHAlignCenter + kVAlignTop,
                       RightBottom = kHAlignRight + kVAlignBottom, RightCenter = kHAlignRight + kVAlignCenter,
                       RightTop = kHAlignRight + kVAlignTop };
ENUM_NAMES(TextAlign) = {
    { TextAlign::LeftBottom, "left_bottom" },
    { TextAlign::LeftCenter, "left_center" },
    { TextAlign::LeftTop, "left_top" },
    { TextAlign::CenterBottom, "center_bottom" },
    { TextAlign::Center, "center" },
    { TextAlign::CenterTop, "center_top" },
    { TextAlign::RightBottom, "right_bottom" },
    { TextAlign::RightCenter, "right_center" },
    { TextAlign::RightTop, "right_top" }
};

} // namespace root_ext