// The author disclaims copyright to this source code.

define_prefix(':', define,         COLOR_RED,     1);
define_prefix('^', compile,        COLOR_GREEN,   0);
define_prefix('~', execute,        COLOR_YELLOW,  0);
define_prefix('\'', tick,          COLOR_BLUE,    0);
define_prefix('`', compile_tick,   COLOR_BLUE,    0);
// define_prefix(',', compile_inline, COLOR_CYAN,    0);

define_primitive(s, ENTRY_NAME("nop"), OP_NOP);
define_primitive(s, ENTRY_NAME("."), OP_PRINT_TOS);
define_primitive(s, ENTRY_NAME("dup"), OP_DUP);
define_primitive(s, ENTRY_NAME("drop"), OP_DROP);
define_primitive(s, ENTRY_NAME("swap"), OP_SWAP);
define_primitive(s, ENTRY_NAME("over"), OP_OVER);
define_primitive(s, ENTRY_NAME("rot"), OP_ROT);
define_primitive(s, ENTRY_NAME("-rot"), OP_MINUS_ROT);
define_primitive(s, ENTRY_NAME("nip"), OP_NIP);
define_primitive(s, ENTRY_NAME("+"), OP_ADD);
define_primitive(s, ENTRY_NAME("-"), OP_SUB);
define_primitive(s, ENTRY_NAME("*"), OP_MUL);
define_primitive(s, ENTRY_NAME("="), OP_EQUAL);
define_primitive(s, ENTRY_NAME("<"), OP_LESS);
define_primitive(s, ENTRY_NAME("bye"), OP_BYE);
// define_primitive(s, ENTRY_NAME("words"), OP_WORDS);
define_primitive(s, ENTRY_NAME("emit"), OP_EMIT);
// define_primitive(s, ENTRY_NAME("key"), OP_KEY);
// define_primitive(s, ENTRY_NAME("@"), OP_LOAD);
// define_primitive(s, ENTRY_NAME("!"), OP_STORE);
// define_primitive(s, ENTRY_NAME("c@"), OP_CLOAD);
// define_primitive(s, ENTRY_NAME("c!"), OP_CSTORE);
// define_primitive(s, ENTRY_NAME("cell"), OP_CELL);
// define_primitive(s, ENTRY_NAME("#code"), OP_CODE_LEN);
// define_primitive(s, ENTRY_NAME("here"), OP_HERE);
// define_primitive(s, ENTRY_NAME("latest"), OP_LATEST);
//
// define_primitive(s, ENTRY_NAME("[^]"), OP_COMPILE);
// define_primitive(s, ENTRY_NAME("[`]"), OP_COMPILE_INLINE);
// define_primitive(s, ENTRY_NAME("[,]"), OP_COMPILE_LITERAL);
// define_primitive(s, ENTRY_NAME("code>"), OP_GET_ENTRY_CODE);
// define_primitive(s, ENTRY_NAME("execute"), OP_EXECUTE);
// define_primitive(s, ENTRY_NAME("execute*"), OP_EXECUTE_STAR);
// define_primitive(s, ENTRY_NAME("cva>"), OP_GET_CVA);
//
// define_primitive(s, ENTRY_NAME("branch"), OP_BRANCH);
// define_primitive(s, ENTRY_NAME("0branch"), OP_ZBRANCH);
// define_primitive(s, ENTRY_NAME("nbranch"), OP_NBRANCH);

define_primitive(s, ENTRY_NAME("if"), OP_IF);
define_primitive(s, ENTRY_NAME("if*"), OP_IF_STAR);
define_primitive(s, ENTRY_NAME("if;"), OP_IF_EXIT);
define_primitive(s, ENTRY_NAME("if-not"), OP_IF_NOT);
define_primitive(s, ENTRY_NAME("if-not*"), OP_IF_NOT_STAR);
define_primitive(s, ENTRY_NAME("if-not;"), OP_IF_NOT_EXIT);

define_primitive(s, ENTRY_NAME("if-else"), OP_IF_ELSE);

define_primitive(s, ENTRY_NAME(".s"), OP_DOT_S);

define_primitive(s, ENTRY_NAME(";"), OP_RETURN);

define_primitive(s, ENTRY_NAME(">R"), OP_R_PUSH);
define_primitive(s, ENTRY_NAME("R>"), OP_R_POP);
define_primitive(s, ENTRY_NAME("R@"), OP_R_FETCH);

define_primitive(s, ENTRY_NAME("clear"), OP_CLEAR);

// init_lib(s);

//#ifdef __USE_REGISTER
//  // A, B, C, I and J registers are state global
//  define_register_primitive(A);
//  define_register_primitive(B);
//  define_register_primitive(C);
//  define_register_primitive(I);
//  define_register_primitive(J);
//  // I, J, K counter
//  // A, B, C, X, Y just register
//#endif
//
//#ifdef __USE_EXTENSIONS
//  load_extensions(s);
//#endif
//
//#ifdef __EMBED_LIB_CF
//  parse_from_embed_lib_cf(s);
//#endif /* __EMBED_LIB_CF */
