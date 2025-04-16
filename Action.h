#ifndef ACTION_H
#define ACTION_H

enum ActionType { INSERT, DELETE };

struct Action {
  ActionType type;
  char character;
  int position;

  Action() {}
  Action(ActionType t, char c, int p) : type(t), character(c), position(p) {}
};
#endif
