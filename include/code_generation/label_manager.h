#ifndef label_MANAGER_H
#define label_MANAGER_H

#include <string>

namespace compiler {

/** This class is a singleton because labels can't be assignt multiple times, so
 * only one label manager should have all the data */
class label_manager_t {
public:
  static std::string newlabel();

private:
  label_manager_t();
  ~label_manager_t();

private:
  static label_manager_t *instance;
  static uint unique_number;
};

} // namespace compiler

#endif // !label_MANAGER_H
