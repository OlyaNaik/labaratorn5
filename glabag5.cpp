#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <Windows.h>
#include <sstream>


using namespace std;

struct STUDENT {
    string fullName;
    int groupNumber;
    int grades[5];

    float average() const {
        return (grades[0] + grades[1] + grades[2] + grades[3] + grades[4]) / 5.0f;
    }

    bool hasFailingGrade() const {
        for (int grade : grades) {
            if (grade == 2) return true;
        }
        return false;
    }
};

bool compareByGroup(const STUDENT& a, const STUDENT& b) {
    return a.groupNumber < b.groupNumber;
}

bool compareByAverage(const STUDENT& a, const STUDENT& b) {
    return a.average() > b.average();
}

void printTableHeaderO() {

    string g = "Группа";
    string f = "ФИО";
    string o = "Оценки";

    cout << string(61, '-') << endl;
    cout << "|" << setw(8) << g << setw(3) << "|" << setw(16) << f << setw(15) << "|" << setw(12) << o << setw(6) << "|" << endl;
    cout << string(61, '-') << endl;
}

void printTableHeaderT() {

    string g = "Группа";
    string f = "ФИО";
    string c = "Средний балл";

    cout << string(61, '-') << endl;
    cout << "|" << setw(8) << g << setw(3) << "|" << setw(16) << f << setw(15) << "|" << setw(15) << c << setw(3) << "|" << endl;
    cout << string(61, '-') << endl;
}

void printTableHeaderS() {

    string g = "Группа";
    string s = "Количество студентов";
    string d = "Количество студентов без двоек";

    cout << string(72, '-') << endl;
    cout << "|" << setw(8) << g << setw(3) << "|" << setw(22) << s << setw(3) << "|" << setw(32) << d << setw(3) << "|" << endl;
    cout << string(72, '-') << endl;
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    bool isHuman = false;
    if (argc <= 1 || strcmp(argv[1], "false") != 0) {
        isHuman = true;
    }

    int N;
    if (isHuman) {
        cout << "Введите количество студентов: " << endl;
    }
    cin >> N;
    cin.ignore();

    STUDENT* students = new STUDENT[N];

    for (int i = 0; i < N; ++i) {
        if (isHuman) {
            cout << "Введите данные студента: " << endl;
        }
        getline(cin, students[i].fullName);
        cin >> students[i].groupNumber;
        for (int j = 0; j < 5; ++j) {
            cin >> students[i].grades[j];
        }
        cin.ignore();
    }

    sort(students, students + N, compareByGroup);

    int filteredCount = 0;
    STUDENT* filteredStudents = new STUDENT[N];

    for (int i = 0; i < N; ++i) {
        if (students[i].average() > 4.0f) {
            filteredStudents[filteredCount++] = students[i];
        }
    }

    sort(filteredStudents, filteredStudents + filteredCount, compareByAverage);

    int* groupNumbers = new int[N];
    int* counts = new int[N]();
    int* badCounts = new int[N]();
    int groupCount = 0;

    if (isHuman) {
        printTableHeaderO();
    }

    for (int i = 0; i < N; ++i) {
        auto it = find(groupNumbers, groupNumbers + groupCount, students[i].groupNumber);
        if (it != groupNumbers + groupCount) {
            int index = distance(groupNumbers, it);
            counts[index]++;
            if (students[i].hasFailingGrade()) {
                badCounts[index]++;
            }
        }
        else {
            groupNumbers[groupCount] = students[i].groupNumber;
            counts[groupCount] = 1;
            badCounts[groupCount] = students[i].hasFailingGrade() ? 1 : 0;
            groupCount++;
        }
    }

    int* indices = new int[groupCount];
    for (int i = 0; i < groupCount; i++) {
        indices[i] = i;
    }
    sort(indices, indices + groupCount, [&badCounts](int a, int b) {
        return badCounts[a] > badCounts[b];
        });

    if (isHuman) {
        for (int i = 0; i < N; ++i) {
            cout << "|" << setw(10) << students[i].groupNumber << "|"
                << setw(30) << students[i].fullName << "|"
                << setw(5) << students[i].grades[0] << ", " << students[i].grades[1] << ", "
                << students[i].grades[2] << ", " << students[i].grades[3] << ", "
                << students[i].grades[4] << "|" << endl;
        }
        cout << string(61, '-') << endl;
    }
    else {
        for (int i = 0; i < N; ++i) {
            cout << students[i].groupNumber << " - " << students[i].fullName << ": " << students[i].grades[0] << ", " << students[i].grades[1] << ", "
                << students[i].grades[2] << ", " << students[i].grades[3] << ", "
                << students[i].grades[4] << endl;
        }
    }

    if (isHuman) {
        printTableHeaderT();
    }
    if (filteredCount == 0) {
        cout << "NO" << endl;
    }
    else {
        for (int i = 0; i < filteredCount; ++i)
        {
            double avg = filteredStudents[i].average();

            ostringstream oss;
            oss << fixed << setprecision(2) << avg;
            string avgStr = oss.str();

            if (avgStr.find('.') != string::npos) {
                avgStr.erase(avgStr.find_last_not_of('0') + 1, string::npos);
                if (avgStr.back() == '.') {
                    avgStr.pop_back();
                }
            }
            if (isHuman) {
                cout << "|" << setw(10) << filteredStudents[i].groupNumber << "|"
                    << setw(30) << filteredStudents[i].fullName << "|"
                    << setw(17) << avgStr << "|" << endl;
            }
            else {
                cout << filteredStudents[i].groupNumber << ", "
                << filteredStudents[i].fullName << " - "
                << avgStr << endl;
            }
        }
        if (isHuman) {
        cout << string(61, '-') << endl;
        }
    }

    if (isHuman) {
        printTableHeaderS();
    }
    for (int i = 0; i < groupCount; ++i) {
        if (isHuman) {
            cout << "|" << setw(10) << groupNumbers[indices[i]] << "|" << setw(24) << counts[indices[i]] << "|" << setw(34) << badCounts[indices[i]] << "|" << endl;
        }
        else {
            cout << groupNumbers[indices[i]] << " - " << counts[indices[i]] << " - " << badCounts[indices[i]] << endl;
        }
    }
    if (isHuman) {
        cout << string(72, '-') << endl;
    }

    delete[] students;
    delete[] filteredStudents;
    delete[] groupNumbers;
    delete[] counts;
    delete[] badCounts;
    delete[] indices;

    return 0;
}