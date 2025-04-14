#ifndef ACTION_H
#define ACTION_H

enum ActionType { INSERT, DELETE };

struct Action {
  ActionType type;
  char character;
  int posisiton;
};

#endif
