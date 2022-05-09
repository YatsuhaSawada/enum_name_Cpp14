// https://github.com/Neargye/nameof
// 本当は上記を使いたいがC++17でないといけない
// このためC++14でenum変数から名称を出せる機能だけを
// 自身で定義する
// 後で置き換え可能なようにIFの名称をそろえておく
#include "enum_name_tool.hpp"

#define NAMEOF_ENUM(v) Conv2Name(v)