#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <cstdlib>

#define getMin(a,b) ((a)>(b)?(b):(a))
#define getMax(a,b) ((a)<(b)?(b):(a))

#define MaxToleranceCapacity 100

using namespace std;

struct Student {
    int grade, gender;
    int storey, plug, capacity;
    int stuEffect;
    int preferA, preferB, preferC; // preference of different parts of buildings like A, B, and C.
    int generic;
    double satisfactory;
    string classroom;
    string inClassroom;
    bool isPlugged;
};

struct Res {
    int id;
    double satisfactory;
    int freeCapacity;
    double val;
};

struct Classroom {
    string id;
    int storey, plug, capacity, part;
    int currentC, currentP;
};

bool cmpBests(struct Res a, struct Res b) {
    return a.val > b.val;
}

vector<Student> students;
vector<Classroom> classrooms;

ofstream fcout("/Users/haotian/Desktop/PJ/res.txt");

int notFoundCnt = 0;
int foundCnt = 0;
double SFTotal = 0;
int freeCapacity;
double average;

void init();

void make();

void readData();

void preProcess();

void process();

void dumpData();


int main() {
    init();
    make();
    return 0;
}

void init() {
    srand((unsigned int) time(NULL));
}

void readData() {
    Student student;
    Classroom classroom;
    ifstream finc("/Users/haotian/Desktop/PJ/classroom.in"), fins("/Users/haotian/Desktop/PJ/student.in");

    // check stream
    if (finc.fail()) {
        cout << "finc fail." << endl;
        exit(5);
    }
    if (fins.fail()) {
        cout << "fins fail." << endl;
        exit(5);
    }

    // read student data here.
    int t, feel;
    int t1, t2, t3;
    while (!fins.eof()) {
        fins >> student.grade >> student.gender >> student.storey >> student.plug;
        feel = 0;
        for (int i = 0; i < 5; i++) {
            fins >> t;
            if (t) feel++;
        }
        fins >> t;
        if (t) feel = 0;
        student.stuEffect = feel;

        fins >> t1 >> t2 >> t3;
        if (t1 == -2) t1 = 3;
        if (t2 == -2) t2 = 3;
        if (t3 == -2) t3 = 3;

        student.preferA = t1;
        student.preferB = t2;
        student.preferC = t3;

        fins >> student.generic;
        fins >> t; // jump too much choice.
        fins >> t; // capacity
        if (t < 0) t = MaxToleranceCapacity;

        student.capacity = t;

        fins >> student.classroom;
        students.push_back(student);
    }

    // read classroom data here.

    while (!finc.eof()) {
        finc >> classroom.id >> classroom.capacity >> classroom.plug;
        classroom.part = classroom.id[0] - 'A';
        classroom.storey = classroom.id[1] - '0';
        classroom.currentC = classroom.currentP = 0;
        classrooms.push_back(classroom);
    }
}

void preProcess() {
    ifstream fin("/Users/haotian/Desktop/PJ/process.out");
    vector<Student> shuffled = students;
    int randAdd;
    auto engine = std::default_random_engine{};

    if (fin.fail()) {
        cout << "Fin failed." << endl;
        exit(5);
    }

    fin >> randAdd;

    // map the students to desired numbers.

    std::shuffle(std::begin(shuffled), std::end(shuffled), engine);

    randAdd -= students.size();

    students.insert(students.end(), shuffled.begin(), shuffled.begin() + randAdd);

    // shuffle the student list

    std::shuffle(std::begin(students), std::end(students), engine);

    // shutdown classrooms

    char part;
    int storey, type;
    Classroom classroom;

    while (!fin.eof()) {
        fin >> part >> storey >> type;
        int cnt = 0;
        for (int i = 0; i < classrooms.size(); i++) {
            classroom = classrooms[i];
            // type (0,1,2) => (any, small, large)
            if (classroom.id[0] == part
                && (classroom.storey == storey || storey == -1)
                && (type == 0
                    || (type == 1 && classroom.capacity < 100)
                    || (type == 2 && classroom.capacity >= 100))) {
                cnt++;
                freeCapacity += classroom.capacity;
            } else if (cnt > 0) {
                classrooms.erase(classrooms.begin() + i - cnt, classrooms.begin() + i);
                cnt = 0;
            }
        }
        if (cnt > 0) {
            classrooms.erase(classrooms.end() - cnt, classrooms.end());
            cnt = 0;
        }
    }

}

void process() {
    Student student;
    Classroom classroom;

    double satisfactory, bestSF;
    int bestClassroom;
    int probability;

    for (int i = 0; i < students.size(); i++) {
        student = students[i];
        bestSF = 0;
        bestClassroom = -1;
        for (int j = 0; j < classrooms.size(); j++) {
            int stuPart;

            satisfactory = 1;
            classroom = classrooms[j];

            if (classroom.capacity <= classroom.currentC)
                continue;

            // check storage
            switch (student.storey) {
                case 1:
                    satisfactory *= 0.6 + classroom.storey / 10.0;
                    break;
                case 2:
                    satisfactory *= 1.0 - classroom.storey / 10.0;
                    break;
                case 3:
                    satisfactory *= 1.0 - (classroom.storey - 2) * (classroom.storey - 2) * 0.1;
                    break;
                default:
                    break;
            }

            // check plug
            switch (student.plug) {
                case 1:
                    if (classroom.plug == 0) satisfactory = 0;
                    break;
                case 2:
                    if (classroom.plug == 0) satisfactory *= 0.6;
                    break;
                case 3:
                    if (classroom.plug == 0) satisfactory *= 0.8;
                    break;
                default:
                    break;
            }

            // check capacity
            if (classroom.currentC > student.capacity) {
                satisfactory *= pow(0.8, student.stuEffect * getMax(0, classroom.currentC - student.capacity));
            }

            // check part of the classroom
            switch (classroom.part) {
                case 1:
                    stuPart = student.preferB;
                    break;
                case 2:
                    stuPart = student.preferC;
                    break;
                default:
                    stuPart = student.preferA;
                    break;
            }

            satisfactory *= 0.6 + (3 - stuPart) * 0.2;

            // update best
            if (satisfactory > bestSF) {
                bestClassroom = j;
                bestSF = satisfactory;
            }
        }

        // check if best found and manipulate data.
        if (bestClassroom != -1) {
            classroom = classrooms[bestClassroom];
            if (student.plug <= 3 && student.plug >= 1 && classroom.currentP < classroom.plug) {
                probability = rand() % 3 + 1;
                if (student.plug > probability) {
                    students[i].isPlugged = true;
                    classrooms[bestClassroom].currentP++;
                }
            }
            students[i].satisfactory = bestSF;
            students[i].inClassroom = classroom.id;
            classrooms[bestClassroom].currentC++;
            SFTotal += bestSF;
            foundCnt++;
        } else {
            notFoundCnt++;
        }
    }
}

void dumpData() {
    Student student;
    Classroom classroom;

    average = SFTotal / students.size();
    double stdDeviation = 0;

    for (int i = 0; i < students.size(); i++) {
        student = students[i];
        stdDeviation += pow(average - student.satisfactory, 2);
    }

    stdDeviation /= students.size();
    stdDeviation = sqrt(stdDeviation);


    fcout << "Free capacity: " << freeCapacity << endl;

    fcout << "Found: " << foundCnt << "(" << double(foundCnt / students.size() * 100) << " %)" << endl;
    fcout << "Estimated Satisfactory: " << average << endl;
    fcout << "Standard deviation: " << stdDeviation << endl;

    fcout << "Failed: " << notFoundCnt << "(" << double(notFoundCnt / students.size() * 100) << " %)" << endl;
}

void make() {
    readData();

    vector<Student> studentsBak = students;
    vector<Classroom> classroomsBak = classrooms;

    vector<string> reqList;

    for (char building = 'A'; building <= 'C'; building++) {
        for (int storey = -1; storey <= 4; storey++) {
            if (building == 'B' && storey == 0)
                continue;
            if (building == 'C' && (storey == 0 || storey == 4))
                continue;
            for (int type = 0; type <= 0; type++) {
                reqList.push_back(string(1, building) + " " + to_string(storey) + " " + to_string(type));
            }
        }
    }

    int size = reqList.size();
    long long shifts = (long long)1 << size;
    ofstream td;
    ifstream fin("/Users/haotian/Desktop/PJ/process.out");
    int randAdd;

    vector<struct Res> bests;
    struct Res stat;


    bool fa, fb, fc;
    vector<int> record;
    vector<vector<int>> records;

    fin >> randAdd;

    for (int i = 0; i < shifts; i++) {
        students = studentsBak;
        classrooms = classroomsBak;
        int t = i;
        int p = 0;

        // initialize global vars.
        notFoundCnt = 0;
        foundCnt = 0;
        SFTotal = 0;
        freeCapacity = 0;

        fa = fb = fc = false;
        record.clear();

        td.open("/Users/haotian/Desktop/PJ/process.out");
        td << randAdd << endl;
        while (t > 0) {
            if (t % 2) {
                switch (reqList[p][0]) {
                    case 'A':
                        if (fa) goto skip;
                        else if (reqList[p][2] == '-') fa = true;
                        break;
                    case 'B':
                        if (fb) goto skip;
                        else if (reqList[p][2] == '-') fb = true;
                        break;
                    case 'C':
                        if (fc) goto skip;
                        else if (reqList[p][2] == '-') fc = true;
                        break;
                    default:
                        break;
                }
                td << reqList[p] << endl;
                record.push_back(p);
                //fcout << reqList[p] << endl;
            }

            skip:
            t >>= 1;
            p++;
        }

        sort(record.begin(), record.end());
        if (std::find(records.begin(), records.end(), record) == records.end()) {
            records.push_back(record);
            fcout << "=======================" << endl;
            fcout << "No. " << i << endl;
            for (int j = 0; j < record.size(); j++) {
                fcout << reqList[record[j]] << endl;
            }
            fcout << "-----------------------" << endl;
        } else {
            td.close();
            continue;
        }

        td.close();
        preProcess();
        process();
        dumpData();

        stat.id = i;
        stat.satisfactory = average;
        stat.freeCapacity = freeCapacity;
        stat.val = average * freeCapacity;
        bests.push_back(stat);
    }

    sort(bests.begin(), bests.end(), cmpBests);

    fcout << "===================" << endl;
    fcout << "Final Results (SORTED): " << endl;
    for (int i = 0; i < bests.size(); i++) {
        fcout << "Id: " << bests[i].id
              << " Res: " << bests[i].satisfactory
              << " Free: " << bests[i].freeCapacity
              << " Mark: " << bests[i].val
              << endl;
    }

}
