#include "varstore.h"
#include "list.h"
#include "evaluator.h"

void args_unification(list f_args, list call_args, varstore& global, varstore& local) {
  
  if (f_args.iscons() && call_args.isnil())
    throw FunctionApplicationError(std::string("Insufficient arguments!"));

  if (f_args.isnil() && call_args.iscons())
    throw FunctionApplicationError(std::string("Too many arguments!"));

  if (f_args.iscons() && call_args.iscons()) {

    list fst_f = f_args.getfirst();
    list fst_call = call_args.getfirst();

    local.init(fst_f.getstring(), eval(fst_call, global, local));

    list rest_f = f_args.getrest();
    list rest_call = call_args.getrest();

    args_unification(rest_f, rest_call, global, local);
  }

}

list eval(list E, varstore& global, varstore& local) {

  if (!E.iscons()) {

    if (E.isstring()) {

      list* e = local.lookup(E.getstring());

      if (e != 0) {
        return (*e);
      }

      e = global.lookup(E.getstring());
      if (e != 0) {
        return (*e);
      }

      if (E.getstring() == "quit") {
        return list("EOF");
      }

      if (E.isnil()) return E;

      else throw NameError(std::string("Undefined variable: ") + E.getstring() + "!");

    }

    if (E.isbigint()) return E;

  } else {

    if (E.getfirst().isbigint()) return E;

      if (E.getfirst().getstring() == "if") {

        list e = E.getrest();
        list conditional = eval(e.getfirst(), global, local);

        if (conditional.istrue()) {
          e = e.getrest().getfirst();
          return eval(e, global, local);
        }

        e = e.getrest().getrest().getfirst();
        return eval(e, global, local);
      }

      if (E.getfirst().getstring() == "set") {

        list e = E.getrest();
        if (!e.getfirst().isstring()) 
          throw NameError(std::string("Identifier name must be alphanumeric!"));

        list ev = eval(e.getrest().getfirst(), global, local);
        global.init(e.getfirst().getstring(), ev);
        return ev;
      }

      if (E.getfirst().getstring() == "setq") {
        list e = E.getrest();

        if (!e.getfirst().isstring()) 
          throw NameError(std::string("Identifier name must be alphanumeric!"));

        global.init(e.getfirst().getstring(), e.getrest());

        return list("nil");
      }

      if (E.getfirst().getstring() == "define") {
        list f_name = E.getrest().getfirst();
        list args = E.getrest().getrest().getfirst();
        list body = E.getrest().getrest().getrest().getfirst();

        list lambda = list(list("lambda"), list(args, body));

        global.init(f_name.getstring(), lambda);

        return list("nil");
      }

      if (E.getfirst().getstring() == "plus") {

        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if (!fst.isbigint() || !snd.isbigint()) 
          throw TypeError(std::string("Values must be numerical!"));

        return list(fst.getbigint() + snd.getbigint());
      }

      if (E.getfirst().getstring() == "minus") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if (!fst.isbigint() || !snd.isbigint()) 
          throw TypeError(std::string("Values must be numerical!"));

        return list(fst.getbigint() - snd.getbigint());
      }

      if (E.getfirst().getstring() == "times") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);
        if (!fst.isbigint() || !snd.isbigint()) 
          throw TypeError(std::string("Values must be numerical!"));

        return list(fst.getbigint() * snd.getbigint());
      }

      if (E.getfirst().getstring() == "divides") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if (!fst.isbigint() || !snd.isbigint()) 
          throw TypeError(std::string("Values must be numerical!"));

        if (snd.getbigint() == 0)
          throw ZeroDivisionError(std::string("Cannot divide by zero!"));

        return list(fst.getbigint() / snd.getbigint());
      }

      if (E.getfirst().getstring() == "head") {
        list fst = eval(E.getrest().getfirst(), global, local);
        if (fst.iscons()) {
          return fst.getfirst();
        } else
          throw TypeError("Not a constructor");
      }

      if (E.getfirst().getstring() == "concat") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);
        return list(fst, snd);
      }

      if (E.getfirst().getstring() == "tail") {

        list fst = eval(E.getrest().getfirst(), global, local);
        if (fst.iscons()) {
          return fst.getrest();
        } else
          throw TypeError("Not a constructor");
      }

      if (E.getfirst().getstring() == "equals") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if ((!snd.isbigint() && fst.isbigint()) || (!snd.isstring() && fst.isstring()))
          return list("false");

        if (fst.isbigint() && fst.getbigint() == snd.getbigint())
          return list("true");

        return list("false");
      }

      if (E.getfirst().getstring() == "gt") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if ((!snd.isbigint() && fst.isbigint()) || (!snd.isstring() && fst.isstring()))
          return list("false");

        if (fst.isbigint() && fst.getbigint() > snd.getbigint())
          return list("true");

        return list("false");
      }

      if (E.getfirst().getstring() == "lt") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if ((!snd.isbigint() && fst.isbigint()) || (!snd.isstring() && fst.isstring()))
          return list("false");

        if (fst.isbigint() && fst.getbigint() < snd.getbigint())
          return list("true");

        return list("false");
      }

      if (E.getfirst().getstring() == "geq") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if ((!snd.isbigint() && fst.isbigint()) || (!snd.isstring() && fst.isstring()))
          return list("false");

        if (fst.isbigint() && fst.getbigint() >= snd.getbigint())
          return list("true");

        return list("false");
      }

      if (E.getfirst().getstring() == "leq") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if ((!snd.isbigint() && fst.isbigint()) || (!snd.isstring() && fst.isstring()))
          return list("false");

        if (fst.isbigint() && fst.getbigint() <= snd.getbigint())
          return list("true");

        return list("false");
      }

      if (E.getfirst().getstring() == "notequals") {
        list fst = eval(E.getrest().getfirst(), global, local);
        list snd = eval(E.getrest().getrest().getfirst(), global, local);

        if ((!snd.isbigint() && fst.isbigint()) || (!snd.isstring() && fst.isstring()))
          return list("true");

        if (fst.isbigint() && fst.getbigint() == snd.getbigint())
          return list("false");

        return list("true");
      }

      if (E.getfirst().getstring() == "and") {
          list fst = eval(E.getrest().getfirst(), global, local);
          list snd = E.getrest().getrest().getfirst();
          if(!fst.istrue()) return fst;
          return eval(snd, global, local);
      }

      if (E.getfirst().getstring() == "or") {
          list fst = eval(E.getrest().getfirst(), global, local);
          list snd = E.getrest().getrest().getfirst();
          if(fst.istrue()) return fst;
          if (snd.isnil()) return list("false");
          return eval(snd, global, local);
      }

      if (E.getrest().isnil()) {
        return eval(E.getfirst(), global, local);
      }

      varstore::backtrackpoint p = local.getbacktrackpoint();
      list f_name = E.getfirst();

      list* f = global.lookup(f_name.getstring());

      if (f == 0)
        throw NameError("Undefined function: " + f_name.getstring());

      if (!(*f).islambda())
        throw TypeError(std::string(f_name.getstring()) + " is not a function!");

      list f_args = (*f).getrest().getfirst();
      list f_body = (*f).getrest().getrest();

      args_unification(f_args, E.getrest(), global, local);

      list ev = eval(f_body, global, local);

      return ev;
  }
  return list("nil");
}