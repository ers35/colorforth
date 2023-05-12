#ifndef __HASH_DEF
#define __HASH_DEF

#include <stdint.h>

typedef uint32_t  hash_t;

#ifdef __KEEP_ENTRY_NAMES
#define ENTRY_NAME(x) (x)
#else
#define ENTRY_NAME(x) (NULL)
#endif

// Define __SHOW_MISSING_HASH in conf.h to display name <-> hash relationship
//
// Use `.hash" <string to hash>"` to find them in colorForth

// <-- BEGIN AUTOGEN PART -->

// src/colorforth.c
#define NOP_HASH                       (hash_t) 0x5964713D                // nop
#define PRINT_TOS_HASH                 (hash_t) 0x2E00486A                // .
#define DUP_HASH                       (hash_t) 0xF500DB1                 // dup
#define OVER_HASH                      (hash_t) 0x38C2FE2A                // over
#define SWAP_HASH                      (hash_t) 0xD902F543                // swap
#define DROP_HASH                      (hash_t) 0x1940D1AD                // drop
#define ROT_HASH                       (hash_t) 0x7D519FD                 // rot
#define MINUS_ROT_HASH                 (hash_t) 0xE8E1884C                // -rot
#define NIP_HASH                       (hash_t) 0x4D5E4E2B                // nip
#define ADD_HASH                       (hash_t) 0x2B0043B1                // +
#define SUB_HASH                       (hash_t) 0x2D0046D7                // -
#define MUL_HASH                       (hash_t) 0x2A00421E                // *
#define EQUAL_HASH                     (hash_t) 0x3D006007                // =
#define LESS_HASH                      (hash_t) 0x3C005E74                // <
#define BYE_HASH                       (hash_t) 0x253F7458                // bye
#define WORDS_HASH                     (hash_t) 0x30E76679                // words
#define EMIT_HASH                      (hash_t) 0x9B01D09B                // emit
#define KEY_HASH                       (hash_t) 0x485BCDF                 // key
#define LOAD_HASH                      (hash_t) 0x400064C0                // @
#define STORE_HASH                     (hash_t) 0x210033F3                // !
#define CLOAD_HASH                     (hash_t) 0x72F4F1DB                // c@
#define CSTORE__HASH                   (hash_t) 0xD1F58768                // c!
#define CELL_HASH                      (hash_t) 0x1415DDAC                // cell
#define CODE_LEN_HASH                  (hash_t) 0x61C76E30                // #code
#define HERE_HASH                      (hash_t) 0xD786E25E                // here
#define LATEST_HASH                    (hash_t) 0xA307AFAB                // latest
#define COMPILE_HASH                   (hash_t) 0x12AD19B0                // [^]
#define COMPILE_INLINE_HASH            (hash_t) 0x56B201EA                // [`]
#define COMPILE_LITERAL_HASH           (hash_t) 0x176F15FE                // [,]
#define GET_ENTRY_CODE_HASH            (hash_t) 0x3BC9871                 // code>
#define EXECUTE_HASH                   (hash_t) 0x54DA4CCF                // execute
#define EXECUTE_STAR_HASH              (hash_t) 0x78A70C7F                // execute*
#define GET_CVA_HASH                   (hash_t) 0x5E52BA1E                // cva>
#define BRANCH_HASH                    (hash_t) 0x65C38130                // branch
#define ZBRANCH_HASH                   (hash_t) 0x84502A20                // 0branch
#define NBRANCH_HASH                   (hash_t) 0xB041B3B2                // nbranch
#define IF_HASH                        (hash_t) 0x790405D7                // if
#define IF_EXIT_HASH                   (hash_t) 0x6D555284                // if;
#define IF_NOT_HASH                    (hash_t) 0x1A0E810B                // if-not
#define IF_NOT_EXIT_HASH               (hash_t) 0x34D55E90                // if-not;
#define IF_STAR_HASH                   (hash_t) 0x7E556D47                // if*
#define IF_EXIT_STAR_HASH              (hash_t) 0x5C7B5A34                // if*;
#define IF_NOT_STAR_HASH               (hash_t) 0x25D546F3                // if-not*
#define IF_NOT_EXIT_STAR_HASH          (hash_t) 0x56BE6CD8                // if-not*;
#define IF_ELSE_HASH                   (hash_t) 0xD59EAA1B                // if-else
#define DOT_S_HASH                     (hash_t) 0x83717F5B                // .s
#define RETURN_HASH                    (hash_t) 0x3B005CE1                // ;
#define R_PUSH_HASH                    (hash_t) 0x6299EDD8                // >R
#define R_POP_HASH                     (hash_t) 0x3ECB51F8                // R>
#define R_FETCH_HASH                   (hash_t) 0x6CCB9A62                // R@
#define CLEAR_HASH                     (hash_t) 0x93C5A06D                // clear

// src/lib.c
#define HASH_HASH                      (hash_t) 0x3475B2F0                // hash
#define BASE_HASH                      (hash_t) 0x7D3CE245                // base
#define ENTRY_IS_HASH                  (hash_t) 0x84041728                // is
#define ENTRY_C_IS_HASH                (hash_t) 0x6F1B27F5                // c>is
#define ENTRY_HIDE_HASH                (hash_t) 0x7C95E439                // entry/hide
#define ROOM_HASH                      (hash_t) 0xE48409A3                // room
#define DROP_ROOM_HASH                 (hash_t) 0xC57E1E23                // drop-room

// ext/dict.c
#define SEE_HASH                       (hash_t) 0x519873B3                // see
#define DISASSEMBLE_HASH               (hash_t) 0xF9B7D344                // disassemble
#define FULLROOM_HASH                  (hash_t) 0x85D1BFBA                // fullroom
#define ENTRY__PATCH_HASH              (hash_t) 0x8AC21565                // entry/patch
#define CHECK_DICT_HASH                (hash_t) 0xC03D691F                // check-dict

// ext/ext-math.c
#define FLOAT_HASH                     (hash_t) 0xEF099DDA                // float
#define F_DOTS_HASH                    (hash_t) 0xDF52D985                // f.s
#define F_DOT_HASH                     (hash_t) 0x4EFD07F4                // f.
#define FDROP_HASH                     (hash_t) 0x36C934A7                // fdrop
#define FDUP_HASH                      (hash_t) 0x31A4657F                // fdup
#define FSWAP_HASH                     (hash_t) 0xC890D5A5                // fswap
#define FOVER_HASH                     (hash_t) 0x81A85DC0                // fover
#define FROT_HASH                      (hash_t) 0xE25FE90B                // frot
#define F_SUBROT_HASH                  (hash_t) 0x124F37B6                // f-rot
#define FNIP_HASH                      (hash_t) 0xC38C7AB5                // fnip
#define F_ADD_HASH                     (hash_t) 0x4BFD033B                // f+
#define F_SUB_HASH                     (hash_t) 0x51FD0CAD                // f-
#define F_MUL_HASH                     (hash_t) 0x4AFD01A8                // f*
#define F_DIV_HASH                     (hash_t) 0x4FFD0987                // f/
#define F_TO__HASH                     (hash_t) 0x3EFCEEC4                // f>
#define F_FROM_HASH                    (hash_t) 0x40FCF1EA                // f<
#define F_LOAD_HASH                    (hash_t) 0x64FD2A96                // f@
#define F_STORE_HASH                   (hash_t) 0x45FCF9C9                // f!
#define F_INLINE_HASH                  (hash_t) 0xBF067E82                // [f,]

// ext/io.c
#define GETCHAR_HASH                   (hash_t) 0x151AFB78                // getchar
#define ECHO_ADDR_HASH                 (hash_t) 0xE63F83C1                // echo
#define FILE_SUBSIZE_HASH              (hash_t) 0xD4DD4B30                // file-size
#define FILE_LOAD_HASH                 (hash_t) 0xDE8C953C                // load
#define FILE_SAVE_HASH                 (hash_t) 0x84B337FD                // save
#define INCLUDED_HASH                  (hash_t) 0x7916FE10                // included

// ext/load-ext.c
#define REQUIRE_IO_HASH                (hash_t) 0x1019FCBE                // require-io
#define REQUIRE_PARSER_HASH            (hash_t) 0x293B86D9                // require-parser
#define REQUIRE_OS_HASH                (hash_t) 0x140AF880                // require-os
#define REQUIRE_DICT_HASH              (hash_t) 0x2F05E7C2                // require-dict
#define REQUIRE_TIME_HASH              (hash_t) 0x7AD67647                // require-time
#define REQUIRE_MATH_HASH              (hash_t) 0xBCC8CAB4                // require-math
#define REQUIRE_EXT_MATH_HASH          (hash_t) 0x90F5D17E                // require-ext-math
#define REQUIRE_MP_MATH_HASH           (hash_t) 0x792EF708                // require-mp-math
#define REQUIRE_THREADS_HASH           (hash_t) 0xB4207C0B                // require-threads
#define REQUIRE_NETWORK_HASH           (hash_t) 0xD11CB26C                // require-network
#define REQUIRE_SDL_HASH               (hash_t) 0x86818FEF                // require-sdl

// ext/math.c
#define DIV_HASH                       (hash_t) 0x2F0049FD                // /
#define MOD_HASH                       (hash_t) 0xD32A27E4                // mod
#define DIVMOD_HASH                    (hash_t) 0x465A474B                // /mod
#define MUL_DIV_HASH                   (hash_t) 0x4F683323                // */
#define LSHIFT_HASH                    (hash_t) 0x346A9A70                // lshift
#define RSHIFT_HASH                    (hash_t) 0xB6A4D846                // rshift
#define SUP_HASH                       (hash_t) 0x3E00619A                // >
#define INF_EQUAL_HASH                 (hash_t) 0xBD946CEB                // <=
#define SUP_EQUAL_HASH                 (hash_t) 0x4199B9E5                // >=
#define INFSUP_HASH                    (hash_t) 0xBE946E7E                // <>
#define OR_HASH                        (hash_t) 0x8D132FDD                // or
#define AND_HASH                       (hash_t) 0xADD16739                // and
#define MIN_HASH                       (hash_t) 0xED395B5C                // min
#define MAX_HASH                       (hash_t) 0xDB25B74E                // max
#define ABS_HASH                       (hash_t) 0xB6B4F350                // abs
#define RAND_HASH                      (hash_t) 0xE4758C3F                // rand
#define SRAND_HASH                     (hash_t) 0x69BDC0A                 // srand
#define RANDOM_HASH                    (hash_t) 0x5EB42A27                // random

// ext/mp-math.c
#define MPZ_HASH                       (hash_t) 0xE300F30D                // mpz
#define M_DOTS_HASH                    (hash_t) 0xE18EAD58                // m.s
#define M_DOT_HASH                     (hash_t) 0x510E543B                // m.
#define MDROP_HASH                     (hash_t) 0x864B7ACE                // mdrop
#define MDUP_HASH                      (hash_t) 0xE0DC5F8C                // mdup
#define MSWAP_HASH                     (hash_t) 0x967583F0                // mswap
#define MOVER_HASH                     (hash_t) 0x37AA046D                // mover
#define MROT_HASH                      (hash_t) 0xE60E870C                // mrot
#define M_SUBROT_HASH                  (hash_t) 0xDC31BA2F                // m-rot
#define MNIP_HASH                      (hash_t) 0x7022FC52                // mnip
#define M_ADD_HASH                     (hash_t) 0x540E58F4                // m+
#define M_SUB_HASH                     (hash_t) 0x520E55CE                // m-
#define M_MUL_HASH                     (hash_t) 0x550E5A87                // m*
#define M_DIV_HASH                     (hash_t) 0x500E52A8                // m/
#define M_TO__HASH                     (hash_t) 0x410E3B0B                // m>
#define M_FROM_HASH                    (hash_t) 0x430E3E31                // m<
#define M_EQUAL_HASH                   (hash_t) 0x420E3C9E                // m=
#define M_LOAD_HASH                    (hash_t) 0x6F0E8375                // m@
#define M_STORE_HASH                   (hash_t) 0x4E0E4F82                // m!
#define M_INLINE_HASH                  (hash_t) 0xDC27337F                // [m,]

// ext/network.c
#define SERVER_START_HASH              (hash_t) 0x3F90A4AA                // server-start
#define CLIENT_START_HASH              (hash_t) 0x54C4D466                // client-start
#define SERVER_STOP_HASH               (hash_t) 0x73F7E84A                // server-stop
#define SERVER_NONBLOCKING_HASH        (hash_t) 0x3A7FBA86                // server-nonblocking
#define SERVER_ACCEPT_HASH             (hash_t) 0xA66BDA62                // server-accept
#define SOCKET_SEND_HASH               (hash_t) 0xB830ECB2                // socket-send
#define SOCKET_SEND_CHAR_HASH          (hash_t) 0x49DA8E15                // socket-send-char
#define SOCKET_RECV_HASH               (hash_t) 0x9897D554                // socket-recv

// ext/os.c
#define SYSTEM_HASH                    (hash_t) 0x7BB8F701                // system
#define C_SUBALLOC_HASH                (hash_t) 0xF25B6721                // c-alloc
#define C_SUBFREE_HASH                 (hash_t) 0x318C2312                // c-free

// ext/parser.c
#define PARSE_HASH                     (hash_t) 0x9555EDEF                // parse
#define CLEAR_TIB_HASH                 (hash_t) 0xDE5DBC77                // clear-tib

// ext/sdl.c
#define SDL_INIT_HASH                  (hash_t) 0x92FE07C0                // sdl/init
#define SDL_CLOSE_HASH                 (hash_t) 0xFAD1E96                 // sdl/close
#define SDL_DELAY_HASH                 (hash_t) 0xD811784D                // sdl/delay
#define SDL_SHOW_CURSOR_HASH           (hash_t) 0x6BD0F784                // sdl/show-cursor
#define SDL_HIDE_CURSOR_HASH           (hash_t) 0xB3052F5F                // sdl/hide-cursor
#define SDL_POLL_EVENT_HASH            (hash_t) 0x42D5F536                // sdl/poll-event
#define SDL_PRESENT_HASH               (hash_t) 0x1B628525                // sdl/present
#define SDL_KEY_PRESS_HASH             (hash_t) 0x4224F69A                // sdl/key?
#define SDL_MOUSE_BUTTON_HASH          (hash_t) 0x6755EA17                // sdl/button?
#define SDL_GET_MOUSE_X_HASH           (hash_t) 0x9E5030C4                // sdl/mouse-x@
#define SDL_GET_MOUSE_Y_HASH           (hash_t) 0xC452AB2D                // sdl/mouse-y@
#define SDL_BACKGROUND_HASH            (hash_t) 0x4A51D2AA                // sdl/background
#define SDL_OPEN_FONT_HASH             (hash_t) 0x9D24F958                // sdl/open-font
#define SDL_PUT_TEXT_HASH              (hash_t) 0x25B908D5                // sdl/put-text

// ext/threads.c
#define THREAD__RUN_HASH               (hash_t) 0xE47BB892                // thread/run
#define THREAD__JOIN_SUBALL_HASH       (hash_t) 0x9B920C1F                // thread/join-all
#define THREAD__JOIN_HASH              (hash_t) 0x37303F01                // thread/join
#define THREAD__KILL_HASH              (hash_t) 0x6B3D7E01                // thread/kill
#define THREAD__LOCK_HASH              (hash_t) 0x91795D9A                // thread/lock
#define THREAD__UNLOCK_HASH            (hash_t) 0xE4DECEBD                // thread/unlock

// ext/time.c
#define TIME_HASH                      (hash_t) 0xA16F62B9                // time
#define UTIME_HASH                     (hash_t) 0x9592595E                // utime
#define SLEEP_HASH                     (hash_t) 0xD91187F3                // sleep
#define MSSLEEP_HASH                   (hash_t) 0xBAD535EF                // mssleep

// Register A
#define REG_A_LOAD_HASH                (hash_t) 0x66A0AFE9                // A@
#define REG_A_STORE_HASH               (hash_t) 0xC5A14576                // A!
#define REG_A_ADD_STORE_HASH           (hash_t) 0x57EF066B                // A+!
#define REG_A_ADD_ADD_HASH             (hash_t) 0x51EEFCF9                // A++
#define REG_A_SUB_SUB_HASH             (hash_t) 0x77FE4355                // A--
#define REG_A_TO_R_HASH                (hash_t) 0x2D43D1F                 // A>R
#define REG_R_TO_A_HASH                (hash_t) 0x9311A63B                // R>A

// Register B
#define REG_B_LOAD_HASH                (hash_t) 0x8CA32A52                // B@
#define REG_B_STORE_HASH               (hash_t) 0xADA35E45                // B!
#define REG_B_ADD_STORE_HASH           (hash_t) 0x603C1122                // B+!
#define REG_B_ADD_ADD_HASH             (hash_t) 0x663C1A94                // B++
#define REG_B_SUB_SUB_HASH             (hash_t) 0x603793F4                // B--
#define REG_B_TO_R_HASH                (hash_t) 0x755808AE                // B>R
#define REG_R_TO_B_HASH                (hash_t) 0x9411A7CE                // R>B

// Register C
#define REG_C_LOAD_HASH                (hash_t) 0xB2A5A4BB                // C@
#define REG_C_STORE_HASH               (hash_t) 0xD1A5D588                // C!
#define REG_C_ADD_STORE_HASH           (hash_t) 0xCD007A95                // C+!
#define REG_C_ADD_ADD_HASH             (hash_t) 0xD3008407                // C++
#define REG_C_SUB_SUB_HASH             (hash_t) 0xCD04F7C3                // C--
#define REG_C_TO_R_HASH                (hash_t) 0xDBE4BBB5                // C>R
#define REG_R_TO_C_HASH                (hash_t) 0x9511A961                // R>C

// Register I
#define REG_I_LOAD_HASH                (hash_t) 0x96B48331                // I@
#define REG_I_STORE_HASH               (hash_t) 0xB5B4B3FE                // I!
#define REG_I_ADD_STORE_HASH           (hash_t) 0xAE5EC4B3                // I+!
#define REG_I_ADD_ADD_HASH             (hash_t) 0xB85ED471                // I++
#define REG_I_SUB_SUB_HASH             (hash_t) 0x5E6D514D                // I--
#define REG_I_TO_R_HASH                (hash_t) 0x79924CA7                // I>R
#define REG_R_TO_I_HASH                (hash_t) 0x8B1199A3                // R>I

// Register J
#define REG_J_LOAD_HASH                (hash_t) 0xBCB6FD9A                // J@
#define REG_J_STORE_HASH               (hash_t) 0xDDB7318D                // J!
#define REG_J_ADD_STORE_HASH           (hash_t) 0x374A69AA                // J+!
#define REG_J_ADD_ADD_HASH             (hash_t) 0x2D4A59EC                // J++
#define REG_J_SUB_SUB_HASH             (hash_t) 0xA74509CC                // J--
#define REG_J_TO_R_HASH                (hash_t) 0xCC15E5D6                // J>R
#define REG_R_TO_J_HASH                (hash_t) 0x8C119B36                // R>J
// <-- END AUTOGEN PART -->

#endif /* __HASH_DEF */
