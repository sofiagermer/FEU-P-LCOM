#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"

char kbd_manager(uint8_t scanCode) {
  char letter;

  switch (scanCode) {
    case ENTER_BREAK:
      letter = '1';
      break;
    case ESC_BREAK:
      letter = '2';
      break;
    case A_BREAK:
      letter = 'a';
      break;
    case B_BREAK:
      letter = 'b';
      break;
    case C_BREAK:
      letter = 'c';
      break;
    case D_BREAK:
      letter = 'd';
      break;
    case E_BREAK:
      letter = 'e';
      break;
    case F_BREAK:
      letter = 'f';
      break;
    case G_BREAK:
      letter = 'g';
      break;
    case H_BREAK:
      letter = 'h';
      break;
    case I_BREAK:
      letter = 'i';
      break;
    case J_BREAK:
      letter = 'j';
      break;
    case K_BREAK:
      letter = 'k';
      break;
    case L_BREAK:
      letter = 'l';
      break;
    case M_BREAK:
      letter = 'm';
      break;
    case N_BREAK:
      letter = 'n';
      break;
    case O_BREAK:
      letter = 'o';
      break;
    case P_BREAK:
      letter = 'p';
      break;
    case Q_BREAK:
      letter = 'q';
      break;
    case R_BREAK:
      letter = 'r';
      break;
    case S_BREAK:
      letter = 's';
      break;
    case T_BREAK:
      letter = 't';
      break;
    case U_BREAK:
      letter = 'u';
      break;
    case V_BREAK:
      letter = 'v';
      break;
    case W_BREAK:
      letter = 'w';
      break;
    case X_BREAK:
      letter = 'x';
      break;
    case Y_BREAK:
      letter = 'y';
      break;
    case Z_BREAK:
      letter = 'z';
      break;
    default:
      letter = '.';
      break;
  }

  return letter;
}
