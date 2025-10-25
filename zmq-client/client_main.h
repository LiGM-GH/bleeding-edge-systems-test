#ifndef CLIENT_MAIN_H
#define CLIENT_MAIN_H

#include <optional>
#define CLIENT_MAIN_H
#include <set>
#include <string>
#include <zmq.hpp>

#define STUDLINE_MAX_LENGTH 100

class Student {
public:
  std::string name = std::string{};
  std::string surname = std::string{};

  int birth_date_day = 0;
  int birth_date_month = 0;
  int birth_date_year = 0;

  static std::optional<Student> parse(std::string_view line);

  bool operator==(Student rhs) const;

  bool operator<(Student rhs) const;

  struct HashFunction {
    size_t operator()(const Student &stud) const;
  };

  std::string to_string() const;
};

std::set<std::optional<Student>> client_main();
#endif // CLIENT_MAIN_H
