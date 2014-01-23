struct TypeError {
  std::string message;
  TypeError (std::string  msg) : message(msg) {};
};

struct NameError {
  std::string message;
  NameError (std::string  msg) : message(msg) {};
};

struct ZeroDivisionError {
  std::string message;
  ZeroDivisionError (std::string  msg) : message(msg) {};
};

struct FunctionApplicationError {
  std::string message;
  FunctionApplicationError (std::string msg) : message(msg) {};
}; 

void args_unification(list f_args, list call_args, varstore& global, varstore& local);

list eval(list E, varstore& global, varstore& local);