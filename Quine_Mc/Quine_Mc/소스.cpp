#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
int Number_one(string temp);//1의 갯수를 반환하는 함수
void next_col(vector<vector<string>>& column, vector<vector<string>>& next_column);//Hamming distance가 1인 최소항끼리 결합하여 새로운 벡터를 구성하는 함수
void combine(string& s1, string& s2, string& newstr, int& flag);//최소항끼리 결합하여 새로운 implicant를 만드는 함수
void continue_combine(list < vector<vector<string>>>& qm);//결합이 안될때까지 반복하여 모든 행을 만드는 함수
int Hamming_dis(string s1, string s2);//두 최소항의 Hamming distance를 반환하는 함수
void prime_implicant(vector<vector<string>>& column);//한 행에서 prime implicant를 뽑고 결합여부를 체크하는 함수
void Essential(vector<string> minterm, int num);//prime implicant에서 Essential만 뽑아내는 함수
int bitLen = 0; //비트의 길이
list<string> prime;//prime implicant를 저장하기위한 리스트 
list<string> essential;//최종적으로 Essential만 저장하기위한 리스트
int main() {
    ifstream fin;//파일로부터 읽어오기위한 객체
    ofstream fout;//파일에 저장하기 위한 객체
    string temp;//파일로부터 읽어와 임시로 저장할 변수
    vector<string> minterm; //minterm을 저장할 벡터
    list < vector<vector<string>>> qm;//implicant table의 한 column을 2차원 벡터로 저장하여 노드의 데이터로 가지는 리스트
    int num_min = 0;//minterm의 갯수
    list < vector<vector<string>>>::iterator it;//반복자
    list<string>::iterator it1;
    fin.open("input_minterm.txt");
    fout.open("result.txt");
    getline(fin, temp);
    bitLen = stoi(temp);//bit의 수를 저장

    while (!fin.eof()) {//파일의 끝까지 반복
        getline(fin, temp);
        if (temp == "")//공백 무시
            continue;
        temp.erase(0, 2);//앞에서부터 2칸 삭제
        minterm.push_back(temp);//minterm을 저장하는 리스트에 push
        num_min++;//minterm의 갯수 증가
    }
    vector<vector<string>> column(bitLen + 1); //최소항들이 저장될 2차원 벡터의 행의 크기를 미리 할당
    int num = 0; //1의 개수를 저장하는 변수
    for (int i = 0; i < minterm.size(); i++) {
        num = Number_one(minterm[i]);
        column[num].push_back(minterm[i]);
    } //1의 개수에 해당하는 행에 minterm을 저장

    qm.push_back(column);//qm리스트에 첫 열 push
    continue_combine(qm);//모든 열을 만들어서 qm리스트에 저장함
    for (it = qm.begin(); it != qm.end(); it++) {
        vector<vector<string>> temp = *it;
        prime_implicant(temp);//리스트에 모든 요소를 순환하며 모든 prime implicant를 prime 리스트에 저장
    }
    prime.unique();//중복된 prime implicant 제거
    Essential(minterm, num_min);
    list<string>::iterator iter3;
    for (iter3 = essential.begin(); iter3 != essential.end(); iter3++) {
        cout << *iter3 << endl;
    }
    fin.close();
    fout.close();
}

int Number_one(string temp) { // 1의 개수를 반환하는 함수
    int cnt = 0;
    for (int i = 0; i < temp.length(); i++) {
        if (temp[i] == '1')
            cnt++;
    }
    return cnt;
}

void next_col(vector<vector<string>>& column, vector<vector<string>>& next_column) {//Hamming distance가 1인 최소항끼리 결합하여 새로운 열을 구성하는 함수
    string temp; // 최소항끼리 결합하여 만든 implicant
    for (int i = 0; i < column.size() - 1; i++) {//행
        for (int j = 0; j < column[i].size(); j++) {//열
            if (column[i][j] == "")
                continue;//공백 무시
            for (int k = 0; k < column[i + 1].size(); k++) {
                int flag = 0;
                combine(column[i][j], column[i + 1][k], temp, flag);//한 그룹에 저장된 하나의 minterm과 다음 그룹의 모든 minterm을 비교
                if (flag == 1) {
                    next_column[i].push_back(temp);//결합이 된다면 다음 열에 저장
                }
            }
        }
    }
}

void combine(string& s1, string& s2, string& newstr, int& flag) {//최소항끼리 결합하여 새로운 implicant를 만드는 함수
    int len = min(s1.length(), s2.length());//더 작은 길이 저장
    int index = 0; // 숫자가 다른 비트의 인덱스를 저장하는 변수
    for (int i = 0; i < len; i++) {
        if (s1[i] == s2[i]) {//같으면 건너뜀
            continue;
        }
        else {
            index = i;//다르면 해당 인덱스를 저장
        }
    }
    if (Hamming_dis(s1, s2) != 1) {//Hamming distance가 1이 아니면 결합되지 않음
        flag = 0;
        return;
    }
    else {//Hamming distance가 1이면
        newstr = s1;//s1을 저장
        newstr[index] = '_';//저장한 인덱스를 _로 바꿈
        flag = 1;//flag를 1로 초기화
    }
}
int Hamming_dis(string s1, string s2) {//두 최소항의 Hamming distance를 반환하는 함수
    int len = min(s1.length(), s2.length());
    int cnt = 0;//Hamming distance를 저장할 변수
    for (int i = 0; i < len; i++) {
        if (s1[i] == s2[i]) {
            continue;//같으면 건너뜀
        }
        else {
            cnt++;//다르면 Hamming distance 증가
        }
    }
    return cnt;
}
void prime_implicant(vector<vector<string>>& column) {//한 행에서 prime implicant를 뽑고 결합여부를 체크하는 함수
    for (int i = 0; i < column.size() - 1; i++) {
        for (int j = 0; j < column[i].size(); j++) {
            if (column[i][j] == "")
                continue;
            for (int k = 0; k < column[i + 1].size(); k++) {
                int n = 0;
                n = Hamming_dis(column[i][j], column[i + 1][k]);//한 그룹의 minterm과 다음 그룹의 모든 minterm을 비교
                if (n == 1) {//Hamming distance가 1이라면 결합이 가능하므로
                    if (column[i][j].back() != '*')
                        column[i][j] += '*';//*을 붙혀서 체크
                    if (column[i + 1][k].back() != '*')
                        column[i + 1][k] += '*';//*을 붙혀서 체크
                }
            }
        }
    }
    for (int i = 0; i < column.size() - 1; i++) {
        for (int j = 0; j < column[i].size(); j++) {
            if (column[i][j] != "" && column[i][j].back() != '*')//*이 붙어있지 않으면 PI이므로
                prime.push_back(column[i][j]);//PI를 prime 리스트에 push
        }
    }
}

void continue_combine(list < vector<vector<string>>>& qm) {//결합이 안될때까지 반복하여 모든 열을 만드는 함수
    vector<vector<string>> cur_col = qm.back();//마지막 열을 저장
    vector<vector<string>> next_column(bitLen + 1);//다음 열의 크기를 미리 지정
    next_col(cur_col, next_column);//next_column에 다음 열을 생성
    int k = 0;
    int flag = 0;
    while (k < bitLen + 1) {
        if (!next_column[k].empty())//각 열의 그룹이 비어있지 않다면 flag를 1로 초기화
            flag = 1;
        k++;
    }
    if (flag == 1) {//flag가 1이면 다음 2차원 벡터(열)는 비어있지 않으므로 리스트에 추가 후 다음 결합 실행
        qm.push_back(next_column);//저장된 열을 qm리스트에 push
        continue_combine(qm);//다시 함수를 호출하여 반복
    }
    else {
        return;//flag가 1이 아니라면 다음열이 비어있으므로 종료
    }
}
void Essential(vector<string> minterm, int num) {//prime implicant에서 Essential만 뽑아내는 함수
    int row = prime.size() + 1;//PI의 갯수+1
    int col = num + 1;//minterm의 갯수+1
    int* arr = new int[col + 1];//Essential이 아닌 PI가 포함하는 minterm의 갯수를 저장하는 배열
    vector<vector<string>> chart;//주항차트를 구성하는 2차원 벡터
    vector<string>::iterator iter1 = minterm.begin();
    list<string>::iterator iter2 = prime.begin();
    chart.assign(row + 1, vector<string>(col + 1, ""));//row+1, col+1로 행과열의 크기를 할당
    chart[0][0] = "X";
    for (int i = 0; i < col - 1; i++) {//0행에 minterm들 저장, 0,0은 비워둠
        chart[0][i + 1] = *iter1++;
    }
    for (int i = 0; i < row - 1; i++) {//0열에 PI들 저장, 0,0은 비워둠
        chart[i + 1][0] = *iter2++;
    }
    for (int i = 1; i < row; i++) {//PI
        for (int j = 1; j < col; j++) {//minterm
            int flag = 0;
            for (int k = 0; k < bitLen; k++) {//각 인덱스
                if (chart[i][0][k] == '_') {//i행의 0열인 PI의 k번째 문자가 _이면
                    continue;
                }
                else {//0이나 1일 때
                    if (chart[i][0][k] != chart[0][j][k]) {//k번째 문자가 다르다면
                        flag++;
                    }
                }
            }
            if (flag == 0) {//PI가 minterm을 포함하면
                chart[i][j] = "*";// 해당 PI가 있는 열에 *로 체크 
            }
        }
    }
    for (int i = 1; i < col; i++) {//포함하는 minterm이 1개인 Essenetial PI만 뽑아서 리스트에 저장
        int cnt = 0;//포함하는 minterm의 갯수
        int idx_row;
        int idx_col;
        for (int j = 1; j < row; j++) {
            if (chart[j][i] == "*") {//한 열씩 비교하므로 j,i로 비교
                cnt++;
                idx_row = j;//해당 행 저장
                idx_col = i;//해당 열 저장
            }
        }
        if (cnt == 1&& chart[idx_row][0].back() != '*') {//한 PI가 포함하는 minterm의 갯수가 1개면 Essential PI, 이미 체크된 PI가 아닐 때
            essential.push_back(chart[idx_row][0]);//해당 행의 0열인 PI가 Essential이 됨
            chart[idx_row][0] += "*";//해당 행의 PI는 Essential로 뺌
            chart[0][idx_col] += "*";//해당 열의 minterm은 더이상 고려 X
        }
    }
    for (int i = 1; i < row; i++) {
        if (chart[i][0].back() == '*') {//Essential PI로 뽑은 항들만 체크
            for (int j = 1; j < col; j++) {
                if (chart[i][j] == "*" && chart[0][j].back() != '*') {//체크된 부분의 minterm이 체크되어있지 않다면 체크
                    chart[0][j] += "*";
                }
            }
        }
    }
    while (1) {//모든 minterm을 포함하는 Essential PI가 나올때까지 반복
        bool flag = false;
        for (int i = 1; i < col; i++) {
            if (chart[0][i].back() != '*') {//하나라도 체크가 안된 minterm이 있다면 flag를 true로 바꿈
                flag = true;
            }
        }
        if (!flag) {//flag가 false라면 모든 minterm이 체크되어있으므로 반복종료
            delete[] arr;
            return;
        }
        for (int i = 1; i < row; i++) {
            if (chart[i][0].back() != '*') {//체크가 안된 PI들이 포함시키는 minterm의 갯수를 저장
                for (int j = 1; j < col; j++) {
                    if (chart[i][j] == "*")
                        arr[i]++;
                }
            }
        }
        int max = 0;
        int idx_PI = 0;//최대로 많은 minterm을 저장하는 PI의 행(chart에서)을 저장할 변수
        for (int i = 1; i <= col; i++) {
            if (arr[i] == 0)
                continue;
            if (max <= arr[i]) {
                idx_PI = i;//포함하는 minterm이 가장 많은 인덱스를 저장
            }
        }
        essential.push_back(chart[idx_PI][0]);//해당 PI를 Essential에 넣어줌
        chart[idx_PI][0] += "*";//해당 PI를 체크
        for (int i = 1; i < col; i++) {
            if (chart[idx_PI][i] == "*") {//Essential로 포함한 PI에 해당하는 minterm을 체크
                chart[0][i] += "*";
            }
        }
    }
}
