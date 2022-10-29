#include "BigDecimal.h"

using namespace std;

// 乱数群を生成する
vector<int> generateRandomNumbers() {
    random_device random;
    mt19937 engine(random());
    uniform_int_distribution<> dist(1, 10);
    vector<int> randomNumbers;
    for (int i = 0; i < 10000; i++) {
        randomNumbers.push_back(dist(engine));
    }
    return randomNumbers;
}

// 乱数を1つ生成する
int generateRandomNumber(random_device *random, bool forOperators) {
    mt19937 engine((*random)());
    if (forOperators) {
        uniform_int_distribution<> dist1(1, 4);
        return dist1(engine);
    } else {
        uniform_int_distribution<> dist2(0, 10000);
        return dist2(engine);
    }
}

// 末尾のゼロを削除
string removeEndingZero(const string& str) {
    string copy = str;
    int counter = 0;
    for (int i = (int)copy.size() - 1; i >= 0; i--) {
        if (copy[i] == '0') {
            counter++;
        } else {
            break;
        }
    }
    copy.erase(copy.end() - counter, copy.end());
    return copy;
}

int main(int argc, char* argv[]) {
   
    try {
        random_device random;
        int counter = 0;
        BigDecimal bd3;
        BigDecimal bd4;
        BigDecimal bd5;
        int operators = 0;
        clock_t start = clock();
        for (int i = 0; i < 10000; i++) {
            bd3.setRandom(generateRandomNumbers()[generateRandomNumber(&random, false)]);
            bd4.setRandom(generateRandomNumbers()[generateRandomNumber(&random, false)]);
            cout << endl;
            cout << "first : " << bd4.toString() << endl;
            cout << "second : " << bd3.toString() << endl;
            operators = generateRandomNumber(&random, true);
            string comp2;
            if (operators == 1) {
                bd5 = bd4 + bd3;
                cout << "BigDecimal 計算結果(+) : " << bd5.toString() << endl;
                cout << "Double 計算結果(+) : " << setprecision(20)
                    << bd4.convertToDouble() + bd3.convertToDouble() << endl;
                cout << endl;
                comp2 = removeEndingZero(to_string(bd4.convertToDouble() + bd3.convertToDouble()));
            } else if (operators == 2) {
                bd5 = bd4 - bd3;
                cout << "BigDecimal 計算結果(-) : " << bd5.toString() << endl;
                cout << "Double 計算結果(-) : " << setprecision(20)
                    << bd4.convertToDouble() - bd3.convertToDouble() << endl;
                cout << endl;
                comp2 = removeEndingZero(to_string(bd4.convertToDouble() - bd3.convertToDouble()));
            } else if (operators == 3) {
                bd5 = bd4 * bd3;
                cout << "BigDecimal 計算結果(*) : " << bd5.toString() << endl;
                cout << "Double 計算結果(*) : " << setprecision(20)
                    << bd4.convertToDouble() * bd3.convertToDouble() << endl;
                cout << endl;
                comp2 = removeEndingZero(to_string(bd4.convertToDouble() * bd3.convertToDouble()));
            } else if (operators == 4) {
                bd5 = bd4 / bd3;
                cout << "BigDecimal 計算結果(/) : " << bd5.toString() << endl;
                cout << "Double 計算結果(/) : " << setprecision(20)
                    << bd4.convertToDouble() / bd3.convertToDouble() << endl;
                cout << endl;
                comp2 = removeEndingZero(to_string(bd4.convertToDouble() / bd3.convertToDouble()));
                comp2.erase(comp2.end() - 1, comp2.end());
            }
            counter++;
            string comp1 = bd5.toString();
            int stack = 0;
            if (comp1.size() > comp2.size()) {
                // -1しておくのは丸め誤差での差異に引っかからないようにするため
                stack = (int)comp2.size() - 1;
            } else {
                stack = (int)comp1.size() - 1;
            }
            stack = min(12, stack);
            for (int i = 0; i < stack - 1; i++) {
                if (comp2[i] != comp1[i]) {
                    cout << "comp1 : " << comp1 << endl;
                    cout << "comp2 : " << comp2 << endl;
                    cout << "誤差が発生しています" << endl;
                    cout << "演算回数 : " << counter << endl;
                    assert(comp2[i] == comp1[i]);
                    break;
                }
            }
        }
        clock_t end = clock();
        cout << " 計算時間 : " << 1000.0*((double)(end - start) / CLOCKS_PER_SEC) << " [ms]" << endl;
        cout << "演算回数 : " << counter << endl;
    } catch (const runtime_error& e) {
            cerr << e.what() << endl;
    }
    return 0;
}

