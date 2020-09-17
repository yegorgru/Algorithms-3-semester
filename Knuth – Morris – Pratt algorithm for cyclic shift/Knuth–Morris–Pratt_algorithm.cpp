#include <iostream>
#include <string>
#include <deque>

#include "TestFramework.h"
#include "Profile.h"

using namespace std;

bool check(const string& str1, const string& str2) {
    if (str1.size() != str2.size()) {
        //Якщо рядки мають різну довжину - повернути false
        return false;
    }
    else if (str1.size() == 0) {
        //Якщо довжина обох рядків = 0 - повернути true
        return true;
    }
    else {
        size_t pos_in_first=0;  //Номер наступного символу для порівняння в першому рядку
        size_t pos_in_second=0; //Номер наступного символу для порівняння в другому рядку
        std::deque<size_t> next_first_char; //Контейнер для зберігання можливих номерів 
            //перших елементів циклічного зсуву. Складність доступу до елементу за індексом,
            //вставки в кінець і видалення з початку = О(1)
        bool was_end=false; //Змінна, яка відслідковує, чи був досягнутий кінець другого рядка
            //і блокує зациклювання програми
        while(true){
            if (str1[pos_in_first] == str2[pos_in_second]) {
                if (pos_in_first == str1.size()-1) {
                    //Всі символи співпали
                    return true;
                }
                else {
                    if (str2[pos_in_second] == str1[0]) {
                        //pos_in_second - можливий початок циклічного зсуву
                        next_first_char.push_back(pos_in_second);
                    }
                    //Підсунути позицію в першому рядку на наступний символ
                    pos_in_first++;
                    if (pos_in_second == str2.size() - 1) {
                        //pos_in_second досягла кінця другого рядка
                        if (was_end) {
                            //Якщо це сталося вдруге - повернути false
                            return false;
                        }
                        else {
                            //Інакше - відзначити, що був досягнутий кінець 2 рядка
                            //і перескочити на його початок
                            pos_in_second = 0;
                            was_end = true;
                        }
                    }
                    else {
                        //Підсунути позицію в другому рядку на наступний символ
                        pos_in_second++;
                    }
                }
            }
            //Символи у рядках не не співпали, але є варіанти 
            //відновити порівняння не з самого початку
            else if (next_first_char.size()>1) {
                //next_first_char[0] - це початок цього порівняння,
                //тому берем next_first_char[1]
                //Оновлюємо pos_in_first для нового порівняння
                if (next_first_char[1] > pos_in_second) {
                    pos_in_first = pos_in_second + str2.size() - next_first_char[1];
                }
                else {
                    pos_in_first = pos_in_second - next_first_char[1];
                }
                //Видаляємо інформацію про перший елемент минулого порівняння
                next_first_char.pop_front();
            }
            //Символи у рядках не не співпали, немає варіантів
            //відновити порівняння не з самого початку
            else {
                //Оновлюємо pos_in_first
                pos_in_first = 0;
                if (was_end) {
                    //Якщо кінець другого рядка вже досягався - повернути false
                    return false;
                }
                if (str2[pos_in_second] != str1[0]) {
                    //Оновлюємо pos_in_second
                    if (pos_in_second == str2.size() - 1) {
                        //pos_in_second досягла кінця другого рядка
                        //Відзначити, що був досягнутий кінець 2 рядка
                        //і перескочити на його початок
                        pos_in_second = 0;
                        was_end = true;
                    }
                    else {
                        pos_in_second++;
                    }
                }
                //Видаляємо інформацію про перший елемент минулого порівняння
                next_first_char.clear();
            }
        }
    }    
}

void Tests() {
    ASSERT(check("", ""));
    ASSERT(check("a", "a"));
    ASSERT(check("ab", "ba"));
    ASSERT(check("abc", "cab"));
    ASSERT(check("aabaabaabaabkaab", "kaabaabaabaabaab"));
    ASSERT(check("ABC ABCDAB ABCDABCDABDE", "ABCDABCDABDEABC ABCDAB "));
    ASSERT(check("sdfsdfsdfhjgcjsdhctysafchvhdc", "jgcjsdhctysafchvhdcsdfsdfsdfh"));
    ASSERT(check("AAAAAAAAAAAAB", "AAABAAAAAAAAA"));
    ASSERT(check("AAAAAAABAAAAAAAAA", "AAABAAAAAAAAAAAAA"));
    ASSERT(check("typabcabcirtyr", "rtyrtypabcabci"));
    ASSERT(check("abcde", "deabc"));
    
    ASSERT(!check("AAAAAAAAAAA", "AAAAABAAAAA"));
    ASSERT(!check("a", "h"));
    ASSERT(!check("ac", "ba"));
    ASSERT(!check("ahc", "cab"));
    ASSERT(!check("abh", "cab"));
    ASSERT(!check("aabaabaabaabkaab", "kaabaabadbaabaab"));
    ASSERT(!check("ABC ABCDAB ABCDABCDABDE", "ABCDABAABCDABCDABDEABC "));
    ASSERT(!check("mdfsdfsdfhjgcjsdhctysafchvhdc", "jgcjsdhctysafchvhdcsdfsdfsdfh"));
    ASSERT(!check("sdfsdfsdfhjgcjsdhctysafchvhdc", "jgcjsdhctysafchvhdcsdfhdfsdfh"));
    ASSERT(!check("AAAAAAAAAAAAB", "AAABvAAAAAAAA"));
    ASSERT(!check("hAAAAAABAAAAAAAAA", "AAABAAAAAAAAAAAAA"));
    ASSERT(!check("AAAAAAABAAAAAAAAA", "AAABAAAAAAAAAAAAk"));
    ASSERT(!check("seegeweg", "xdisduft"));
}

void TestSpeed() {
    {
        LOG_DURATION("Worst case false");
        string s1(1000000, 'A');
        string s2(1000000, 'A');
        s2[999999] = 'B';
        ASSERT(!check(s1, s2));
    }
    {
        LOG_DURATION("Worst case false reverse");
        string s1(1000000, 'A');
        string s2(1000000, 'A');
        s2[999999] = 'B';
        ASSERT(!check(s2, s1));
    }
    {
        LOG_DURATION("Worst case true");
        string s1(1000000, 'A');
        string s2(1000000, 'A');
        s2[999999] = 'B';
        s1[0] = 'B';
        ASSERT(check(s1, s2));
    }
    {
        LOG_DURATION("Worst case true reverse");
        string s1(1000000, 'A');
        string s2(1000000, 'A');
        s2[999999] = 'B';
        s1[0] = 'B';
        ASSERT(check(s2, s1));
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr,Tests);

    RUN_TEST(tr,TestSpeed);
   
    string str1, str2;
    cout << "Enter first string" << endl;
    getline(cin,str1);
    cout << "Enter second string" << endl;
    getline(cin, str2);
    if (check(str1, str2)) {
        cout << "This is cyclic shift" << endl;
    }
    else {
        cout << "This is not cyclic shift" << endl;
    }
    return 0;
}