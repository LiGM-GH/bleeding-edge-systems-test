#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <thread>

#include <unordered_set>
#include <zmq.h>
#include <zmq.hpp>

#define STUDLINE_MAX_LENGTH 100

class Student {
  std::string name = std::string{};
  std::string surname = std::string{};

  int birth_date_day = 0;
  int birth_date_month = 0;
  int birth_date_year = 0;

public:
  static std::optional<Student> parse(std::string_view line) {
    std::cerr << "Parsing: '" << line << "'\n";
    Student self;
    std::string self_name;
    std::string self_surname;

    self_name.reserve(20);
    self_surname.reserve(20);
    int i = 0;
    int i_prev = 0;

    auto dbg = [](auto value) {
      std::cout << "DEBUG: substring is '" << value << "'\n";
      return value;
    };

    // SKIP THE FIRST ELEMENT START ----------------------------------------- //
    for (; i < STUDLINE_MAX_LENGTH && line[i] != 0 && line[i] != ' '; i++) { //
    } //
    if (line[i] == 0) {            //
      std::cerr << "Line ended\n"; //
      return std::nullopt;         //
    } //
    i_prev = ++i; //
    // SKIP THE FIRST ELEMENT END ------------------------------------------- //

    for (; i < STUDLINE_MAX_LENGTH && line[i] != 0 && line[i] != ' '; i++) {
    }

    if (line[i] == 0) {
      std::cerr << "Line ended\n";
      return std::nullopt;
    }

    self.name = dbg(line.substr(i_prev, i - i_prev));

    i_prev = ++i;

    for (; i < STUDLINE_MAX_LENGTH && line[i] != 0 && line[i] != ' '; i++) {
    }

    if (line[i] == 0) {
      std::cerr << "Line ended\n";
      return std::nullopt;
    }

    self.surname = dbg(line.substr(i_prev, i - i_prev));
    i_prev = ++i;

    for (; i < STUDLINE_MAX_LENGTH && line[i] != 0 && line[i] != '.'; i++) {
    }

    if (line[i] == 0) {
      std::cerr << "Line ended\n";
      return std::nullopt;
    }

    try {
      self.birth_date_day =
          std::stoi(dbg(std::string(line.substr(i_prev, i - i_prev))));
    } catch (...) {
      std::cerr << "Error while stoi\n";
      return std::nullopt;
    }
    i_prev = ++i;

    for (; i < STUDLINE_MAX_LENGTH && line[i] != 0 && line[i] != '.'; i++) {
    }

    if (line[i] == 0) {
      std::cerr << "Line ended\n";
      return std::nullopt;
    }

    try {
      self.birth_date_month =
          std::stoi(dbg(std::string(line.substr(i_prev, i - i_prev))));
    } catch (...) {
      std::cerr << "Error while stoi\n";
      return std::nullopt;
    }
    i_prev = ++i;

    for (; i < STUDLINE_MAX_LENGTH && line[i] != 0 && line[i] != '.'; i++) {
    }

    try {
      self.birth_date_year =
          std::stoi(dbg(std::string(line.substr(i_prev, i - i_prev))));
    } catch (...) {
      std::cerr << "Error while stoi\n";
      return std::nullopt;
    }

    // 1  Январь   - 31 день.
    // 2  Февраль  - 28 или 29 дней (в високосном году - 29).
    // 3  Март     - 31 день.
    // 4  Апрель   - 30 дней.
    // 5  Май      - 31 день.
    // 6  Июнь     - 30 дней.
    // 7  Июль     - 31 день.
    // 8  Август   - 31 день.
    // 9  Сентябрь - 30 дней.
    // 10 Октябрь  - 31 день.
    // 11 Ноябрь   - 30 дней.
    // 12 Декабрь  - 31 день.

    if (self.birth_date_month > 12 && self.birth_date_month < 1) {
      std::cerr << "Invalid month" << self.birth_date_month << "\n";
      return std::nullopt;
    }
    if (self.birth_date_month == 1 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "January && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 2 &&
        (self.birth_date_day > 29 || self.birth_date_day < 1)) {
      std::cerr << "February && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 3 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "March && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 4 &&
        (self.birth_date_day > 30 || self.birth_date_day < 1)) {
      std::cerr << "April && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 5 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "May && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 6 &&
        (self.birth_date_day > 30 || self.birth_date_day < 1)) {
      std::cerr << "June && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 7 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "July && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 8 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "August && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 9 &&
        (self.birth_date_day > 30 || self.birth_date_day < 1)) {
      std::cerr << "September && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 10 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "October && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 11 &&
        (self.birth_date_day > 30 || self.birth_date_day < 1)) {
      std::cerr << "November && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    if (self.birth_date_month == 12 &&
        (self.birth_date_day > 31 || self.birth_date_day < 1)) {
      std::cerr << "December && day is " << self.birth_date_day << "\n";
      return std::nullopt;
    }

    return self;
  }

  bool operator==(Student rhs) const {
    return this->birth_date_year == rhs.birth_date_year &&
           this->birth_date_month == rhs.birth_date_month &&
           this->birth_date_day == rhs.birth_date_day &&
           this->name == rhs.name && this->surname == rhs.surname;
  }

  bool operator<(Student rhs) const {
    if (this->name > rhs.name) {
      return false;
    }

    if (this->name < rhs.name) {
      return true;
    }

    if (this->surname > rhs.surname) {
      return false;
    }

    if (this->surname < rhs.surname) {
      return true;
    }

    if (this->birth_date_year > rhs.birth_date_year) {
      return false;
    }

    if (this->birth_date_year < rhs.birth_date_year) {
      return true;
    }

    if (this->birth_date_month > rhs.birth_date_month) {
      return false;
    }

    if (this->birth_date_month < rhs.birth_date_month) {
      return true;
    }

    if (this->birth_date_day > rhs.birth_date_day) {
      return false;
    }

    if (this->birth_date_day < rhs.birth_date_day) {
      return true;
    }

    return true;
  }

  struct HashFunction {
    size_t operator()(const Student &stud) const {
      size_t year_hash = std::hash<int>()(stud.birth_date_year);
      size_t month_hash = std::hash<int>()(stud.birth_date_month);
      size_t day_hash = std::hash<int>()(stud.birth_date_day);

      std::hash<std::string> name_hasher;
      size_t name_hash = name_hasher(stud.name);

      std::hash<std::string> surname_hasher;
      size_t surname_hash = surname_hasher(stud.surname);

      return year_hash ^ month_hash ^ day_hash ^ name_hash ^ surname_hash;
    }
  };

  std::string to_string() const {
    return this->name + " " + this->surname + " " +
           std::to_string(this->birth_date_day) + "." +
           std::to_string(this->birth_date_month) + "." +
           std::to_string(this->birth_date_year);
  }
};

int main() {
  using namespace std::chrono_literals;

  std::string_view fname1 = "student_file1.txt";
  std::string_view fname2 = "student_file2.txt";

  std::ifstream if1(fname1.data());
  if (!if1.is_open()) {
    std::cerr << "Couldn't open " << fname1 << "\n";
    return -1;
  }
  std::ifstream if2(fname2.data());
  if (!if2.is_open()) {
    std::cerr << "Couldn't open " << fname2 << "\n";
    return -1;
  }

  std::unordered_set<Student, Student::HashFunction> students;

  char stud_line[STUDLINE_MAX_LENGTH] = {0};

  while (!if1.eof() && if1.is_open()) {
    std::cout << "Reading line from if1\n";
    if1.getline(stud_line, STUDLINE_MAX_LENGTH);
    std::string_view line_wrapper = stud_line;
    std::optional<Student> student = Student::parse(line_wrapper);

    if (!student) {
      continue;
    }

    students.insert(student.value());
  }

  while (!if2.eof() && if2.is_open()) {
    std::cout << "Reading line from if2\n";
    if2.getline(stud_line, STUDLINE_MAX_LENGTH);
    std::string_view line_wrapper = stud_line;
    std::optional<Student> student = Student::parse(line_wrapper);

    if (!student) {
      continue;
    }

    students.insert(student.value());
  }

  // initialize the zmq context with a single IO thread
  zmq::context_t context{1};

  // construct a REP (reply) socket and bind to interface
  zmq::socket_t socket{context, zmq::socket_type::rep};
  socket.bind("tcp://127.0.0.1:5152");

  // prepare some static data for responses
  std::string data;

  std::cout << "Started iterating over students\n";
  for (auto stud : students) {
    std::cout << "Unique student " << stud.to_string() << "\n";
    data.append(stud.to_string() + "\n");
  }
  std::cout << "Ended iterating over students\n";

  for (;;) {
    zmq::message_t request;

    // receive a request from client
    socket.recv(request, zmq::recv_flags::none);
    std::cout << "Received " << request.to_string() << std::endl;

    // send the reply to the client
    socket.send(zmq::buffer(data), zmq::send_flags::none);
  }

  return 0;
}
