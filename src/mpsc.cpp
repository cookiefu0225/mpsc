#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int max(int num1, int num2) {
    if (num1 >= num2) {
        return num1;
    }
    else {
        return num2;
    }
}

class Data {
    public:
        int N;                  //number of chords
        int* chords;            //record the points that are bonded together 
        Data();                 //constructor
        Data(int num);          //constructor with number of chords N
};

class Solution {
    private:
        int** m;
        int** find_case;
        int** passed;
        int arraysize;      
    public:
        Solution();             
        Solution(Data& data);   //constructor 
        int MPSC(Data& data, int start, int end);
                                //the number of MPSC
        void find_chords(Data& data, int start, int end, char* filename);
};


Data::Data() {
    N = 0;
}

Data::Data(int num) {
    N = num;
    chords = new int[2*N];
}


Solution::Solution() {}

Solution::Solution(Data& data) {
    arraysize = 2 * data.N;
    m = new int*[arraysize];
    find_case = new int*[arraysize];
    passed = new int*[arraysize];
    for (int i = 0; i < arraysize; i++) {
        m[i] = new int[arraysize];
        find_case[i] = new int[arraysize];
        passed[i] = new int[arraysize];
    }
}



//if kj is out of ij, m(i, j) = m(i, j - 1)
//if kj is in ij, m(i, j) = max( m(k + 1, j - 1) + 1 + m(i, k - 1) , m(i, j - 1) )
//if kj = ij, m(i, j) = 1 + m(i + 1, j - 1)
//if i <= j, return 0
int Solution::MPSC(Data& data, int start, int end) {
    if (start >= end) {
        return 0;
    }
    
    if (passed[start][end] == 1) {
        return m[start][end];
    }
    else {
        int EDpoint = data.chords[end];
        if (EDpoint == start) {
            m[start][end] = MPSC(data, start + 1, end - 1) + 1;
            find_case[start][end] = 1;
            passed[start][end] = 1;
        }
        else if (EDpoint > start && EDpoint < end) {
            m[start][end] = max(MPSC(data, start, end - 1),
                            MPSC(data, start, EDpoint - 1) + 1 + MPSC(data, EDpoint + 1, end - 1));
            if (m[start][end] == MPSC(data, start, EDpoint - 1) + 1 + MPSC(data, EDpoint + 1, end - 1)){
                find_case[start][end] = 2;
                passed[start][end] = 1;
            }
            else {
                find_case[start][end] = 3;
                passed[start][end] = 1;
            }
        }
        else {
            m[start][end] = MPSC(data, start, end - 1);
            find_case[start][end] = 3;
            passed[start][end] = 1;
        }
    }


    return m[start][end];
    
    

    //end
}

void Solution::find_chords(Data& data, int start, int end, char* filename) {
    //cout << start << " " << end << endl;
    fstream fout;
    if (find_case[start][end] == 1) {
        fout.open(filename, ios::app);
        fout << start << " " << end << endl;
        find_chords(data, start + 1, end - 1, filename);
    }
    else if (find_case[start][end] == 2) {
        int k = data.chords[end];
        find_chords(data, start, k - 1, filename);
        fout.open(filename, ios::app);
        fout << k << " " << end << endl;
        find_chords(data, k + 1, end - 1, filename);
    }
    else if (find_case[start][end] == 3) {
        find_chords(data, start, end - 1, filename);
    }
    else {
        return;
    }
}


int main(int argc, char* argv[]) {
    fstream fin(argv[1]);
    int two_N;
    fin >> two_N;
    Data circle(two_N / 2);

    int num1, num2;
    while (fin >> num1 >> num2) {
        circle.chords[num1] = num2;
        circle.chords[num2] = num1;
    }

    char* outputfile_name = argv[2];

    fstream fout;
    fout.open(outputfile_name, ios::out);

    Solution s(circle);
    int a = s.MPSC(circle, 0, two_N - 1);
    fout << a << endl;
    fout.close();
    s.find_chords(circle, 0, two_N - 1, outputfile_name);

    return 0;
}