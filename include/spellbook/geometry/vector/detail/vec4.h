#ifndef au_detail_vec4_h
#define au_detail_vec4_h

namespace au {

template <typename T>
std::ostream& operator<<(std::ostream& o, const vec4<T>& v)
{
    o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return o;
}

template <typename U, typename V>
vec4<decltype(U()*V())> operator*(const matrix4<U>& m, const vec4<V>& v)
{
    typedef decltype(U()*V()) ResultType;
    vec4<ResultType> ret;
    ret.x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w;
    ret.y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w;
    ret.z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w;
    ret.w = m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w;
    return ret;
}

} // namespace au

#endif
