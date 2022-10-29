#include "BigDecimal.h"

// 演算時における最大桁数上限値
constexpr int LIMIT_OF_DIGITS = 1000;

// デフォルトコンストラクタ
BigDecimal::BigDecimal() {}

// コンストラクタ
BigDecimal::BigDecimal(const string& str) {
    parse(str);
}

// 0の端からの数を取得する
int BigDecimal::checkZero(bool isFront) const {
    for (int i = 0; i < (int)mantissa.size(); i++) {
        int idx = (isFront) ? i : (int)mantissa.size() - i - 1;
        if ((int)mantissa[idx] != 0) { // 数字が0でなくなったとき
            return i;
        }
    }
    return (int)mantissa.size();
}

// 端の0を除去する
void BigDecimal::removeZero(bool onlyForward) {
    mantissa.erase(mantissa.begin(), mantissa.begin() + checkZero(true));
    if (mantissa.empty()) { // 仮数が空であるとき
        mantissa.push_back((int8_t)0);
        code = 1;
    }
    if (onlyForward) { // 前方のみと指示があるとき
        return;
    }
    index += checkZero(false);
    mantissa.erase(mantissa.end() - checkZero(false), mantissa.end());
    if (mantissa.empty()) { // 仮数が空であるとき
        mantissa.push_back((int8_t)0);
    }
}

// 四捨五入する
void BigDecimal::roundOff() {
    if ((int)mantissa.size() < LIMIT_OF_DIGITS) { // 仮数が桁数上限値を以下のとき
        return;
    }
    int check = (int)mantissa[LIMIT_OF_DIGITS - 1];
    int memoryOfSize = (int)mantissa.size();
    mantissa.erase(mantissa.begin() + LIMIT_OF_DIGITS - 1, mantissa.end());
    if (check > 4) { // 仮数の1000桁目の数字が4より大きいとき
        for (int i = (int)mantissa.size() - 1; i >= 0; i--) {
            if ((int)mantissa[i] == 9) { // 繰り上がった先の数字が9のとき
                mantissa[i] = (int8_t)0;
            } else {
                mantissa[i] += (int8_t)1;
                break;
            }
        }
    }
    index += abs(memoryOfSize - (int)mantissa.size());
}

// 累乗を計算する
int BigDecimal::caluculatePower(int radix, int powerIndex) const {
    int result = 1;
    for (int i = 0; i < powerIndex; i++) {
        result *= radix;
    }
    return result;
}

// 正の整数を文字列に変換する
string BigDecimal::convertIntegerToString(unsigned int number) const {
    string str;
    int update = number;
    for (int i = 0; i < countDigits(number); i++) {
        int numberJuggling = caluculatePower(10, (countDigits(number) - i - 1));
        str += (char)(update / numberJuggling) + '0';
        update -= (int)(update / numberJuggling) * numberJuggling;
    }
    return str;
}

// 文字列を実数に変換する
double BigDecimal::convertToDouble(const string& str) {
    double result = 0.0;
    int decimal = 1;
    int code = 1;
    for (int i = 0; i < (int)str.size(); i++) {
        if (str[i] == '-') { // 文字列の最初に-符号がついているとき
            code = -1;
            continue;
        }
        if (str[i] == '.') {
            decimal += i;
            continue;
        }
        result = (str[i] - '0') + result*10;
    }
    if (decimal == 1) {
        decimal = (int)str.size();
    }
    result /= caluculatePower(10, (int)str.size() - decimal);
    result *= code;
    return result;
}

// 文字列を実数に変換する
double BigDecimal::convertToDouble() {
    string str = toString();
    return convertToDouble(str);
}

// 正の整数をベクタに変換する
vector<int8_t> BigDecimal::convertToVector(unsigned int number) const {
    vector<int8_t> vector;
    int update = number;
    for (int i = 0; i < countDigits(number); i++) {
        int numberJuggling = caluculatePower(10, (countDigits(number) - i - 1));
        vector.push_back((int8_t)(update / numberJuggling));
        update -= (int)(update / numberJuggling) * numberJuggling;
    }
    return vector;
}

// 正の整数の桁数を数える
int BigDecimal::countDigits(unsigned int number) const {
    int count = 1;
    while (number /= 10) {
        count++;
    }
    return count;
}

// パース処理を行う
void BigDecimal::parse(const string& str) {
    int periodIdx = -1;
    code = 1;
    if (str[0] == '-') { // 最初の文字がマイナスのとき
        code = -1;
    } else if (str[0] == '.') { // 最初の文字がピリオドのとき
        periodIdx = 0;
    } else if (str[0] < '0' || str[0] > '9') { // 最初の文字が数字でないとき
        throw runtime_error("Syntax Error.\n");
    }
    for (int i = 1; i < str.length(); i++) {
        if (str[i] == '.') { // 文字がピリオドのとき
            if (periodIdx >= 0) { // ピリオドがこれ以前に存在していたとき
                throw runtime_error("Syntax Error.\n");
            }
            periodIdx = i;
        } else if ((str[i] < '0') || (str[i] > '9')) { // 文字がピリオドまたは数字でないとき
            throw runtime_error("Syntax Error.\n");
        }
    }
    string str1;
    string str2;
    if (periodIdx == -1) { // 文字列中にピリオドが無いとき
        str1 = str.substr((int)((1.f - code) / 2.f), str.length());
    } else {
        str1 = str.substr((int)((1.f - code) / 2.f), periodIdx
                          - (int)((1.f - code) / 2.f));
        str2 = str.substr(periodIdx + 1, str.length());
        index = -(int)str2.length();
    }
    int conv = 0;
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] >= '0' && str1[i] <= '9') { // str1[j]が0-9に該当するとき
            conv = str1[i] - '0';
        }
        mantissa.push_back((int8_t)conv);
    }
    for (int i = 0; i < str2.length(); i++) {
        if (str2[i] >= '0' && str2[i] <= '9') { // str2[k]が0-9に該当するとき
            conv = str2[i] - '0';
        }
        mantissa.push_back((int8_t)conv);
    }
    removeZero(false);
}

// 乱数を生成しセットする
void BigDecimal::setRandom(int digits) {
    mantissa.clear();
    random_device random;
    mt19937 engine(random());
    uniform_int_distribution<> dist1(0, 9);
    uniform_int_distribution<> dist2(1, 9);
    uniform_int_distribution<> dist3(0, 1);
    uniform_int_distribution<> dist4(-4, 0);
    mantissa.push_back(dist2(engine));
    for (int i = 1; i < digits - 1; i++) {
        mantissa.push_back(dist1(engine));
    }
    mantissa.push_back(dist2(engine));
    code = 2 * dist3(engine) - 1;
    index = dist4(engine);
}

// 内部表現を文字列化する
string BigDecimal::toString() const {
    string str;
    if (code == -1) { // 符号が負のとき
        str = "-";
    }
    int additionOfDecimalPoint = 0;
    if (index < 0) { // 指数が負であるとき
        additionOfDecimalPoint = -index;
    }
    if (additionOfDecimalPoint >= (int)mantissa.size()
        && additionOfDecimalPoint != 0) { // 仮数の大きさより負の指数の絶対値が大きいとき
        str += "0.";
        for (int i = 0; i < additionOfDecimalPoint - (int)mantissa.size(); i++) {
            str += "0";
        }
        for (int i = 0; i < (int)mantissa.size(); i++) {
            str += (int)mantissa[i] + '0';
        }
    } else if (additionOfDecimalPoint < (int)mantissa.size()
               && additionOfDecimalPoint != 0) { // 仮数の大きさより負の指数の絶対値が大きいとき
        for (int i = 0; i < (int)mantissa.size() - additionOfDecimalPoint; i++) {
            str += (int)mantissa[i] + '0';
        }
        str += ".";
        for (int i = (int)mantissa.size() - additionOfDecimalPoint;
             i < (int)mantissa.size(); i++) {
            str += (int)mantissa[i] + '0';
        }
    } else if (additionOfDecimalPoint == 0) { // 指数が負でないとき
        for (int i = 0; i < (int)mantissa.size(); i++) {
            str += (int)mantissa[i] + '0';
        }
        for (int i = 0; i < abs(index); i++) {
            str += "0";
        }
    }
    return str;
}

// 仮数の引き算の計算結果を取得する
vector<int8_t>  BigDecimal::getSubtractionStack(const BigDecimal& outSmall,
                                                int length) {
    int stackNumber = 0;
    for (int i = length - 1; i >= 0; i--) {
        if (mantissa[i] < outSmall.mantissa[i]) { // 引く側の数字の方が大きいとき
            for (int j = i - 1; j >= 0; j--) {
                if (mantissa[j] != (int8_t)0) { // iより大きい桁の数字が0でないとき
                    mantissa[j] -= (int8_t)1;
                    for (int k = j + 1; k <= i - 1; k++) {
                        mantissa[k] = (int8_t)9;
                    }
                    stackNumber = (int)((int8_t)10
                                        + mantissa[i] - outSmall.mantissa[i]);
                    break;
                }
            }
        } else {
            stackNumber = (int)(mantissa[i] - outSmall.mantissa[i]);
        }
        mantissa[i] = (int8_t)stackNumber;
    }
    return mantissa;
}

// 仮数の足し算の計算結果を取得する
void BigDecimal::getAdditionStack(const BigDecimal& outObj, int length) {
    int carriedNumber = 0;
    for (int i = length - 1; i >= 0; i--) {
        int stackNumber = (int)mantissa[i]
                                    + (int)outObj.mantissa[i] + carriedNumber;
        mantissa[i] = (int8_t)(stackNumber % 10);
        carriedNumber = stackNumber / 10;
    }
    mantissa.insert(mantissa.begin(), (int8_t)carriedNumber);
}

// 絶対値的に大きい値か否かの判定値を取得する
bool BigDecimal::getJudgmentValue(const BigDecimal& sub) const {
    BigDecimal result(*this);
    BigDecimal obj(sub);
    bool isResultBigger = true;
    int set = 0;
    set = abs(index - sub.index);
    for (int i = 0; i < set; i++) {
        if (index >= sub.index) { // 1つ目の数字の指数の方が大きいとき
            result.mantissa.push_back((int8_t)0);
            result.index = obj.index;
        } else {
            obj.mantissa.push_back((int8_t)0);
            obj.index = result.index;
        }
    }
    if (result.mantissa.size() == obj.mantissa.size()) {
        for (int i = 0; i < (int)result.mantissa.size(); i++) {
            if (mantissa[i] < sub.mantissa[i]) { // subの仮数のi番目の方が大きいとき
                isResultBigger = false;
                break;
            } else if (mantissa[i] > sub.mantissa[i]) { // subの仮数のi番目の方が小さいとき
                break;
            }
        }
    } else if (result.mantissa.size() < obj.mantissa.size()) {
        isResultBigger = false;
    }
    return isResultBigger;
}

// 仮数の桁数を揃える
void BigDecimal::alignMantissa(BigDecimal* outSub) {
    vector<int8_t> addZero(abs((int)mantissa.size() - (int)outSub->mantissa.size()));
    if (mantissa.size() >= outSub->mantissa.size()) { // 1つ目の数字の仮数の方が長いとき
        outSub->mantissa.insert(outSub->mantissa.begin(),
                                                addZero.begin(), addZero.end());
    } else {
        mantissa.insert(mantissa.begin(), addZero.begin(), addZero.end());
    }
}

// 指数と符号と仮数に対して足し算または引き算をする
BigDecimal BigDecimal::doAdditionOrSubtraction(const BigDecimal& obj,
                                                        bool isPlus) const {
    BigDecimal result(*this);
    BigDecimal sub(obj);
    bool isResultBigger = result.getJudgmentValue(sub);
    int set = 0;
    set = abs(result.index - sub.index);
    for (int i = 0; i < set; i++) {
        if (result.index >= sub.index) { // 1つ目の数字の指数の方が大きいとき
            result.mantissa.push_back((int8_t)0);
        } else {
            sub.mantissa.push_back((int8_t)0);
        }
    }
    if (result.index >= sub.index) { // 1つ目の数字の指数の方が大きいとき
        result.index = sub.index;
    }
    result.alignMantissa(&sub);
    int length = (int)result.mantissa.size();
    if (code == obj.code) { // 両者の符号が等しいとき
        if(isPlus) { // 足し算のとき
            result.getAdditionStack(sub, length);
            result.code = code;
        } else if (isResultBigger) { // 前者の数字の方が絶対値的に大きいとき
            result.mantissa = result.getSubtractionStack(sub, length);
            result.code = code;
        } else {
            result.mantissa = sub.getSubtractionStack(result, length);
            result.code = -code;
        }
    } else {
        if (isPlus) { // 足し算のとき
            if (isResultBigger) { // 前者の数字の方が絶対値的に大きいとき
                result.mantissa = result.getSubtractionStack(sub, length);
                result.code = code;
            } else {
                result.mantissa = sub.getSubtractionStack(result, length);
                result.code = obj.code;
            }
        } else {
            result.getAdditionStack(sub, length);
            result.code = code;
        }
    }
    //result.removeZero(false);
    result.roundOff();
    result.removeZero(false);
    return result;
}

// +演算子のオーバロードを行う
BigDecimal BigDecimal::operator+(const BigDecimal& obj) const {
    return doAdditionOrSubtraction(obj, true);
}

// +=演算子オーバーロード
BigDecimal& BigDecimal::operator+=(const BigDecimal& obj) {
    *this = *this + obj;
    return *this;
}

// -演算子のオーバロードを行う
BigDecimal BigDecimal::operator-(const BigDecimal& obj) const {
    return doAdditionOrSubtraction(obj, false);
}

// -=演算子オーバーロード
BigDecimal& BigDecimal::operator-=(const BigDecimal& obj) {
    *this = *this - obj;
    return *this;
}

// *演算子のオーバロードを行う
BigDecimal BigDecimal::operator*(const BigDecimal& obj) const {
    BigDecimal result;
    BigDecimal sub;
    for (int i = (int)mantissa.size() - 1; i >= 0; i--) {
        int carriedNumber = 0;
        for (int j = (int)obj.mantissa.size() - 1; j >= 0; j--) {
            int stackNumber = (int)mantissa[i] * (int)obj.mantissa[j]
                                                            + carriedNumber;
            sub.mantissa.push_back((int8_t)(stackNumber % 10));
            carriedNumber = (stackNumber - stackNumber % 10) / 10;
        }
        sub.mantissa.push_back((int8_t)carriedNumber);
        reverse(sub.mantissa.begin(), sub.mantissa.end());
        result.alignMantissa(&sub);
        result.getAdditionStack(sub, (int)result.mantissa.size());
        sub.mantissa.clear();
        sub.mantissa.resize((int)mantissa.size() - i);
    }
    result.code = code * obj.code;
    result.index = index + obj.index;
    result.removeZero(false);
    result.roundOff();
    result.removeZero(false);
    return result;
}

// *=演算子オーバーロード
BigDecimal& BigDecimal::operator*=(const BigDecimal& obj) {
    *this = *this * obj;
    return *this;
}

// /演算子のオーバロードを行う
BigDecimal BigDecimal::operator/(const BigDecimal& obj) const {
    if (obj.checkZero(true) == (int)obj.mantissa.size()) { // objの仮数が全てゼロであるとき
        throw runtime_error("Divide by Zero");
    }
    BigDecimal result; // 結果格納用
    BigDecimal dividend; // 割られる数
    dividend.mantissa = this->mantissa;
    BigDecimal currentDividend; // 計算時に使用する割られる数
    BigDecimal divisor;
    divisor.mantissa = obj.mantissa;
    int memoryOfResultLength = 0;
    if (divisor.mantissa.size() < dividend.mantissa.size()) {
        for (int i = 0; i < (int)divisor.mantissa.size(); i++) {
            currentDividend.mantissa.push_back(dividend.mantissa[i]);
        }
        memoryOfResultLength = (int)(dividend.mantissa.size() - currentDividend.mantissa.size());
        if (currentDividend.getJudgmentValue(divisor)) {
            memoryOfResultLength++;
        }
        currentDividend.mantissa.clear();
    } else if (divisor.mantissa.size() == dividend.mantissa.size()) {
        if (dividend.getJudgmentValue(divisor)) {
            memoryOfResultLength = 1;
        }
    }
    int beforeStack = 0;
    int num = 0;
    while ((int)result.mantissa.size() < LIMIT_OF_DIGITS + 1) {
        // 割り算結果が1001桁を超えたとき終了
        num++;
        bool dividendIsBigger = dividend.getJudgmentValue(divisor);
        int stack = 0;
        if (!dividendIsBigger) { // 割られる数が割る数より小さいとき
            stack += abs((int)(dividend.mantissa.size() - divisor.mantissa.size()));
            for (int i = 0; i < stack; i++) {
                dividend.mantissa.push_back((int8_t)0);
            }
        }
        dividendIsBigger = dividend.getJudgmentValue(divisor);
        beforeStack = abs((int)divisor.mantissa.size() - beforeStack);
        if (!dividendIsBigger) { // 同桁に調整してもなお割られる数が割る数より小さいとき
            stack++;
            dividend.mantissa.push_back((int8_t)0);
        }
        for (int i = 0; i < (int)divisor.mantissa.size(); i++) {
            currentDividend.mantissa.push_back(dividend.mantissa[i]);
        }
        dividend.mantissa.erase(dividend.mantissa.begin(),
                    dividend.mantissa.begin() + (int)divisor.mantissa.size());
        bool currentDividendIsBigger = currentDividend.getJudgmentValue(divisor);
        if (!currentDividendIsBigger) { // 直近で使用する割られる数が割る数より小さいとき
            beforeStack++;
            currentDividend.mantissa.push_back(dividend.mantissa[0]);
            dividend.mantissa.erase(dividend.mantissa.begin(),
                                    dividend.mantissa.begin() + 1);
        }
        stack--;
        beforeStack--;
        if (num == 1) {
            for (int i = 0; i < stack; i++) {
                result.mantissa.push_back((int8_t)0); // ここを消してデバッグ①
            }
        } else {
            for (int i = 0; i < beforeStack; i++) {
                result.mantissa.push_back((int8_t)0); // ここを消してデバッグ②
            }
        }
        currentDividendIsBigger = currentDividend.getJudgmentValue(divisor);
        int counter = 0;
        currentDividendIsBigger = true; // ここでは必ずtrueのハズだが念の為
        stack = (int)currentDividend.mantissa.size();
        while (currentDividendIsBigger) {
            currentDividend.alignMantissa(&divisor); // 仮数のみに演算
            int length = (int)currentDividend.mantissa.size();
            currentDividend.getSubtractionStack(divisor, length); // 仮数のみに演算
            currentDividend.removeZero(true);
            divisor.removeZero(true);
            currentDividendIsBigger = currentDividend.getJudgmentValue(divisor);
            counter++;
        }
        beforeStack = (int)currentDividend.mantissa.size();
        result.mantissa.push_back((int8_t)counter); // ここを消してデバッグ③
        if (currentDividend.mantissa.size() == 1
            && currentDividend.mantissa[0] == 0) { // 割った余りがゼロだったとき
            for (int i = 0; i < dividend.checkZero(true); i++) {
                // 余りが0のときの0での調整処理
                result.mantissa.push_back((int8_t)0); // ここを消してデバッグ④
            }
            dividend.removeZero(true);
            if (dividend.mantissa.size() == 1 && dividend.mantissa[0] == 0) { // 全ての割る数が割り切れたとき
                break;
            } else if (dividend.mantissa.empty()) { // 割られる数がもうないとき
                break;
            }
            currentDividend.mantissa.clear();
            if ((int)dividend.mantissa.size() >= (int)divisor.mantissa.size()) {
                beforeStack = (int)divisor.mantissa.size() - 1;
                for (int i = 0; i < (int)divisor.mantissa.size() - 1; i++) {
                    result.mantissa.push_back((int8_t)0); // ここを消してデバッグ⑤
                }
            } else {
                beforeStack = (int)dividend.mantissa.size();
                for (int i = 0; i < (int)dividend.mantissa.size() /*- 1*/; i++) {
                    result.mantissa.push_back((int8_t)0); // ここを消してデバッグ⑥
                }
            }
            continue;
        }
        dividend.mantissa.insert(dividend.mantissa.begin(),
            currentDividend.mantissa.begin(), currentDividend.mantissa.end());
        currentDividend.mantissa.clear();
    }
    result.code = code * obj.code;
    result.index = index - obj.index - ((int)result.mantissa.size()
                                                    - memoryOfResultLength);
    result.removeZero(false);
    return result;
}


// /=演算子オーバーロード
BigDecimal& BigDecimal::operator/=(const BigDecimal& obj) {
    *this = *this / obj;
    return *this;
}

