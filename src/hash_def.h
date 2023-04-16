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
#define NOP_HASH                       (hash_t) 0x91B79D2937A849A9        // nop
#define PRINT_TOS_HASH                 (hash_t) 0x6878508594E148F         // .
#define DUP_HASH                       (hash_t) 0xF1E269D53B1F5D03        // dup
#define OVER_HASH                      (hash_t) 0xACB60B9BECE895DD        // over
#define SWAP_HASH                      (hash_t) 0xC15BEFA996B98FD         // swap
#define DROP_HASH                      (hash_t) 0x35726EA4E8045A0C        // drop
#define ROT_HASH                       (hash_t) 0x5EBABF8E587EF9D2        // rot
#define MINUS_ROT_HASH                 (hash_t) 0xAC08F26541939150        // -rot
#define NIP_HASH                       (hash_t) 0x332943A6D699840F        // nip
#define ADD_HASH                       (hash_t) 0x687850925C6A2DA         // +
#define SUB_HASH                       (hash_t) 0x6878509FD6346A4         // -
#define MUL_HASH                       (hash_t) 0x687850AC9F36C4B         // *
#define EQUAL_HASH                     (hash_t) 0x687850E20526389         // =
#define LESS_HASH                      (hash_t) 0x687850FC47FB168         // <
#define BYE_HASH                       (hash_t) 0xEDDA931417035137        // bye
#define WORDS_HASH                     (hash_t) 0x3BC8D2C60847445D        // words
#define EMIT_HASH                      (hash_t) 0xEC91E9D3FF8EAB26        // emit
#define KEY_HASH                       (hash_t) 0xE7A6F7A644E0FCBE        // key
#define LOAD_HASH                      (hash_t) 0x6878AF3895E2BD6         // @
#define STORE_HASH                     (hash_t) 0x68785074CA6FBA1         // !
#define CLOAD_HASH                     (hash_t) 0x97C28A1872E49F5D        // c@
#define CSTORE__HASH                   (hash_t) 0x97C28A1350479502        // c!
#define CELL_HASH                      (hash_t) 0x144376F7473CEAF0        // cell
#define CODE_LEN_HASH                  (hash_t) 0xA6780B74F31FCFC5        // #code
#define HERE_HASH                      (hash_t) 0xDA59178201DBE664        // here
#define LATEST_HASH                    (hash_t) 0x5697B20AE38F3D4F        // latest
#define COMPILE_HASH                   (hash_t) 0xF340DC6215E408C6        // [^]
#define COMPILE_INLINE_HASH            (hash_t) 0x8B0622FF5389E8B1        // [`]
#define COMPILE_LITERAL_HASH           (hash_t) 0x2724B103D85288F0        // [,]
#define GET_ENTRY_CODE_HASH            (hash_t) 0x349734B66EE516ED        // code>
#define EXECUTE_HASH                   (hash_t) 0x16ECB1185CE6CF7D        // execute
#define GET_CVA_HASH                   (hash_t) 0x7F76FB8CC9A1FC19        // cva>
#define BRANCH_HASH                    (hash_t) 0xA5AA826E0EAAC82D        // branch
#define ZBRANCH_HASH                   (hash_t) 0x2235384E16532903        // 0branch
#define NBRANCH_HASH                   (hash_t) 0xE44C301ECA3FEAB7        // nbranch
#define WHEN_HASH                      (hash_t) 0x42D5B9EB84C8B159        // when
#define WHEN_EXIT_HASH                 (hash_t) 0xEE168CFB38E8428C        // when;
#define UNLESS_HASH                    (hash_t) 0xA76D26C0676F8BCF        // unless
#define UNLESS_EXIT_HASH               (hash_t) 0xD2A461BA3162F807        // unless;
#define WHEN_STAR_HASH                 (hash_t) 0xEE168CF513849688        // when*
#define WHEN_EXIT_STAR_HASH            (hash_t) 0x4B70BCD8E606C5C0        // when*;
#define UNLESS_STAR_HASH               (hash_t) 0xD2A461A7DEEC7720        // unless*
#define UNLESS_EXIT_STAR_HASH          (hash_t) 0x9CD8944E95E87990        // unless*;
#define IF_ELSE_HASH                   (hash_t) 0xF2C7C3B5370332D8        // if-else
#define DOT_S_HASH                     (hash_t) 0xB885C051A3F61023        // .s
#define RETURN_HASH                    (hash_t) 0x687850F68ACDEA7         // ;
#define R_PUSH_HASH                    (hash_t) 0xF7694DF1EEABE10E        // >R
#define R_POP_HASH                     (hash_t) 0xFA0DE9E321A67D04        // R>
#define R_FETCH_HASH                   (hash_t) 0xFA0DEA01CEE7241B        // R@

// src/lib.c
#define HASH_HASH                      (hash_t) 0x5B533835D84DF08D        // hash
#define BASE_HASH                      (hash_t) 0x41B5597128C8146C        // base
#define ENTRY_IS_HASH                  (hash_t) 0xB7355B4FA8479A6B        // is
#define ENTRY_C_IS_HASH                (hash_t) 0xCB23AE93BC3922          // c>is
#define ENTRY_HIDE_HASH                (hash_t) 0xF4922B65554DA031        // entry/hide
#define ROOM_HASH                      (hash_t) 0x237314D20477DE6A        // room
#define DROP_ROOM_HASH                 (hash_t) 0x81936F837B626272        // drop-room

// ext/dict.c
#define SEE_HASH                       (hash_t) 0xFEB7E0960004EEAC        // see
#define DISASSEMBLE_HASH               (hash_t) 0x3D8AAE808F382E35        // disassemble
#define FULLROOM_HASH                  (hash_t) 0xFE24DC71DAB689A1        // fullroom
#define ENTRY__PATCH_HASH              (hash_t) 0x83B0C2D12740E1F7        // entry/patch

// ext/ext-math.c
#define FLOAT_HASH                     (hash_t) 0x695BFEEDD19BCA44        // float
#define F_DOTS_HASH                    (hash_t) 0x2F722F7F5ACF1A83        // f.s
#define F_DOT_HASH                     (hash_t) 0x4BE43CB346531494        // f.
#define FDROP_HASH                     (hash_t) 0x87E070AFFF04AE01        // fdrop
#define FDUP_HASH                      (hash_t) 0xE52CAED141C09D73        // fdup
#define FSWAP_HASH                     (hash_t) 0x77633FA8C8C8E9D4        // fswap
#define FOVER_HASH                     (hash_t) 0x97914062688BB17B        // fover
#define FROT_HASH                      (hash_t) 0xF825E20341C489A7        // frot
#define F_SUBROT_HASH                  (hash_t) 0x7905A83CAB85D6EA        // f-rot
#define FNIP_HASH                      (hash_t) 0x59A3265492A4C521        // fnip
#define F_ADD_HASH                     (hash_t) 0x4BE43CB28D377242        // f+
#define F_SUB_HASH                     (hash_t) 0x4BE43CB0E22625EF        // f-
#define F_MUL_HASH                     (hash_t) 0x4BE43CB2291823F3        // f*
#define F_DIV_HASH                     (hash_t) 0x4BE43CB33A8DC79D        // f/
#define F_TO__HASH                     (hash_t) 0x4BE43CBD05603B9B        // f>
#define F_FROM_HASH                    (hash_t) 0x4BE43CBACD03FEA9        // f<
#define F_LOAD_HASH                    (hash_t) 0x4BE43C926E94CD89        // f@
#define F_STORE_HASH                   (hash_t) 0x4BE43CB754503A19        // f!
#define F_INLINE_HASH                  (hash_t) 0xF92E7960B81DF42         // [f,]

// ext/io.c
#define GETCHAR_HASH                   (hash_t) 0xF1D35324C6C42EE6        // getchar
#define ECHO_ADDR_HASH                 (hash_t) 0x4234E926A4A54AD9        // echo
#define FILE_SUBSIZE_HASH              (hash_t) 0xB5AC4C5C183B0325        // file-size
#define FILE_LOAD_HASH                 (hash_t) 0x451BC0BF988679D3        // load
#define FILE_SAVE_HASH                 (hash_t) 0xD47CB184CD2E9CFF        // save
#define INCLUDED_HASH                  (hash_t) 0xE989F1C3CEDB076E        // included

// ext/load-ext.c
#define REQUIRE_IO_HASH                (hash_t) 0x7252A9377093B6AF        // require-io
#define REQUIRE_PARSER_HASH            (hash_t) 0xA609B2ED15E998AA        // require-parser
#define REQUIRE_OS_HASH                (hash_t) 0x5A0E3F5A2D78F6E3        // require-os
#define REQUIRE_DICT_HASH              (hash_t) 0x5B3D83354E74B4D9        // require-dict
#define REQUIRE_TIME_HASH              (hash_t) 0x98C8DAC2B56DAA60        // require-time
#define REQUIRE_MATH_HASH              (hash_t) 0x59BF9BB79623E37D        // require-math
#define REQUIRE_EXT_MATH_HASH          (hash_t) 0xE039FC2AA64F06F3        // require-ext-math
#define REQUIRE_MP_MATH_HASH           (hash_t) 0xDABE36DE9B263B7B        // require-mp-math
#define REQUIRE_THREADS_HASH           (hash_t) 0x1DF35B3B3EE18BBD        // require-threads
#define REQUIRE_NETWORK_HASH           (hash_t) 0x5165BFE0227DCD4C        // require-network

// ext/math.c
#define DIV_HASH                       (hash_t) 0x6878508B51D2587         // /
#define MOD_HASH                       (hash_t) 0xE99B5B3DFDE51800        // mod
#define DIVMOD_HASH                    (hash_t) 0x69FEFBE39F1DDB26        // /mod
#define MUL_DIV_HASH                   (hash_t) 0x988E9946714B59C6        // */
#define LSHIFT_HASH                    (hash_t) 0xB219454BCFBFC9F4        // lshift
#define RSHIFT_HASH                    (hash_t) 0xBD6BD2CF6B605380        // rshift
#define SUP_HASH                       (hash_t) 0x687850E9C22F4FD         // >
#define INF_EQUAL_HASH                 (hash_t) 0x61B45F27AC80BB42        // <=
#define SUP_EQUAL_HASH                 (hash_t) 0xF7694DF78796B076        // >=
#define INFSUP_HASH                    (hash_t) 0x61B45F27D772A2D0        // <>
#define OR_HASH                        (hash_t) 0x8E433BD16BB7290F        // or
#define AND_HASH                       (hash_t) 0xAB5AEB165103835         // and
#define MIN_HASH                       (hash_t) 0x3AA5005589608EB2        // min
#define MAX_HASH                       (hash_t) 0xD43766A3AD27B74A        // max
#define ABS_HASH                       (hash_t) 0xC513D6AA10E12577        // abs
#define RAND_HASH                      (hash_t) 0xF71F39784F3A1D16        // rand
#define SRAND_HASH                     (hash_t) 0xC206EB1D5CE916DA        // srand
#define RANDOM_HASH                    (hash_t) 0xC1ABCFCC5CBEE428        // random

// ext/mp-math.c
#define MPZ_HASH                       (hash_t) 0x7ECD3F3F40E31266        // mpz
#define M_DOTS_HASH                    (hash_t) 0xC4233FEED1AE0911        // m.s
#define M_DOT_HASH                     (hash_t) 0x83D1F7C12F0676E4        // m.
#define MDROP_HASH                     (hash_t) 0x8D1870885F98FB35        // mdrop
#define MDUP_HASH                      (hash_t) 0x8AE8CC07B1CA5804        // mdup
#define MSWAP_HASH                     (hash_t) 0x45AD255D10F748E0        // mswap
#define MOVER_HASH                     (hash_t) 0xB6DFD6EF40940E14        // mover
#define MROT_HASH                      (hash_t) 0xBFC43BF1286F2D3E        // mrot
#define M_SUBROT_HASH                  (hash_t) 0x2B10A3EC9C3EB84F        // m-rot
#define MNIP_HASH                      (hash_t) 0xF4D17408A5593E4D        // mnip
#define M_ADD_HASH                     (hash_t) 0x83D1F7FEE41050DE        // m+
#define M_SUB_HASH                     (hash_t) 0x83D1F7FE5CED5408        // m-
#define M_MUL_HASH                     (hash_t) 0x83D1F7FE803E8217        // m*
#define M_DIV_HASH                     (hash_t) 0x83D1F7C19335183D        // m/
#define M_TO__HASH                     (hash_t) 0x83D1F7FAF21F7549        // m>
#define M_FROM_HASH                    (hash_t) 0x83D1F7FBBBB1CAFB        // m<
#define M_EQUAL_HASH                   (hash_t) 0x83D1F7F81F806D3B        // m=
#define M_LOAD_HASH                    (hash_t) 0x83D1F798BBE73C4E        // m@
#define M_STORE_HASH                   (hash_t) 0x83D1F7FA8ECDE546        // m!
#define M_INLINE_HASH                  (hash_t) 0x42348764F0C5FB62        // [m,]

// ext/network.c
#define SERVER_START_HASH              (hash_t) 0xC670035AAE0D84AC        // server-start
#define CLIENT_START_HASH              (hash_t) 0xF4DD0A27D6A5A81C        // client-start
#define SERVER_STOP_HASH               (hash_t) 0xC003BCC4231718FE        // server-stop
#define SERVER_NONBLOCKING_HASH        (hash_t) 0xA5A3F315C41B0434        // server-nonblocking
#define SERVER_ACCEPT_HASH             (hash_t) 0x998CCD683DCD6193        // server-accept
#define SOCKET_SEND_HASH               (hash_t) 0xBC6280D5DCFE0B42        // socket-send
#define SOCKET_SEND_CHAR_HASH          (hash_t) 0x260610C7935D308A        // socket-send-char
#define SOCKET_RECV_HASH               (hash_t) 0xC0F1A79F78505ADD        // socket-recv

// ext/os.c
#define SYSTEM_HASH                    (hash_t) 0x43D9019460539EBE        // system
#define C_SUBALLOC_HASH                (hash_t) 0x591DB76581BC91BD        // c-alloc
#define C_SUBFREE_HASH                 (hash_t) 0x6BF67107EB0E101E        // c-free

// ext/parser.c
#define PARSE_HASH                     (hash_t) 0xF09F075E8A7FD0AE        // parse
#define CLEAR_TIB_HASH                 (hash_t) 0x6ED4167919FF52BE        // clear-tib

// ext/threads.c
#define THREAD__RUN_HASH               (hash_t) 0xD2A6FE9B14153C6E        // thread/run
#define THREAD__JOIN_SUBALL_HASH       (hash_t) 0xBA298EA8C02BA628        // thread/join-all
#define THREAD__JOIN_HASH              (hash_t) 0x3DD869A09AA81B15        // thread/join
#define THREAD__KILL_HASH              (hash_t) 0xDB90E52A2038D070        // thread/kill
#define THREAD__LOCK_HASH              (hash_t) 0x82AE2C9BD8EEE342        // thread/lock
#define THREAD__UNLOCK_HASH            (hash_t) 0x3FF045071DAF1C05        // thread/unlock

// ext/time.c
#define TIME_HASH                      (hash_t) 0x88FFAFDF90085710        // time
#define UTIME_HASH                     (hash_t) 0x6CCFAC3228BB0972        // utime
#define SLEEP_HASH                     (hash_t) 0x403E60E4AA433AB9        // sleep
#define MSSLEEP_HASH                   (hash_t) 0x85FA9DF00932C810        // mssleep

// Register A
#define REG_A_LOAD_HASH                (hash_t) 0x50ED353EB1844715        // A@
#define REG_A_STORE_HASH               (hash_t) 0x50ED34DC813A73BC        // A!
#define REG_A_ADD_STORE_HASH           (hash_t) 0x84DB02F9A39DD238        // A+!
#define REG_A_ADD_ADD_HASH             (hash_t) 0x84DB02E55D9B4E74        // A++
#define REG_A_SUB_SUB_HASH             (hash_t) 0x164E82CBA463673A        // A--
#define REG_A_TO_R_HASH                (hash_t) 0xCC40504953C8F67         // A>R
#define REG_R_TO_A_HASH                (hash_t) 0xE35C44FDEEB26256        // R>A

// Register B
#define REG_B_LOAD_HASH                (hash_t) 0xB1BFFDE1549EB62F        // B@
#define REG_B_STORE_HASH               (hash_t) 0xB1BFFDC341F0EAB6        // B!
#define REG_B_ADD_STORE_HASH           (hash_t) 0xC201430060B481E4        // B+!
#define REG_B_ADD_ADD_HASH             (hash_t) 0xC20143059BAAFD4D        // B++
#define REG_B_SUB_SUB_HASH             (hash_t) 0xA114D7558CE9C401        // B--
#define REG_B_TO_R_HASH                (hash_t) 0x61A629858CCC4804        // B>R
#define REG_R_TO_B_HASH                (hash_t) 0xE35C44FC429F139D        // R>B

// Register C
#define REG_C_LOAD_HASH                (hash_t) 0xD5B1DE05A39D8968        // C@
#define REG_C_STORE_HASH               (hash_t) 0xD5B1DE11F5840937        // C!
#define REG_C_ADD_STORE_HASH           (hash_t) 0x4F12E7FA06E7DAB2        // C+!
#define REG_C_ADD_ADD_HASH             (hash_t) 0x4F12E7FFAC91AE7A        // C++
#define REG_C_SUB_SUB_HASH             (hash_t) 0xE4B27C1E0EEB5D8F        // C--
#define REG_C_TO_R_HASH                (hash_t) 0xCED481FA49A26E9F        // C>R
#define REG_R_TO_C_HASH                (hash_t) 0xE35C44FCA56CCA94        // R>C

// Register I
#define REG_I_LOAD_HASH                (hash_t) 0xB7BB0EDCCF913F4C        // I@
#define REG_I_STORE_HASH               (hash_t) 0xB7BB0EBE14DF06D6        // I!
#define REG_I_ADD_STORE_HASH           (hash_t) 0xB29C448FD82768E4        // I+!
#define REG_I_ADD_ADD_HASH             (hash_t) 0xB29C448937060C3F        // I++
#define REG_I_SUB_SUB_HASH             (hash_t) 0xCC356BA6FED29A6F        // I--
#define REG_I_TO_R_HASH                (hash_t) 0x5B3121E7C3A1F265        // I>R
#define REG_R_TO_I_HASH                (hash_t) 0xE35C44F2CC5C934F        // R>I

// Register J
#define REG_J_LOAD_HASH                (hash_t) 0xD02E72BC2E8A9FAA        // J@
#define REG_J_STORE_HASH               (hash_t) 0xD02E72B10C65814D        // J!
#define REG_J_ADD_STORE_HASH           (hash_t) 0xC4957CAE14974FD8        // J+!
#define REG_J_ADD_ADD_HASH             (hash_t) 0xC4957CA27EDCFB2C        // J++
#define REG_J_SUB_SUB_HASH             (hash_t) 0xC9607CD83D5D8992        // J--
#define REG_J_TO_R_HASH                (hash_t) 0x1CCF898FF814662F        // J>R
#define REG_R_TO_J_HASH                (hash_t) 0xE35C44F12034A527        // R>J
// <-- END AUTOGEN PART -->

#endif /* __HASH_DEF_64 */
