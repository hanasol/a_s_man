#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;
int Number_one(string temp);//1�� ������ ��ȯ�ϴ� �Լ�
void next_col(vector<vector<string>>& column, vector<vector<string>>& next_column);//Hamming distance�� 1�� �ּ��׳��� �����Ͽ� ���ο� ���͸� �����ϴ� �Լ�
void combine(string& s1, string& s2, string& newstr, int& flag);//�ּ��׳��� �����Ͽ� ���ο� implicant�� ����� �Լ�
void continue_combine(list < vector<vector<string>>>& qm);//������ �ȵɶ����� �ݺ��Ͽ� ��� ���� ����� �Լ�
int Hamming_dis(string s1, string s2);//�� �ּ����� Hamming distance�� ��ȯ�ϴ� �Լ�
void prime_implicant(vector<vector<string>>& column);//�� �࿡�� prime implicant�� �̰� ���տ��θ� üũ�ϴ� �Լ�
void Essential(vector<string> minterm, int num);//prime implicant���� Essential�� �̾Ƴ��� �Լ�
int bitLen = 0; //��Ʈ�� ����
list<string> prime;//prime implicant�� �����ϱ����� ����Ʈ 
list<string> essential;//���������� Essential�� �����ϱ����� ����Ʈ
int main() {
    ifstream fin;//���Ϸκ��� �о�������� ��ü
    ofstream fout;//���Ͽ� �����ϱ� ���� ��ü
    string temp;//���Ϸκ��� �о�� �ӽ÷� ������ ����
    vector<string> minterm; //minterm�� ������ ����
    list < vector<vector<string>>> qm;//implicant table�� �� column�� 2���� ���ͷ� �����Ͽ� ����� �����ͷ� ������ ����Ʈ
    int num_min = 0;//minterm�� ����
    list < vector<vector<string>>>::iterator it;//�ݺ���
    list<string>::iterator it1;
    fin.open("input_minterm.txt");
    fout.open("result.txt");
    getline(fin, temp);
    bitLen = stoi(temp);//bit�� ���� ����

    while (!fin.eof()) {//������ ������ �ݺ�
        getline(fin, temp);
        if (temp == "")//���� ����
            continue;
        temp.erase(0, 2);//�տ������� 2ĭ ����
        minterm.push_back(temp);//minterm�� �����ϴ� ����Ʈ�� push
        num_min++;//minterm�� ���� ����
    }
    vector<vector<string>> column(bitLen + 1); //�ּ��׵��� ����� 2���� ������ ���� ũ�⸦ �̸� �Ҵ�
    int num = 0; //1�� ������ �����ϴ� ����
    for (int i = 0; i < minterm.size(); i++) {
        num = Number_one(minterm[i]);
        column[num].push_back(minterm[i]);
    } //1�� ������ �ش��ϴ� �࿡ minterm�� ����

    qm.push_back(column);//qm����Ʈ�� ù �� push
    continue_combine(qm);//��� ���� ���� qm����Ʈ�� ������
    for (it = qm.begin(); it != qm.end(); it++) {
        vector<vector<string>> temp = *it;
        prime_implicant(temp);//����Ʈ�� ��� ��Ҹ� ��ȯ�ϸ� ��� prime implicant�� prime ����Ʈ�� ����
    }
    prime.unique();//�ߺ��� prime implicant ����
    Essential(minterm, num_min);
    list<string>::iterator iter3;
    for (iter3 = essential.begin(); iter3 != essential.end(); iter3++) {
        cout << *iter3 << endl;
    }
    fin.close();
    fout.close();
}

int Number_one(string temp) { // 1�� ������ ��ȯ�ϴ� �Լ�
    int cnt = 0;
    for (int i = 0; i < temp.length(); i++) {
        if (temp[i] == '1')
            cnt++;
    }
    return cnt;
}

void next_col(vector<vector<string>>& column, vector<vector<string>>& next_column) {//Hamming distance�� 1�� �ּ��׳��� �����Ͽ� ���ο� ���� �����ϴ� �Լ�
    string temp; // �ּ��׳��� �����Ͽ� ���� implicant
    for (int i = 0; i < column.size() - 1; i++) {//��
        for (int j = 0; j < column[i].size(); j++) {//��
            if (column[i][j] == "")
                continue;//���� ����
            for (int k = 0; k < column[i + 1].size(); k++) {
                int flag = 0;
                combine(column[i][j], column[i + 1][k], temp, flag);//�� �׷쿡 ����� �ϳ��� minterm�� ���� �׷��� ��� minterm�� ��
                if (flag == 1) {
                    next_column[i].push_back(temp);//������ �ȴٸ� ���� ���� ����
                }
            }
        }
    }
}

void combine(string& s1, string& s2, string& newstr, int& flag) {//�ּ��׳��� �����Ͽ� ���ο� implicant�� ����� �Լ�
    int len = min(s1.length(), s2.length());//�� ���� ���� ����
    int index = 0; // ���ڰ� �ٸ� ��Ʈ�� �ε����� �����ϴ� ����
    for (int i = 0; i < len; i++) {
        if (s1[i] == s2[i]) {//������ �ǳʶ�
            continue;
        }
        else {
            index = i;//�ٸ��� �ش� �ε����� ����
        }
    }
    if (Hamming_dis(s1, s2) != 1) {//Hamming distance�� 1�� �ƴϸ� ���յ��� ����
        flag = 0;
        return;
    }
    else {//Hamming distance�� 1�̸�
        newstr = s1;//s1�� ����
        newstr[index] = '_';//������ �ε����� _�� �ٲ�
        flag = 1;//flag�� 1�� �ʱ�ȭ
    }
}
int Hamming_dis(string s1, string s2) {//�� �ּ����� Hamming distance�� ��ȯ�ϴ� �Լ�
    int len = min(s1.length(), s2.length());
    int cnt = 0;//Hamming distance�� ������ ����
    for (int i = 0; i < len; i++) {
        if (s1[i] == s2[i]) {
            continue;//������ �ǳʶ�
        }
        else {
            cnt++;//�ٸ��� Hamming distance ����
        }
    }
    return cnt;
}
void prime_implicant(vector<vector<string>>& column) {//�� �࿡�� prime implicant�� �̰� ���տ��θ� üũ�ϴ� �Լ�
    for (int i = 0; i < column.size() - 1; i++) {
        for (int j = 0; j < column[i].size(); j++) {
            if (column[i][j] == "")
                continue;
            for (int k = 0; k < column[i + 1].size(); k++) {
                int n = 0;
                n = Hamming_dis(column[i][j], column[i + 1][k]);//�� �׷��� minterm�� ���� �׷��� ��� minterm�� ��
                if (n == 1) {//Hamming distance�� 1�̶�� ������ �����ϹǷ�
                    if (column[i][j].back() != '*')
                        column[i][j] += '*';//*�� ������ üũ
                    if (column[i + 1][k].back() != '*')
                        column[i + 1][k] += '*';//*�� ������ üũ
                }
            }
        }
    }
    for (int i = 0; i < column.size() - 1; i++) {
        for (int j = 0; j < column[i].size(); j++) {
            if (column[i][j] != "" && column[i][j].back() != '*')//*�� �پ����� ������ PI�̹Ƿ�
                prime.push_back(column[i][j]);//PI�� prime ����Ʈ�� push
        }
    }
}

void continue_combine(list < vector<vector<string>>>& qm) {//������ �ȵɶ����� �ݺ��Ͽ� ��� ���� ����� �Լ�
    vector<vector<string>> cur_col = qm.back();//������ ���� ����
    vector<vector<string>> next_column(bitLen + 1);//���� ���� ũ�⸦ �̸� ����
    next_col(cur_col, next_column);//next_column�� ���� ���� ����
    int k = 0;
    int flag = 0;
    while (k < bitLen + 1) {
        if (!next_column[k].empty())//�� ���� �׷��� ������� �ʴٸ� flag�� 1�� �ʱ�ȭ
            flag = 1;
        k++;
    }
    if (flag == 1) {//flag�� 1�̸� ���� 2���� ����(��)�� ������� �����Ƿ� ����Ʈ�� �߰� �� ���� ���� ����
        qm.push_back(next_column);//����� ���� qm����Ʈ�� push
        continue_combine(qm);//�ٽ� �Լ��� ȣ���Ͽ� �ݺ�
    }
    else {
        return;//flag�� 1�� �ƴ϶�� �������� ��������Ƿ� ����
    }
}
void Essential(vector<string> minterm, int num) {//prime implicant���� Essential�� �̾Ƴ��� �Լ�
    int row = prime.size() + 1;//PI�� ����+1
    int col = num + 1;//minterm�� ����+1
    int* arr = new int[col + 1];//Essential�� �ƴ� PI�� �����ϴ� minterm�� ������ �����ϴ� �迭
    vector<vector<string>> chart;//������Ʈ�� �����ϴ� 2���� ����
    vector<string>::iterator iter1 = minterm.begin();
    list<string>::iterator iter2 = prime.begin();
    chart.assign(row + 1, vector<string>(col + 1, ""));//row+1, col+1�� ������� ũ�⸦ �Ҵ�
    chart[0][0] = "X";
    for (int i = 0; i < col - 1; i++) {//0�࿡ minterm�� ����, 0,0�� �����
        chart[0][i + 1] = *iter1++;
    }
    for (int i = 0; i < row - 1; i++) {//0���� PI�� ����, 0,0�� �����
        chart[i + 1][0] = *iter2++;
    }
    for (int i = 1; i < row; i++) {//PI
        for (int j = 1; j < col; j++) {//minterm
            int flag = 0;
            for (int k = 0; k < bitLen; k++) {//�� �ε���
                if (chart[i][0][k] == '_') {//i���� 0���� PI�� k��° ���ڰ� _�̸�
                    continue;
                }
                else {//0�̳� 1�� ��
                    if (chart[i][0][k] != chart[0][j][k]) {//k��° ���ڰ� �ٸ��ٸ�
                        flag++;
                    }
                }
            }
            if (flag == 0) {//PI�� minterm�� �����ϸ�
                chart[i][j] = "*";// �ش� PI�� �ִ� ���� *�� üũ 
            }
        }
    }
    for (int i = 1; i < col; i++) {//�����ϴ� minterm�� 1���� Essenetial PI�� �̾Ƽ� ����Ʈ�� ����
        int cnt = 0;//�����ϴ� minterm�� ����
        int idx_row;
        int idx_col;
        for (int j = 1; j < row; j++) {
            if (chart[j][i] == "*") {//�� ���� ���ϹǷ� j,i�� ��
                cnt++;
                idx_row = j;//�ش� �� ����
                idx_col = i;//�ش� �� ����
            }
        }
        if (cnt == 1&& chart[idx_row][0].back() != '*') {//�� PI�� �����ϴ� minterm�� ������ 1���� Essential PI, �̹� üũ�� PI�� �ƴ� ��
            essential.push_back(chart[idx_row][0]);//�ش� ���� 0���� PI�� Essential�� ��
            chart[idx_row][0] += "*";//�ش� ���� PI�� Essential�� ��
            chart[0][idx_col] += "*";//�ش� ���� minterm�� ���̻� ��� X
        }
    }
    for (int i = 1; i < row; i++) {
        if (chart[i][0].back() == '*') {//Essential PI�� ���� �׵鸸 üũ
            for (int j = 1; j < col; j++) {
                if (chart[i][j] == "*" && chart[0][j].back() != '*') {//üũ�� �κ��� minterm�� üũ�Ǿ����� �ʴٸ� üũ
                    chart[0][j] += "*";
                }
            }
        }
    }
    while (1) {//��� minterm�� �����ϴ� Essential PI�� ���ö����� �ݺ�
        bool flag = false;
        for (int i = 1; i < col; i++) {
            if (chart[0][i].back() != '*') {//�ϳ��� üũ�� �ȵ� minterm�� �ִٸ� flag�� true�� �ٲ�
                flag = true;
            }
        }
        if (!flag) {//flag�� false��� ��� minterm�� üũ�Ǿ������Ƿ� �ݺ�����
            delete[] arr;
            return;
        }
        for (int i = 1; i < row; i++) {
            if (chart[i][0].back() != '*') {//üũ�� �ȵ� PI���� ���Խ�Ű�� minterm�� ������ ����
                for (int j = 1; j < col; j++) {
                    if (chart[i][j] == "*")
                        arr[i]++;
                }
            }
        }
        int max = 0;
        int idx_PI = 0;//�ִ�� ���� minterm�� �����ϴ� PI�� ��(chart����)�� ������ ����
        for (int i = 1; i <= col; i++) {
            if (arr[i] == 0)
                continue;
            if (max <= arr[i]) {
                idx_PI = i;//�����ϴ� minterm�� ���� ���� �ε����� ����
            }
        }
        essential.push_back(chart[idx_PI][0]);//�ش� PI�� Essential�� �־���
        chart[idx_PI][0] += "*";//�ش� PI�� üũ
        for (int i = 1; i < col; i++) {
            if (chart[idx_PI][i] == "*") {//Essential�� ������ PI�� �ش��ϴ� minterm�� üũ
                chart[0][i] += "*";
            }
        }
    }
}
