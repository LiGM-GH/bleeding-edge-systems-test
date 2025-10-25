#include "mainwindow.h"
#include "./client_main.h"
#include "./ui_mainwindow.h"
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  std::thread th = std::thread([this]() { this->run_client(); });
  this->th = std::move(th);
}

MainWindow::~MainWindow() {
  // SAFETY: this is unsafe, but let's pretend we have guarantees that the
  // client will stop at some point.
  this->th.join();
  delete ui;
}

void MainWindow::run_client() {
  std::optional<std::set<std::optional<Student>>> studs_set_opt = client_main();

  if (!studs_set_opt.has_value()) {
    std::cerr << "Exiting the thread_func\n";
    return;
  }
  std::cerr << "Proceeding with the result of non-emptiness: " << studs_set_opt.has_value() << "\n";

  std::set<std::optional<Student>> studs_set = studs_set_opt.value();
  std::string studs;
  int errors = 0;

  for (auto stud : studs_set) {
    if (stud) {
      studs.append(stud.value().to_string());
      studs.push_back('\n');
    } else {
      errors++;
    }
  }

  this->ui->label->setText(QString::fromStdString(studs.data()));
  this->ui->label_2->setText(QString::fromStdString(
      std::string("Number of errors: ") + std::to_string(errors)));
}
