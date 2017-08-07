#include <iostream>

class BigInt {
private:
    std::string digits; // start from the lowest digit
    bool sign;
public:
    BigInt() {
        this->sign = true;
        this->digits = "0";
    }

    BigInt(std::string notation) {
        size_t len = notation.length();
        if (len == 0) {
            BigInt();
        } else {
            if (notation[0] == '-' || notation[0] == '+') {
                this->sign = (notation[0] == '+') ? true : false;
                if (len == 1) {
                    this->digits = "0";
                } else {
                    notation.erase(0, 1);
                    notation.erase(0, notation.find_first_not_of('0'));
                    if (notation.length() == 0)
                        this->digits = "0";
                    else
                        this->digits = std::string(notation.rbegin(), notation.rend());
                }
            } else {
                this->sign = true;
                notation.erase(0, notation.find_first_not_of('0'));
                if (notation.length() == 0)
                    this->digits = "0";
                else
                    this->digits = std::string(notation.rbegin(), notation.rend());
            }
        }
    }

    // get the right order of digits
    std::string getDigits() const {
        return std::string(this->digits.rbegin(), this->digits.rend());
    }

    char getSign() const {
        return sign ? '+' : '-';
    }

    // set
    void setDigits(std::string digits) {
        if (digits.find_first_not_of('0') != std::string::npos)
            digits.erase(0, digits.find_first_not_of('0'));
        else
            digits = "0";
        digits = std::string(digits.rbegin(), digits.rend());
        this->digits = digits;
    }

    void setSign(bool sign) {
        this->sign = sign;
    }

    int compare(BigInt b) {
        if (this->digits == "0" && b.digits == "0")
            return 0;

        if (this->sign == true && b.sign == false)
            return 1;
        else if (this->sign == false && b.sign == true)
            return -1;
        else if (this->sign == true && b.sign == true) {
            if (this->digits.length() > b.digits.length())
                return 1;
            else if (this->digits.length() < b.digits.length())
                return -1;
            else {
                size_t len = digits.length();
                std::string aDigits = this->getDigits();
                std::string bDigits = b.getDigits();
                for (int i = 0; i < len; i++) {
                    if (aDigits[i] > bDigits[i])
                        return 1;
                    else if (aDigits[i] < bDigits[i])
                        return -1;
                }
                return 0;
            }
        } else {
            if (this->digits.length() > b.digits.length())
                return -1;
            else if (this->digits.length() < b.digits.length())
                return 1;
            else {
                size_t len = digits.length();
                std::string aDigits = this->getDigits();
                std::string bDigits = b.getDigits();
                for (int i = 0; i < len; i++) {
                    if (aDigits[i] > bDigits[i])
                        return -1;
                    else if (aDigits[i] < bDigits[i])
                        return 1;
                }
                return 0;
            }
        }
    }

    bool operator>(BigInt b) {
        return this->compare(b) == 1 ? true : false;
    }

    bool operator==(BigInt b) {
        return this->compare(b) == 0 ? true : false;
    }

    bool operator<(BigInt b) {
        return this->compare(b) == -1 ? true : false;
    }

    bool operator<=(BigInt b) {
        return this->compare(b) == 1 ? false : true;
    }

    bool operator>=(BigInt b) {
        return this->compare(b) == -1 ? false : true;
    }

    BigInt abs() {
        BigInt abs;
        abs.sign = true;
        abs.digits = this->digits;
        return abs;
    }

    BigInt operator+(BigInt b) {
        bool resultSign;
        std::string resultDigits = "";
        BigInt result;
        int8_t value, carry = 0;
        if (this->sign == b.sign) {
            resultSign = b.sign;
            std::string add1, add2;
            if (this->digits.length() >= b.digits.length()) {
                add1 = this->digits;
                add2 = b.digits;
            } else {
                add1 = b.digits;
                add2 = this->digits;
            }
            size_t len1 = add1.length();
            size_t len2 = add2.length();

            for (size_t i = 0; i < len2; i++) {
                value = add1[i] - '0' + add2[i] - '0' + carry;
                carry = value > 9 ? 1 : 0;
                value %= 10;
                resultDigits.append(1, '0' + value);
            }
            if (len1 == len2) {
                if (carry > 0)
                    resultDigits.append(1, '0' + carry);
            } else {
                for (size_t i = len2; i < len1; i++) {
                    value = add1[i] - '0' + carry;
                    carry = value > 9 ? 1 : 0;
                    value %= 10;
                    resultDigits.append(1, '0' + value);
                }
                if (carry > 0)
                    resultDigits.append(1, '0' + carry);
            }
            removeTrailingZeros(resultDigits);
            result.digits = resultDigits;
            result.sign = resultSign;
            return result;
        } else {
            BigInt add1, add2;
            if (this->sign) {
                add1 = *this;
                add2 = b;
            } else {
                add1 = b;
                add2 = *this;
            }
            if (add1.abs() >= add2.abs()) {
                result = add1.minus1(add2);
                removeTrailingZeros(result.digits);
                return result;
            } else {
                result = add2.minus1(add1);
                result.sign = false;
                removeTrailingZeros(result.digits);
                return result;
            }
        }
    }

    BigInt operator-(BigInt b) {
        b.setSign(!b.sign);
        return *this + b;
    }

    BigInt &operator+=(const BigInt &m) {
        *this = *this + m;
        return *this;
    }

    BigInt &operator-=(const BigInt &m) {
        *this = *this - m;
        return *this;
    }

    BigInt operator*(BigInt b) {
        BigInt prod1, prod2;
        // prod1 is at least longer than prod2
        if (this->digits.length() >= b.digits.length()) {
            prod1 = *this;
            prod2 = b;
        } else {
            prod1 = b;
            prod2 = *this;
        }
        size_t len1, len2;
        len1 = prod1.digits.length();
        len2 = prod2.digits.length();

        BigInt tmpSum("0");
        for (size_t i = 0; i < len2; i++) {
            BigInt tmpAdd = prod1.timesOneDigit(prod2.digits[i]);
            tmpAdd.digits.insert(0, i, '0');
            tmpSum += tmpAdd;
        }
        if (prod1.sign != prod2.sign)
            tmpSum.sign = false;
        else
            tmpSum.sign = true;
        return tmpSum;
    }

    // only return the quotient
    BigInt operator/(BigInt b) {
        if (b == BigInt("0")) {
            throw std::overflow_error("Divide by zero exception");
        }
        std::string reverseOrderDigits = "";
        BigInt remain = *this;
        remain.sign = true;

        char curDigit;
        if (remain < b.abs()) {
            std::string sign = this->sign == b.sign ? "+" : "-";
            return BigInt(sign + "0");
        }
        // digitsDiff -> quotient's digits will be between (digitsDiff, digitsDiff + 1)
        size_t digitsDiff = this->digits.length() - b.digits.length();
        size_t len2 = b.digits.length();
        size_t remainDigits = len2 - 1;
        // i is the number of 0s you need to add for dividing
        for (int i = digitsDiff; i >= 0; i--) {
            curDigit = calOneDigitForDivide(remain.getDigits().substr(0, remainDigits + 1), b.getDigits(),
                                            &remainDigits);
            reverseOrderDigits.append(1, curDigit);
            remain = remain - b.abs() * BigInt(std::string(1, '1').append(i, '0')) * BigInt(std::string(1, curDigit));
        }
        BigInt quotient(reverseOrderDigits);
        quotient.sign = this->sign == b.sign;
        return quotient;
    }


private:
    /*
     * a bigger positive minus smaller positive number;
     */
    BigInt minus1(BigInt b) {
        int8_t borrow = 0, value;
        std::string resultDigits = "";
        std::string minus1 = this->digits;
        std::string minus2 = b.digits;
        size_t len1 = minus1.length();
        size_t len2 = minus2.length();

        for (size_t i = 0; i < len2; i++) {
            value = minus1[i] - minus2[i] - borrow;
            borrow = value < 0 ? 1 : 0;
            value = borrow ? value + 10 : value;
            resultDigits.append(1, '0' + value);
        }
        for (size_t i = len2; i < len1; i++) {
            value = minus1[i] - '0' - borrow;
            borrow = value < 0 ? 1 : 0;
            value = borrow ? value + 10 : value;
            resultDigits.append(1, '0' + value);
        }
        BigInt result;
        result.digits = resultDigits;
        result.sign = true;
        return result;
    }

    /*
     * multi digits number times one digit number
     */
    BigInt timesOneDigit(char oneDigit) {
        size_t len = this->digits.length();
        char value, carry = 0;
        std::string resultDigits("");
        std::string prod1 = this->digits;
        for (size_t i = 0; i < len; i++) {
            value = (prod1[i] - '0') * (oneDigit - '0') + carry;
            carry = value / 10;
            value %= 10;
            resultDigits.append(1, '0' + value);
        }
        if (carry > 0) {
            resultDigits.append(1, '0' + carry);
        }
        BigInt result;
        result.digits = resultDigits;
        result.sign = this->sign;
        return result;
    }

    /*
     * calculate only one digit, for dividing
     */
    char static calOneDigitForDivide(std::string d1, std::string d2, size_t *remainDigits) {
        BigInt dividend(d1), divisor(d2);
        BigInt curRemain;
        const BigInt zero("0");
        if (dividend < divisor) {
            *remainDigits = d1.length();
            return '0';
        }
        char start = 1 + '0', end = 9 + '0', curDigit;
        BigInt curProduct;
        do {
            curDigit = (start + end) / 2;
            curProduct = BigInt(std::string(1, curDigit)) * divisor;
            curRemain = dividend - curProduct;
            if (curRemain >= zero && curRemain < divisor) {
                size_t len = curRemain.getDigits().length();
                *remainDigits = curRemain == zero ? 0 : len;
                return curDigit;
            } else if (curRemain < zero) {
                end = curDigit - 1;
            } else {
                start = curDigit + 1;
            }
        } while (1);
    }


    void static removeTrailingZeros(std::string &digits) {
        size_t tmp = digits.find_last_not_of('0');
        if (tmp != std::string::npos) {
            digits.erase(tmp + 1, SIZE_T_MAX);
        } else {
            digits = "0";
        }
    }
};

std::ostream &operator<<(std::ostream &os, BigInt const &m) {
    os << m.getDigits().insert(0, 1, m.getSign());
    return os;
}

int main() {
    BigInt a("+0");
    BigInt b("+234");
    BigInt c("+233");
    BigInt d("-233");
    BigInt e("-893242342348932479823749235623490872190346");
    BigInt f("3234");
    std::cout << (e / f) << std::endl;
    std::cout << (e + f) << std::endl;
    std::cout << (e - f) << std::endl;
    std::cout << (e * f) << std::endl;
    return 0;
}