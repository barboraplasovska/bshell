#ifndef CH_H
#define CH_H

#include "../builtin/builtin.h"

/**
** @brief               Builtin echo options.
** @param nflag         Option -i is activated.
** @param ind           Index of the first argument (Options excluded).
*/
typedef struct          Options
{
  bool                  iflag; // interactive (asks if sure to delete)
  bool                  fflag; // force delete
  bool                  rflag; // or -R recursive delet (for dir)
  ssize_t                ind;
} Options;

#endif

