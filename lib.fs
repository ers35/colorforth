(one (line (comment
:\ ^key ^10 ^= ^if ^; ^\ ^;
~\ one line comment

:cr ^10 ^emit ^;
:.s ^. ^. ^. ^. ^. ^. ^. ^. ^cr ^;

:alloc ^here ^@ ^swap ^over ^+ ^here ^! ^;

(create (null-terminated (string
~\ stored in memory as as length, characters...
:"init ^here ^@ ^cell ^+ ^;
:"loop ^dup ^key ^dup ~key " ^= ^if ^; ^swap ^c! ^1 ^+ ^"loop ^;
:"done ^drop ^drop ^here ^@ ^swap ^- ^dup (null ^1 ^+ ^alloc ^swap ^over ^! ^;
:" ^"init ^"loop ^"done ^;

(print (string
:."init ^dup ^dup ^@ ^+ ^swap ^cell ^+ ^;
:."loop ^over ^over ^= ^if ^; ^dup ^c@ ^emit ^1 ^+ ^."loop ^;
:." ^."init ^."loop ^cr ^;

:msg ~" hello world" ^;
~msg ~."

:cmd ^cell ^+ ^system ^;
:uname ~" uname -a" ^cmd ^;
~uname

:square ^dup ^* ^;
~\ calculated at compile time
:25square ~25 ~square ^;
~25square ~. ~cr
