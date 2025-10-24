#include <iostream>
#include <set>
#include <string>
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
    if (this->surname > rhs.surname) {
      return false;
    }

    if (this->surname < rhs.surname) {
      return true;
    }

    if (this->name > rhs.name) {
      return false;
    }

    if (this->name < rhs.name) {
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
  zmq::context_t context(1);
  zmq::socket_t socket(context, zmq::socket_type::req);

  std::cout << "Connecting to the server..." << std::endl;
  socket.connect("tcp://127.0.0.1:5152");

  zmq::message_t request(5);
  memcpy(request.data(), "Hello", 5);
  std::cout << "Sending request\n";
  socket.send(request, zmq::send_flags::none);

  zmq::message_t reply;
  socket.recv(reply, zmq::recv_flags::none);
  std::cout << "Received\n";

  std::string body = reply.to_string();
  std::set<Student> studs;

  while (body.length() != 0) {
    // split by line
    int pos = body.find('\n');
    auto stud = Student::parse(body.substr(0, pos));

    if (stud) {
      studs.insert(stud.value());
    }

    body = body.substr(pos + 1, body.length() - pos);
  }

  for (auto stud : studs) {
    std::cout << "Stud: " << stud.to_string() << "\n";
  }

  return 0;
}
