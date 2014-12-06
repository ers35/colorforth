(one (line (comment
:\ ^key ^10 ^= ^if ^; ^\ ^;
\ one line comment

:cr ^10 ^emit ^;
:.s ^. ^. ^. ^. ^. ^. ^. ^. ^cr ^;

:alloc ^here ^@ ^swap ^over ^+ ^here ^! ^;

:dbg ^.s ^bye ^;

(string
:"init ^here ^@ ^;
:"loop ^dup ^key ^dup ^34 ^= ^if ^; ^swap ^! ^cell ^+ ^"loop ^;
:"done ^drop ^drop ^here ^@ ^swap ^- ^dup ^alloc ^;
:" ^"init ^"loop ^"done ^;

\ :." ^swap ^dup ^0 ^= ^if ^; ^swap ^dup ^@ ^emit ^cell ^swap ^+ ^swap ^cell ^swap ^- ^swap ^." ^;
(calculate (end (pointer
:."init ^swap ^over ^+ ^swap ^;
:."loop ^over ^over ^= ^if ^; ^dup ^@ ^emit ^cell ^+ ^."loop ^;
:." ^."init ^."loop ^cr ^;

" hello world" ."
