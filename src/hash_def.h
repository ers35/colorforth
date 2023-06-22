#ifndef __HASH_DEF
#define __HASH_DEF

#include <stdint.h>

typedef uint32_t hash_t;
typedef hash_t opcode_t;

#ifdef __KEEP_ENTRY_NAMES
#define ENTRY_NAME(x) (x)
#else
#define ENTRY_NAME(x) (NULL)
#endif

// Define __SHOW_MISSING_HASH in conf.h to display name <-> hash relationship
//
// Use `.hash" <string to hash>"` to find them in colorForth

// Internal OP
#define OP_CALL                      (opcode_t) 0xF85D926E                // __OP_CALL
#define OP_TAIL_CALL                 (opcode_t) 0x1A03DDDD                // __OP_TAIL_CALL
#define OP_NUMBER                    (opcode_t) 0xA2AB56D7                // __OP_NUMBER
#define OP_TICK_NUMBER               (opcode_t) 0x544A7A21                // __OP_TICK_NUMBER

// <-- BEGIN AUTOGEN PART -->

// src/colorforth.c
#define OP_NOP                       (opcode_t) 0x5964713D                // nop
#define OP_PRINT_TOS                 (opcode_t) 0x2E00486A                // .
#define OP_DUP                       (opcode_t) 0xF500DB1                 // dup
#define OP_OVER                      (opcode_t) 0x38C2FE2A                // over
#define OP_SWAP                      (opcode_t) 0xD902F543                // swap
#define OP_DROP                      (opcode_t) 0x1940D1AD                // drop
#define OP_ROT                       (opcode_t) 0x7D519FD                 // rot
#define OP_MINUS_ROT                 (opcode_t) 0xE8E1884C                // -rot
#define OP_NIP                       (opcode_t) 0x4D5E4E2B                // nip
#define OP_ADD                       (opcode_t) 0x2B0043B1                // +
#define OP_SUB                       (opcode_t) 0x2D0046D7                // -
#define OP_MUL                       (opcode_t) 0x2A00421E                // *
#define OP_EQUAL                     (opcode_t) 0x3D006007                // =
#define OP_LESS                      (opcode_t) 0x3C005E74                // <
#define OP_BYE                       (opcode_t) 0x253F7458                // bye
#define OP_WORDS                     (opcode_t) 0x30E76679                // words
#define OP_EMIT                      (opcode_t) 0x9B01D09B                // emit
#define OP_KEY                       (opcode_t) 0x485BCDF                 // key
#define OP_LOAD                      (opcode_t) 0x400064C0                // @
#define OP_STORE                     (opcode_t) 0x210033F3                // !
#define OP_CLOAD                     (opcode_t) 0x72F4F1DB                // c@
#define OP_CSTORE_                   (opcode_t) 0xD1F58768                // c!
#define OP_CELL                      (opcode_t) 0x1415DDAC                // cell
#define OP_CODE_LEN                  (opcode_t) 0x61C76E30                // #code
#define OP_HERE                      (opcode_t) 0xD786E25E                // here
#define OP_LATEST                    (opcode_t) 0xA307AFAB                // latest
#define OP_COMPILE                   (opcode_t) 0x12AD19B0                // [^]
#define OP_COMPILE_INLINE            (opcode_t) 0x56B201EA                // [`]
#define OP_COMPILE_LITERAL           (opcode_t) 0x176F15FE                // [,]
#define OP_GET_ENTRY_CODE            (opcode_t) 0x3BC9871                 // code>
#define OP_EXECUTE                   (opcode_t) 0x54DA4CCF                // execute
#define OP_EXECUTE_STAR              (opcode_t) 0x78A70C7F                // execute*
#define OP_GET_CVA                   (opcode_t) 0x5E52BA1E                // cva>
#define OP_BRANCH                    (opcode_t) 0x65C38130                // branch
#define OP_ZBRANCH                   (opcode_t) 0x84502A20                // 0branch
#define OP_NBRANCH                   (opcode_t) 0xB041B3B2                // nbranch
#define OP_IF                        (opcode_t) 0x790405D7                // if
#define OP_IF_STAR                   (opcode_t) 0x7E556D47                // if*
#define OP_IF_EXIT                   (opcode_t) 0x6D555284                // if;
#define OP_IF_NOT                    (opcode_t) 0x1A0E810B                // if-not
#define OP_IF_NOT_STAR               (opcode_t) 0x25D546F3                // if-not*
#define OP_IF_NOT_EXIT               (opcode_t) 0x34D55E90                // if-not;
#define OP_IF_ELSE                   (opcode_t) 0xD59EAA1B                // if-else
#define OP_DOT_S                     (opcode_t) 0x83717F5B                // .s
#define OP_RETURN                    (opcode_t) 0x3B005CE1                // ;
#define OP_R_PUSH                    (opcode_t) 0x6299EDD8                // >R
#define OP_R_POP                     (opcode_t) 0x3ECB51F8                // R>
#define OP_R_FETCH                   (opcode_t) 0x6CCB9A62                // R@
#define OP_CLEAR                     (opcode_t) 0x93C5A06D                // clear

// src/lib.c
#define OP_HASH                      (opcode_t) 0x3475B2F0                // hash
#define OP_BASE                      (opcode_t) 0x7D3CE245                // base
#define OP_ENTRY_IS                  (opcode_t) 0x84041728                // is
#define OP_ENTRY_C_IS                (opcode_t) 0x6F1B27F5                // c>is
#define OP_ENTRY_HIDE                (opcode_t) 0x7C95E439                // entry/hide
#define OP_ROOM                      (opcode_t) 0xE48409A3                // room
#define OP_DROP_ROOM                 (opcode_t) 0xC57E1E23                // drop-room

// ext/dict.c
#define OP_SEE                       (opcode_t) 0x519873B3                // see
#define OP_DISASSEMBLE               (opcode_t) 0xF9B7D344                // disassemble
#define OP_FULLROOM                  (opcode_t) 0x85D1BFBA                // fullroom
#define OP_ENTRY__PATCH              (opcode_t) 0x8AC21565                // entry/patch
#define OP_CHECK_DICT                (opcode_t) 0xC03D691F                // check-dict

// ext/ext-math.c
#define OP_FLOAT                     (opcode_t) 0xEF099DDA                // float
#define OP_F_DOTS                    (opcode_t) 0xDF52D985                // f.s
#define OP_F_DOT                     (opcode_t) 0x4EFD07F4                // f.
#define OP_FDROP                     (opcode_t) 0x36C934A7                // fdrop
#define OP_FDUP                      (opcode_t) 0x31A4657F                // fdup
#define OP_FSWAP                     (opcode_t) 0xC890D5A5                // fswap
#define OP_FOVER                     (opcode_t) 0x81A85DC0                // fover
#define OP_FROT                      (opcode_t) 0xE25FE90B                // frot
#define OP_F_SUBROT                  (opcode_t) 0x124F37B6                // f-rot
#define OP_FNIP                      (opcode_t) 0xC38C7AB5                // fnip
#define OP_F_ADD                     (opcode_t) 0x4BFD033B                // f+
#define OP_F_SUB                     (opcode_t) 0x51FD0CAD                // f-
#define OP_F_MUL                     (opcode_t) 0x4AFD01A8                // f*
#define OP_F_DIV                     (opcode_t) 0x4FFD0987                // f/
#define OP_F_TO_                     (opcode_t) 0x3EFCEEC4                // f>
#define OP_F_FROM                    (opcode_t) 0x40FCF1EA                // f<
#define OP_F_LOAD                    (opcode_t) 0x64FD2A96                // f@
#define OP_F_STORE                   (opcode_t) 0x45FCF9C9                // f!
#define OP_F_INLINE                  (opcode_t) 0xBF067E82                // [f,]

// ext/io.c
#define OP_GETCHAR                   (opcode_t) 0x151AFB78                // getchar
#define OP_ECHO_ADDR                 (opcode_t) 0xE63F83C1                // echo
#define OP_FILE_SUBSIZE              (opcode_t) 0xD4DD4B30                // file-size
#define OP_FILE_LOAD                 (opcode_t) 0xDE8C953C                // load
#define OP_FILE_SAVE                 (opcode_t) 0x84B337FD                // save
#define OP_INCLUDED                  (opcode_t) 0x7916FE10                // included

// ext/load-ext.c
#define OP_REQUIRE_IO                (opcode_t) 0x1019FCBE                // require-io
#define OP_REQUIRE_PARSER            (opcode_t) 0x293B86D9                // require-parser
#define OP_REQUIRE_OS                (opcode_t) 0x140AF880                // require-os
#define OP_REQUIRE_DICT              (opcode_t) 0x2F05E7C2                // require-dict
#define OP_REQUIRE_TIME              (opcode_t) 0x7AD67647                // require-time
#define OP_REQUIRE_MATH              (opcode_t) 0xBCC8CAB4                // require-math
#define OP_REQUIRE_EXT_MATH          (opcode_t) 0x90F5D17E                // require-ext-math
#define OP_REQUIRE_MP_MATH           (opcode_t) 0x792EF708                // require-mp-math
#define OP_REQUIRE_THREADS           (opcode_t) 0xB4207C0B                // require-threads
#define OP_REQUIRE_NETWORK           (opcode_t) 0xD11CB26C                // require-network
#define OP_REQUIRE_SDL               (opcode_t) 0x86818FEF                // require-sdl

// ext/math.c
#define OP_DIV                       (opcode_t) 0x2F0049FD                // /
#define OP_MOD                       (opcode_t) 0xD32A27E4                // mod
#define OP_DIVMOD                    (opcode_t) 0x465A474B                // /mod
#define OP_MUL_DIV                   (opcode_t) 0x4F683323                // */
#define OP_LSHIFT                    (opcode_t) 0x346A9A70                // lshift
#define OP_RSHIFT                    (opcode_t) 0xB6A4D846                // rshift
#define OP_SUP                       (opcode_t) 0x3E00619A                // >
#define OP_INF_EQUAL                 (opcode_t) 0xBD946CEB                // <=
#define OP_SUP_EQUAL                 (opcode_t) 0x4199B9E5                // >=
#define OP_INFSUP                    (opcode_t) 0xBE946E7E                // <>
#define OP_OR                        (opcode_t) 0x8D132FDD                // or
#define OP_AND                       (opcode_t) 0xADD16739                // and
#define OP_MIN                       (opcode_t) 0xED395B5C                // min
#define OP_MAX                       (opcode_t) 0xDB25B74E                // max
#define OP_ABS                       (opcode_t) 0xB6B4F350                // abs
#define OP_RAND                      (opcode_t) 0xE4758C3F                // rand
#define OP_SRAND                     (opcode_t) 0x69BDC0A                 // srand
#define OP_RANDOM                    (opcode_t) 0x5EB42A27                // random

// ext/mp-math.c
#define OP_MPZ                       (opcode_t) 0xE300F30D                // mpz
#define OP_M_DOTS                    (opcode_t) 0xE18EAD58                // m.s
#define OP_M_DOT                     (opcode_t) 0x510E543B                // m.
#define OP_MDROP                     (opcode_t) 0x864B7ACE                // mdrop
#define OP_MDUP                      (opcode_t) 0xE0DC5F8C                // mdup
#define OP_MSWAP                     (opcode_t) 0x967583F0                // mswap
#define OP_MOVER                     (opcode_t) 0x37AA046D                // mover
#define OP_MROT                      (opcode_t) 0xE60E870C                // mrot
#define OP_M_SUBROT                  (opcode_t) 0xDC31BA2F                // m-rot
#define OP_MNIP                      (opcode_t) 0x7022FC52                // mnip
#define OP_M_ADD                     (opcode_t) 0x540E58F4                // m+
#define OP_M_SUB                     (opcode_t) 0x520E55CE                // m-
#define OP_M_MUL                     (opcode_t) 0x550E5A87                // m*
#define OP_M_DIV                     (opcode_t) 0x500E52A8                // m/
#define OP_M_TO_                     (opcode_t) 0x410E3B0B                // m>
#define OP_M_FROM                    (opcode_t) 0x430E3E31                // m<
#define OP_M_EQUAL                   (opcode_t) 0x420E3C9E                // m=
#define OP_M_LOAD                    (opcode_t) 0x6F0E8375                // m@
#define OP_M_STORE                   (opcode_t) 0x4E0E4F82                // m!
#define OP_M_INLINE                  (opcode_t) 0xDC27337F                // [m,]

// ext/network.c
#define OP_SERVER_START              (opcode_t) 0x3F90A4AA                // server-start
#define OP_CLIENT_START              (opcode_t) 0x54C4D466                // client-start
#define OP_SERVER_STOP               (opcode_t) 0x73F7E84A                // server-stop
#define OP_SERVER_NONBLOCKING        (opcode_t) 0x3A7FBA86                // server-nonblocking
#define OP_SERVER_ACCEPT             (opcode_t) 0xA66BDA62                // server-accept
#define OP_SOCKET_SEND               (opcode_t) 0xB830ECB2                // socket-send
#define OP_SOCKET_SEND_CHAR          (opcode_t) 0x49DA8E15                // socket-send-char
#define OP_SOCKET_RECV               (opcode_t) 0x9897D554                // socket-recv

// ext/os.c
#define OP_SYSTEM                    (opcode_t) 0x7BB8F701                // system
#define OP_C_SUBALLOC                (opcode_t) 0xF25B6721                // c-alloc
#define OP_C_SUBFREE                 (opcode_t) 0x318C2312                // c-free

// ext/parser.c
#define OP_PARSE                     (opcode_t) 0x9555EDEF                // parse
#define OP_CLEAR_TIB                 (opcode_t) 0xDE5DBC77                // clear-tib

// ext/sdl.c
#define OP_SDL_INIT                  (opcode_t) 0x92FE07C0                // sdl/init
#define OP_SDL_CLOSE                 (opcode_t) 0xFAD1E96                 // sdl/close
#define OP_SDL_DELAY                 (opcode_t) 0xD811784D                // sdl/delay
#define OP_SDL_SHOW_CURSOR           (opcode_t) 0x6BD0F784                // sdl/show-cursor
#define OP_SDL_HIDE_CURSOR           (opcode_t) 0xB3052F5F                // sdl/hide-cursor
#define OP_SDL_POLL_EVENT            (opcode_t) 0x42D5F536                // sdl/poll-event
#define OP_SDL_PRESENT               (opcode_t) 0x1B628525                // sdl/present
#define OP_SDL_KEY_PRESS             (opcode_t) 0x4224F69A                // sdl/key?
#define OP_SDL_MOUSE_BUTTON          (opcode_t) 0x6755EA17                // sdl/button?
#define OP_SDL_GET_MOUSE_X           (opcode_t) 0x9E5030C4                // sdl/mouse-x@
#define OP_SDL_GET_MOUSE_Y           (opcode_t) 0xC452AB2D                // sdl/mouse-y@
#define OP_SDL_BACKGROUND            (opcode_t) 0x4A51D2AA                // sdl/background
#define OP_SDL_OPEN_FONT             (opcode_t) 0x9D24F958                // sdl/open-font
#define OP_SDL_PUT_TEXT              (opcode_t) 0x25B908D5                // sdl/put-text
#define OP_SDL_GET_TEXT_SIZE         (opcode_t) 0x3B567CA7                // sdl/text-size@
#define OP_SDL_LOAD_IMAGE            (opcode_t) 0xA30EA3E2                // sdl/load-image
#define OP_SDL_PUT_IMAGE             (opcode_t) 0x3DFA782B                // sdl/put-image
#define OP_SDL_PLAY_SOUND            (opcode_t) 0x8EFAE1EA                // sdl/play-sound
#define OP_SDL_HALT_SOUND            (opcode_t) 0xB451120F                // sdl/halt-sound
#define OP_SDL_VOLUME                (opcode_t) 0xCCB3E567                // sdl/volume!
#define OP_SDL_LOAD_SOUND            (opcode_t) 0x3F337FC                 // sdl/load-sound
#define OP_SDL_GET_TICKS             (opcode_t) 0xA0C11FC                 // sdl/ticks@

// ext/threads.c
#define OP_THREAD__RUN               (opcode_t) 0xE47BB892                // thread/run
#define OP_THREAD__JOIN_SUBALL       (opcode_t) 0x9B920C1F                // thread/join-all
#define OP_THREAD__JOIN              (opcode_t) 0x37303F01                // thread/join
#define OP_THREAD__KILL              (opcode_t) 0x6B3D7E01                // thread/kill
#define OP_THREAD__LOCK              (opcode_t) 0x91795D9A                // thread/lock
#define OP_THREAD__UNLOCK            (opcode_t) 0xE4DECEBD                // thread/unlock

// ext/time.c
#define OP_TIME                      (opcode_t) 0xA16F62B9                // time
#define OP_UTIME                     (opcode_t) 0x9592595E                // utime
#define OP_SLEEP                     (opcode_t) 0xD91187F3                // sleep
#define OP_MSSLEEP                   (opcode_t) 0xBAD535EF                // mssleep

// Register A
#define OP_REG_A_LOAD                (opcode_t) 0x66A0AFE9                // A@
#define OP_REG_A_STORE               (opcode_t) 0xC5A14576                // A!
#define OP_REG_A_ADD_STORE           (opcode_t) 0x57EF066B                // A+!
#define OP_REG_A_ADD_ADD             (opcode_t) 0x51EEFCF9                // A++
#define OP_REG_A_SUB_SUB             (opcode_t) 0x77FE4355                // A--
#define OP_REG_A_TO_R                (opcode_t) 0x2D43D1F                 // A>R
#define OP_REG_R_TO_A                (opcode_t) 0x9311A63B                // R>A

// Register B
#define OP_REG_B_LOAD                (opcode_t) 0x8CA32A52                // B@
#define OP_REG_B_STORE               (opcode_t) 0xADA35E45                // B!
#define OP_REG_B_ADD_STORE           (opcode_t) 0x603C1122                // B+!
#define OP_REG_B_ADD_ADD             (opcode_t) 0x663C1A94                // B++
#define OP_REG_B_SUB_SUB             (opcode_t) 0x603793F4                // B--
#define OP_REG_B_TO_R                (opcode_t) 0x755808AE                // B>R
#define OP_REG_R_TO_B                (opcode_t) 0x9411A7CE                // R>B

// Register C
#define OP_REG_C_LOAD                (opcode_t) 0xB2A5A4BB                // C@
#define OP_REG_C_STORE               (opcode_t) 0xD1A5D588                // C!
#define OP_REG_C_ADD_STORE           (opcode_t) 0xCD007A95                // C+!
#define OP_REG_C_ADD_ADD             (opcode_t) 0xD3008407                // C++
#define OP_REG_C_SUB_SUB             (opcode_t) 0xCD04F7C3                // C--
#define OP_REG_C_TO_R                (opcode_t) 0xDBE4BBB5                // C>R
#define OP_REG_R_TO_C                (opcode_t) 0x9511A961                // R>C

// Register I
#define OP_REG_I_LOAD                (opcode_t) 0x96B48331                // I@
#define OP_REG_I_STORE               (opcode_t) 0xB5B4B3FE                // I!
#define OP_REG_I_ADD_STORE           (opcode_t) 0xAE5EC4B3                // I+!
#define OP_REG_I_ADD_ADD             (opcode_t) 0xB85ED471                // I++
#define OP_REG_I_SUB_SUB             (opcode_t) 0x5E6D514D                // I--
#define OP_REG_I_TO_R                (opcode_t) 0x79924CA7                // I>R
#define OP_REG_R_TO_I                (opcode_t) 0x8B1199A3                // R>I

// Register J
#define OP_REG_J_LOAD                (opcode_t) 0xBCB6FD9A                // J@
#define OP_REG_J_STORE               (opcode_t) 0xDDB7318D                // J!
#define OP_REG_J_ADD_STORE           (opcode_t) 0x374A69AA                // J+!
#define OP_REG_J_ADD_ADD             (opcode_t) 0x2D4A59EC                // J++
#define OP_REG_J_SUB_SUB             (opcode_t) 0xA74509CC                // J--
#define OP_REG_J_TO_R                (opcode_t) 0xCC15E5D6                // J>R
#define OP_REG_R_TO_J                (opcode_t) 0x8C119B36                // R>J
// <-- END AUTOGEN PART -->

#endif /* __HASH_DEF */
