// The author disclaims copyright to this source code.

//       case OP_R_PUSH:
//       {
//         ENSURE_STACK_MIN(1, END); ENSURE_R_STACK_MAX(1, END);
//         const cell n = POP();
//         R_PUSH(n);
//         END;
//       }
//
//       case OP_R_POP:
//       {
//         ENSURE_R_STACK_MIN(1, END); ENSURE_STACK_MAX(1, END);
//         const cell n = R_POP();
//         PUSH(n);
//         END;
//       }
//
//       case OP_R_FETCH:
//       {
//         ENSURE_R_STACK_MIN(1, END);
//         ENSURE_STACK_MAX(1, END);
//         PUSH(R_CELLS[R_SP]);
//         END;
//       }
//
// #ifdef __USE_REGISTER
//       define_register(A);
//       define_register(B);
//       define_register(C);
//       define_register(I);
//       define_register(J);
// #endif
//
//       case OP_DUP:
//       {
//         ENSURE_STACK_MIN(1, END);
//         ENSURE_STACK_MAX(1, END);
//         PUSH(CELLS[SP - 1]);
//         END;
//       }
//
//       case OP_DROP:
//       {
//         ENSURE_STACK_MIN(1, END);
//         SP -= 1;
//         END;
//       }
//
//       case OP_SWAP:
//       {
//         ENSURE_STACK_MIN(2, END);
//         const cell n = CELLS[SP];
//         CELLS[SP] = CELLS[SP - 1];
//         CELLS[SP - 1] = n;
//         END;
//       }
//
//       case OP_OVER:
//       {
//         ENSURE_STACK_MIN(2, END);
//         ENSURE_STACK_MAX(1, END);
//         PUSH(CELLS[SP - 2]);
//         END;
//       }
//
//       case OP_ROT:
//       {
//         ENSURE_STACK_MIN(3, END);
//         const cell n = CELLS[SP - 2];
//         CELLS[SP - 2] = CELLS[SP - 1];
//         CELLS[SP - 1] = CELLS[SP];
//         CELLS[SP] = n;
//         END;
//       }
//
//       case OP_MINUS_ROT:
//       {
//         ENSURE_STACK_MIN(3, END);
//         const cell n = CELLS[SP];
//         CELLS[SP] = CELLS[SP - 1];
//         CELLS[SP - 1] = CELLS[SP - 2];
//         CELLS[SP - 2] = n;
//         END;
//       }
//
//       case OP_NIP:
//       {
//         ENSURE_STACK_MIN(2, END);
//         SP -= 1;
//         CELLS[SP] = CELLS[SP + 1];
//         END;
//       }
//
//       case OP_LOAD:
//       {
//         ENSURE_STACK_MIN(1, END);
//         CELLS[SP] = *(cell*) CELLS[SP];
//         END;
//       }
//
//       case OP_STORE:
//       {
//         ENSURE_STACK_MIN(2, END);
//         cell *ptr = (cell*) CELLS[SP];
//         *ptr = CELLS[SP - 1];
//         SP -= 2;
//         END;
//       }
//
//       case OP_CLOAD:
//       {
//         ENSURE_STACK_MIN(1, END);
//         CELLS[SP] = *(char*) CELLS[SP];
//         END;
//       }
//
//       case OP_CSTORE:
//       {
//         ENSURE_STACK_MIN(2, END);
//         char *ptr = (char*) CELLS[SP];
//         *ptr = CELLS[SP - 1];
//         SP -= 2;
//         END;
//       }
//
case OP_CALL:
{
  pc += sizeof(opcode_t);
  ENSURE_STACK_MAX(1, END);
  R_PUSH(pc + sizeof(cell));
  pc = HEAP(pc, cell);
  continue;
}

case OP_TAIL_CALL:
{
  pc += sizeof(opcode_t);
  pc = HEAP(pc, cell);
  continue;
}

//       case OP_FUNCTION_CALL:
//       {
//         // Call extension function
//         ((void (*)(struct state *s)) pc->value)(s);
//         END;
//       }

case OP_NUMBER:
// case OP_TICK_NUMBER:
{
  ENSURE_STACK_MAX(1, END);
  SP += 1;
  pc += sizeof(opcode_t);
  CELLS[SP] = HEAP(pc, cell);
  pc += sizeof(cell);
  continue;
}

case OP_ADD:
{
  ENSURE_STACK_MIN(2, END);
  SP -= 1;
  CELLS[SP] = CELLS[SP] + CELLS[SP + 1];
  END;
}

//       case OP_SUB:
//       {
//         ENSURE_STACK_MIN(2, END);
//         SP -= 1;
//         CELLS[SP] = CELLS[SP] - CELLS[SP + 1];
//         END;
//       }
//
//       case OP_MUL:
//       {
//         ENSURE_STACK_MIN(2, END);
//         SP -= 1;
//         CELLS[SP] = CELLS[SP] * CELLS[SP + 1];
//         END;
//       }
//
//       case OP_EQUAL:
//       {
//         ENSURE_STACK_MIN(2, END);
//         SP -= 1;
//         CELLS[SP] = CELLS[SP] == CELLS[SP + 1];
//         END;
//       }
//
//       case OP_LESS:
//       {
//         ENSURE_STACK_MIN(2, END);
//         SP -= 1;
//         CELLS[SP] = CELLS[SP] < CELLS[SP + 1];
//         END;
//       }
//
//       case OP_BRANCH:
//       {
//         pc = (struct code*)pc->value;
//         continue;
//       }
//
//       case OP_ZBRANCH:
//       {
//         POP1();
//         if (!p1) { pc = (struct code*)pc->value; continue; }
//         END;
//       }
//
//       case OP_NBRANCH:
//       {
//         POP1();
//         if (p1) { pc = (struct code*)pc->value; continue; }
//         END;
//       }
//
//       case OP_IF:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f) {
//           R_PUSH((cell)pc);
//           pc = entry_->code - 1;
//         }
//
//         END;
//       }
//
//       case OP_IF_EXIT:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f) {
//           R_PUSH((cell)pc);
//           pc = entry_->code - 1;
//           R_SP -= 1;
//         }
//
//         END;
//       }
//
//       case OP_IF_NOT:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f == 0) {
//           R_PUSH((cell)pc);
//           pc = entry_->code - 1;
//         }
//
//         END;
//       }
//
//       case OP_IF_NOT_EXIT:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f == 0) {
//           R_PUSH((cell)pc);
//           pc = entry_->code - 1;
//           R_SP -= 1;
//         }
//
//         END;
//       }
//
//       case OP_IF_STAR:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f) pc = entry_->code - 1;
//
//         END;
//       }
//
//       case OP_IF_STAR_EXIT:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f) {
//           pc = entry_->code - 1;
//           R_SP -= 1;
//         }
//
//         END;
//       }
//
//       case OP_IF_NOT_STAR:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f == 0) pc = entry_->code - 1;
//
//         END;
//       }
//
//       case OP_IF_NOT_STAR_EXIT:
//       {
//         ENSURE_STACK_MIN(2, END);
//         struct entry *entry_ = (struct entry*)POP();
//         const cell f = POP();
//
//         if (f == 0) {
//           pc = entry_->code - 1;
//           R_SP -= 1;
//         }
//
//         END;
//       }
//
//       case OP_IF_ELSE:
//       {
//         ENSURE_STACK_MIN(3, END);
//         struct entry *entry_false_ = (struct entry*)POP();
//         struct entry *entry_true_ = (struct entry*)POP();
//         const cell f = POP();
//
//         R_PUSH((cell)pc);
//         if (f) {
//           pc = entry_true_->code - 1;
//         }
//         else {
//           pc = entry_false_->code - 1;
//         }
//
//         END;
//       }

case OP_EMIT:
{
  ENSURE_STACK_MIN(1, END);
  cf_putchar(s, (char)CELLS[SP]);
  SP -= 1;
  END;
}

//       case OP_KEY:
//       {
//         ENSURE_STACK_MAX(1, END);
//         PUSH((char)cf_getchar(s));
//         END;
//       }
//
//       case OP_CELL:
//       {
//         ENSURE_STACK_MAX(1, END);
//         PUSH(sizeof(cell));
//         END;
//       }
//
//       case OP_CODE_LEN:
//       {
//         ENSURE_STACK_MAX(1, END);
//         PUSH(sizeof(struct code));
//         END;
//       }
//
//       case OP_GET_ENTRY_CODE:
//       {
//         ENSURE_STACK_MIN(1, END);
//         struct entry *entry_ = (struct entry*)POP();
//         PUSH((cell)entry_->code);
//         END;
//       }
//
//       case OP_EXECUTE:
//       {
//         ENSURE_STACK_MIN(1, END);
//         ENSURE_R_STACK_MAX(1, END);
//         struct code *code_ = (struct code*)POP();
//         R_PUSH((cell)pc);
//         pc = code_ - 1;
//         END;
//       }
//
//       // Like execute but leave xt on the stack
//       case OP_EXECUTE_STAR:
//       {
//         ENSURE_STACK_MIN(1, END);
//         ENSURE_R_STACK_MAX(1, END);
//         R_PUSH((cell)pc);
//         pc = (struct code*)CELLS[SP] - 1;
//         END;
//       }
//
//       case OP_HERE:
//       {
//         ENSURE_STACK_MAX(1, END);
//         PUSH((cell)&s->here);
//         END;
//       }
//
//       case OP_LATEST:
//       {
//         ENSURE_STACK_MAX(1, END);
//         PUSH((cell)&s->dict.latest);
//         END;
//       }
//
//       case OP_GET_CVA: // Code value address
//       {
//         ENSURE_STACK_MIN(1, END);
//         struct code *code = (struct code *) POP();
//         PUSH((cell) &code->value);
//         END;
//       }
//
//       case OP_COMPILE:
//       {
//         ENSURE_STACK_MIN(1, END);
//         struct entry *entry_ = (struct entry*)POP();
//         compile_entry(s, entry_);
//         END;
//       }
//
//       case OP_COMPILE_INLINE:
//       {
//         ENSURE_STACK_MIN(1, END);
//         struct entry *entry_ = (struct entry*)POP();
//         inline_entry(s, entry_);
//         END;
//       }
//
//       case OP_COMPILE_LITERAL:
//       {
//         ENSURE_STACK_MIN(1, END);
//         cell n = POP();
//         compile_literal(s, n);
//         END;
//       }
//
case OP_BYE:
{
  quit(s);
  return;
}

//       case OP_WORDS:
//       {
//         words(s);
//         END;
//       }
//
//       case OP_NOP: {
//         END;
//       }
//
case OP_PRINT_TOS:
{
  ENSURE_STACK_MIN(1, END);
  const cell n = POP();
  cf_print_cell(s, n);
  cf_printf(s, " ");
  cf_fflush();
  END;
}

case OP_DOT_S:
{
  dot_s(s, s->stack);
  END;
}

case OP_CLEAR:
{
  SP = 0;
  END;
}
