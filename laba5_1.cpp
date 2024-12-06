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

struct StudentGroup {
    int groupNumber;
    int count;
    int badCount;

    StudentGroup() : groupNumber(0), count(0), badCount(0) {}
};

bool compareByGroup(const STUDENT& a, const STUDENT& b) {
    return a.groupNumber < b.groupNumber;
}

bool compareByAverage(const STUDENT& a, const STUDENT& b) {
    return a.average() > b.average();
}

void printTableHeaderS() {
    string g = "Группа";
    string s = "Количество студентов";
    string d = "Количество студентов без двоек";

    cout << string(72, '-') << endl;
    cout << "|" << setw(8) << g << setw(3) << "|" << setw(22) << s << setw(3) << "|" << setw(32) << d << setw(3) << "|" << endl;
    cout << string(72, '-') << endl;
}

void bubbleSortByGroup(STUDENT* students, int N) {
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N - i - 1; ++j) {
            if (students[j].groupNumber > students[j + 1].groupNumber) {
                swap(students[j], students[j + 1]);
            }
        }
    }
}

void bubbleSortByAverage(STUDENT* students, int count) {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (students[j].average() < students[j + 1].average()) {
                swap(students[j], students[j + 1]);
            }
        }
    }
}

void bubbleSortIndices(int* indices, int groupCount, StudentGroup* groups) {
    for (int i = 0; i < groupCount - 1; ++i) {
        for (int j = 0; j < groupCount - i - 1; ++j) {
            if (groups[indices[j]].badCount < groups[indices[j + 1]].badCount) {
                swap(indices[j], indices[j + 1]);
            }
        }
    }
}

int linearSearch(StudentGroup* arr, int size, int value) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].groupNumber == value) {
            return i;
        }
    }
    return -1;
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

    bubbleSortByGroup(students, N);

    int filteredCount = 0;
    STUDENT* filteredStudents = new STUDENT[N];

    for (int i = 0; i < N; ++i) {
        if (students[i].average() > 4.0f) {
            filteredStudents[filteredCount++] = students[i];
        }
    }

    bubbleSortByAverage(filteredStudents, filteredCount);

    StudentGroup* groups = new StudentGroup[N];
    int groupCount = 0;

    for (int i = 0; i < N; ++i) {
        int index = linearSearch(groups, groupCount, students[i].groupNumber);
        if (index != -1) {
            groups[index].count++;
            if (students[i].hasFailingGrade()) {
                groups[index].badCount++;
            }
        }
        else {
            groups[groupCount].groupNumber = students[i].groupNumber;
            groups[groupCount].count = 1;
            groups[groupCount].badCount = students[i].hasFailingGrade() ? 1 : 0;
            groupCount++;
        }
    }

    int* indices = new int[groupCount];
    for (int i = 0; i < groupCount; i++) {
        indices[i] = i;
    }
    bubbleSortIndices(indices, groupCount, groups);

    if (isHuman) {
        cout << "Полный список студентов по возрастанию номера группы:" << endl;
        for (int i = 0; i < N; ++i) {
            cout << students[i].groupNumber << " - " << students[i].fullName << ": " << students[i].grades[0] << ", " << students[i].grades[1] << ", "
                << students[i].grades[2] << ", " << students[i].grades[3] << ", "
                << students[i].grades[4] << endl;
        }
    }
    else {
        for (int i = 0; i < N; ++i) {
            cout << students[i].groupNumber << " - " << students[i].fullName << ": " << students[i].grades[0] << ", " << students[i].grades[1] << ", "
                << students[i].grades[2] << ", " << students[i].grades[3] << ", "
                << students[i].grades[4] << endl;
        }
    }

    if (filteredCount == 0) {
        cout << "NO" << endl;
    }
    else {
        if (isHuman) {
            cout << "Студенты со средним баллом выше 4.0, упорядоченные по убыванию среднего балла:" << endl;
        }

        for (int i = 0; i < filteredCount; ++i) {
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
                cout << filteredStudents[i].groupNumber << ", "
                    << filteredStudents[i].fullName << " - "
                    << avgStr << endl;
            }
            else {
                cout << filteredStudents[i].groupNumber << ", "
                    << filteredStudents[i].fullName << " - "
                    << avgStr << endl;
            }
        }
    }

    if (isHuman) {
        printTableHeaderS();
    }

    for (int i = 0; i < groupCount; ++i) {
        if (isHuman) {
            cout << "|" << setw(7) << groups[indices[i]].groupNumber << setw(4) << "|"
                << setw(13) << groups[indices[i]].count << setw(12) << "|"
                << setw(18) << groups[indices[i]].badCount << setw(17) << "|" << endl;
        }
        else {
            cout << groups[indices[i]].groupNumber << " - "
                << groups[indices[i]].count << " - "
                << groups[indices[i]].badCount << endl;
        }
    }

    if (isHuman) {
        cout << string(72, '-') << endl;
    }

    delete[] students;
    delete[] filteredStudents;
    delete[] groups;
    delete[] indices;

    return 0;
}