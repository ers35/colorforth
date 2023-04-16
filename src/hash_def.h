#ifndef __HASH_DEF_64
#define __HASH_DEF_64

typedef unsigned long hash_t;

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
#define NOP_HASH                       (hash_t) 0xB889592                 // nop
#define PRINT_TOS_HASH                 (hash_t) 0x2B5D3                   // .
#define DUP_HASH                       (hash_t) 0xB886BCE                 // dup
#define OVER_HASH                      (hash_t) 0x17C9BF101               // over
#define SWAP_HASH                      (hash_t) 0x17C9E2640               // swap
#define DROP_HASH                      (hash_t) 0x17C95D91A               // drop
#define ROT_HASH                       (hash_t) 0xB88A69A                 // rot
#define MINUS_ROT_HASH                 (hash_t) 0x17C77B047               // -rot
#define NIP_HASH                       (hash_t) 0xB8894CC                 // nip
#define ADD_HASH                       (hash_t) 0x2B5D0                   // +
#define SUB_HASH                       (hash_t) 0x2B5D2                   // -
#define MUL_HASH                       (hash_t) 0x2B5CF                   // *
#define EQUAL_HASH                     (hash_t) 0x2B5E2                   // =
#define LESS_HASH                      (hash_t) 0x2B5E1                   // <
#define BYE_HASH                       (hash_t) 0xB8863C5                 // bye
#define WORDS_HASH                     (hash_t) 0x3110A73474              // words
#define EMIT_HASH                      (hash_t) 0x17C964F74               // emit
#define KEY_HASH                       (hash_t) 0xB88878E                 // key
#define LOAD_HASH                      (hash_t) 0x2B5E5                   // @
#define STORE_HASH                     (hash_t) 0x2B5C6                   // !
#define CLOAD_HASH                     (hash_t) 0x597748                  // c@
#define CSTORE__HASH                   (hash_t) 0x597729                  // c!
#define CELL_HASH                      (hash_t) 0x17C951505               // cell
#define CODE_LEN_HASH                  (hash_t) 0x310AB088C3              // #code
#define HERE_HASH                      (hash_t) 0x17C97D3A9               // here
#define LATEST_HASH                    (hash_t) 0x6530AE8CA72             // latest
#define COMPILE_HASH                   (hash_t) 0xB88427B                 // [^]
#define COMPILE_INLINE_HASH            (hash_t) 0xB8842BD                 // [`]
#define COMPILE_LITERAL_HASH           (hash_t) 0xB883C09                 // [,]
#define GET_ENTRY_CODE_HASH            (hash_t) 0x310F3D0EBE              // code>
#define EXECUTE_HASH                   (hash_t) 0xD0B281C09678            // execute
#define GET_CVA_HASH                   (hash_t) 0x17C955BBD               // cva>
#define BRANCH_HASH                    (hash_t) 0x652F4BE8DB3             // branch
#define ZBRANCH_HASH                   (hash_t) 0xD0A25F8A8A83            // 0branch
#define NBRANCH_HASH                   (hash_t) 0xD0B504254141            // nbranch
#define WHEN_HASH                      (hash_t) 0x17CA01877               // when
#define WHEN_EXIT_HASH                 (hash_t) 0x3110A32792              // when;
#define UNLESS_HASH                    (hash_t) 0x65320CE139F             // unless
#define UNLESS_EXIT_HASH               (hash_t) 0xD0B73A9087BA            // unless;
#define WHEN_STAR_HASH                 (hash_t) 0x3110A32781              // when*
#define WHEN_EXIT_STAR_HASH            (hash_t) 0x653250817DC             // when*;
#define UNLESS_STAR_HASH               (hash_t) 0xD0B73A9087A9            // unless*
#define UNLESS_EXIT_STAR_HASH          (hash_t) 0x1AE79E8CA17D04          // unless*;
#define IF_ELSE_HASH                   (hash_t) 0xD0B387B8444A            // if-else
#define DOT_S_HASH                     (hash_t) 0x5970A6                  // .s
#define RETURN_HASH                    (hash_t) 0x2B5E0                   // ;
#define R_PUSH_HASH                    (hash_t) 0x597295                  // >R
#define R_POP_HASH                     (hash_t) 0x597515                  // R>
#define R_FETCH_HASH                   (hash_t) 0x597517                  // R@

// src/lib.c
#define HASH_HASH                      (hash_t) 0x17C97C2C9               // hash
#define BASE_HASH                      (hash_t) 0x17C947880               // base
#define ENTRY_IS_HASH                  (hash_t) 0x597841                  // is
#define ENTRY_C_IS_HASH                (hash_t) 0x17C946EC2               // c>is
#define ENTRY_HIDE_HASH                (hash_t) 0x7270A6674E62BAC0        // entry/hide
#define ROOM_HASH                      (hash_t) 0x17C9D79A2               // room
#define DROP_ROOM_HASH                 (hash_t) 0x377C5DA113A5224         // drop-room

// ext/dict.c
#define SEE_HASH                       (hash_t) 0xB88A982                 // see
#define DISASSEMBLE_HASH               (hash_t) 0xC07F2D5B2CDA32F1        // disassemble
#define FULLROOM_HASH                  (hash_t) 0x1AE70BD01BC455          // fullroom
#define ENTRY__PATCH_HASH              (hash_t) 0xC08573511B46B7F6        // entry/patch

// ext/ext-math.c
#define FLOAT_HASH                     (hash_t) 0x310F71E19B              // float
#define F_DOTS_HASH                    (hash_t) 0xB886B2C                 // f.s
#define F_DOT_HASH                     (hash_t) 0x597799                  // f.
#define FDROP_HASH                     (hash_t) 0x310F6D8D20              // fdrop
#define FDUP_HASH                      (hash_t) 0x17C96B714               // fdup
#define FSWAP_HASH                     (hash_t) 0x310F75DA46              // fswap
#define FOVER_HASH                     (hash_t) 0x310F73A507              // fover
#define FROT_HASH                      (hash_t) 0x17C96F1E0               // frot
#define F_SUBROT_HASH                  (hash_t) 0x310F4F644D              // f-rot
#define FNIP_HASH                      (hash_t) 0x17C96E012               // fnip
#define F_ADD_HASH                     (hash_t) 0x597796                  // f+
#define F_SUB_HASH                     (hash_t) 0x597798                  // f-
#define F_MUL_HASH                     (hash_t) 0x597795                  // f*
#define F_DIV_HASH                     (hash_t) 0x59779A                  // f/
#define F_TO__HASH                     (hash_t) 0x5977A9                  // f>
#define F_FROM_HASH                    (hash_t) 0x5977A7                  // f<
#define F_LOAD_HASH                    (hash_t) 0x5977AB                  // f@
#define F_STORE_HASH                   (hash_t) 0x59778C                  // f!
#define F_INLINE_HASH                  (hash_t) 0x17C90ADEF               // [f,]

// ext/io.c
#define GETCHAR_HASH                   (hash_t) 0xD0B2F07241E3            // getchar
#define ECHO_ADDR_HASH                 (hash_t) 0x17C9624C4               // echo
#define FILE_SUBSIZE_HASH              (hash_t) 0x377C80EAB4BB74D         // file-size
#define FILE_LOAD_HASH                 (hash_t) 0x17C9A2D85               // load
#define FILE_SAVE_HASH                 (hash_t) 0x17C9DCB54               // save
#define INCLUDED_HASH                  (hash_t) 0x1AE727650CA10D          // included

// ext/load-ext.c
#define REQUIRE_IO_HASH                (hash_t) 0x7272BF856C1C9407        // require-io
#define REQUIRE_PARSER_HASH            (hash_t) 0xE5289E60A05C7EBC        // require-parser
#define REQUIRE_OS_HASH                (hash_t) 0x7272BF856C1C94D1        // require-os
#define REQUIRE_DICT_HASH              (hash_t) 0xDA20B690E58EE793        // require-dict
#define REQUIRE_TIME_HASH              (hash_t) 0xDA20B690E597AEDE        // require-time
#define REQUIRE_MATH_HASH              (hash_t) 0xDA20B690E593B719        // require-math
#define REQUIRE_EXT_MATH_HASH          (hash_t) 0xD1C9B8A3ECDA3577        // require-ext-math
#define REQUIRE_MP_MATH_HASH           (hash_t) 0x8A3C6A73E3170C03        // require-mp-math
#define REQUIRE_THREADS_HASH           (hash_t) 0x8A3C6A75F021A5FA        // require-threads
#define REQUIRE_NETWORK_HASH           (hash_t) 0x8A3C6A741B730CB9        // require-network

// ext/math.c
#define DIV_HASH                       (hash_t) 0x2B5D4                   // /
#define MOD_HASH                       (hash_t) 0xB889145                 // mod
#define DIVMOD_HASH                    (hash_t) 0x17C78B3B4               // /mod
#define MUL_DIV_HASH                   (hash_t) 0x596FDE                  // */
#define LSHIFT_HASH                    (hash_t) 0x6530C27FE4F             // lshift
#define RSHIFT_HASH                    (hash_t) 0x6531A26F215             // rshift
#define SUP_HASH                       (hash_t) 0x2B5E3                   // >
#define INF_EQUAL_HASH                 (hash_t) 0x59723E                  // <=
#define SUP_EQUAL_HASH                 (hash_t) 0x597280                  // >=
#define INFSUP_HASH                    (hash_t) 0x59723F                  // <>
#define OR_HASH                        (hash_t) 0x597906                  // or
#define AND_HASH                       (hash_t) 0xB885E18                 // and
#define MIN_HASH                       (hash_t) 0xB889089                 // min
#define MAX_HASH                       (hash_t) 0xB888F8B                 // max
#define ABS_HASH                       (hash_t) 0xB885C9B                 // abs
#define RAND_HASH                      (hash_t) 0x17C9D3DEA               // rand
#define SRAND_HASH                     (hash_t) 0x311060307D              // srand
#define RANDOM_HASH                    (hash_t) 0x65318E46F26             // random

// ext/mp-math.c
#define MPZ_HASH                       (hash_t) 0xB88917C                 // mpz
#define M_DOTS_HASH                    (hash_t) 0xB8888F3                 // m.s
#define M_DOT_HASH                     (hash_t) 0x597880                  // m.
#define MDROP_HASH                     (hash_t) 0x310FEC38A7              // mdrop
#define MDUP_HASH                      (hash_t) 0x17C9A8DBB               // mdup
#define MSWAP_HASH                     (hash_t) 0x310FF485CD              // mswap
#define MOVER_HASH                     (hash_t) 0x310FF2508E              // mover
#define MROT_HASH                      (hash_t) 0x17C9AC887               // mrot
#define M_SUBROT_HASH                  (hash_t) 0x310FCE0FD4              // m-rot
#define MNIP_HASH                      (hash_t) 0x17C9AB6B9               // mnip
#define M_ADD_HASH                     (hash_t) 0x59787D                  // m+
#define M_SUB_HASH                     (hash_t) 0x59787F                  // m-
#define M_MUL_HASH                     (hash_t) 0x59787C                  // m*
#define M_DIV_HASH                     (hash_t) 0x597881                  // m/
#define M_TO__HASH                     (hash_t) 0x597890                  // m>
#define M_FROM_HASH                    (hash_t) 0x59788E                  // m<
#define M_EQUAL_HASH                   (hash_t) 0x59788F                  // m=
#define M_LOAD_HASH                    (hash_t) 0x597892                  // m@
#define M_STORE_HASH                   (hash_t) 0x597873                  // m!
#define M_INLINE_HASH                  (hash_t) 0x17C90CBB6               // [m,]

// ext/network.c
#define SERVER_START_HASH              (hash_t) 0xDAD471AD605C6997        // server-start
#define CLIENT_START_HASH              (hash_t) 0xCFBFFAACFE51E27F        // client-start
#define SERVER_STOP_HASH               (hash_t) 0xC0D022799E12528F        // server-stop
#define SERVER_NONBLOCKING_HASH        (hash_t) 0x409DE8055967B95D        // server-nonblocking
#define SERVER_ACCEPT_HASH             (hash_t) 0x3562A75940BA01F9        // server-accept
#define SOCKET_SEND_HASH               (hash_t) 0xC0D1B4F8D1074A65        // socket-send
#define SOCKET_SEND_CHAR_HASH          (hash_t) 0x106927251A24F110        // socket-send-char
#define SOCKET_RECV_HASH               (hash_t) 0xC0D1B4F8D106BCAB        // socket-recv

// ext/os.c
#define SYSTEM_HASH                    (hash_t) 0x6531CEEE48A             // system
#define C_SUBALLOC_HASH                (hash_t) 0xD0B138959C40            // c-alloc
#define C_SUBFREE_HASH                 (hash_t) 0x652F235EAB7             // c-free

// ext/parser.c
#define PARSE_HASH                     (hash_t) 0x311020DD80              // parse
#define CLEAR_TIB_HASH                 (hash_t) 0x377C453F329FBF8         // clear-tib

// ext/threads.c
#define THREAD__RUN_HASH               (hash_t) 0x727317CCDA8200A1        // thread/run
#define THREAD__JOIN_SUBALL_HASH       (hash_t) 0x76D38AD3FF9B8E02        // thread/join-all
#define THREAD__JOIN_HASH              (hash_t) 0xC0D611682ABD97FC        // thread/join
#define THREAD__KILL_HASH              (hash_t) 0xC0D611682ABE0B38        // thread/kill
#define THREAD__LOCK_HASH              (hash_t) 0xC0D611682ABEAFF5        // thread/lock
#define THREAD__UNLOCK_HASH            (hash_t) 0x4EA00C1DEA1BE3F8        // thread/unlock

// ext/time.c
#define TIME_HASH                      (hash_t) 0x17C9E7894               // time
#define UTIME_HASH                     (hash_t) 0x3110859C29              // utime
#define SLEEP_HASH                     (hash_t) 0x31105CF61E              // sleep
#define MSSLEEP_HASH                   (hash_t) 0xD0B4DEEAB1DE            // mssleep

// Register A
#define REG_A_LOAD_HASH                (hash_t) 0x5972E6                  // A@
#define REG_A_STORE_HASH               (hash_t) 0x5972C7                  // A!
#define REG_A_ADD_STORE_HASH           (hash_t) 0xB87CD12                 // A+!
#define REG_A_ADD_ADD_HASH             (hash_t) 0xB87CD1C                 // A++
#define REG_A_SUB_SUB_HASH             (hash_t) 0xB87CD60                 // A--
#define REG_A_TO_R_HASH                (hash_t) 0xB87CFB6                 // A>R
#define REG_R_TO_A_HASH                (hash_t) 0xB8817F6                 // R>A

// Register B
#define REG_B_LOAD_HASH                (hash_t) 0x597307                  // B@
#define REG_B_STORE_HASH               (hash_t) 0x5972E8                  // B!
#define REG_B_ADD_STORE_HASH           (hash_t) 0xB87D153                 // B+!
#define REG_B_ADD_ADD_HASH             (hash_t) 0xB87D15D                 // B++
#define REG_B_SUB_SUB_HASH             (hash_t) 0xB87D1A1                 // B--
#define REG_B_TO_R_HASH                (hash_t) 0xB87D3F7                 // B>R
#define REG_R_TO_B_HASH                (hash_t) 0xB8817F7                 // R>B

// Register C
#define REG_C_LOAD_HASH                (hash_t) 0x597328                  // C@
#define REG_C_STORE_HASH               (hash_t) 0x597309                  // C!
#define REG_C_ADD_STORE_HASH           (hash_t) 0xB87D594                 // C+!
#define REG_C_ADD_ADD_HASH             (hash_t) 0xB87D59E                 // C++
#define REG_C_SUB_SUB_HASH             (hash_t) 0xB87D5E2                 // C--
#define REG_C_TO_R_HASH                (hash_t) 0xB87D838                 // C>R
#define REG_R_TO_C_HASH                (hash_t) 0xB8817F8                 // R>C

// Register I
#define REG_I_LOAD_HASH                (hash_t) 0x5973EE                  // I@
#define REG_I_STORE_HASH               (hash_t) 0x5973CF                  // I!
#define REG_I_ADD_STORE_HASH           (hash_t) 0xB87EF1A                 // I+!
#define REG_I_ADD_ADD_HASH             (hash_t) 0xB87EF24                 // I++
#define REG_I_SUB_SUB_HASH             (hash_t) 0xB87EF68                 // I--
#define REG_I_TO_R_HASH                (hash_t) 0xB87F1BE                 // I>R
#define REG_R_TO_I_HASH                (hash_t) 0xB8817FE                 // R>I

// Register J
#define REG_J_LOAD_HASH                (hash_t) 0x59740F                  // J@
#define REG_J_STORE_HASH               (hash_t) 0x5973F0                  // J!
#define REG_J_ADD_STORE_HASH           (hash_t) 0xB87F35B                 // J+!
#define REG_J_ADD_ADD_HASH             (hash_t) 0xB87F365                 // J++
#define REG_J_SUB_SUB_HASH             (hash_t) 0xB87F3A9                 // J--
#define REG_J_TO_R_HASH                (hash_t) 0xB87F5FF                 // J>R
#define REG_R_TO_J_HASH                (hash_t) 0xB8817FF                 // R>J
// <-- END AUTOGEN PART -->

#endif /* __HASH_DEF_64 */
