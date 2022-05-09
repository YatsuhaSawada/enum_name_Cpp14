#include <utility>
#include <type_traits>
#include <cstring>
#include <string>

#define ENUM_NAME_ELM_NUM_LIMIT_MAX 64 //扱えるENUMの要素数上限

namespace enum_name_tool {

//__PRETTY_FUNCTION__を返す関数
//この関数はEにENUM型、VにENUMの要素を受け取ることで以下のようにENUMの要素名を
//含む関数名を取得することができる。
//例
//constexpr const char* name_e_e() [with E = Fruit1; E V = Fruit1::BANANA2]
template <typename E, E V> constexpr const char* name_e_e() {
  return __PRETTY_FUNCTION__;
}

//__PRETTY_FUNCTION__にENUMの要素が含まれているかを確認する。
//https://taylorconor.com/blog/enum-reflection/
//のブログで公開されているコードを元にしている。そのままではC++14では動かないため一部修正している
//コメントはそのまま残している。
//
// __PRETTY_FUNCTION__の末尾から数えて初めのスペースの次の文字がアルファベット以外ならENUM名が入っていないと
// 判断している。
template <typename E, E V> constexpr bool isValid() {
  // When compiled with clang, `name` will contain a prettified function name,
  // including the enum value name for `V` if valid. For example:
  // "bool isValid() [E = Fruit, V = Fruit::BANANA]" for valid enum values, or:
  // "bool isValid() [E = Fruit, V = 10]" for invalid enum values.
  auto name = __PRETTY_FUNCTION__;
  int i = strlen(name);
  // Find the final space character in the pretty name.
  for (; i >= 0; --i) {
    if (name[i] == ' ') {
      break;
    }
  }
  // The character after the final space will indicate if
  // it's a valid value or not.
  char c = name[i + 1];
  if (c >= '0' && c <= '9') {
    return false;
  }

  if (!((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z'))) {
    return false;
  }

  return true;
}

//コンパイル時にinternalElementCountで渡された0~ENUM_NAME_ELM_NUM_LIMIT_MAXの
//数値がENUMの要素でなければ呼ばれる
template <typename E> constexpr int countValid() {
  return 0;
}

//コンパイル時にinternalElementCountで渡された0~ENUM_NAME_ELM_NUM_LIMIT_MAXの
//数値がENUMの要素であれば呼ばれる
template <typename E, E A, E... B> constexpr int countValid() {
  bool is_valid = isValid<E, A>();
  return countValid<E, B...>() + static_cast<int>(is_valid);
}

template <typename E, int... I> constexpr int internalElementCount(std::integer_sequence<int, I...> unused) {
  return countValid<E, (E)I...>();
}

template <typename E> struct elementCount {
  static const int value = internalElementCount<E>(std::make_integer_sequence<int, ENUM_NAME_ELM_NUM_LIMIT_MAX>());
};

//__PRETTY_FUNCTION__から取得した名称からEnumの要素数を抜き出す
//constexpr const char* name_e_e() [with E = Fruit1; E V = Fruit1::BANANA2]
std::string extract_enum_name(const char* v) {
  int end = strlen(v);
  int i = end;  
  for (; i >= 0; --i) {
    if (v[i] == ' ') {
      break;
    }
  }

  int start = i + 1;

  return std::string(&v[start],&v[end-1]);
} 
} // end_of_namespace

// ------------------------------------------------------------------
#include <map>

template <class T, std::size_t SIZE>
struct CONV {
    static constexpr std::size_t size = SIZE;
};

#define FUNC_NAME_OF_IDX_ELEM(NUM_IDX) static const char* name ## NUM_IDX () {\
        return enum_name_tool::name_e_e<T, static_cast<T>(NUM_IDX)>();\
    }

#define ROW_OF_CONV_TABLE(NUM_IDX) { static_cast<T>(NUM_IDX), std::string(enum_name_tool::extract_enum_name(name ## NUM_IDX())) }

// --------------------------
template <class T>
struct CONV<T, 2> {
    static constexpr std::size_t size = 2;
      
    FUNC_NAME_OF_IDX_ELEM(0)
    FUNC_NAME_OF_IDX_ELEM(1)

    static const char* name(T v){
        const static std::map<T, std::string> table = {
          ROW_OF_CONV_TABLE(0),
          ROW_OF_CONV_TABLE(1),
        };

        auto p = table.find(v);
        if (p == table.end()) {
          return "";
        }
        return p->second.c_str();
    }
};

// --------------------------
template <class T>
struct CONV<T, 3> {
    static constexpr std::size_t size = 3;
      
    FUNC_NAME_OF_IDX_ELEM(0)
    FUNC_NAME_OF_IDX_ELEM(1)
    FUNC_NAME_OF_IDX_ELEM(2)

    static const char* name(T v){
        const static std::map<T, std::string> table = {
          ROW_OF_CONV_TABLE(0),
          ROW_OF_CONV_TABLE(1),
          ROW_OF_CONV_TABLE(2),
        };

        auto p = table.find(v);
        if (p == table.end()) {
          return "";
        }
        return p->second.c_str();
    }
};

// --------------------------
template <typename E> constexpr const char* Conv2Name(E v) {
  return CONV<E, enum_name_tool::elementCount<E>::value>::name(v);
}

#define CONV2NAME(v) Conv2Name(v)