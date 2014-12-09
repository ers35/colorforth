(one (line (comment
:\ ^key ^10 ^= ^if ^; ^\ ^;
~\ one line comment

:cr ^10 ^emit ^;
:.s ^. ^. ^. ^. ^. ^. ^. ^. ^cr ^;

:dbg ^.s ^bye ^;

:alloc ^here ^@ ^swap ^over ^+ ^here ^! ^;

(string
~\ stored in memory as as length, characters...
:"init ^here ^@ ^cell ^+ ^;
:"loop ^dup ^key ^dup ^34 ^= ^if ^; ^swap ^! ^cell ^+ ^"loop ^;
:"done ^drop ^drop ^here ^@ ^swap ^- ^dup ^alloc ^swap ^over ^! ^;
:" ^"init ^"loop ^"done ^;

(calculate (end (pointer
~\ :."init ^swap ^over ^+ ^swap ^;
:."init ^dup ^dup ^@ ^+ ^swap ^cell ^+ ^;
:."loop ^over ^over ^= ^if ^; ^dup ^@ ^emit ^cell ^+ ^."loop ^;
:." ^."init ^."loop ^cr ^;

:square ^dup ^* ^;
~\ calculated at compile time
:25square ~25 ~square ^;
~25square ~.
:msg ~" hello world" ^;
~msg ~." ~bye
