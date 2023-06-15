#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>

//9 ������� - �, �, �
//���������� - ���������, ������, ���������� ��������
//��������� - ���, �����, ������ 


using namespace std;

string inputFile = "data.txt";
string outputFile = "output.txt";
string timeStampsFile = "timestamps.txt";

ofstream foutTime(timeStampsFile);

int batchNum = 4;

int dims[7] = { 100, 500, 1000, 5000, 10000, 50000, 100000 };

struct Product {
    string name;
    string country;
    unsigned int volume;
    unsigned int sum;

    Product() {

    }

    Product(string name, string country, unsigned int volume, unsigned int sum)
    {
        this->name = name;
        this->country = country;
        this->volume = volume;
        this->sum = sum;
    }

    //���������� ����������
    friend bool operator== (const Product& a, const Product& b) {
        if (a.name == b.name && a.volume == b.volume
            && a.country == b.country)
            return true;
        return false;
    }

    friend bool operator< (const Product& a, const Product& b) {
        if (a.name < b.name || ((a.name == b.name) && (a.volume < b.volume)) ||
            ((a.name == b.name) && (a.volume == b.volume) && (a.country < b.country)))
            return true;
        return false;
    }

    friend bool operator<= (const Product& a, const Product& b) {
        if (a < b || a == b)
            return true;
        return false;
    }

    friend bool operator> (const Product& a, const Product& b) {
        if (!(a < b) && !(a == b))
            return true;
        return false;
    }

    friend bool operator>= (const Product& a, const Product& b) {
        if (!(a < b))
            return true;
        return false;
    }


    friend ostream& operator<<(ostream& os, const Product& a) {
        os << a.name << ' ' << a.volume << ' ' << a.country << '\n';
        return os;
    }
};

string randomString(const int len) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

void makeTxtFile() {
    srand((unsigned)time(NULL));
    ofstream fout(inputFile);

    for (int i = 0; i < batchNum; ++i) {
        //����� ����� �������
        fout << dims[i] << "\n";
        for (int j = 0; j < dims[i]; ++j) {
            //����� ����� �� �������
            fout << randomString(8) << "\n";    //���
            fout << randomString(10) << "\n";   //������
            fout << rand() + 1 << "\n";         //�����
            fout << rand() + 1 << "\n";         //�����
        }
    }
}

vector<vector<Product>> readTxtFile() {
    ifstream fin(inputFile);

    vector<vector<Product>> result;

    int dim;
    string name;
    string country;
    unsigned int volume;
    unsigned int sum;

    for (int i = 0; i < batchNum; ++i) {
        //���� ����� �������
        fin >> dim;
        vector<Product> v;
        for (int j = 0; j < dim; ++j) {
            //���� ����� �� �������
            fin >> name >> country >> volume >> sum;
            Product temp(name, country, volume, sum);
            v.push_back(temp);
        }
        result.push_back(v);
    }

    return result;
}

//���������� ���������
void bubbleSort(vector<Product>& a) {
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < a.size() - i; j++) {
            if (j + 1 < a.size() && a[j] > a[j + 1])
                std::swap(a[j], a[j + 1]);
        }
    }

    return;
}

//���������� ��������
void shakerSort(vector<Product>& arr) {
    int control = arr.size() - 1;
    int left = 0;
    int right = arr.size() - 1;
    do {
        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                control = i;
            }
        }
        right = control;
        for (int i = right; i > left; i--) {
            if (arr[i] < arr[i - 1]) {
                swap(arr[i], arr[i - 1]);
                control = i;
            }
        }
        left = control;
    } while (left < right);

    return;
}

//���������� ��� ������� L � M � arr
void merge(vector<Product>& arr, int p, int q, int r) {
    //������� ������� L = A[p..q] � M = A[q+1..r]
    int n1 = q - p + 1;
    int n2 = r - q;

    Product* L = new Product[n1];
    Product* M = new Product[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    //��������� 3 �������� �������� ��������
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    while (i < n1 && j < n2) {
        if (L[i] <= M[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = M[j];
        j++;
        k++;
    }
}

//���������� ��������
void mergeSort(vector<Product>& arr, int l, int r) {
    if (l < r) {
        // m - ��� ����� ���������� ��� ����������
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        //���������� ��������������� ����������
        merge(arr, l, m, r);
    }
}

void writeOutput(string title, vector<vector<Product>>& res) {
    ofstream fout(outputFile, ios::app);

    fout << title << "\n";

    for (int i = 0; i < batchNum; ++i) {
        //����� ����� �������
        fout << "������: " << res[i].size() << "\n";
        for (int j = 0; j < res[i].size(); ++j) {
            //����� �������
            fout << res[i][j] << "\n";
        }
    }
}

void writeTime(string title, vector<std::chrono::steady_clock::time_point> time) {
    foutTime << title << "\n";

    for (int i = 0; i < time.size() - 1; ++i)
        foutTime << "�����, ����������� ��� ���������� " << dims[i] << " ��������: " << chrono::duration_cast<chrono::milliseconds>(time[i + 1] - time[i]).count() << " [ms]\n";

    foutTime << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    makeTxtFile();

    vector<vector<Product>> initialArray = readTxtFile();

    vector<vector<Product>> bubbleSortArray = initialArray,
        mergeSortArray = initialArray, shakerSortArray = initialArray;

    vector<std::chrono::steady_clock::time_point> time;

    time.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < batchNum; ++i) {
        bubbleSort(bubbleSortArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }

    writeOutput("���������� ���������:\n", bubbleSortArray);
    writeTime("���������� ���������:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < batchNum; ++i) {
        mergeSort(mergeSortArray[i], 0, mergeSortArray[i].size() - 1);
        time.push_back(std::chrono::steady_clock::now());
    }

    writeOutput("���������� ��������:\n", mergeSortArray);
    writeTime("���������� ��������:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < batchNum; ++i) {
        shakerSort(shakerSortArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }

    writeOutput("���������� ��������:\n", shakerSortArray);
    writeTime("���������� ��������:\n", time);
    time.clear();

    return 0;
}
