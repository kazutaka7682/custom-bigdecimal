#ifndef BigDecimal_h
#define BigDecimal_h

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

// 倍浮動小数を拡張した丸め誤差のない数字を扱う
class BigDecimal {
protected:
    // 仮数
    vector<int8_t> mantissa;
    // 符号
    int code = 1;
    // 指数
    int index = 0;
protected:
    // 0の端からの数を取得する
    int checkZero(bool isFront) const;
    // 端の0を除去する
    void removeZero(bool onlyForward);
    // 四捨五入する
    void roundOff();
    // 累乗を計算する
    int caluculatePower(int radix, int powerIndex) const;
    // 正の整数を文字列に変換する
    string convertIntegerToString(unsigned int number) const;
    // 正の整数をベクタに変換する
    vector<int8_t> convertToVector(unsigned int number) const;
    // 正の整数の桁数を数える
    int countDigits(unsigned int number) const;
    // パース処理を行う
    void parse(const string& str);
    // 仮数の引き算の計算結果を取得する
    // @param small 引き算する値
    vector<int8_t> getSubtractionStack(const BigDecimal& outSmall, int length);
    // 仮数の足し算の計算結果を取得する
    void getAdditionStack(const BigDecimal& outObj, int length);
    // 絶対値的に大きい値の判定値を取得する
    bool getJudgmentValue(const BigDecimal& sub) const;
    // 仮数の桁数を揃える
    void alignMantissa(BigDecimal* outSub);
    // 足し算または引き算を行う
    BigDecimal doAdditionOrSubtraction(const BigDecimal& obj, bool plusMinus) const;
public:
    // デフォルトコンストラクタ
    BigDecimal();
    // コンストラクタ
    BigDecimal(const string& str);
    // BigDecimal内部表現用乱数を生成しセットする
    void setRandom(int digits);
    // 内部表現を文字列化する
    string toString() const;
    // 実数を文字列に変換する
    string convertDoubleToString(const double number) const;
    // 文字列を実数に変換する
    double convertToDouble();
    // 文字列を実数に変換する
    double convertToDouble(const string& str);
    // 仮数を取得する
    vector<int8_t> getMantissa() const { return mantissa; };
    // 符合を取得する
    int getCode() const { return code; };
    // 指数を取得する
    int getIndex() const { return index; };
    // +演算子オーバーロード
    BigDecimal operator+(const BigDecimal& obj) const;
    // +=演算子オーバーロード
    BigDecimal& operator+=(const BigDecimal& obj);
    // -演算子オーバーロード
    BigDecimal operator-(const BigDecimal& obj) const;
    // -=演算子オーバーロード
    BigDecimal& operator-=(const BigDecimal& obj);
    // *演算子オーバーロード
    BigDecimal operator*(const BigDecimal& obj) const;
    // *=演算子オーバーロード
    BigDecimal& operator*=(const BigDecimal& obj);
    // /演算子オーバーロード
    BigDecimal operator/(const BigDecimal& obj) const;
    // /=演算子オーバーロード
    BigDecimal& operator/=(const BigDecimal& obj);
};

#endif
