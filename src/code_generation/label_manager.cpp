#include "code_generation/label_manager.h"
#include <fmt/format.h>
#include <stdexcept>

using namespace compiler;

label_manager_t *label_manager_t::instance = nullptr;
uint label_manager_t::unique_number = 0;

std::string label_manager_t::newlabel() {
  if (!label_manager_t::instance) {
    label_manager_t::instance = new label_manager_t();
  }

  if (label_manager_t::unique_number + 1 == 0) {
    throw std::runtime_error("label manager has generated to many lables so "
                             "there is no new unique_id.");
  };

  return fmt::format("t{}", ++label_manager_t::unique_number);
}
label_manager_t::label_manager_t() { this->instance = this; }
label_manager_t::~label_manager_t() { delete this->instance; }
